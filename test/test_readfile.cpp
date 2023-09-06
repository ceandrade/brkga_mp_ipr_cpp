#include "brkga_mp_ipr.hpp"
#include "decoders.hpp"

using namespace std;
using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    if(argc < 4) {
        cerr << "Usage: "<< argv[0]
             << " <chromosome-size> <config-file> <seed>" << endl;
        return 1;
    }

//    cout << BRKGA::BiasFunction::LOGINVERSE << endl;
//
//    cin.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);
//    try {
//        BRKGA::BiasFunction bias;
//        cin >> bias;
//    }
//    catch(ifstream::failure& e) {
//        cerr << "Failure when reading configfile: " << e.what() << endl;
//        return 65;  // BSD file read error code
//    }

    try {
        const unsigned seed = 2384785233;
        const unsigned chr_size = atoi(argv[1]);

//        Sum_Decoder decoder;
        Order_Decoder decoder;

        // The BRKGA_MP_IPR algorithm object.
        BRKGA_MP_IPR<Order_Decoder> algorithm(
                decoder, BRKGA::Sense::MAXIMIZE, seed,
                chr_size, string(argv[2]));


        algorithm.setBiasCustomFunction(
            [](const double x) { return 1 / (x * x);}
        );

        vector<Chromosome> initial_chromosomes(algorithm.getChromosomeSize());

        std::mt19937 rng(seed);
        for(unsigned i = 0; i < algorithm.getChromosomeSize(); ++i) {
            initial_chromosomes[i].resize(chr_size);
            for(unsigned j = 0; j < chr_size; ++j) {
                initial_chromosomes[i][j] =
                        generate_canonical<double,
                                           numeric_limits<double>::digits>(rng);
            }
        }

        initial_chromosomes[0][0] = 0.4;
        initial_chromosomes[0][1] = 0.3;
        initial_chromosomes[0][2] = 0.2;
        initial_chromosomes[0][3] = 0.1;

        initial_chromosomes[1][0] = 0.1;
        initial_chromosomes[1][1] = 0.2;
        initial_chromosomes[1][2] = 0.3;
        initial_chromosomes[1][3] = 0.4;

//        for(auto &chr : initial_chromosomes) {
//            cout << "\n+ ";
//            for(auto &v : chr)
//                cout << v << " ";
//        }

//        cpu_timer timer;
//        timer.start();
//        timer.stop();
//        cpu_times loading_time(timer.elapsed());

        algorithm.setInitialPopulation(initial_chromosomes);
        algorithm.initialize();
        algorithm.evolve();

        //std::shared_ptr<DistanceFunctionBase> dist_func(new HammingDistance(0.25));
        std::shared_ptr<DistanceFunctionBase> dist_func(new KendallTauDistance());

        cout << "\n\n path relink" << endl;
        cout << "\n\n- it works? "
             << algorithm.pathRelink(dist_func, 1000)
             << endl;

        algorithm.writeConfiguration("test.conf");
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
