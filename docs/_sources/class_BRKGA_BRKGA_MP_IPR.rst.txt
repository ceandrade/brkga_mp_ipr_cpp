.. index:: pair: class; BRKGA::BRKGA_MP_IPR
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r:

template class BRKGA::BRKGA_MP_IPR
==================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

This class represents a Multi-Parent Biased Random-key Genetic Algorithm with Implicit Path Relinking (BRKGA-MP-IPR). :ref:`More...<details-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	template <class Decoder>
	class BRKGA_MP_IPR {
	public:
		// construction
	
		:ref:`BRKGA_MP_IPR<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a041a64b7b5a81b563fd54cfbeec1bb96>`(
			Decoder& decoder_reference,
			const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense,
			const unsigned seed,
			const unsigned chromosome_size,
			const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& params,
			const unsigned max_threads = 1,
			const bool evolutionary_mechanism_on = true
			);

		// methods
	
		void :ref:`setInitialPopulation<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>`(const std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`>& chromosomes);
		void :ref:`setBiasCustomFunction<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>`(const std::function<double(const unsigned)>& func);
		void :ref:`initialize<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a65fbb0b0c6b2daba98f346601354d957>`(bool reset = false);
		void :ref:`evolve<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>`(unsigned generations = 1);
	
		:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` :ref:`pathRelink<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>`(
			:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type,
			:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection,
			std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
			unsigned number_pairs,
			double minimum_distance,
			std::size_t block_size = 1,
			long max_time = 0,
			double percentage = 1.0
			);
	
		:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` :ref:`pathRelink<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a891815a683e55722a68cf52f47efe44e>`(
			std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
			long max_time = 0
			);
	
		void :ref:`exchangeElite<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>`(unsigned num_immigrants);
		void :ref:`reset<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>`();
	
		void :ref:`shake<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>`(
			unsigned intensity,
			:ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` shaking_type,
			unsigned population_index = std::numeric_limits<unsigned>::infinity()
			);
	
		void :ref:`injectChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>`(
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome,
			unsigned population_index,
			unsigned position
			);
	
		const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& :ref:`getCurrentPopulation<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a878b5edaec7438669ceec2e5fdd42d43>`(unsigned population_index = 0) const;
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& :ref:`getBestChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>`() const;
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` :ref:`getBestFitness<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39>`() const;
	
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f>`(
			unsigned population_index,
			unsigned position
			) const;
	
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` :ref:`getFitness<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a9d6636a50f519bf0f1e85257282c6065>`(
			unsigned population_index,
			unsigned position
			) const;
	
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& :target:`getBrkgaParams<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a27bdc9e01cbb9e5ac180adb1afcc92f0>`() const;
		:ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` :target:`getOptimizationSense<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a5a4466f754d00b07cfc64e4a6da21f9a>`() const;
		unsigned :target:`getChromosomeSize<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab5737430a77071ce139dca80cdcf521b>`() const;
		unsigned :target:`getEliteSize<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1af1618bca3c4b10af17fdde43dad7b147>`() const;
		unsigned :target:`getNumMutants<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a55127a2fcc65d31ad90a20a0c798f517>`() const;
		bool :target:`evolutionaryIsMechanismOn<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a5edb36fe57167b00d8ae7babd4f867cb>`() const;
		unsigned :target:`getMaxThreads<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a6dcc3a60d2c08271053e8c84498f69bd>`() const;
	};
.. _details-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

This class represents a Multi-Parent Biased Random-key Genetic Algorithm with Implicit Path Relinking (BRKGA-MP-IPR).

Carlos Eduardo de Andrade `ce.andrade@gmail.com <mailto:ce.andrade@gmail.com>`__

2021



.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1main_cap:

Main capabilities
~~~~~~~~~~~~~~~~~



.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1evol_process:

Evolutionary process
--------------------

In the BRKGA-MP-IPR, we keep a population of chromosomes divided between the elite and the non-elite group. During the mating, multiple parents are chosen from the elite group and the non-elite group. They are sorted either on no-decreasing order for minimization or non-increasing order to maximization problems. Given this order, a bias function is applied to the rank of each chromosome, resulting in weight for each one. Using a roulette method based on the weights, the chromosomes are combined using a biased crossover.

This code also implements the island model, where multiple populations can be evolved in parallel, and migration between individuals between the islands are performed using :ref:`exchangeElite() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>` method.

This code requires the template argument ``Decoder`` be a class or functor object capable to map a chromosome to a solution for the specific problem, and return a value to be used as fitness to the decoded chromosome. The decoder must have the method

.. ref-code-block:: cpp

	double decode(:ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome, bool rewrite);

where :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>` is a ``vector<double>`` representing a solution and ``rewrite`` is a boolean indicating that if the decode should rewrite the chromosome in case it implements local searches and modifies the initial solution decoded from the chromosome. Since this API has the capability of decoding several chromosomes in parallel, the user must guarantee that ``Decoder::decode(...)`` is thread-safe. Therefore, we do recommend to have the writable variables per thread. Please, see the example that follows this code.





.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ipr:

Implicit Path Relinking
-----------------------

This API also implements the Implicit Path Relinking leveraging the decoder capabilities. To perform the path relinking, the user must call :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>` method, indicating the type of path relinking (direct or permutation-based, see :ref:`PathRelinking::Type <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>`), the selection criteria (best solution or random elite, see :ref:`PathRelinking::Selection <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`), the distance function (to choose individuals far enough, see :ref:`BRKGA::DistanceFunctionBase <doxid-class_b_r_k_g_a_1_1_distance_function_base>`, :ref:`BRKGA::HammingDistance <doxid-class_b_r_k_g_a_1_1_hamming_distance>`, and :ref:`BRKGA::KendallTauDistance <doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`), a maximum time or a maximum path size.

In the presence of multiple populations, the path relinking is performed between elite chromosomes from different populations, in a circular fashion. For example, suppose we have 3 populations. The framework performs 3 path relinkings: the first between individuals from populations 1 and 2, the second between populations 2 and 3, and the third between populations 3 and 1. In the case of just one population, both base and guiding individuals are sampled from the elite set of that population.

Note that the algorithm tries to find a pair of base and guiding solutions with a minimum distance given by the distance function. If this is not possible, a new pair of solutions are sampled (without replacement) and tested against the distance. In case it is not possible to find such pairs for the given populations, the algorithm skips to the next pair of populations (in a circular fashion, as described above). Yet, if such pairs are not found in any case, the algorithm declares failure. This indicates that the populations are very homogeneous.

The API will call ``Decoder::decode()`` always with ``rewrite = false``. The reason is that if the decoder rewrites the chromosome, the path between solutions is lost and inadvertent results may come up. Note that at the end of the path relinking, the method calls the decoder with ``rewrite = true`` in the best chromosome found to guarantee that this chromosome is re-written to reflect the best solution found.







.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1other_cap:

Other capabilities
~~~~~~~~~~~~~~~~~~



.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1multi_start:

Multi-start
-----------

This API also can be used as a simple multi-start algorithm without evolution. To do that, the user must supply in the constructor the argument ``evolutionary_mechanism_on = false``. This argument makes the elite set has one individual and the number of mutants n - 1, where n is the size of the population. This setup disables the evolutionary process completely.





.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1init_pop:

Initial Population
------------------

This API allows the user provides a set of initial solutions to warm start the algorithm. In general, initial solutions are created using other (fast) heuristics and help the convergence of the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`. To do that, the user must encode the solutions on :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>` (``vector<double>``) and pass to the method :ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>` as a ``vector<#Chromosome>``.







.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1gen_usage:

General Usage
~~~~~~~~~~~~~

#. The user must call the :ref:`BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>` constructor and pass the desired parameters. Please, see :ref:`BRKGA_MP_IPR::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a041a64b7b5a81b563fd54cfbeec1bb96>` for parameter details;
   
   #. (Optional) The user provides the warm start solutions using :ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>`;

#. The user must call the method :ref:`initialize() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a65fbb0b0c6b2daba98f346601354d957>` to start the data structures and perform the decoding of the very first populations;

#. Main evolutionary loop:
   
   #. On each iteration, the method :ref:`evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>` should be called to perform the evolutionary step (or multi-steps if desired);
   
   #. The user can check the current best chromosome (:ref:`getBestChromosome() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>`) and its fitness (:ref:`getBestFitness() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39>`) and perform checking and logging operations;
   
   #. (Optional) The user can perform the individual migration between populations (:ref:`exchangeElite() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>`);
   
   #. (Optional) The user can perform the path relinking (:ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>`);
   
   #. (Optional) The user can reset and start the algorithm over (:ref:`reset() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>`);

For a comprehensive and detailed usage, please see the examples that follow this API.





.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1multi_thread:

About multi-threading
~~~~~~~~~~~~~~~~~~~~~

This API is capable of decoding several chromosomes in parallel, as mentioned before. This capability is based on OpenMP (`http://www.openmp.org <http://www.openmp.org>`__) and the compiler must have support to it. Most recent versions of GNU G++ and Intel C++ Compiler support OpenMP. Clang supports OpenMP since 3.8. However, there are some issues with the libraries, and sometimes, the parallel sections are not enabled. On the major, the user can find fixes to his/her system.

Since, in general, the decoding process can be complex and lengthy, it is recommended that **the number of threads used DO NOT exceed the number of physical cores in the machine.** This improves the overall performance drastically, avoiding cache misses and racing conditions. Note that the number of threads is also tied to the memory utilization, and it should be monitored carefully.





.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1hist:

History
~~~~~~~

This API was based on the code by Rodrigo Franco Toso, Sep 15, 2011. `http://github.com/rfrancotoso/brkgaAPI <http://github.com/rfrancotoso/brkgaAPI>`__

Construction
------------

.. index:: pair: function; BRKGA_MP_IPR
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a041a64b7b5a81b563fd54cfbeec1bb96:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	BRKGA_MP_IPR(
		Decoder& decoder_reference,
		const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense,
		const unsigned seed,
		const unsigned chromosome_size,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& params,
		const unsigned max_threads = 1,
		const bool evolutionary_mechanism_on = true
		)

Builds the algorithm and its data strtuctures with the given arguments.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- decoder_reference

		- a reference to the decoder object. **NOTE:** :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` uses such object directly for decoding.

	*
		- sense

		- the optimization sense (maximization or minimization).

	*
		- seed

		- the seed for the random number generator.

	*
		- chromosome_size

		- number of genes in each chromosome.

	*
		- params

		- :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR parameters object loaded from a configuration file or manually created. All the data is copied.

	*
		- max_threads

		- 
		  number of threads to perform parallel decoding.
		  
		  **NOTE** : ``Decoder::decode()`` MUST be thread-safe.

	*
		- evolutionary_mechanism_on

		- if false, no evolution is performed but only chromosome decoding. Very useful to emulate a multi-start algorithm.

	*
		- std::range_error

		- if some parameter or combination of parameters does not fit.

Methods
-------

.. index:: pair: function; setInitialPopulation
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void setInitialPopulation(const std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`>& chromosomes)

