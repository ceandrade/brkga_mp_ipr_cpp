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
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1aeb8d4c9cd3b87b08f84762ed83d347ff>`(
			const std::vector<double>& vector1,
			const std::vector<double>& vector2
			);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a0901afc35a6f2a44427d0a1ce78b2daa>`(
			const double key1,
			const double key2
			);
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a4638274b38b50051eaba3f399b1b9c5f>`(
			std::vector<double>::const_iterator v1_begin,
			std::vector<double>::const_iterator v2_begin,
			const std::size_t block_size
			);
	};

Inherited Members
-----------------

.. ref-code-block:: cpp
	:class: doxyrest-overview-inherited-code-block

	public:
		// methods
	
		virtual double :ref:`distance<doxid-class_b_r_k_g_a_1_1_distance_function_base_1acee8e7b478010aff23b3a46cd113860a>`(
			const std::vector<double>& v1,
			const std::vector<double>& v2
			) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1a243c8a1031fb149c6abc16f3869f2ace>`(
			const double key1,
			const double key2
			) = 0;
	
		virtual bool :ref:`affectSolution<doxid-class_b_r_k_g_a_1_1_distance_function_base_1aca0fc7103888cd025a67a16575954875>`(
			std::vector<double>::const_iterator v1_begin,
			std::vector<double>::const_iterator v2_begin,
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
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1aeb8d4c9cd3b87b08f84762ed83d347ff:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual double distance(
		const std::vector<double>& vector1,
		const std::vector<double>& vector2
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
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a0901afc35a6f2a44427d0a1ce78b2daa:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		const double key1,
		const double key2
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
.. _doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a4638274b38b50051eaba3f399b1b9c5f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	virtual bool affectSolution(
		std::vector<double>::const_iterator v1_begin,
		std::vector<double>::const_iterator v2_begin,
		const std::size_t block_size
		)

Returns true if the changing of the blocks of keys ``v1`` by the blocks of keys ``v2`` affects the solution.

Todo (ceandrade): implement this properly.



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

