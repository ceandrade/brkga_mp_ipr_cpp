/*******************************************************************************
 * brkga_mp_ipr.hpp: Biased Random-Key Genetic Algorithm Multi-Parent
 *                   with Implict Path Relinking.
 *
 * (c) Copyright 2015-2025, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * Created on : Jan 06, 2015 by ceandrade.
 * Last update: Mar 28, 2025 by ceandrade.
 *
 * This code is released under BRKGA-MP-IPR License:
 * https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
 *
 * (c) Copyright 2010, 2011 Rodrigo F. Toso, Mauricio G.C. Resende.
 * All Rights Reserved.
 *
 * This code is released under MIT License:
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * - The above copyright notice and this permission notice shall be included
 *   in all copies or substantial portions of the Software.
 *
 * Collaborators:
 * - Alberto Kummer, 2021 (parallel mating).
 * - Daniele Ferone, 2023 (bug fix on IPR).
 * - Pedro H.D.B Hokama, 2025 (bug fix on evolution).
 * - Mário C. San Felice, 2025 (bug fix on evolution).
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#ifndef BRKGA_MP_IPR_HPP_
#define BRKGA_MP_IPR_HPP_

#include "fitness_type.hpp"
#include "chromosome.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <omp.h>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdexcept>
#include <sys/time.h>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// This includes helpers to read and write enums.
//
// NOTE: Due to some weird scope resolution within clang compilers,
// we must imbue the enum facilities into namespace BRKGA.
// This does not happen with g++. We have not test to MSVC.
namespace BRKGA {
#include "third_part/enum_io.hpp"
}

//----------------------------------------------------------------------------//

/**
 * \defgroup compiler_directives Compiler directives
 *
 * Changes the behavior of the code according to the user supplied directives.
 * Usually, pass by flag `-D` (in GCC and Clang). For instance,
 *
 *   $ gcc -DBRKGA_MULTIPLE_INCLUSIONS code.cpp -o code.o
 */
///@{

/**
 * \brief Allows inclusion within multiple translation units.
 *
 * If we need to include this file in multiple translation units (files) that
 * are compiled separately, we have to `inline` some functions and template
 * definitions to avoid multiple definitions and linking problems. However,
 * such inlining can make the object code grows large. In other cases, the
 * compiler may complain about too many inline functions, if you are already
 * using several inline functions.
 */
#ifdef DOXYGEN_DOC_MACROS
    #define BRKGA_MULTIPLE_INCLUSIONS
#endif

#ifdef BRKGA_MULTIPLE_INCLUSIONS
    /// \cond DOXYGEN_IGNORE_THIS
    #define INLINE inline
    /// \endcond
#else
    /// \cond DOXYGEN_IGNORE_THIS
    #define INLINE
    /// \endcond
#endif

//----------------------------------------------------------------------------//

/**
 * \name Mating control directives
 * These preprocessor flags determine how the mating process will happen
 * regarding reproducibility. One of the following options should be used.
 * If more than one is given, MATING_FULL_SPEED takes priority over
 * MATING_SEED_ONLY, which takes priority over MATING_SEQUENTIAL.
 * If no option is supplied, BRKGA-MP-IPR assume MATING_FULL_SPEED.
 */
///@{

/**
 * \brief Enables full speed parallel mating.
 *
 * At full speed (MATING_FULL_SPEED), the mating process is done in parallel,
 * using independent RNGs. The results are reproducible if and only if you use
 * the same seed and the same number of threads.
 */
#ifdef DOXYGEN_DOC_MACROS
    #define MATING_FULL_SPEED
#endif

/**
 * \brief Enables Seed-only parallel mating.
 *
 * Using MATING_SEED_ONLY, the mating is still parallel, but each RNG is
 * seeded on each mating. This is a little bit slower than full speed, but we
 * depend only on the seed, regardless of the number of threads. I.e., Runs
 * with a different number of threads, but the same seed should result in the
 * same sequence of decisions.
 */
#ifdef DOXYGEN_DOC_MACROS
    #define MATING_SEED_ONLY
#endif

/**
 * \brief Enables sequential mating.
 *
 * Using MATING_SEQUENTIAL, the mating process is completely sequential,
 * as in the original BRKGA. The reproducibility is guaranteed with only
 * the same seed. This option can be very slow for large chromosomes and
 * large populations. But it makes debugging easier.
 */
#ifdef DOXYGEN_DOC_MACROS
    #define MATING_SEQUENTIAL
#endif

#if defined(MATING_FULL_SPEED)
    #undef MATING_SEED_ONLY
    #undef MATING_SEQUENTIAL
#elif defined(MATING_SEED_ONLY)
    #undef MATING_FULL_SPEED
    #undef MATING_SEQUENTIAL
#elif defined(MATING_SEQUENTIAL)
    #undef MATING_FULL_SPEED
    #undef MATING_SEED_ONLY
#else
    /// \cond DOXYGEN_IGNORE_THIS
    #define MATING_FULL_SPEED
    /// \endcond
#endif
///@} mating_directives
///@} compiler_directives

//----------------------------------------------------------------------------//
// Namespace BRKGA: the main namespace
//----------------------------------------------------------------------------//

/**
 * \brief This namespace contains all facilities related to BRKGA Multi Parent
 * with Implicit Path Relinking.
 */
namespace BRKGA {}

/// Holds the enumerations for Path Relinking algorithms.
namespace BRKGA::PathRelinking {}

//----------------------------------------------------------------------------//
// Streaming operators
//----------------------------------------------------------------------------//

/**
 * \defgroup chrono_helpers I/O chrono help functions
 *
 * Defines some helpers for chrono types.
 *
 * \todo These functions may not be necessary for C++23,
 *       since streaming operators for chrono types are defined there.
 */
///@{
#if __cplusplus < 202300L

// For GCC, we must inject these direct into BRKGA namespace.
#ifndef __clang__
/// \cond DOXYGEN_IGNORE_THIS
namespace BRKGA {
/// \endcond
#endif

/// Helper concept that only accept std::duration types.
template<typename T>
concept DurationType =
    std::is_convertible_v <
        T,
        std::chrono::duration<typename T::rep, typename T::period>
    >;

/**
 * \brief Input stream operator for std::chrono durations.
 *
 * Until C++20, we have no operator>> for reading std::chrono.
 * So, the following definition is only for reading seconds.
 * We first read the integer representation of a second,
 * then convert it to the duration.
 *
 * \tparam durantion_t a chrono::duration type.
 * \param is the input stream.
 * \param duration the duration
 * \return the input stream.
 */
template<DurationType durantion_t>
INLINE std::istream&
operator>>(std::istream& is, durantion_t& duration) {
    typename durantion_t::rep value;
    is >> value;
    duration = durantion_t {value};
    return is;
}

#if defined(__clang__) && !defined(_LIBCPP_HAS_NO_LOCALIZATION)
/**
 * \brief Output stream operator for chrono durations (and seconds) for
 *        Clang/libc.
 *
 * To the date, libc (clang) has no support to stream chrono objects.
 * This is a working around only for seconds.
 *
 * \todo Remove this when clang/libc get the support.
 *
 * \param os the output operator.
 * \param dur the duration.
 * \return the output operator.
 */
INLINE std::ostream&
operator<<(std::ostream& os, const std::chrono::duration<double>& dur) {
    os << dur.count() << "s";
    return os;
}
#endif // __clang__

#ifndef __clang__
} // end of namespace BRKGA for GCC
#endif

#endif // __cplusplus
///@} chrono_helpers

//----------------------------------------------------------------------------//
// Forward declarations
//----------------------------------------------------------------------------//

namespace BRKGA {

// Captures the external operators for clang.
// TODO: Remove this when C++23 comes up.
#if defined(__clang__)
    using ::operator<<;
    using ::operator>>;
#endif

class Population;

//----------------------------------------------------------------------------//
// Utility functions
//----------------------------------------------------------------------------//

/**
 * \defgroup utility_functions Functions for equality comparisons
 *
 * These are helper functions that, at compiler time, detect if the `fitness_t`
 * is a floating point type, and applies the absolute diference. Otherwise,
 * the compiler generates the equality comparison.
 */
///@{

/**
 * \brief Compares two values to equality.
 *
 * If these values are real numbers, we compare their absolute difference with
 * `EQUALITY_THRESHOLD`, i.e., \f$|a - b| < EQUALITY\_THRESHOLD\f$. In other
 * cases (except tuples, which have a specialization), we use the `operator==`
 * directly. Therefore, `fitness_t` must define `operator==`.
 *
 * \param a First item to be compared.
 * \param b Second item to be compared.
 * \return true if a and b are equal.
 */
template <class T>
constexpr bool close_enough(T a, T b) {
   if constexpr (std::is_floating_point_v<T>)
      return fabs(a - b) < EQUALITY_THRESHOLD;
   else
      return a == b;
}

/**
 * \brief Compares two tuples to equality.
 *
 * This specialization iterates, recursively, of each tuples' members and
 * compares them. Note that this is done in compilation time, with no impact at
 * running time.
 *
 * \param a First tuple to be compared.
 * \param b Second tuple to be compared.
 * \return true if a and b are equal.
 */
template <size_t I = 0, typename T, typename... Ts>
constexpr bool close_enough(std::tuple<T, Ts...> a, std::tuple<T, Ts...> b)
{
    // If we have iterated through all elements, just return true.
    if constexpr(I == sizeof...(Ts) + 1)
        return true;
    else
        return close_enough(std::get<I>(a), std::get<I>(b)) &&
               close_enough<I + 1>(a, b);
}
///@} utility_functions

//----------------------------------------------------------------------------//
// Enumerations
//----------------------------------------------------------------------------//

/// Specifies objective as minimization or maximization.
enum class Sense {
    MINIMIZE = false,  ///< Minimization.
    MAXIMIZE = true    ///< Maximization.
};

/**
 * \brief Specifies a bias function type when choosing parents to mating
 * (`r` is a given parameter).
 *
 * This function substitutes the `rhoe` parameter from the original BRKGA.
 */
enum class BiasFunctionType {
    // 1 / num. parents for mating
    /** \f$\frac{1}{\text{num. parents for mating}}\f$
     * (all individuals have the same probability).
     */
    CONSTANT,

    // r^-3
    /// \f$r^{-3}\f$
    CUBIC,

    // e^-r
    /// \f$e^{-r}\f$
    EXPONENTIAL,

    // 1/r
    /// \f$1/r\f$
    LINEAR,

    // 1 / log(r + 1)
    /// \f$\frac{1}{\log(r + 1)}\f$ (usually works better than other functions).
    LOGINVERSE,

    // r^-2
    /// \f$r^{-2}\f$
    QUADRATIC,

    /// Indicates a custom bias function supplied by the user.
    CUSTOM
};

/// Specifies the type of shaking to be performed.
enum class ShakingType {
    /**
     * Applies the following perturbations:
     *  1. Inverts the value of a random chosen, i.e., from `value` to
     *     `1 - value`;
     *  2. Assigns a random value to a random key.
     */
    CHANGE,

    /**
     * Applies two swap perturbations:
     *  1. Swaps the values of a randomly chosen key `i` and its
     *     neighbor `i + 1`;
     *  2. Swaps values of two randomly chosen keys.
     */
    SWAP,

    /// Indicates a custom shaking procedure supplied by the user.
    CUSTOM
};

// Holds the enumerations for Path Relinking algorithms.
namespace PathRelinking {

/// Specifies type of path relinking.
enum class Type {
    /// Changes each key for the correspondent in the other chromosome.
    DIRECT,

    /// Switches the order of a key for that in the other chromosome.
    PERMUTATION
};

/// Specifies which individuals used to build the path.
enum class Selection {
    /// Selects, in the order, the best solution of each population.
    BESTSOLUTION,

    /// Chooses uniformly random solutions from the elite sets.
    RANDOMELITE
};

/// Specifies the distance function between two chromosomes during IPR.
enum class DistanceFunctionType {
    /// Uses the default Hamming distance calculator.
    HAMMING,

    /// Uses the default KendallTau distance calculator.
    KENDALLTAU,

    /// Indicates a custom function supplied by the user.
    CUSTOM
};

/// Specifies the result type/status of path relink procedure.
enum class PathRelinkingResult {
    /**
     * The chromosomes among the populations are too homogeneous and the path
     * relink will not generate improveded solutions.
     */
    TOO_HOMOGENEOUS = 0,

    /// Path relink was done but no improveded solution was found.
    NO_IMPROVEMENT = 1,

    /**
     * An improved solution among the elite set was found, but the best
     * solution was not improved.
     */
    ELITE_IMPROVEMENT = 3,

    /// The best solution was improved.
    BEST_IMPROVEMENT = 7
};

/**
 * \brief Performs bitwise `OR` between two `PathRelinkingResult` returning
 *        the highest rank `PathRelinkingResult`.
 *
 *  For example
 *  - TOO_HOMOGENEOUS | NO_IMPROVEMENT == NO_IMPROVEMENT
 *  - NO_IMPROVEMENT | ELITE_IMPROVEMENT == ELITE_IMPROVEMENT
 *  - ELITE_IMPROVEMENT | BEST_IMPROVEMENT == BEST_IMPROVEMENT
 *
 * \param lhs the left-hand side status (lvalue).
 * \param rhs the right-hand side status (rvalue).
 * \return a PathRelinkingResult according to the table above.
 */
inline PathRelinkingResult& operator|=(PathRelinkingResult& lhs,
                                       PathRelinkingResult rhs) {
    lhs = PathRelinkingResult(static_cast<unsigned>(lhs) |
                              static_cast<unsigned>(rhs));
    return lhs;
}
} // namespace PathRelinking

//----------------------------------------------------------------------------//
// I/O helpers
//----------------------------------------------------------------------------//

//----------------------------------------//
// Template specializations for enum I/O
//----------------------------------------//

/**
 * \defgroup enum_io_templates Template specializations for enum I/O
 *
 * Using slightly modified template class provided by Bradley Plohr
 * (https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header)
 * we specialize that template to enums in the BRKGA namespace.
 *
 * The EnumIO class helps to read and write enums from streams directly,
 * saving time in coding custom solutions. Please, see third_part/enum_io.hpp
 * for complete reference and examples.
 *
 * \note Due to some weird scope resolution within clang compilers,
 *     we must imbue the enum facilities into namespace BRKGA.
 *     This does not happen with g++. We have not test to MSVC.
 *
 * \warning The specialization must be inline-d to avoid multiple definitions
 *      issues across different modules. However, this can cause "inline"
 *      overflow, and compromise your code. If you include this header only
 *      once along with your code, it is safe to remove the `inline`s from the
 *      specializations. But, if this is not the case, you should move these
 *      specializations to a module you know is included only once, for
 *      instance, the `main()` module.
 */
///@{

/// Template specialization to BRKGA::Sense.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::Sense>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "MINIMIZE",
        "MAXIMIZE"
    };
    return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Type.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Type>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "DIRECT",
        "PERMUTATION"
    };
    return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Selection.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Selection>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "BESTSOLUTION",
        "RANDOMELITE"
    };
    return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::DistanceFunctionType.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::DistanceFunctionType>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "HAMMING",
        "KENDALLTAU",
        "CUSTOM"
    };
    return enum_names_;
}

/// Template specialization to BRKGA::BiasFunctionType.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::BiasFunctionType>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "CONSTANT",
        "CUBIC",
        "EXPONENTIAL",
        "LINEAR",
        "LOGINVERSE",
        "QUADRATIC",
        "CUSTOM"
    };
    return enum_names_;
}

/// Template specialization to BRKGA::ShakingType.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::ShakingType>::enum_names() {
    static std::vector<std::string> enum_names_ {
        "CHANGE",
        "SWAP",
        "CUSTOM"
    };
    return enum_names_;
}
///@} enum_io_templates

//----------------------------------------------------------------------------//
// Distance functions
//----------------------------------------------------------------------------//

/**
 * \defgroup distance_functions Distance functions
 *
 * This is a collection of some basic distance functions/functors between
 * two vectors/chromosomes. Notably used during the implicity path-reliking.
 */
///@{

/**
 * \brief Distance Function Base.
 *
 * This class is a interface for functors that compute the distance between
 * two chromosomes.
 */
class DistanceFunctionBase {
public:
    /// Default constructor.
    DistanceFunctionBase() = default;

    /// Default destructor.
    virtual ~DistanceFunctionBase() = default;

    /**
     * \brief Computes the distance between two vectors.
     * \param v1 first chromosome.
     * \param v2 second chromosome.
     */
    virtual double distance(const Chromosome& v1,
                            const Chromosome& v2) = 0;

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key.
     * \param key2 the second key.
     */
    virtual bool affectSolution(const Chromosome::value_type key1,
                                const Chromosome::value_type key2) = 0;

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by the
     *        blocks of keys `v2` affects the solution.
     * \param v1_begin begin of the first blocks of keys.
     * \param v2_begin begin of the first blocks of keys.
     * \param block_size number of keys to be considered.
     */
    virtual bool affectSolution(
            Chromosome::const_iterator v1_begin,
            Chromosome::const_iterator v2_begin,
            const std::size_t block_size) = 0;
};

