/*******************************************************************************
 * brkga_mp_ipr.hpp: Biased Random-Key Genetic Algorithm Multi-Parent
 *                   with Implict Path Relinking.
 *
 * Author: Carlos Eduardo de Andrade
 *         <cea@research.att.com / ce.andrade@gmail.com>
 *
 * (c) Copyright 2015-2018.
 *     Institute of Computing, University of Campinas.
 *     AT&T Labs Research.
 *
 * (c) Copyright 2010, 2011 Rodrigo F. Toso <rtoso@cs.rutgers.edu>
 *     and Mauricio G.C. Resende <mgcr@research.att.com>.
 *
 *  Created on : Jan 06, 2015 by andrade.
 *  Last update: Jun 12, 2018 by andrade.
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
#include <unordered_map>
#include <utility>
#include <vector>

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
}

/// Specifies a bias function when choosing parents to mating
/// (`r` is a given parameter). This function substitutes the `rhoe`
/// parameter from the original BRKGA.
enum class BiasFunction {
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
            if((vector1[i] < threshold ? 0 : 1) !=
               (vector2[i] < threshold ? 0 : 1))
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
            if((*v1_begin < threshold ? 0 : 1) !=
               (*v2_begin < threshold ? 0 : 1))
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
     * TODO (ceandrade): implement this properly.
     *
     * \param v1_begin begin of the first blocks of keys
     * \param v2_begin begin of the first blocks of keys
     * \param block_size number of keys to be considered.
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
 * making the implementation easier. All methods and attributes are public and
 * can be manipulated directly from BRKGA algorithms. Note that this class is
 * not meant to be used externally of this unit.
 */
class Population {
public:
    /** \name Data members */
    //@{
    /// Population as vectors of probabilities.
    std::vector<Chromosome> population;

    /// Fitness (double) of a each chromosome.
    std::vector<std::pair<double, unsigned>> fitness;
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
    double getBestFitness() const {
        return getFitness(0);
    }

    /// Returns the fitness of chromosome i.
    double getFitness(const unsigned i)  const {
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
        typedef std::pair<double, unsigned> local_type;
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
    void setFitness(const unsigned chromosome, const double value) {
        fitness[chromosome] = std::make_pair(value, chromosome);
    }
    //@}
};

//----------------------------------------------------------------------------//
// The Multi-Parent Biased Random-key Genetic Algorithm with Implicit
// Path Relinking
//----------------------------------------------------------------------------//

/**
 * \brief This class represents a Multi-Parent Biased Random-key Genetic
 * Algorithm with Implicit Path Relinking (BRKGA-MP-IPR).
 *
 * \author Carlos Eduardo de Andrade <cea@research.att.com/ce.andrade@gmail.com>
 * \date 2018
 *
 * This API was based on code by Rodrigo Franco Toso, Sep 15, 2011.
 * http://github.com/rfrancotoso/brkgaAPI
 *
 * Main capabilities
 * ========================
 *
 * Evolutionary process
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
 * object able to map a chromosome to a solution for the specific problem,
 * and return a value to be used as fitness to the decoded chromosome.
 * The decoder must have the method
 * \code{.cpp}
 *      double decode(Chromosome& chromosome, bool writeback);
 * \endcode
 *
 * where #Chromosome is a `vector<double>` representing a solution and
 * `writeback` is a boolean indicating that if the decode should rewrite the
 * chromosome in case it implements local searches and modifies the initial
 * solution decoded from the chromosome. Since this API has the capability of
 * decoding several chromosomes in parallel, the user must guarantee that
 * `Decoder::decode(...)` is thread-safe. Therefore, we do recommend to have
 * the writable variables per thread. Please, see the example that follows this
 * code.
 *
 * Implicit Path Relinking
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
 * with `writeback = false`. The reason is that if the decoder rewrites the
 * chromosome, the path between solutions is lost and inadvertent results may
 * come up. Note that at the end of the path relinking, the method calls the
 * decoder with `writeback = true` in the best chromosome found to guarantee
 * that this chromosome is re-written to reflect the best solution found.
 *
 * Other capabilities
 * ========================
 *
 * Multi-start
 * ------------------------
 *
 * This API also can be used as a simple multi-start algorithm without
 * evolution. To do that, the user must supply in the constructor the argument
 * `evolutionary_mechanism_on = false`. This argument makes the elite set has
 * one individual and the number of mutants n - 1, where n is the size of the
 * population. This setup disables the evolutionary process completely.
 *
 * Initial Population
 * ------------------------
 *
 * This API allows the user provides a set of initial solutions to warm start
 * the algorithm. In general, initial solutions are created using other (fast)
 * heuristics and help the convergence of the BRKGA. To do that, the user must
 * encode the solutions on #Chromosome (`vector<double>`) and pass to the method
 * setInitialPopulation() as a `vector<#Chromosome>`.
 *
 * General Usage
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
 * About multi-threading
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
 */
template<class Decoder>
class BRKGA_MP_IPR {
public:
    /** \name Constructors and destructor */
    //@{
    /**
     * \brief Builds the algorithm with the given arguments.
     *
     * \param decoder_reference a reference to the decoder object.
     * \param sense the optimization sense (maximization or minimization).
     * \param seed the seed for the random number generator.
     * \param chromosome_size number of genes in each chromosome.
     * \param population_size number of individuals in each population.
     * \param elite_percentage percentage of elite individuals into each
     *        population.
     * \param mutants_percentage percentage of mutants introduced at each
     *        generation into the population.
     * \param evolutionary_mechanism_on if false, no evolution is performed
     *        but only chromosome decoding. Very useful to emulate a
     *        multi-start algorithm.
     * \param num_elite_parents number of parents from elite set used on the
     *        multi-parent crossover. By setting `num_elite_parents == 1` and
     *        `total_parents == 2`, standard BRKGA crossover is performed
     *        (one elite and one non-elite sets). To emulate standard BRKGA
     *        bias `rhoe`, a custom bias function must be used.
     * \param total_parents number of parents to be used on the multi-parent
     *        crossover. By setting `num_elite_parents == 1` and
     *        `total_parents == 2`, standard BRKGA crossover is performed
     *        (one elite and one non-elite sets). To emulate standard BRKGA
     *        bias `rhoe`, a custom bias function must be used.
     * \param bias bias function used to choose the parents during multi-parent
     *        crossover. This function substitutes the `rhoe` parameter from
     *        the original BRKGA.
     * \param num_independent_populations number of independent populations
     *        (island model).
     * \param max_threads number of threads to perform parallel decoding.\n
     *        **NOTE**: `Decoder::decode()` MUST be thread-safe.
     *
     * \throw std::range_error if some parameter or combination of parameters
     *        does not fit.
     */
    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        Sense sense,
        unsigned seed,
        unsigned chromosome_size,
        unsigned population_size,
        double elite_percentage,
        double mutants_percentage,
        bool evolutionary_mechanism_on = true,
        unsigned num_elite_parents = 1,
        unsigned total_parents = 2,
        BiasFunction bias = BiasFunction::LOGINVERSE,
        unsigned num_independent_populations = 1,
        unsigned max_threads = 1);

