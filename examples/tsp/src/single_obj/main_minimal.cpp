/******************************************************************************
 * main_minimal.cpp: minimal code for calling BRKGA algorithms to solve
 *                   instances of the Traveling Salesman Problem.
 *
 * (c) Copyright 2015-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * Created on : Mar 05, 2019 by ceandrade
 * Last update: Sep 20, 2023 by ceandrade
 *
 * This code is released under BRKGA-MP-IPR License:
 * https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
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

#include "tsp/tsp_instance.hpp"
#include "decoders/tsp_decoder.hpp"
#include "brkga_mp_ipr.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    if(argc < 4) {
        cerr
        << "Usage: " << argv[0]
        << " <seed> <config-file> <maximum-running-time>"
        << " <tsp-instance-file>"
        << endl;
        return 1;
    }

    try {
        ////////////////////////////////////////
        // Read command-line arguments and the instance
        ////////////////////////////////////////

        const unsigned seed = stoi(argv[1]);
        const string config_file = argv[2];
        const string instance_file = argv[4];
        const unsigned num_threads = 4;

        cout << "Reading data..." << endl;
        auto instance = TSP_Instance(instance_file);

        ////////////////////////////////////////
        // Read algorithm parameters
        ////////////////////////////////////////

        cout << "Reading parameters..." << endl;

        auto [brkga_params, control_params] =
            BRKGA::readConfiguration(config_file);

        // Overwrite the maximum time from the config file.
        control_params.maximum_running_time = chrono::seconds {stoi(argv[3])};

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////

        cout << "Building BRKGA data and initializing..." << endl;

        TSP_Decoder decoder(instance);

        BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
            decoder, BRKGA::Sense::MINIMIZE, seed,
            instance.num_nodes, brkga_params, num_threads
        );

        ////////////////////////////////////////
        // Find good solutions / evolve
        ////////////////////////////////////////

        cout << "Running for " << control_params.maximum_running_time << "..."
             << endl;

        const auto final_status = algorithm.run(control_params, &cout);

        cout
        << "\nAlgorithm status: " << final_status
        << "\n\nBest cost: " << final_status.best_fitness
        << endl;
    }
    catch(exception& e) {
        cerr
        << "\n" << string(40, '*') << "\n"
        << "Exception Occurred: " << e.what()
        << "\n" << string(40, '*')
        << endl;
        return 1;
    }
    return 0;
}
