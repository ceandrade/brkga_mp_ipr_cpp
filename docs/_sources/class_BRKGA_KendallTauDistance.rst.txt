.. index:: pair: class; BRKGA::KendallTauDistance
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance:

class BRKGA::KendallTauDistance
===============================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Kendall Tau distance between two vectors. :ref:`More...<details-class_b_r_k_g_a_1_1_kendall_tau_distance>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class KendallTauDistance: public :ref:`BRKGA::DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>` {
	public:
		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a7c95831e7f477e7bbc4203fab5fdffb1>`(
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector1,
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector2
		);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a94c50ba4c6a2ef14d657b6579f2a8739>`(
			const Chromosome::value_type key1,
			const Chromosome::value_type key2
		);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a06d79321c14a511195daa4c1d143ed7e>`(
			Chromosome::const_iterator v1_begin,
			Chromosome::const_iterator v2_begin,
			const std::size_t block_size
		);
	};

Inherited Members
-----------------

.. ref-code-block:: cpp
	:class: doxyrest-overview-inherited-code-block

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

.. _details-class_b_r_k_g_a_1_1_kendall_tau_distance:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Kendall Tau distance between two vectors.

This class is a functor that computes the Kendall Tau distance between two vectors. This version is not normalized.

Methods
-------

.. index:: pair: function; distance
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a7c95831e7f477e7bbc4203fab5fdffb1:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual double distance(
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector1,
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector2
	)

Computes the Kendall Tau distance between two vectors.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- vector1

		- first vector

	*
		- vector2

		- second vector

.. index:: pair: function; affectSolution
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a94c50ba4c6a2ef14d657b6579f2a8739:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		const Chromosome::value_type key1,
		const Chromosome::value_type key2
	)

Returns true if the changing of ``key1`` by ``key2`` affects the solution.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- key1

		- the first key

	*
		- key2

		- the second key

.. index:: pair: function; affectSolution
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a06d79321c14a511195daa4c1d143ed7e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		Chromosome::const_iterator v1_begin,
		Chromosome::const_iterator v2_begin,
		const std::size_t block_size
	)

Returns true if the changing of the blocks of keys ``v1`` by the blocks of keys ``v2`` affects the solution.

Todo This is the same implementation of Hamming. We need to figure out how to do it properly.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- v1_begin

		- begin of the first blocks of keys

	*
		- v2_begin

		- begin of the first blocks of keys

	*
		- block_size

		- number of keys to be considered.