    /**
     * \brief Builds the algorithm with the parameters read from the
     *        configuration file.
     *
     * \param decoder_reference a reference to the decoder object.
     * \param sense the optimization sense (maximization or minimization).
     * \param seed the seed for the random number generator.
     * \param chromosome_size number of genes in each chromosome.
     * \param configuration_file text file with the BRKGA parameters.
     *        An example can be found at
     *        <a href="example.conf">example.conf</a>. Note that the content
     *        after "#" is considered comments and it is ignored.
     * \param evolutionary_mechanism_on if false, no evolution is performed
     *        but only chromosome decoding. Very useful to emulate a
     *        multi-start algorithm.
     * \param max_threads number of threads to perform parallel decoding.\n
     *        **NOTE**: `Decoder::decode()` MUST be thread-safe.
     *
     * \throw std::fstream::failure in case of errors in the file.
     *
     * \throw std::range_error if some parameter or combination of parameters
     *        does not fit.
     */
    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        Sense sense,
        unsigned seed,
        unsigned chromosome_size,
        const std::string& configuration_file,
        bool evolutionary_mechanism_on = true,
        unsigned max_threads = 1);

    /// Destructor
    ~BRKGA_MP_IPR() {}
    //@}

    /** \name Initialization methods */
    //@{
    /**
     * \brief Sets individuals to initial population.
     *
     * This method enables warm starts from solutions obtained from other
     * methods. All given solutions are assigned to one population only.
     * Therefore, the maximum number of solutions is the size of the
     * populations.
     * \param chromosomes a set of individuals encoded as Chromosomes.
     * \throw std::runtime_error if the number of given chromosomes is larger
     *        than the population size; if the sizes of the given chromosomes
     *        do not match with the required chromosome size.
     */
    void setInitialPopulation(const std::vector<Chromosome>& chromosomes);

    /**
     * \brief Sets a custom bias function used to build the probabilities.
     *
     * For example
     * \code{.cpp}
     *      setBiasCustomFunction(
     *          [](const double x) {
     *              return 1.0 / (x * x);
     *          }
     *      );
     * \endcode
     * sets an inverse quadratic function.
     *
     * \param func a unary non-increasing function such domain is
     *        [1, #total_parents] and returns values in the interval [0, 1].
     */
    void setBiasCustomFunction(const std::function<double(const double)>& func);

    /**
     * \brief Initializes the populations and others parameters of the
     *        algorithm.
     *
     * If a initial population is supplied, this method completes the remain
     * individuals, if they do not exist.
     *
     * **NOTE:** this method must be call before any evolutionary or population
     * handling method.
     *
     * \param true_init when set false, it ignores all solutions provided
     *        by `setInitialPopulation()`, and builds a completely random
     *        population. This parameter is set to false during reset phase.
     *        When true, `initialize()` considers all solutions given by
     *        `setInitialPopulation()`.
     */
    void initialize(bool true_init = true);
    //@}

    /** \name Evolution */
    //@{
    /**
     * \brief Evolves the current populations following the guidelines of
     *        Multi-parent BRKGAs.
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
     * \brief Performs path relinking between elite solutions that are,
     *        at least, a given minimum distance between themselves.
     *
     * In this method, the local/loaded parameters are ignored in favor to
     * the supplied ones.
     *
     * \param dist the functor/object to compute the distance between two
     *        chromosomes.
     * \param number_pairs number of chromosome pairs to be tested.
     *        If 0, all pairs are tested.
     * \param minimum_distance between two chromosomes.
     * \param pathrelinkingtype type of path relinking to be performed.
     * \param selection of which individuals use to path relinking.
     * \param block_size number of alleles to be exchanged at once in each
     *        iteration. If one, the traditional path relinking is performed.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     * \param percentage defines the size, in percentage, of the path to
     *        build. Default: 1.0 (100%).
     *
     * \throw std::range_error if the percentage or size of the path is
     *        not in (0, 1].
     */
    bool pathRelink(std::shared_ptr<DistanceFunctionBase> dist,
                    unsigned number_pairs,
                    double minimum_distance,
                    PathRelinking::Type pathrelinkingtype,
                    PathRelinking::Selection selection,
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
     * where \f$\alpha\f$ is #alpha_block_size and \f$p\f$ is #population_size.
     * If the size is larger than the chromosome size, the size is set to
     * half of the chromosome size.
     *
     * \param dist the functor/object to compute the distance between two
     *        chromosomes.
     * \param max_time aborts path relinking when reach `max_time`.
     *        If `max_time <= 0`, no limit is imposed.
     *
     * \throw std::range_error if the percentage or size of the path is
     *        not in (0, 1].
     */
    bool pathRelink(std::shared_ptr<DistanceFunctionBase> dist,
                    long max_time = 0);
    //@}

    /** \name Population manipulation methods */
    //@{
    /**
     * \brief Exchanges elite-solutions between the populations.
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
     * \throw std::runtime_error if the algorithm is not initialized.
     */
    void reset();

    /**
     * \brief Inject a chromosome and its fitness into a population in the
     *         given place position.
     *
     * If fitness is not provided (`fitness == Inf`), the decoding is performed
     * over chromosome. Once the chromosome is injected, the population is
     * re-sorted according to the chromosomes' fitness.
     *
     * \param chromosome the chromosome to be injected.
     * \param population_index the population index.
     * \param position the chromosome position.
     * \param fitness the pre-computed fitness if it is available.
     *
     * \throw std::range_error eitheir if `population_index` is larger
     *        than number of populations; or `position` is larger than the
     *        population size; or ` chromosome.size() != chromosome_size`
     */
    void injectChromosome(const Chromosome& chromosome,
                          unsigned population_index,
                          unsigned position,
                          double fitness =
                                      std::numeric_limits<double>::infinity());
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

    /// Returns the best fitness found so far among all populations.
    double getBestFitness() const;

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

    //@}

    /** \name Parameter getters */
    //@{
    /**
     * \brief Writes the parameters into a file. This file is similar to
     *        <a href="example.conf">example.conf</a> without comments.
     * \param filename the configuration file.
     * \throw std::fstream::failure in case of errors in the file.
     */
    void writeConfiguration(const std::string& filename);

    Sense getOptimizationSense() const { return OPT_SENSE; }

    unsigned getChromosomeSize() const { return CHROMOSOME_SIZE; }

    unsigned getPopulationSize() const { return population_size; }

    unsigned getEliteSize() const { return elite_size; }

    unsigned getNumMutants() const { return num_mutants; }

    unsigned getNumEliteParents() const { return num_elite_parents; }

    unsigned getTotalParents() const { return total_parents; }

    BiasFunction getBias() const { return bias; }

    bool evolutionaryIsMechanismOn() const { return evolutionary_mechanism_on; }

    double getPathRelinkingMinimumDistance() const { return pr_minimum_distance; }

    PathRelinking::Type getPathRelinkingType() const { return pr_type; }

    PathRelinking::Selection getPathRelinkingSelection() const { return pr_selection; }

    double getPathRelinkingAlphaBlockSize() const { return alpha_block_size; }

    double getPathRelinkingPercentage() const { return pr_percentage; }

    unsigned getNumIndependentPopulations() const {
        return num_independent_populations;
    }

    unsigned getMaxThreads() const { return MAX_THREADS; }

    unsigned getExchangeInterval() const { return exchange_interval; }

    unsigned getNumExchangeIndivuduals() const {
        return num_exchange_indivuduals;
    }

    unsigned getResetInterval() const { return reset_interval; }
    //@}

protected:
    /** \name BRKGA Hyper-parameters */
    //@{
    /// Indicate if is maximization or minimization.
    const Sense OPT_SENSE;

    /// Number of genes in the chromosome.
    const unsigned CHROMOSOME_SIZE;

    /// Number of elements in the population.
    unsigned population_size;

    /// Number of elite items in the population.
    unsigned elite_size;

    /// Number of mutants introduced at each generation into the population.
    unsigned num_mutants;

    /// Number of elite parents for mating.
    unsigned num_elite_parents;

    /// Number of total parents for mating.
    unsigned total_parents;

    /// Type of bias that will be used.
    BiasFunction bias;

    /// If false, no evolution is performed but only chromosome decoding.
    /// Very useful to emulate a multi-start algorithm.
    bool evolutionary_mechanism_on;
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

    /** \name Parallel computing parameters */
    //@{
    /// Number of independent parallel populations.
    unsigned num_independent_populations;

    /// Number of threads for parallel decoding.
    const unsigned MAX_THREADS;
    //@}

    /** \name Additional control parameters
     *
     * These parameters are not used directly in the BRKGA nor in the path
     * relinking. However, they are loaded from the configuration file and can
     * be called by the user to perform out-loop controlling.
     */
    //@{
    /// Interval at which elite chromosomes are exchanged (0 means no exchange).
    unsigned exchange_interval;

    /// Number of elite chromosomes exchanged from each population.
    unsigned num_exchange_indivuduals;

    /// Interval at which the populations are reset (0 means no reset).
    unsigned reset_interval;
    //@}

protected:
    /** \name Engines */
    //@{
    /// Reference to the problem-dependent Decoder.
    Decoder& decoder;

    /// Mersenne twister random number generator.
    std::mt19937 rng;
    //@}

    /** \name Algorithm data */
    //@{
    /// Previous population.
    std::vector<std::shared_ptr<Population>> previous;

    /// Current population.
    std::vector<std::shared_ptr<Population>> current;

    /// Reference for the bias function as defined by #bias.
    std::function<double(const double)> bias_function;

    /// Holds the sum of the results of each raking given a bias function.
    /// This value is needed to normalization.
    double total_bias_weight;

    /// Used to shuffled individual/chromosome indices during the mate.
    std::vector<unsigned> shuffled_individuals;

    /// Defines the order of parents during the mating.
    std::vector<std::pair<double, unsigned>> parents_ordered;

    /// Indicates if a initial population is set.
    bool initial_population;

    /// Indicates if the algorithm was proper initialized.
    bool initialized;

    /// Indicates if the algorithm have been reset.
    bool reset_phase;

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
     * \brief Performs the direct path relinking, changing each allele or block
     * of alleles of base chromosome for the correspondent one in the guide
     * chromosome.
     *
     * This method is a multi-thread implementation. Instead of to build and
     * decode each chromosome one at a time, the method builds a list of
     * candidates, altering the alleles/keys according to the guide solution,
     * and then decode all candidates in parallel. Note that
     * O(CHROMOSOME_SIZE^2 / BLOCK_SIZE) additional memory is necessary to build
     * the candidates, which can be costly if the CHROMOSOME_SIZE is very large.
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
                          std::pair<double, Chromosome>& best_found,
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
                                   std::pair<double, Chromosome>& best_found,
                                   std::size_t block_size,
                                   long max_time,
                                   double percentage);
    //@}

    /** \name Helper functions */
    //@{
    /**
     * \brief Reads the configuration file to setup the BRKGA and Path Relinking
     *        parameters.
     *
     * \param filename the configuration file.
     * \throw std::fstream::failure in case of errors in the file.
     */
    void readConfiguration(const std::string& filename);

    /** \brief Checks the parameters given in the constructors. It also
     * initializes some data structures.
     *
     * \throw std::range_error in case some parameter is not correct.
     */
    void checkConfigurationAndInit();

    /**
     * \brief Returns true if a1 is better than a2.
     *
     * This method depends on the optimization sense. When the optimization
     * sense is `Sense::MINIMIZE`, `a1 < a2` will return true, otherwise false.
     * The opposite happens for `Sense::MAXIMIZE`.
     */
    inline bool betterThan(const double a1, const double a2) const;

    /// Evenly distributes real values of given precision across [0, 1].
    inline double rand01();

    /// Returns a number between 0 and n.
    inline uint_fast32_t randInt(const uint_fast32_t n);
    //@}
};