Sets individuals to initial population.

Set initial individuals into the poulation to work as warm-starters. Such individuals can be obtained from solutions of external procedures such as fast heuristics, other methaheuristics, or even relaxations from a mixed integer programming model that models the problem.

We assign as many individuals as possible across all populations. Extra individuals are disregarded.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosomes

		- a set of individuals encoded as Chromosomes.

.. index:: pair: function; setBiasCustomFunction
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void setBiasCustomFunction(const std::function<double(const unsigned)>& func)

Sets a custom bias function used to build the probabilities.

It must be a **positive non-increasing function**, i.e. :math:`f: \mathbb{N}^+ \to \mathbb{R}^+` such that :math:`f(i) \ge 0` and :math:`f(i) \ge f(i+1)` for :math:`i \in [1..total\_parents]`. For example

.. ref-code-block:: cpp

	:ref:`setBiasCustomFunction <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>`(
	    [](const unsigned x) {
	        return 1.0 / (x * x);
	    }
	);

sets an inverse quadratic function.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- func

		- a reference to a unary positive non-increasing function.

	*
		- std::runtime_error

		- in case the function is not a non-decreasing positive function.

.. index:: pair: function; initialize
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a65fbb0b0c6b2daba98f346601354d957:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void initialize(bool reset = false)

