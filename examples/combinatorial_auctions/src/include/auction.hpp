/******************************************************************************
 * auction.hpp: Interface for Auction class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Jun 24, 2011 by andrade
 *  Last update: Jul 22, 2011 by andrade
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

#ifndef AUCTION_HPP
#define AUCTION_HPP

/**
 * \brief Generic auction description.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains the description a generic auction problem.
 */
class Auction {
    public:
        /** \name Constructors and Destructor*/
        //@{
        /// Default constructor.
        Auction() {}

        /// Destructor
        inline virtual ~Auction() {};
        //@}

        /** \name Member Methods */
        //@{
        /** \brief Load a problem from a file.
         * \param filename The file that contains the problem description.
         * \return True if success.
         */
        virtual bool loadProblem(const char* filename) = 0;

        /** \brief Display the problem data. */
        virtual void display() const = 0;
        //@}

    public:
        /** \name Enum Definitions */
        //@{
        /// Indicates the pricing type.
        enum Pricing {
            FirstPrice = 'F',   ///< Indicates that a bidder pays his bid
            SecondPrice = 'S'   ///< Indicates that a bidder pays the highest bid below his.
        };
        //@}
};

#endif  //AUCTION_HPP
