.. index:: pair: class; BRKGA::BrkgaParams
.. _doxid-class_b_r_k_g_a_1_1_brkga_params:
.. _cid-brkga.brkgaparams:

class BRKGA::BrkgaParams
========================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Represents the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR hyper-parameters. :ref:`More...<details-doxid-class_b_r_k_g_a_1_1_brkga_params>`











.. index:: pair: function; ~BrkgaParams
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aef6cbb5c8fcc3252fd02860b396ec5b1:
.. _cid-brkga.brkgaparams.~brkgaparams:

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class BrkgaParams
	{
	public:
		// fields
	
		unsigned :ref:`population_size<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>`;
		double :ref:`elite_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571>`;
		unsigned :ref:`mutants_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a84964a7fc9667892e7078f1017542e23>`;
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
	
		:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_brkga_params_1a90e04fa686fdfc9c8d7c19ea71db4ba2>` (const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`&);
	};
.. _details-doxid-class_b_r_k_g_a_1_1_brkga_params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Represents the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR hyper-parameters.

Fields
------

.. index:: pair: variable; population_size
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571:
.. _cid-brkga.brkgaparams.population_size:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned population_size

Number of elements in the population.

.. index:: pair: variable; elite_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571:
.. _cid-brkga.brkgaparams.elite_percentage:

.. ref-code-block:: cpp
	:class: title-code-block

	double elite_percentage

Percentage of individuals to become the elite set (0, 1].

.. index:: pair: variable; mutants_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a84964a7fc9667892e7078f1017542e23:
.. _cid-brkga.brkgaparams.mutants_percentage:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned mutants_percentage

Percentage of mutants to be inserted in the population.

.. index:: pair: variable; num_elite_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec9a49fd5bb618908ccc8633c0244157:
.. _cid-brkga.brkgaparams.num_elite_parents:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned num_elite_parents

Number of elite parents for mating.

.. index:: pair: variable; total_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8c214f59c688ba40baf86b4c4b4377be:
.. _cid-brkga.brkgaparams.total_parents:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned total_parents

Number of total parents for mating.

.. index:: pair: variable; bias_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1af20949ce2efbd51609e5fb6fcf465c2e:
.. _cid-brkga.brkgaparams.bias_type:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>` bias_type

Type of bias that will be used.

.. index:: pair: variable; num_independent_populations
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926:
.. _cid-brkga.brkgaparams.num_independent_populations:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned num_independent_populations

Number of independent parallel populations.

.. index:: pair: variable; pr_number_pairs
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a80bb3ed52989c9daf9828c8214d85230:
.. _cid-brkga.brkgaparams.pr_number_pairs:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned pr_number_pairs

Number of pairs of chromosomes to be tested to path relinking.

.. index:: pair: variable; pr_minimum_distance
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec6bb6fc8e77fa719b0522679e912231:
.. _cid-brkga.brkgaparams.pr_minimum_distance:

.. ref-code-block:: cpp
	:class: title-code-block

	double pr_minimum_distance

Mininum distance between chromosomes selected to path-relinking.

.. index:: pair: variable; pr_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1ab3cd1ab9a9f434869800bcad07251496:
.. _cid-brkga.brkgaparams.pr_type:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type

Path relinking type.

.. index:: pair: variable; pr_selection
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8b5eba6b815e9559b296634457e65110:
.. _cid-brkga.brkgaparams.pr_selection:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection

Individual selection to path-relinking.

.. index:: pair: variable; alpha_block_size
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d:
.. _cid-brkga.brkgaparams.alpha_block_size:

.. ref-code-block:: cpp
	:class: title-code-block

	double alpha_block_size

Defines the block size based on the size of the population.

.. index:: pair: variable; pr_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a51bb97f66bde623ef8e8826eca78127b:
.. _cid-brkga.brkgaparams.pr_percentage:

.. ref-code-block:: cpp
	:class: title-code-block

	double pr_percentage

Percentage / path size to be computed. Value in (0, 1].

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a90e04fa686fdfc9c8d7c19ea71db4ba2:
.. _cid-brkga.brkgaparams.operator:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& operator = (const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`&)

Assignment operator for complaince.

