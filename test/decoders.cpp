#include "decoders.hpp"

#include <iostream>
using namespace std;

double Sum_Decoder::decode(BRKGA::Chromosome& chromosome, bool /*non-used*/) {
    // Just sum the values.
    double total = 0.0;
    for(const auto &v : chromosome)
        total += v;

    return total;
}

double Order_Decoder::decode(BRKGA::Chromosome& chromosome, bool /*non-use*/) {
    // Just sum the values.
    double total = 0.0;
    double last = chromosome.front();
    for(const auto &v : chromosome) {
        if(last < v)
            total += 1.0;
        last = v;
    }

    return total;
}
