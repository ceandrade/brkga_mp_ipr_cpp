/******************************************************************************
 * main_minimal.cpp: minimal code for calling BRKGA algorithms to solve
 *                   instances of the Traveling Salesman Problem.
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Mar 06, 2019 by andrade
 *  Last update: May 03, 2019 by andrade
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

#include "tsp/tsp_instance.hpp"

// We may choose the simple thread-safe decoder...
//#include "decoders/tsp_decoder.hpp"
//... or the pre-allocating thread-safe decoder.
#include "decoders/tsp_decoder_pre_allocating.hpp"

#include "heuristics/greedy_tour.hpp"
#include "brkga_mp_ipr.hpp"

#define DOCOPT_HEADER_ONLY
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
    TARGET = 'T',
    UNKNOWN = 'U',
};

//-------------------------[ Simple logging function ]------------------------//

void log(const string& message) {
    auto start_time = system_clock::to_time_t(system_clock::now());
    string ss(ctime(&start_time));
    ss.pop_back();  // Workaround to skip unwanted end-of-line.
    cout << "\n[" << ss << "] " << message << endl;
}

//-------------------------[ Compute the elapse time ]------------------------//

double elapsedFrom(const time_point<system_clock>& start_time) {
    return chrono::duration_cast<milliseconds>(
        system_clock::now() - start_time
    ).count() / 1000.0;
}

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    const std::string USAGE =
    R"(Usage:
  main_complete -c <config_file> -s <seed> -r <stop_rule> -a <stop_arg> -t <max_time> -i <instance_file> [-n <num_threads>] [--no_evolution]
  main_complete (-h | --help)

Options:

    -c <config_file>    Text file with the BRKGA-MP-IPR parameters.

    -s <seed>           Seed for the random number generator.

    -r <stop_rule>      Stop rule where:
                        - (G)enerations: number of evolutionary generations.
                        - (I)terations: maximum number of generations without
                          improvement in the solutions.
                        - (T)arget: runs until obtains the target value.

    -a <stop_arg>       Argument value for '-r'.

    -t <max_time>       Maximum time in seconds.

    -i <instance_file>  Instance file.

    -n <num_threads>    NUmber of threads to be used during parallel decoding.

    --no_evolution      If supplied, no evolutionary operators are applied. So,
                        the algorithm becomes a simple multi-start algorithm.

    -h --help           Produce help message.
 )";

    string config_file;
    unsigned seed = 0;
    StopRule stop_rule = StopRule::UNKNOWN;
    double stop_arg = 0.0;
    double max_time = 0.0;
    string instance_file;
    unsigned num_threads = 1;
    bool perform_evolution = true;

    // Parse the command line arguments.
    try {
        auto args = docopt::docopt(USAGE, { argv + 1, argv + argc }, true);

        config_file = args["<config_file>"].asString();
        seed = static_cast<unsigned>(args["<seed>"].asLong());
        stop_rule = static_cast<StopRule>(toupper(args["<stop_rule>"].
                                          asString()[0]));
        stop_arg = stod(args["<stop_arg>"].asString());
        max_time = stod(args["<max_time>"].asString());
        instance_file = args["<instance_file>"].asString();
        perform_evolution = !args["--no_evolution"].asBool();

        if(args["-n"].asBool()) {
            num_threads = static_cast<unsigned>(args["<num_threads>"].asLong());
        }

        if(stop_rule != StopRule::GENERATIONS &&
           stop_rule != StopRule::IMPROVEMENT &&
           stop_rule != StopRule::TARGET)
            throw std::logic_error("Incorrect stop rule. Must be either "
                                   "(G)enerations, (I)terations, or (T)arget");

        stringstream ss;
        if(stop_rule != StopRule::TARGET && stop_arg < 1e-6) {
            ss << "when using either 'Generations' or 'Improvement' rules, "
               << "stop_arg must be positive number. Given '"
               << stop_arg << "'";
            throw std::logic_error(ss.str());
        }

        if(max_time < 1e-6) {
            ss << "max_time must be > 0. Given '" << max_time << "'";
            throw std::logic_error(ss.str());
        }
    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what()
                  << ". Please use -h/--help for correct usage.";
        return 64;  // BSD usage error code.
    }
    catch(...) {
        std::cerr << "Unknown error!" << "\n";
        return 64;  // BSD usage error code.
    }

    /////////////////////////////////////////
    // Load config file and show basic info.
    /////////////////////////////////////////

    auto params = BRKGA::readConfiguration(config_file);
    auto& brkga_params = params.first;
    auto& control_params = params.second;

    // C++17 syntax.
    // auto [brkga_params, control_params] =
    //     BRKGA::readConfiguration(config_file)

    // Main algorithm.
    try {
        { // Local scope for start_time.
        auto start_time = system_clock::to_time_t(system_clock::now());

        cout <<
        "------------------------------------------------------\n" <<
        "> Experiment started at " << ctime(&start_time) <<
        "> Instance: " << instance_file <<
        "\n> Configuration: " << config_file <<
        "\n> Algorithm Parameters";
        }

        if(!perform_evolution)
            cout << ">    - Simple multi-start: on (no evolutionary operators)";
        else {
            cout <<
            "\n>   - population_size: " << brkga_params.population_size <<
            "\n>   - elite_percentage: " << brkga_params.elite_percentage <<
            "\n>   - mutants_percentage: " << brkga_params.mutants_percentage <<
            "\n>   - num_elite_parents: " << brkga_params.num_elite_parents <<
            "\n>   - total_parents: " << brkga_params.total_parents <<
            "\n>   - bias_type: " << brkga_params.bias_type <<
            "\n>   - num_independent_populations: " <<
                brkga_params.num_independent_populations <<
            "\n>   - pr_number_pairs: " << brkga_params.pr_number_pairs <<
            "\n>   - pr_minimum_distance: " <<
                brkga_params.pr_minimum_distance <<
            "\n>   - pr_type: " << brkga_params.pr_type <<
            "\n>   - pr_selection: " << brkga_params.pr_selection <<
            "\n>   - alpha_block_size: " << brkga_params.alpha_block_size <<
            "\n>   - pr_percentage: " << brkga_params.pr_percentage <<
            "\n>   - exchange_interval: " << control_params.exchange_interval <<
            "\n>   - num_exchange_indivuduals: " <<
                control_params.num_exchange_indivuduals <<
            "\n>   - reset_interval: " << control_params.reset_interval;
        }

        cout <<
        "\n> Seed: " << seed <<
        "\n> Stop rule: " <<
            (stop_rule == StopRule::GENERATIONS ? "Generations" :
             (stop_rule == StopRule::TARGET ? "Target" : "Improvement")) <<
        "\n> Stop argument: " << stop_arg <<
        "\n> Maximum time (s): " << max_time <<
        "\n> Number of parallel threads for decoding: " << num_threads <<
        "\n------------------------------------------------------" << endl;

        ////////////////////////////////////////
        // Load instance and adjust BRKGA parameters
        ////////////////////////////////////////
        log("Reading TSP data...");

        auto instance = TSP_Instance(instance_file);
        cout << "Number of nodes: " << instance.num_nodes << endl;

        log("Generating initial tour...");

        // Generate a greedy solution to be used as warm start for BRKGA.
        auto initial_solution = greedy_tour(instance);
        cout << "Initial cost: " << initial_solution.first << endl;

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////
        log("Building BRKGA...");

        brkga_params.population_size = min(brkga_params.population_size,
                                           10 * instance.num_nodes);
        cout << "New population size: " << brkga_params.population_size << endl;

        // We may choose the simple thread-safe decoder...
        // typedef TSP_Decoder LocalDecoder;
        // TSP_Decoder decoder(instance);

        //... or the pre-allocating thread-safe decoder.
        typedef TSP_Decoder_pre_allocating LocalDecoder;
        LocalDecoder decoder(instance, num_threads);

        BRKGA::BRKGA_MP_IPR<LocalDecoder> algorithm(
                decoder, BRKGA::Sense::MINIMIZE, seed, instance.num_nodes,
                brkga_params, num_threads, perform_evolution);

        ////////////////////////////////////////
        // Injecting the initial/incumbent solution
        ////////////////////////////////////////
        log("Injecting initial solution...");

        { // local scope to deallocate memory.
        // To inject the initial tour, we need to create chromosome representing
        // that solution. First, we create a set of keys to be used in the
        // chromosome.
        std::mt19937 rng(seed);
        vector<double> keys(instance.num_nodes);
        for(auto& key : keys)
            key = generate_canonical<double,
                                     numeric_limits<double>::digits>(rng);

        sort(keys.begin(), keys.end());

        // Then, we visit each node in the tour and assign to it a key.
        BRKGA::Chromosome initial_chromosome(instance.num_nodes);
        auto& initial_tour = initial_solution.second;
        for(size_t i = 0; i < keys.size(); i++)
            initial_chromosome[initial_tour[i]] = keys[i];

        algorithm.setInitialPopulation(
            vector<BRKGA::Chromosome>(1, initial_chromosome));
        }

        ////////////////////////////////////////
        // Initialize BRKGA
        ////////////////////////////////////////

        // NOTE: don't forget to initialize the algorithm.
        log("Initializing BRKGA...");
        algorithm.initialize();

        ////////////////////////////////////////
        // Evolving
        ////////////////////////////////////////

        // Distance functor for path-relink.
        std::shared_ptr<BRKGA::DistanceFunctionBase> dist_func;
        if(brkga_params.pr_type == BRKGA::PathRelinking::Type::DIRECT)
            dist_func.reset(new BRKGA::HammingDistance(0.5));
        else
            dist_func.reset(new BRKGA::KendallTauDistance());

        // Optimization info.
        double best_fitness = numeric_limits<double>::max();
        BRKGA::Chromosome best_solution(instance.num_nodes, 0.0);

        unsigned last_update_iteration = 0;
        double last_update_time;
        unsigned large_offset = 0;

        double path_relink_time = 0.0;
        unsigned num_path_relink_calls = 0;
        unsigned num_homogenities = 0;
        unsigned num_best_improvements = 0;
        unsigned num_elite_improvements = 0;

        // For control the optimization
        unsigned iteration = 0;
        bool run = true;

        log("Evolving...");
        cout << "* Iteration | Cost | CurrentTime" << endl;

        const auto start_time = system_clock::now();
        while(run) {
            // Run one iteration
            algorithm.evolve();

            double fitness = algorithm.getBestFitness();
            if((best_fitness - fitness) > 0.0) {
                last_update_time = elapsedFrom(start_time);
                best_fitness = fitness;

                auto update_offset = iteration - last_update_iteration;
                last_update_iteration = iteration;

                if(large_offset < update_offset)
                    large_offset = update_offset;

                best_solution = algorithm.getBestChromosome();

                cout <<
                "* " << iteration << " | " <<
                setiosflags(ios::fixed) << setprecision(0) <<
                best_fitness << " | " <<
                setiosflags(ios::fixed) << setprecision(2) <<
                last_update_time <<
                endl;
            }

            unsigned iters_no_improvement = iteration - last_update_iteration;

            if(control_params.exchange_interval > 0 &&
               iters_no_improvement > 0 &&
               (iters_no_improvement % control_params.exchange_interval == 0)) {

                cout << "\nPerforming path relink at iteration "
                     << iteration << endl;

                num_path_relink_calls += 1;

                const auto pr_start_time = system_clock::now();
                auto result = algorithm.pathRelink(
                    brkga_params.pr_type,
                    brkga_params.pr_selection,
                    dist_func,
                    brkga_params.pr_number_pairs,
                    brkga_params.pr_minimum_distance
                    1, // block_size doesn't not matter for permutation.
                    max_time - elapsedFrom(start_time),
                    brkga_params.pr_percentage);

                const auto pr_time = elapsedFrom(pr_start_time);
                path_relink_time += pr_time;

                using BRKGA::PathRelinking::PathRelinkingResult;
                switch(result) {
                    case PathRelinkingResult::TOO_HOMOGENEOUS:
                        ++num_homogenities;
                        cout << "- Populations are too too homogeneous | "
                                "Elapsed time: " << pr_time << endl;
                        break;

                    case PathRelinkingResult::NO_IMPROVEMENT:
                        cout << "- No improvement found | "
                                "Elapsed time: " << pr_time << endl;
                        break;

                    case PathRelinkingResult::ELITE_IMPROVEMENT:
                        ++num_elite_improvements;
                        cout << "- Improvement on the elite set but not in the "
                                "best individual | Elapsed time: "
                             << pr_time << endl;
                        break;

                    case PathRelinkingResult::BEST_IMPROVEMENT:
                        ++num_best_improvements;
                        fitness = algorithm.getBestFitness();

                        cout << "- Best individual improvement: "
                             << fitness
                             << " | Elapsed time: " << pr_time << endl;

                        if((best_fitness - fitness) > 0.0) {
                            last_update_time = elapsedFrom(start_time);
                            best_fitness = fitness;

                            auto update_offset = iteration -
                                                 last_update_iteration;
                            last_update_iteration = iteration;

                            if(large_offset < update_offset)
                                large_offset = update_offset;

                            best_solution = algorithm.getBestChromosome();

                            cout <<
                            "* " << iteration << " | " <<
                            setiosflags(ios::fixed) << setprecision(0) <<
                            best_fitness << " | " <<
                            setiosflags(ios::fixed) << setprecision(2) <<
                            last_update_time <<
                            endl;
                        }
                        break;

                    default:;
                } // end switch
            } // End if IPR

            // Time to stop?
            switch(stop_rule) {
                case StopRule::GENERATIONS:
                    if(iteration == static_cast<unsigned>(stop_arg))
                        run = false;
                    break;

                case StopRule::IMPROVEMENT:
                    if(iters_no_improvement >= static_cast<unsigned>(stop_arg))
                        run = false;
                    break;

                case StopRule::TARGET:
                    if(best_fitness > stop_arg - 1e-9)
                        run = false;
                    break;
                default:;
            }

            if(elapsedFrom(start_time) > max_time)
                run = false;
            ++iteration;
        } // end while / main loop

        const auto total_num_iterations = iteration;
        const auto total_elapsed_time = elapsedFrom(start_time);

        log("End of optimization");
        cout <<
        "\nTotal number of iterations: " << total_num_iterations <<
        "\nLast update iteration: " << last_update_iteration <<
        "\nTotal optimization time: " << total_elapsed_time <<
        "\nLast update time: " << last_update_time <<
        "\nLarge number of iterations between improvements: " << large_offset <<
        "\nTotal path relink time: " << path_relink_time <<
        "\nTotal path relink calls: " << num_path_relink_calls <<
        "\nNumber of homogenities: " << num_homogenities <<
        "\nImprovements in the elite set: " << num_elite_improvements <<
        "\nBest individual improvements: " << num_best_improvements <<
        endl;

        ////////////////////////////////////////
        // Extracting the best tour
        ////////////////////////////////////////

        best_fitness = algorithm.getBestFitness();
        best_solution = algorithm.getBestChromosome();

        vector<pair<double, unsigned>> tour(instance.num_nodes);
        for(unsigned i = 0; i < instance.num_nodes; ++i)
            tour[i] = make_pair(best_solution[i], i);

        sort(tour.begin(), tour.end());

        cout << "\n% Best tour cost: "
            << setiosflags(ios::fixed) << setprecision(0)
            << best_fitness
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

        cout <<
        "\n\nInstance,Seed,NumNodes,TotalIterations,TotalTime,"
        "TotalPRTime,PRCalls,NumHomogenities,NumPRImprovElite,"
        "NumPrImprovBest,LargeOffset,LastUpdateIteration,LastUpdateTime,Cost" <<
        endl;

        cout <<
        instance_name << "," <<
        seed << "," <<
        instance.num_nodes << "," <<
        total_num_iterations << "," <<
        setiosflags(ios::fixed) << setprecision(2) <<
        total_elapsed_time << "," <<
        path_relink_time << "," <<
        num_path_relink_calls << "," <<
        num_homogenities << "," <<
        num_elite_improvements << "," <<
        num_best_improvements << "," <<
        large_offset << "," <<
        last_update_iteration << "," <<
        last_update_time << "," <<
        setiosflags(ios::fixed) << setprecision(0) <<
        best_fitness;

        cout.flush();
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
