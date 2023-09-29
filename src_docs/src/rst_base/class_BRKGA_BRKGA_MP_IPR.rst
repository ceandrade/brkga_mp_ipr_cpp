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

		void :ref:`setInitialPopulation<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>`(const std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`>& chromosomes);
		void :ref:`setBiasCustomFunction<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>`(const std::function<double(const unsigned)>& func);
		void :ref:`setShakingMethod<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169>`(const std::function<void(double lower_bound, double upper_bound, std::vector<std::shared_ptr<:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`>>&populations, std::vector<std::pair<unsigned, unsigned>>&shaken)>& func);
		void :ref:`setStoppingCriteria<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1adee8fc8410a56e71b2af84ed6f4f2a7c>`(const std::function<bool(const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`&)>& stopping_criteria);
		void :ref:`addNewSolutionObserver<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa6cf3aca1879ffd4dc0c986340163254>`(const std::function<bool(const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`&)>& func);

		:ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>` :ref:`run<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>`(
			const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params,
			std::ostream* logger = &std::cout
		);

		void :ref:`evolve<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>`(unsigned generations = 1);

		:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` :ref:`pathRelink<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>`(
			:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type,
			:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection,
			std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
			unsigned number_pairs,
			double minimum_distance,
			std::size_t block_size = 1,
			std::chrono::seconds max_time = std::chrono::seconds{0},
			double percentage = 1.0
		);

		:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` :ref:`pathRelink<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ae9c15595bc46c3554b3eb5656ab08a53>`(
			std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
			std::chrono::seconds max_time = std::chrono::seconds{0}
		);

		void :ref:`exchangeElite<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>`(unsigned num_immigrants);
		void :ref:`reset<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>`();

		void :ref:`shake<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>`(
			unsigned intensity,
			:ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` shaking_type,
			unsigned population_index = std::numeric_limits<unsigned>::infinity()
		);

		void :ref:`injectChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>`(
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& chromosome,
			unsigned population_index,
			unsigned position
		);

		const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& :ref:`getCurrentPopulation<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a878b5edaec7438669ceec2e5fdd42d43>`(unsigned population_index = 0) const;
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& :ref:`getBestChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>`() const;
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`getBestFitness<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39>`() const;

		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f>`(
			unsigned population_index,
			unsigned position
		) const;

		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`getFitness<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a9d6636a50f519bf0f1e85257282c6065>`(
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

2023



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

	double decode(:ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& chromosome, bool rewrite);

where :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>` is a ``vector<double>`` (in general) representing a solution and ``rewrite`` is a boolean indicating that if the decode should rewrite the chromosome in case it implements local searches and modifies the initial solution decoded from the chromosome. Since this API has the capability of decoding several chromosomes in parallel, the user must guarantee that ``Decoder::decode(...)`` is thread-safe. Therefore, we do recommend to have the writable variables per thread. Please, see the example that follows this code.





.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ipr:

Implicit Path Relinking
-----------------------

This API also implements the Implicit Path Relinking leveraging the decoder capabilities. To perform the path relinking, the user must call :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>` method, indicating the type of path relinking (direct or permutation-based, see :ref:`PathRelinking::Type <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>`), the selection criteria (best solution or random elite, see :ref:`PathRelinking::Selection <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`), the distance function (to choose individuals far enough, see :ref:`BRKGA::DistanceFunctionBase <doxid-class_b_r_k_g_a_1_1_distance_function_base>`, :ref:`BRKGA::HammingDistance <doxid-class_b_r_k_g_a_1_1_hamming_distance>`, and :ref:`BRKGA::KendallTauDistance <doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`), a maximum time or a maximum path size.

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

This API allows the user provides a set of initial solutions to warm start the algorithm. In general, initial solutions are created using other (fast) heuristics and help the convergence of the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`. To do that, the user must encode the solutions on :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>` (``vector<double>``) and pass to the method ``:ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>``` as a ``vector<#Chromosome>``.







.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1gen_usage:

General Usage
~~~~~~~~~~~~~

#. The user must call the :ref:`BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>` constructor and pass the desired parameters. Please, see :ref:`BRKGA_MP_IPR::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a041a64b7b5a81b563fd54cfbeec1bb96>` for parameter details;

#. (Optional) The user provides the warm start solutions using ``:ref:`setInitialPopulation() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```;

