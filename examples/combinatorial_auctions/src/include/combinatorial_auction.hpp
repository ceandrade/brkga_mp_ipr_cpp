/******************************************************************************
 * combinatorial_auction.hpp: Interface for Combinatorial Auction class.
 *
 * Created on : Jun 24, 2011 by andrade
 * Last update: Mar 23, 2013 by andrade
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2013 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
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

#ifndef COMBINATORIAL_AUCTION_HPP
#define COMBINATORIAL_AUCTION_HPP

#include <list>
#include <vector>
using std::list;
using std::vector;

#include "auction.hpp"

/**
 * \brief Combinatorial Auction description.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the description a combinatorial auction problem.
 * It contains bids on packages of goods.
 */
class CombinatorialAuction: public Auction {
    public:
        /** \name Constructors and Destructor*/
        //@{
        /// Default constructor.
        CombinatorialAuction();

        /// Destructor
        ~CombinatorialAuction();
        //@}

        /** \name Member Methods */
        //@{
        /** \brief Load a problem from a file.
         * \param filename The file that contains the problem description.
         * \return True if success.
         */
        virtual bool loadProblem(const char* filename);

        /** \brief Display the problem data. */
        virtual void display() const;

        /** \brief Verify if a solution is a valid one.
         * \param solution a list of bid indices.
         * \param revenue the reached revenue.
         * \param pricing_type indicate if the revenue comes from first or
         *        second pricing scheme.
         * \return true if the bids are pair-wise disjointed.
         */
        bool verifySolution(const list< unsigned >& solution,
                            double revenue,
                            Pricing pricing_type) const;

        /** \brief Returns the revenue of a solution.
         * \param solution a list of bid indices.
         * \param pricing_type to retrieval first or second prices.
         * \return a double with the value of offered bids.
         */
        double getSolutionRevenue(const list< unsigned >& solution,
                                  Pricing pricing_type) const;
        //@}

    public:
        /** \name Type and Enum Definitions */
        //@{
        /// A bid offered.
        struct Bid {
            list< unsigned > goods;    ///< A list of goods.
            double value;              ///< The value offered for this list.

            Bid(): goods(), value(0.0) {}
        };
        //@}

    public:
        /** \name Data members */
        //@{
        unsigned num_goods;     ///< Number of goods to be negotiate.
        unsigned num_bids;      ///< Number of offered bids.
        unsigned num_dummy_goods;   ///< Dummy goods (for XOR bids).

        /// Vector of bids.
        vector< Bid > bids;

        /// Vector of dual values for each good.
        vector< double > good_duals;

        /// Mapping indicating what bids that contain the item.
        vector< list< unsigned > > goods_bids;

        /// Holds the edges of the intersection graph among the bids.
        /// The edges are ordered by their value (bid value, non-incresing order).
        vector< vector< unsigned > > edges;

        ///Holds the pointer to next lower valued bids related to bid index
        /// in question. These iterators are extracted from ordered #edges above.
        vector< vector< unsigned >::iterator > next_lower_bids;

        double worst_offer;
        //@}
};

#endif  //COMBINATORIAL_AUCTION_HPP