//----------------------------------------------------------------------------//

template<class Decoder>
BRKGA_MP_IPR<Decoder>::BRKGA_MP_IPR(
        Decoder& _decoder_reference,
        Sense _sense,
        unsigned _seed,
        unsigned _chromosome_size,
        unsigned _population_size,
        double _elite_percentage,
        double _mutants_percentage,
        bool _evolutionary_mechanism_on,
        unsigned _num_elite_parents,
        unsigned _total_parents,
        BiasFunction _bias,
        unsigned _num_independent_populations,
        unsigned _max_threads):

        // Algorithm parameters.
        OPT_SENSE(_sense),
        CHROMOSOME_SIZE(_chromosome_size),
        population_size(_population_size),
        elite_size(_evolutionary_mechanism_on?
                    unsigned(_elite_percentage * population_size) : 1),
        num_mutants(_evolutionary_mechanism_on?
                    unsigned(_mutants_percentage * population_size):
                    population_size - 1),
        num_elite_parents(_num_elite_parents),
        total_parents(_total_parents),
        bias(_bias),
        evolutionary_mechanism_on(_evolutionary_mechanism_on),
        pr_number_pairs(0),
        pr_minimum_distance(0.0),
        pr_type(PathRelinking::Type::DIRECT),
        pr_selection(PathRelinking::Selection::BESTSOLUTION),
        alpha_block_size(1.0),
        pr_percentage(1.0),
        num_independent_populations(_num_independent_populations),
        MAX_THREADS(_max_threads),
        exchange_interval(0),
        num_exchange_indivuduals(0),
        reset_interval(0),

        // Internal data.
        decoder(_decoder_reference),
        rng(_seed),
        previous(num_independent_populations, nullptr),
        current(num_independent_populations, nullptr),
        bias_function(),
        total_bias_weight(0.0),
        shuffled_individuals(population_size),
        parents_ordered(total_parents),
        initial_population(false),
        initialized(false),
        reset_phase(false),
        pr_start_time()
{
    checkConfigurationAndInit();
}

