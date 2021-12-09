/******************************************************************************
 * tsp_decoder_pre_allocating.cpp: Implementation for TSP_Instance class.
 *
 * Created on : May 03, 2019 by andrade
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

#include "decoders/tsp_decoder_pre_allocating.hpp"

#include <algorithm>
#include <omp.h>

using namespace std;
using namespace BRKGA;

//-----------------------------[ Constructor ]--------------------------------//

TSP_Decoder_pre_allocating::TSP_Decoder_pre_allocating(
            const TSP_Instance& _instance, const unsigned num_threads):
    instance(_instance),
    // Pre-allocate space for permutations for each thread.
    permutation_per_thread(num_threads, Permutation(instance.num_nodes))
{}

//-------------------------------[ Decode ]-----------------------------------//

BRKGA::fitness_t  TSP_Decoder_pre_allocating::decode(Chromosome& chromosome,
                                                    bool /* not-used */) {
    // If you have OpenMP available, get the allocated memory per thread ID.
    #ifdef _OPENMP
    auto& permutation = permutation_per_thread[omp_get_thread_num()];
    #else
    auto& permutation = permutation_per_thread[0];
    #endif

    for(unsigned i = 0; i < instance.num_nodes; ++i)
        permutation[i] = make_pair(chromosome[i], i);

    sort(permutation.begin(), permutation.end());

    double cost = instance.distance(permutation.front().second,
                                    permutation.back().second);

    for(unsigned i = 0; i < instance.num_nodes - 1; ++i)
        cost += instance.distance(permutation[i].second,
                                  permutation[i + 1].second);

    return cost;
}
