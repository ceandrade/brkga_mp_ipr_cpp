.. index:: pair: class; BRKGA::DistanceFunctionBase
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base:
.. _cid-brkga.distancefunctionbase:

class BRKGA::DistanceFunctionBase
=================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Distance Function Base. :ref:`More...<details-doxid-class_b_r_k_g_a_1_1_distance_function_base>`











.. index:: pair: function; ~DistanceFunctionBase
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1a61acb94a73fd16ffa7740f2a383173d1:
.. _cid-brkga.distancefunctionbase.~distancefunctionbase:

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class DistanceFunctionBase
	{
	public:
		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_distance_function_base_1acee8e7b478010aff23b3a46cd113860a>`(const std::vector<double>& v1, const std::vector<double>& v2) = 0;
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1a243c8a1031fb149c6abc16f3869f2ace>`(const double key1, const double key2) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1aca0fc7103888cd025a67a16575954875>`(
			std::vector<double>::const_iterator v1_begin,
			std::vector<double>::const_iterator v2_begin,
			const std::size_t block_size
			) = 0;
	};

	// direct descendants

	class :ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance>`;
	class :ref:`KendallTauDistance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`;
.. _details-doxid-class_b_r_k_g_a_1_1_distance_function_base:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Distance Function Base.

This class is a interface for functors that compute the distance between two vectors of double numbers.

Methods
-------

.. index:: pair: function; distance
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1acee8e7b478010aff23b3a46cd113860a:
.. _cid-brkga.distancefunctionbase.distance:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual double distance(
		const std::vector<double>& v1,
		const std::vector<double>& v2
		) = 0

Computes the distance between two vectors.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- v1

		- first vector

	*
		- v2

		- second vector

.. index:: pair: function; affectSolution
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1a243c8a1031fb149c6abc16f3869f2ace:
.. _cid-brkga.distancefunctionbase.affectsolution:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual bool affectSolution(const double key1, const double key2) = 0

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
.. _doxid-class_b_r_k_g_a_1_1_distance_function_base_1aca0fc7103888cd025a67a16575954875:
.. _cid-brkga.distancefunctionbase.affectsolution-2:

.. ref-code-block:: cpp
	:class: title-code-block

	virtual bool affectSolution(
		std::vector<double>::const_iterator v1_begin,
		std::vector<double>::const_iterator v2_begin,
		const std::size_t block_size
		) = 0

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