//----------------------------------------------------------------------------//

template<class Decoder>
BRKGA_MP_IPR<Decoder>::BRKGA_MP_IPR(
    Decoder& _decoder_reference,
    Sense _sense,
    unsigned _seed,
    unsigned _chromosome_size,
    const std::string& _configuration_file,
    bool _evolutionary_mechanism_on,
    unsigned _max_threads):

        // Algorithm parameters.
        OPT_SENSE(_sense),
        CHROMOSOME_SIZE(_chromosome_size),
        population_size(0),
        elite_size(0),
        num_mutants(0),
        num_elite_parents(0),
        total_parents(0),
        bias(BRKGA::BiasFunction::LOGINVERSE),
        evolutionary_mechanism_on(_evolutionary_mechanism_on),
        pr_number_pairs(0),
        pr_minimum_distance(0.0),
        pr_type(PathRelinking::Type::DIRECT),
        pr_selection(PathRelinking::Selection::BESTSOLUTION),
        alpha_block_size(0.0),
        pr_percentage(0.0),
        num_independent_populations(1),
        MAX_THREADS(_max_threads),
        exchange_interval(0),
        num_exchange_indivuduals(0),
        reset_interval(0),

        // Internal data.
        decoder(_decoder_reference),
        rng(_seed),
        previous(),
        current(),
        bias_function(),
        total_bias_weight(0.0),
        shuffled_individuals(0),
        parents_ordered(0),
        initial_population(false),
        initialized(false),
        reset_phase(false),
        pr_start_time()
{
    readConfiguration(_configuration_file);
    checkConfigurationAndInit();

    previous.resize(num_independent_populations, nullptr);
    current.resize(num_independent_populations, nullptr);
    shuffled_individuals.resize(population_size);
    parents_ordered.resize(total_parents);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::readConfiguration(const std::string& filename) {
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
        {"ELITE_PARENTS", false},
        {"TOTAL_PARENTS", false},
        {"BIAS_FUNCTION", false},
        {"INDEPENDENT_POPULATIONS", false},
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

    double elite_percentage = 0.0;
    double mutants_percentage = 0.0;

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
            error_msg << "Invalid token on line " << line_count
                      << ": " << token;
            throw std::fstream::failure(error_msg.str());
        }

        if(tokens[token]) {
            error_msg << "Duplicate attribute on line " << line_count
                      << ": " << token << " already read!";
            throw std::fstream::failure(error_msg.str());
        }

        std::stringstream data_stream(data);
        bool fail = false;

        // TODO: for c++17, we may use std:any to short this code using a loop.
        if(token == "POPULATION_SIZE")
            fail = !bool(data_stream >> population_size);
        else
        if(token == "ELITE_PERCENTAGE")
            fail = !bool(data_stream >> elite_percentage);
        else
        if(token == "MUTANTS_PERCENTAGE")
            fail = !bool(data_stream >> mutants_percentage);
        else
        if(token == "ELITE_PARENTS")
            fail = !bool(data_stream >> num_elite_parents);
        else
        if(token == "TOTAL_PARENTS")
            fail = !bool(data_stream >> total_parents);
        else
        if(token == "BIAS_FUNCTION")
            fail = !bool(data_stream >> bias);
        else
        if(token == "INDEPENDENT_POPULATIONS")
            fail = !bool(data_stream >> num_independent_populations);
        else
        if(token == "PR_NUMBER_PAIRS")
            fail = !bool(data_stream >> pr_number_pairs);
        else
        if(token == "PR_MINIMUM_DISTANCE")
            fail = !bool(data_stream >> pr_minimum_distance);
        else
        if(token == "PR_TYPE")
            fail = !bool(data_stream >> pr_type);
        else
        if(token == "PR_SELECTION")
            fail = !bool(data_stream >> pr_selection);
        else
        if(token == "ALPHA_BLOCK_SIZE")
            fail = !bool(data_stream >> alpha_block_size);
        else
        if(token == "PR_PERCENTAGE")
            fail = !bool(data_stream >> pr_percentage);
        else
        if(token == "EXCHANGE_INTERVAL")
            fail = !bool(data_stream >> exchange_interval);
        else
        if(token == "NUM_EXCHANGE_INDIVUDUALS")
            fail = !bool(data_stream >> num_exchange_indivuduals);
        else
        if(token == "RESET_INTERVAL")
            fail = !bool(data_stream >> reset_interval);

        if(fail) {
            error_msg << "Invalid value for '" << token
                      << "' on line "<< line_count
                      << ": '" << data << "'";
            throw std::fstream::failure(error_msg.str());
        }

        tokens[token] = true;
    }

    for(const auto& attribute_flag : tokens) {
        if(!attribute_flag.second) {
            error_msg << "Argument '" << attribute_flag.first
                      << "' was not supplied in the config file";
            throw std::fstream::failure(error_msg.str());
        }
    }

    elite_size = evolutionary_mechanism_on?
                 unsigned(elite_percentage * population_size) : 1;

    num_mutants = evolutionary_mechanism_on?
                  unsigned(mutants_percentage * population_size) :
                  population_size - 1;
}


