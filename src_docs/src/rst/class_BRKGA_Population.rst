.. index:: pair: class; BRKGA::Population
.. _doxid-class_b_r_k_g_a_1_1_population:
.. _cid-brkga.population:

class BRKGA::Population
=======================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Encapsulates a population of chromosomes. :ref:`More...<details-doxid-class_b_r_k_g_a_1_1_population>`











.. index:: pair: function; ~Population
.. _doxid-class_b_r_k_g_a_1_1_population_1a3d6fdd4cf00ab477910a6597655fafc6:
.. _cid-brkga.population.~population:

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class Population
	{
	public:
		// fields
	
		std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`> :ref:`population<doxid-class_b_r_k_g_a_1_1_population_1a6c7b2ab4336c57d4659f09ca0c324a64>`;
		std::vector<std::pair<double, unsigned>> :ref:`fitness<doxid-class_b_r_k_g_a_1_1_population_1aae21b41493146e6d64ee39973b168dda>`;

		// construction
	
		:ref:`Population<doxid-class_b_r_k_g_a_1_1_population_1a19bd527e27e46bf18a1cfa3663f9f080>`(const unsigned chr_size, const unsigned pop_size);
		:ref:`Population<doxid-class_b_r_k_g_a_1_1_population_1acad8426e87b3d2f7b1c62852f8dca67b>`(const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& other);

		// methods
	
		:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_population_1ac5f739b17b2bcf6db820f4bcd5a2fbcb>` (const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`&);
		unsigned :ref:`getChromosomeSize<doxid-class_b_r_k_g_a_1_1_population_1a856736e44609a0f652fcd4f19fbbf8e0>`() const;
		unsigned :ref:`getPopulationSize<doxid-class_b_r_k_g_a_1_1_population_1a0be8c63eeca28c5c43386d4b6a999375>`() const;
		double :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1a8994402e065ce2f327d565ce49dc320f>` (const unsigned chromosome, const unsigned allele) const;
		double& :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1a86d677ddf7dcbb89753c99222d97095a>` (const unsigned chromosome, const unsigned allele);
		:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& :ref:`operator ()<doxid-class_b_r_k_g_a_1_1_population_1ae1d8028f658ed1009b379650f3182c46>` (unsigned chromosome);
		double :ref:`getBestFitness<doxid-class_b_r_k_g_a_1_1_population_1a55cccc61fa2ca7f131dd0361ed6f9f0c>`() const;
		double :ref:`getFitness<doxid-class_b_r_k_g_a_1_1_population_1a9b0cdd9402ff3682e106717630175431>`(const unsigned i) const;
		:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_population_1a6ed0c28c9dd264620c8bd7cd356c6022>`(unsigned i);
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& :ref:`getChromosome<doxid-class_b_r_k_g_a_1_1_population_1a9df2d69c9fbf43309b0671d4047045df>`(const unsigned i) const;
		void :ref:`sortFitness<doxid-class_b_r_k_g_a_1_1_population_1a76fb0dc9cde602acbbaa2897326c3d1b>`(const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense);
		void :ref:`setFitness<doxid-class_b_r_k_g_a_1_1_population_1a56eb4c547d2580ab4fd152713149a5d8>`(const unsigned chromosome, const double value);
	};
.. _details-doxid-class_b_r_k_g_a_1_1_population:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Encapsulates a population of chromosomes.

Encapsulates a population of chromosomes providing supporting methods for making the implementation easier.

All methods and attributes are public and can be manipulated directly from :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` algorithms. Note that this class is not meant to be used externally of this unit.

Fields
------

.. index:: pair: variable; population
.. _doxid-class_b_r_k_g_a_1_1_population_1a6c7b2ab4336c57d4659f09ca0c324a64:
.. _cid-brkga.population.population:

.. ref-code-block:: cpp
	:class: title-code-block

	std::vector<:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`> population

:ref:`Population <doxid-class_b_r_k_g_a_1_1_population>` as vectors of probabilities.

.. index:: pair: variable; fitness
.. _doxid-class_b_r_k_g_a_1_1_population_1aae21b41493146e6d64ee39973b168dda:
.. _cid-brkga.population.fitness:

.. ref-code-block:: cpp
	:class: title-code-block

	std::vector<std::pair<double, unsigned>> fitness

Fitness (double) of a each chromosome.

Construction
------------

.. index:: pair: function; Population
.. _doxid-class_b_r_k_g_a_1_1_population_1a19bd527e27e46bf18a1cfa3663f9f080:
.. _cid-brkga.population.population-2:

.. ref-code-block:: cpp
	:class: title-code-block

	Population(const unsigned chr_size, const unsigned pop_size)

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
.. _doxid-class_b_r_k_g_a_1_1_population_1acad8426e87b3d2f7b1c62852f8dca67b:
.. _cid-brkga.population.population-3:

.. ref-code-block:: cpp
	:class: title-code-block

	Population(const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& other)

Copy constructor.

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_population_1ac5f739b17b2bcf6db820f4bcd5a2fbcb:
.. _cid-brkga.population.operator:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`& operator = (const :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`&)

