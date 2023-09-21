/******************************************************************************
 * main_complete.cpp: complete code for calling BRKGA algorithms to solve
 *                    instances of the Traveling Salesman Problem.
 *
 * Created on : Mar 06, 2019 by ceandrade
 * Last update: Sep 20, 2023 by ceandrade
 *
 * (c) Copyright 2015-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
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

// We may choose the simple thread-safe decoder...
//#include "decoders/tsp_decoder.hpp"
//... or the pre-allocating thread-safe decoder.
#include "decoders/tsp_decoder_pre_allocating.hpp"

#include "heuristics/greedy_tour.hpp"
#include "brkga_mp_ipr.hpp"

#include "third_part/docopt/docopt.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

using namespace std;
using namespace std::chrono;

//-------------------------[ Some control constants ]-------------------------//

// Controls stop criteria.
enum class StopRule {
    GENERATIONS = 'G',
    IMPROVEMENT = 'I',
    UNKNOWN = 'U',
};

//-------------------------[ Simple logging function ]------------------------//

void log(const string& message) {
    auto start_time = system_clock::to_time_t(system_clock::now());
    string ss(ctime(&start_time));
    ss.pop_back();  // Workaround to skip unwanted end-of-line.
    cout << "\n[" << ss << "] " << message << endl;
}

//--------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    const std::string usage =
    R"(
Usage:
  main_complete
        --config <config_file>
        --seed <seed>
        --stop_rule <stop_rule>
        --stop_arg <stop_arg>
        --maxtime <max_time>
        --instance <instance_file>
        [--threads <num_threads>]
        [--no_evolution]
  main_complete (-h | --help)

Options:
  --config <arg>     Text file with the BRKGA-MP-IPR parameters.
  --seed <arg>       Seed for the random number generator.
  --stop_rule <arg>  Stop rule where:
                     - (G)enerations: number of evolutionary generations.
                     - (I)terations: maximum number of generations without
                       improvement in the solutions.
  --stop_arg <arg>     Argument value for the stopping rule.
  --maxtime <arg>      Maximum time in seconds.
  --instance <arg>     Instance file.
  --threads <arg>    Number of threads to be used during parallel decoding.
                     It must in the range [1, 64] [default: 1].
  --no_evolution     If supplied, no evolutionary operators are applied.
                     So, the algorithm becomes a simple multi-start algorithm.
  -h --help          Produce help message.
)";

    string config_file;
    unsigned seed = 0;
    StopRule stop_rule = StopRule::UNKNOWN;
    unsigned stop_arg = 0;
    chrono::seconds max_time {0};
    string instance_file;
    unsigned num_threads = 1;
    bool perform_evolution = true;

    // Parse the command line arguments.
    try {
        auto args = docopt::docopt(usage, { argv + 1, argv + argc }, true);

        #ifdef DEBUG
        for(auto& [key, value] : args)
            std::cout << "\n"<< key << " : " << value;
        #endif

        config_file = args["--config"].asString();
        seed = static_cast<unsigned>(args["--seed"].asLong());
        stop_rule = static_cast<StopRule>(
            toupper(args["--stop_rule"].asString()[0])
        );
        stop_arg = static_cast<unsigned>(args["--stop_arg"].asLong());
        max_time = chrono::seconds {args["--maxtime"].asLong()};
        instance_file = args["--instance"].asString();
        num_threads = static_cast<unsigned> (args["--threads"].asLong());
        perform_evolution = !args["--no_evolution"].asBool();

        if(stop_rule != StopRule::GENERATIONS &&
           stop_rule != StopRule::IMPROVEMENT)
            throw std::logic_error("Incorrect stop rule. Must be either "
                                   "(G)enerations or (I)terations");

        using namespace std::chrono_literals;
        if(max_time <= 0s) {
            stringstream ss;
            ss << "'maxtime' must be > 0. Given '" << max_time << "'";
            throw std::logic_error(ss.str());
        }

        if(num_threads == 0 || num_threads > 64) {
            stringstream ss;
            ss
            << "'num_threads' must be in [1, 64]. Given '"
            << num_threads << "'";
            throw std::logic_error(ss.str());
        }
    }
    catch(exception& e) {
        cerr
        << '\n' << string(40, '*') << "\n"
        << "ERROR: " << e.what()
        << ". Please use -h/--help for correct usage.\n"
        << string(40, '*')
        << endl;
        return 1;
    }

    // Main algorithm.
    try {
        log("Experiment started");

        cout
        << "> Instance: '" << instance_file << "'"
        << "\n> Loading config file: '" << config_file << "'"
        << endl;

        // Load config file and show basic info.
        auto [brkga_params, control_params] =
            BRKGA::readConfiguration(config_file);

        // Let's overwrite the maximum time and the stall iterations.
        control_params.maximum_running_time = max_time;
        if(stop_rule == StopRule::IMPROVEMENT)
            control_params.stall_offset = stop_arg;

        cout
        << "> Algorithm parameters:\n"
        << brkga_params
        << "> Control parameters:\n"
        << control_params
        << "\n> Seed: " << seed
        << "\n> Stop rule: "
        << (stop_rule == StopRule::GENERATIONS ? "Generations" : "Improvement")
        << "\n> Stop argument: " << stop_arg
        << "\n> Number of threads for decoding: " << num_threads;
        if(!perform_evolution)
            cout << "\n> Simple multi-start: on (no evolutionary operators)";
        cout << endl;

        ////////////////////////////////////////
        // Load instance and create an initial solution.
        ////////////////////////////////////////
        log("Reading TSP data");

        auto instance = TSP_Instance(instance_file);
        cout << "Number of nodes: " << instance.num_nodes << endl;

        log("Generating initial tour");

        // Generate a greedy solution to be used as warm start for BRKGA.
        auto initial_solution = greedy_tour(instance);
        cout << "Initial cost: " << initial_solution.first << endl;

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////
        log("Building BRKGA");

        brkga_params.population_size =
            min(brkga_params.population_size, 10 * instance.num_nodes);

        const auto chromosome_size = instance.num_nodes;

        cout
        << "New population size: " << brkga_params.population_size << "\n"
        << "Chromosome size: " << chromosome_size
        << endl;

        // We may choose the simple thread-safe decoder...
        // using LocalDecoder = TSP_Decoder;
        // TSP_Decoder decoder(instance);

        //... or the pre-allocating thread-safe decoder.
        using LocalDecoder = TSP_Decoder_pre_allocating;
        LocalDecoder decoder(instance, num_threads);

        BRKGA::BRKGA_MP_IPR<LocalDecoder> algorithm(
            decoder, BRKGA::Sense::MINIMIZE, seed, chromosome_size,
            brkga_params, num_threads, perform_evolution
        );

        // We add a simple callback to track the best solution evolution.
        algorithm.addNewSolutionObserver(
            [](const BRKGA::AlgorithmStatus& status) {
                cout
                << "* " << status.current_iteration << " | "
                << get<0>(status.best_fitness) << " | "
                << get<1>(status.best_fitness) << " | "
                << status.current_time
                << endl;
                return true;    // Don't stop the optimization.
            }
        );

        // Add a callback if we need to stop using the number of generations.
        if(stop_rule == StopRule::GENERATIONS) {
            algorithm.setStoppingCriteria(
                [&](const BRKGA::AlgorithmStatus& status) {
                    return status.current_iteration == stop_arg;
                }
            );
        }

        ////////////////////////////////////////
        // Injecting the initial/incumbent solution
        ////////////////////////////////////////
        log("Injecting initial solution");

        { // local scope to deallocate memory.
        // To inject the initial tour, we need to create chromosome representing
        // that solution. First, we create a set of keys to be used in the
        // chromosome.
        std::mt19937 rng(seed);
        vector<double> keys(chromosome_size);
        for(auto& key : keys)
            key = generate_canonical<double, numeric_limits<double>::digits>(rng);

        sort(keys.begin(), keys.end());

        // Then, we visit each node in the tour and assign to it a key.
        BRKGA::Chromosome initial_chromosome(chromosome_size);
        auto& initial_tour = initial_solution.second;
        for(size_t i = 0; i < keys.size(); i++)
            initial_chromosome[initial_tour[i]] = keys[i];

        algorithm.setInitialPopulation(
            vector<BRKGA::Chromosome>(1, initial_chromosome));
        } // end local scope

        ////////////////////////////////////////
        // Optimizing
        ////////////////////////////////////////
        log("Optimizing...");
        cout << "* Iteration | Cost | LargestEdge | CurrentTime" << endl;

        const auto final_status = algorithm.run(control_params, &cout);

        log("End of optimization");
        cout << "\n> Final status:" << final_status << endl;

        ////////////////////////////////////////
        // Extracting the best tour
        ////////////////////////////////////////

        vector<pair<double, unsigned>> tour(instance.num_nodes);
        for(unsigned i = 0; i < instance.num_nodes; ++i)
            tour[i] = make_pair(final_status.best_chromosome[i], i);

        sort(tour.begin(), tour.end());

        // We need to import operator<< to print tuples.
        using BRKGA::operator<<;

        cout
        << "\n% Best tour cost, Largest Edge: "
        << setiosflags(ios::fixed) << setprecision(0)
        << final_status.best_fitness
        << "\n% Best tour: ";
        for(const auto& kv : tour)
            cout << kv.second << " ";

        string instance_name(instance_file);
        size_t pos = instance_name.rfind('/');
        if(pos != string::npos)
                instance_name = instance_name.substr(pos + 1);

        pos = instance_name.rfind('.');
        if(pos != string::npos)
                instance_name = instance_name.substr(0, pos);

        // We print a one-liner so that we can extract the log's last line
        // and insert it on a table / CSV file.
        cout <<
        "\n\nInstance,"
        "Seed,"
        "Cost,"
        "LargestEdge,"
        "NumNodes,"
        "TotalIterations,"
        "LastUpdateIteration,"
        "TotalTime,"
        "LastUpdateTime,"
        "LargestIterationOffset,"
        "StalledIterations,"
        "PRTime,"
        "PRCalls,"
        "PRNumHomogenities,"
        "PRNumPrImprovBest,"
        "PRNumImprovElite,"
        "NumExchanges,"
        "NumShakes,"
        "NumResets"
        << endl;

        // For timers, we use count() to get the absolute number with unit indicator.
        cout
        << instance_name << ","
        << seed << ","
        << setiosflags(ios::fixed) << setprecision(0)
        << get<0>(final_status.best_fitness) << ","
        << get<1>(final_status.best_fitness) << ","
        << instance.num_nodes << ","
        << final_status.current_iteration << ","
        << final_status.last_update_iteration << ","
        << setiosflags(ios::fixed) << setprecision(2)
        << final_status.current_time.count() << ","
        << final_status.last_update_time.count() << ","
        << setiosflags(ios::fixed) << setprecision(0)
        << final_status.largest_iteration_offset << ","
        << final_status.stalled_iterations << ","
        << setiosflags(ios::fixed) << setprecision(2)
        << final_status.path_relink_time.count() << ","
        << setiosflags(ios::fixed) << setprecision(0)
        << final_status.num_path_relink_calls << ","
        << final_status.num_homogenities << ","
        << final_status.num_best_improvements << ","
        << final_status.num_elite_improvements << ","
        << final_status.num_exchanges << ","
        << final_status.num_shakes << ","
        << final_status.num_resets;
        cout.flush();
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
