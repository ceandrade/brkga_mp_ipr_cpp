.. index:: pair: class; BRKGA::HammingDistance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance:

class BRKGA::HammingDistance
============================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Hamming distance between two vectors. :ref:`More...<details-class_b_r_k_g_a_1_1_hamming_distance>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class HammingDistance: public :ref:`BRKGA::DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>` {
	public:
		// fields
	
		double :ref:`threshold<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae4f83a7d514e75c3abf3d47663c1b9d9>` {0.5};

		// construction
	
		:ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ac4a9b6c41241556c35c08535050bea53>`(const double _threshold = 0.5);

		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_hamming_distance_1abeb4f21b0bc65d5d21bc9b752f682848>`(
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector1,
			const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector2
		);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae50ee3109e7c89e41a3e46b8bc66c51d>`(
			const Chromosome::value_type key1,
			const Chromosome::value_type key2
		);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_hamming_distance_1aceac2e7381d0454f0118372662fa155c>`(
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

.. _details-class_b_r_k_g_a_1_1_hamming_distance:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Hamming distance between two vectors.

This class is a functor that computes the Hamming distance between two vectors. It takes a threshold parameter to "binarize" the vectors.

Fields
------

.. index:: pair: variable; threshold
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae4f83a7d514e75c3abf3d47663c1b9d9:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double threshold {0.5}

Threshold parameter used to rounding the values to 0 or 1.

Construction
------------

.. index:: pair: function; HammingDistance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ac4a9b6c41241556c35c08535050bea53:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	HammingDistance(const double _threshold = 0.5)

Default constructor.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- _threshold

		- used to rounding the values to 0 or 1.

Methods
-------

.. index:: pair: function; distance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1abeb4f21b0bc65d5d21bc9b752f682848:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual double distance(
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector1,
		const :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`& vector2
	)

Computes the Hamming distance between two vectors.



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
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae50ee3109e7c89e41a3e46b8bc66c51d:

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
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1aceac2e7381d0454f0118372662fa155c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		Chromosome::const_iterator v1_begin,
		Chromosome::const_iterator v2_begin,
		const std::size_t block_size
	)

Returns true if the changing of the blocks of keys ``v1`` by the blocks of keys ``v2`` affects the solution.



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

