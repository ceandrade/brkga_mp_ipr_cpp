/*******************************************************************************
 * brkga_mp_ipr.hpp: Biased Random-Key Genetic Algorithm Multi-Parent
 *                   with Implict Path Relinking.
 *
 * Created on : Jan 06, 2015 by andrade.
 * Last update: May 08, 2023 by andrade.
 *
 * (c) Copyright 2015-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * This code is released under BRKGA-MP-IPR License:
 * https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
 *
 * (c) Copyright 2010, 2011 Rodrigo F. Toso, Mauricio G.C. Resende.
 * All Rights Reserved.
 *
 * This code is released under MIT License:
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * - The above copyright notice and this permission notice shall be included
 *   in all copies or substantial portions of the Software.
 *
 * Collaborators:
 *  - Alberto Kummer, 2021 (parallel mating).
 *  - Daniele Ferone, 2023 (bug fix on IPR).
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

#ifndef BRKGA_MP_IPR_HPP_
#define BRKGA_MP_IPR_HPP_

// This includes helpers to read and write enums.
#include "third_part/enum_io.hpp"

#include "fitness_type.hpp"
#include "chromosome.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <omp.h>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdexcept>
#include <sys/time.h>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

//----------------------------------------------------------------------------//

/// If we need to include this file in multiple translation units (files) that
/// are compiled separately, we have to `inline` some functions and template
/// definitions to avoid multiple definitions and linking problems. However,
/// such inlining can make the object code grows large. In other cases, the
/// compiler may complain about too many inline functions, if you are already
/// using several inline functions.
#ifdef BRKGA_MULTIPLE_INCLUSIONS
    #define INLINE inline
#else
    #define INLINE
#endif

//----------------------------------------------------------------------------//

// These preprocessor flags determine how the mating process will happen
// regarding reproducibility. One of the following options should be used.
// If more than one is given, MATING_FULL_SPEED takes priority over
// MATING_SEED_ONLY, which takes priority over MATING_SEQUENTIAL. If no option
// is supplied, BRKGA-MP-IPR assume MATING_FULL_SPEED.
//
// At full speed, the mating process is done in parallel, using independent
// RNGs. The results are reproducible if and only if you use the same seed and
// the same number of threads.
//
// Using the following option, the mating is still parallel, but each RNG is
// seeded on each mating. This is a little bit slower than full speed, but we
// depend only on the seed, regardless of the number of threads. I.e., Runs with
// a different number of threads, but the same seed should result in the same
// sequence of decisions.
//
// Using this option, the mating process is completely sequential, as in the
// original BRKGA. The reproducibility is guaranteed with only the same seed.
// This option can be very slow for large chromosomes and large populations.
// But it makes debugging easier.
#if defined(MATING_FULL_SPEED)
    #undef MATING_SEQUENTIAL
    #undef MATING_SEED_ONLY
#elif defined(MATING_SEED_ONLY)
    #undef MATING_SEQUENTIAL
    #undef MATING_FULL_SPEED
#elif defined(MATING_SEQUENTIAL)
    #undef MATING_SEED_ONLY
    #undef MATING_FULL_SPEED
#else
    #define MATING_FULL_SPEED
#endif

/**
 * \brief This namespace contains all stuff related to BRKGA Multi Parent
 * with Implicit Path Relinking.
 */
namespace BRKGA {

//----------------------------------------------------------------------------//
// Enumerations
//----------------------------------------------------------------------------//

/// Specifies objective as minimization or maximization.
enum class Sense {
    MINIMIZE = false,  ///< Minimization.
    MAXIMIZE = true    ///< Maximization.
};

/// Holds the enumerations for Path Relinking algorithms.
namespace PathRelinking {

/// Specifies type of path relinking.
enum class Type {
    /// Changes each key for the correspondent in the other chromosome.
    DIRECT,

    /// Switches the order of a key for that in the other chromosome.
    PERMUTATION
};

/// Specifies which individuals used to build the path.
enum class Selection {
    /// Selects, in the order, the best solution of each population.
    BESTSOLUTION,

    /// Chooses uniformly random solutions from the elite sets.
    RANDOMELITE
};

/// Specifies the result type/status of path relink procedure.
enum class PathRelinkingResult {
    /// The chromosomes among the populations are too homogeneous and the path
    /// relink will not generate improveded solutions.
    TOO_HOMOGENEOUS = 0,

    /// Path relink was done but no improveded solution was found.
    NO_IMPROVEMENT = 1,

    /// An improved solution among the elite set was found, but the best
    /// solution was not improved.
    ELITE_IMPROVEMENT = 3,

    /// The best solution was improved.
    BEST_IMPROVEMENT = 7
};

/**
 *  \brief Perform bitwise `OR` between two `PathRelinkingResult` returning
 *         the highest rank `PathRelinkingResult`.
 *
 *  For example
 *  - TOO_HOMOGENEOUS | NO_IMPROVEMENT == NO_IMPROVEMENT
 *  - NO_IMPROVEMENT | ELITE_IMPROVEMENT == ELITE_IMPROVEMENT
 *  - ELITE_IMPROVEMENT | BEST_IMPROVEMENT == BEST_IMPROVEMENT
 */
inline PathRelinkingResult& operator|=(PathRelinkingResult& lhs,
                                       PathRelinkingResult rhs) {
    lhs = PathRelinkingResult(static_cast<unsigned>(lhs) |
                              static_cast<unsigned>(rhs));
    return lhs;
}
} // namespace PathRelinking

/// Specifies a bias function type when choosing parents to mating
/// (`r` is a given parameter). This function substitutes the `rhoe`
/// parameter from the original BRKGA.
enum class BiasFunctionType {
    // 1 / num. parents for mating
    /// \f$\frac{1}{\text{num. parents for mating}}\f$
    /// (all individuals have the same probability)
    CONSTANT,

    // r^-3
    /// \f$r^{-3}\f$
    CUBIC,

    // e^-r
    /// \f$e^{-r}\f$
    EXPONENTIAL,

    // 1/r
    /// \f$1/r\f$
    LINEAR,

    // 1 / log(r + 1)
    /// \f$\frac{1}{\log(r + 1)}\f$ (usually works better than other functions)
    LOGINVERSE,

    // r^-2
    /// \f$r^{-2}\f$
    QUADRATIC,

    /// Indicates a custom function supplied by the user.
    CUSTOM
};

/// Specifies the type of shaking to be performed.
enum class ShakingType {
    /// Applies the following perturbations:
    /// 1. Inverts the value of a random chosen, i.e., from `value` to
    ///    `1 - value`;
    /// 2. Assigns a random value to a random key.
    CHANGE = 0,

    /// Applies two swap perturbations:
    /// 1. Swaps the values of a randomly chosen key `i` and its
    ///    neighbor `i + 1`;
    /// 2. Swaps values of two randomly chosen keys.
    SWAP = 1
};

//----------------------------------------------------------------------------//
// Distance functions
//----------------------------------------------------------------------------//

/**
 * \brief Distance Function Base.
 *
 * This class is a interface for functors that compute the distance between
 * two vectors of double numbers.
 */
class DistanceFunctionBase {
public:
    /// Default constructor.
    DistanceFunctionBase() {}

    /// Default destructor.
    virtual ~DistanceFunctionBase() {}

    /**
     * \brief Computes the distance between two vectors.
     * \param v1 first vector
     * \param v2 second vector
     */
    virtual double distance(const std::vector<double>& v1,
                            const std::vector<double>& v2) = 0;

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key
     * \param key2 the second key
     */
    virtual bool affectSolution(const double key1, const double key2) = 0;

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by the
     *        blocks of keys `v2` affects the solution.
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
     */
    virtual bool affectSolution(
            std::vector<double>::const_iterator v1_begin,
            std::vector<double>::const_iterator v2_begin,
            const std::size_t block_size) = 0;
};

//----------------------------------------------------------------------------//

/**
 * \brief Hamming distance between two vectors.
 *
 * This class is a functor that computes the Hamming distance between two
 * vectors. It takes a threshold parameter to "binarize" the vectors.
 */
class HammingDistance: public DistanceFunctionBase {
public:
    /**
     * \brief Default constructor
     * \param _threshold used to rounding the values to 0 or 1.
     */
    explicit HammingDistance(const double _threshold = 0.5):
        threshold(_threshold) {}

    /// Default destructor
    virtual ~HammingDistance() {}

    /**
     * \brief Computes the Hamming distance between two vectors.
     * \param vector1 first vector
     * \param vector2 second vector
     */
    virtual double distance(const std::vector<double>& vector1,
                            const std::vector<double>& vector2) {
        if(vector1.size() != vector2.size())
            throw std::runtime_error("The size of the vector must "
                                     "be the same!");

        int dist = 0;
        for(std::size_t i = 0; i < vector1.size(); ++i) {
            if((vector1[i] < threshold) != (vector2[i] < threshold))
                ++dist;
        }

        return dist;
    }

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key
     * \param key2 the second key
     */
    virtual bool affectSolution(const double key1, const double key2) {
        return (key1 < threshold ? 0 : 1) != (key2 < threshold ? 0 : 1);
    }

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by the
     *        blocks of keys `v2` affects the solution.
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
     */
    virtual bool affectSolution(
            std::vector<double>::const_iterator v1_begin,
            std::vector<double>::const_iterator v2_begin,
            const std::size_t block_size) {
        for(std::size_t i = 0; i < block_size;
            ++i, ++v1_begin, ++v2_begin) {
            if((*v1_begin < threshold) != (*v2_begin < threshold))
                return true;
        }
        return false;
    }

public:
    /// Threshold parameter used to rounding the values to 0 or 1.
    double threshold;
};

//----------------------------------------------------------------------------//

/**
 * \brief Kendall Tau distance between two vectors.
 *
 * This class is a functor that computes the Kendall Tau distance between two
 * vectors. This version is not normalized.
 */
class KendallTauDistance: public DistanceFunctionBase {
public:
    /// Default constructor.
    KendallTauDistance() {}

    /// Default destructor.
    virtual ~KendallTauDistance() {}

    /**
     * \brief Computes the Kendall Tau distance between two vectors.
     * \param vector1 first vector
     * \param vector2 second vector
     */
    virtual double distance(const std::vector<double>& vector1,
                            const std::vector<double>& vector2) {
        if(vector1.size() != vector2.size())
            throw std::runtime_error("The size of the vector must "
                                     "be the same!");

        const std::size_t size = vector1.size();

        std::vector<std::pair<double, std::size_t>> pairs_v1;
        std::vector<std::pair<double, std::size_t>> pairs_v2;

        pairs_v1.reserve(size);
        std::size_t rank = 0;
        for(const auto &v : vector1)
            pairs_v1.emplace_back(v, ++rank);

        pairs_v2.reserve(size);
        rank = 0;
        for(const auto &v : vector2)
            pairs_v2.emplace_back(v, ++rank);

        std::sort(begin(pairs_v1), end(pairs_v1));
        std::sort(begin(pairs_v2), end(pairs_v2));

        unsigned disagreements = 0;
        for(std::size_t i = 0; i < size - 1; ++i) {
            for(std::size_t j = i + 1; j < size; ++j) {
                if((pairs_v1[i].second < pairs_v1[j].second
                    && pairs_v2[i].second > pairs_v2[j].second) ||
                   (pairs_v1[i].second > pairs_v1[j].second
                    && pairs_v2[i].second < pairs_v2[j].second))
                    ++disagreements;
            }
        }

        return double(disagreements);
    }

    /**
     * \brief Returns true if the changing of `key1` by `key2` affects
     *        the solution.
     * \param key1 the first key
     * \param key2 the second key
     */
    virtual bool affectSolution(const double key1, const double key2) {
        return fabs(key1 - key2) > 1e-6;
    }