//----------------------------------------------------------------------------//

/**
 * \brief Hamming distance between two vectors.
 *
 * This class is a functor that computes the Hamming distance between two
 * vectors. It takes a threshold parameter to "binarize" the vectors.
 */
class HammingDistance: public DistanceFunctionBase {
public:
    /**
     * \brief Default constructor
     * \param _threshold used to rounding the values to 0 or 1.
     */
    explicit HammingDistance(const double _threshold = 0.5):
        threshold(_threshold) {}

    /// Default destructor
    virtual ~HammingDistance() {}

    /**
     * \brief Computes the Hamming distance between two vectors.
     * \param vector1 first vector
     * \param vector2 second vector
     */
    virtual double distance(const Chromosome& vector1,
                            const Chromosome& vector2) override {
        if(vector1.size() != vector2.size())
            throw std::runtime_error("The size of the vector must "
                                     "be the same!");

        unsigned dist = 0;
        for(std::size_t i = 0; i < vector1.size(); ++i) {
            if((vector1[i] < threshold) != (vector2[i] < threshold))
                ++dist;
        }

        return double(dist);
    }

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key
     * \param key2 the second key
     */
    virtual bool affectSolution(const Chromosome::value_type key1,
                                const Chromosome::value_type key2) override {
        return (key1 < threshold ? 0 : 1) != (key2 < threshold ? 0 : 1);
    }

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by
     *        the blocks of keys `v2` affects the solution.
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
     */
    virtual bool affectSolution(Chromosome::const_iterator v1_begin,
                                Chromosome::const_iterator v2_begin,
                                const std::size_t block_size) override {
        for(std::size_t i = 0; i < block_size;
            ++i, ++v1_begin, ++v2_begin) {
            if((*v1_begin < threshold) != (*v2_begin < threshold))
                return true;
        }
        return false;
    }

public:
    /// Threshold parameter used to rounding the values to 0 or 1.
    double threshold {0.5};
};

//----------------------------------------------------------------------------//

/**
 * \brief Kendall Tau distance between two vectors.
 *
 * This class is a functor that computes the Kendall Tau distance between two
 * vectors. This version is not normalized.
 */
class KendallTauDistance: public DistanceFunctionBase {
public:
    /// Default constructor.
    KendallTauDistance() = default;

    /// Default destructor.
    virtual ~KendallTauDistance() = default;

    /**
     * \brief Computes the Kendall Tau distance between two vectors.
     * \param vector1 first vector
     * \param vector2 second vector
     */
    virtual double distance(const Chromosome& vector1,
                            const Chromosome& vector2) override {
        if(vector1.size() != vector2.size())
            throw std::runtime_error("The size of the vector must "
                                     "be the same!");

        const std::size_t size = vector1.size();

        std::vector<std::pair<double, std::size_t>> pairs_v1;
        std::vector<std::pair<double, std::size_t>> pairs_v2;

        pairs_v1.reserve(size);
        std::size_t rank = 0;
        for(const auto &v : vector1)
            pairs_v1.emplace_back(v, ++rank);

        pairs_v2.reserve(size);
        rank = 0;
        for(const auto &v : vector2)
            pairs_v2.emplace_back(v, ++rank);

        std::sort(begin(pairs_v1), end(pairs_v1));
        std::sort(begin(pairs_v2), end(pairs_v2));

        unsigned disagreements = 0;
        for(std::size_t i = 0; i < size - 1; ++i) {
            for(std::size_t j = i + 1; j < size; ++j) {
                if((pairs_v1[i].second < pairs_v1[j].second
                    && pairs_v2[i].second > pairs_v2[j].second) ||
                   (pairs_v1[i].second > pairs_v1[j].second
                    && pairs_v2[i].second < pairs_v2[j].second))
                    ++disagreements;
            }
        }

        return double(disagreements);
    }

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key
     * \param key2 the second key
     */
    virtual bool affectSolution(const Chromosome::value_type key1,
                                const Chromosome::value_type key2) override {
        return fabs(key1 - key2) > 1e-6;
    }

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by the
     *        blocks of keys `v2` affects the solution.
     *
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
     *
     * \todo This is the same implementation of Hamming. We need to figure out
     *       how to do it properly.
     */
    virtual bool affectSolution(Chromosome::const_iterator v1_begin,
                                Chromosome::const_iterator v2_begin,
                                const std::size_t block_size) override {
        return block_size == 1?
              affectSolution(*v1_begin, *v2_begin) : true;
    }
};
///@} distance_functions

//----------------------------------------------------------------------------//
// BRKGA Params class.
//----------------------------------------------------------------------------//

/**
 * \defgroup brkga_control_params BRKGA and Control Parameters
 */
///@{

/**
 * \brief Represents the BRKGA and IPR hyper-parameters.
 */
class BrkgaParams {
public:
    /** \name BRKGA Hyper-parameters */
    //@{
    /// Number of elements in the population (> 0).
    unsigned population_size {0};

    /// Percentage of individuals to become the elite set (0.0, 1.0].
    double elite_percentage {0.0};

    /// Percentage of mutants to be inserted in the population (0.0, 1.0].
    double mutants_percentage {0.0};

    /// Number of elite parents for mating (> 0).
    unsigned num_elite_parents {0};

    /// Number of total parents for mating (> 0).
    unsigned total_parents {0};

    /// Type of bias that will be used.
    BiasFunctionType bias_type {BiasFunctionType::CONSTANT};

    /// Number of independent parallel populations (> 0).
    unsigned num_independent_populations {0};
    //@}

    /** \name Path Relinking parameters */
    //@{
    /// Number of pairs of chromosomes to be tested to path relinking (> 0).
    unsigned pr_number_pairs {0};

    /** Mininum distance between chromosomes selected to path-relinking.
     * Value range depends on the used distance function.
     */
    double pr_minimum_distance {0.0};

    /// Path relinking type.
    PathRelinking::Type pr_type {PathRelinking::Type::DIRECT};

    /// Individual selection to path-relinking.
    PathRelinking::Selection pr_selection {PathRelinking::Selection::BESTSOLUTION};

    /// Type of the distance function used on path-relinking.
    PathRelinking::DistanceFunctionType pr_distance_function_type
        {PathRelinking::DistanceFunctionType::CUSTOM};

    /// The distance functor used on path-relinking.
    std::shared_ptr<DistanceFunctionBase> pr_distance_function {};

    /// Defines the block size based on the size of the population (> 0).
    double alpha_block_size {0.0};

    /// Percentage / path size to be computed (0.0, 1.0].
    double pr_percentage {0.0};
    //@}

    /** \name Population exchange parameters */
    //@{
    /// Number of elite chromosomes exchanged from each population (> 0).
    unsigned num_exchange_individuals {0};
    //@}

    /** \name Shaking parameters */
    //@{
    /// Type of the shaking procedure.
    ShakingType shaking_type {ShakingType::CHANGE};

    /** \brief Shaking intensity lower bound (0.0, 1.0].
     *
     * For default CHANGE and SWAP
     * procedures, this value is a percentage of the chromosome size.
     * If `shaking_intensity_lower_bound < shaking_intensity_upper_bound`,
     * we uniformly draw a random intensity between these bounds on each
     * shaking call.
     * If `shaking_intensity_lower_bound = shaking_intensity_upper_bound`,
     * then a fixed intensity is always used.
     * See \ref BrkgaParams.shaking_intensity_upper_bound.
     */
    double shaking_intensity_lower_bound {0.0};

    /** \brief Shaking intensity upper bound (0.0, 1.0].
     *
     * For default CHANGE and SWAP
     * procedures, this value is a percentage of the chromosome size.
     * If `shaking_intensity_lower_bound < shaking_intensity_upper_bound`,
     * we uniformly draw a random intensity between these bounds on each
     * shaking call.
     * If `shaking_intensity_lower_bound = shaking_intensity_upper_bound`,
     * then a fixed intensity is always used.
     * See \ref BrkgaParams.shaking_intensity_lower_bound.
     */
    double shaking_intensity_upper_bound {0.0};

    /** \brief This is the custom shaking procedure given by the user.
     *
     * - Parameters `lower_bound` and `upper_bound` is the shaking intensity
     *   bounds to be applied. Usually, the define a range where the intensity
     *   is sampled.
     *
     * - Parameter `populations` are the current BRKGA populations.
     *
     * - Parameter `shaken` is a list of `<population index, chromosome index>`
     *   pairs indicating which chromosomes were shaken on which population,
     *   so that they got re-decoded.
     *
     * See BRKGA_MP_IPR::setShakingMethod() for details and examples.
     *
     * \note If `shaken` is empty, all chromosomes of all populations are
     *      re-decoded. This may be slow. Even if you intention is to do so,
     *      it is faster to populate `shaken`.
     *
     * \warning This procedure can be very intrusive since it must manipulate
     *      the population. So, the user must make sure that BRKGA invariants
     *      are kept, such as chromosome size and population size.
     *      Otherwise, the overall functionaly may be compromised.
     */
    std::function<void(
        double lower_bound,
        double upper_bound,
        std::vector<std::shared_ptr<Population>>& populations,
        std::vector<std::pair<unsigned, unsigned>>& shaken
    )> custom_shaking {};
    //@}

    /// Default constructor.
    BrkgaParams() = default;

    /// Destructor.
    ~BrkgaParams() __attribute__((noinline)) = default;
};

//----------------------------------------------------------------------------//
// Control Parameters class.
//----------------------------------------------------------------------------//

/**
 * \brief Represents additional control parameters that can be used outside this
 * framework.
 *
 * These parameters are not used directly in the BRKGA nor in the path
 * relinking. However, they are loaded from the configuration file and can be
 * called by the user to perform out-loop controlling.
 */
class ControlParams {
public:
    /// Maximum running time.
    std::chrono::seconds maximum_running_time {0};

    /**
     * \brief Interval / number of interations without improvement in the best
     * solution at which elite chromosomes are exchanged (0 means no exchange).
     */
    unsigned exchange_interval {0};

    /**
     * \brief Interval / number of interations without improvement in the best
     * solution at which the Implicit Path Relink is called (0 means no IPR).
     */
    unsigned ipr_interval {0};

    /**
     * \brief Interval / number of interations without improvement in the best
     * solution at which the populations are shaken (0 means no shake).
     */
    unsigned shake_interval {0};

    /**
     * \brief Interval / number of interations without improvement in the best
     * solution at which the populations are reset (0 means no reset).
     */
    unsigned reset_interval {0};

    /**
     * \brief Defines the numbers iterations to stop when the best solution is
     * not improved, i.e., the algorithm converged (0 means don't stop by stall).
     */
    unsigned stall_offset {0};

    /// Default constructor.
    ControlParams() = default;

    /// Destructor.
    ~ControlParams() __attribute__((noinline)) = default;
};

//----------------------------------------------------------------------------//
// Loading the parameters from file
//----------------------------------------------------------------------------//

/**
 * \brief Reads the parameters from an input stream.
 *
 * \param input the input stream. It can be a file or a string stream.
 * \param logger a output stream to log some information such as missing
 *               no-required parameters. It does not log errors.
 * \returns a tuple containing the BRKGA and external control parameters.
 * \throws std::fstream::failure in case of errors in the file.
 *
 * \todo This method can beneficiate from introspection tools.
 *       We would like achieve a code similar to the
 *       [Julia counterpart](<https://github.com/ceandrade/brkga_mp_ipr_julia>).
 */
INLINE std::pair<BrkgaParams, ControlParams>
readConfiguration(std::istream& input, std::ostream& logger = std::cout) {

    // A simple struct to verify whether the parameter was correct loaded.
    class AuxParam {
    public:
        // Indicates if the param is required.
        bool required {false};

        // Indicates if the param was supplied.
        bool supplied {false};

        // This wil be a wrapper lambda function to set the parameter.
        std::function<void()> setter {};

        // Constructor.
        AuxParam(bool _required, std::function<void()> _setter):
            required(_required),
            supplied(false),
            setter(_setter)
            {}

        // Just call `setter()` and adjust the supplied flag.
        // Note that `setter()` will handle the errors.
        void set() {
            setter();
            supplied = true;
        }
    };

    // We have to declare here so that this variables are captured by
    // the following lambda functions.
    std::string token, data;
    unsigned line_count;

    // This is a setter function used to parse and test each parameter.
    auto set_param = [&](auto& param_value) {
        std::stringstream data_stream(data);
        if(!(data_stream >> param_value)) {
            std::stringstream error_msg;
            error_msg
            << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
            << "Invalid value for '" << token << "' on config line "
            << line_count << ": '" << data;
            throw std::fstream::failure(error_msg.str());
        }
    };

    // Now, we build the full parameter map.
    BrkgaParams brkga_params;
    ControlParams control_params;

    std::unordered_map<std::string, AuxParam> token_map {{
        // Required.
        {"population_size",
          AuxParam {true, [&]() { set_param(brkga_params.population_size); }} },
        {"elite_percentage",
         AuxParam {true, [&]() { set_param(brkga_params.elite_percentage); }} },
        {"mutants_percentage",
          AuxParam {true, [&]() { set_param(brkga_params.mutants_percentage); }} },
        {"population_size",
         AuxParam {true, [&] { set_param(brkga_params.population_size); }} },
        {"elite_percentage",
         AuxParam {true, [&] { set_param(brkga_params.elite_percentage); }} },
        {"mutants_percentage",
         AuxParam {true, [&] { set_param(brkga_params.mutants_percentage); }} },
        {"num_elite_parents",
         AuxParam {true, [&] { set_param(brkga_params.num_elite_parents); }} },
        {"total_parents",
         AuxParam {true, [&] { set_param(brkga_params.total_parents); }} },
        {"bias_type",
         AuxParam {true, [&] { set_param(brkga_params.bias_type); }} },
        {"num_independent_populations",
         AuxParam {true, [&] { set_param(brkga_params.num_independent_populations); }} },
        {"pr_number_pairs",
         AuxParam {true, [&] { set_param(brkga_params.pr_number_pairs); }} },
        {"pr_minimum_distance",
         AuxParam {true, [&] { set_param(brkga_params.pr_minimum_distance); }} },
        {"pr_type",
         AuxParam {true, [&] { set_param(brkga_params.pr_type); }} },
        {"pr_selection",
         AuxParam {true, [&] { set_param(brkga_params.pr_selection); }} },
        {"pr_distance_function_type",
         AuxParam {true, [&] { set_param(brkga_params.pr_distance_function_type); }} },
        {"alpha_block_size",
         AuxParam {true, [&] { set_param(brkga_params.alpha_block_size); }} },
        {"pr_percentage",
         AuxParam {true, [&] { set_param(brkga_params.pr_percentage); }} },
        // Optional.
        {"num_exchange_individuals",
         AuxParam {false, [&] { set_param(brkga_params.num_exchange_individuals); }} },
        {"shaking_type",
         AuxParam {false, [&] { set_param(brkga_params.shaking_type); }} },
        {"shaking_intensity_lower_bound",
         AuxParam {false, [&] { set_param(brkga_params.shaking_intensity_lower_bound); }} },
        {"shaking_intensity_upper_bound",
         AuxParam {false, [&] { set_param(brkga_params.shaking_intensity_upper_bound); }} },
        {"maximum_running_time",
         AuxParam {false, [&] { set_param(control_params.maximum_running_time); }} },
        {"exchange_interval",
         AuxParam {false, [&] { set_param(control_params.exchange_interval); }} },
        {"shake_interval",
         AuxParam {false, [&] { set_param(control_params.shake_interval); }} },
        {"ipr_interval",
         AuxParam {false, [&] { set_param(control_params.ipr_interval); }} },
        {"reset_interval",
         AuxParam {false, [&] { set_param(control_params.reset_interval); }} },
        {"stall_offset",
         AuxParam {false, [&] { set_param(control_params.stall_offset); }} }
    }};

    std::string line;
    line_count = 0;
    while(std::getline(input, line)) {
        ++line_count;
        std::string::size_type pos = line.find_first_not_of(" \t\n\v");

        // Ignore all comments and blank lines.
        if(pos == std::string::npos || line[pos] == '#')
            continue;

        std::stringstream line_stream(line);
        line_stream >> token >> data;

        std::transform(token.begin(), token.end(), token.begin(), tolower);

        if(auto it_token = token_map.find(token); it_token != token_map.end()) {
            if(it_token->second.supplied) {
                std::stringstream error_msg;
                error_msg
                << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                << "Duplicate attribute on config line " << line_count
                << ": '" << token << "' already read";
                throw std::fstream::failure(error_msg.str());
            }

            it_token->second.set();
        }
        else {
            std::stringstream error_msg;
            error_msg
            << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
            << "Invalid token on config line " << line_count
            << ": '" << token << "'";
            throw std::fstream::failure(error_msg.str());
        }
    }

    // Check requeriments.
    for(const auto& param : token_map) {
        if(!param.second.supplied) {
            if(param.second.required) {
                std::stringstream error_msg;
                error_msg
                << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                << "Parameter '" << param.first
                << "' is required but it was not supplied in the config file";
                throw std::fstream::failure(error_msg.str());
            }
            else {
                std::stringstream error_msg;
                error_msg
                << "WARNING: parameter '" << param.first
                << "' was not supplied in the config file. Using default value.";
                logger << error_msg.str() << "\n";
            }
        }
    }

    // Check distance function for IPR.
    switch(brkga_params.pr_distance_function_type) {
    case PathRelinking::DistanceFunctionType::HAMMING:
        brkga_params.pr_distance_function =
            std::shared_ptr<DistanceFunctionBase> {new HammingDistance};
        break;

    case PathRelinking::DistanceFunctionType::KENDALLTAU:
        brkga_params.pr_distance_function =
            std::shared_ptr<DistanceFunctionBase> {new KendallTauDistance};
        break;

    default:
        std::stringstream error_msg;
        error_msg
        << "WARNING: distance function set to '"
        << brkga_params.pr_distance_function_type << "'. "
        << "The user must supply his/her own distance functor if using IPR.";
        logger << error_msg.str() << "\n";
        break;
    }

    // Check shaking procedure.
    if(brkga_params.shaking_type == ShakingType::CUSTOM) {
        logger << "WARNING: shaking distance set to 'CUSTOM'. "
                   "The user must supply his/her own if using shaking.";
    }

    return std::make_pair(std::move(brkga_params), std::move(control_params));
}

