/******************************************************************************
 * tsp_instance.cpp: Implementation for TSP_Instance class.
 *
 * Created on : Mar 05, 2019 by andrade
 * Last update: Mar 05, 2019 by andrade
 *
 * (c) Copyright 2015-2022, Carlos Eduardo de Andrade.
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

#include <fstream>
#include <stdexcept>

using namespace std;

//-----------------------------[ Constructor ]--------------------------------//

TSP_Instance::TSP_Instance(const std::string& filename):
    num_nodes(0),
    distances()
{
    ifstream file(filename, ios::in);
    if(!file)
        throw runtime_error("Cannot open instance file");

    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file >> num_nodes;
        distances.resize((num_nodes * (num_nodes - 1)) / 2);
        for(unsigned i = 0; i < distances.size(); ++i)
            file >> distances[i];
    }
    catch(std::ifstream::failure& e) {
        throw fstream::failure("Error reading the instance file");
    }
}

//-------------------------------[ Distance ]---------------------------------//

double TSP_Instance::distance(unsigned i, unsigned j) const {
    if(i > j)
        swap(i, j);
    return distances[(i * (num_nodes - 1)) - ((i - 1) * i / 2) + (j - i - 1)];
}