    /**
     * \brief Returns true if the changing of the blocks of keys `v1` by the
     *        blocks of keys `v2` affects the solution.
     *
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
     *
     * \todo (ceandrade): implement this properly.
     */
    virtual bool affectSolution(
            std::vector<double>::const_iterator v1_begin,
            std::vector<double>::const_iterator v2_begin,
            const std::size_t block_size) {
        return block_size == 1?
              affectSolution(*v1_begin, *v2_begin) : true;
    }
};

//----------------------------------------------------------------------------//
// Population class.
//----------------------------------------------------------------------------//

/**
 * \brief Encapsulates a population of chromosomes.
 *
 * Encapsulates a population of chromosomes providing supporting methods for
 * making the implementation easier.
 *
 * \warning All methods and attributes are public and can be manipulated
 * directly from BRKGA algorithms. Note that this class is not meant to be used
 * externally of this unit.
 */
class Population {
public:
    /** \name Data members */
    //@{
    /// Population as vectors of probabilities.
    std::vector<Chromosome> population;

    /// Fitness of each chromosome.
    std::vector<std::pair<fitness_t, unsigned>> fitness;
    //@}

    /** \name Default constructors and destructor */
    //@{
    /**
     * \brief Default constructor.
     *
     * \param chr_size size of chromosome.
     * \param pop_size size of population.
     * \throw std::range_error if population size or chromosome size is zero.
     */
    Population(const unsigned chr_size, const unsigned pop_size):
        population(pop_size, Chromosome(chr_size, 0.0)),
        fitness(pop_size)
    {
        if(pop_size == 0)
            throw std::range_error("Population size cannot be zero.");

        if(chr_size == 0)
            throw std::range_error("Chromosome size cannot be zero.");
    }

    /// Copy constructor.
    Population(const Population& other):
        population(other.population),
        fitness(other.fitness)
    {}

    /// Assignment operator for complaince.
    Population& operator=(const Population&) = default;

    /// Destructor.
    ~Population() = default;
    //@}

    /** \name Simple access methods */
    //@{
    /// Returns the size of each chromosome.
    unsigned getChromosomeSize() const {
        return population[0].size();
    }

    /// Returns the size of the population.
    unsigned getPopulationSize() const {
        return population.size();
    };

    /**
     * \brief Returns a copy of an allele for a given chromosome.
     * \param chromosome index of desired chromosome.
     * \param allele index of desired allele.
     * \returns a copy of the allele value.
     */
    double operator()(const unsigned chromosome, const unsigned allele) const {
        return population[chromosome][allele];
    }

    /**
     * \brief Returns a reference for an allele for a given chromosome.
     *
     * Usually used to set the allele value.
     * \param chromosome index of desired chromosome.
     * \param allele index of desired allele.
     * \returns a reference of the allele value.
     */
    double& operator()(const unsigned chromosome, const unsigned allele) {
        return population[chromosome][allele];
    }

    /**
     * \brief Returns a reference to a chromosome.
     * \param chromosome index of desired chromosome.
     * \returns a reference to chromosome.
     */
    Chromosome& operator()(unsigned chromosome) {
        return population[chromosome];
    }
    //@}

    /** \name Special access methods
     *
     * These methods REQUIRE fitness to be sorted, and thus a call to
     * `sortFitness()` beforehand.
     */
    //@{
    /// Returns the best fitness in this population.
    fitness_t getBestFitness() const {
        return getFitness(0);
    }

    /// Returns the fitness of chromosome i.
    fitness_t getFitness(const unsigned i)  const {
        return fitness[i].first;
    }

    /// Returns a reference to the i-th best chromosome.
    Chromosome& getChromosome(unsigned i) {
        return population[fitness[i].second];
    }

    /// Returns a const reference to the i-th best chromosome.
    const Chromosome& getChromosome(const unsigned i) const {
        return population[fitness[i].second];
    }
    //@}

    /** \name Other methods */
    //@{
    /**
     * \brief Sorts `fitness` by its first parameter according to the sense.
     * \param sense Optimization sense.
     */
    void sortFitness(const Sense sense) {
        typedef std::pair<fitness_t, unsigned> local_type;
        if(sense == Sense::MAXIMIZE)
            std::sort(fitness.begin(), fitness.end(),
                      std::greater<local_type>());
        else
            std::sort(fitness.begin(), fitness.end(), std::less<local_type>());
    }

    /**
     * \brief Sets the fitness of chromosome.
     * \param chromosome index of chromosome.
     * \param value allele value.
     */
    void setFitness(const unsigned chromosome, const fitness_t value) {
        fitness[chromosome] = std::make_pair(value, chromosome);
    }
    //@}
};

//----------------------------------------------------------------------------//
// BRKGA Params class.
//----------------------------------------------------------------------------//

/**
 * \brief Represents the BRKGA and IPR hyper-parameters.
 */
class BrkgaParams {
public:
    /** \name BRKGA Hyper-parameters */
    //@{
    /// Number of elements in the population.
    unsigned population_size;

    /// Percentage of individuals to become the elite set (0, 1].
    double elite_percentage;

    /// Percentage of mutants to be inserted in the population.
    double mutants_percentage;

    /// Number of elite parents for mating.
    unsigned num_elite_parents;

    /// Number of total parents for mating.
    unsigned total_parents;

    /// Type of bias that will be used.
    BiasFunctionType bias_type;

    /// Number of independent parallel populations.
    unsigned num_independent_populations;
    //@}

    /** \name Path Relinking parameters */
    //@{
    /// Number of pairs of chromosomes to be tested to path relinking.
    unsigned pr_number_pairs;

    /// Mininum distance between chromosomes selected to path-relinking.
    double pr_minimum_distance;

    /// Path relinking type.
    PathRelinking::Type pr_type;

    /// Individual selection to path-relinking.
    PathRelinking::Selection pr_selection;

    /// Defines the block size based on the size of the population.
    double alpha_block_size;

    /// Percentage / path size to be computed. Value in (0, 1].
    double pr_percentage;
    //@}

public:
    /** \name Default operators */
    //@{
    /// Default constructor.
    BrkgaParams():
        population_size(0),
        elite_percentage(0.0),
        mutants_percentage(0.0),
        num_elite_parents(0),
        total_parents(0),
        bias_type(BiasFunctionType::CONSTANT),
        num_independent_populations(0),
        pr_number_pairs(0),
        pr_minimum_distance(0.0),
        pr_type(PathRelinking::Type::DIRECT),
        pr_selection(PathRelinking::Selection::BESTSOLUTION),
        alpha_block_size(0.0),
        pr_percentage(0.0)
    {}

    /// Assignment operator for complaince.
    BrkgaParams& operator=(const BrkgaParams&) = default;

    /// Destructor.
    ~BrkgaParams() = default;
    //@}
};

//----------------------------------------------------------------------------//
// External Control Params class.
//----------------------------------------------------------------------------//

/**
 * \brief Represents additional control parameters that can be used outside this
 * framework.
 *
 * These parameters are not used directly in the BRKGA nor in the path
 * relinking. However, they are loaded from the configuration file and can be
 * called by the user to perform out-loop controlling.
 */
class ExternalControlParams {
public:
    /// Interval at which elite chromosomes are exchanged (0 means no exchange).
    unsigned exchange_interval;

    /// Number of elite chromosomes exchanged from each population.
    unsigned num_exchange_indivuduals;

    /// Interval at which the populations are reset (0 means no reset).
    unsigned reset_interval;

public:
    /** \name Default operators */
    //@{
    /// Default constructor.
    ExternalControlParams():
        exchange_interval(0),
        num_exchange_indivuduals(0),
        reset_interval(0)
    {}

    /// Assignment operator for complaince.
    ExternalControlParams& operator=(const ExternalControlParams&) = default;

