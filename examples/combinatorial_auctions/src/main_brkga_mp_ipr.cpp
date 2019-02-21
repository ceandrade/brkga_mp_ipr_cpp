/******************************************************************************
 * main.cpp: Main Process.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2013 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Jun 24, 2011 by andrade
 *  Last update: Jan 13, 2015 by andrade
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
#include "timer.hpp"
#include "combinatorial_auction.hpp"
#include "combinatorial_auction_decoder.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <iterator>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace BRKGA;

//------------------------[ Some control constants ]-------------------------//

// Controls stop criteria
enum StopRule{GENERATIONS = 'G', TARGET = 'T', IMPROVEMENT = 'I'};

// Initial population criteria
enum InitApproach{RAMDOM = 'R', LPRELAXATION = 'L'};

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[])
{
    // First read parameters from command line:
    const int num_args = 15;

    #ifndef TUNING
    if(argc < num_args) {
    #else
    if(argc < num_args + 15) {
    #endif
        cout << "usage: " << argv[0]
             << " <config-file> <seed> <stop-rule> <stop-arg> <decode-approach>"
             << " <initial-population> <init-percentage> <init-max-time> <init-cplex-time> <init-cplex-iters>"
             << " <max-time> <pricing-scheme> <instance-file> <instance-lp-file>"
            #ifdef TUNING
             << " <pop_size> <elite_percentage> <mutants_percentage>"
                " <num_elite_parents> <total_parents> <bias>"
                " <num_independent_populations>"
                " <pr_minimum_distance> <pr_type> <pr_selection>"
                " <alpha_block_size> <pr_percentage> <exchange_interval>"
                " <num_exchange_indivuduals> <reset_interval>"
            #endif
             << "\nwhere: "
             << "\n - <config-file>: parameters of BRKGA algorithm"
             << "\n - <seed>: seed for random generator"
             << "\n - <stop-rule> <stop-arg>: stop rule and its arguments where"
             << "\n\t+ Generations <number_generations>: the algorithm runs until <number_generations>"
             << "\n\t+ Target <value of expected target>: runs until obtains the target value"
             << "\n\t+ Iterations <max generations without improvement>: runs until the solutions don't improve by max generations"
             << "\n - <decode-approach>: the approach used to make solutions:"
             << "\n\t+ Lazy: sort the bids in non-increasing order of they genes and in the following order"
             << "\n\t+ Greedy: sort the bids in non-increasing order of utility and choose the items in this order"
             << "\n\t+ Pirkul: sort the bids in non-increasing order of pseudo-utility(dual multipliers) and choose the items in this order"
             << "\n - <initial-population> <init-percentage> <init-iterations>: indicates how the algorithm must start the initial population"
             << "\n\t+ Random 0 0 0 0: generate all individuals randomly (the 0 has no effect)"
             << "\n\t+ LP <init-percentage> <init-max-time> <init-cplex-time> <init-cplex-iters>: use population-size <init-percentage> LP "
             << "\n\t  relaxations as start individuals (if not generate all, complete with random ones). Each call to Cplex is limited"
             << "\n\t  to <init-cplex-time>, that can uses <init-cplex-iters> tries to tight the model. This procedure are limited to"
             << "\n\t  <init-max-time> seconds. The algorithm uses <init-iterations> to tight the formulation and to generate each relaxation."
             << "\n - <max-time>: max running time (in seconds). If 0, the algorithm stops on chosen stop rule."
             << "\n - <pricing-scheme>: Princing scheme to the auction. Possible values are:"
             << "\n\t+ First: to first price scheme"
             << "\n\t+ Second: to second price scheme"
             << "\n - <instance-file>: instance file"
             << "\n - [instance-lp-file]: instance LP file (here, any format readable by Cplex). Mandatory only if initial population to be LP relaxations"
             << "\n - <path-relink-type>: defines the distance function and path-relink approach"
             << "\n\t+ Direct: uses the direct path-relink and the Hamming distance"
             << "\n\t+ Permutation: uses the permutation based path-relink and the Kendall Tau distance"
             << "\n - <mininum-distance>: mininum distance between solution to path-relink"
             << "\n\n ALL PARAMETERS ARE MANDATORY\n"
             << endl;
        return 64;  // BSD usage error code
    }

    // Loading parameters from command line
    const string configFile(argv[1]);       // config file
    unsigned long seed;                     // RNG seed
    char stop_rule;                         // stopping rule
    double stop_arg;                        // argument to stopping rule
    CombinatorialAuctionDecoder::DecoderApproach decode_approach; // true to greedy, false to lazy
    char init_population;                   // how initial population must be generated, ...
    double init_percentage;                 // ... how many individuals and...
    double init_max_time;                   // ... for how much time...
    double init_cplex_time;                 // ... using this time for each iteration...
    unsigned init_cplex_iters;              // ... how many iterations to be used
    double max_time;                        // max running time
    Auction::Pricing pricing;               // princing scheme
    const char* instance_file = argv[13];   // instance file
//    const char* lp_file = argv[14];         // LP file

    // BRKGA params
    unsigned pop_size;
    double elite_percentage;
    double mutants_percentage;
    unsigned num_elite_parents;
    unsigned total_parents;
    BiasFunction bias;
    unsigned num_independent_populations;
    const unsigned MAX_THR = 1;

    // IPR params
    double pr_minimum_distance;
    PathRelinking::Type pr_type;
    PathRelinking::Selection pr_selection;
    double alpha_block_size;
    double pr_percentage;

    // Main loop params
    unsigned exchange_interval;
    unsigned num_exchange_indivuduals;
    unsigned reset_interval;

    try {
        seed = boost::lexical_cast<unsigned long>(argv[2]);
        stop_rule = StopRule(toupper(argv[3][0]));
        stop_arg = boost::lexical_cast<double>(argv[4]);
        decode_approach = CombinatorialAuctionDecoder::DecoderApproach(toupper(argv[5][0]));
        init_population = InitApproach(toupper(argv[6][0]));
        init_percentage = boost::lexical_cast<double>(argv[7]);
        init_max_time = boost::lexical_cast<double>(argv[8]);
        init_cplex_time = boost::lexical_cast<double>(argv[9]);
        init_cplex_iters = boost::lexical_cast<unsigned>(argv[10]);
        max_time = boost::lexical_cast<double>(argv[11]);
        pricing = Auction::Pricing(toupper(argv[12][0]));

        if((stop_rule != 'G' && stop_rule != 'T' && stop_rule != 'I') ||
           (decode_approach != CombinatorialAuctionDecoder::LAZY &&
            decode_approach != CombinatorialAuctionDecoder::GREEDY &&
            decode_approach != CombinatorialAuctionDecoder::PIRKUL) ||
           (init_population != 'R' && init_population != 'L') ||
           (pricing != Auction::FirstPrice && pricing != Auction::SecondPrice))

            throw exception();
    }
    catch(exception& e) {
        cout << "*** Bad arguments. Verify them!" << endl;
        return 64; // BSD usage error code
    }

    // Loading algorithm parameters from config file (code from rtoso).
    ifstream fin(configFile, std::ios::in);
    if(!fin) {
        cerr << "Cannot open configuration file: " << configFile << endl;
        return 66; // BSD file not found error code
    }

    fin.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
    try {
        string line;
        fin >> pop_size;                        getline(fin, line);
        fin >> elite_percentage;                getline(fin, line);
        fin >> mutants_percentage;              getline(fin, line);
        fin >> num_elite_parents;               getline(fin, line);
        fin >> total_parents;                   getline(fin, line);
        fin >> bias;                            getline(fin, line);
        fin >> num_independent_populations;     getline(fin, line);
        fin >> pr_minimum_distance;             getline(fin, line);
        fin >> pr_type;                         getline(fin, line);
        fin >> pr_selection;                    getline(fin, line);
        fin >> alpha_block_size;                getline(fin, line);
        fin >> pr_percentage;                   getline(fin, line);
        fin >> exchange_interval;               getline(fin, line);
        fin >> num_exchange_indivuduals;        getline(fin, line);
        fin >> reset_interval;                  getline(fin, line);
        fin.close();
    }
    catch(ifstream::failure& e) {
        cerr << "Failure when reading config-file: " << e.what() << endl;
        fin.close();
        return 65;  // BSD file read error code
    }

    //-----------------------------------------//
    // Tuning
    //-----------------------------------------//

    #ifdef TUNING
    pop_size = boost::lexical_cast<unsigned>(argv[num_args]);
    elite_percentage = boost::lexical_cast<double>(argv[num_args + 1]);
    mutants_percentage = boost::lexical_cast<double>(argv[num_args + 2]);
    num_elite_parents = boost::lexical_cast<unsigned>(argv[num_args + 3]);
    total_parents = boost::lexical_cast<unsigned>(argv[num_args + 4]);
    bias = toEnum<BiasFunction>(argv[num_args + 5]);
    num_independent_populations = boost::lexical_cast<unsigned>(argv[num_args + 6]);
    pr_minimum_distance = boost::lexical_cast<double>(argv[num_args + 7]);
    pr_type = toEnum<PathRelinking::Type>(argv[num_args + 8]);
    pr_selection = toEnum<PathRelinking::Selection>(argv[num_args + 9]);
    alpha_block_size = boost::lexical_cast<double>(argv[num_args + 10]);
    pr_percentage = boost::lexical_cast<double>(argv[num_args + 11]);
    exchange_interval = boost::lexical_cast<unsigned>(argv[num_args + 12]);
    num_exchange_indivuduals = boost::lexical_cast<unsigned>(argv[num_args + 13]);
    reset_interval = boost::lexical_cast<unsigned>(argv[num_args + 14]);
    #endif

    //-----------------------------------------//

    cout << "------------------------------------------------------"
         << "\n> Parameters\n"
         << "\nConfig file: " << configFile
         << "\n   + % of Elite: " << elite_percentage
         << "\n   + % of Mutants: " << mutants_percentage
         << "\n   + # of independent populations: " << num_independent_populations
         << "\n   + maximum size of each population: " << pop_size
         << "\n   + # of threads: " << MAX_THR
         << "\n   + interval of chromossome exchange: " << exchange_interval
         << "\n   + # of elite chromossome of each population: " << num_exchange_indivuduals
         << "\n   + reset interval: " << reset_interval
         << "\nSeed: " << seed
         << "\nStop Rule: [" << stop_rule << "] "
         << (stop_rule == 'G' ? "Generations -> " :
             (stop_rule == 'T' ? "Target -> " : "Improvement -> "))
         << stop_arg
         << "\nDecode approach: " << (decode_approach == CombinatorialAuctionDecoder::LAZY? "Lazy" :
                                     (decode_approach == CombinatorialAuctionDecoder::GREEDY? "Greedy" : "Pirkul"))
         << "\nInitial Population approach: " << (init_population == 'R'? "Random" : "LP Relaxations ");

    if(init_population == 'L')
        cout << "\n   + % of individuals: " << init_percentage
             << "\n   + Iterations: " << init_cplex_iters
             << "\n   + Init. Max time: " << init_max_time
             << "\n   + Init. Cplex time: " << init_cplex_time
             << "\n   + Init. Cplex iterarions: " << init_cplex_iters;

    cout << "\nPath relink type: " << pr_type;
    cout << "\nMininum path relink distance: " << pr_minimum_distance
         << "\nMax Time: " << max_time
         << "\nPricing Scheme: " << (pricing == Auction::FirstPrice? "First Price" : "Second Price")
         << "\n------------------------------------------------------" << endl;


    // Begin of real fun :-)
    try {
        // Loading instance file
        CombinatorialAuction auction;
        if(!auction.loadProblem(instance_file))
            return 65;  // BSD file read error code

        // Decoder to auction
        CombinatorialAuctionDecoder decoder(&auction, pricing, decode_approach);

        unsigned population_size = 10 * auction.num_bids;
        if(population_size > pop_size)
            population_size = (auction.num_bids > pop_size)? auction.num_bids : pop_size;

        // The BRKGA_IPR algorithm object.
        BRKGA_MP_IPR<CombinatorialAuctionDecoder> algorithm(
                decoder, BRKGA::Sense::MAXIMIZE, seed,
                auction.num_bids, pop_size, elite_percentage,
                mutants_percentage, true, num_elite_parents, total_parents,
                bias, num_independent_populations, MAX_THR);

        std::shared_ptr<DistanceFunctionBase> dist_func;

        if(pr_type == BRKGA::PathRelinking::Type::DIRECT)
            dist_func.reset(new BRKGA::HammingDistance(0.5));
        else
            dist_func.reset(new BRKGA::KendallTauDistance());

        // Hold best results
        double best_fitness = numeric_limits< double >::min();
        vector< double > best_chromosome;
        unsigned last_update_iteration = 0;
        double last_update_time = 0.0;
        unsigned update_offset = 0;
        unsigned large_offset = 0;
        double init_population_time = 0.0;
        double path_relink_time = 0.0;
        unsigned path_relink_improvements = 0;
        unsigned num_homogenities = 0;

        Timer timer;

        // Initialize properly and take elapsed time to do it
        algorithm.initialize();
        init_population_time = timer.elapsed();

        unsigned iteration = 1;
        bool run = true;

        cout << "\n\n>>>>> Evolving <<<<<" << endl;
        cout << setiosflags(ios::fixed) << setprecision(6);

        // Reset the timer and do the iterations
        timer.restart();
        while(run) {
            // Run one iteration
            algorithm.evolve();

            // Take the best solution
            double fitness = algorithm.getBestFitness();

            // Print some information
            #ifdef DEBUG
            cout << "Iteration " << iteration
                 << " / Best Current: " << fitness
                 << " / Best Overall: " << best_fitness << endl;
            #endif

            if((fitness - best_fitness) > 0.000001) {
                last_update_time = timer.elapsed();

                if(stop_rule != TARGET)
                    cout << "Improvement Iteration " << iteration
                         << " / Current Time: " << last_update_time
                         << " / Best Current: " << fitness
                         << " / Best Overall: " << best_fitness
                         << endl;

                best_fitness = fitness;
                best_chromosome = algorithm.getBestChromosome();
                update_offset = iteration - last_update_iteration;
                last_update_iteration = iteration;

                if(large_offset < update_offset)
                    large_offset = update_offset;
            }

            unsigned iterWithoutImprovement = iteration - last_update_iteration;

            // Path relink.
            if((exchange_interval > 0) && (iterWithoutImprovement > 0) &&
               (iterWithoutImprovement % exchange_interval == 0)) {

                std::size_t block_size = ceil(alpha_block_size * sqrt(pop_size));
                if(block_size > algorithm.getChromosomeSize())
                    block_size = algorithm.getChromosomeSize() / 2;

                cout << "Path relink at " << iteration << " iteration. "
                     << "Block size: " << block_size << ". "
                     << "Type: " << pr_type << ": ";
                cout.flush();

                auto now = timer.elapsed();
                bool res = algorithm.pathRelink(
                                dist_func,
                                auction.num_bids * pr_minimum_distance,
                                pr_type, pr_selection, block_size,
                                long(max_time) - (timer.elapsed() / long(1e9)),
                                pr_percentage);
                path_relink_time += timer.elapsed() - now;

                if(res) {
                    cout << " Current Time: " << timer.elapsed()
                         << " / Best so far: " << algorithm.getBestFitness()
                         << endl;

                    fitness = algorithm.getBestFitness();

                    if((fitness - best_fitness) > 1e-6) {
                        ++path_relink_improvements;
                        last_update_time = timer.elapsed();

                        if(stop_rule != TARGET)
                            cout << "Improvement Iteration " << iteration
                                 << " / Current Time: " << last_update_time
                                 << " / Best Current: " << fitness
                                 << " / Best Overall: " << best_fitness
                                 << endl;

                        best_fitness = fitness;
                        best_chromosome = algorithm.getBestChromosome();
                        update_offset = iteration - last_update_iteration;
                        last_update_iteration = iteration;

                        if(large_offset < update_offset)
                            large_offset = update_offset;
                    }
                }
                else {
                    ++num_homogenities;
                    cout << "Populations are too homogeneous to path relink" << endl;
                }
            }


            // Time to reset?
            if(reset_interval > 0 && iterWithoutImprovement > 0 &&
               iterWithoutImprovement % reset_interval == 0) {
                algorithm.reset();
                if(stop_rule != TARGET)
                    cout << ">> Reset chromosomes with random keys after " << reset_interval
                         << " iterations without improvement." << endl;
            }

            // Time to stop?
            switch(stop_rule) {
                case GENERATIONS:
                    if(iteration == (unsigned)stop_arg)
                        run = false;
                    break;

                case IMPROVEMENT:
                    if(iterWithoutImprovement >= (unsigned)stop_arg)
                        run = false;
                    break;

                case TARGET:
                    if((stop_arg - best_fitness) < 0.000001)
                        run = false;
                    break;
            }

            if(timer.elapsed() > max_time) {
                run = false;
            }

            ++iteration;
        } // end of while
        #ifdef DEBUG
        cout << "\n\n> End of evolving"
             << "\n------------------------------------------------------" << endl;
        #endif

        // Take elapsed time to optmization
        double elapsed_time = timer.elapsed();

        if(stop_rule != TARGET) {
            // Print results
            list< unsigned > solution = decoder.getSolutionFromChromosome(best_chromosome);
            double value_by_auction = auction.getSolutionRevenue(solution, pricing);

            cout << "\nBest solution > value: " << value_by_auction << " | (bids): ";
            copy(solution.begin(), solution.end(), ostream_iterator< unsigned >(cout, " "));

            cout << "\n\nRevenue & Iterations & Last Update Iteration & Last Update Time & "
                 << "Update Offset & Large Offset & Init. Time (s) & Total Elap. Time (s) & "
                 << "PathRelinkingImprovements & PathRelinkingTime & Num_homogenities\n"
                 << setiosflags(ios::fixed) << setprecision(6)
                 << best_fitness << " & "
                 << setprecision(2)
                 << --iteration << " & "            // only to correct the iteration number
                 << last_update_iteration << " & "
                 << last_update_time << " & "
                 << update_offset << " & "
                 << large_offset << " & "
                 << init_population_time << " & "
                 << elapsed_time << " & "
                 << path_relink_improvements << " & "
                 << path_relink_time << " & "
                 << num_homogenities;

            #ifdef TUNING
            cout << "\n\n"
                 << (-1 * best_fitness);
            cout.flush();
            #endif
        }
        else {
            cout << setiosflags(ios::fixed) << setprecision(2)
                 << best_fitness << " & "
                 << setprecision(2)
                 << --iteration << " & "            // only to correct the iteration number
                 << init_population_time << " & "
                 << elapsed_time;
        }

        cout.flush();
        dist_func.reset();
    }
    catch(exception& e) {
        cout << "\n***********************************************************"
             << "\n****  Exception Occured: " << e.what()
             << "\n***********************************************************"
             << endl;
        return 70; // BSD software internal error code
    }
    return 0;
}