#. (Optional) The user provides a callback to track the optimization using ``:ref:`addNewSolutionObserver() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa6cf3aca1879ffd4dc0c986340163254>```;

#. (Optional) The user provides custom stopping criteria function using ``:ref:`setStoppingCriteria() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1adee8fc8410a56e71b2af84ed6f4f2a7c>```;

#. (Optional) The user provides a custom shaking procedure using ``:ref:`setShakingMethod() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169>```;

#. The user calls ``:ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>``` to cary out the optimization; This method returns a ``:ref:`AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>``` object with the results of the optimization.

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

	void setInitialPopulation(const std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`>& chromosomes)

Sets individuals to initial population.

Set initial individuals into the population to work as warm-starters. Such individuals can be obtained from solutions of external procedures such as fast heuristics, other methaheuristics, or even relaxations from a mixed integer programming model that models the problem.

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

It must be a **positive non-increasing function**, i.e. :math:`f: \mathbb{N}^+ \to \mathbb{R}^+` such that :math:`f(i) \ge 0` and :math:`f(i) \ge f(i+1)` for :math:`i \in [1, \ldots, total\_parents]`. For example

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

.. index:: pair: function; setShakingMethod
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void setShakingMethod(const std::function<void(double lower_bound, double upper_bound, std::vector<std::shared_ptr<:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`>>&populations, std::vector<std::pair<unsigned, unsigned>>&shaken)>& func)

Sets a custom shaking procedure.

For more details, see :ref:`BrkgaParams::custom_shaking <doxid-class_b_r_k_g_a_1_1_brkga_params_1a4a5db47283d5f03e0605058338476337>`.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- func

		-
		  a callback function. For example, the code below implements the standard mutation:

		  .. ref-code-block:: cpp

		  	// A random number generator.
		  	std::mt19937 rng(2700001);
		  	rng.discard(rng.state_size);

		  	// Change some values from elite chromosomes from all populations.
		  	// Similar to a standard mutation.
		  	algorithm.setShakingMethod(
		  	    [&](double lower_bound, double upper_bound,
		  	        std::vector<std::shared_ptr<Population>>& populations,
		  	        std::vector<std::pair<unsigned, unsigned>>& shaken) {

		  	        // Determines whether we change the allele or not.
		  	        std::bernoulli_distribution must_change(0.50);

		  	        // Determines the value of the allele.
		  	        std::uniform_real_distribution<> allele_value(lower_bound, upper_bound);

		  	        for(unsigned pop_idx = 0; pop_idx < populations.size(); ++pop_idx) {
		  	            auto& population = populations[0]->population;
		  	            for(unsigned chr_idx = 0; chr_idx < population.size(); ++chr_idx) {
		  	                auto& chromosome = population[chr_idx];

		  	                bool change = false;
		  	                for(unsigned i = 0; i < chromosome.size(); ++i) {
		  	                    if(must_change(rng)) {
		  	                        chromosome[i] = allele_value(rng);
		  	                        change = true;
		  	                    }
		  	                }

		  	                if(change)
		  	                    shaken.push_back({pop_idx, chr_idx});
		  	            }
		  	        }
		  	    };
		  	);

.. index:: pair: function; setStoppingCriteria
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1adee8fc8410a56e71b2af84ed6f4f2a7c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void setStoppingCriteria(const std::function<bool(const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`&)>& stopping_criteria)

Sets a custom stopping criteria supplied by the user.

The algorithm always test for *the maximum running time* and for *the maximum stalled iterations/generations* given by ``:ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>``` indenpendently of the stopping criteria function supplied by the user. This is especially important when activating the implicit path reliking which is **very timing consuming**.

.. warning::

	If you are using IPR, we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** since this is the core stopping criteria on IPR.

If you really mean to have no maximum time and/or maximum stalled iterations set, we recommend to use the following code:

.. ref-code-block:: cpp

	// After reading your parameters, e.g.,
	// auto [brkga_params, control_params] = readConfiguration("config.conf");

	// You can set to the max.
	control_params.maximum_running_time = std::chrono::seconds::max();

	control_params.stall_offset = numeric_limits<unsigned>::max();



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- stopping_criteria

		-
		  a callback function to determine is the algorithm must stop. For instance, the following lambda function tests if the best solution reached a given value for a minimization problem:

		  .. ref-code-block:: cpp

		  	:ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` my_magical_solution = 10;

		  	algorithm.setStoppingCriteria(
		  	    [&](const AlgorithmStatus& status) {
		  	        return status.best_fitness <= my_magical_solution;
		  	    }
		  	);

.. index:: pair: function; addNewSolutionObserver
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa6cf3aca1879ffd4dc0c986340163254:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void addNewSolutionObserver(const std::function<bool(const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`&)>& func)