    /// Destructor.
    ~ExternalControlParams() = default;
    //@}
};

//----------------------------------------------------------------------------//
// Loading the parameters from file
//----------------------------------------------------------------------------//

/**
 * \brief Reads the parameters from a configuration file.
 *
 * \param filename the configuration file.
 * \returns a tuple containing the BRKGA and external control parameters.
 * \throw std::fstream::failure in case of errors in the file.
 * \todo (ceandrade) This method can beneficiate from introspection tools
 *   from C++17. We would like achieve a code similar to the
 *   [Julia counterpart](<https://github.com/ceandrade/brkga_mp_ipr_julia>).
 */
INLINE std::pair<BrkgaParams, ExternalControlParams>
readConfiguration(const std::string& filename) {
    std::ifstream input(filename, std::ios::in);
    std::stringstream error_msg;

    if(!input) {
        error_msg << "File '" << filename << "' cannot be opened!";
        throw std::fstream::failure(error_msg.str());
    }

    std::unordered_map<std::string, bool> tokens({
        {"POPULATION_SIZE", false},
        {"ELITE_PERCENTAGE", false},
        {"MUTANTS_PERCENTAGE", false},
        {"NUM_ELITE_PARENTS", false},
        {"TOTAL_PARENTS", false},
        {"BIAS_TYPE", false},
        {"NUM_INDEPENDENT_POPULATIONS", false},
        {"PR_NUMBER_PAIRS", false},
        {"PR_MINIMUM_DISTANCE", false},
        {"PR_TYPE", false},
        {"PR_SELECTION", false},
        {"ALPHA_BLOCK_SIZE", false},
        {"PR_PERCENTAGE", false},
        {"EXCHANGE_INTERVAL", false},
        {"NUM_EXCHANGE_INDIVUDUALS", false},
        {"RESET_INTERVAL", false}
    });

    BrkgaParams brkga_params;
    ExternalControlParams control_params;

    std::string line;
    unsigned line_count = 0;

    while(std::getline(input, line)) {
        ++line_count;
        std::string::size_type pos = line.find_first_not_of(" \t\n\v");

        // Ignore all comments and blank lines.
        if(pos == std::string::npos || line[pos] == '#')
            continue;

        std::stringstream line_stream(line);
        std::string token, data;

        line_stream >> token >> data;

        std::transform(token.begin(), token.end(), token.begin(), toupper);
        if(tokens.find(token) == tokens.end()) {
            error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                      << "Invalid token on line " << line_count
                      << ": " << token;
            throw std::fstream::failure(error_msg.str());
        }

        if(tokens[token]) {
            error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                      << "Duplicate attribute on line " << line_count
                      << ": " << token << " already read!";
            throw std::fstream::failure(error_msg.str());
        }

        std::stringstream data_stream(data);
        bool fail = false;

        // TODO: for c++17, we may use std:any to short this code using a loop.
        if(token == "POPULATION_SIZE")
            fail = !bool(data_stream >> brkga_params.population_size);
        else
        if(token == "ELITE_PERCENTAGE")
            fail = !bool(data_stream >> brkga_params.elite_percentage);
        else
        if(token == "MUTANTS_PERCENTAGE")
            fail = !bool(data_stream >> brkga_params.mutants_percentage);
        else
        if(token == "NUM_ELITE_PARENTS")
            fail = !bool(data_stream >> brkga_params.num_elite_parents);
        else
        if(token == "TOTAL_PARENTS")
            fail = !bool(data_stream >> brkga_params.total_parents);
        else
        if(token == "BIAS_TYPE")
            fail = !bool(data_stream >> brkga_params.bias_type);
        else
        if(token == "NUM_INDEPENDENT_POPULATIONS")
            fail = !bool(data_stream >> brkga_params.num_independent_populations);
        else
        if(token == "PR_NUMBER_PAIRS")
            fail = !bool(data_stream >> brkga_params.pr_number_pairs);
        else
        if(token == "PR_MINIMUM_DISTANCE")
            fail = !bool(data_stream >> brkga_params.pr_minimum_distance);
        else
        if(token == "PR_TYPE")
            fail = !bool(data_stream >> brkga_params.pr_type);
        else
        if(token == "PR_SELECTION")
            fail = !bool(data_stream >> brkga_params.pr_selection);
        else
        if(token == "ALPHA_BLOCK_SIZE")
            fail = !bool(data_stream >> brkga_params.alpha_block_size);
        else
        if(token == "PR_PERCENTAGE")
            fail = !bool(data_stream >> brkga_params.pr_percentage);
        else
        if(token == "EXCHANGE_INTERVAL")
            fail = !bool(data_stream >> control_params.exchange_interval);
        else
        if(token == "NUM_EXCHANGE_INDIVUDUALS")
            fail = !bool(data_stream >> control_params.num_exchange_indivuduals);
        else
        if(token == "RESET_INTERVAL")
            fail = !bool(data_stream >> control_params.reset_interval);

        if(fail) {
            error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                      << "Invalid value for '" << token
                      << "' on line "<< line_count
                      << ": '" << data << "'";
            throw std::fstream::failure(error_msg.str());
        }

        tokens[token] = true;
    }

    for(const auto& attribute_flag : tokens) {
        if(!attribute_flag.second) {
            error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                      << "Argument '" << attribute_flag.first
                      << "' was not supplied in the config file";
            throw std::fstream::failure(error_msg.str());
        }
    }

    return std::make_pair(std::move(brkga_params), std::move(control_params));
}

//----------------------------------------------------------------------------//
// Writing the parameters into file
//----------------------------------------------------------------------------//

/**
 * \brief Writes the parameters into a file..
 *
 * \param filename the configuration file.
 * \param brkga_params the BRKGA parameters.
 * \param control_params the external control parameters. Default is an empty
 *        object.
 * \throw std::fstream::failure in case of errors in the file.
 * \todo (ceandrade) This method can beneficiate from introspection tools
 *   from C++17. We would like achieve a code similar to the
 *   [Julia counterpart](<https://github.com/ceandrade/brkga_mp_ipr_julia>).
 */
INLINE void writeConfiguration(const std::string& filename,
        const BrkgaParams& brkga_params,
        const ExternalControlParams& control_params = ExternalControlParams()) {

    std::ofstream output(filename, std::ios::out);
    if(!output) {
        std::stringstream error_msg;
        error_msg << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
                  << "File '" << filename << "' cannot be opened!";
        throw std::fstream::failure(error_msg.str());
    }

    output << "population_size " << brkga_params.population_size << "\n"
           << "elite_percentage " << brkga_params.elite_percentage << "\n"
           << "mutants_percentage " << brkga_params.mutants_percentage << "\n"
           << "num_elite_parents " << brkga_params.num_elite_parents << "\n"
           << "total_parents " << brkga_params.total_parents << "\n"
           << "bias_type " << brkga_params.bias_type << "\n"
           << "num_independent_populations "
           << brkga_params.num_independent_populations << "\n"
           << "pr_number_pairs " << brkga_params.pr_number_pairs << "\n"
           << "pr_minimum_distance " << brkga_params.pr_minimum_distance << "\n"
           << "pr_type " << brkga_params.pr_type << "\n"
           << "pr_selection " << brkga_params.pr_selection << "\n"
           << "alpha_block_size " << brkga_params.alpha_block_size << "\n"
           << "pr_percentage " << brkga_params.pr_percentage << "\n"
           << "exchange_interval " << control_params.exchange_interval << "\n"
           << "num_exchange_indivuduals "
           << control_params.num_exchange_indivuduals << "\n"
           << "reset_interval " << control_params.reset_interval
           << std::endl;

    output.close();
}

//----------------------------------------------------------------------------//
// Functions for equality comparisons
//----------------------------------------------------------------------------//

namespace {

/** \name Functions for equality comparisons
 *
 * This is a helper function that, at compiler time, detect if the `fitness_t`
 * is a floating point type, and applies the absolute diference. Otherwise,
 * the compiler generates the equality comparison.
 */
//@{

/**
 * \brief Compare two values to equality.
 *
 * If these values are real numbers, we compare their absolute difference with
 * `EQUALITY_THRESHOLD`, i.e., \f$|a - b| < EQUALITY\_THRESHOLD\f$. In other
 * cases (except tuples, which have a specialization), we use the `operator==`
 * directly. Therefore, `fitness_t` must define `operator==`.
 *
 * \param a First item to be compared.
 * \param b Second item to be compared.
 * \return true if a and b are equal.
 */
template <class T>
constexpr bool close_enough(T a, T b) {
   if constexpr (std::is_floating_point_v<T>)
      return fabs(a - b) < EQUALITY_THRESHOLD;
   else
      return a == b;
}

/**
 * \brief Compare two tuples to equality.
 *
 * This specialization iterates, recursively, of each tuples' members and
 * compares them. Note that this is done in compilation time, with no impact at
 * running time.
 *
 * \todo Could we implement this using C++17 template folding?
 *
 * \param a First tuple to be compared.
 * \param b Second tuple to be compared.
 * \return true if a and b are equal.
 */
template <size_t I = 0, typename T, typename... Ts>
constexpr bool close_enough(std::tuple<T, Ts...> a, std::tuple<T, Ts...> b)
{
    // If we have iterated through all elements, just return true.
    if constexpr(I == sizeof...(Ts) + 1)
        return true;
    else
        return close_enough(std::get<I>(a), std::get<I>(b)) &&
               close_enough<I + 1>(a, b);
}
//@}
}

//----------------------------------------------------------------------------//
// The Multi-Parent Biased Random-key Genetic Algorithm with Implicit
// Path Relinking
//----------------------------------------------------------------------------//

/**
 * \brief This class represents a Multi-Parent Biased Random-key Genetic
 * Algorithm with Implicit Path Relinking (BRKGA-MP-IPR).
 *
 * \author Carlos Eduardo de Andrade <ce.andrade@gmail.com>
 * \date 2021
 *
 * Main capabilities {#main_cap}
 * ========================
 *
 * Evolutionary process {#evol_process}
 * ------------------------
 *
 * In the BRKGA-MP-IPR, we keep a population of chromosomes divided between the
 * elite and the non-elite group. During the mating, multiple parents are chosen
 * from the elite group and the non-elite group. They are sorted either on
 * no-decreasing order for minimization or non-increasing order to maximization
 * problems. Given this order, a bias function is applied to the rank of each
 * chromosome, resulting in weight for each one. Using a roulette method based
 * on the weights, the chromosomes are combined using a biased crossover.
 *
 * This code also implements the island model, where multiple populations
 * can be evolved in parallel, and migration between individuals between
 * the islands are performed using exchangeElite() method.
 *
 * This code requires the template argument `Decoder` be a class or functor
 * object capable to map a chromosome to a solution for the specific problem,
 * and return a value to be used as fitness to the decoded chromosome.
 * The decoder must have the method
 * \code{.cpp}
 *      double decode(Chromosome& chromosome, bool rewrite);
 * \endcode
 *
 * where #Chromosome is a `vector<double>` representing a solution and
 * `rewrite` is a boolean indicating that if the decode should rewrite the
 * chromosome in case it implements local searches and modifies the initial
 * solution decoded from the chromosome. Since this API has the capability of
 * decoding several chromosomes in parallel, the user must guarantee that
 * `Decoder::decode(...)` is thread-safe. Therefore, we do recommend to have
 * the writable variables per thread. Please, see the example that follows this
 * code.
 *
 * Implicit Path Relinking {#ipr}
 * ------------------------
 *
 * This API also implements the Implicit Path Relinking leveraging the decoder
 * capabilities. To perform the path relinking, the user must call pathRelink()
 * method, indicating the type of path relinking (direct or permutation-based,
 * see #PathRelinking::Type), the selection criteria (best solution or random
 * elite, see #PathRelinking::Selection), the distance function
 * (to choose individuals far enough, see BRKGA::DistanceFunctionBase,
 * BRKGA::HammingDistance, and BRKGA::KendallTauDistance), a maximum time or
 * a maximum path size.
 *
 * In the presence of multiple populations, the path relinking is performed
 * between elite chromosomes from different populations, in a circular fashion.
 * For example, suppose we have 3 populations. The framework performs 3 path
 * relinkings: the first between individuals from populations 1 and 2, the
 * second between populations 2 and 3, and the third between populations 3
 * and 1. In the case of just one population, both base and guiding individuals
 * are sampled from the elite set of that population.
 *
 * Note that the algorithm tries to find a pair of base and guiding solutions
 * with a minimum distance given by the distance function. If this is not
 * possible, a new pair of solutions are sampled (without replacement) and
 * tested against the distance. In case it is not possible to find such pairs
 * for the given populations, the algorithm skips to the next pair of
 * populations (in a circular fashion, as described above). Yet, if such pairs
 * are not found in any case, the algorithm declares failure. This indicates
 * that the populations are very homogeneous.
 *
 * The API will call `Decoder::decode()` always
 * with `rewrite = false`. The reason is that if the decoder rewrites the
 * chromosome, the path between solutions is lost and inadvertent results may
 * come up. Note that at the end of the path relinking, the method calls the
 * decoder with `rewrite = true` in the best chromosome found to guarantee
 * that this chromosome is re-written to reflect the best solution found.
 *
 * Other capabilities {#other_cap}
 * ========================
 *
 * Multi-start {#multi_start}
 * ------------------------
 *
 * This API also can be used as a simple multi-start algorithm without
 * evolution. To do that, the user must supply in the constructor the argument
 * `evolutionary_mechanism_on = false`. This argument makes the elite set has
 * one individual and the number of mutants n - 1, where n is the size of the
 * population. This setup disables the evolutionary process completely.
 *
 * Initial Population {#init_pop}
 * ------------------------
 *
 * This API allows the user provides a set of initial solutions to warm start
 * the algorithm. In general, initial solutions are created using other (fast)
 * heuristics and help the convergence of the BRKGA. To do that, the user must
 * encode the solutions on #Chromosome (`vector<double>`) and pass to the method
 * setInitialPopulation() as a `vector<#Chromosome>`.
 *
 * General Usage {#gen_usage}
 * ========================
 *
 *  -# The user must call the BRKGA_MP_IPR constructor and pass the desired
 *     parameters. Please, see BRKGA_MP_IPR::BRKGA_MP_IPR for parameter details;
 *
 *      -# (Optional) The user provides the warm start solutions using
 *         setInitialPopulation();
 *
 *  -# The user must call the method initialize() to start the data structures
 *     and perform the decoding of the very first populations;
 *
 *  -# Main evolutionary loop:
 *
 *      -# On each iteration, the method evolve() should be called to perform
 *         the evolutionary step (or multi-steps if desired);
 *
 *      -# The user can check the current best chromosome (getBestChromosome())
 *         and its fitness (getBestFitness()) and perform checking and logging
 *         operations;
 *
 *      -# (Optional) The user can perform the individual migration between
 *         populations (exchangeElite());
 *
 *      -# (Optional) The user can perform the path relinking (pathRelink());
 *
 *      -# (Optional) The user can reset and start the algorithm over (reset());
 *
 * For a comprehensive and detailed usage, please see the examples that follow
 * this API.
 *
 * About multi-threading {#multi_thread}
 * ========================
 *
 * This API is capable of decoding several chromosomes in
 * parallel, as mentioned before. This capability is based on OpenMP
 * (<http://www.openmp.org>) and the compiler must have support to it.
 * Most recent versions of GNU G++ and Intel C++ Compiler support OpenMP.
 * Clang supports OpenMP since 3.8. However, there are some issues with the
 * libraries, and sometimes, the parallel sections are not enabled. On the
 * major, the user can find fixes to his/her system.
 *
 * Since, in general, the decoding process can be complex and lengthy, it is
 * recommended that **the number of threads used DO NOT exceed the number of
 * physical cores in the machine.** This improves the overall performance
 * drastically, avoiding cache misses and racing conditions. Note that the
 * number of threads is also tied to the memory utilization, and it should be
 * monitored carefully.
 *
 * History {#hist}
 * ========================
 *
 * This API was based on the code by Rodrigo Franco Toso, Sep 15, 2011.
 * http://github.com/rfrancotoso/brkgaAPI
 *
 */
template <class Decoder>
class BRKGA_MP_IPR {
public:
    /** \name Constructors and destructor */
    //@{
    /**
     * \brief Builds the algorithm and its data strtuctures with the given
     *        arguments.
     *
     * \param decoder_reference a reference to the decoder object.
     *        **NOTE:** BRKGA uses such object directly for decoding.
     * \param sense the optimization sense (maximization or minimization).
     * \param seed the seed for the random number generator.
     * \param chromosome_size number of genes in each chromosome.
     * \param params BRKGA and IPR parameters object loaded from a
     *        configuration file or manually created. All the data is copied.
     * \param max_threads number of threads to perform parallel decoding.\n
     *        **NOTE**: `Decoder::decode()` MUST be thread-safe.
     * \param evolutionary_mechanism_on if false, no evolution is performed
     *        but only chromosome decoding. Very useful to emulate a
     *        multi-start algorithm.
     *
     * \throw std::range_error if some parameter or combination of parameters
     *        does not fit.
     */
    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        const Sense sense,
        const unsigned seed,
        const unsigned chromosome_size,
        const BrkgaParams& params,
        const unsigned max_threads = 1,
        const bool evolutionary_mechanism_on = true);

