/******************************************************************************
 * test_run.cpp: test method "run()".
 *
 * (c) Copyright 2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Sep 07, 2023 by ceandrade
 *  Last update: Sep 13, 2023 by ceandrade
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
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    if(argc < 3) {
        cerr << "Usage: "<< argv[0]
             << " <chromosome-size> <seed>" << endl;
        return 1;
    }

    try {
        const unsigned chr_size = atoi(argv[1]);
        const unsigned seed = atoi(argv[2]);
        const bool evolutionary_mechanism_on = true;
        const unsigned max_threads = 1;

        auto [brkga_params, control_params] = readConfiguration("config_full.conf");
        // control_params.maximum_running_time = 5s;
        // brkga_params.shaking_intensity_lower_bound = 0.7;
        // brkga_params.shaking_intensity_upper_bound = 0.3;
        // control_params.maximum_running_time =
        //     decltype(ControlParams::maximum_running_time)::max();
        // control_params.stall_offset = 1000;

        using MyDecoder = SumDecoder;
        // using MyDecoder = Order_Decoder;
        MyDecoder decoder;

        // The BRKGA_MP_IPR algorithm object.
        BRKGA_MP_IPR<MyDecoder> algorithm(
            decoder, BRKGA::Sense::MAXIMIZE, seed, chr_size, brkga_params,
            max_threads, evolutionary_mechanism_on
        );

        // A random number generator.
        std::mt19937 rng(2700001);
        rng.discard(rng.state_size);

        // Change some values from elite chromosomes from all populations.
        // Similar to a standard mutation.
        algorithm.setShakingMethod(
            [&](double lower_bound, double upper_bound,
                std::vector<std::shared_ptr<Population>>& populations,
                std::vector<std::pair<unsigned, unsigned>>& shaken) {

                // Determines whether we change the allel or not.
                std::bernoulli_distribution must_change(0.50);

                // Determines the value of the allele.
                std::uniform_real_distribution<> allele_value(lower_bound, upper_bound);

                for(unsigned pop_idx = 0; pop_idx < populations.size(); ++pop_idx) {
                    auto& chromosomes = populations[0]->chromosomes;
                    for(unsigned chr_idx = 0; chr_idx < chromosomes.size(); ++chr_idx) {
                        auto& chromosome = chromosomes[chr_idx];

                        bool change = false;
                        for(unsigned i = 0; i < chromosome.size(); ++i) {
                            if(must_change(rng)) {
                                chromosome[i] = allele_value(rng);
                                change = true;
                            }
                        }

                        if(change)
                            shaken.push_back({pop_idx, chr_idx});
                    }
                }
            }
        );

        algorithm.addNewSolutionObserver([](const AlgorithmStatus& status) {
            cout
            << "> Iter: " << status.current_iteration
            << " | solution: " << status.best_fitness
            << " | time: " << status.current_time
            << endl;

            return true;    // Don't stop the optimization.
        });

        algorithm.setStoppingCriteria(
            [](const AlgorithmStatus& status) {
                return
                    // No updates for 100 iterations...
                    (status.stalled_iterations == 1000)
                     ||
                    // ... or maximum time of 10 seconds.
                    (status.current_time > 60s);
            }
        );

        auto final_status = algorithm.run(control_params, &cout);

        cout
        << final_status
        << "\ntest: " << final_status.current_time
        << endl;

    }
    catch(exception& e) {
        cerr << "\n***********************************************************"
             << "\n**** Exception Occured: " << e.what()
             << "\n***********************************************************"
             << endl;
        return 1;
    }
    return 0;
}
