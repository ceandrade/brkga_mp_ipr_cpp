/******************************************************************************
 * greedy_tour.hpp: Interface for greedy_tour heuristic.
 *
 * Created on : Mar 06, 2019 by andrade
 * Last update: Mar 06, 2019 by andrade
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
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

#include "heuristics/greedy_tour.hpp"

#include <limits>
#include <numeric>
#include <vector>

using namespace std;

std::pair<double, std::vector<unsigned>> greedy_tour(
        const TSP_Instance& instance) {

    vector<unsigned> tour(instance.num_nodes, 0);
    tour[0] = 0;

    vector<unsigned> remaining_nodes(instance.num_nodes - 1, 0);
    iota(remaining_nodes.begin(), remaining_nodes.end(), 1);

    double cost = 0.0;
    unsigned current = 0;
    auto it_next = remaining_nodes.begin();
    unsigned idx = 1;

    while(!remaining_nodes.empty()) {
        double best_dist = std::numeric_limits<double>::max();
        for(auto it = remaining_nodes.begin(); it != remaining_nodes.end();
            ++it) {
            auto dist = instance.distance(current, *it);
            if(dist < best_dist) {
                best_dist = dist;
                it_next = it;
            }
        }
        cost += best_dist;
        tour[idx++] = *it_next;
        current = *it_next;
        remaining_nodes.erase(it_next);
    }
    cost += instance.distance(tour.front(), tour.back());

    return std::make_pair(cost, tour);
}