Initializes the populations and others parameters of the algorithm.

If a initial population is supplied, this method completes the remain individuals, if they do not exist. This method also performs the initial decoding of the chromosomes. Therefore, depending on the decoder implementation, this can take a while, and the user may want to time such procedure in his/her experiments.

This method must be call before any evolutionary or population handling method.

As it is in :ref:`evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>`, the decoding is done in parallel using threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If such property cannot be held, we suggest using a single thread for optimization.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- reset

		- when set true, it ignores all solutions provided by ``:ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```, and builds a completely random population. This parameter is set to true during reset phase. When false, ``:ref:`initialize() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a65fbb0b0c6b2daba98f346601354d957>``` considers all solutions given by ``:ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```.

.. index:: pair: function; evolve
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void evolve(unsigned generations = 1)

Evolves the current populations following the guidelines of Multi-parent BRKGAs.

The decoding is done in parallel using threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If such property cannot be held, we suggest using a single thread for optimization.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- generations

		- number of generations to be evolved. Must be larger than zero.

	*
		- std::runtime_error

		- if the algorithm is not initialized.

	*
		- std::range_error

		- if the number of generations is zero.

.. index:: pair: function; pathRelink
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` pathRelink(
		:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type,
		:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection,
		std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
		unsigned number_pairs,
		double minimum_distance,
		std::size_t block_size = 1,
		long max_time = 0,
		double percentage = 1.0
		)

Performs path relinking between elite solutions that are, at least, a given minimum distance between themselves. In this method, the local/loaded parameters are ignored in favor to the supplied ones.

In the presence of multiple populations, the path relinking is performed between elite chromosomes from different populations, in a circular fashion. For example, suppose we have 3 populations. The framework performs 3 path relinkings: the first between individuals from populations 1 and 2, the second between populations 2 and 3, and the third between populations 3 and 1. In the case of just one population, both base and guiding individuals are sampled from the elite set of that population.

Note that the algorithm tries to find a pair of base and guiding solutions with a minimum distance given by the distance function. If this is not possible, a new pair of solutions are sampled (without replacement) and tested against the distance. In case it is not possible to find such pairs for the given populations, the algorithm skips to the next pair of populations (in a circular fashion, as described above). Yet, if such pairs are not found in any case, the algorithm declares failure. This indicates that the populations are very homogeneous.

If the found solution is the best solution found so far, IPR replaces the worst solution by it. Otherwise, IPR computes the distance between the found solution and all other solutions in the elite set, and replaces the worst solution by it if and only if the found solution is, at least, ``minimum_distance`` from all them.

The API will call ``Decoder::decode()`` function always with ``rewrite = false``. The reason is that if the decoder rewrites the chromosome, the path between solutions is lost and inadvertent results may come up. Note that at the end of the path relinking, the method calls the decoder with ``rewrite = true`` in the best chromosome found to guarantee that this chromosome is re-written to reflect the best solution found.

This method is a multi-thread implementation. Instead of to build and decode each chromosome one at a time, the method builds a list of candidates, altering the alleles/keys according to the guide solution, and then decode all candidates in parallel. Note that ``O(chromosome_size^2 / block_size)`` additional memory is necessary to build the candidates, which can be costly if the ``chromosome_size`` is very large.

As it is in :ref:`evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>`, the decoding is done in parallel using threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If such property cannot be held, we suggest using a single thread for optimization.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- pr_type

		- type of path relinking to be performed. See :ref:`PathRelinking::Type <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>`.

	*
		- pr_selection

		- of which individuals use to path relinking. See :ref:`PathRelinking::Selection <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`.

	*
		- dist

		- a pointer to a functor/object to compute the distance between two chromosomes. This object must be inherited from :ref:`BRKGA::DistanceFunctionBase <doxid-class_b_r_k_g_a_1_1_distance_function_base>` and implement its methods.

	*
		- number_pairs

		- number of chromosome pairs to be tested. If 0, all pairs are tested.

	*
		- minimum_distance

		- between two chromosomes computed byt ``dist``.

	*
		- block_size

		- number of alleles to be exchanged at once in each iteration. If one, the traditional path relinking is performed.

	*
		- max_time

		- aborts path relinking when reach ``max_time``. If ``max_time <= 0``, no limit is imposed.

	*
		- percentage

		- defines the size, in percentage, of the path to build. Default: 1.0 (100%).

	*
		- std::range_error

		- if the percentage or size of the path is not in (0, 1].



.. rubric:: Returns:

A :ref:`PathRelinking::PathRelinkingResult <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` depending on the relink status.

.. index:: pair: function; pathRelink
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a891815a683e55722a68cf52f47efe44e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` pathRelink(
		std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
		long max_time = 0
		)