//----------------------------------------------------------------------------//

/**
 * \brief Reads the parameters from a configuration file.
 *
 * \param filename the configuration file.
 * \param logger a output stream to log some information such as missing
 *               no-required parameters. It does not log errors.
 * \returns a tuple containing the BRKGA and external control parameters.
 * \throws std::fstream::failure in case of errors in the file.
 */
INLINE std::pair<BrkgaParams, ControlParams>
readConfiguration(const std::string& filename,
                  std::ostream& logger = std::cout) {

    std::ifstream input(filename, std::ios::in);
    if(!input) {
        std::stringstream error_msg;
        error_msg << "File '" << filename << "' cannot be opened!";
        throw std::fstream::failure(error_msg.str());
    }
    return readConfiguration(input, logger);
}

//----------------------------------------------------------------------------//
// Writing the parameters into file
//----------------------------------------------------------------------------//

/**
 * \brief Output stream operator for BrkgaParams.
 *
 * \param os the output stream.
 * \param brkga_params the parameters.
 * \return the output stream.
 */
INLINE std::ostream&
operator<<(std::ostream& os, const BrkgaParams& brkga_params) {
    os
    << "population_size " << brkga_params.population_size << "\n"
    << std::setiosflags(std::ios::fixed) << std::setprecision(2)
    << "elite_percentage " << brkga_params.elite_percentage << "\n"
    << "mutants_percentage " << brkga_params.mutants_percentage << "\n"
    << "num_elite_parents " << brkga_params.num_elite_parents << "\n"
    << "total_parents " << brkga_params.total_parents << "\n"
    << "bias_type " << brkga_params.bias_type << "\n"
    << "num_independent_populations "
    << brkga_params.num_independent_populations << "\n"
    << "pr_number_pairs " << brkga_params.pr_number_pairs << "\n"
    << "pr_minimum_distance " << brkga_params.pr_minimum_distance << "\n"
    << "pr_type " << brkga_params.pr_type << "\n"
    << "pr_selection " << brkga_params.pr_selection << "\n"
    << "pr_distance_function_type " << brkga_params.pr_distance_function_type << "\n"
    << "alpha_block_size " << brkga_params.alpha_block_size << "\n"
    << "pr_percentage " << brkga_params.pr_percentage << "\n"
    << "num_exchange_individuals "
    << brkga_params.num_exchange_individuals << "\n"
    << "shaking_type " << brkga_params.shaking_type << "\n"
    << "shaking_intensity_lower_bound " << brkga_params.shaking_intensity_lower_bound << "\n"
    << "shaking_intensity_upper_bound " << brkga_params.shaking_intensity_upper_bound << "\n";
    return os;
}

/**
 * \brief Output stream operator for ControlParams.
 *
 * \param os the output stream.
 * \param control_params the parameters.
 * \return the output stream.
 */
INLINE std::ostream&
operator<<(std::ostream& os, const ControlParams& control_params) {
    os
    << "maximum_running_time " << control_params.maximum_running_time << "\n"
    << "exchange_interval " << control_params.exchange_interval << "\n"
    << "shake_interval " << control_params.shake_interval << "\n"
    << "ipr_interval " << control_params.ipr_interval << "\n"
    << "reset_interval " << control_params.reset_interval << "\n"
    << "stall_offset " << control_params.stall_offset;
    return os;
}

/**
 * \brief Writes the parameters into a output stream.
 *
 * \note All floating point parameters are written with two point precision.
 *
 * \param output the output stream.
 * \param brkga_params the BRKGA parameters.
 * \param control_params the external control parameters. Default is an empty
 *        object.
 * \throws std::fstream::failure in case of errors in the file.
 *
 * \todo This method can beneficiate from introspection tools.
 *       We would like achieve a code similar to the
 *       [Julia counterpart](<https://github.com/ceandrade/brkga_mp_ipr_julia>).
 */
INLINE void writeConfiguration(std::ostream& output,
        const BrkgaParams& brkga_params,
        const ControlParams& control_params = ControlParams{}) {
    output << brkga_params << control_params << "\n";
}

//----------------------------------------------------------------------------//

/**
 * \brief Writes the parameters into a configuration file.
 *
 * \note All floating point parameters are written with two point precision.
 *
 * \param filename the configuration file.
 * \param brkga_params the BRKGA parameters.
 * \param control_params the external control parameters.
 *        Default is an empty object.
 * \throws std::fstream::failure in case of errors in the file.
 */
INLINE void writeConfiguration(const std::string& filename,
        const BrkgaParams& brkga_params,
        const ControlParams& control_params = ControlParams{}) {

    std::ofstream output(filename, std::ios::out);
    if(!output) {
        std::stringstream error_msg;
        error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                  << "File '" << filename << "' cannot be opened!";
        throw std::fstream::failure(error_msg.str());
    }

    writeConfiguration(output, brkga_params, control_params);
    output.close();
}
///@} brkga_control_params

//----------------------------------------------------------------------------//
// Algorithm status
//----------------------------------------------------------------------------//

/**
 * \defgroup algorithm_status Algorithm status utilities
 */
///@{

/**
 * \brief Defines the current status of the algorithm for a given
 * BRKGA_MP_IPR::run() call.
 *
 * \note We use `std::chrono::duration<double>` instead `std::chrono::seconds`
 *      for keep better precision.
 */
class AlgorithmStatus {
public:
    /** \name Best current solution */
    //@{
    /// Current best fitness value.
    fitness_t best_fitness {};

    /// A pointer to the current best chromosome.
    Chromosome best_chromosome {};
    //@}

    /** \name General iteration counters */
    //@{
    /// The current algorithm iteration / generation.
    unsigned current_iteration {0};

    /// The iteration of the last update.
    unsigned last_update_iteration {0};

    /// The time of the last update (in seconds).
    std::chrono::duration<double> current_time {0.0};

    /// The time of the last update (in seconds).
    std::chrono::duration<double> last_update_time {0.0};

    /// The largest number of iterations between two updates.
    unsigned largest_iteration_offset {0};

    /// Last number of iterations without improvement in the best solution.
    unsigned stalled_iterations {0};
    //@}

    /** \name Path relinking counters */
    //@{
    /// Total time spent on path relinking so far (in seconds).
    std::chrono::duration<double> path_relink_time {0.0};

    /// Number of call to path relinking.
    unsigned num_path_relink_calls {0};

    /** Number of homogenities, i.e., number of times that the populations are
     * too homogenious and the path reliking could not be performed.
     */
    unsigned num_homogenities {0};

    /// Number of improvements in the best solution by the path reliking.
    unsigned num_best_improvements {0};

    /** Number of improvements in elite individuals, but not the best one,
     * by path reliking.
     */
    unsigned num_elite_improvements {0};
    //@}

    /** \name Exchange, reset, and shake counters */
    //@{
    /** Number of exchange between populations performed
     * (not number of individuals).
     */
    unsigned num_exchanges {0};

    /// Number of shakes performed.
    unsigned num_shakes {0};

    /// Number of population resets performed.
    unsigned num_resets {0};
    //@}

    /// Default constructor.
    AlgorithmStatus() = default;

    /// Destructor.
    ~AlgorithmStatus() __attribute__((noinline)) = default;
};

//----------------------------------------------------------------------------//

/**
 * \brief Output streaming operator for AlgorithmStatus.
 *
 * \param output the output stream.
 * \param status the algorithm status.
 * \return the output stream.
 *
 * \todo Clang fails printing chrono times. When they fix it, please, revise
 *       this function.
 */
INLINE
std::ostream& operator<<(std::ostream& output, const AlgorithmStatus& status) {
    output
    << "\nbest_fitness: " << status.best_fitness
    << "\ncurrent_iteration: " << status.current_iteration
    << "\nlast_update_iteration: " << status.last_update_iteration
    << "\ncurrent_time: " << status.current_time
    << "\nlast_update_time: " << status.last_update_time
    << "\nlargest_iteration_offset: " << status.largest_iteration_offset
    << "\nstalled_iterations: " << status.stalled_iterations
    << "\npath_relink_time: " << status.path_relink_time
    << "\nnum_path_relink_calls: " << status.num_path_relink_calls
    << "\nnum_homogenities: " << status.num_homogenities
    << "\nnum_best_improvements: " << status.num_best_improvements
    << "\nnum_elite_improvements: " << status.num_elite_improvements
    << "\nnum_exchanges: " << status.num_exchanges
    << "\nnum_shakes: " << status.num_shakes
    << "\nnum_resets: " << status.num_resets;
    return output;
}
///@}algorithm_status

//----------------------------------------------------------------------------//
// Population class.
//----------------------------------------------------------------------------//

/**
 * \brief Encapsulates a population of chromosomes.
 *
 * Encapsulates a population of chromosomes providing supporting methods for
 * making the implementation easier.
 *
 * J
 *      All methods and attributes are public and can be manipulated directly
 *      from BRKGA algorithms. Note that this class is not meant to be used
 *      externally of this unit.
 */
class Population {
public:
    /** \name Data members */
    ///@{
    /// Chromosomes as vectors of probabilities.
    std::vector<Chromosome> chromosomes;

    /// Fitness of each chromosome.
    std::vector<std::pair<fitness_t, unsigned>> fitness;
    ///@}

    /** \name Default constructors and destructor */
    ///@{
    /**
     * \brief Default constructor.
     * \param chr_size size of chromosome.
     * \param pop_size size of population.
     * \throws std::range_error if population size or chromosome size is zero.
     */
    Population(const unsigned chr_size, const unsigned pop_size):
        chromosomes(pop_size, Chromosome(chr_size, 0.0)),
        fitness(pop_size)
    {
        if(pop_size == 0)
            throw std::range_error("Population size cannot be zero.");

        if(chr_size == 0)
            throw std::range_error("Chromosome size cannot be zero.");
    }

    /// Copy constructor.
    Population(const Population& other) = default;

    /// Destructor.
    ~Population() __attribute__((noinline)) = default;

    /// Assignment operator.
    Population& operator=(const Population&) = default;
    ///@}

    /** \name Simple access methods */
    ///@{
    /// Returns the size of each chromosome.
    unsigned getChromosomeSize() const {
        return chromosomes[0].size();
    }

    /// Returns the size of the population.
    unsigned getPopulationSize() const {
        return chromosomes.size();
    };

    /**
     * \brief Returns a copy of an allele for a given chromosome.
     * \param chromosome index of desired chromosome.
     * \param allele index of desired allele.
     * \returns a copy of the allele value.
     */
    double operator()(const unsigned chromosome, const unsigned allele) const {
        return chromosomes[chromosome][allele];
    }

    /**
     * \brief Returns a reference for an allele for a given chromosome.
     *
     * Usually used to set the allele value.
     * \param chromosome index of desired chromosome.
     * \param allele index of desired allele.
     * \returns a reference of the allele value.
     */
    double& operator()(const unsigned chromosome, const unsigned allele) {
        return chromosomes[chromosome][allele];
    }

    /**
     * \brief Returns a reference to a chromosome.
     * \param chromosome_index index of desired chromosome.
     * \returns a reference to chromosome.
     */
    Chromosome& operator()(const unsigned chromosome_index) {
        return chromosomes[chromosome_index];
    }
    ///@}

    /** \name Special access methods
     *
     * These methods REQUIRE fitness to be sorted, and thus a call to
     * `sortFitness()` beforehand.
     */
    ///@{
    /// Returns the best fitness in this population.
    fitness_t getBestFitness() const {
        return getFitness(0);
    }

    /// Returns the fitness of chromosome i.
    fitness_t getFitness(const unsigned i)  const {
        return fitness[i].first;
    }

    /// Returns a reference to the i-th best chromosome.
    Chromosome& getChromosome(const unsigned i) {
        return chromosomes[fitness[i].second];
    }

    /// Returns a const reference to the i-th best chromosome.
    const Chromosome& getChromosome(const unsigned i) const {
        return chromosomes[fitness[i].second];
    }
    ///@}

    /** \name Other methods */
    ///@{
    /**
     * \brief Sorts `fitness` by its first parameter according to the sense.
     * \param sense Optimization sense.
     */
    void sortFitness(const Sense sense) {
        if(sense == Sense::MAXIMIZE)
            std::sort(fitness.begin(), fitness.end(), std::greater<>());
        else
            std::sort(fitness.begin(), fitness.end(), std::less<>());
    }

    /**
     * \brief Sets the fitness of chromosome.
     * \param chromosome_index index of chromosome.
     * \param value allele value.
     */
    void setFitness(const unsigned chromosome_index, const fitness_t value) {
        fitness[chromosome_index] = std::make_pair(value, chromosome_index);
    }
    ///@}
};

//----------------------------------------------------------------------------//
// The Multi-Parent Biased Random-key Genetic Algorithm with Implicit
// Path Relinking
//----------------------------------------------------------------------------//

/**
 * \defgroup main_algorithms Main BRKGA-MP-IPR algorithms
 */