    /// Destructor
    ~BRKGA_MP_IPR() {}
    //@}

    /** \name Initialization methods */
    //@{
    /**
     * \brief Sets individuals to initial population.
     *
     * Set initial individuals into the poulation to work as warm-starters.
     * Such individuals can be obtained from solutions of external procedures
     * such as fast heuristics, other methaheuristics, or even relaxations from
     * a mixed integer programming model that models the problem.
     *
     * We assign as many individuals as possible across all populations.
     * Extra individuals are disregarded.
     *
     * \param chromosomes a set of individuals encoded as Chromosomes.
     */
    void setInitialPopulation(const std::vector<Chromosome>& chromosomes);

    /**
     * \brief Sets a custom bias function used to build the probabilities.
     *
     * It must be a **positive non-increasing function**, i.e.
     * \f$ f: \mathbb{N}^+ \to \mathbb{R}^+\f$ such that
     * \f$f(i) \ge 0\f$ and \f$f(i) \ge f(i+1)\f$ for
     * \f$i \in [1..total\_parents]\f$.
     * For example
     * \code{.cpp}
     *      setBiasCustomFunction(
     *          [](const unsigned x) {
     *              return 1.0 / (x * x);
     *          }
     *      );
     * \endcode
     * sets an inverse quadratic function.
     *
     * \param func a reference to a unary positive non-increasing function.
     * \throw std::runtime_error in case the function is not a non-decreasing
     *        positive function.
     */
    void setBiasCustomFunction(const std::function<double(const unsigned)>& func);

    /**
     * \brief Initializes the populations and others parameters of the
     *        algorithm.
     *
     * If a initial population is supplied, this method completes the remain
     * individuals, if they do not exist. This method also performs the initial
     * decoding of the chromosomes. Therefore, depending on the decoder
     * implementation, this can take a while, and the user may want to time
     * such procedure in his/her experiments.
     *
     * \warning
     *      This method must be call before any evolutionary or population
     *      handling method.
     *
     * \warning
     *     As it is in #evolve(), the decoding is done in parallel using
     *     threads, and the user **must guarantee that the decoder is
     *     THREAD-SAFE.** If such property cannot be held, we suggest using
     *     a single thread  for optimization.
     *
     * \param reset when set true, it ignores all solutions provided
     *        by `setInitialPopulation()`, and builds a completely random
     *        population. This parameter is set to true during reset phase.
     *        When false, `initialize()` considers all solutions given by
     *        `setInitialPopulation()`.
     */
    void initialize(bool reset = false);
    //@}

    /** \name Evolution */
    //@{
    /**
     * \brief Evolves the current populations following the guidelines of
     *        Multi-parent BRKGAs.
     *
     * \warning
     *     The decoding is done in parallel using threads, and the user **must
     *     guarantee that the decoder is THREAD-SAFE.** If such property cannot
     *     be held, we suggest using a single thread for optimization.
     *
     * \param generations number of generations to be evolved. Must be larger
     *        than zero.
     * \throw std::runtime_error if the algorithm is not initialized.
     * \throw std::range_error if the number of generations is zero.
     */
    void evolve(unsigned generations = 1);
    //@}

    /** \name Path relinking */
    //@{
    /**
     * \brief Performs path relinking between elite solutions that are, at least,
     * a given minimum distance between themselves. In this method, the
     * local/loaded parameters are ignored in favor to the supplied ones.
     *
     * In the presence of multiple populations, the path relinking is performed
     * between elite chromosomes from different populations, in a circular
     * fashion. For example, suppose we have 3 populations. The framework
     * performs 3 path relinkings: the first between individuals from
     * populations 1 and 2, the second between populations 2 and 3, and the
     * third between populations 3 and 1. In the case of just one population,
     * both base and guiding individuals are sampled from the elite set of that
     * population.
     *
     * Note that the algorithm tries to find a pair of base and guiding
     * solutions with a minimum distance given by the distance function. If this
     * is not possible, a new pair of solutions are sampled (without
     * replacement) and tested against the distance. In case it is not possible
     * to find such pairs for the given populations, the algorithm skips to the
     * next pair of populations (in a circular fashion, as described above).
     * Yet, if such pairs are not found in any case, the algorithm declares
     * failure. This indicates that the populations are very homogeneous.
     *
     * If the found solution is the best solution found so far, IPR replaces the
     * worst solution by it. Otherwise, IPR computes the distance between the
     * found solution and all other solutions in the elite set, and replaces the
     * worst solution by it if and only if the found solution is, at least,
     * `minimum_distance` from all them.
     *
     * The API will call `Decoder::decode()` function always with `rewrite =
     * false`. The reason is that if the decoder rewrites the chromosome, the
     * path between solutions is lost and inadvertent results may come up. Note
     * that at the end of the path relinking, the method calls the decoder with
     * `rewrite = true` in the best chromosome found to guarantee that this
     * chromosome is re-written to reflect the best solution found.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)` additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * \warning
     *     As it is in #evolve(), the decoding is done in parallel using
     *     threads, and the user **must guarantee that the decoder is
     *     THREAD-SAFE.** If such property cannot be held, we suggest using
     *     a single thread  for optimization.
     *
     * \param pr_type type of path relinking to be performed.
     *        See PathRelinking::Type.
     * \param pr_selection of which individuals use to path relinking.
     *        See PathRelinking::Selection.
     * \param dist a pointer to a functor/object to compute the distance between
     *        two chromosomes. This object must be inherited from
     *        BRKGA::DistanceFunctionBase and implement its methods.
     * \param number_pairs number of chromosome pairs to be tested.
     *        If 0, all pairs are tested.
     * \param minimum_distance between two chromosomes computed byt `dist`.
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage defines the size, in percentage, of the path to
     *        build. Default: 1.0 (100%).
     *
     * \returns A PathRelinking::PathRelinkingResult depending on the relink
     *          status.
     *
     * \throw std::range_error if the percentage or size of the path is
     *        not in (0, 1].
     */
    PathRelinking::PathRelinkingResult pathRelink(
                    PathRelinking::Type pr_type,
                    PathRelinking::Selection pr_selection,
                    std::shared_ptr<DistanceFunctionBase> dist,
                    unsigned number_pairs,
                    double minimum_distance,
                    std::size_t block_size = 1,
                    long max_time = 0,
                    double percentage = 1.0);

    /**
     * \brief Performs path relinking between elite solutions that are,
     *        at least, a given minimum distance between themselves.
     *
     * This method uses all parameters supplied in the constructor.
     * In particular, the block size is computed by
     * \f$\lceil \alpha \times \sqrt{p} \rceil\f$
     * where \f$\alpha\f$ is BrkgaParams#alpha_block_size and
     * \f$p\f$ is BrkgaParams#population_size.
     * If the size is larger than the chromosome size, the size is set to
     * half of the chromosome size.
     *
     * Please, refer to #pathRelink() for details.
     *
     * \param dist a pointer to a functor/object to compute the distance between
     *        two chromosomes. This object must be inherited from
     *        BRKGA::DistanceFunctionBase and implement its methods.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     *
     * \returns A PathRelinking::PathRelinkingResult depending on the relink
     *          status.
     *
     * \throw std::range_error if the percentage or size of the path is
     *        not in (0, 1].
     */
    PathRelinking::PathRelinkingResult pathRelink(
                    std::shared_ptr<DistanceFunctionBase> dist,
                    long max_time = 0);
    //@}

    /** \name Population manipulation methods */
    //@{
    /**
     * \brief Exchanges elite-solutions between the populations.

     * Given a population, the `num_immigrants` best solutions are copied to
     * the neighbor populations, replacing their worth solutions. If there is
     * only one population, nothing is done.

     * \param num_immigrants number of elite chromosomes to select from each
     *      population.
     * \throw std::range_error if the number of immigrants less than one or
     *        it is larger than or equal to the population size divided by
     *        the number of populations minus one, i.e. \f$\lceil
     *        \frac{population\_size}{num\_independent\_populations} \rceil
     *         - 1\f$.
     */
    void exchangeElite(unsigned num_immigrants);

    /**
     * \brief Resets all populations with brand new keys.
     *
     * All warm-start solutions provided setInitialPopulation() are discarded.
     * You may use injectChromosome() to insert those solutions again.
     * \throw std::runtime_error if the algorithm is not initialized.
     */
    void reset();

    /**
     * \brief Performs a shaking in the chosen population.
     * \param intensity the intensity of the shaking.
     * \param shaking_type either `CHANGE` or `SWAP` moves.
     * \param population_index the index of the population to be shaken. If
     * `population_index >= num_independent_populations`, all populations
     * are shaken.
     */
    void shake(unsigned intensity, ShakingType shaking_type,
               unsigned population_index =
                    std::numeric_limits<unsigned>::infinity());

    /**
     * \brief Injects/Replaces a chromosome of a given population into a
     *         given position.
     *
     * The new chromosome replaces the old one, and the decoder is triggered to
     * compute the new fitness. Once done, the population is re-sorted
     * according to the chromosomes' fitness.
     *
     * \param chromosome the chromosome to be injected.
     * \param population_index the population index.
     * \param position the chromosome position.
     *
     * \throw std::range_error eitheir if `population_index` is larger
     *        than number of populations; or `position` is larger than the
     *        population size; or ` chromosome.size() != chromosome_size`
     */
    void injectChromosome(const Chromosome& chromosome,
                          unsigned population_index,
                          unsigned position);
    //@}

    /** \name Support methods */
    //@{
    /**
     * \brief Returns a reference to a current population.
     * \param population_index the population index.
     * \throw std::range_error if the index is larger than number of
     *        populations.
     */
    const Population& getCurrentPopulation(unsigned population_index = 0) const;

    /// Returns a reference to the chromosome with best fitness so far among
    /// all populations.
    const Chromosome& getBestChromosome() const;

    /// Return the best fitness found so far among all populations.
    fitness_t getBestFitness() const;

    /**
     * \brief Returns a reference to a chromosome of the given population.
     * \param population_index the population index.
     * \param position the chromosome position, ordered by fitness.
     *        The best chromosome is located in position 0.
     * \throw std::range_error eitheir if `population_index` is larger
     *        than number of populations, or `position` is larger than the
     *        population size.
     */
    const Chromosome& getChromosome(unsigned population_index,
                                    unsigned position) const;

    /**
     * \brief Returns the fitness of a chromosome of the given population.
     * \param population_index the population index.
     * \param position the chromosome position, ordered by fitness.
     *        The best chromosome is located in position 0.
     * \throw std::range_error eitheir if `population_index` is larger
     *        than number of populations, or `position` is larger than the
     *        population size.
     */
    fitness_t getFitness(unsigned population_index, unsigned position) const;
    //@}

    /** \name Parameter getters */
    //@{
    const BrkgaParams& getBrkgaParams() const { return params; }

    Sense getOptimizationSense() const { return OPT_SENSE; }

    unsigned getChromosomeSize() const { return CHROMOSOME_SIZE; }

    unsigned getEliteSize() const { return elite_size; }

    unsigned getNumMutants() const { return num_mutants; }

