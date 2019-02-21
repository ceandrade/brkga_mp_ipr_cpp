/******************************************************************************
 * combinatorial_auction_decoder.cpp: Implementation for Combinatorial Auction
 * Decoder class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2013 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : Jun 24, 2011 by andrade
 *  Last update: Mar 23, 2013 by andrade
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

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#include <iterator>
#include <cassert>
#include <stdexcept>
#endif

#include <algorithm>
#include <functional>
#include <limits>
using namespace std;

#include "combinatorial_auction_decoder.hpp"

//--------------------------[ Default Constructor ]--------------------------//

CombinatorialAuctionDecoder::CombinatorialAuctionDecoder(
        const CombinatorialAuction* _auction, Auction::Pricing _pricing,
        DecoderApproach _decode_approach):

        pricing(_pricing), decode_approach(_decode_approach),
        #ifdef DEBUG
        bid_list_size(0), num_calls(0),
        #endif
        auction(_auction), ordered_bids(_auction->num_bids, 0)
{
    #ifdef FULLDEBUG
    cout << setiosflags(ios::fixed) << setprecision(6)
         << "\n------------------------------------------------------\n"
         << "> Building decoder\n> ";
    #endif

    // The epsilon second price
    const double epsilon = _auction->worst_offer / _auction->num_bids;

    VectorPair sorted_bids(_auction->num_bids);

    for(unsigned bid = 0; bid < _auction->num_bids; ++bid) {
        double value;

        // Sort by cost/benefit
        if(_pricing == Auction::FirstPrice) {
            value = _auction->bids[bid].value;
        }
        else {
            if(_auction->next_lower_bids[bid] != _auction->edges[bid].end())
                value = _auction->bids[*(_auction->next_lower_bids[bid])].value;
            else
                value = epsilon;
        }

        // Take the benefit of the bid, i.e., the offered value per bid.
        // If Pirkul, uses the duals values from the goods to make a pseudo benefit.
        if(decode_approach == PIRKUL) {
            double shadow = 1e-16;
            for(list< unsigned >::const_iterator it_good = _auction->bids[bid].goods.begin();
                it_good != _auction->bids[bid].goods.end(); ++it_good) {

                shadow += _auction->good_duals[*it_good];
            }

            value /= shadow;
        }
        else {
            value /= _auction->bids[bid].goods.size();
        }

        sorted_bids[bid] = ValueBid(value, bid);
    }

    sort(sorted_bids.begin(), sorted_bids.end(), greater< ValueBid >());

    for(unsigned i = 0; i < _auction->num_bids; ++i) {
        ordered_bids[sorted_bids[i].second] = i;

        #ifdef FULLDEBUG
        cout << "(" << sorted_bids[i].second << "," << sorted_bids[i].first << ") ";
        cout.flush();
        #endif
    }

    #ifdef FULLDEBUG
    cout << "\n> [";
    copy(ordered_bids.begin(), ordered_bids.end(), ostream_iterator< unsigned >(cout, " "));

    cout << "]\n------------------------------------------------------"
         << endl;
    #endif
}

//--------------------------------[ Decode ]---------------------------------//

double CombinatorialAuctionDecoder::decode(vector< double >& chromosome, bool writeback) {
    #ifdef FULLDEBUG
    cout << setiosflags(ios::fixed) << setprecision(6)
         << "\n------------------------------------------------------\n"
         << "> Decoding chromossome using "
         //<< (greedy? "Greedy" : "Allele Value") << " approach."
         << endl;

    copy(chromosome.begin(), chromosome.end(), ostream_iterator< double >(cout, " "));

    vector< double > chr_compare(chromosome.size());
    copy(chromosome.begin(), chromosome.end(), chr_compare.begin());
    #endif

    // Verify goods already chosen for bids.
    vector< bool > used_goods(auction->num_goods + auction->num_dummy_goods, false);
    unsigned remainder_goods = used_goods.size();

    // If we negate a bid, we mark it with "true" flag
    vector< bool > negate_bids(chromosome.size(), false);

    double value = 0.0;             // Value of the solution
    list< unsigned > solution;      // Bids chosen.

    vector< unsigned > sorted_bids;
    sorted_bids.reserve(chromosome.size());

    // Greedy approach: take all bids with alleles >0.5 and extracts a valid
    // solution by benefit/cost greedily.
    if(decode_approach != LAZY) {
        for(unsigned i = 0; i < chromosome.size(); ++i)
            if(chromosome[i] >= 0.5)
                sorted_bids.push_back(i);

        orderByBenefit(&sorted_bids);
    }
    // Take the bids from alleles but not use any
    // structural information from the auction
    else {
        VectorPair temp_bids;
        temp_bids.reserve(chromosome.size());

        for(unsigned i = 0; i < chromosome.size(); ++i) {
            if(chromosome[i] >= 0.5)
                temp_bids.push_back(ValueBid(chromosome[i], i));
        }

        sort(temp_bids.begin(), temp_bids.end(), greater< ValueBid >());

        for(VectorPair::iterator it = temp_bids.begin(); it != temp_bids.end(); ++it)
            sorted_bids.push_back(it->second);
    }

    #ifdef FULLDEBUG
    const double epsilon = auction->worst_offer / auction->num_bids;

    cout << "\nSorted:\n";
    for(unsigned i = 0; i < sorted_bids.size(); ++i) {
        cout << "(" << sorted_bids[i] << ",";

        if(pricing == Auction::FirstPrice)
            value = auction->bids[sorted_bids[i]].value;
        else
            if(auction->next_lower_bids[sorted_bids[i]] != auction->edges[sorted_bids[i]].end())
                value = auction->bids[*(auction->next_lower_bids[sorted_bids[i]])].value;
            else
                value = epsilon;

        //cout << (greedy? value / auction->bids[sorted_bids[i]].goods.size() : value) << ") ";
    }

    cout << endl;
    #endif

    // Traverse the bids with good genes.
    for(vector< unsigned >::iterator it_bid = sorted_bids.begin();
        it_bid != sorted_bids.end(); ++it_bid) {

        #ifdef FULLDEBUG
        cout << "\n\nTrying bid: " << *it_bid;
        #endif

        // Look for goods already used.
        bool found = false;
        for(list< unsigned >::const_iterator it_good = auction->bids[*it_bid].goods.begin();
            it_good != auction->bids[*it_bid].goods.end(); ++it_good) {

            // If the good already was taken by other bid, negate the current.
            if(used_goods[*it_good]) {
                found = true;
                negate_bids[*it_bid] = true;

                // Modify the chromosome for future usage
                if(writeback && chromosome[*it_bid] > 0.5)
                    chromosome[*it_bid] = 1 - chromosome[*it_bid];
                break;
            }
        }

        // if any found, choose this bid for solution.
        if(!found) {
            // Set goods as used
            for(list< unsigned >::const_iterator it_good = auction->bids[*it_bid].goods.begin();
                it_good != auction->bids[*it_bid].goods.end(); ++it_good) {

                used_goods[*it_good] = true;
                --remainder_goods;
            }

            solution.push_back(*it_bid);

            if(pricing == Auction::FirstPrice) {
                value += auction->bids[*it_bid].value;
            }
            else {
                if(auction->next_lower_bids[*it_bid] != auction->edges[*it_bid].end())
                    value += auction->bids[*(auction->next_lower_bids[*it_bid])].value;
            }
        }

        #ifdef FULLDEBUG
        cout << "\nChosen bids: ";
        copy(solution.begin(), solution.end(), ostream_iterator< unsigned >(cout, " "));
        cout << "\nChosen goods: ";
        unsigned i = 0;
        for(vector< bool >::iterator it = used_goods.begin();
            it != used_goods.end(); ++it, ++i)
            if(*it) cout << i << " ";

        cout << "\nActual value: " << value;
        #endif
    } // end of bids iteration

    //---------------[ Second Phase ]---------------//

    #ifdef FULLDEBUG
    cout << "\n\nNumber of remainder goods: " << remainder_goods << endl;
    #endif

    // Now, we try to complete our solution, if some good isn't allocated.
    if(remainder_goods > 0) {
        #ifdef FULLDEBUG
        cout << "\n>>> Try to complete the solution"
             << "\nNegate bids (not used for this completion): ";
        unsigned aa = 0;
        for(vector< bool >::iterator it = negate_bids.begin();
            it != negate_bids.end(); ++it, ++aa)
            if(*it) cout << aa << " ";
        cout << endl;
        #endif

        // Take the best bids
        vector< unsigned > bids_list;
        bids_list.reserve(auction->num_bids - solution.size());

        // If we already considered a bid, we mark it with "true" flag
        vector< bool > considered_bids(chromosome.size(), false);

        // Looking for unused goods
        for(unsigned i = 0; i < used_goods.size(); ++i) {
            if(used_goods[i])
                continue;

            #ifdef FULLDEBUG
            cout << "\nLooking for bids to cover unused good: " << i << endl;
            #endif

            for(list< unsigned >::const_iterator it_bid = auction->goods_bids[i].begin();
                it_bid != auction->goods_bids[i].end(); ++it_bid) {

                // Skip negated and taken bids
                if(negate_bids[*it_bid] || considered_bids[*it_bid]) {
                    #ifdef FULLDEBUG
                    cout << "(" << *it_bid << ") ";
                    #endif

                    continue;
                }
                #ifdef FULLDEBUG
                cout << *it_bid << " ";
                #endif

                considered_bids[*it_bid] = true;
                bids_list.push_back(*it_bid);
            }
        }

        // Sort bids by their benefit.
        orderByBenefit(&bids_list);

        #ifdef FULLDEBUG
        cout << "\n\n> Bids by benefit:\n";
        for(unsigned i = 0; i < bids_list.size(); ++i)
            cout << "(" << bids_list[i] << ","
                 << auction->bids[bids_list[i]].value / auction->bids[bids_list[i]].goods.size()
                 << ") ";
        cout << endl;

        #pragma omp critical
        bid_list_size += bids_list.size();
        #endif

        // Look for the best bids
        for(vector< unsigned >::iterator it = bids_list.begin(); it != bids_list.end(); ++it) {
            unsigned bid = *it;

            #ifdef FULLDEBUG
            cout << "\n>>> Trying bid: " << bid;
            #endif

            bool valid = true;
            for(list< unsigned >::const_iterator
                it_good = auction->bids[bid].goods.begin();
                it_good != auction->bids[bid].goods.end(); ++it_good) {

                // If the good already was taken by other bid, negate the current.
                if(used_goods[*it_good]) {
                    negate_bids[bid] = true;

                    // Modify the chromosome for future usage
                    if(writeback && chromosome[bid] > 0.5)
                        chromosome[bid] = 1 - chromosome[bid];

                    #ifdef FULLDEBUG
                    cout << " | Not valid!";
                    #endif

                    valid = false;
                    break;
                }
            }
            if(!valid)
                continue;

            // Set goods as used
            #ifdef FULLDEBUG
            cout << "\nMarking goods: ";
            #endif
            for(list< unsigned >::const_iterator
                it_good = auction->bids[bid].goods.begin();
                it_good != auction->bids[bid].goods.end(); ++it_good) {

                used_goods[*it_good] = true;
                #ifdef FULLDEBUG
                cout << *it_good << " ";
                #endif
            }

            solution.push_back(bid);

            if(pricing == Auction::FirstPrice) {
                value += auction->bids[bid].value;
            }
            else {
                if(auction->next_lower_bids[bid] != auction->edges[bid].end())
                    value += auction->bids[*(auction->next_lower_bids[bid])].value;
            }

            // Modify the chromosome for future usage
            if(writeback && chromosome[bid] < 0.5)
                chromosome[bid] = 1 - chromosome[bid];
        } // end while
    }

    #ifdef FULLDEBUG
    cout << "\n\nBid\tOriginal\tModified";
    for(unsigned i = 0; i < chromosome.size(); ++i)
        cout << "\n" << i << "\t" << chr_compare[i] << "\t" << chromosome[i];

    cout << "\n\n+++ Solution found: ";
    copy(solution.begin(), solution.end(), ostream_iterator< unsigned >(cout, " "));

    cout << "\n+++ Value of solution: " << value
         << "\n------------------------------------------------------" << endl;

    #pragma omp critical
    ++num_calls;

    if(!auction->verifySolution(solution, value, pricing))
        throw runtime_error("The presented solution isn't valid!");
    #endif

    return value;
}

//---------------------[ Get Solution from Chromosome ]----------------------//

list< unsigned > CombinatorialAuctionDecoder::getSolutionFromChromosome(vector< double >& chromosome) const {
    list< unsigned > solution;
    for(unsigned i = 0; i < chromosome.size(); ++i)
        if(chromosome[i] > 0.5)
            solution.push_back(i);

    return solution;
}

//---------------------[ Get Solution from Chromosome ]----------------------//

void CombinatorialAuctionDecoder::orderByBenefit(vector< unsigned > *bids) const {
    #ifdef FULLDEBUG
    cout << "\n** orderByBenefit:"
         << "\n**> Original: ";
    for(unsigned i = 0; i < bids->size(); ++i)
        cout << "(" << (*bids)[i] << ","
             << auction->bids[(*bids)[i]].value / auction->bids[(*bids)[i]].goods.size()
             << ") ";
    #endif

    vector< unsigned > sorted_bids(auction->num_bids, 0);

    unsigned i;
    for(i = 0; i < bids->size(); ++i)
        sorted_bids[ordered_bids[(*bids)[i]]] = (*bids)[i] + 1;

    i = 0;
    for(vector< unsigned >::iterator it = sorted_bids.begin(); it != sorted_bids.end(); ++it) {
        if(*it != 0) {
            (*bids)[i++] = *it - 1;
        }
    }

    #ifdef FULLDEBUG
    cout << "\n**> Sorted: ";
    for(i = 0; i < bids->size(); ++i)
        cout << "(" << (*bids)[i] << ","
             << auction->bids[(*bids)[i]].value / auction->bids[(*bids)[i]].goods.size()
             << ") ";

    cout << endl;
    #endif
}
