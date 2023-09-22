.. index:: pair: class; BRKGA::BrkgaParams
.. _doxid-class_b_r_k_g_a_1_1_brkga_params:

class BRKGA::BrkgaParams
========================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Represents the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR hyper-parameters. :ref:`More...<details-class_b_r_k_g_a_1_1_brkga_params>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class BrkgaParams {
	public:
		// fields
	
		unsigned :ref:`population_size<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>`;
		double :ref:`elite_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571>`;
		double :ref:`mutants_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a29f0ff6ae4506e2e34ac7bfd633a802c>`;
		unsigned :ref:`num_elite_parents<doxid-class_b_r_k_g_a_1_1_brkga_params_1aec9a49fd5bb618908ccc8633c0244157>`;
		unsigned :ref:`total_parents<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8c214f59c688ba40baf86b4c4b4377be>`;
		:ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>` :ref:`bias_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1af20949ce2efbd51609e5fb6fcf465c2e>`;
		unsigned :ref:`num_independent_populations<doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926>`;
		unsigned :ref:`pr_number_pairs<doxid-class_b_r_k_g_a_1_1_brkga_params_1a80bb3ed52989c9daf9828c8214d85230>`;
		double :ref:`pr_minimum_distance<doxid-class_b_r_k_g_a_1_1_brkga_params_1aec6bb6fc8e77fa719b0522679e912231>`;
		:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` :ref:`pr_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1ab3cd1ab9a9f434869800bcad07251496>`;
		:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` :ref:`pr_selection<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8b5eba6b815e9559b296634457e65110>`;
		double :ref:`alpha_block_size<doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>`;
		double :ref:`pr_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a51bb97f66bde623ef8e8826eca78127b>`;

		// methods
	
		BrkgaParams& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_brkga_params_1a92e6be8770d83d9338806b892875b935>` (const BrkgaParams&);
	};
.. _details-class_b_r_k_g_a_1_1_brkga_params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Represents the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR hyper-parameters.

Fields
------

.. index:: pair: variable; population_size
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned population_size

Number of elements in the population.

.. index:: pair: variable; elite_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double elite_percentage

Percentage of individuals to become the elite set (0, 1].

.. index:: pair: variable; mutants_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a29f0ff6ae4506e2e34ac7bfd633a802c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double mutants_percentage

Percentage of mutants to be inserted in the population.

.. index:: pair: variable; num_elite_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec9a49fd5bb618908ccc8633c0244157:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_elite_parents

Number of elite parents for mating.

.. index:: pair: variable; total_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8c214f59c688ba40baf86b4c4b4377be:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned total_parents

Number of total parents for mating.

.. index:: pair: variable; bias_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1af20949ce2efbd51609e5fb6fcf465c2e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>` bias_type

Type of bias that will be used.

.. index:: pair: variable; num_independent_populations
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_independent_populations

Number of independent parallel populations.

.. index:: pair: variable; pr_number_pairs
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a80bb3ed52989c9daf9828c8214d85230:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned pr_number_pairs

Number of pairs of chromosomes to be tested to path relinking.

.. index:: pair: variable; pr_minimum_distance
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec6bb6fc8e77fa719b0522679e912231:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double pr_minimum_distance

Mininum distance between chromosomes selected to path-relinking.

.. index:: pair: variable; pr_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1ab3cd1ab9a9f434869800bcad07251496:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type

Path relinking type.

.. index:: pair: variable; pr_selection
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8b5eba6b815e9559b296634457e65110:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection

Individual selection to path-relinking.

.. index:: pair: variable; alpha_block_size
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double alpha_block_size

Defines the block size based on the size of the population.

.. index:: pair: variable; pr_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a51bb97f66bde623ef8e8826eca78127b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double pr_percentage

Percentage / path size to be computed. Value in (0, 1].

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a92e6be8770d83d9338806b892875b935:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	BrkgaParams& operator = (const BrkgaParams&)

Assignment operator for complaince.

