/******************************************************************************
 * test_custom_bias_function.cpp: test bias functions.
 *
 * (c) Copyright 2015-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Jan 06, 2015 by andrade
 *  Last update: Sep 07, 2023 by andrade
 *
 * This code is released under LICENSE.md.
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
 *****************************************************************************/

#include "brkga_mp_ipr.hpp"
#include "decoders.hpp"

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;
using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    try {
        const unsigned chr_size = 1000;
        const unsigned seed = 270001;
        const bool evolutionary_mechanism_on = true;
        const unsigned max_threads = 1;

        auto [brkga_params, _] = readConfiguration("config_full.conf");

        // The BRKGA_MP_IPR algorithm object.
        Sum_Decoder decoder;
        BRKGA_MP_IPR<Sum_Decoder> algorithm(decoder, BRKGA::Sense::MINIMIZE,
            seed, chr_size, brkga_params, evolutionary_mechanism_on,
            max_threads
        );

        cout
        << "\nAlgorithm bias function type: "
        << algorithm.getBrkgaParams().bias_type << endl;

        cout << "LOG";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / log1p(r); }
        );
        cout << endl;

        cout << "LINEAR";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / r; }
        );
        cout << endl;

        cout << "QUADRATIC";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -2); }
        );
        cout << endl;

        cout << "CUBIC";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -3); }
        );
        cout << endl;

        cout << "EXPONENTIAL";
        algorithm.setBiasCustomFunction(
            [](const unsigned r) { return exp(-1.0 * r); }
        );
        cout << endl;

        cout << "CONSTANT";
        algorithm.setBiasCustomFunction(
            [&](const unsigned) { return 1.0 / brkga_params.total_parents; }
        );
        cout << endl;

        cout << "ERROR";
        algorithm.setBiasCustomFunction(
            [&](const unsigned r) { return 2.0 * r; }
        );
        cout << endl;
    }
    catch(exception& e) {
        cerr << "\n***********************************************************"
             << "\n****  Exception Occured: " << e.what()
             << "\n***********************************************************"
             << endl;
        return 70; // BSD software internal error code
    }
    return 0;
}