    bool evolutionaryIsMechanismOn() const { return evolutionary_mechanism_on; }

    unsigned getMaxThreads() const { return MAX_THREADS; }
    //@}

protected:
    /** \name BRKGA Hyper-parameters */
    //@{
    /// The BRKGA and IPR hyper-parameters.
    BrkgaParams params;

    /// Indicates whether we are maximizing or minimizing.
    const Sense OPT_SENSE;

    /// Number of genes in the chromosome.
    const unsigned CHROMOSOME_SIZE;

    /// Number of elite items in the population.
    unsigned elite_size;

    /// Number of mutants introduced at each generation into the population.
    unsigned num_mutants;

    /// If false, no evolution is performed but only chromosome decoding.
    /// Very useful to emulate a multi-start algorithm.
    bool evolutionary_mechanism_on;
    //@}

    /** \name Parallel computing parameters */
    //@{
    /// Number of threads for parallel decoding.
    const unsigned MAX_THREADS;
    //@}

protected:
    /** \name Engines */
    //@{
    /// Reference to the problem-dependent Decoder.
    Decoder& decoder;

    /// Mersenne twister random number generators. For parallel mating, we must
    /// have one RNG per thread so that we can reproduce the results of an
    /// experiment. We use the first RNG as the main generator in several parts
    /// of the code. The other RNGs are used only during mating.
    std::vector<std::mt19937> rng_per_thread;
    //@}

    /** \name Algorithm data */
    //@{
    /// Previous populations.
    std::vector<std::shared_ptr<Population>> previous;

    /// Current populations.
    std::vector<std::shared_ptr<Population>> current;

    /// Reference for the bias function.
    std::function<double(const unsigned)> bias_function;

    /// Holds the sum of the results of each raking given a bias function.
    /// This value is needed to normalization.
    double total_bias_weight;

    #ifdef MATING_SEED_ONLY
    /// During parallel mating, we need to be sure that the same random values
    /// are generated in each mating, despite the number of threads available.
    /// Therefore, on each iteration, we generate a set of seeds for each RNG.
    std::vector<std::mt19937::result_type> mating_seeds;
    #endif

    /// Used to shuffled individual/chromosome indices during the mate.
    /// We have one for each thread during parallel mating.
    std::vector<std::vector<unsigned>> shuffled_individuals_per_thread;

    /// Defines the order of parents during the mating.
    /// We have one for each thread during parallel mating.
    std::vector<std::vector<std::pair<fitness_t, unsigned>>>
    parents_ordered_per_thread;

    /// Temporary structures that hold the offsrping per thread. Used
    /// to reduce the caching missing, and speed up the mating.
    std::vector<std::vector<double>> offspring_per_thread;

    /// Indicates if a initial population is set.
    bool initial_population;

    /// Indicates if the algorithm was proper initialized.
    bool initialized;

    /// Holds the start time for a call of the path relink procedure.
    std::chrono::system_clock::time_point pr_start_time;
    //@}

protected:
    /** \name Core local methods */
    //@{
    /**
     * \brief Evolves the current population to the next.
     *
     * Note that the next population will be re-populate completely.
     *
     * \param[in] curr current population.
     * \param[out] next next population.
     */
    void evolution(Population& curr, Population& next);

    /**
     * \brief Performs the direct path relinking.
     *
     * This method changes each allele or block of alleles of base chromosome
     * for the correspondent one in the guide chromosome.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)` additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * \param chr1 first chromosome.
     * \param chr2 second chromosome
     * \param dist distance functor (distance between two chromosomes).
     * \param[out] best_found best solution found in the search.
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time abort path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage define the size, in percentage, of the path to
     *        build. Default: 1.0 (100%).
     */
    void directPathRelink(const Chromosome& chr1, const Chromosome& chr2,
                          std::shared_ptr<DistanceFunctionBase> dist,
                          std::pair<fitness_t, Chromosome>& best_found,
                          std::size_t block_size,
                          long max_time,
                          double percentage);

    /**
     * \brief Performs the permutation-based path relinking.
     *
     * In this method, the permutation induced by the keys in the guide
     * solution is used to change the order of the keys in the permutation
     * induced by the base solution.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * `O(chromosome_size^2 / block_size)` additional memory is necessary to
     * build the candidates, which can be costly if the `chromosome_size` is
     * very large.
     *
     * The path relinking is performed by changing the order of
     * each allele of base chromosome for the correspondent one in
     * the guide chromosome.
     * \param chr1 first chromosome
     * \param chr2 second chromosome
     * \param dist distance functor (distance between two chromosomes)
     * \param[out] best_found best solution found in the search
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time abort path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage define the size, in percentage, of the path to
     *        build. Default: 1.0 (100%)
     */
    void permutatioBasedPathRelink(Chromosome& chr1, Chromosome& chr2,
                                   std::shared_ptr<DistanceFunctionBase> dist,
                                   std::pair<fitness_t, Chromosome>& best_found,
                                   std::size_t block_size,
                                   long max_time,
                                   double percentage);
    //@}

    /** \name Helper functions */
    //@{
    /**
     * \brief Returns `true` if `a1` is better than `a2`.
     *
     * This method depends on the optimization sense. When the optimization
     * sense is `Sense::MINIMIZE`, `a1 < a2` will return true, otherwise false.
     * The opposite happens for `Sense::MAXIMIZE`.
     */
    inline bool betterThan(const fitness_t& a1, const fitness_t& a2) const;

    /**
     * Distributes real values of given precision across [0, 1] evenly.
     * \param rng The random number generator to be used.
     */
    inline double rand01(std::mt19937& rng);

    /**
     * Returns an integer number between `0` and `n`.
     * \param n The upper bound.
     * \param rng The random number generator to be used.
     */
    inline uint_fast32_t randInt(const uint_fast32_t n, std::mt19937& rng);
    //@}
};

//----------------------------------------------------------------------------//

