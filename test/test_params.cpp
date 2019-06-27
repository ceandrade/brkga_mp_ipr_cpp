#include "brkga_mp_ipr.hpp"
#include "decoders.hpp"

using namespace std;
using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

// int main(int argc, char* argv[]) {
int main() {
    try {
        auto params = readConfiguration("config.conf");
        BrkgaParams& brkga_params = params.first;
        ExternalControlParams& control_params = params.second;

        cout << "\n\n" << brkga_params.num_elite_parents;

        writeConfiguration("test.conf", brkga_params);
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