Assignment operator for complaince.

.. index:: pair: function; getChromosomeSize
.. _doxid-class_b_r_k_g_a_1_1_population_1a856736e44609a0f652fcd4f19fbbf8e0:
.. _cid-brkga.population.getchromosomesize:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned getChromosomeSize() const

Return the size of each chromosome.

.. index:: pair: function; getPopulationSize
.. _doxid-class_b_r_k_g_a_1_1_population_1a0be8c63eeca28c5c43386d4b6a999375:
.. _cid-brkga.population.getpopulationsize:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned getPopulationSize() const

Return the size of the population.

.. index:: pair: function; operator()
.. _doxid-class_b_r_k_g_a_1_1_population_1a8994402e065ce2f327d565ce49dc320f:
.. _cid-brkga.population.operator-2:

.. ref-code-block:: cpp
	:class: title-code-block

	double operator () (const unsigned chromosome, const unsigned allele) const

Return a copy of an allele for a given chromosome.



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
.. _doxid-class_b_r_k_g_a_1_1_population_1a86d677ddf7dcbb89753c99222d97095a:
.. _cid-brkga.population.operator-3:

.. ref-code-block:: cpp
	:class: title-code-block

	double& operator () (const unsigned chromosome, const unsigned allele)

Return a reference for an allele for a given chromosome.

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
.. _doxid-class_b_r_k_g_a_1_1_population_1ae1d8028f658ed1009b379650f3182c46:
.. _cid-brkga.population.operator-4:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& operator () (unsigned chromosome)

Return a reference to a chromosome.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of desired chromosome.



.. rubric:: Returns:

a reference to chromosome.

.. index:: pair: function; getBestFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a55cccc61fa2ca7f131dd0361ed6f9f0c:
.. _cid-brkga.population.getbestfitness:

.. ref-code-block:: cpp
	:class: title-code-block

	double getBestFitness() const

Return the best fitness in this population.

.. index:: pair: function; getFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a9b0cdd9402ff3682e106717630175431:
.. _cid-brkga.population.getfitness:

.. ref-code-block:: cpp
	:class: title-code-block

	double getFitness(const unsigned i) const

Return the fitness of chromosome i.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_population_1a6ed0c28c9dd264620c8bd7cd356c6022:
.. _cid-brkga.population.getchromosome:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getChromosome(unsigned i)

Return a reference to the i-th best chromosome.

.. index:: pair: function; getChromosome
.. _doxid-class_b_r_k_g_a_1_1_population_1a9df2d69c9fbf43309b0671d4047045df:
.. _cid-brkga.population.getchromosome-2:

.. ref-code-block:: cpp
	:class: title-code-block

	const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`& getChromosome(const unsigned i) const

Return a const reference to the i-th best chromosome.

.. index:: pair: function; sortFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a76fb0dc9cde602acbbaa2897326c3d1b:
.. _cid-brkga.population.sortfitness:

.. ref-code-block:: cpp
	:class: title-code-block

	void sortFitness(const :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>` sense)

Sort ``fitness`` by its first parameter according to the sense.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- sense

		- Optimization sense.

.. index:: pair: function; setFitness
.. _doxid-class_b_r_k_g_a_1_1_population_1a56eb4c547d2580ab4fd152713149a5d8:
.. _cid-brkga.population.setfitness:

.. ref-code-block:: cpp
	:class: title-code-block

	void setFitness(const unsigned chromosome, const double value)

Set the fitness of chromosome.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- chromosome

		- index of chromosome.

	*
		- value

		- allele value.

