/*******************************************************************************
 * chromosome.hpp: Interface for Chromosome class/structure.
 *
 * (c) Copyright 2015-2021, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * Created on : Jan 06, 2015 by andrade.
 * Last update: Jan 05, 2018 by andrade.
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
 ******************************************************************************/

#ifndef BRKGA_MP_IPR_CHROMOSOME_HPP_
#define BRKGA_MP_IPR_CHROMOSOME_HPP_

#include <vector>

namespace BRKGA {

/**
 * \brief Chromosone representation.
 *
 * The chromosome is represented using a vector in the unitary hypercube, i.e.,
 * \f$v \in [0,1]^n\f$ where \f$n\f$ is the size of the array (dimensions of the
 * hypercube). We use double precision because float precision maybe not be
 * enough for some applications.
 *
 * We could use `std::vector<double>` directly. However, using `typedef`, we can
 * add additional capabilities to the Chromosome class in the future, such as
 * parenting track. For example, we may want to do this:
 *
 * \code{.cpp}
 * class Chromosome: public std::vector<double> {
 *     public:
 *         Chromosome() :
 *             std::vector<double>(), my_personal_data(0.0)
 *             {}
 *     public:
 *         double my_personal_data;
 * };
 * \endcode
 *
 * to keep track of some data within a specifc chromosome.
 *
 * In general, most people do not recommend to inherit publicly from
 * `std::vector` because it has no virtual destructor. However, we may do that
 * as long as:
 *
 *   a) We remember that every operation provided by `std::vector` must be a
 *   semantically valid operation on an object of the derived class;
 *
 *   b) We avoid creating derived class objects with dynamic storage duration;
 *
 *   c) We **DO AVOID** polymorphism:
 *      \code{.cpp}
 *      std::vector<double>* pt = new Chromosome();     // Bad idea
 *      delete pt;      // Delete does not call the Chromosome destructor
 *      \endcode
 */
typedef std::vector<double> Chromosome;

} // end namespace BRKGA_MP_IPR

#endif // BRKGA_MP_IPR_CHROMOSOME_HPP_
