/******************************************************************************
 * combinatorial_auction.hpp: Implementation for Combinatorial Auction class.
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <new>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <map>
#include <set>
#include <iomanip>

#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/lexical_cast.hpp>
#pragma GCC diagnostic pop

using namespace std;

#include "combinatorial_auction.hpp"

//-------------------------[ Default Construtor ]----------------------------//

CombinatorialAuction::CombinatorialAuction():
    Auction(), num_goods(0), num_bids(0), num_dummy_goods(0), bids(), good_duals(),
    goods_bids(), edges(), next_lower_bids(), worst_offer(numeric_limits< double >::max())
    {}

//-------------------------[ Default Destructor ]----------------------------//

CombinatorialAuction::~CombinatorialAuction() {}

//-----------------------------[ Load Problem ]------------------------------//

bool CombinatorialAuction::loadProblem(const char* filename) {
    #ifdef DEBUG
    cout << "\n------------------------------------------------------\n"
         << "> Loading the problem: " << filename;
    cout.flush();
    #endif

    string line;
    string data;
    stringstream error_msg;

    // Try to open the file
    ifstream file(filename, ios::in);

    // Try to read and parse the file
    try{
        // First, take off comments and blank lines,
        // and load the first parameters
        while(getline(file, line)) {
            string::size_type i = line.find_first_not_of ("\t\n\v" );

            // Ignore all comments and blank lines
            if((i != string::npos && line[i] == '%') || line.empty())
                continue;

            istringstream stream_line(line, ios_base::out);
            stream_line >> data;

            // Parse the first parameters
            if(data == "goods") {
                stream_line >> num_goods;
                continue;
            }
            if(data == "bids") {
                stream_line >> num_bids;
                continue;
            }
            if(data == "dummy") {
                stream_line >> num_dummy_goods;
                break;
            }
        } // end of while

        // Check the limits
        if(num_goods < 1 || num_bids < 1) {
            error_msg << "Incorrect number of goods or bids: "
                      << "\n** goods: " << num_goods
                      << "\n** bids: " << num_bids;
            throw std::runtime_error(error_msg.str());
        }

        // Set size of the bids vector
        bids.resize(num_bids);
        goods_bids.resize(num_goods + num_dummy_goods);

        // Load the bids
        unsigned bid_id;
        double value;
        unsigned bids_count = 0;
        while(getline(file, line)) {
            // Blank lines
            if(line.empty())
                continue;

            istringstream stream_line(line, ios_base::out);

            stream_line >> bid_id >> value;
            bids[bid_id].value = value;

            if(worst_offer > value)
                worst_offer = value;

            stream_line >> data;
            do {
                unsigned good = boost::lexical_cast<unsigned>(data);

                if(good > (num_goods + num_dummy_goods - 1)) {
                    error_msg << "Invalid good: " << good;
                    throw std::runtime_error(error_msg.str());
                }

                // put the good on bid
                bids[bid_id].goods.push_back(good);
                // put the bid in the list's good
                goods_bids[good].push_back(bid_id);

                stream_line >> data;
            } while(data[0] != '#');

            ++bids_count;
        } // end of while
        // Check the limits
        if(bids_count != num_bids) {
            error_msg << "Loaded bids don't correspond to expected ones"
                      << "\n** expected: " << num_bids
                      << "\n** loaded: " << bids_count;
            throw std::runtime_error(error_msg.str());
        }
    }
    catch(exception& e) {
        cout << "\n******************************************************\n"
             << "Error on " <<  __PRETTY_FUNCTION__
             <<  ": line " << __LINE__
             << "\nData file cannot be loaded: " << filename
             << "\nError: " << e.what()
             << "\n******************************************************"
             << endl;

        file.close();
        return false;
    }

    // Build the intersection graph.
    vector< vector< bool > > temp_graph(num_bids);
    for(vector< vector< bool > >::iterator it = temp_graph.begin();
        it != temp_graph.end(); ++it) {

        it->reserve(num_bids);
        it->resize(num_bids, false);
    }

    // For each good, build the edges among all bids
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(2)
    #endif
    for(unsigned good = 0; good < (num_goods + num_dummy_goods); ++good) {
        for(list< unsigned >::iterator bid_i = goods_bids[good].begin();
            bid_i != goods_bids[good].end(); ++bid_i) {

            list< unsigned >::iterator bid_j = bid_i;
            ++bid_j;

            for(; bid_j != goods_bids[good].end(); ++bid_j) {
                temp_graph[*bid_i][*bid_j] = true;
                temp_graph[*bid_j][*bid_i] = true;
            }
        }
    }

    #ifdef FULLDEBUG
    map< unsigned, map< unsigned, bool > > repeat;
    ofstream graphoutput("graph.dot");
    graphoutput << "graph G {\n";

    for(unsigned i = 0; i < bids.size(); ++i) {
        graphoutput << "\tnode" << i << " [label=" << "\""
                    << i << "\\n "
                    << bids[i].value << "\"]\n" ;
    }
    #endif

    // So, clean duplicates and sort by value
    edges.resize(num_bids);
    next_lower_bids.resize(num_bids);

    #ifdef _OPENMP
        #pragma omp parallel for num_threads(2)
    #endif
    for(unsigned bid_i = 0; bid_i < temp_graph.size(); ++bid_i) {
        // Sort by values
        typedef pair<double, unsigned> ValueBid;
        typedef vector< ValueBid > VectorPair;
        VectorPair sorted_bids;

        for(unsigned bid_j = 0; bid_j < num_bids; ++bid_j) {
            if(temp_graph[bid_i][bid_j])
                sorted_bids.push_back(ValueBid(bids[bid_j].value, bid_j));
        }

        sort(sorted_bids.begin(), sorted_bids.end(), greater< ValueBid >());

        // Copy to edges
        edges[bid_i].resize(sorted_bids.size());
        vector< unsigned >::iterator it_edge = edges[bid_i].begin();

        for(VectorPair::iterator it = sorted_bids.begin(); it != sorted_bids.end(); ++it) {
            *(it_edge++) = it->second;

            #ifdef FULLDEBUG
            #ifdef _OPENMP
            #pragma omp critical
            {
            if(!repeat[bid_i][it->second]) {
                graphoutput << "\tnode" << bid_i  << " -- node" << it->second << "\n";
                repeat[bid_i][it->second] = true;
                repeat[it->second][bid_i] = true;
            }
            }
            #endif
            #endif
        }

        // Locate the next lower valued bid related to the bid in question.
        for(it_edge = edges[bid_i].begin();  it_edge != edges[bid_i].end(); ++it_edge)
            if(bids[bid_i].value > bids[*it_edge].value)
                break;

        next_lower_bids[bid_i] = it_edge;
    }
    #ifdef DEBUG
    graphoutput << "}\n";
    graphoutput.close();
    #endif
    #ifdef DEBUG
    display();
    cout << "\n------------------------------------------------------" << endl;
    #endif

    return true;
}

//-------------------------------[ Display ]---------------------------------//

void CombinatorialAuction::display() const {
    cout << "\nNumbers of goods: " << num_goods
         << "\nNumbers of bids: " << num_bids
         << "\nNumbers of dummy goods: " << num_dummy_goods
         << "\nBid\tValue\t\tGoods";

    unsigned i = 0;
    for(vector< Bid >::const_iterator it = bids.begin(); it != bids.end(); ++it) {
        cout << "\n" << i++ << "\t" << it->value << "\t\t";

        copy(it->goods.begin(), it->goods.end(),
             ostream_iterator< unsigned >(cout, " "));
    }

    cout << "\n\nWorst offer: " << worst_offer;

    cout << "\n\nGood\tBids";
    i = 0;
    for(vector< list< unsigned > >::const_iterator it = goods_bids.begin();
        it != goods_bids.end(); ++it) {

        cout << "\n" << i++ << "\t";
        copy(it->begin(), it->end(), ostream_iterator< unsigned >(cout, " "));
    }

    cout << "\n\nBids\tFirst Price\tSecond Price";
    for(unsigned bid_j = 0; bid_j < num_bids; ++bid_j) {
        double second_value = 0.0;

        if(next_lower_bids[bid_j] != edges[bid_j].end())
            second_value = bids[*(next_lower_bids[bid_j])].value;

        cout << "\n" << bid_j << "\t" << bids[bid_j].value << "\t\t" << second_value;
    }


}

//----------------------------[ Verify Solution ]----------------------------//

bool CombinatorialAuction::verifySolution(const list< unsigned >& solution,
                                  double revenue, Pricing pricing_type) const {

    #ifdef DEBUG
    cout << setiosflags(ios::fixed) << setprecision(6)
         << "\n------------------------------------------------------\n"
         << "> Verifying the solution:\n"
         << "> Revenue: " << revenue << " | "
         << "Pricing type: " << (pricing_type == Auction::FirstPrice? "First Price" : "Second Price")
         << "\n> Bids: ";

    copy(solution.begin(), solution.end(), ostream_iterator< unsigned >(cout, " "));
    #endif

    vector< bool > used_goods(num_goods + num_dummy_goods, false);
    bool double_good = false;

    for(list< unsigned >::const_iterator it_bid = solution.begin();
        it_bid != solution.end(); ++it_bid) {

        for(list< unsigned >::const_iterator it_good = bids[*it_bid].goods.begin();
            it_good != bids[*it_bid].goods.end(); ++it_good) {

            if(!used_goods[*it_good]) {
                used_goods[*it_good] = true;
            }
            else {
                double_good = true;
                break;
            }
        }
        if(double_good)
            break;
    }

    double found_revenue = getSolutionRevenue(solution, pricing_type);

    bool ok = !double_good && (abs(revenue - found_revenue) < 0.000001);

    #ifdef DEBUG
    cout << setiosflags(ios::fixed) << setprecision(6)
         << "\n> Found revenue: " << found_revenue;

    if(!ok) cout << "\n> Solution couldn't be verified";
    cout << "\n------------------------------------------------------" << endl;
    #endif

    return ok;
}

//--------------------------[ Get Solution Value ]---------------------------//

double CombinatorialAuction::getSolutionRevenue(const list< unsigned >& solution,
                                                Pricing pricing_type) const {
    double sum = 0.0;

    if(pricing_type == Auction::FirstPrice) {
        for(list< unsigned >::const_iterator it = solution.begin();
            it != solution.end(); ++it) {
            sum += bids[*it].value;
        }
    }
    else {
        #ifdef DEBUG
        cout << "\n";
        #endif
        for(list< unsigned >::const_iterator it = solution.begin();
            it != solution.end(); ++it) {
            // If we have some intersect bid whose value is below that the
            // current bid, take its value.
            if(next_lower_bids[*it] != edges[*it].end()) {
                #ifdef FULLDEBUG
                cout << "\n-- bid: " << *it
                     << ", next_bid: " << *(next_lower_bids[*it])
                     << ", value: " << bids[*(next_lower_bids[*it])].value;
                #endif

                sum += bids[*(next_lower_bids[*it])].value;
            }
        }
    }

    return sum;
}
