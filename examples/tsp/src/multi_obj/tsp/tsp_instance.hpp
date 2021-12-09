/******************************************************************************
 * tsp_instance.hpp: Interface for TSP_Instance class.
 *
 * Created on : Mar 05, 2019 by andrade
 * Last update: Mar 05, 2019 by andrade
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

#ifndef TSP_INSTANCE_HPP_
#define TSP_INSTANCE_HPP_

#include <string>
#include <vector>

/**
 * \brief Interface for TSP_Instance class.
 *
 * Represents an instance for the Traveling Salesman Problem. The constructor
 * loads a upper triangular matrix as following:
 *
 * \verbatim
 * number of nodes (n)
 * dist12 dist13 dist14 ... dist1n
 * dist23 dist24 ... dist2(n - 1)
 * ...
 * dist(n-2)(n-1)
 * \endverbatim
 *
 * For example, for n = 4 we have
 * \verbatim
 * 4
 * 12 13 14
 * 23 24
 * 34
 * \endverbatim
 */
class TSP_Instance {
public:
    /// Default Constructor.
    TSP_Instance(const std::string& filename);

    /// Return the distance between nodes i and j.
    double distance(unsigned i, unsigned j) const;

public:
    /// Number of nodes.
    unsigned num_nodes;

    /// Distances between the nodes.
    std::vector<double> distances;
};

#endif // TSP_INSTANCE_HPP_