//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::writeConfiguration(const std::string& filename) {
    std::ofstream output(filename, std::ios::out);
    if(!output) {
        std::stringstream error_msg;
        error_msg << "File '" << filename << "' cannot be opened!";
        throw std::fstream::failure(error_msg.str());
    }

    const double elite_percentage = (double) elite_size / population_size;
    const double mutants_percentage = (double) num_mutants / population_size;

    output << "population_size " << population_size << "\n"
           << "elite_percentage " << elite_percentage << "\n"
           << "mutants_percentage " << mutants_percentage << "\n"
           << "elite_parents " << num_elite_parents << "\n"
           << "total_parents " << total_parents << "\n"
           << "bias_function " << bias << "\n"
           << "independent_populations " << num_independent_populations << "\n"
           << "pr_number_pairs " << pr_number_pairs << "\n"
           << "pr_minimum_distance " << pr_minimum_distance << "\n"
           << "pr_type " << pr_type << "\n"
           << "pr_selection " << pr_selection << "\n"
           << "alpha_block_size " << alpha_block_size << "\n"
           << "pr_percentage " << pr_percentage << "\n"
           << "exchange_interval " << exchange_interval << "\n"
           << "num_exchange_indivuduals " << num_exchange_indivuduals << "\n"
           << "reset_interval " << reset_interval
           << std::endl;

    output.close();
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::checkConfigurationAndInit() {
    using std::range_error;
    std::stringstream ss;

    if(CHROMOSOME_SIZE == 0)
        ss << "Chromosome size must be larger than zero: " << CHROMOSOME_SIZE;
    else
    if(population_size == 0)
        ss << "Population size must be larger than zero: " << population_size;
    else
    if(elite_size == 0)
        ss << "Elite-set size equals zero.";
    else
    if(elite_size > population_size)
        ss << "Elite-set size (" << elite_size
           << ") greater than population size (" << population_size << ")";
    else
    if(num_mutants > population_size)
        ss << "Mutant-set size (" << num_mutants
           << ") greater than population size (" << population_size << ")";
    else
    if(elite_size + num_mutants > population_size)
        ss << "Elite (" << elite_size << ") + mutant sets (" << num_mutants
           << ") greater than population size (" << population_size << ")";
    else
    if(num_elite_parents < 1)
        ss << "Num_elite_parents must be at least 1: " << num_elite_parents;
    else
    if(total_parents < 2)
        ss << "Total_parents must be at least 2: " << total_parents;
    else
    if(num_elite_parents >= total_parents)
        ss << "Num_elite_parents (" << num_elite_parents << ") "
           << "is greater than total_parents (" << total_parents << ")";
    else
    if(num_elite_parents > elite_size)
        ss << "Num_elite_parents (" << num_elite_parents
           << ") is greater than elite set (" << elite_size << ")";
    else
    if(num_independent_populations == 0)
        ss << "Number of parallel populations cannot be zero.";
    else
    if(alpha_block_size < 1e-6)
        ss << "(alpha) block size <= 0.0";
    else
    if(pr_percentage < 1e-6 || pr_percentage > 1.0)
        ss << "Path relinking percentage (" << pr_percentage
           << ") is not in the range (0, 1].";

    const auto str_error = ss.str();
    if(str_error.length() > 0)
        throw range_error(str_error);

    // Chooses the bias function.
    switch(bias) {
    case BiasFunction::LOGINVERSE:
        // Same as log(r + 1), but avoids precision loss.
        setBiasCustomFunction(
            [](const double r) { return 1 / log1p(r); }
        );
        break;

    case BiasFunction::LINEAR:
        setBiasCustomFunction(
            [](const double r) { return 1 / r; }
        );
        break;

    case BiasFunction::QUADRATIC:
        setBiasCustomFunction(
            [](const double r) { return pow(r, -2); }
        );
        break;

    case BiasFunction::CUBIC:
        setBiasCustomFunction(
                [](const double r) { return pow(r, -3); }
        );
        break;

    case BiasFunction::EXPONENTIAL:
        setBiasCustomFunction(
                [](const double r) { return exp(-r); }
        );
        break;

    case BiasFunction::CONSTANT:
    default:
        setBiasCustomFunction(
                [&](const double) { return 1.0 / total_parents; }
        );
        break;
    }
}