Performs path relinking between elite solutions that are, at least, a given minimum distance between themselves.

This method uses all parameters supplied in the constructor. In particular, the block size is computed by :math:`\lceil \alpha \times \sqrt{p} \rceil` where :math:`\alpha` is :ref:`BrkgaParams::alpha_block_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>` and :math:`p` is :ref:`BrkgaParams::population_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>`. If the size is larger than the chromosome size, the size is set to half of the chromosome size.

Please, refer to :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a95529466a3e942e4aafa26259aa83d0f>` for details.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- dist

		- a pointer to a functor/object to compute the distance between two chromosomes. This object must be inherited from :ref:`BRKGA::DistanceFunctionBase <doxid-class_b_r_k_g_a_1_1_distance_function_base>` and implement its methods.

	*
		- max_time

		- aborts path relinking when reach ``max_time``. If ``max_time <= 0``, no limit is imposed.

	*
		- std::range_error

		- if the percentage or size of the path is not in (0, 1].



.. rubric:: Returns:

A :ref:`PathRelinking::PathRelinkingResult <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` depending on the relink status.

.. index:: pair: function; exchangeElite
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void exchangeElite(unsigned num_immigrants)

Exchanges elite-solutions between the populations.

Given a population, the ``num_immigrants`` best solutions are copied to the neighbor populations, replacing their worth solutions. If there is only one population, nothing is done.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- num_immigrants

		- number of elite chromosomes to select from each population.

	*
		- std::range_error

		- if the number of immigrants less than one or it is larger than or equal to the population size divided by the number of populations minus one, i.e. :math:`\lceil \frac{population\_size}{num\_independent\_populations} \rceil - 1`.

.. index:: pair: function; reset
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void reset()

Resets all populations with brand new keys.

All warm-start solutions provided :ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>` are discarded. You may use :ref:`injectChromosome() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>` to insert those solutions again.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- std::runtime_error

		- if the algorithm is not initialized.

