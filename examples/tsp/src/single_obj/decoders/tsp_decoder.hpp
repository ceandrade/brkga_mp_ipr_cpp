/******************************************************************************
 * tsp_decoder.hpp: Interface for TSP_Decoder class.
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

#ifndef TSP_DECODER_HPP_
#define TSP_DECODER_HPP_

#include "tsp/tsp_instance.hpp"
#include "brkga_mp_ipr/fitness_type.hpp"
#include "brkga_mp_ipr/chromosome.hpp"

/**
 * \brief Interface for TSP_Decoder class.
 *
 * Simple Traveling Salesman Problem decoder. It creates a permutation of nodes
 * induced by the chromosome and computes the cost of the tour.
 */
class TSP_Decoder {
public:
    /** \brief Default Constructor.
     * \param instance TSP instance.
     */
    TSP_Decoder(const TSP_Instance& instance);

    /** \brief Given a chromossome, builds a tour.
     *
     * \param chromosome A vector of doubles represent a problem solution.
     * \param rewrite Indicates if the chromosome must be rewritten. Not used
     *                this decoder, but keep due to API requirements.
     * \return the cost of the tour.
     */
    BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool rewrite);

public:
    /// A reference to a TSP instance.
    const TSP_Instance& instance;
};

#endif // TSP_DECODER_HPP_