//----------------------------------------------------------------------------//

template<class Decoder>
inline bool BRKGA_MP_IPR<Decoder>::betterThan(const double a1,
                                              const double a2) const {
    return (a1 < a2) == (OPT_SENSE == Sense::MINIMIZE);
}

//----------------------------------------------------------------------------//

template<class Decoder>
const Population&
BRKGA_MP_IPR<Decoder>::getCurrentPopulation(unsigned population_index) const {
    if(population_index >= current.size())
        throw std::range_error("The index is larger than number of "
                               "populations");
    return (*current[population_index]);
}

//----------------------------------------------------------------------------//

template<class Decoder>
double BRKGA_MP_IPR<Decoder>::getBestFitness() const {
    double best = current[0]->fitness[0].first;
    for(unsigned i = 1; i < num_independent_populations; ++i) {
        if(betterThan(current[i]->fitness[0].first, best))
            best = current[i]->fitness[0].first;
    }
    return best;
}

//----------------------------------------------------------------------------//

template<class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getBestChromosome() const {
    unsigned best_k = 0;
    for(unsigned i = 1; i < num_independent_populations; ++i)
        if(betterThan(current[i]->getBestFitness(),
                      current[best_k]->getBestFitness()))
            best_k = i;

    // Since the chromosomes are ordered by fitness, the first is the best.
    return current[best_k]->getChromosome(0);
}

//----------------------------------------------------------------------------//

