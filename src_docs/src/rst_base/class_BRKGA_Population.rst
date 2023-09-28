.. index:: pair: class; BRKGA::Population
.. _doxid-class_b_r_k_g_a_1_1_population:

class BRKGA::Population
=======================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Encapsulates a population of chromosomes. :ref:`More...<details-class_b_r_k_g_a_1_1_population>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class Population {
	public:
		// fields
	
		std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`> :ref:`chromosomes<doxid-class_b_r_k_g_a_1_1_population_1a09d07b01903052413cd7a44a6056df8d>`;
		std::vector<std::pair<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`, unsigned>> :ref:`fitness<doxid-class_b_r_k_g_a_1_1_population_1a2c64d275b924bb8acdb4574f3dec98da>`;

		// construction
	
		:ref:`Population<doxid-class_b_r_k_g_a_1_1_population_1a19bd527e27e46bf18a1cfa3663f9f080>`(
			const unsigned chr_size,
			const unsigned pop_size
		);
	
		:ref:`Population<doxid-class_b_r_k_g_a_1_1_population_1a1d3bed8783d91b44c0c64c9283e4bf11>`(const Population& other);

		// methods
	
		Population& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_population_1a187d3a40748d877ca2df4e22f1f2b1aa>` (const Population&);
		unsigned :ref:`getChromosomeSize<doxid-class_b_r_k_g_a_1_1_population_1a856736e44609a0f652fcd4f19fbbf8e0>`() const;
		unsigned :ref:`getPopulationSize<doxid-class_b_r_k_g_a_1_1_population_1a0be8c63eeca28c5c43386d4b6a999375>`() const;
	
		double :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1a8994402e065ce2f327d565ce49dc320f>` (
			const unsigned chromosome,
			const unsigned allele
		) const;
	
		double& :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1a8dff4e162325b46658757fc9f6154c13>` (
			const unsigned chromosome,
			const unsigned allele
		);
	
		:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1a8deb224899de974e727a36435e864d21>` (unsigned chromosome);
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`getBestFitness<doxid-class_b_r_k_g_a_1_1_population_1a6a2fb67c7023720559c206495a1854fc>`() const;
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`getFitness<doxid-class_b_r_k_g_a_1_1_population_1a339ab8b3ee1573fe6ce7274e8f6ff730>`(const unsigned i) const;
		:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_population_1adce0928054001cba9b995176bd89fc81>`(unsigned i);
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_population_1ab3f9b7450892a43556e215f2714ae1a4>`(const unsigned i) const;
		void :ref:`sortFitness<doxid-class_b_r_k_g_a_1_1_population_1a76fb0dc9cde602acbbaa2897326c3d1b>`(const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense);
	
		void :ref:`setFitness<doxid-class_b_r_k_g_a_1_1_population_1a9d65efb0bf4271fd08b2315e3b7675bb>`(
			const unsigned chromosome,
			const :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` value
		);
	};
.. _details-class_b_r_k_g_a_1_1_population:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Encapsulates a population of chromosomes.

Encapsulates a population of chromosomes providing supporting methods for making the implementation easier.

J All methods and attributes are public and can be manipulated directly from :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` algorithms. Note that this class is not meant to be used externally of this unit.

Fields
------

.. index:: pair: variable; chromosomes
.. _doxid-class_b_r_k_g_a_1_1_population_1a09d07b01903052413cd7a44a6056df8d:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`> chromosomes

Chromosomes as vectors of probabilities.

.. index:: pair: variable; fitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a2c64d275b924bb8acdb4574f3dec98da:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::vector<std::pair<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`, unsigned>> fitness

Fitness of each chromosome.

Construction
------------

.. index:: pair: function; Population
.. _doxid-class_b_r_k_g_a_1_1_population_1a19bd527e27e46bf18a1cfa3663f9f080:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	Population(
		const unsigned chr_size,
		const unsigned pop_size
	)

Default constructor.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chr_size

		- size of chromosome.

	*
		- pop_size

		- size of population.

	*
		- std::range_error

		- if population size or chromosome size is zero.

.. index:: pair: function; Population
.. _doxid-class_b_r_k_g_a_1_1_population_1a1d3bed8783d91b44c0c64c9283e4bf11:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	Population(const Population& other)

Copy constructor.

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_population_1a187d3a40748d877ca2df4e22f1f2b1aa:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	Population& operator = (const Population&)

Assignment operator.

.. index:: pair: function; getChromosomeSize
.. _doxid-class_b_r_k_g_a_1_1_population_1a856736e44609a0f652fcd4f19fbbf8e0:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned getChromosomeSize() const

Returns the size of each chromosome.

.. index:: pair: function; getPopulationSize
.. _doxid-class_b_r_k_g_a_1_1_population_1a0be8c63eeca28c5c43386d4b6a999375:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned getPopulationSize() const

Returns the size of the population.

.. index:: pair: function; operator()
.. _doxid-class_b_r_k_g_a_1_1_population_1a8994402e065ce2f327d565ce49dc320f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double operator () (
		const unsigned chromosome,
		const unsigned allele
	) const

Returns a copy of an allele for a given chromosome.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of desired chromosome.

	*
		- allele

		- index of desired allele.



.. rubric:: Returns:

a copy of the allele value.

.. index:: pair: function; operator()
.. _doxid-class_b_r_k_g_a_1_1_population_1a8dff4e162325b46658757fc9f6154c13:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double& operator () (
		const unsigned chromosome,
		const unsigned allele
	)

Returns a reference for an allele for a given chromosome.

Usually used to set the allele value.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of desired chromosome.

	*
		- allele

		- index of desired allele.



.. rubric:: Returns:

a reference of the allele value.

.. index:: pair: function; operator()
.. _doxid-class_b_r_k_g_a_1_1_population_1a8deb224899de974e727a36435e864d21:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& operator () (unsigned chromosome)

Returns a reference to a chromosome.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of desired chromosome.



.. rubric:: Returns:

a reference to chromosome.

.. index:: pair: function; getBestFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a6a2fb67c7023720559c206495a1854fc:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` getBestFitness() const

Returns the best fitness in this population.

.. index:: pair: function; getFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a339ab8b3ee1573fe6ce7274e8f6ff730:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` getFitness(const unsigned i) const

Returns the fitness of chromosome i.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_population_1adce0928054001cba9b995176bd89fc81:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& getChromosome(unsigned i)

Returns a reference to the i-th best chromosome.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_population_1ab3f9b7450892a43556e215f2714ae1a4:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& getChromosome(const unsigned i) const

Returns a const reference to the i-th best chromosome.

.. index:: pair: function; sortFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a76fb0dc9cde602acbbaa2897326c3d1b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void sortFitness(const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense)

Sorts ``fitness`` by its first parameter according to the sense.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- sense

		- Optimization sense.

.. index:: pair: function; setFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a9d65efb0bf4271fd08b2315e3b7675bb:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	void setFitness(
		const unsigned chromosome,
		const :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` value
	)

Sets the fitness of chromosome.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of chromosome.

	*
		- value

		- allele value.