///@{
/**
 * \brief This class represents a Multi-Parent Biased Random-key Genetic
 * Algorithm with Implicit Path Relinking (BRKGA-MP-IPR).
 *
 * \author Carlos Eduardo de Andrade <ce.andrade@gmail.com>
 * \date 2023
 *
 * Main capabilities {#main_cap}
 * ========================
 *
 * Evolutionary process {#evol_process}
 * ------------------------
 *
 * In the BRKGA-MP-IPR, we keep a population of chromosomes divided between the
 * elite and the non-elite group. During the mating, multiple parents are chosen
 * from the elite group and the non-elite group. They are sorted either on
 * no-decreasing order for minimization or non-increasing order to maximization
 * problems. Given this order, a bias function is applied to the rank of each
 * chromosome, resulting in weight for each one. Using a roulette method based
 * on the weights, the chromosomes are combined using a biased crossover.
 *
 * This code also implements the island model, where multiple populations
 * can be evolved in parallel, and migration between individuals between
 * the islands are performed using #exchangeElite() method.
 *
 * This code requires the template argument `Decoder` be a class or functor
 * object capable to map a chromosome to a solution for the specific problem,
 * and return a value to be used as fitness to the decoded chromosome.
 * The decoder must have the method
 * \code{.cpp}
 *      double decode(Chromosome& chromosome, bool rewrite);
 * \endcode
 *
 * where #Chromosome is a `vector<double>` (in general) representing a solution
 * and `rewrite` is a boolean indicating that if the decode should rewrite the
 * chromosome in case it implements local searches and modifies the initial
 * solution decoded from the chromosome. Since this API has the capability of
 * decoding several chromosomes in parallel, the user must guarantee that
 * `Decoder::decode(...)` is thread-safe. Therefore, we do recommend to have
 * the writable variables per thread. Please, see the example that follows this
 * code.
 *
 * Implicit Path Relinking {#ipr}
 * ------------------------
 *
 * This API also implements the Implicit Path Relinking leveraging the decoder
 * capabilities. To perform the path relinking, the user must call #pathRelink()
 * method, indicating the type of path relinking (direct or permutation-based,
 * see #PathRelinking::Type), the selection criteria (best solution or random
 * elite, see #PathRelinking::Selection), the distance function
 * (to choose individuals far enough, see BRKGA::DistanceFunctionBase,
 * BRKGA::HammingDistance, and BRKGA::KendallTauDistance), a maximum time or
 * a maximum path size.
 *
 * In the presence of multiple populations, the path relinking is performed
 * between elite chromosomes from different populations, in a circular fashion.
 * For example, suppose we have 3 populations. The framework performs 3 path
 * relinkings: the first between individuals from populations 1 and 2, the
 * second between populations 2 and 3, and the third between populations 3
 * and 1. In the case of just one population, both base and guiding individuals
 * are sampled from the elite set of that population.
 *
 * Note that the algorithm tries to find a pair of base and guiding solutions
 * with a minimum distance given by the distance function. If this is not
 * possible, a new pair of solutions are sampled (without replacement) and
 * tested against the distance. In case it is not possible to find such pairs
 * for the given populations, the algorithm skips to the next pair of
 * populations (in a circular fashion, as described above). Yet, if such pairs
 * are not found in any case, the algorithm declares failure. This indicates
 * that the populations are very homogeneous.
 *
 * The API will call `Decoder::decode()` always
 * with `rewrite = false`. The reason is that if the decoder rewrites the
 * chromosome, the path between solutions is lost and inadvertent results may
 * come up. Note that at the end of the path relinking, the method calls the
 * decoder with `rewrite = true` in the best chromosome found to guarantee
 * that this chromosome is re-written to reflect the best solution found.
 *
 * Other capabilities {#other_cap}
 * ========================
 *
 * Multi-start {#multi_start}
 * ------------------------
 *
 * This API also can be used as a simple multi-start algorithm without
 * evolution. To do that, the user must supply in the constructor the argument
 * `evolutionary_mechanism_on = false`. This argument makes the elite set has
 * one individual and the number of mutants n - 1, where n is the size of the
 * population. This setup disables the evolutionary process completely.
 *
 * Initial Population {#init_pop}
 * ------------------------
 *
 * This API allows the user provides a set of initial solutions to warm start
 * the algorithm. In general, initial solutions are created using other (fast)
 * heuristics and help the convergence of the BRKGA. To do that, the user must
 * encode the solutions on #Chromosome (`vector<double>`) and pass to the method
 * `setInitialPopulation()` as a `vector<#Chromosome>`.
 *
 * General Usage {#gen_usage}
 * ========================
 *
 *  -# The user must call the BRKGA_MP_IPR constructor and pass the desired
 *     parameters. Please, see BRKGA_MP_IPR::BRKGA_MP_IPR for parameter details;
 *
 *  -# (Optional) The user provides the warm start solutions using
 *     `setInitialPopulation()`;
 *
 *  -# (Optional) The user provides a callback to track the optimization using
 *     `addNewSolutionObserver()`;
 *
 *  -# (Optional) The user provides custom stopping criteria function using
 *      `setStoppingCriteria()`;
 *
 *  -# (Optional) The user provides a custom shaking procedure using
 *     `setShakingMethod()`;
 *
 *  -# The user calls `run()` to cary out the optimization; This method returns
 *     a `AlgorithmStatus` object with the results of the optimization.
 *
 * For a comprehensive and detailed usage, please see the examples that follow
 * this API.
 *
 * About multi-threading {#multi_thread}
 * ========================
 *
 * This API is capable of decoding several chromosomes in
 * parallel, as mentioned before. This capability is based on OpenMP
 * (<http://www.openmp.org>) and the compiler must have support to it.
 * Most recent versions of GNU G++ and Intel C++ Compiler support OpenMP.
 * Clang supports OpenMP since 3.8. However, there are some issues with the
 * libraries, and sometimes, the parallel sections are not enabled. On the
 * major, the user can find fixes to his/her system.
 *
 * Since, in general, the decoding process can be complex and lengthy, it is
 * recommended that **the number of threads used DO NOT exceed the number of
 * physical cores in the machine.** This improves the overall performance
 * drastically, avoiding cache misses and racing conditions. Note that the
 * number of threads is also tied to the memory utilization, and it should be
 * monitored carefully.
 *
 * History {#hist}
 * ========================
 *
 * This API was based on the code by Rodrigo Franco Toso, Sep 15, 2011.
 * http://github.com/rfrancotoso/brkgaAPI
 *
 */
template <class Decoder>
class BRKGA_MP_IPR {
public:
    /** \name Constructors and destructor */
    ///@{
    /**
     * \brief Builds the algorithm and its data strtuctures with the given
     *        arguments.
     *
     * \param decoder_reference a reference to the decoder object.
     *        **NOTE:** BRKGA uses such object directly for decoding.
     * \param sense the optimization sense (maximization or minimization).
     * \param seed the seed for the random number generator.
     * \param chromosome_size number of genes in each chromosome.
     * \param params BRKGA and IPR parameters object loaded from a
     *        configuration file or manually created. All the data is copied.
     * \param max_threads number of threads to perform parallel decoding.\n
     *        **NOTE**: `Decoder::decode()` MUST be thread-safe.
     * \param evolutionary_mechanism_on if false, no evolution is performed
     *        but only chromosome decoding. Very useful to emulate a
     *        multi-start algorithm.
     *
     * \throws std::range_error if some parameter or combination of parameters
     *         does not fit.
     */
    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        const Sense sense,
        const unsigned seed,
        const unsigned chromosome_size,
        const BrkgaParams& params,
        const unsigned max_threads = 1,
        const bool evolutionary_mechanism_on = true
    );

    /// Destructor.
    ~BRKGA_MP_IPR() __attribute__((noinline)) = default;
    ///@}

    /** \name Initialization methods */
    ///@{
    /**
     * \brief Sets individuals to initial population.
     *
     * Set initial individuals into the population to work as warm-starters.
     * Such individuals can be obtained from solutions of external procedures
     * such as fast heuristics, other methaheuristics, or even relaxations from
     * a mixed integer programming model that models the problem.
     *
     * We assign as many individuals as possible across all populations.
     * Extra individuals are disregarded.
     *
     * \param chromosomes a set of individuals encoded as Chromosomes.
     */
    void setInitialPopulation(const std::vector<Chromosome>& chromosomes);

    /**
     * \brief Sets a custom bias function used to build the probabilities.
     *
     * It must be a **positive non-increasing function**, i.e.
     * \f$ f: \mathbb{N}^+ \to \mathbb{R}^+\f$ such that
     * \f$f(i) \ge 0\f$ and \f$f(i) \ge f(i+1)\f$ for
     * \f$i \in [1, \ldots, total\_parents]\f$.
     * For example
     * \code{.cpp}
     * setBiasCustomFunction(
     *     [](const unsigned x) {
     *         return 1.0 / (x * x);
     *     }
     * );
     * \endcode
     * sets an inverse quadratic function.
     *
     * \param func a reference to a unary positive non-increasing function.
     * \throws std::runtime_error in case the function is not a non-decreasing
     *         positive function.
     */
    void setBiasCustomFunction(
        const std::function<double(const unsigned)>& func
    );

    /**
     * \brief Sets a custom shaking procedure.
     *
     * For more details, see BrkgaParams::custom_shaking.
     *
     * \param func a callback function. For example, the code below implements
     *        the standard mutation:
     * \code{.cpp}
     * // A random number generator.
     * std::mt19937 rng(2700001);
     * rng.discard(rng.state_size);
     *
     * // Change some values from elite chromosomes from all populations.
     * // Similar to a standard mutation.
     * algorithm.setShakingMethod(
     *     [&](double lower_bound, double upper_bound,
     *         std::vector<std::shared_ptr<Population>>& populations,
     *         std::vector<std::pair<unsigned, unsigned>>& shaken) {
     *
     *         // Determines whether we change the allele or not.
     *         std::bernoulli_distribution must_change(0.50);
     *
     *         // Determines the value of the allele.
     *         std::uniform_real_distribution<> allele_value(lower_bound, upper_bound);
     *
     *         for(unsigned pop_idx = 0; pop_idx < populations.size(); ++pop_idx) {
     *             auto& population = populations[0]->population;
     *             for(unsigned chr_idx = 0; chr_idx < population.size(); ++chr_idx) {
     *                 auto& chromosome = population[chr_idx];
     *
     *                 bool change = false;
     *                 for(unsigned i = 0; i < chromosome.size(); ++i) {
     *                     if(must_change(rng)) {
     *                         chromosome[i] = allele_value(rng);
     *                         change = true;
     *                     }
     *                 }
     *
     *                 if(change)
     *                     shaken.push_back({pop_idx, chr_idx});
     *             }
     *         }
     *     };
     * );
     * \endcode
     */
    void setShakingMethod(
        const std::function<void(
            double lower_bound,
            double upper_bound,
            std::vector<std::shared_ptr<Population>>& populations,
            std::vector<std::pair<unsigned, unsigned>>& shaken
        )>& func
    );

    /**
     * \brief Sets a custom stopping criteria supplied by the user.

     * The algorithm always test for _the maximum running time_ and for
     * _the maximum stalled iterations/generations_ given by `ControlParams`
     * indenpendently of the stopping criteria function supplied by the user.
     * This is especially important when activating the implicit path reliking
     * which is **very timing consuming**.
     *
     * \warning If you are using IPR, we **STRONGLY RECOMMEND TO SET A MAXIMUM
     *      TIME** since this is the core stopping criteria on IPR.
     *
     * If you really mean to have no maximum time and/or maximum stalled
     * iterations set, we recommend to use the following code:
     *
     * \code{.cpp}
     * // After reading your parameters, e.g.,
     * // auto [brkga_params, control_params] = readConfiguration("config.conf");
     *
     * // You can set to the max.
     * control_params.maximum_running_time = std::chrono::seconds::max();
     *
     * control_params.stall_offset = numeric_limits<unsigned>::max();
     * \endcode
     *
     * \param stopping_criteria a callback function to determine is the
     *        algorithm must stop. For instance, the following lambda
     *        function tests if the best solution reached a given value for
     *        a minimization problem:
     * \code{.cpp}
     * fitness_t my_magical_solution = 10;
     *
     * algorithm.setStoppingCriteria(
     *     [&](const AlgorithmStatus& status) {
     *         return status.best_fitness <= my_magical_solution;
     *     }
     * );
     * \endcode
     */
    void setStoppingCriteria(
        const std::function<bool(const AlgorithmStatus&)>& stopping_criteria
    );

    /**
     * \brief Adds a callback function called when the best solution is
     * improved.
     *
     * It must take a reference to AlgorithmStatus and return `true`
     * if the algorithm should stop immediately. You may have as many observers
     * as you want. They will be called in the order they are added.
     *
     * \param func a callback function such as
     * \code{.cpp}
     * bool check_solution(const AlgorithmStatus& status) {
     *     std::cout << "\n" << status.best_fitness;
     *     return true; // Stop the optimization.
     * }
     * //...
     * algorithm.addNewSolutionObserver(check_solution);
     * \endcode
     * \n
     * or a lambda function such as
     * \n
     * \code{.cpp}
     * algorithm.addNewSolutionObserver(
     *     [](const AlgorithmStatus& status) {
     *         std::cout
     *         << "> Iter: " << status.current_iteration
     *         << " | solution: " << status.best_fitness
     *         << " | time: " << status.current_time
     *         << std::endl;
     *         return false; // Dont' stop the optimization.
     *      }
     * );
     * \endcode
     */
    void addNewSolutionObserver(
        const std::function<bool(const AlgorithmStatus&)>& func
    );
    ///@}

    /** \name Full algorithm runner */
    ///@{
    /**
     * \brief Runs the full framework performing evolution, path-reliking,
     *        exchanges, shakes, and resets according to the parameters.
     *
     * This method uses all facilities associated with this BRKGA-MP-IPR
     * library, providing a comprehensive and easy-to-use single-entry point.
     * The main loop always evolves one generation per iteration and calls
     * other procedures based on the number of stalled iterations (i.e.,
     * the number of iterations without improvement in the best solution),
     * and the given user thresholds in Control Parameters.
     * _If the thresholds are all the same_, the main loop should be like this:
     *
     * \code{.cpp}
     * while(!must_stop) {
     *     evolve(); // One generation.
     *     if(best solution improvement) {
     *         Save best solution;
     *         Call observer callbacks;
     *     }
     *
     *     if(!must_stop && ipr_interval > 0 && stalled_iterations > 0 &&
     *        stalled_iterations % ipr_interval == 0) {
     *          pathRelink();
     *          if(best solution improvement) {
     *              Save best solution;
     *              Call observer callbacks;
     *          }
     *      }
     *
     *     if(!must_stop && exchange_interval > 0 && stalled_iterations > 0 &&
     *        stalled_iterations % exchange_interval == 0) {
     *          exchangeElite();
     *     }
     *
     *     if(!must_stop && shake_interval > 0 && stalled_iterations > 0 &&
     *        stalled_iterations % shake_interval == 0) {
     *          shake();
     *     }
     *
     *     if(!must_stop && reset_interval > 0 && stalled_iterations > 0 &&
     *        stalled_iterations % reset_interval == 0) {
     *          reset();
     *     }
     * }
     * \endcode
     *
     * Therefore, note that the order that #pathRelink(), #exchangeElite(),
     * #shake(), and #reset() are called, depends on the thresholds defined
     * in ControlParams.
     *
     * For path relinking, the block size is computed by
     * \f$\lceil \alpha \times \sqrt{p} \rceil\f$
     * where \f$\alpha\f$ is BrkgaParams#alpha_block_size and
     * \f$p\f$ is BrkgaParams#population_size.
     * If the size is larger than the chromosome size, the size is set to
     * half of the chromosome size. For more details on that, refer to
     * #pathRelink().
     *
     * \note The algorithm always test against maximum running time and for the
     *      maximum stalled iterations/generations given by ControlParams
     *      indenpendently of the stopping criteria function supplied by the
     *      user. This is especially important when activating the implicit path
     *      reliking which is **very timing consuming**. If you are using IPR,
     *      we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** since this is
     *      the core stopping criteria on IPR.
     *
     * \warning The decoding is done in parallel using threads, and the user
     *      **must guarantee that the decoder is THREAD-SAFE.** If such property
     *      cannot be held, we suggest using a single thread for optimization.
     *
     * \param control_params the parameters to control the algorithm flow,
     *        such as calling exchanges, shakes, and IPR.
     *
     * \param logger a output stream to log some information.
     *
     * \returns The last algorithm status before the stopping criteria are met.
     *
     * \throws std::runtime_error in the following cases:
     *      -# IPR is active (ipr_interva > 0) but the distance function is
     *         not set;
     *      -# Shaking is active (shake_interval > 0) and it is set as 'CUSTOM'.
     *         However the custom shaking procedure was not supplied.
     *      -# Shaking is active (shake_interval > 0). However, the intensity
     *         bounds are out of range. Should be (0.0, 1.0] and
     *         'shaking_intensity_lower_bound <= shaking_intensity_upper_bound'.
     */
    AlgorithmStatus run(
        const ControlParams& control_params,
        std::ostream* logger = &std::cout
    );
    ///@}

    /** \name Evolution */
    ///@{
    /**
     * \brief Evolves the current populations following the guidelines of
     *        Multi-parent BRKGAs.
     *
     * \warning The decoding is done in parallel using threads, and the user
     *      **must guarantee that the decoder is THREAD-SAFE.** If such property
     *      cannot be held, we suggest using a single thread for optimization.
     *
     * \param generations number of generations to be evolved. Must be larger
     *        than zero.
     * \throws std::range_error if the number of generations is zero.
     */
    void evolve(unsigned generations = 1);
    ///@}

    /** \name Path relinking */
    ///@{
    /**
     * \brief Performs path relinking between elite solutions that are, at least,
     * a given minimum distance between themselves. In this method, the
     * local/loaded parameters are ignored in favor to the supplied ones.
     *
     * In the presence of multiple populations, the path relinking is performed
     * between elite chromosomes from different populations, in a circular
     * fashion. For example, suppose we have 3 populations. The framework
     * performs 3 path relinkings: the first between individuals from
     * populations 1 and 2, the second between populations 2 and 3, and the
     * third between populations 3 and 1. In the case of just one population,
     * both base and guiding individuals are sampled from the elite set of that
     * population.
     *
     * Note that the algorithm tries to find a pair of base and guiding
     * solutions with a minimum distance given by the distance function. If this
     * is not possible, a new pair of solutions are sampled (without
     * replacement) and tested against the distance. In case it is not possible
     * to find such pairs for the given populations, the algorithm skips to the
     * next pair of populations (in a circular fashion, as described above).
     * Yet, if such pairs are not found in any case, the algorithm declares
     * failure. This indicates that the populations are very homogeneous.
     *
     * If the found solution is the best solution found so far, IPR replaces the
     * worst solution by it. Otherwise, IPR computes the distance between the
     * found solution and all other solutions in the elite set, and replaces the
     * worst solution by it if and only if the found solution is, at least,
     * `minimum_distance` from all them.
     *
     * The API will call `Decoder::decode()` function always with `rewrite =
     * false`. The reason is that if the decoder rewrites the chromosome, the
     * path between solutions is lost and inadvertent results may come up. Note
     * that at the end of the path relinking, the method calls the decoder with
     * `rewrite = true` in the best chromosome found to guarantee that this
     * chromosome is re-written to reflect the best solution found.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)`
     * additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * \warning As it is in #evolve(), the decoding is done in parallel using
     *      threads, and the user **must guarantee that the decoder is
     *      THREAD-SAFE.** If such property cannot be held, we suggest using
     *      a single thread for optimization.
     *
     * \param pr_type type of path relinking to be performed.
     *        See PathRelinking::Type.
     * \param pr_selection of which individuals use to path relinking.
     *        See PathRelinking::Selection.
     * \param dist a pointer to a functor/object to compute the distance between
     *        two chromosomes. This object must be inherited from
     *        BRKGA::DistanceFunctionBase and implement its methods.
     * \param number_pairs number of chromosome pairs to be tested.
     *        If 0, all pairs are tested.
     * \param minimum_distance between two chromosomes computed by `dist`.
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage defines the size, in percentage, of the path to
     *        build. Default: 1.0 (100%).
     *
     * \returns A PathRelinking::PathRelinkingResult depending on the relink
     *          status.
     *
     * \throws std::range_error if the percentage or size of the path is
     *         not in (0, 1].
     */
    PathRelinking::PathRelinkingResult pathRelink(
        PathRelinking::Type pr_type,
        PathRelinking::Selection pr_selection,
        std::shared_ptr<DistanceFunctionBase> dist,
        unsigned number_pairs,
        double minimum_distance,
        std::size_t block_size = 1,
        std::chrono::seconds max_time = std::chrono::seconds{0},
        double percentage = 1.0
    );

    /**
     * \brief Performs path relinking between elite solutions that are,
     *        at least, a given minimum distance between themselves.
     *
     * This method uses all parameters supplied in the constructor.
     * In particular, the block size is computed by
     * \f$\lceil \alpha \times \sqrt{p} \rceil\f$
     * where \f$\alpha\f$ is BrkgaParams#alpha_block_size and
     * \f$p\f$ is BrkgaParams#population_size.
     * If the size is larger than the chromosome size, the size is set to
     * half of the chromosome size.
     *
     * Please, refer to #pathRelink() for details.
     *
     * \param dist a pointer to a functor/object to compute the distance between
     *        two chromosomes. This object must be inherited from
     *        BRKGA::DistanceFunctionBase and implement its methods.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     *
     * \returns A PathRelinking::PathRelinkingResult depending on the relink
     *          status.
     *
     * \throws std::range_error if the percentage or size of the path is
     *         not in (0, 1].
     */
    PathRelinking::PathRelinkingResult pathRelink(
        std::shared_ptr<DistanceFunctionBase> dist,
        std::chrono::seconds max_time = std::chrono::seconds{0}
    );
    ///@}

    /** \name Population manipulation methods */
    ///@{
    /**
     * \brief Exchanges elite-solutions between the populations.

     * Given a population, the `num_immigrants` best solutions are copied to
     * the neighbor populations, replacing their worth solutions. If there is
     * only one population, nothing is done.

     * \param num_immigrants number of elite chromosomes to select from each
     *        population.
     * \throws std::range_error if the number of immigrants less than one or
     *         it is larger than or equal to the population size divided by
     *         the number of populations minus one, i.e. \f$\lceil
     *         \frac{population\_size}{num\_independent\_populations} \rceil
     *         - 1\f$.
     */
    void exchangeElite(unsigned num_immigrants);

    /**
     * \brief Resets all populations with brand new keys.
     *
     * All warm-start solutions provided #setInitialPopulation() are discarded.
     * You may use #injectChromosome() to insert those solutions again.
     */
    void reset();

    /**
     * \brief Performs a shaking in the chosen population.
     * \param intensity the intensity of the shaking.
     * \param shaking_type either `CHANGE` or `SWAP` moves.
     * \param population_index the index of the population to be shaken. If
     * `population_index >= num_independent_populations`, all populations
     * are shaken.
     */
    void shake(
        unsigned intensity,
        ShakingType shaking_type,
        unsigned population_index = std::numeric_limits<unsigned>::infinity()
    );

    /**
     * \brief Injects/Replaces a chromosome of a given population into a
     *         given position.
     *
     * The new chromosome replaces the old one, and the decoder is triggered to
     * compute the new fitness. Once done, the population is re-sorted
     * according to the chromosomes' fitness.
     *
     * \param chromosome the chromosome to be injected.
     * \param population_index the population index.
     * \param position the chromosome position.
     *
     * \throws std::range_error eitheir if `population_index` is larger
     *         than number of populations; or `position` is larger than the
     *         population size; or ` chromosome.size() != chromosome_size`
     */
    void injectChromosome(
        const Chromosome& chromosome,
        unsigned population_index,
        unsigned position
    );
    ///@}

    /** \name Support methods */
    ///@{
    /**
     * \brief Returns a reference to a current population.
     * \param population_index the population index.
     * \throws std::range_error if the index is larger than number of
     *         populations.
     */
    const Population& getCurrentPopulation(unsigned population_index = 0) const;

    /**
     * \brief Returns a reference to the chromosome with best fitness among
     * all current populations.
     *
     * \warning Note that this method **does not** return the best solution but
     *      the one within the current population. If a #shake() or #reset()
     *      is called, the best solution may be lose in the populations.
     *      However, if you are using #run(), the best solution is returned
     *      by that method. If not, you must keep track of the best solution.
     *
     */
    const Chromosome& getBestChromosome() const;

    /**
     * \brief Returns the best fitness among all current populations.
     *
     * \warning Note that this method **does not** return the best fitness but
     *      the one within the current population. If a #shake() or #reset()
     *      is called, the best fitness may be lose in the populations.
     *      However, if you are using #run(), the best fitness is returned
     *      by that method. If not, you must keep track of the best fitness.
     */
    fitness_t getBestFitness() const;

    /**
     * \brief Returns a reference to a chromosome of the given population.
     * \param population_index the population index.
     * \param position the chromosome position, ordered by fitness.
     *        The best chromosome is located in position 0.
     * \throws std::range_error eitheir if `population_index` is larger
     *         than number of populations, or `position` is larger than the
     *         population size.
     */
    const Chromosome& getChromosome(
        unsigned population_index,
        unsigned position
    ) const;

    /**
     * \brief Returns the fitness of a chromosome of the given population.
     * \param population_index the population index.
     * \param position the chromosome position, ordered by fitness.
     *        The best chromosome is located in position 0.
     * \throws std::range_error eitheir if `population_index` is larger
     *         than number of populations, or `position` is larger than the
     *         population size.
     */
    fitness_t getFitness(unsigned population_index, unsigned position) const;
    ///@}

    /** \name Parameter getters */
    ///@{
    const BrkgaParams& getBrkgaParams() const { return params; }

    Sense getOptimizationSense() const { return optimization_sense; }

    unsigned getChromosomeSize() const { return chromosome_size; }

    unsigned getEliteSize() const { return elite_size; }

    unsigned getNumMutants() const { return num_mutants; }

    bool evolutionaryIsMechanismOn() const { return evolutionary_mechanism_on; }

    unsigned getMaxThreads() const { return max_threads; }
    ///@}

