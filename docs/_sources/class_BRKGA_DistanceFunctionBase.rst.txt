.. index:: pair: class; BRKGA::DistanceFunctionBase
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base:

class BRKGA::DistanceFunctionBase
=================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Distance Function Base. :ref:`More...<details-class_b_r_k_g_a_1_1_distance_function_base>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class DistanceFunctionBase {
	public:
		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_distance_function_base_1a97950c58509065dee6c352cbe4fb5b35>`(
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& v1,
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& v2
		) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1ac43fa85761c96a6a16c679781ee468fa>`(
			const Chromosome::value_type key1,
			const Chromosome::value_type key2
		) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1a02fe55b2288dbef73e51aa55d65a69cf>`(
			Chromosome::const_iterator v1_begin,
			Chromosome::const_iterator v2_begin,
			const std::size_t block_size
		) = 0;
	};

	// direct descendants

	class :ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance>`;
	class :ref:`KendallTauDistance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`;
.. _details-class_b_r_k_g_a_1_1_distance_function_base:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Distance Function Base.

This class is a interface for functors that compute the distance between two chromosomes.

Methods
-------

.. index:: pair: function; distance
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1a97950c58509065dee6c352cbe4fb5b35:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual double distance(
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& v1,
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& v2
	) = 0

Computes the distance between two vectors.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- v1

		- first chromosome.

	*
		- v2

		- second chromosome.

.. index:: pair: function; affectSolution
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1ac43fa85761c96a6a16c679781ee468fa:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		const Chromosome::value_type key1,
		const Chromosome::value_type key2
	) = 0

Returns true if the changing of ``key1`` by ``key2`` affects the solution.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- key1

		- the first key.

	*
		- key2

		- the second key.

.. index:: pair: function; affectSolution
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1a02fe55b2288dbef73e51aa55d65a69cf:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		Chromosome::const_iterator v1_begin,
		Chromosome::const_iterator v2_begin,
		const std::size_t block_size
	) = 0

Returns true if the changing of the blocks of keys ``v1`` by the blocks of keys ``v2`` affects the solution.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- v1_begin

		- begin of the first blocks of keys.

	*
		- v2_begin

		- begin of the first blocks of keys.

	*
		- block_size

		- number of keys to be considered.

