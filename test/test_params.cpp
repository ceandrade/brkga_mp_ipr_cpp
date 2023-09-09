/******************************************************************************
 * test_params.cpp: unit test for loading and writing parameters.
 *
 * Created on : Mar 06, 2019 by andrade
 * Last update: Sep 06, 2023 by andrade
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

#include "brkga_mp_ipr.hpp"
#include "decoders.hpp"

#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

using namespace BRKGA;

//-------------------------------[ Main ]------------------------------------//

int main(int argc, char* argv[]) {
    if(argc < 2) {
        cerr << "Usage: test-params <config_file>";
        return 1;
    }

    try {
        // auto start = high_resolution_clock::now();

        auto [brkga_params, control_params] = readConfiguration(argv[1]);
        writeConfiguration(cout, brkga_params, control_params);

        // std::this_thread::sleep_for(2s);

        // auto stop = high_resolution_clock::now();
        // cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start) << endl;
    }
    catch(exception& e) {
        cerr << "\n***********************************************************"
             << "\n****  Exception Occured: " << e.what()
             << "\n***********************************************************"
             << endl;
        return 1;
    }
    return 0;
}
