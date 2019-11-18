/******************************************************************************
 * combinatorial_auction_decoder.hpp: Interface for Combinatorial Auction
 * Decoder class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2015 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Jun 24, 2011 by andrade
 *  Last update: Jan 19, 2015 by andrade
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

#ifndef COMBINATORIAL_AUCTION_DECODER_HPP
#define COMBINATORIAL_AUCTION_DECODER_HPP

#include "combinatorial_auction.hpp"

#include <utility>
using std::pair;

/**
 * \brief Combinatorial Auction Decoder description.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the decoder method that takes a chromosome from
 * BRKGA algorithm and transformes to a solution of combinatorial auction.
 */
class CombinatorialAuctionDecoder { //: public BRKGA_Decoder {
    public:
        /** \name Enumeration Definitions */
        //@{
        /// Decoder approach.
        enum DecoderApproach {
            /** Sort the bids in non-increasing order of they genes and
                in the following order. */
            LAZY = 'L',

            /** Sort the bids in non-increasing order of utility and choose
                the items in this order. */
            GREEDY = 'G',

            /** Sort the bids in non-increasing order of pseudo-utility using
                dual multipliers of each good and choose the items in this order. */
            PIRKUL = 'P'
        };
        //@}

    public:
        /** \name Constructor and Destructor */
        //@{
        /** \brief Default Constructor.
         * \param _auction used to decode solutions.
         * \param _pricing pricing scheme should be used on decoding.
         * \param decode_approach the decoding approach.
         */
        CombinatorialAuctionDecoder(const CombinatorialAuction* _auction,
                                    Auction::Pricing _pricing,
                                    DecoderApproach _decode_approach);

        virtual ~CombinatorialAuctionDecoder() {}
        //@}

        /** \name Mandatory Method for BRKGA */
        //@{
        /** \brief This method decodes a chromossome to a solution for
         *         a problem and returns its fitness. We sort the chromosome
         *         in non-increasing order and take the bids according this
         *         sorting. If the current bid has items intersecting any
         *         previous bid, it's dropped from solution. Then, we try to
         *         complete the solution with bids with low probability
         *         according the chromosome.
         *
         * \param chromosome A vector of doubles represent a problem solution.
         * \param writeback rewrite the chromosome to reflect the solution.
         * \return a double with fitness value.
         */
        virtual double decode(vector< double >& chromosome, bool writeback);
        //@}

        /** \name Member Methods */
        //@{
        /** \brief This method makes a solution (list of bids) from a chromossome.
         *         The bids to be considered are the ones whose genes are 1.
         *  \param chromosome a vector that represents a chromosome.
         *  \return a list of winner bids indices.
         */
        list< unsigned > getSolutionFromChromosome(vector< double >& chromosome) const;
        //@}

    public:
        /// Indicates what pricing scheme should be used on decoding.
        Auction::Pricing pricing;

        /// The decoder approach.
        DecoderApproach decode_approach;

        #ifdef DEBUG
        /** \name Debug Statistics
         * To calculate the average number of bids in 2nd phase
         */
        //@{
        /// Number of bids in 2nd phase.
        mutable unsigned bid_list_size;

        /// Number of call to 2nd phase.
        mutable unsigned num_calls;
        //@}
        #endif

    protected:
        /** Take a permutation of supplied bids based on the order
         *  #ordered_bids.
         *  \param[in, out] bids to create the permutation.
         */
        void orderByBenefit(vector< unsigned > *bids) const;

    protected:
        /** \name Type definitions */
        //@{
        typedef pair<double, unsigned> ValueBid;
        typedef vector< ValueBid > VectorPair;
        //@}

    protected:
        /// The auction used to decode solutions.
        const CombinatorialAuction* auction;

        /// Keep the indices of ordered bids.
        vector< unsigned > ordered_bids;

    private:
        CombinatorialAuctionDecoder(const CombinatorialAuctionDecoder&) = delete;
        CombinatorialAuctionDecoder& operator=(const CombinatorialAuctionDecoder&) = delete;
};

#endif //COMBINATORIAL_AUCTION_DECODER_HPP
