.. index:: pair: class; BRKGA::HammingDistance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance:
.. _cid-brkga.hammingdistance:

class BRKGA::HammingDistance
============================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Hamming distance between two vectors. :ref:`More...<details-doxid-class_b_r_k_g_a_1_1_hamming_distance>`











.. index:: pair: function; ~HammingDistance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1a63f407a819c88ca7d9469c13850c5ebd:
.. _cid-brkga.hammingdistance.~hammingdistance:

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class HammingDistance: public :ref:`BRKGA::DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`
	{
	public:
		// fields
	
		double :ref:`threshold<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae4f83a7d514e75c3abf3d47663c1b9d9>`;

		// construction
	
		:ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ac4a9b6c41241556c35c08535050bea53>`(const double _threshold = 0.5);

		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_hamming_distance_1ade9120b27adf3834a36406c508cb6ee9>`(
			const std::vector<double>& vector1,
			const std::vector<double>& vector2
			);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_hamming_distance_1abd15c245ad7be970116330e4f91c3c0f>`(const double key1, const double key2);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_hamming_distance_1a4df949516a669e17a5b7a6550dd9cbca>`(
			std::vector<double>::const_iterator v1_begin,
			std::vector<double>::const_iterator v2_begin,
			const std::size_t block_size
			);
	};

Inherited Members
-----------------

.. ref-code-block:: cpp
	:class: overview-inherited-code-block

	public:
		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_distance_function_base_1acee8e7b478010aff23b3a46cd113860a>`(const std::vector<double>& v1, const std::vector<double>& v2) = 0;
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1a243c8a1031fb149c6abc16f3869f2ace>`(const double key1, const double key2) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1aca0fc7103888cd025a67a16575954875>`(
			std::vector<double>::const_iterator v1_begin,
			std::vector<double>::const_iterator v2_begin,
			const std::size_t block_size
			) = 0;

.. _details-doxid-class_b_r_k_g_a_1_1_hamming_distance:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Hamming distance between two vectors.

This class is a functor that computes the Hamming distance between two vectors. It takes a threshold parameter to "binarize" the vectors.

Fields
------

.. index:: pair: variable; threshold
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ae4f83a7d514e75c3abf3d47663c1b9d9:
.. _cid-brkga.hammingdistance.threshold:

.. ref-code-block:: cpp
	:class: title-code-block

	double threshold

Threshold parameter used to rounding the values to 0 or 1.

Construction
------------

.. index:: pair: function; HammingDistance
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ac4a9b6c41241556c35c08535050bea53:
.. _cid-brkga.hammingdistance.hammingdistance:

.. ref-code-block:: cpp
	:class: title-code-block

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
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1ade9120b27adf3834a36406c508cb6ee9:
.. _cid-brkga.hammingdistance.distance:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual double distance(
		const std::vector<double>& vector1,
		const std::vector<double>& vector2
		)

Compute the Hamming distance between two vectors.



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
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1abd15c245ad7be970116330e4f91c3c0f:
.. _cid-brkga.hammingdistance.affectsolution:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual bool affectSolution(const double key1, const double key2)

Return true if the changing of ``key1`` by ``key2`` affects the solution.



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
.. _doxid-class_b_r_k_g_a_1_1_hamming_distance_1a4df949516a669e17a5b7a6550dd9cbca:
.. _cid-brkga.hammingdistance.affectsolution-2:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual bool affectSolution(
		std::vector<double>::const_iterator v1_begin,
		std::vector<double>::const_iterator v2_begin,
		const std::size_t block_size
		)

Return true if the changing of the blocks of keys ``v1`` by the blocks of keys ``v2`` affects the solution.



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

