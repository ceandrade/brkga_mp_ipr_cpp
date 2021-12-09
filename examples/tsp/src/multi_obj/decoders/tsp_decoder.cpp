/******************************************************************************
 * tsp_instance.cpp: Implementation for TSP_Instance class.
 *
 * Created on : Mar 05, 2019 by andrade
 * Last update: Nov 30, 2021 by andrade
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

#include "decoders/tsp_decoder.hpp"

#include <limits>
#include <algorithm>

using namespace std;
using namespace BRKGA;

//-----------------------------[ Constructor ]--------------------------------//

TSP_Decoder::TSP_Decoder(const TSP_Instance& _instance):
    instance(_instance)
{}

//-------------------------------[ Decode ]-----------------------------------//

BRKGA::fitness_t TSP_Decoder::decode(Chromosome& chromosome,
                                     bool /* not-used */) {
    vector<pair<double, unsigned>> permutation(instance.num_nodes);
    for(unsigned i = 0; i < instance.num_nodes; ++i)
        permutation[i] = make_pair(chromosome[i], i);

    sort(permutation.begin(), permutation.end());

    double largest_edge = numeric_limits<double>::min();

    double cost = instance.distance(permutation.front().second,
                                    permutation.back().second);

    for(unsigned i = 0; i < instance.num_nodes - 1; ++i) {
        auto dist = instance.distance(permutation[i].second,
                                      permutation[i + 1].second);

        largest_edge = max(largest_edge, dist);
        cost += dist;
    }

    return {cost, largest_edge};
}
