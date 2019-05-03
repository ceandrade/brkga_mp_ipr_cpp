/******************************************************************************
 * tsp_decoder_pre_allocating.hpp: Interface for memory pre-allocate,
 *      thread-safe TSP_Decoder class.
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : May 03, 2019 by andrade
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

#ifndef TSP_DECODER_PRE_ALLOCATING_HPP_
#define TSP_DECODER_PRE_ALLOCATING_HPP_

#include "tsp/tsp_instance.hpp"
#include "chromosome.hpp"

/**
 * \brief Interface for TSP_Decoder class.
 *
 * Simple Traveling Salesman Problem decoder. It creates a permutation of nodes
 * induced by the chromosome and computes the cost of the tour.
 *
 * This version pre-allocates memory for the permutations using during the
 * decoding process.
 */
class TSP_Decoder_pre_allocating {
public:
    /** \brief Default Constructor.
     * \param instance TSP instance.
     * \param num_threads Number of parallel decoding threads.
     */
    TSP_Decoder_pre_allocating(const TSP_Instance& instance,
                               const unsigned num_threads = 1);

    /** \brief Given a chromossome, build a tour.
     *
     * \param chromosome A vector of doubles represent a problem solution.
     * \return the cost of the tour.
     */
    double decode(BRKGA::Chromosome& chromosome, bool rewrite = true);

public:
    /// A reference to a TSP instance.
    const TSP_Instance& instance;

protected:
    /// Defines a vector that holds node permutations during the decoding.
    typedef std::vector<std::pair<double, unsigned>> Permutation;

    /// For each thread, pre-allocate and hold memory for node permutation
    /// during the decode. All memory is allocated in the constructor,
    /// speeding up the decode process.
    std::vector<Permutation> permutation_per_thread;
};

#endif // TSP_DECODER_PRE_ALLOCATING_HPP_