Adds a callback function called when the best solution is improved.

It must take a reference to :ref:`AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>` and return ``true`` if the algorithm should stop immediately. You may have as many observers as you want. They will be called in the order they are added.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- func

		-
		  a callback function such as

		  .. ref-code-block:: cpp

		  	bool check_solution(const AlgorithmStatus& status) {
		  	    std::cout << "\n" << status.best_fitness;
		  	    return true; // Stop the optimization.
		  	}
		  	//...
		  	algorithm.addNewSolutionObserver(check_solution);



		  or a lambda function such as



		  .. ref-code-block:: cpp

		  	algorithm.addNewSolutionObserver(
		  	    [](const AlgorithmStatus& status) {
		  	        std::cout
		  	        << "> Iter: " << status.current_iteration
		  	        << " | solution: " << status.best_fitness
		  	        << " | time: " << status.current_time
		  	        << std::endl;
		  	        return false; // Dont' stop the optimization.
		  	     }
		  	);

.. index:: pair: function; run
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>` run(
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params,
		std::ostream* logger = &std::cout
	)

Runs the full framework performing evolution, path-reliking, exchanges, shakes, and resets according to the parameters.

This method uses all facilities associated with this BRKGA-MP-IPR library, providing a comprehensive and easy-to-use single-entry point. The main loop always evolves one generation per iteration and calls other procedures based on the number of stalled iterations (i.e., the number of iterations without improvement in the best solution), and the given user thresholds in Control Parameters. *If the thresholds are all the same*, the main loop should be like this:

.. ref-code-block:: cpp

	while(!must_stop) {
	    :ref:`evolve <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>`(); // One generation.
	    if(best solution improvement) {
	        Save best solution;
	        Call observer callbacks;
	    }

	    if(!must_stop && ipr_interval > 0 && stalled_iterations > 0 &&
	       stalled_iterations % ipr_interval == 0) {
	         :ref:`pathRelink <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>`();
	         if(best solution improvement) {
	             Save best solution;
	             Call observer callbacks;
	         }
	     }

	    if(!must_stop && exchange_interval > 0 && stalled_iterations > 0 &&
	       stalled_iterations % exchange_interval == 0) {
	         :ref:`exchangeElite <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>`();
	    }

	    if(!must_stop && shake_interval > 0 && stalled_iterations > 0 &&
	       stalled_iterations % shake_interval == 0) {
	         :ref:`shake <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>`();
	    }

	    if(!must_stop && reset_interval > 0 && stalled_iterations > 0 &&
	       stalled_iterations % reset_interval == 0) {
	         :ref:`reset <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>`();
	    }
	}

Therefore, note that the order that :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>`, :ref:`exchangeElite() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>`, :ref:`shake() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>`, and :ref:`reset() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>` are called, depends on the thresholds defined in :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>`.

For path relinking, the block size is computed by :math:`\lceil \alpha \times \sqrt{p} \rceil` where :math:`\alpha` is :ref:`BrkgaParams::alpha_block_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>` and :math:`p` is :ref:`BrkgaParams::population_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>`. If the size is larger than the chromosome size, the size is set to half of the chromosome size. For more details on that, refer to :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>`.