template <class Decoder>
BRKGA_MP_IPR<Decoder>::BRKGA_MP_IPR(
        Decoder& _decoder_reference,
        const Sense _sense,
        unsigned _seed,
        unsigned _chromosome_size,
        const BrkgaParams& _params,
        const unsigned _max_threads,
        const bool _evolutionary_mechanism_on):

        // Algorithm parameters.
        params(_params),
        OPT_SENSE(_sense),
        CHROMOSOME_SIZE(_chromosome_size),
        elite_size(_evolutionary_mechanism_on?
                    unsigned(params.elite_percentage *
                             params.population_size)
                    : 1),
        num_mutants(_evolutionary_mechanism_on?
                    unsigned(params.mutants_percentage *
                             params.population_size):
                    params.population_size - 1),
        evolutionary_mechanism_on(_evolutionary_mechanism_on),
        MAX_THREADS(_max_threads),

        // Internal data.
        decoder(_decoder_reference),
        rng_per_thread(_max_threads),
        previous(params.num_independent_populations, nullptr),
        current(params.num_independent_populations, nullptr),
        bias_function(),
        total_bias_weight(0.0),
        #ifdef MATING_SEED_ONLY
            mating_seeds(params.population_size - elite_size - num_mutants, 0),
        #endif
        #ifndef MATING_SEQUENTIAL
            shuffled_individuals_per_thread(
                _max_threads,
                typename decltype(shuffled_individuals_per_thread)
                    ::value_type(params.population_size)
            ),
            parents_ordered_per_thread(
                _max_threads,
                typename decltype(parents_ordered_per_thread)
                    ::value_type(params.total_parents)
            ),
            offspring_per_thread(
                _max_threads,
                typename decltype(offspring_per_thread)
                    ::value_type(_chromosome_size)
            ),
        #else
            shuffled_individuals_per_thread(1,
                typename decltype(shuffled_individuals_per_thread)
                    ::value_type(params.population_size)
            ),
            parents_ordered_per_thread(1,
                typename decltype(parents_ordered_per_thread)
                    ::value_type(params.total_parents)
            ),
            offspring_per_thread(1,
                typename decltype(offspring_per_thread)
                    ::value_type(_chromosome_size)
            ),
        #endif
        initial_population(false),
        initialized(false),
        pr_start_time()
{
    using std::range_error;
    std::stringstream ss;

    if(CHROMOSOME_SIZE < 2)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Chromosome size must be larger than one, current size: "
           << CHROMOSOME_SIZE;
    else
    if(params.population_size == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Population size must be larger than zero, current size: "
           << params.population_size;
    else
    if(elite_size == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite-set size equals zero.";
    else
    if(elite_size > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite-set size (" << elite_size
           << ") greater than population size (" << params.population_size << ")";
    else
    if(num_mutants > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Mutant-set size (" << num_mutants
           << ") greater than population size (" << params.population_size << ")";
    else
    if(elite_size + num_mutants > params.population_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Elite (" << elite_size << ") + mutant sets (" << num_mutants
           << ") greater than population size (" << params.population_size << ")";
    else
    if(params.num_elite_parents < 1)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "num_elite_parents must be at least 1, current: "
           << params.num_elite_parents;
    else
    if(params.total_parents < 2)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Total_parents must be at least 2, current: " << params.total_parents;
    else
    if(params.num_elite_parents >= params.total_parents)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Num_elite_parents (" << params.num_elite_parents << ") "
           << "is greater than total_parents (" << params.total_parents << ")";
    else
    if(params.num_elite_parents > elite_size)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Num_elite_parents (" << params.num_elite_parents
           << ") is greater than elite set (" << elite_size << ")";
    else
    if(params.num_independent_populations == 0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Number of independent/parallel populations cannot be zero.";
    else
    if(params.alpha_block_size < 1e-6)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "(alpha) block size <= 0.0";
    else
    if(params.pr_percentage < 1e-6 || params.pr_percentage > 1.0)
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Path relinking percentage (" << params.pr_percentage
           << ") is not in the range (0, 1].";

    const auto str_error = ss.str();
    if(str_error.length() > 0)
        throw range_error(str_error);

    // Chooses the bias function.
    switch(params.bias_type) {
    case BiasFunctionType::LOGINVERSE:
        // Same as log(r + 1), but avoids precision loss.
        setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / log1p(r); }
        );
        break;

    case BiasFunctionType::LINEAR:
        setBiasCustomFunction(
            [](const unsigned r) { return 1.0 / r; }
        );
        break;

    case BiasFunctionType::QUADRATIC:
        setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -2); }
        );
        break;

    case BiasFunctionType::CUBIC:
        setBiasCustomFunction(
            [](const unsigned r) { return pow(r, -3); }
        );
        break;

    case BiasFunctionType::EXPONENTIAL:
        setBiasCustomFunction(
            [](const unsigned r) { return exp(-1.0 * r); }
        );
        break;

    case BiasFunctionType::CONSTANT:
    default:
        setBiasCustomFunction(
            [&](const unsigned) { return 1.0 / params.total_parents; }
        );
        break;
    }

    // We could initialize each RNG with the same seed. However, this can skew
    // the mating process slightly because we would have the same choices for
    // groups of alleles in the same iteration. Therefore, we just initialize
    // the RNGs in a chain so that they have different states, although still
    // reproducible. So, we start with the given sedd for the first RNG.
    rng_per_thread[0].seed(_seed);
    rng_per_thread[0].discard(rng_per_thread[0].state_size);

    // For the other RNGs, we use the state of the previous RNG as seed.
    #ifndef MATING_SEQUENTIAL
        for(size_t i = 1; i < rng_per_thread.size(); ++i) {
            auto& local_rng = rng_per_thread[i];
            local_rng.seed(rng_per_thread[i - 1]());
            local_rng.discard(local_rng.state_size);
        }
    #endif
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline bool BRKGA_MP_IPR<Decoder>::betterThan(const fitness_t& a1,
                                              const fitness_t& a2) const {
    return (a1 < a2) == (OPT_SENSE == Sense::MINIMIZE);
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Population&
BRKGA_MP_IPR<Decoder>::getCurrentPopulation(unsigned population_index) const {
    if(population_index >= current.size())
        throw std::range_error("The index is larger than number of "
                               "populations");
    return (*current[population_index]);
}

//----------------------------------------------------------------------------//

template <class Decoder>
fitness_t BRKGA_MP_IPR<Decoder>::getBestFitness() const {
    fitness_t best = current[0]->fitness[0].first;
    for(unsigned i = 1; i < params.num_independent_populations; ++i) {
        if(betterThan(current[i]->fitness[0].first, best))
            best = current[i]->fitness[0].first;
    }
    return best;
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getBestChromosome() const {
    unsigned best_k = 0;
    for(unsigned i = 1; i < params.num_independent_populations; ++i)
        if(betterThan(current[i]->getBestFitness(),
                      current[best_k]->getBestFitness()))
            best_k = i;

    // Since the chromosomes are ordered by fitness, the first is the best.
    return current[best_k]->getChromosome(0);
}

//----------------------------------------------------------------------------//

template <class Decoder>
fitness_t BRKGA_MP_IPR<Decoder>::getFitness(unsigned population_index,
                                            unsigned position) const {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    return current[population_index]->fitness[position].first;
}

//----------------------------------------------------------------------------//

template <class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getChromosome(
        unsigned population_index, unsigned position) const {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    return current[population_index]->getChromosome(position);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::injectChromosome(const Chromosome& chromosome,
        unsigned population_index, unsigned position) {
    if(population_index >= current.size()) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The population index is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(position >= params.population_size) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The chromosome position is larger than number of populations";
        throw std::range_error(ss.str());
    }

    if(chromosome.size() != CHROMOSOME_SIZE) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Wrong chromosome size. It should be " << CHROMOSOME_SIZE
           << " but get " << chromosome.size();
        throw std::range_error(ss.str());
    }

    auto& pop = current[population_index];
    auto& local_chr = pop->population[pop->fitness[position].second];
    local_chr = chromosome;

    pop->setFitness(position, decoder.decode(local_chr, true));
    pop->sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setBiasCustomFunction(
            const std::function<double(const unsigned)>& func) {

    std::vector<double> bias_values(params.total_parents);
    std::iota(bias_values.begin(), bias_values.end(), 1);
    std::transform(bias_values.begin(), bias_values.end(),
                   bias_values.begin(), func);

    // If it is not non-increasing, throw an error.
    if(!std::is_sorted(bias_values.rbegin(), bias_values.rend())) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The bias function must be positive non-decreasing";
        throw std::range_error(ss.str());
    }

    if(bias_function)
        params.bias_type = BiasFunctionType::CUSTOM;

    bias_function = func;
    total_bias_weight = std::accumulate(bias_values.begin(),
                                        bias_values.end(), 0.0);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::reset() {
    if(!initialized) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The algorithm hasn't been initialized. "
              "Don't forget to call initialize() method";
        throw std::range_error(ss.str());
    }
    initialize(true);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::evolve(unsigned generations) {
    if(!initialized) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The algorithm hasn't been initialized. "
              "Don't forget to call initialize() method";
        throw std::range_error(ss.str());
    }

    if(generations == 0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Cannot evolve for 0 generations.";
        throw std::range_error(ss.str());
    }

    for(unsigned i = 0; i < generations; ++i) {
        for(unsigned j = 0; j < params.num_independent_populations; ++j) {
            // First evolve the population (current, next).
            evolution(*current[j], *previous[j]);
            std::swap(current[j], previous[j]);
        }
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::exchangeElite(unsigned num_immigrants) {
    if(params.num_independent_populations == 1)
        return;

    unsigned immigrants_threshold =
        ceil(params.population_size / (params.num_independent_populations - 1));

    if(num_immigrants < 1 || num_immigrants >= immigrants_threshold) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Number of immigrants (" << num_immigrants << ") less than one, "
              "or larger than or equal to population size / "
              "num_independent_populations (" << immigrants_threshold << ")";
        throw std::range_error(ss.str());
    }

    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(unsigned i = 0; i < params.num_independent_populations; ++i) {
        // Population i will receive some elite members from each Population j.
        // Last chromosome of i (will be overwritten below).
        unsigned dest = params.population_size - 1;
        for(unsigned j = 0; j < params.num_independent_populations; ++j) {
            if(j == i)
                continue;

            // Copy the num_immigrants best from Population j into Population i.
            for(unsigned m = 0; m < num_immigrants; ++m) {
                // Copy the m-th best of Population j into the 'dest'-th
                // position of Population i
                const auto best_of_j = current[j]->getChromosome(m);
                std::copy(best_of_j.begin(), best_of_j.end(),
                          current[i]->getChromosome(dest).begin());
                current[i]->fitness[dest].first = current[j]->fitness[m].first;
                --dest;
            }
        }
    }

    // Re-sort each population since they were modified.
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(unsigned i = 0; i < params.num_independent_populations; ++i)
        current[i]->sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::setInitialPopulation(
                                const std::vector<Chromosome>& chromosomes) {
    // First, reserve some memory.
    for(auto& pop : current) {
        pop.reset(new Population(CHROMOSOME_SIZE, 1));
        pop->population.pop_back();
        pop->population.reserve(params.population_size);
    }

    auto it_init_chr = chromosomes.begin();
    auto it_pop = current.begin();

    unsigned counter = 0;
    while(it_init_chr != chromosomes.end()) {
        if(it_init_chr->size() != CHROMOSOME_SIZE) {
            std::stringstream ss;
            ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
               << "Error on setting initial population: chromosome " << counter
               << " does not have the required dimension"
               << " (actual size: " << it_init_chr->size()
               << ", required size: " << CHROMOSOME_SIZE << ")";
            throw std::runtime_error(ss.str());
        }

        (*it_pop)->population.push_back(*it_init_chr);
        ++counter;
        ++it_init_chr;

        if(counter == params.population_size) {
            ++it_pop;
            counter = 0;
        }

        if(it_pop == current.end())
            it_init_chr = chromosomes.end();
    }

    initial_population = true;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::initialize(bool reset) {
    auto& rng = rng_per_thread[0];

    // Check and complete the populations.
    for(auto& pop : current) {
        if(!pop) {
            pop.reset(new Population(CHROMOSOME_SIZE, params.population_size));
        }
        else {
            pop->fitness.resize(params.population_size);
        }

        if(reset)
            pop->population.clear();

        if(pop->population.size() < params.population_size) {
            Chromosome chromosome(CHROMOSOME_SIZE);
            unsigned last_chromosome = pop->population.size();

            pop->population.resize(params.population_size);
            for(; last_chromosome < params.population_size; ++last_chromosome) {
                for(unsigned k = 0; k < CHROMOSOME_SIZE; ++k)
                    chromosome[k] = rand01(rng);
                pop->population[last_chromosome] = chromosome;
            }
        }
    }

    // Initialize and decode each chromosome of the current population,
    // then copy to previous.
    for(unsigned i = 0; i < params.num_independent_populations; ++i) {
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) schedule(static,1)
        #endif
        for(unsigned j = 0; j < params.population_size; ++j)
            current[i]->setFitness(j, decoder.decode((*current[i])(j), true));

        // Sort and copy to previous.
        current[i]->sortFitness(OPT_SENSE);
        previous[i].reset(new Population(*current[i]));
    }

    initialized = true;
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::shake(unsigned intensity,
                                  ShakingType shaking_type,
                                  unsigned population_index) {
    if(!initialized) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "The algorithm hasn't been initialized. "
              "Don't forget to call initialize() method";
        throw std::range_error(ss.str());
    }

    auto& rng = rng_per_thread[0];

    unsigned pop_start = population_index;
    unsigned pop_end = population_index;
    if(population_index >= params.num_independent_populations) {
        pop_start = 0;
        pop_end = params.num_independent_populations - 1;
    }

    for(; pop_start <= pop_end; ++pop_start) {
        auto& pop = current[pop_start]->population;

        // Shake the elite set.
        for(unsigned e = 0; e < elite_size; ++e) {
            for(unsigned k = 0; k < intensity; ++k) {
                auto i = randInt(CHROMOSOME_SIZE - 2, rng);
                if(shaking_type == ShakingType::CHANGE) {
                    // Invert value.
                    pop[e][i] = 1.0 - pop[e][i];
                }
                else {
                    // Swap with neighbor.
                    std::swap(pop[e][i], pop[e][i + 1]);
                }

                i = randInt(CHROMOSOME_SIZE - 1, rng);
                if(shaking_type == ShakingType::CHANGE) {
                    // Change to random value.
                    pop[e][i] = rand01(rng);
                }
                else {
                    // Swap two random positions.
                    auto j = randInt(CHROMOSOME_SIZE - 1, rng);
                    std::swap(pop[e][i], pop[e][j]);
                }
            }
        }

        // Reset the remaining population.
        for(unsigned ne = elite_size; ne < params.population_size; ++ne) {
            for(unsigned k = 0; k < CHROMOSOME_SIZE; ++k)
                pop[ne][k] = rand01(rng);
        }

        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) schedule(static,1)
        #endif
        for(unsigned j = 0; j < params.population_size; ++j)
            current[pop_start]->
                setFitness(j, decoder.decode((*current[pop_start])(j), true));

        // Now we must sort by fitness, since things might have changed.
        current[pop_start]->sortFitness(OPT_SENSE);
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::evolution(Population& curr,
                                      Population& next) {

    // To ensure the reproducibility, we need to be sure that the same random
    // values are generated in each mating, despite the number of threads
    // available. For that, we first generate a seed for each mating step. Then
    // use them in a RNG within the a thread.
    #ifdef MATING_SEED_ONLY
    { // hidden namespace
        auto& rng = rng_per_thread[0];
        for(auto& s: mating_seeds)
            s = rng();
    }
    #endif

    // First, we copy the elite chromosomes to the next generation.
    for(unsigned chr = 0; chr < elite_size; ++chr) {
        next.population[chr] = curr.population[curr.fitness[chr].second];
        next.fitness[chr] = std::make_pair(curr.fitness[chr].first, chr);
    }

    // Second, we mate 'pop_size - elite_size - num_mutants' pairs.
    // This parallel region allows processing the costly std::shuffle
    // for each mating in parallel.
    #ifndef MATING_SEQUENTIAL
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS) schedule(static, 1)
    #endif
    #endif
    for(unsigned chr = elite_size;
        chr < params.population_size - num_mutants; ++chr) {

        #ifdef _OPENMP
            auto& shuffled_individuals =
                shuffled_individuals_per_thread[omp_get_thread_num()];
            auto& parents_ordered =
                parents_ordered_per_thread[omp_get_thread_num()];
            auto& offspring = offspring_per_thread[omp_get_thread_num()];
            auto& rng = rng_per_thread[omp_get_thread_num()];
        #else
            auto& shuffled_individuals = shuffled_individuals_per_thread[0];
            auto& parents_ordered = parents_ordered_per_thread[0];
            auto& offspring = offspring_per_thread[0];
            auto& rng = rng_per_thread[0];
        #endif

        #ifdef MATING_SEED_ONLY
        // Reseed the RNG to guarantee reproducibility.
        rng.seed(mating_seeds[chr - elite_size]);
        #endif

        // First, we shuffled the elite set and non-elite set indices,
        // then we take the elite and non-elite parents. Note that we cannot
        // shuffled both sets together, otherwise we would mix elite
        // and non-elite individuals.

        // Rebuild the indices.
        std::iota(shuffled_individuals.begin(), shuffled_individuals.end(), 0);

        // Shuffles elite.
        std::shuffle(shuffled_individuals.begin(),
                     shuffled_individuals.begin() + elite_size, rng);

        // Shuffles non-elite.
        std::shuffle(shuffled_individuals.begin() + elite_size,
                     shuffled_individuals.end(), rng);

        parents_ordered.clear();

        // Take the elite parents.
        for(unsigned j = 0; j < params.num_elite_parents; ++j) {
            parents_ordered.emplace_back(curr.fitness[shuffled_individuals[j]]);
        }

        // Take the non-elite parents.
        for(unsigned j = 0;
            j < params.total_parents - params.num_elite_parents; ++j) {
            parents_ordered.emplace_back(
                    curr.fitness[shuffled_individuals[j + elite_size]]);
        }

        if(OPT_SENSE == Sense::MAXIMIZE)
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::greater<std::pair<fitness_t, unsigned>>());
        else
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::less<std::pair<fitness_t, unsigned>>());

        // Performs the mate.
        for(unsigned allele = 0; allele < CHROMOSOME_SIZE; ++allele) {
            // Roullete method.
            unsigned parent = 0;
            double cumulative_probability = 0.0;
            const double toss = rand01(rng);
            do {
                // Start parent from 1 because the bias function.
                cumulative_probability += bias_function(++parent) /
                                          total_bias_weight;
            } while(cumulative_probability < toss);

            // Decrement parent to the right index, and take the allele value.
            offspring[allele] = curr(parents_ordered[--parent].second, allele);
        }

        // This strategy of setting the offpring in a local variable, and then
        // copying to the population seems to reduce the overall cache misses
        // counting.
        next.getChromosome(chr) = offspring;
    }

    // To finish, we fill up the remaining spots with mutants.
    #ifndef MATING_SEQUENTIAL
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS) schedule(static, 1)
    #endif
    #endif
    for(unsigned chr = params.population_size - num_mutants;
        chr < params.population_size; ++chr) {
        #ifdef _OPENMP
            auto& rng = rng_per_thread[omp_get_thread_num()];
        #else
            auto& rng = rng_per_thread[0];
        #endif
        for(auto& allele : next.population[chr])
            allele = rand01(rng);
    }

    // Time to compute fitness, in parallel.
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS) schedule(static, 1)
    #endif
    for(unsigned i = elite_size; i < params.population_size; ++i)
        next.setFitness(i, decoder.decode(next.population[i], true));

    // Now we must sort by fitness, since things might have changed.
    next.sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template <class Decoder>
PathRelinking::PathRelinkingResult BRKGA_MP_IPR<Decoder>::pathRelink(
                    PathRelinking::Type pr_type,
                    PathRelinking::Selection pr_selection,
                    std::shared_ptr<DistanceFunctionBase> dist,
                    unsigned number_pairs,
                    double minimum_distance,
                    std::size_t block_size,
                    long max_time,
                    double percentage) {

    using PR = PathRelinking::PathRelinkingResult;

    auto& rng = rng_per_thread[0];

    if(percentage < 1e-6 || percentage > 1.0) {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << ", line " << __LINE__ << ": "
           << "Percentage/size of path relinking invalid, current: "
           << percentage;
        throw std::range_error(ss.str());
    }

    if(max_time <= 0)
        max_time = std::numeric_limits<long>::max();

    Chromosome initial_solution(CHROMOSOME_SIZE);
    Chromosome guiding_solution(CHROMOSOME_SIZE);

    // Perform path relinking between elite chromosomes from different
    // populations. This is done in a circular fashion.
    std::deque<std::pair<std::size_t, std::size_t>> index_pairs;

    // Keep track of the time.
    pr_start_time = std::chrono::system_clock::now();

    auto final_status = PR::TOO_HOMOGENEOUS;

    for(unsigned pop_count = 0; pop_count < params.num_independent_populations;
        ++pop_count) {
        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time).count();
        if(elapsed_seconds > max_time)
            break;

        unsigned pop_base = pop_count;
        unsigned pop_guide = pop_count + 1;
        bool found_pair = false;

        // If we have just one population, we take the both solution from it.
        if(params.num_independent_populations == 1) {
            pop_base = pop_guide = 0;
            pop_count = params.num_independent_populations;
        }
        // If we have two populations, perform just one path relinking.
        else if(params.num_independent_populations == 2) {
            pop_count = params.num_independent_populations;
        }

        // Do the circular thing.
        if(pop_guide == params.num_independent_populations)
            pop_guide = 0;

        index_pairs.clear();
        for(std::size_t i = 0; i < elite_size; ++i)
            for(std::size_t j = 0; j < elite_size; ++j)
                index_pairs.emplace_back(std::make_pair(i, j));

        unsigned tested_pairs_count = 0;
        if(number_pairs == 0)
            number_pairs = index_pairs.size();

        while(!index_pairs.empty() && tested_pairs_count < number_pairs &&
              elapsed_seconds < max_time) {
            const auto index =
                    (pr_selection == PathRelinking::Selection::BESTSOLUTION?
                     0 : randInt(index_pairs.size() - 1, rng));

            const auto pos1 = index_pairs[index].first;
            const auto pos2 = index_pairs[index].second;

            const auto& chr1 = current[pop_base]->
                    population[current[pop_base]->fitness[pos1].second];

            const auto& chr2 = current[pop_guide]->
                    population[current[pop_base]->fitness[pos2].second];

            if(dist->distance(chr1, chr2) >= minimum_distance - 1e-6) {
                copy(begin(chr1), end(chr1), begin(initial_solution));
                copy(begin(chr2), end(chr2), begin(guiding_solution));
                found_pair = true;
                break;
            }

            index_pairs.erase(begin(index_pairs) + index);
            ++tested_pairs_count;
            elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
                (std::chrono::system_clock::now() - pr_start_time).count();
        }

        // The elite sets are too homogeneous, we cannot do
        // a good path relinking. Let's try other populations.
        if(!found_pair)
            continue;

        // Create a empty solution.
        std::pair<fitness_t, Chromosome> best_found;
        best_found.second.resize(current[0]->getChromosomeSize(), 0.0);

        const bool sense = OPT_SENSE == Sense::MAXIMIZE;
        if(sense)
            best_found.first = FITNESS_T_MIN;
        else
            best_found.first = FITNESS_T_MAX;

        const auto fence = best_found.first;

        // Perform the path relinking.
        if(pr_type == PathRelinking::Type::DIRECT) {
            directPathRelink(initial_solution, guiding_solution, dist,
                             best_found, block_size, max_time, percentage);
        }
        else {
            permutatioBasedPathRelink(initial_solution, guiding_solution, dist,
                                      best_found, block_size, max_time,
                                      percentage);
        }

        final_status |= PR::NO_IMPROVEMENT;

        // **NOTE:** is fitness_t contains float types, so the comparison
        // `best_found.first == fence` may be unfase. Therefore, we use
        // helper functions that define the correct behavior at compilation
        // time.
        if(close_enough(best_found.first, fence))
            continue;

        // Re-decode and apply local search if the decoder are able to do it.
        best_found.first = decoder.decode(best_found.second, true);

        // Now, check if the best solution found is really good.
        // If it is the best, overwrite the worse solution in the population.
        bool include_in_population =
           (sense && best_found.first > current[pop_base]->fitness[0].first) ||
           (!sense && best_found.first < current[pop_base]->fitness[0].first);

        const auto best_overall = this->getBestFitness();

        if((sense && best_found.first > best_overall) ||
           (!sense && best_found.first < best_overall))
            final_status |= PR::BEST_IMPROVEMENT;

        // If not the best, but is better than the worst elite member, check
        // if the distance between this solution and all elite members
        // is at least minimum_distance.
        if(!include_in_population &&
           ((sense && best_found.first >
                        current[pop_base]->fitness[elite_size - 1].first) ||
            (!sense && best_found.first <
                        current[pop_base]->fitness[elite_size - 1].first))) {

            include_in_population = true;
            for(unsigned i = 0; i < elite_size; ++i) {
                if(dist->distance(best_found.second, current[pop_base]->
                            population[current[pop_base]->fitness[i].second])
                   < minimum_distance - 1e-6) {
                    include_in_population = false;
                    final_status |= PR::NO_IMPROVEMENT;
                    break;
                }
            }
        }

        if(include_in_population) {
            std::copy(begin(best_found.second), end(best_found.second),
                      begin(current[pop_base]->
                                population[current[pop_base]->
                                    fitness.back().second]));

            current[pop_base]->fitness.back().first = best_found.first;
            // Reorder the chromosomes.
            current[pop_base]->sortFitness(OPT_SENSE);
            final_status |= PR::ELITE_IMPROVEMENT;
        }
    }

    return final_status;
}

//----------------------------------------------------------------------------//

template <class Decoder>
PathRelinking::PathRelinkingResult BRKGA_MP_IPR<Decoder>::pathRelink(
            std::shared_ptr<DistanceFunctionBase> dist,
            long max_time) {

    size_t block_size = ceil(params.alpha_block_size *
                             sqrt(params.population_size));
    if(block_size > CHROMOSOME_SIZE)
        block_size = CHROMOSOME_SIZE / 2;

    return pathRelink(params.pr_type, params.pr_selection, dist,
                      params.pr_number_pairs, params.pr_minimum_distance,
                      block_size, max_time,
                      params.pr_percentage);
}

//----------------------------------------------------------------------------//

// This is a multi-thread version. For small chromosomes, it may be slower than
// single thread version.
template <class Decoder>
void BRKGA_MP_IPR<Decoder>::directPathRelink(
            const Chromosome& chr1, const Chromosome& chr2,
            std::shared_ptr<DistanceFunctionBase> dist,
            std::pair<fitness_t, Chromosome>& best_found,
            std::size_t block_size, long max_time, double percentage) {

    const std::size_t NUM_BLOCKS =
            std::size_t(ceil((double)chr1.size() / block_size));
    const std::size_t PATH_SIZE = std::size_t(percentage * NUM_BLOCKS);

    // Create the set of indices to test.
    std::set<std::size_t> remaining_blocks;
    for(std::size_t i = 0; i < NUM_BLOCKS; ++i)
        remaining_blocks.insert(i);
    Chromosome old_keys(chr1.size());

    struct Triple {
    public:
        Chromosome chr;
        fitness_t fitness;
        std::size_t block_index;
        Triple(): chr(), fitness(FITNESS_T_MAX), block_index(0) {}
    };

    // Allocate memory for the candidates.
    std::vector<Triple> candidates_left(NUM_BLOCKS);
    std::vector<Triple> candidates_right(NUM_BLOCKS);

    for(auto &cand : candidates_left)
        cand.chr.resize(chr1.size());

    for(auto &cand : candidates_right)
        cand.chr.resize(chr1.size());

    const Chromosome* base = &chr1;
    const Chromosome* guide = &chr2;
    std::vector<Triple>* candidates_base = &candidates_left;
    std::vector<Triple>* candidates_guide = &candidates_right;

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(std::size_t i = 0; i < candidates_left.size(); ++i)
        std::copy(begin(*base), end(*base), begin(candidates_left[i].chr));

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(std::size_t i = 0; i < candidates_right.size(); ++i)
        std::copy(begin(*guide), end(*guide), begin(candidates_right[i].chr));

    const bool sense = OPT_SENSE == Sense::MAXIMIZE;

    std::size_t iterations = 0;
    while(!remaining_blocks.empty()) {
        // Set the block of keys from the guide solution for each candidate.
        auto it_block_idx = remaining_blocks.begin();

        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            const auto block_base = (*it_block_idx) * block_size;

            const auto it_key_block1 =
                    (*candidates_base)[i].chr.begin() + block_base;
            const auto it_key_block2 = guide->begin() + block_base;

            const auto bs = (block_base + block_size > guide->size())?
                            guide->size() - block_base : block_size;

            // If these keys do not affect the solution, skip them.
            if(!dist->affectSolution(it_key_block1, it_key_block2, bs)) {
                it_block_idx = remaining_blocks.erase(it_block_idx);
                --i;
                continue;
            }

            // Save the former keys before...
            std::copy_n((*candidates_base)[i].chr.begin() + block_base, bs,
                        old_keys.begin() + block_base);

            // ... copy the keys from the guide solution.
            std::copy_n(guide->begin() + block_base, bs,
                        (*candidates_base)[i].chr.begin() + block_base);

            (*candidates_base)[i].block_index = *it_block_idx;
            ++it_block_idx;
        }

        if (remaining_blocks.empty()) {
            break;
        }

        // Decode the candidates.
        volatile bool times_up = false;
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) shared(times_up) \
                schedule(static, 1)
        #endif
        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            if(sense)
                (*candidates_base)[i].fitness = FITNESS_T_MIN;
            else
                (*candidates_base)[i].fitness = FITNESS_T_MAX;

            if(times_up) continue;

            (*candidates_base)[i].fitness =
                    decoder.decode((*candidates_base)[i].chr, false);

            const auto elapsed_seconds =
                 std::chrono::duration_cast<std::chrono::seconds>
                 (std::chrono::system_clock::now() - pr_start_time).count();
            if(elapsed_seconds > max_time)
                times_up = true;
        }

        // Locate the best candidate.
        std::size_t best_index = 0;
        std::size_t best_block_index = 0;

        fitness_t best_value;
        if(sense)
            best_value = FITNESS_T_MIN;
        else
            best_value = FITNESS_T_MAX;

        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            if((best_value < (*candidates_base)[i].fitness && sense) ||
               (best_value > (*candidates_base)[i].fitness && !sense)) {
                best_block_index = (*candidates_base)[i].block_index;
                best_value = (*candidates_base)[i].fitness;
                best_index = i;
            }
        }

        // Hold it, if it is the best found until now.
        if((sense && best_found.first < (*candidates_base)[best_index].fitness)
           ||
           (!sense && best_found.first > (*candidates_base)[best_index].fitness)
           ) {
            best_found.first = (*candidates_base)[best_index].fitness;
            std::copy(begin((*candidates_base)[best_index].chr),
                      end((*candidates_base)[best_index].chr),
                      begin(best_found.second));
        }

        // Restore original keys and copy the block of keys for all future
        // candidates. The last candidate will not be used.
        it_block_idx = remaining_blocks.begin();
        for(std::size_t i = 0; i < remaining_blocks.size() - 1;
            ++i, ++it_block_idx) {

            auto block_base = (*it_block_idx) * block_size;
            auto bs = (block_base + block_size > guide->size())?
                      guide->size() - block_base : block_size;

            std::copy_n(old_keys.begin() + block_base, bs,
                        (*candidates_base)[i].chr.begin() + block_base);

            // Recompute the offset for the best block.
            block_base = best_block_index * block_size;
            bs = (block_base + block_size > guide->size())?
                 guide->size() - block_base : block_size;

            std::copy_n((*candidates_base)[best_index].chr.begin() + block_base,
                        bs, (*candidates_base)[i].chr.begin() + block_base);
        }

        std::swap(base, guide);
        std::swap(candidates_base, candidates_guide);
        remaining_blocks.erase(best_block_index);

        const auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time).count();

        if((elapsed_seconds > max_time) || (iterations++ > PATH_SIZE))
            break;
    } // end while
}

//----------------------------------------------------------------------------//

template <class Decoder>
void BRKGA_MP_IPR<Decoder>::permutatioBasedPathRelink(
                Chromosome& chr1, Chromosome& chr2,
                std::shared_ptr<DistanceFunctionBase> /*non-used*/,
                std::pair<fitness_t, Chromosome>& best_found,
                std::size_t /*non-used block_size*/,
                long max_time, double percentage) {

    const std::size_t PATH_SIZE = std::size_t(percentage * CHROMOSOME_SIZE);

    std::set<std::size_t> remaining_indices;
    for(std::size_t i = 0; i < chr1.size(); ++i)
        remaining_indices.insert(i);

    struct DecodeStruct {
    public:
        Chromosome chr;
        fitness_t fitness;
        std::size_t key_index;
        std::size_t pos1;
        std::size_t pos2;
        DecodeStruct(): chr(), fitness(FITNESS_T_MAX),
                        key_index(0), pos1(0), pos2(0) {}
    };

    // Allocate memory for the candidates.
    std::vector<DecodeStruct> candidates_left(chr1.size());
    std::vector<DecodeStruct> candidates_right(chr1.size());

    for(auto& cand : candidates_left)
        cand.chr.resize(chr1.size());

    for(auto& cand : candidates_right)
        cand.chr.resize(chr1.size());

    Chromosome* base = &chr1;
    Chromosome* guide = &chr2;
    std::vector<DecodeStruct>* candidates_base = &candidates_left;
    std::vector<DecodeStruct>* candidates_guide = &candidates_right;

    std::vector<std::size_t> chr1_indices(chr1.size());
    std::vector<std::size_t> chr2_indices(chr1.size());
    std::vector<std::size_t>* base_indices = &chr1_indices;
    std::vector<std::size_t>* guide_indices = &chr2_indices;

    // Create and order the indices.
    std::vector<std::pair<double, std::size_t>> sorted(chr1.size());

    for(unsigned j = 0; j < 2; ++j) {
        for(std::size_t i = 0; i < base->size(); ++i)
            sorted[i] = std::pair<double, std::size_t>((*base)[i], i);

        std::sort(begin(sorted), end(sorted));
        for(std::size_t i = 0; i < base->size(); ++i)
            (*base_indices)[i] = sorted[i].second;

        swap(base, guide);
        swap(base_indices, guide_indices);
    }

    base = &chr1;
    guide = &chr2;
    base_indices = &chr1_indices;
    guide_indices = &chr2_indices;

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(std::size_t i = 0; i < candidates_left.size(); ++i) {
        std::copy(begin(*base), end(*base), begin(candidates_left[i].chr));
    }

    #ifdef _OPENMP
    #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(std::size_t i = 0; i < candidates_right.size(); ++i) {
        std::copy(begin(*guide), end(*guide), begin(candidates_right[i].chr));
    }

    const bool sense = OPT_SENSE == Sense::MAXIMIZE;

    std::size_t iterations = 0;
    while(!remaining_indices.empty()) {
        std::size_t position_in_base;
        std::size_t position_in_guide;

        auto it_idx = remaining_indices.begin();
        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            position_in_base = (*base_indices)[*it_idx];
            position_in_guide = (*guide_indices)[*it_idx];

            if(position_in_base == position_in_guide) {
                it_idx = remaining_indices.erase(it_idx);
                --i;
                continue;
            }

            (*candidates_base)[i].key_index = *it_idx;
            (*candidates_base)[i].pos1 = position_in_base;
            (*candidates_base)[i].pos2 = position_in_guide;

            if(sense)
                (*candidates_base)[i].fitness = FITNESS_T_MIN;
            else
                (*candidates_base)[i].fitness = FITNESS_T_MAX;
            ++it_idx;
        }

        if(remaining_indices.size() == 0)
            break;

        // Decode the candidates.
        volatile bool times_up = false;
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) shared(times_up) \
                schedule(static, 1)
        #endif
        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            if(times_up) continue;

            std::swap((*candidates_base)[i].chr[(*candidates_base)[i].pos1],
                      (*candidates_base)[i].chr[(*candidates_base)[i].pos2]);

            (*candidates_base)[i].fitness =
                    decoder.decode((*candidates_base)[i].chr, false);

            std::swap((*candidates_base)[i].chr[(*candidates_base)[i].pos1],
                      (*candidates_base)[i].chr[(*candidates_base)[i].pos2]);

            const auto elapsed_seconds =
                    std::chrono::duration_cast<std::chrono::seconds>
                    (std::chrono::system_clock::now() - pr_start_time).count();
            if(elapsed_seconds > max_time)
                times_up = true;
        }

        // Locate the best candidate
        std::size_t best_key_index = 0;
        std::size_t best_index;

        fitness_t best_value;
        if(sense)
            best_value = FITNESS_T_MIN;
        else
            best_value = FITNESS_T_MAX;

        for(std::size_t i = 0; i < remaining_indices.size(); ++i) {
            if((best_value < (*candidates_base)[i].fitness && sense) ||
               (best_value > (*candidates_base)[i].fitness && !sense)) {
                best_index = i;
                best_key_index = (*candidates_base)[i].key_index;
                best_value = (*candidates_base)[i].fitness;
            }
        }

        position_in_base = (*base_indices)[best_key_index];
        position_in_guide = (*guide_indices)[best_key_index];

        // Commit the best exchange in all candidates.
        // The last will not be used.
        for(std::size_t i = 0; i < remaining_indices.size() - 1; ++i) {
            std::swap((*candidates_base)[i].chr[position_in_base],
                      (*candidates_base)[i].chr[position_in_guide]);
        }

        std::swap((*base_indices)[position_in_base],
                  (*base_indices)[position_in_guide]);

        // Hold, if it is the best found until now
        if((sense && best_found.first < best_value) ||
           (!sense && best_found.first > best_value)) {
            const auto& best_chr = (*candidates_base)[best_index].chr;
            best_found.first = best_value;
            copy(begin(best_chr), end(best_chr), begin(best_found.second));
        }

        std::swap(base_indices, guide_indices);
        std::swap(candidates_base, candidates_guide);
        remaining_indices.erase(best_key_index);

        // Is time to stop?
        const auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now() - pr_start_time).count();

        if((elapsed_seconds > max_time) || (iterations++ > PATH_SIZE))
            break;
    }
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline double BRKGA_MP_IPR<Decoder>::rand01(std::mt19937& rng) {
    // **NOTE:** instead to use std::generate_canonical<> (which can be
    // a little bit slow), we may use
    //    rng() * (1.0 / std::numeric_limits<std::mt19937::result_type>::max());
    // However, this approach has some precision problems on some platforms
    // (notably Linux).
    return std::generate_canonical<double, std::numeric_limits<double>::digits>
          (rng);
}