.. index:: pair: function; shake
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void shake(
		unsigned intensity,
		:ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` shaking_type,
		unsigned population_index = std::numeric_limits<unsigned>::infinity()
		)

Performs a shaking in the chosen population.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- intensity

		- the intensity of the shaking.

	*
		- shaking_type

		- either ``CHANGE`` or ``SWAP`` moves.

	*
		- population_index

		- the index of the population to be shaken. If ``population_index >= num_independent_populations``, all populations are shaken.

.. index:: pair: function; injectChromosome
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void injectChromosome(
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& chromosome,
		unsigned population_index,
		unsigned position
		)

Injects/Replaces a chromosome of a given population into a given position.

The new chromosome replaces the old one, and the decoder is triggered to compute the new fitness. Once done, the population is re-sorted according to the chromosomes' fitness.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- the chromosome to be injected.

	*
		- population_index

		- the population index.

	*
		- position

		- the chromosome position.

	*
		- std::range_error

		- eitheir if ``population_index`` is larger than number of populations; or ``position`` is larger than the population size; or ``chromosome.size() != chromosome_size``

.. index:: pair: function; getCurrentPopulation
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a878b5edaec7438669ceec2e5fdd42d43:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& getCurrentPopulation(unsigned population_index = 0) const

Returns a reference to a current population.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- population_index

		- the population index.

	*
		- std::range_error

		- if the index is larger than number of populations.

.. index:: pair: function; getBestChromosome
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getBestChromosome() const

Returns a reference to the chromosome with best fitness so far among all populations.

.. index:: pair: function; getBestFitness
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` getBestFitness() const

Return the best fitness found so far among all populations.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getChromosome(
		unsigned population_index,
		unsigned position
		) const

Returns a reference to a chromosome of the given population.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- population_index

		- the population index.

	*
		- position

		- the chromosome position, ordered by fitness. The best chromosome is located in position 0.

	*
		- std::range_error

		- eitheir if ``population_index`` is larger than number of populations, or ``position`` is larger than the population size.

.. index:: pair: function; getFitness
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a9d6636a50f519bf0f1e85257282c6065:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` getFitness(
		unsigned population_index,
		unsigned position
		) const

Returns the fitness of a chromosome of the given population.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- population_index

		- the population index.

	*
		- position

		- the chromosome position, ordered by fitness. The best chromosome is located in position 0.

	*
		- std::range_error

		- eitheir if ``population_index`` is larger than number of populations, or ``position`` is larger than the population size.