.. note::

	The algorithm always test against maximum running time and for the maximum stalled iterations/generations given by :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>` indenpendently of the stopping criteria function supplied by the user. This is especially important when activating the implicit path reliking which is **very timing consuming**. If you are using IPR, we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** since this is the core stopping criteria on IPR.



.. warning::

	The decoding is done in parallel using threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If such property cannot be held, we suggest using a single thread for optimization.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- control_params

		- the parameters to control the algorithm flow, such as calling exchanges, shakes, and IPR.

	*
		- logger

		- a output stream to log some information.

	*
		- std::runtime_error

		-
		  in the following cases:

		  #. IPR is active (ipr_interva > 0) but the distance function is not set;

		  #. Shaking is active (shake_interval > 0) and it is set as 'CUSTOM'. However the custom shaking procedure was not supplied.

		  #. Shaking is active (shake_interval > 0). However, the intensity bounds are out of range. Should be (0.0, 1.0] and 'shaking_intensity_lower_bound <= shaking_intensity_upper_bound'.



.. rubric:: Returns:

The last algorithm status before the stopping criteria are met.

.. index:: pair: function; evolve
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void evolve(unsigned generations = 1)

Evolves the current populations following the guidelines of Multi-parent BRKGAs.

.. warning::

	The decoding is done in parallel using threads, and the user **must guarantee that the decoder is THREAD-SAFE.** If such property cannot be held, we suggest using a single thread for optimization.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- generations

		- number of generations to be evolved. Must be larger than zero.

	*
		- std::range_error

		- if the number of generations is zero.

.. index:: pair: function; pathRelink
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` pathRelink(
		:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type,
		:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection,
		std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
		unsigned number_pairs,
		double minimum_distance,
		std::size_t block_size = 1,
		std::chrono::seconds max_time = std::chrono::seconds{0},
		double percentage = 1.0
	)

Performs path relinking between elite solutions that are, at least, a given minimum distance between themselves. In this method, the local/loaded parameters are ignored in favor to the supplied ones.

In the presence of multiple populations, the path relinking is performed between elite chromosomes from different populations, in a circular fashion. For example, suppose we have 3 populations. The framework performs 3 path relinkings: the first between individuals from populations 1 and 2, the second between populations 2 and 3, and the third between populations 3 and 1. In the case of just one population, both base and guiding individuals are sampled from the elite set of that population.

Note that the algorithm tries to find a pair of base and guiding solutions with a minimum distance given by the distance function. If this is not possible, a new pair of solutions are sampled (without replacement) and tested against the distance. In case it is not possible to find such pairs for the given populations, the algorithm skips to the next pair of populations (in a circular fashion, as described above). Yet, if such pairs are not found in any case, the algorithm declares failure. This indicates that the populations are very homogeneous.

If the found solution is the best solution found so far, IPR replaces the worst solution by it. Otherwise, IPR computes the distance between the found solution and all other solutions in the elite set, and replaces the worst solution by it if and only if the found solution is, at least, ``minimum_distance`` from all them.

The API will call ``Decoder::decode()`` function always with ``rewrite = false``. The reason is that if the decoder rewrites the chromosome, the path between solutions is lost and inadvertent results may come up. Note that at the end of the path relinking, the method calls the decoder with ``rewrite = true`` in the best chromosome found to guarantee that this chromosome is re-written to reflect the best solution found.

This method is a multi-thread implementation. Instead of to build and decode each chromosome one at a time, the method builds a list of candidates, altering the alleles/keys according to the guide solution, and then decode all candidates in parallel. Note that ``O(chromosome_size^2 / block_size)`` additional memory is necessary to build the candidates, which can be costly if the ``chromosome_size`` is very large.

.. warning::

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

		- between two chromosomes computed by ``dist``.

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
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ae9c15595bc46c3554b3eb5656ab08a53:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` pathRelink(
		std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> dist,
		std::chrono::seconds max_time = std::chrono::seconds{0}
	)

Performs path relinking between elite solutions that are, at least, a given minimum distance between themselves.

This method uses all parameters supplied in the constructor. In particular, the block size is computed by :math:`\lceil \alpha \times \sqrt{p} \rceil` where :math:`\alpha` is :ref:`BrkgaParams::alpha_block_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>` and :math:`p` is :ref:`BrkgaParams::population_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>`. If the size is larger than the chromosome size, the size is set to half of the chromosome size.

Please, refer to :ref:`pathRelink() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>` for details.



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
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& chromosome,
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

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& getBestChromosome() const

Returns a reference to the chromosome with best fitness among all current populations.

.. warning::

	Note that this method **does not** return the best solution but the one within the current population. If a :ref:`shake() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>` or :ref:`reset() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>` is called, the best solution may be lose in the populations. However, if you are using :ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>`, the best solution is returned by that method. If not, you must keep track of the best solution.

.. index:: pair: function; getBestFitness
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` getBestFitness() const

Returns the best fitness among all current populations.

.. warning::

	Note that this method **does not** return the best fitness but the one within the current population. If a :ref:`shake() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>` or :ref:`reset() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>` is called, the best fitness may be lose in the populations. However, if you are using :ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>`, the best fitness is returned by that method. If not, you must keep track of the best fitness.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& getChromosome(
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

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` getFitness(
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

