/*******************************************************************************
 * fitness_type.hpp: Interface for fitness_t structure.
 *
 * (c) Copyright 2021, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : May 18, 2021 by andrade.
 *  Last update: Nov 30, 2021 by andrade.
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

#ifndef BRKGA_MP_IPR_FITNESS_TYPE_HPP_
#define BRKGA_MP_IPR_FITNESS_TYPE_HPP_

#include <limits>
#include <tuple>

namespace BRKGA {

/**
 * \brief Fitness type representation.
 *
 * In general, fitness_t is a single number for mono-objective problems.
 * For instance:
 *
 * \code{.cpp}
 * typedef double fitness_t;
 * \endcode
 *
 * For multi-objective problems (with dominance/lexicographical sorting), we
 * need to use multiple values. We can either use a class, structure, or
 * `std::tuple<>`. Using a custom class or structure, we must provide copy
 * assignment and comparison operators. We have all these perks using
 * `std:tuple<>`. For example, assume we have three minimization objective
 * functions. Then we may have:
 *
 * \code{.cpp}
 * typedef std::tuple<double, double, double> fitness_t;
 * \endcode
 *
 * We must all define two const expressions `FITNESS_T_MIN` and `FITNESS_T_MAX`,
 * as we define later.
 */
typedef double fitness_t;

/**
 * \brief Minimum value of the fitness_t.
 *
 * Defines the minimum value for the fitness type. Usually, it is a simple
 * minimum value in the type. For example, if the type is a single double, we
 * can do
 *
 * \code{.cpp}
 * static constexpr fitness_t FITNESS_T_MIN = std::numeric_limits<fitness_t>::min();
 * \endcode
 *
 * For custom class/structure, we must create an object with the desired values.
 * For `std::tuple<>`, we can use tuple facilities. For example, suppose we
 * have the `std::tuple<unsigned, double>`, then:
 *
 * \code{.cpp}
 * static constexpr fitness_t FITNESS_T_MIN = {
 *     std::numeric_limits<std::tuple_element<0, fitness_t>::type>::min(),
 *     std::numeric_limits<std::tuple_element<1, fitness_t>::type>::min()
 * };
 * \endcode
 */
static constexpr fitness_t FITNESS_T_MIN = std::numeric_limits<fitness_t>::min();

/**
 * \brief Maximum value of the fitness_t.
 *
 * Defines the maximum value for the fitness type. Usually, it is a simple
 * maximum value in the type. For example, if the type is a single double, we
 * can do
 *
 * \code{.cpp}
 * static constexpr fitness_t FITNESS_T_MAX = std::numeric_limits<fitness_t>::max();
 * \endcode
 *
 * For custom class/structure, we must create an object with the desired values.
 * For `std::tuple<>`, we can use tuple facilities. For example, suppose we
 * have the `std::tuple<unsigned, double>`, then:
 *
 * \code{.cpp}
 * static constexpr fitness_t FITNESS_T_MAX = {
 *     std::numeric_limits<std::tuple_element<0, fitness_t>::type>::max(),
 *     std::numeric_limits<std::tuple_element<1, fitness_t>::type>::max()
 * };
 * \endcode
 */
static constexpr fitness_t FITNESS_T_MAX = std::numeric_limits<fitness_t>::max();

} // end namespace BRKGA_MP_IPR

#endif // BRKGA_MP_IPR_FITNESS_TYPE_HPP_