//----------------------------------------------------------------------------//

template <class Decoder>
inline uint_fast32_t BRKGA_MP_IPR<Decoder>::randInt(const uint_fast32_t n,
                                                    std::mt19937& rng) {
    // This code was adapted from Magnus Jonsson (magnus@smartelectronix.com)
    // Find which bits are used in n. Note that this is specific
    // for uint_fast32_t types.
    uint_fast32_t used = n;
    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;

    // Draw numbers until one is found in [0, n].
    uint_fast32_t i;
    do {
        i = rng() & used;  // Toss unused bits to shorten search.
    } while(i > n);
    return i;
}

} // end namespace BRKGA_MP_IPR

//----------------------------------------------------------------------------//
// Template specializations for enum I/O
//----------------------------------------------------------------------------//

/**
 * \defgroup template_specs Template specializations for enum I/O.
 *
 * Using slightly modified template class provided by Bradley Plohr
 * (https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header)
 * we specialize that template to enums in the BRKGA namespace.
 *
 * The EnumIO class helps to read and write enums from streams directly,
 * saving time in coding custom solutions. Please, see third_part/enum_io.hpp
 * for complete reference and examples.
 *
 * \note
 *      The specializations must be done in the global namespace.
 *
 * \warning The specialization must be inline-d to avoid multiple definitions
 * issues across different modules. However, this can cause "inline" overflow,
 * and compromise your code. If you include this header only once along with
 * your code, it is safe to remove the `inline`s from the specializations. But,
 * if this is not the case, you should move these specializations to a module
 * you know is included only once, for instance, the `main()` module.
 */
///@{

/// Template specialization to BRKGA::Sense.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::Sense>::enum_names() {
    static std::vector<std::string> enum_names_({
        "MINIMIZE",
        "MAXIMIZE"
    });
    return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Type.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Type>::enum_names() {
    static std::vector<std::string> enum_names_({
        "DIRECT",
        "PERMUTATION"
    });
    return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Selection.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Selection>::enum_names() {
    static std::vector<std::string> enum_names_({
        "BESTSOLUTION",
        "RANDOMELITE"
    });
    return enum_names_;
}

/// Template specialization to BRKGA::BiasFunctionType.
template <>
INLINE const std::vector<std::string>&
EnumIO<BRKGA::BiasFunctionType>::enum_names() {
    static std::vector<std::string> enum_names_({
        "CONSTANT",
        "CUBIC",
        "EXPONENTIAL",
        "LINEAR",
        "LOGINVERSE",
        "QUADRATIC",
        "CUSTOM"
    });
    return enum_names_;
}
///@}

#endif // BRKGA_MP_IPR_HPP_