template<class Decoder>
const Chromosome& BRKGA_MP_IPR<Decoder>::getChromosome(
        unsigned population_index, unsigned position) const {
    if(population_index >= current.size())
        throw std::range_error("The population index is larger than number of "
                               "populations");
    if(position >= population_size)
        throw std::range_error("The chromosome position is larger than number "
                               "of populations");

    return current[population_index]->getChromosome(position);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::injectChromosome(const Chromosome& chromosome,
        unsigned population_index, unsigned position, double fitness) {
    if(population_index >= current.size())
        throw std::range_error("The population index is larger than number of "
                               "populations");

    if(position >= population_size)
        throw std::range_error("The chromosome position is larger than number "
                               "of populations");

    if(chromosome.size() != CHROMOSOME_SIZE)
        throw std::range_error("Wrong chromosome size");

    auto& pop = current[population_index];
    auto& local_chr = pop->population[pop->fitness[position].second];
    local_chr = chromosome;

    if(!(fitness < std::numeric_limits<double>::infinity()))
        fitness = decoder.decode(local_chr);

    pop->setFitness(position, fitness);
    pop->sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::setBiasCustomFunction(
            const std::function<double(const double)>& func) {

    bias_function = func;
    total_bias_weight = 0.0;
    for(unsigned i = 1; i < total_parents + 1; ++i)
        total_bias_weight += bias_function(i);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::reset() {
    if(!initialized) {
        throw std::runtime_error("The algorithm hasn't been initialized. "
                                 "Don't forget to call initialize() method");
    }
    reset_phase = true;
    initialize(false);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::evolve(unsigned generations) {
    if(!initialized)
        throw std::runtime_error("The algorithm hasn't been initialized. "
                                 "Don't forget to call initialize() method");

    if(generations == 0)
        throw std::range_error("Cannot evolve for 0 generations.");

    for(unsigned i = 0; i < generations; ++i) {
        for(unsigned j = 0; j < num_independent_populations; ++j) {
            // First evolve the population (current, next).
            evolution(*current[j], *previous[j]);
            std::swap(current[j], previous[j]);
        }
    }
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::exchangeElite(unsigned num_immigrants) {
    if(num_independent_populations == 1)
        return;

    unsigned immigrants_threshold =
        ceil(population_size / (num_independent_populations - 1));

    if(num_immigrants < 1 || num_immigrants >= immigrants_threshold) {
        std::stringstream ss;
        ss << "Number of immigrants (" << num_immigrants << ") less than one, "
              "or larger than or equal to population size / "
              "num_independent_populations (" << immigrants_threshold << ")";
        throw std::range_error(ss.str());
    }

    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS)
    #endif
    for(unsigned i = 0; i < num_independent_populations; ++i) {
        // Population i will receive some elite members from each Population j.
        // Last chromosome of i (will be overwritten below).
        unsigned dest = population_size - 1;
        for(unsigned j = 0; j < num_independent_populations; ++j) {
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
    for(unsigned i = 0; i < num_independent_populations; ++i)
        current[i]->sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::setInitialPopulation(
                                const std::vector<Chromosome>& chromosomes) {

    if(chromosomes.size() > population_size) {
        std::stringstream ss;
        ss << "Number of given chromosomes (" << chromosomes.size()
           << ") is larger than the population size ("
           << population_size << ")";
        throw std::runtime_error(ss.str());
    }

    current[0].reset(new Population(CHROMOSOME_SIZE, chromosomes.size()));

    unsigned i = 0;
    for(const auto& chr : chromosomes) {
        if(chr.size() != CHROMOSOME_SIZE) {
            std::stringstream ss;
            ss << "Error on setting initial population: chromosome " << i
               << " does not have the required dimension (actual size: "
               << chr.size() << ", required size: " << CHROMOSOME_SIZE << ")";
            throw std::runtime_error(ss.str());
        }
        std::copy(chr.begin(), chr.end(), current[0]->population[i].begin());
        ++i;
    }
    initial_population = true;
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::initialize(bool true_init) {
    // Verify the initial population and complete or prune it!
    unsigned start = 0;
    if(initial_population && true_init) {
        if(current[0]->population.size() < population_size) {
            auto pop = current[0];
            Chromosome chromosome(CHROMOSOME_SIZE);
            unsigned last_chromosome = pop->population.size();

            pop->population.resize(population_size);
            pop->fitness.resize(population_size);

            for(; last_chromosome < population_size; ++last_chromosome) {
                for(unsigned k = 0; k < CHROMOSOME_SIZE; ++k)
                    chromosome[k] = rand01();
                pop->population[last_chromosome] = chromosome;
            }
        }
        // Prune some additional chromosomes.
        else if(current[0]->population.size() > population_size) {
            current[0]->population.resize(population_size);
            current[0]->fitness.resize(population_size);
        }
        start = 1;
    }

    // Initialize each chromosome of the current population.
    for(; start < num_independent_populations; ++start) {
        if(!reset_phase)
            current[start].reset(new Population(CHROMOSOME_SIZE,
                                                population_size));

        for(unsigned j = 0; j < population_size; ++j) {
            for(unsigned k = 0; k < CHROMOSOME_SIZE; ++k) {
                (*current[start])(j, k) = rand01();
            }
        }
    }

    // Initialize and decode each chromosome of the current population,
    // then copy to previous.
    for(unsigned i = 0; i < num_independent_populations; ++i) {
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) schedule(static,1)
        #endif
        for(unsigned j = 0; j < population_size; ++j)
            current[i]->setFitness(j, decoder.decode((*current[i])(j)));

        // Sort and copy to previous.
        current[i]->sortFitness(OPT_SENSE);
        if(!reset_phase)
            previous[i].reset(new Population(*current[i]));

        if(reset_phase && true_init)
            previous[i].reset(new Population(*current[i]));
    }

    initialized = true;
    reset_phase = false;
}

//----------------------------------------------------------------------------//

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::evolution(Population& curr,
                                      Population& next) {
    // First, we copy the elite chromosomes to the next generation.
    for(unsigned chr = 0; chr < elite_size; ++chr) {
        next.population[chr] = curr.population[curr.fitness[chr].second];
        next.fitness[chr] = std::make_pair(curr.fitness[chr].first, chr);
    }

    // Second, we mate 'pop_size - elite_size - num_mutants' pairs.
    for(unsigned chr = elite_size; chr < population_size - num_mutants; ++chr) {
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
        for(unsigned j = 0; j < num_elite_parents; ++j) {
            parents_ordered.emplace_back(curr.fitness[shuffled_individuals[j]]);
        }

        // Take the non-elite parents.
        for(unsigned j = 0; j < total_parents - num_elite_parents; ++j) {
            parents_ordered.emplace_back(
                    curr.fitness[shuffled_individuals[j + elite_size]]);
        }

        if(OPT_SENSE == Sense::MAXIMIZE)
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::greater<std::pair<double, unsigned>>());
        else
            std::sort(parents_ordered.begin(), parents_ordered.end(),
                      std::less<std::pair<double, unsigned>>());

        // Performs the mate.
        for(unsigned allele = 0; allele < CHROMOSOME_SIZE; ++allele) {
            // Start parent from 1 because the bias function.
            unsigned parent = 0;
            double cumulative_probability = 0.0;
            const double toss = rand01();

            do {
                cumulative_probability += bias_function(++parent) /
                                          total_bias_weight;
            } while(cumulative_probability < toss);

            // Decrement parent to the right index, and take the allele value.
            next(chr, allele) = curr(parents_ordered[--parent].second, allele);
        }
    }

    // To finish, we fill up the remaining spots with mutants.
    for(unsigned chr = population_size - num_mutants; chr < population_size;
        ++chr) {
        for(auto& allele : next.population[chr])
            allele = rand01();
    }

    // Time to compute fitness, in parallel.
    #ifdef _OPENMP
        #pragma omp parallel for num_threads(MAX_THREADS) schedule(static, 1)
    #endif
    for(unsigned i = elite_size; i < population_size; ++i)
        next.setFitness(i, decoder.decode(next.population[i]));

    // Now we must sort by fitness, since things might have changed.
    next.sortFitness(OPT_SENSE);
}

//----------------------------------------------------------------------------//

template<class Decoder>
bool BRKGA_MP_IPR<Decoder>::pathRelink(
                     std::shared_ptr<DistanceFunctionBase> dist,
                     unsigned number_pairs,
                     double minimum_distance,
                     PathRelinking::Type pathrelinkingtype,
                     PathRelinking::Selection selection,
                     std::size_t block_size,
                     long max_time, double percentage) {

    if(percentage < 1e-6 || percentage > 1.0)
        throw std::range_error("Percentage/size of path relinking invalid.");

    if(max_time <= 0)
        max_time = std::numeric_limits<long>::max();

    Chromosome initial_solution(CHROMOSOME_SIZE);
    Chromosome guiding_solution(CHROMOSOME_SIZE);

    // Perform path relinking between elite chromosomes from different
    // populations. This is done in a circular fashion.

    bool path_relinking_possible = false;
    std::deque<std::pair<std::size_t, std::size_t>> index_pairs;

    // Keep track of the time.
    pr_start_time = std::chrono::system_clock::now();

    for(unsigned pop_count = 0; pop_count < num_independent_populations;
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
        if(num_independent_populations == 1) {
            pop_base = pop_guide = 0;
            pop_count = num_independent_populations;
        }
        // If we have two populations, perform just one path relinking.
        else if(num_independent_populations == 2) {
            pop_count = num_independent_populations;
        }

        // Do the circular thing
        if(pop_guide == num_independent_populations)
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
                    (selection == PathRelinking::Selection::BESTSOLUTION? 0 :
                     randInt(index_pairs.size() - 1));

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

        path_relinking_possible |= found_pair;

        // The elite sets are too homogeneous, we cannot do
        // a good path relinking. Let's try other populations.
        if(!found_pair)
            continue;

        // Create a empty solution.
        std::pair<double, Chromosome> best_found;
        best_found.second.resize(current[0]->getChromosomeSize(), 0.0);

        const bool sense = OPT_SENSE == Sense::MAXIMIZE;
        best_found.first = sense? std::numeric_limits<double>::lowest():
                                  std::numeric_limits<double>::max();

        // Perform the path relinking.
        if(pathrelinkingtype == PathRelinking::Type::DIRECT) {
            directPathRelink(initial_solution, guiding_solution, dist,
                             best_found, block_size, max_time, percentage);
        }
        else {
            permutatioBasedPathRelink(initial_solution, guiding_solution, dist,
                                      best_found, block_size, max_time,
                                      percentage);
        }

        // Re-decode and apply local search if the decoder are able to do it.
        best_found.first = decoder.decode(best_found.second, true);

        // Now, check if the best solution found is really good.
        // If it is the best, overwrite the worse solution in the population.
        bool include_in_population =
           (sense && best_found.first > current[pop_base]->fitness[0].first) ||
           (!sense && best_found.first < current[pop_base]->fitness[0].first);

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
        }
    }

    return path_relinking_possible;
}

//----------------------------------------------------------------------------//

template<class Decoder>
bool BRKGA_MP_IPR<Decoder>::pathRelink(
            std::shared_ptr<DistanceFunctionBase> dist,
            long max_time) {

    size_t block_size = ceil(alpha_block_size * sqrt(population_size));
    if(block_size > CHROMOSOME_SIZE)
        block_size = CHROMOSOME_SIZE / 2;

    return pathRelink(dist, pr_number_pairs, pr_minimum_distance, pr_type,
                      pr_selection, block_size, max_time, pr_percentage);
}

//----------------------------------------------------------------------------//

// This is a multi-thread version. For small chromosomes, it may be slower than
// single thread version.
template<class Decoder>
void BRKGA_MP_IPR<Decoder>::directPathRelink(
            const Chromosome& chr1, const Chromosome& chr2,
            std::shared_ptr<DistanceFunctionBase> dist,
            std::pair<double, Chromosome>& best_found,
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
        double fitness;
        std::size_t block_index;
        Triple(): chr(), fitness(0.0), block_index(0) {}
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

        // Decode the candidates.
        volatile bool times_up = false;
        #ifdef _OPENMP
            #pragma omp parallel for num_threads(MAX_THREADS) shared(times_up) \
                schedule(static, 1)
        #endif
        for(std::size_t i = 0; i < remaining_blocks.size(); ++i) {
            (*candidates_base)[i].fitness =
                    (sense)? std::numeric_limits<double>::lowest() :
                             std::numeric_limits<double>::max();

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

        double best_value = (sense)? std::numeric_limits<double>::lowest() :
                                     std::numeric_limits<double>::max();

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

template<class Decoder>
void BRKGA_MP_IPR<Decoder>::permutatioBasedPathRelink(
                Chromosome& chr1, Chromosome& chr2,
                std::shared_ptr<DistanceFunctionBase> /*non-used*/,
                std::pair<double, Chromosome>& best_found,
                std::size_t /*non-used block_size*/,
                long max_time, double percentage) {

    const std::size_t PATH_SIZE = std::size_t(percentage * CHROMOSOME_SIZE);

    std::set<std::size_t> remaining_indices;
    for(std::size_t i = 0; i < chr1.size(); ++i)
        remaining_indices.insert(i);

    struct DecodeStruct {
    public:
        Chromosome chr;
        double fitness;
        std::size_t key_index;
        std::size_t pos1;
        std::size_t pos2;
        DecodeStruct(): chr(), fitness(0.0),
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
            (*candidates_base)[i].fitness =
                    (sense)? std::numeric_limits<double>::lowest() :
                             std::numeric_limits<double>::max();

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
        double best_value = (sense)? std::numeric_limits<double>::lowest() :
                                     std::numeric_limits<double>::max();

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

template<class Decoder>
inline double BRKGA_MP_IPR<Decoder>::rand01() {
    // **NOTE:** instead to use std::generate_canonical<> (which can be
    // a little bit slow), we may use
    //    rng() * (1.0 / std::numeric_limits<std::mt19937::result_type>::max());
    // However, this approach has some precision problems on some platforms
    // (notably Linux)

    return std::generate_canonical<double, std::numeric_limits<double>::digits>
          (rng);
}

//----------------------------------------------------------------------------//

template<class Decoder>
inline uint_fast32_t BRKGA_MP_IPR<Decoder>::randInt(const uint_fast32_t n) {
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
 * **NOTE 1:** the specializations must be done in the global namespace.
 * **NOTE 2:** the specialization must be inline-d to avoid multiple
 *             definitions issues across different modules.
 */
///@{

/// Template specialization to BRKGA::Sense.
template <>
const std::vector<std::string>& EnumIO<BRKGA::Sense>::enum_names() {
    static std::vector<std::string> enum_names_({
        "MINIMIZE",
        "MAXIMIZE"
    });
     return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Type.
template <>
const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Type>::enum_names() {
    static std::vector<std::string> enum_names_({
        "DIRECT",
        "PERMUTATION"
    });
     return enum_names_;
}

/// Template specialization to BRKGA::PathRelinking::Selection.
template <>
const std::vector<std::string>&
EnumIO<BRKGA::PathRelinking::Selection>::enum_names() {
    static std::vector<std::string> enum_names_({
        "BESTSOLUTION",
        "RANDOMELITE"
    });
     return enum_names_;
}

/// Template specialization to BRKGA::BiasFunction.
template <>
const std::vector<std::string>&
EnumIO<BRKGA::BiasFunction>::enum_names() {
    static std::vector<std::string> enum_names_({
        "CONSTANT",
        "CUBIC",
        "EXPONENTIAL",
        "LINEAR",
        "LOGINVERSE",
        "QUADRATIC"
    });
     return enum_names_;
}
///@}

#endif // BRKGA_MP_IPR_HPP_
