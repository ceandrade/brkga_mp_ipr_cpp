// A template class that enables writing and reading enum class
// members as strings.
//
// \author  Bradley Plohr (2017-05-12)
//
// **Note:**  The idea to keep the enum names as a static member in a
// template comes from Loki Astari:
//
// https://codereview.stackexchange.com/questions/14309
//         /conversion-between-enum-and-string-in-c-class-header
//
// Usage example:
//
// Enums.h:
// -------
// #ifndef ENUMS_H_
// #define ENUMS_H_
//
// enum class Family { SLOW, FAST };
//
// TODO:  other enum classes
//
// #endif /* ENUMS_H_ */
//
//
// Enums.cc:
// --------
// #include "Enums.h"
// #include "EnumIO.h"
// #include <string>
// #include <vector>
//
// template <>
// const std::vector<std::string>& EnumIO<Family>::enum_names()
// {
//      static std::vector<std::string> enum_names_({ "SLOW", "FAST" });
//      return enum_names_;
// }
//
// TODO:  enum names for other enum classes
//
//
// t_EnumIO.cc:
// -----------
// #include "EnumIO.h"
// #include "Enums.h"
// #include <iostream>
//
// int
// main()
// {
//     Family family;
//
//     family = Family::SLOW;
//     std::cout << family << std::endl;
//
//     std::cin >> family;
//     std::cout << family << std::endl;
//
//     return 0;
// }
//
// For the input
//
//     fAsT
//
// the output is
//
//     SLOW
//     FAST

#ifndef ENUM_IO_HPP_
#define ENUM_IO_HPP_

#include <algorithm>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

template<typename T>
class EnumIO
{
public:
    static const std::vector<std::string>& enum_names();
};

static inline std::string toUpper(const std::string& input)
{
    std::string copy(input);
    std::transform(copy.cbegin(), copy.cend(), copy.begin(),
            [](const unsigned char& i) { return std::toupper(i); });

    return copy;
}

template <typename T,
          typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
T toEnum(const std::string& s) {
    auto input = toUpper(s);

    // check for a match with a name
    int i = 0;
    for (auto name : EnumIO<T>::enum_names()) {
        if (toUpper(name) == input) {
            // Here we assume that the integer representation of
            // the enum class is the default.  If the enum class
            // members are assigned other integers, this code
            // must be extended by consulting a vector containing
            // the assigned integers.
            return static_cast<T>(i);
        }
        ++i;
    }

    // check for a match with an integer
    int n = static_cast<int>(EnumIO<T>::enum_names().size());
    std::istringstream iss(input);
    int value;
    iss >> value;
    if (not iss.fail() && 0 <= value && value < n) {
        return static_cast<T>(value); // See the comment above.
    }

    throw std::range_error("Invalid enum item");
}

template<typename T,
         typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
std::ostream&
operator<<(std::ostream& os, const T& t)
{
    os << EnumIO<T>::enum_names().at(static_cast<int>(t));

    return os;
}

template<typename T,
         typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
std::istream&
operator>>(std::istream& is, T& t)
{
    std::string input;
    is >> input;
    if (is.fail())
        return is;

    try {
        t = toEnum<T>(input);
    }
    catch(std::range_error& e) {
        is.setstate(std::ios::failbit);
    }

    return is;
}

#endif /* ENUM_IO_HPP_ */