protected:
    /** \name BRKGA Hyper-parameters */
    ///@{
    /// The BRKGA and IPR hyper-parameters.
    BrkgaParams params;

    /// Indicates whether we are maximizing or minimizing.
    const Sense optimization_sense;

    /// Number of genes in the chromosome.
    const unsigned chromosome_size;

    /// Number of elite items in the population.
    unsigned elite_size;

    /// Number of mutants introduced at each generation into the population.
    unsigned num_mutants;

    /**
     * \brief If false, no evolution is performed but only chromosome decoding.
     * Very useful to emulate a multi-start algorithm.
     */
    bool evolutionary_mechanism_on;
    ///@}

    /** \name Parallel computing parameters */
    ///@{
    /// Number of threads for parallel decoding.
    const unsigned max_threads;
    ///@}

    /** \name Engines */
    ///@{
    /// Reference to the problem-dependent Decoder.
    Decoder& decoder;

    /**
     * \brief Mersenne twister random number generators. For parallel mating,
     * we must have one RNG per thread so that we can reproduce the results of
     * an experiment. We use the first RNG as the main generator in
     * several parts of the code. The other RNGs are used only during mating.
     */
    std::vector<std::mt19937> rng_per_thread;

    /// Just a uniform distribution to generate integers.
    std::uniform_int_distribution<unsigned> integer_distribution;
    ///@}

    /** \name Algorithm data */
    ///@{
    /// Previous populations.
    std::vector<std::shared_ptr<Population>> previous;

    /// Current populations.
    std::vector<std::shared_ptr<Population>> current;

    /// Reference for the bias function.
    std::function<double(const unsigned)> bias_function;

    /**
     * \brief Holds the sum of the results of each raking given a bias function.
     * This value is needed to normalization.
     */
    double total_bias_weight;

    #if defined(MATING_SEED_ONLY) || defined(DOXYGEN_DOC_MACROS)
    /**
     * \brief Set of seeds for each RNG if #MATING_SEED_ONLY is enabled.
     *
     * During parallel mating, we need to be sure that the same random values
     * are generated in each mating, despite the number of threads available.
     * Therefore, on each iteration, we generate a set of seeds for each RNG.
     */
    std::vector<std::mt19937::result_type> mating_seeds;
    #endif

    /**
     * \brief Used to shuffled individual/chromosome indices during the mate.
     * We have one for each thread during parallel mating.
     */
    std::vector<std::vector<unsigned>> shuffled_individuals_per_thread;

    /**
     * \brief Defines the order of parents during the mating.
     * We have one for each thread during parallel mating.
     */
    std::vector<std::vector<std::pair<fitness_t, unsigned>>>
    parents_ordered_per_thread;

    /**
     * \brief Temporary structures that hold the offsrping per thread.
     * Used to reduce the caching missing, and speed up the mating.
     */
    std::vector<std::vector<double>> offspring_per_thread;

    /// Indicates whether a initial population is set.
    bool initial_population;

    /// Indicates if the algorithm was proper initialized.
    bool initialized;

    /// Holds the start time for a call of the path relink procedure.
    std::chrono::system_clock::time_point pr_start_time;
    ///@}

    /** \name Callbacks */
    ///@{
    /// Defines a custom stopping criteria supplied by the user.
    std::function<bool(const AlgorithmStatus&)> stopping_criteria;

    /** \brief Callback functions called when the best solution is improved.
     * It must take a reference to AlgorithmStatus and return `true`
     * if the algorithm should stop immediately.
     */
    std::vector<std::function<bool(const AlgorithmStatus&)>> info_callbacks;
    ///@}

protected:
    /** \name Core local methods */
    ///@{
    /**
     * \brief Initializes the populations and others parameters of the
     *        algorithm.
     *
     * If a initial population is supplied, this method completes the remain
     * individuals, if they do not exist. This method also performs the initial
     * decoding of the chromosomes. Therefore, depending on the decoder
     * implementation, this can take a while, and the user may want to time
     * such procedure in his/her experiments.
     *
     * \warning This method must be call before any population handling method.
     *
     * \warning As it is in #evolve(), the decoding is done in parallel using
     *      threads, and the user **must guarantee that the decoder is
     *      THREAD-SAFE.** If such property cannot be held, we suggest using
     *      a single thread for optimization.
     *
     * \param reset when set true, it ignores all solutions provided
     *        by #setInitialPopulation(), and builds a completely random
     *        population. This parameter is set to true during reset phase.
     *        When false, #initialize() considers all solutions given by
     *        #setInitialPopulation().
     */
    void initialize(bool reset = false);

    /**
     * \brief Evolves the current population to the next.
     *
     * Note that the next population will be re-populate completely.
     *
     * \param[in] curr current population.
     * \param[out] next next population.
     */
    void evolution(Population& curr, Population& next);

    /**
     * \brief Performs the direct path relinking.
     *
     * This method changes each allele or block of alleles of base chromosome
     * for the correspondent one in the guide chromosome.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)` additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * \param chr1 first chromosome.
     * \param chr2 second chromosome.
     * \param dist distance functor (distance between two chromosomes).
     * \param[out] best_found best solution found in the search.
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time abort path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage define the size, in percentage, of the path to build.
     */
    void directPathRelink(
        const Chromosome& chr1, const Chromosome& chr2,
        std::shared_ptr<DistanceFunctionBase> dist,
        std::pair<fitness_t, Chromosome>& best_found,
        std::size_t block_size,
        std::chrono::seconds max_time,
        double percentage
    );

    /**
     * \brief Performs the permutation-based path relinking.
     *
     * In this method, the permutation induced by the keys in the guide
     * solution is used to change the order of the keys in the permutation
     * induced by the base solution.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)` additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * The path relinking is performed by changing the order of
     * each allele of base chromosome for the correspondent one in
     * the guide chromosome.
     * \param chr1 first chromosome
     * \param chr2 second chromosome
     * \param dist distance functor (distance between two chromosomes)
     * \param[out] best_found best solution found in the search
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time abort path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage define the size, in percentage, of the path to
     *        build. Default: 1.0 (100%)
     */
    void permutatioBasedPathRelink(
        Chromosome& chr1, Chromosome& chr2,
        std::shared_ptr<DistanceFunctionBase> dist,
        std::pair<fitness_t, Chromosome>& best_found,
        std::size_t block_size,
        std::chrono::seconds max_time,
        double percentage
    );
    ///@}

    /** \name Helper functions */
    ///@{
    /**
     * \brief Returns `true` if `a1` is better than `a2`.
     *
     * This method depends on the optimization sense. When the optimization
     * sense is Sense::MINIMIZE, `a1 < a2` will return true, otherwise false.
     * The opposite happens for Sense::MAXIMIZE.
     */
    inline bool betterThan(const fitness_t& a1, const fitness_t& a2) const;

    /**
     * \brief Distributes real values of given precision across [0, 1] evenly.
     * \param rng The random number generator to be used.
     */
    inline double rand01(std::mt19937& rng);

    /**
     * \brief Returns an integer number between `0` and `n`.
     * \param n The upper bound.
     * \param rng The random number generator to be used.
     */
    inline unsigned randInt(const unsigned n, std::mt19937& rng);
    ///@}
};
///@} main_algorithms

//----------------------------------------------------------------------------//
/// \cond IGNORE_IMPLEMENTATION

template <class Decoder>
BRKGA_MP_IPR<Decoder>::BRKGA_MP_IPR(
        Decoder& _decoder_reference,
        const Sense _sense,
        unsigned _seed,
        unsigned _chromosome_size,
        const BrkgaParams& _params,
        const unsigned _max_threads,
        const bool _evolutionary_mechanism_on):

        // Algorithm parameters.
        params(_params),
        optimization_sense(_sense),
        chromosome_size(_chromosome_size),
        elite_size(_evolutionary_mechanism_on?
                    unsigned(params.elite_percentage *
                             params.population_size)
                    : 1),
        num_mutants(_evolutionary_mechanism_on?
                    unsigned(params.mutants_percentage *
                             params.population_size):
                    params.population_size - 1),
        evolutionary_mechanism_on(_evolutionary_mechanism_on),
        max_threads(_max_threads),

        // Internal data.
        decoder(_decoder_reference),
        rng_per_thread(_max_threads),
        integer_distribution(),
        previous(params.num_independent_populations, nullptr),
        current(params.num_independent_populations, nullptr),
        bias_function(),
        total_bias_weight(0.0),
        #ifdef MATING_SEED_ONLY
            mating_seeds(params.population_size - elite_size - num_mutants, 0),
        #endif
        #ifndef MATING_SEQUENTIAL
            shuffled_individuals_per_thread(
                _max_threads,
                typename decltype(shuffled_individuals_per_thread)
                    ::value_type(params.population_size)
            ),
            parents_ordered_per_thread(
                _max_threads,
                typename decltype(parents_ordered_per_thread)
                    ::value_type(params.total_parents)
            ),
            offspring_per_thread(
                _max_threads,
                typename decltype(offspring_per_thread)
                    ::value_type(_chromosome_size)
            ),
        #else
            shuffled_individuals_per_thread(1,
                typename decltype(shuffled_individuals_per_thread)
                    ::value_type(params.population_size)
            ),
            parents_ordered_per_thread(1,
                typename decltype(parents_ordered_per_thread)
                    ::value_type(params.total_parents)
            ),
            offspring_per_thread(1,
                typename decltype(offspring_per_thread)
                    ::value_type(_chromosome_size)
            ),
        #endif
        initial_population(false),
        initialized(false),
        pr_start_time(),
        stopping_criteria(),
        info_callbacks()
{
    using std::range_error;
    std::stringstream ss;

    if(chromosome_size < 2)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Chromosome size must be larger than one, current size: "
           << chromosome_size;
    else
    if(params.population_size == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Population size must be larger than zero, current size: "
           << params.population_size;
    else
    if(elite_size == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite-set size equals zero";
    else
    if(elite_size > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite-set size (" << elite_size
           << ") greater than population size (" << params.population_size << ")";
    else
    if(num_mutants > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Mutant-set size (" << num_mutants
           << ") greater than population size (" << params.population_size << ")";
    else
    if(elite_size + num_mutants > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite (" << elite_size << ") + mutant sets (" << num_mutants
           << ") greater than population size (" << params.population_size << ")";
    else
    if(params.num_elite_parents < 1)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "num_elite_parents must be at least 1, current: "
           << params.num_elite_parents;
    else
    if(params.total_parents < 2)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Total_parents must be at least 2, current: " << params.total_parents;
    else
    if(params.num_elite_parents >= params.total_parents)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Num_elite_parents (" << params.num_elite_parents << ") "
           << "is greater than total_parents (" << params.total_parents << ")";
    else
    if(params.num_elite_parents > elite_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Num_elite_parents (" << params.num_elite_parents
           << ") is greater than elite set (" << elite_size << ")";
    else
    if(params.num_independent_populations == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Number of independent/parallel populations cannot be zero";
    else
    if(params.alpha_block_size < 1e-6)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "(alpha) block size <= 0.0";
    else
    if(params.pr_percentage < 1e-6 || params.pr_percentage > 1.0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Path relinking percentage (" << params.pr_percentage
           << ") is not in the range (0, 1]";

    const auto str_error = ss.str();
    if(str_error.length() > 0)
        throw range_error(str_error);

    // Chooses the bias function.
    std::function<double(unsigned)> local_bias_function;
    switch(params.bias_type) {
    case BiasFunctionType::LOGINVERSE:
        // Same as log(r + 1), but avoids precision loss.
        local_bias_function = [](const unsigned r) { return 1.0 / log1p(r); };
        break;

    case BiasFunctionType::LINEAR:
        local_bias_function = [](const unsigned r) { return 1.0 / r; };
        break;

    case BiasFunctionType::QUADRATIC:
        local_bias_function = [](const unsigned r) { return pow(r, -2); };
        break;

    case BiasFunctionType::CUBIC:
        local_bias_function = [](const unsigned r) { return pow(r, -3); };
        break;

    case BiasFunctionType::EXPONENTIAL:
        local_bias_function = [](const unsigned r) { return exp(-1.0 * r); };
        break;

    case BiasFunctionType::CONSTANT:
    default:
        local_bias_function = [&](const unsigned) { return 1.0 / params.total_parents; };
        break;
    }

    // We call setBiasCustomFunction() instead to set the bias function directly
    // because this method computes the `total_bias_weight`.
    setBiasCustomFunction(local_bias_function);

    // We could initialize each RNG with the same seed. However, this can skew
    // the mating process slightly because we would have the same choices for
    // groups of alleles in the same iteration. Therefore, we just initialize
    // the RNGs in a chain so that they have different states, although still
    // reproducible. So, we start with the given seed for the first RNG.
    rng_per_thread[0].seed(_seed);
    rng_per_thread[0].discard(rng_per_thread[0].state_size);

    // For the other RNGs, we use the state of the previous RNG as seed.
    #ifndef MATING_SEQUENTIAL
        for(size_t i = 1; i < rng_per_thread.size(); ++i) {
            auto& local_rng = rng_per_thread[i];
            local_rng.seed(rng_per_thread[i - 1]());
            local_rng.discard(local_rng.state_size);
        }
    #endif
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline bool BRKGA_MP_IPR<Decoder>::betterThan(const fitness_t& a1,
                                              const fitness_t& a2) const {
    return (optimization_sense == Sense::MINIMIZE)? (a1 < a2) : (a1 > a2);
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Population&
BRKGA_MP_IPR<Decoder>::getCurrentPopulation(unsigned population_index) const {
    if(population_index >= current.size())
        throw std::range_error("The index is larger than number of "
                               "populations");
    return (*current[population_index]);
}

//----------------------------------------------------------------------------//

template <class Decoder>
fitness_t BRKGA_MP_IPR<Decoder>::getBestFitness() const {
    fitness_t best = current[0]->fitness[0].first;
    for(unsigned i = 1; i < params.num_independent_populations; ++i) {
        const auto& curr_fitness = current[i]->fitness[0].first;
        if(betterThan(curr_fitness, best))
            best = curr_fitness;
    }
    return best;
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getBestChromosome() const {
    unsigned best_k = 0;
    for(unsigned i = 1; i < params.num_independent_populations; ++i)
        if(betterThan(current[i]->getBestFitness(),
                      current[best_k]->getBestFitness()))
            best_k = i;

    // Since the chromosomes are ordered by fitness, the first is the best.
    return current[best_k]->getChromosome(0);
}

//----------------------------------------------------------------------------//

template <class Decoder>
fitness_t BRKGA_MP_IPR<Decoder>::getFitness(unsigned population_index,
                                            unsigned position) const {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    return current[population_index]->fitness[position].first;
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getChromosome(
        unsigned population_index, unsigned position) const {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    return current[population_index]->getChromosome(position);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::injectChromosome(const Chromosome& chromosome,
        unsigned population_index, unsigned position) {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(chromosome.size() != chromosome_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Wrong chromosome size. It should be " << chromosome_size
           << " but get " << chromosome.size();
        throw std::range_error(ss.str());
    }

    auto& pop = current[population_index];
    auto& local_chr = pop->chromosomes[pop->fitness[position].second];
    local_chr = chromosome;

    pop->setFitness(position, decoder.decode(local_chr, true));
    pop->sortFitness(optimization_sense);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setBiasCustomFunction(
            const std::function<double(const unsigned)>& func) {

    std::vector<double> bias_values(params.total_parents);
    std::iota(bias_values.begin(), bias_values.end(), 1);
    std::transform(bias_values.begin(), bias_values.end(),
                   bias_values.begin(), func);

    // If it is not non-increasing, throw an error.
    if(!std::is_sorted(bias_values.rbegin(), bias_values.rend())) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The bias function must be positive non-decreasing";
        throw std::range_error(ss.str());
    }

    if(bias_function)
        params.bias_type = BiasFunctionType::CUSTOM;

    bias_function = func;
    total_bias_weight =
        std::accumulate(bias_values.begin(), bias_values.end(), 0.0);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::reset() {
    initialize(true);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::evolve(unsigned generations) {
    if(generations == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Cannot evolve for 0 generations.";
        throw std::range_error(ss.str());
    }

    if(!initialized)
        initialize();

    for(unsigned i = 0; i < generations; ++i) {
        for(unsigned j = 0; j < params.num_independent_populations; ++j) {
            // Evolve the current population to a new one into `previuos` and...
            evolution(*current[j], *previous[j]);
            //... swap places.
            std::swap(current[j], previous[j]);
        }
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::exchangeElite(unsigned num_immigrants) {
    if(params.num_independent_populations == 1)
        return;

    const unsigned immigrants_threshold =
        ceil(params.population_size / (params.num_independent_populations - 1));

    if(num_immigrants < 1 || num_immigrants >= immigrants_threshold) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Number of immigrants (" << num_immigrants << ") less than one, "
              "or larger than or equal to population size / "
              "num_independent_populations (" << immigrants_threshold << ")";
        throw std::range_error(ss.str());
    }

    if(!initialized)
        initialize();

    #ifdef _OPENMP
        #pragma omp parallel for num_threads(max_threads)
    #endif
    for(unsigned i = 0; i < params.num_independent_populations; ++i) {
        // Population i will receive some elite members from each Population j.
        // Last chromosome of i (will be overwritten below).
        unsigned dest = params.population_size - 1;
        for(unsigned j = 0; j < params.num_independent_populations; ++j) {
            if(j == i)
                continue;

            // Copy the num_immigrants best from Population j into Population i.
            for(unsigned m = 0; m < num_immigrants; ++m) {
                // Copy the m-th best of Population j into the 'dest'-th
                // position of Population i
                const auto& best_of_j = current[j]->getChromosome(m);
                std::copy(best_of_j.begin(), best_of_j.end(),
                          current[i]->getChromosome(dest).begin());
                current[i]->fitness[dest].first = current[j]->fitness[m].first;
                --dest;
            }
        }
    }

    // Re-sort each population since they were modified.
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(max_threads)
    #endif
    for(unsigned i = 0; i < params.num_independent_populations; ++i)
        current[i]->sortFitness(optimization_sense);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setInitialPopulation(
                                const std::vector<Chromosome>& chromosomes) {
    // First, reserve some memory.
    for(auto& pop : current) {
        pop.reset(new Population(chromosome_size, params.population_size));
        pop->chromosomes.clear();
    }

    auto it_init_chr = chromosomes.begin();
    auto it_pop = current.begin();

    unsigned counter = 0;
    while(it_init_chr != chromosomes.end()) {
        if(it_init_chr->size() != chromosome_size) {
            std::stringstream ss;
            ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
               << "Error on setting initial population: chromosome " << counter
               << " does not have the required dimension"
               << " (actual size: " << it_init_chr->size()
               << ", required size: " << chromosome_size << ")";
            throw std::runtime_error(ss.str());
        }

        (*it_pop)->chromosomes.push_back(*it_init_chr);
        ++counter;
        ++it_init_chr;

        if(counter == params.population_size) {
            ++it_pop;
            counter = 0;
        }

        if(it_pop == current.end())
            it_init_chr = chromosomes.end();
    }

    initial_population = true;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::initialize(bool reset) {
    auto& rng = rng_per_thread[0];

    // Check and complete the populations.
    for(auto& pop : current) {
        const bool new_pop = !pop;
        if(new_pop) {
            pop.reset(new Population(chromosome_size, params.population_size));
        }
        else {
            pop->fitness.resize(params.population_size);
        }

        if(reset)
            pop->chromosomes.clear();

        if(new_pop || (pop->chromosomes.size() < params.population_size)) {
            Chromosome chromosome(chromosome_size);
            unsigned last_chromosome = new_pop? 0 : pop->chromosomes.size();

            pop->chromosomes.resize(params.population_size);
            for(; last_chromosome < params.population_size; ++last_chromosome) {
                for(unsigned k = 0; k < chromosome_size; ++k)
                    chromosome[k] = rand01(rng);
                pop->chromosomes[last_chromosome] = chromosome;
            }
        }
    }

    // Initialize and decode each chromosome of the current population,
    // then copy to previous.
    for(unsigned i = 0; i < params.num_independent_populations; ++i) {
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(max_threads) schedule(static,1)
        #endif
        for(unsigned j = 0; j < params.population_size; ++j)
            current[i]->setFitness(j, decoder.decode((*current[i])(j), true));

        // Sort and copy to previous.
        current[i]->sortFitness(optimization_sense);
        previous[i].reset(new Population(*current[i]));
    }

    initialized = true;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::shake(unsigned intensity,
                                  ShakingType shaking_type,
                                  unsigned population_index) {
    if(!initialized)
        initialize();

    auto& rng = rng_per_thread[0];

    unsigned pop_start = population_index;
    unsigned pop_end = population_index;
    if(population_index >= params.num_independent_populations) {
        pop_start = 0;
        pop_end = params.num_independent_populations - 1;
    }

    for(; pop_start <= pop_end; ++pop_start) {
        auto& pop = current[pop_start]->chromosomes;

        // Shake the elite set.
        for(unsigned e = 0; e < elite_size; ++e) {
            for(unsigned k = 0; k < intensity; ++k) {
                auto i = randInt(chromosome_size - 2, rng);
                if(shaking_type == ShakingType::CHANGE) {
                    // Invert value.
                    pop[e][i] = 1.0 - pop[e][i];
                }
                else {
                    // Swap with neighbor.
                    std::swap(pop[e][i], pop[e][i + 1]);
                }

                i = randInt(chromosome_size - 1, rng);
                if(shaking_type == ShakingType::CHANGE) {
                    // Change to random value.
                    pop[e][i] = rand01(rng);
                }
                else {
                    // Swap two random positions.
                    auto j = randInt(chromosome_size - 1, rng);
                    std::swap(pop[e][i], pop[e][j]);
                }
            }
        }

        // Reset the remaining population.
        for(unsigned ne = elite_size; ne < params.population_size; ++ne) {
            for(unsigned k = 0; k < chromosome_size; ++k)
                pop[ne][k] = rand01(rng);
        }

        #ifdef _OPENMP
            #pragma omp parallel for num_threads(max_threads) schedule(static,1)
        #endif
        for(unsigned j = 0; j < params.population_size; ++j)
            current[pop_start]->
                setFitness(j, decoder.decode((*current[pop_start])(j), true));

        // Now we must sort by fitness, since things might have changed.
        current[pop_start]->sortFitness(optimization_sense);
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::evolution(Population& curr,
                                      Population& next) {

    // To ensure the reproducibility, we need to be sure that the same random
    // values are generated in each mating, despite the number of threads
    // available. For that, we first generate a seed for each mating step. Then
    // use them in a RNG within the a thread.
    #ifdef MATING_SEED_ONLY
    { // hidden namespace
        auto& rng = rng_per_thread[0];
        for(auto& s: mating_seeds)
            s = rng();
    }
    #endif

    // First, we copy the elite chromosomes to the next generation.
    for(unsigned chr_idx = 0; chr_idx < elite_size; ++chr_idx) {
        next.chromosomes[chr_idx] = curr.chromosomes[curr.fitness[chr_idx].second];
        next.fitness[chr_idx] = std::make_pair(curr.fitness[chr_idx].first, chr_idx);
    }

    // Second, we mate 'pop_size - elite_size - num_mutants' pairs.
    // This parallel region allows processing the costly std::shuffle
    // for each mating in parallel.
    #ifndef MATING_SEQUENTIAL
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(max_threads) schedule(static, 1)
    #endif
    #endif
    for(auto chr_idx = elite_size;
        chr_idx < params.population_size - num_mutants; ++chr_idx) {

        #ifdef _OPENMP
            auto& shuffled_individuals =
                shuffled_individuals_per_thread[omp_get_thread_num()];
            auto& parents_ordered =
                parents_ordered_per_thread[omp_get_thread_num()];
            auto& offspring = offspring_per_thread[omp_get_thread_num()];
            auto& rng = rng_per_thread[omp_get_thread_num()];
        #else
            auto& shuffled_individuals = shuffled_individuals_per_thread[0];
            auto& parents_ordered = parents_ordered_per_thread[0];
            auto& offspring = offspring_per_thread[0];
            auto& rng = rng_per_thread[0];
        #endif

        #ifdef MATING_SEED_ONLY
        // Reseed the RNG to guarantee reproducibility.
        rng.seed(mating_seeds[chr_idx - elite_size]);
        #endif

        // First, we shuffled the elite set and non-elite set indices,
        // then we take the elite and non-elite parents. Note that we cannot
        // shuffled both sets together, otherwise we would mix elite
        // and non-elite individuals.

        // Rebuild the indices.
        std::iota(shuffled_individuals.begin(), shuffled_individuals.end(), 0);

        // Shuffles elite.
        std::shuffle(shuffled_individuals.begin(),
                     shuffled_individuals.begin() + elite_size, rng);

        // Shuffles non-elite.
        std::shuffle(shuffled_individuals.begin() + elite_size,
                     shuffled_individuals.end(), rng);

        parents_ordered.clear();

        // Take the elite parents.
        for(unsigned j = 0; j < params.num_elite_parents; ++j) {
            parents_ordered.emplace_back(curr.fitness[shuffled_individuals[j]]);
        }

        // Take the non-elite parents.
        for(unsigned j = 0;
            j < params.total_parents - params.num_elite_parents; ++j) {
            parents_ordered.emplace_back(
                    curr.fitness[shuffled_individuals[j + elite_size]]);
        }

        if(optimization_sense == Sense::MAXIMIZE)
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::greater<std::pair<fitness_t, unsigned>>());
        else
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::less<std::pair<fitness_t, unsigned>>());

        // Performs the mate.
        for(unsigned allele = 0; allele < chromosome_size; ++allele) {
            // Roullete method.
            unsigned parent = 0;
            double cumulative_probability = 0.0;
            const double toss = rand01(rng);
            do {
                // Start parent from 1 because the bias function.
                cumulative_probability += bias_function(++parent) /
                                          total_bias_weight;
            } while(cumulative_probability < toss);

            // Decrement parent to the right index, and take the allele value.
            offspring[allele] = curr(parents_ordered[--parent].second, allele);
        }

        // This strategy of setting the offpring in a local variable, and then
        // copying to the population seems to reduce the overall cache misses
        // counting.
        next.chromosomes[chr_idx] = offspring;
        next.fitness[chr_idx] = std::make_pair(-1, chr_idx);
    }

    // To finish, we fill up the remaining spots with mutants.
    #ifndef MATING_SEQUENTIAL
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(max_threads) schedule(static, 1)
    #endif
    #endif
    for(auto chr_idx = params.population_size - num_mutants;
        chr_idx < params.population_size; ++chr_idx) {
        #ifdef _OPENMP
            auto& rng = rng_per_thread[omp_get_thread_num()];
        #else
            auto& rng = rng_per_thread[0];
        #endif
        next.fitness[chr_idx] = std::make_pair(-1, chr_idx);
        for(auto& allele : next.chromosomes[chr_idx])
            allele = rand01(rng);
    }

    // Time to compute fitness, in parallel.
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(max_threads) schedule(static, 1)
    #endif
    for(auto i = elite_size; i < params.population_size; ++i) {
        next.setFitness(i, decoder.decode(next.chromosomes[i], true));
    }

    // Now we must sort by fitness, since things might have changed.
    next.sortFitness(optimization_sense);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setShakingMethod(
        const std::function<void(
            double lower_bound,
            double upper_bound,
            std::vector<std::shared_ptr<Population>>& populations,
            std::vector<std::pair<unsigned, unsigned>>& shaken
        )>& func
    ) {

    params.custom_shaking = func;
    params.shaking_type = ShakingType::CUSTOM;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setStoppingCriteria(
        const std::function<bool(const AlgorithmStatus&)>& _stopping_criteria) {
    stopping_criteria = _stopping_criteria;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::addNewSolutionObserver(
        const std::function<bool(const AlgorithmStatus&)>& func) {
    info_callbacks.push_back(func);
}

//----------------------------------------------------------------------------//

template <class Decoder>
BRKGA::AlgorithmStatus BRKGA_MP_IPR<Decoder>::run(
        const ControlParams& control_params,
        std::ostream* logger) {

    if(!initialized)
        initialize();

    if(control_params.ipr_interval > 0 && !params.pr_distance_function) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "IPR is active (ipr_interval = " << control_params.ipr_interval
           << ") but the distance function is not set.";
        throw std::runtime_error(ss.str());
    }

    if(control_params.shake_interval > 0) {
        if(params.shaking_type == ShakingType::CUSTOM && !params.custom_shaking) {
            std::stringstream ss;
            ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
               << "Shaking is active (shake_interval = "
               << control_params.shake_interval << ") and it is set as 'CUSTOM'. "
               << "However the custom shaking procedure was not supplied.";
            throw std::runtime_error(ss.str());
        }

        if(params.shaking_type != ShakingType::CUSTOM) {
            if((params.shaking_intensity_lower_bound < 1e-6 ||
                params.shaking_intensity_lower_bound > 1) ||
               (params.shaking_intensity_upper_bound < 1e-6 ||
                params.shaking_intensity_upper_bound > 1) ||
               (params.shaking_intensity_lower_bound >
                params.shaking_intensity_upper_bound)) {

                std::stringstream ss;
                ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                   << "Shaking is active (shake_interval = "
                   << control_params.shake_interval << ") and it is set as '"
                   << params.shaking_type << "'. "
                   << "However, the intensity bounds are out of range. "
                      "Should be (0.0, 1.0] and 'shaking_intensity_lower_bound "
                      "<= shaking_intensity_upper_bound' but "
                      "shaking_intensity_lower_bound = "
                   << params.shaking_intensity_lower_bound
                   << " and shaking_intensity_upper_bound = "
                   << params.shaking_intensity_upper_bound;
                throw std::runtime_error(ss.str());
            }
        }
    }

    AlgorithmStatus status;
    status.best_fitness = (
        optimization_sense == Sense::MINIMIZE?
        BRKGA::FITNESS_T_MAX :
        BRKGA::FITNESS_T_MIN
    );

    status.current_iteration = 0;
    status.last_update_iteration = 0;
    status.largest_iteration_offset = 0;

    // This is the shaking multiplier, that generates a random number
    // within the bounds given by the user. Only used during shaking.
    auto random_shaking_multiplier = std::bind(
        std::uniform_real_distribution<double>(
            params.shaking_intensity_lower_bound,
            params.shaking_intensity_upper_bound
        ),
        rng_per_thread[0]   // We just use the first RNG.
    );

    // If using shaking, we reserve some space for the indices of
    // to-be-decoded chromosomes (num. of pops X size of a pop)
    std::vector<std::pair<unsigned, unsigned>> shaken {};
    if(control_params.shake_interval > 0)
        shaken.reserve(current.size() * current[0]->chromosomes.size());

    // We add expiration time and maximum stalled iterations to the user's
    // stopping criteria. Note that if it returns true, we stop the optimization.
    if(!stopping_criteria) {
        if(logger) {
            *logger
            << "Custom stopping criteria not supplied by the user. "
            << "Using max. time = " << control_params.maximum_running_time
            << " and max. stall_offset = " << control_params.stall_offset
            << std::endl;
        }
        stopping_criteria = [](const AlgorithmStatus& /*not used*/) {
            return false;
        };
    }
    else
    if(logger) {
        *logger
        << "Using custom stopping supplied by the user and "
        << "max. time = " << control_params.maximum_running_time
        << " and max. stall_offset = " << control_params.stall_offset
        << std::endl;
    }

    if(logger) {
        *logger
        << "Using " << max_threads << " threads for decoding" << std::endl;
    }

    const auto start_time = std::chrono::system_clock::now();
    bool run = true;

    const auto local_stopping_criteria = [&]() {
        return
            (std::chrono::duration_cast<std::chrono::seconds>
             (std::chrono::system_clock::now() - start_time) >=
             control_params.maximum_running_time)
            ||
            (control_params.stall_offset > 0 &&
             status.stalled_iterations == control_params.stall_offset)
            ||
            stopping_criteria(status);
    };

    while(run) {
        status.current_iteration++;

        //----------------------------------------//
        // Run one iteration of the genetic algorithm.
        //----------------------------------------//
        evolve();

        // Number of iterations without improvement.
        status.stalled_iterations =
            status.current_iteration - status.last_update_iteration;

        // Check for improvements in the best solution.
        if(const auto fitness = getBestFitness();
           betterThan(fitness, status.best_fitness)){

            status.current_time = std::chrono::system_clock::now() - start_time;
            status.last_update_time = status.current_time;

            status.best_fitness = fitness;
            status.best_chromosome = getBestChromosome();
            status.last_update_iteration = status.current_iteration;

            if(status.largest_iteration_offset < status.stalled_iterations)
                status.largest_iteration_offset = status.stalled_iterations;

            status.stalled_iterations = 0;

            for(auto& callback : info_callbacks)
                run &= callback(status);
        }

        // NOTE: we test the stoping criteria several times because some one
        // them might be time-based criterium, and therefore, we must check
        // after each time consuming operation. For instance, let's suppose
        // we call IPR, then shake. Between the two calls, the maximum time
        // may be expired and we cannot proceed.

        //----------------------------------------//
        // Call the implicit path relinking
        // for search intersification.
        //----------------------------------------//

        if(!local_stopping_criteria() &&
           (control_params.ipr_interval > 0) &&
           (status.stalled_iterations > 0) &&
           (status.stalled_iterations % control_params.ipr_interval == 0)) {

            // OK, these numbers are also "well-known" values for IPR,
            // and they were tuned using several situations.
            std::size_t block_size = ceil(params.alpha_block_size *
                                     sqrt(params.population_size));

            if(block_size > chromosome_size)
                block_size = chromosome_size / 2;

            if(logger) {
                *logger
                << "Path relink at " << status.current_iteration << " iteration. "
                << "Block size: " << block_size << ". "
                << "Type: " << params.pr_type << ". "
                << "Distance: " << params.pr_distance_function_type << ". "
                << "Current time: " << status.current_time
                << std::endl;
            }

            status.num_path_relink_calls++;
            pr_start_time = std::chrono::system_clock::now();

            auto result = pathRelink(
                params.pr_type,
                params.pr_selection,
                params.pr_distance_function,
                params.pr_number_pairs,
                params.pr_minimum_distance,
                block_size,
                std::chrono::duration_cast<std::chrono::seconds>
                    (control_params.maximum_running_time - status.current_time),
                params.pr_percentage
            );

            status.path_relink_time +=
                std::chrono::system_clock::now() - pr_start_time;

            status.current_time = std::chrono::system_clock::now() - start_time;

            using BRKGA::PathRelinking::PathRelinkingResult;
            switch(result) {
                case PathRelinkingResult::TOO_HOMOGENEOUS:
                    status.num_homogenities++;
                    if(logger) {
                        *logger
                        << "- Populations are too too homogeneous. "
                        << "Current time: " << status.current_time
                        << std::endl;
                    }
                    break;

                case PathRelinkingResult::NO_IMPROVEMENT:
                    if(logger) {
                        *logger
                        << "- No improvement found. "
                        << "Current time: " << status.current_time
                        << std::endl;
                    }
                    break;

                case PathRelinkingResult::ELITE_IMPROVEMENT:
                    status.num_elite_improvements++;
                    if(logger) {
                        *logger
                        << "- Improvement on the elite set but not in "
                           "the best individual. "
                        << "Current time: " << status.current_time
                        << std::endl;
                    }
                    break;

                case PathRelinkingResult::BEST_IMPROVEMENT:
                    status.num_best_improvements++;
                    if(logger) {
                        *logger
                        << "- Improvement in the best individual. "
                        << "Current time: " << status.current_time
                        << std::endl;
                    }

                    // Check for improvements in the best solution.
                    if(const auto fitness = getBestFitness();
                       betterThan(fitness, status.best_fitness)){

                        status.current_time =
                            std::chrono::system_clock::now() - start_time;
                        status.last_update_time = status.current_time;

                        status.best_fitness = fitness;
                        status.best_chromosome = getBestChromosome();
                        status.last_update_iteration = status.current_iteration;

                        if(status.largest_iteration_offset < status.stalled_iterations)
                            status.largest_iteration_offset = status.stalled_iterations;

                        status.stalled_iterations = 0;

                        for(auto& callback : info_callbacks)
                            run &= callback(status);
                    }
                    break;

                default:;
            } // end switch
        }

        //----------------------------------------//
        // Should we exchange individuals?
        //----------------------------------------//
        if(!local_stopping_criteria() &&
           (control_params.exchange_interval > 0) &&
           (status.stalled_iterations > 0) &&
           (status.stalled_iterations % control_params.exchange_interval == 0)) {

            exchangeElite(params.num_exchange_individuals);
            status.num_exchanges++;
            status.current_time = std::chrono::system_clock::now() - start_time;

            if(logger) {
                *logger
                << "Exchanged " << params.num_exchange_individuals
                << " solutions from each population. "
                << "Iteration " << status.current_iteration << ". "
                << "Current time: " << status.current_time
                << std::endl;
            }
        } // End of exchange.

        //----------------------------------------//
        // Should we shake the populations?
        //----------------------------------------//

        if(!local_stopping_criteria() &&
           (control_params.shake_interval > 0) &&
           (status.stalled_iterations > 0) &&
           (status.stalled_iterations % control_params.shake_interval == 0)) {

            const double perc_intensity = random_shaking_multiplier();
            const unsigned intensity = perc_intensity * chromosome_size;

            if(params.shaking_type != ShakingType::CUSTOM) {
                shake(intensity, params.shaking_type,
                      params.num_independent_populations);
            }
            else {
                shaken.clear();
                params.custom_shaking(
                    params.shaking_intensity_lower_bound,
                    params.shaking_intensity_upper_bound,
                    current,     // The current population.
                    shaken       // Indices to be re-decoded.
                );

                // If shaken is empty, force the re-decode
                // of all chromosomes of all populations.
                // TODO: use views::enumerate when C++23 is ready.
                if(shaken.empty()) {
                    for(unsigned i = 0; i < current.size(); ++i)
                        for(unsigned j = 0; j < current[i]->chromosomes.size(); ++j)
                            shaken.push_back({i, j});
                }

                // Re-decode only the needed chromosomes.
                #ifdef _OPENMP
                    #pragma omp parallel for num_threads(max_threads) schedule(static,1)
                #endif
                for(unsigned i = 0; i < shaken.size(); ++i) {
                    const auto [pop_idx, chr_idx] = shaken[i];
                    current[pop_idx]->setFitness(
                        chr_idx,
                        decoder.decode((*current[pop_idx])(chr_idx), true)
                    );
                }

                // Now we must sort by fitness, since things might have changed.
                #ifdef _OPENMP
                    #pragma omp parallel for num_threads(max_threads) schedule(static,1)
                #endif
                for(unsigned i = 0; i < current.size(); ++i)
                    current[i]->sortFitness(optimization_sense);
            }

            if(logger) {
                if(params.shaking_type != ShakingType::CUSTOM)
                    *logger << "Shaking with intensity "
                            << perc_intensity << ". ";
                else
                    *logger << "Shaking. ";

                *logger
                << "Type " << params.shaking_type << ". "
                << "Iteration " << status.current_iteration << ". "
                << "Current time: " << status.current_time
                << std::endl;
            }

            status.num_shakes++;
        } // End of shaking.

        //----------------------------------------//
        // Time to reset?
        //----------------------------------------//
        if(!local_stopping_criteria() &&
           (control_params.reset_interval > 0) &&
           (status.stalled_iterations > 0) &&
           (status.stalled_iterations % control_params.reset_interval == 0)) {

            reset();
            status.num_resets++;
            status.current_time = std::chrono::system_clock::now() - start_time;

            if(logger) {
                *logger
                << "Reset population after " << control_params.reset_interval
                << " iterations without improvement. "
                << "Iteration " << status.current_iteration << ". "
                << "Current time: " << status.current_time
                << std::endl;
            }
        } // End of reset.

        // Check the stopping cirteria.
        run &= !local_stopping_criteria();
    }

    status.current_time = std::chrono::system_clock::now() - start_time;
    return status;
}

//----------------------------------------------------------------------------//

template <class Decoder>
PathRelinking::PathRelinkingResult BRKGA_MP_IPR<Decoder>::pathRelink(
                    PathRelinking::Type pr_type,
                    PathRelinking::Selection pr_selection,
                    std::shared_ptr<DistanceFunctionBase> dist,
                    unsigned number_pairs,
                    double minimum_distance,
                    std::size_t block_size,
                    std::chrono::seconds max_time,
                    double percentage) {

    using PR = PathRelinking::PathRelinkingResult;

    if(percentage < 1e-6 || percentage > 1.0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Percentage/size of path relinking invalid, current: "
           << percentage;
        throw std::range_error(ss.str());
    }

    if(!initialized)
        initialize();

    auto& rng = rng_per_thread[0];
    Chromosome initial_solution(chromosome_size);
    Chromosome guiding_solution(chromosome_size);

    // Perform path relinking between elite chromosomes from different
    // populations. This is done in a circular fashion.
    std::deque<std::pair<std::size_t, std::size_t>> index_pairs;

    // Keep track of the time.
    pr_start_time = std::chrono::system_clock::now();

    auto final_status = PR::TOO_HOMOGENEOUS;

    for(unsigned pop_count = 0; pop_count < params.num_independent_populations;
        ++pop_count) {
        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time);

        if(elapsed_seconds > max_time)
            break;

        unsigned pop_base = pop_count;
        unsigned pop_guide = pop_count + 1;
        bool found_pair = false;

        // If we have just one population, we take the both solution from it.
        if(params.num_independent_populations == 1) {
            pop_base = pop_guide = 0;
            pop_count = params.num_independent_populations;
        }
        // If we have two populations, perform just one path relinking.
        else if(params.num_independent_populations == 2) {
            pop_count = params.num_independent_populations;
        }

        // Do the circular thing.
        if(pop_guide == params.num_independent_populations)
            pop_guide = 0;

        index_pairs.clear();
        for(std::size_t i = 0; i < elite_size; ++i)
            for(std::size_t j = 0; j < elite_size; ++j)
                index_pairs.emplace_back(std::make_pair(i, j));

        unsigned tested_pairs_count = 0;
        if(number_pairs == 0)
            number_pairs = index_pairs.size();

        while(!index_pairs.empty() && tested_pairs_count < number_pairs &&
              elapsed_seconds < max_time) {
            const auto index =
                    (pr_selection == PathRelinking::Selection::BESTSOLUTION?
                     0 : randInt(index_pairs.size() - 1, rng));

            const auto pos1 = index_pairs[index].first;
            const auto pos2 = index_pairs[index].second;

            const auto& chr1 = current[pop_base]->
                    chromosomes[current[pop_base]->fitness[pos1].second];

            const auto& chr2 = current[pop_guide]->
                    chromosomes[current[pop_base]->fitness[pos2].second];

            if(dist->distance(chr1, chr2) >= minimum_distance - 1e-6) {
                copy(begin(chr1), end(chr1), begin(initial_solution));
                copy(begin(chr2), end(chr2), begin(guiding_solution));
                found_pair = true;
                break;
            }

            index_pairs.erase(begin(index_pairs) + index);
            ++tested_pairs_count;
            elapsed_seconds =
                std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now() - pr_start_time);
        }

        // The elite sets are too homogeneous, we cannot do
        // a good path relinking. Let's try other populations.
        if(!found_pair)
            continue;

        // Create a empty solution.
        std::pair<fitness_t, Chromosome> best_found;
        best_found.second.resize(current[0]->getChromosomeSize(), 0.0);

        const bool sense = optimization_sense == Sense::MAXIMIZE;

        best_found.first = sense? FITNESS_T_MIN : FITNESS_T_MAX;

        const auto fence = best_found.first;

        // Perform the path relinking.
        if(pr_type == PathRelinking::Type::DIRECT) {
            directPathRelink(initial_solution, guiding_solution, dist,
                             best_found, block_size, max_time, percentage);
        }
        else {
            permutatioBasedPathRelink(initial_solution, guiding_solution, dist,
                                      best_found, block_size, max_time,
                                      percentage);
        }

        final_status |= PR::NO_IMPROVEMENT;

        // **NOTE:** is fitness_t contains float types, so the comparison
        // `best_found.first == fence` may be unfase. Therefore, we use
        // helper functions that define the correct behavior at compilation
        // time.
        if(close_enough(best_found.first, fence))
            continue;

        // Re-decode and apply local search if the decoder are able to do it.
        best_found.first = decoder.decode(best_found.second, true);

        // Now, check if the best solution found is really good.
        // If it is the best, overwrite the worse solution in the population.
        bool include_in_population =
           (sense && best_found.first > current[pop_base]->fitness[0].first) ||
           (!sense && best_found.first < current[pop_base]->fitness[0].first);

        const auto best_overall = this->getBestFitness();

        if((sense && best_found.first > best_overall) ||
           (!sense && best_found.first < best_overall))
            final_status |= PR::BEST_IMPROVEMENT;

        // If not the best, but is better than the worst elite member, check
        // if the distance between this solution and all elite members
        // is at least minimum_distance.
        if(!include_in_population &&
           ((sense && best_found.first >
                        current[pop_base]->fitness[elite_size - 1].first) ||
            (!sense && best_found.first <
                        current[pop_base]->fitness[elite_size - 1].first))) {

            include_in_population = true;
            for(unsigned i = 0; i < elite_size; ++i) {
                if(dist->distance(best_found.second, current[pop_base]->
                            chromosomes[current[pop_base]->fitness[i].second])
                   < minimum_distance - 1e-6) {
                    include_in_population = false;
                    final_status |= PR::NO_IMPROVEMENT;
                    break;
                }
            }
        }

        if(include_in_population) {
            std::copy(begin(best_found.second), end(best_found.second),
                      begin(current[pop_base]->
                                chromosomes[current[pop_base]->
                                    fitness.back().second]));

            current[pop_base]->fitness.back().first = best_found.first;
            // Reorder the chromosomes.
            current[pop_base]->sortFitness(optimization_sense);
            final_status |= PR::ELITE_IMPROVEMENT;
        }
    }

    return final_status;
}

//----------------------------------------------------------------------------//

template <class Decoder>
PathRelinking::PathRelinkingResult BRKGA_MP_IPR<Decoder>::pathRelink(
            std::shared_ptr<DistanceFunctionBase> dist,
            std::chrono::seconds max_time) {

    size_t block_size = ceil(params.alpha_block_size *
                             sqrt(params.population_size));
    if(block_size > chromosome_size)
        block_size = chromosome_size / 2;

    return pathRelink(params.pr_type, params.pr_selection, dist,
                      params.pr_number_pairs, params.pr_minimum_distance,
                      block_size, max_time,
                      params.pr_percentage);
}

//----------------------------------------------------------------------------//

// This is a multi-thread version. For small chromosomes, it may be slower than
// single thread version.
template <class Decoder>
void BRKGA_MP_IPR<Decoder>::directPathRelink(
            const Chromosome& chr1, const Chromosome& chr2,
            std::shared_ptr<DistanceFunctionBase> dist,
            std::pair<fitness_t, Chromosome>& best_found,
            std::size_t block_size,
            std::chrono::seconds max_time,
            double percentage) {

    const std::size_t num_blocks =
            std::size_t(ceil((double)chr1.size() / block_size));
    const std::size_t path_size = std::size_t(percentage * num_blocks);

    // Create the set of indices to test.
    std::set<std::size_t> remaining_blocks;
    for(std::size_t i = 0; i < num_blocks; ++i)
        remaining_blocks.insert(i);
    Chromosome old_keys(chr1.size());

    struct Triple {
    public:
        Chromosome chr;
        fitness_t fitness;
        std::size_t block_index;
        Triple(): chr(), fitness(FITNESS_T_MAX), block_index(0) {}
    };

    // Allocate memory for the candidates.
    std::vector<Triple> candidates_left(num_blocks);
    std::vector<Triple> candidates_right(num_blocks);

    for(auto &cand : candidates_left)
        cand.chr.resize(chr1.size());

    for(auto &cand : candidates_right)
        cand.chr.resize(chr1.size());

    const Chromosome* base = &chr1;
    const Chromosome* guide = &chr2;
    std::vector<Triple>* candidates_base = &candidates_left;
    std::vector<Triple>* candidates_guide = &candidates_right;

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(max_threads)
    #endif
    for(std::size_t i = 0; i < candidates_left.size(); ++i)
        std::copy(begin(*base), end(*base), begin(candidates_left[i].chr));

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(max_threads)
    #endif
    for(std::size_t i = 0; i < candidates_right.size(); ++i)
        std::copy(begin(*guide), end(*guide), begin(candidates_right[i].chr));

    const bool sense = optimization_sense == Sense::MAXIMIZE;

    std::size_t iterations = 0;
    while(!remaining_blocks.empty()) {
        // Set the block of keys from the guide solution for each candidate.
        auto it_block_idx = remaining_blocks.begin();

        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            const auto block_base = (*it_block_idx) * block_size;

            const auto it_key_block1 =
                    (*candidates_base)[i].chr.begin() + block_base;
            const auto it_key_block2 = guide->begin() + block_base;

            const auto bs = (block_base + block_size > guide->size())?
                            guide->size() - block_base : block_size;

            // If these keys do not affect the solution, skip them.
            if(!dist->affectSolution(it_key_block1, it_key_block2, bs)) {
                it_block_idx = remaining_blocks.erase(it_block_idx);
                --i;
                continue;
            }

            // Save the former keys before...
            std::copy_n((*candidates_base)[i].chr.begin() + block_base, bs,
                        old_keys.begin() + block_base);

            // ... copy the keys from the guide solution.
            std::copy_n(guide->begin() + block_base, bs,
                        (*candidates_base)[i].chr.begin() + block_base);

            (*candidates_base)[i].block_index = *it_block_idx;
            ++it_block_idx;
        }

        if (remaining_blocks.empty()) {
            break;
        }

        // Decode the candidates.
        volatile bool times_up = false;
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(max_threads) shared(times_up) \
                schedule(static, 1)
        #endif
        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            if(sense)
                (*candidates_base)[i].fitness = FITNESS_T_MIN;
            else
                (*candidates_base)[i].fitness = FITNESS_T_MAX;

            if(times_up) continue;

            (*candidates_base)[i].fitness =
                    decoder.decode((*candidates_base)[i].chr, false);

            const auto elapsed_seconds =
                 std::chrono::duration_cast<std::chrono::seconds>
                 (std::chrono::system_clock::now() - pr_start_time);
            if(elapsed_seconds > max_time)
                times_up = true;
        }

        // Locate the best candidate.
        std::size_t best_index = 0;
        std::size_t best_block_index = 0;

        fitness_t best_value;
        if(sense)
            best_value = FITNESS_T_MIN;
        else
            best_value = FITNESS_T_MAX;

        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            if((best_value < (*candidates_base)[i].fitness && sense) ||
               (best_value > (*candidates_base)[i].fitness && !sense)) {
                best_block_index = (*candidates_base)[i].block_index;
                best_value = (*candidates_base)[i].fitness;
                best_index = i;
            }
        }

        // Hold it, if it is the best found until now.
        if((sense && best_found.first < (*candidates_base)[best_index].fitness)
           ||
           (!sense && best_found.first > (*candidates_base)[best_index].fitness)
           ) {
            best_found.first = (*candidates_base)[best_index].fitness;
            std::copy(begin((*candidates_base)[best_index].chr),
                      end((*candidates_base)[best_index].chr),
                      begin(best_found.second));
        }

        // Restore original keys and copy the block of keys for all future
        // candidates. The last candidate will not be used.
        it_block_idx = remaining_blocks.begin();
        for(std::size_t i = 0; i < remaining_blocks.size() - 1;
            ++i, ++it_block_idx) {

            auto block_base = (*it_block_idx) * block_size;
            auto bs = (block_base + block_size > guide->size())?
                      guide->size() - block_base : block_size;

            std::copy_n(old_keys.begin() + block_base, bs,
                        (*candidates_base)[i].chr.begin() + block_base);

            // Recompute the offset for the best block.
            block_base = best_block_index * block_size;
            bs = (block_base + block_size > guide->size())?
                 guide->size() - block_base : block_size;

            std::copy_n((*candidates_base)[best_index].chr.begin() + block_base,
                        bs, (*candidates_base)[i].chr.begin() + block_base);
        }

        std::swap(base, guide);
        std::swap(candidates_base, candidates_guide);
        remaining_blocks.erase(best_block_index);

        const auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time);

        if((elapsed_seconds > max_time) || (iterations++ > path_size))
            break;
    } // end while
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::permutatioBasedPathRelink(
                Chromosome& chr1, Chromosome& chr2,
                std::shared_ptr<DistanceFunctionBase> /*non-used*/,
                std::pair<fitness_t, Chromosome>& best_found,
                std::size_t /*non-used block_size*/,
                std::chrono::seconds max_time,
                double percentage) {

    const std::size_t path_size = std::size_t(percentage * chromosome_size);

    std::set<std::size_t> remaining_indices;
    for(std::size_t i = 0; i < chr1.size(); ++i)
        remaining_indices.insert(i);

    struct DecodeStruct {
    public:
        Chromosome chr;
        fitness_t fitness;
        std::size_t key_index;
        std::size_t pos1;
        std::size_t pos2;
        DecodeStruct(): chr(), fitness(FITNESS_T_MAX),
                        key_index(0), pos1(0), pos2(0) {}
    };

    // Allocate memory for the candidates.
    std::vector<DecodeStruct> candidates_left(chr1.size());
    std::vector<DecodeStruct> candidates_right(chr1.size());

    for(auto& cand : candidates_left)
        cand.chr.resize(chr1.size());

    for(auto& cand : candidates_right)
        cand.chr.resize(chr1.size());

    Chromosome* base = &chr1;
    Chromosome* guide = &chr2;
    std::vector<DecodeStruct>* candidates_base = &candidates_left;
    std::vector<DecodeStruct>* candidates_guide = &candidates_right;

    std::vector<std::size_t> chr1_indices(chr1.size());
    std::vector<std::size_t> chr2_indices(chr1.size());
    std::vector<std::size_t>* base_indices = &chr1_indices;
    std::vector<std::size_t>* guide_indices = &chr2_indices;

    // Create and order the indices.
    std::vector<std::pair<double, std::size_t>> sorted(chr1.size());

    for(unsigned j = 0; j < 2; ++j) {
        for(std::size_t i = 0; i < base->size(); ++i)
            sorted[i] = std::pair<double, std::size_t>((*base)[i], i);

        std::sort(begin(sorted), end(sorted));
        for(std::size_t i = 0; i < base->size(); ++i)
            (*base_indices)[i] = sorted[i].second;

        swap(base, guide);
        swap(base_indices, guide_indices);
    }

    base = &chr1;
    guide = &chr2;
    base_indices = &chr1_indices;
    guide_indices = &chr2_indices;

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(max_threads)
    #endif
    for(std::size_t i = 0; i < candidates_left.size(); ++i) {
        std::copy(begin(*base), end(*base), begin(candidates_left[i].chr));
    }

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(max_threads)
    #endif
    for(std::size_t i = 0; i < candidates_right.size(); ++i) {
        std::copy(begin(*guide), end(*guide), begin(candidates_right[i].chr));
    }

    const bool sense = optimization_sense == Sense::MAXIMIZE;

    std::size_t iterations = 0;
    while(!remaining_indices.empty()) {
        std::size_t position_in_base;
        std::size_t position_in_guide;

        auto it_idx = remaining_indices.begin();
        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            position_in_base = (*base_indices)[*it_idx];
            position_in_guide = (*guide_indices)[*it_idx];

            if(position_in_base == position_in_guide) {
                it_idx = remaining_indices.erase(it_idx);
                --i;
                continue;
            }

            (*candidates_base)[i].key_index = *it_idx;
            (*candidates_base)[i].pos1 = position_in_base;
            (*candidates_base)[i].pos2 = position_in_guide;

            if(sense)
                (*candidates_base)[i].fitness = FITNESS_T_MIN;
            else
                (*candidates_base)[i].fitness = FITNESS_T_MAX;
            ++it_idx;
        }

        if(remaining_indices.size() == 0)
            break;

        // Decode the candidates.
        volatile bool times_up = false;
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(max_threads) shared(times_up) \
                schedule(static, 1)
        #endif
        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            if(times_up) continue;

            std::swap((*candidates_base)[i].chr[(*candidates_base)[i].pos1],
                      (*candidates_base)[i].chr[(*candidates_base)[i].pos2]);

            (*candidates_base)[i].fitness =
                    decoder.decode((*candidates_base)[i].chr, false);

            std::swap((*candidates_base)[i].chr[(*candidates_base)[i].pos1],
                      (*candidates_base)[i].chr[(*candidates_base)[i].pos2]);

            const auto elapsed_seconds =
                    std::chrono::duration_cast<std::chrono::seconds>
                    (std::chrono::system_clock::now() - pr_start_time);
            if(elapsed_seconds > max_time)
                times_up = true;
        }

        // Locate the best candidate
        std::size_t best_key_index = 0;
        std::size_t best_index = 0;

        fitness_t best_value;
        best_value = sense? FITNESS_T_MIN : FITNESS_T_MAX;

        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            if((best_value < (*candidates_base)[i].fitness && sense) ||
               (best_value > (*candidates_base)[i].fitness && !sense)) {
                best_index = i;
                best_key_index = (*candidates_base)[i].key_index;
                best_value = (*candidates_base)[i].fitness;
            }
        }

        position_in_base = (*base_indices)[best_key_index];
        position_in_guide = (*guide_indices)[best_key_index];

        // Commit the best exchange in all candidates.
        // The last will not be used.
        for(std::size_t i = 0; i < remaining_indices.size() - 1; ++i) {
            std::swap((*candidates_base)[i].chr[position_in_base],
                      (*candidates_base)[i].chr[position_in_guide]);
        }

        std::swap((*base_indices)[position_in_base],
                  (*base_indices)[position_in_guide]);

        // Hold, if it is the best found until now
        if((sense && best_found.first < best_value) ||
           (!sense && best_found.first > best_value)) {
            const auto& best_chr = (*candidates_base)[best_index].chr;
            best_found.first = best_value;
            copy(begin(best_chr), end(best_chr), begin(best_found.second));
        }

        std::swap(base_indices, guide_indices);
        std::swap(candidates_base, candidates_guide);
        remaining_indices.erase(best_key_index);

        // Is time to stop?
        const auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time);

        if((elapsed_seconds > max_time) || (iterations++ > path_size))
            break;
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline double BRKGA_MP_IPR<Decoder>::rand01(std::mt19937& rng) {
    // **NOTE:** instead to use std::generate_canonical<> (which can be
    // a little bit slow), we may use
    // rng() * (1.0 / std::numeric_limits<std::mt19937::result_type>::max());
    // However, this approach has some precision problems on some platforms
    // (notably Linux).
    using namespace std;
    return generate_canonical<double, numeric_limits<double>::digits>(rng);
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline unsigned BRKGA_MP_IPR<Decoder>::randInt(const unsigned n,
                                               std::mt19937& rng) {
    return integer_distribution(
        rng, typename decltype(integer_distribution)::param_type (0, n)
    );
}
/// \endcond IGNORE_IMPLEMENTATION
} // end namespace BRKGA

#endif // BRKGA_MP_IPR_HPP_
