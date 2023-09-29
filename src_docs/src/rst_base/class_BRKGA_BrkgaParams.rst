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

		unsigned :ref:`population_size<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>` {0};
		double :ref:`elite_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571>` {0.0};
		double :ref:`mutants_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a29f0ff6ae4506e2e34ac7bfd633a802c>` {0.0};
		unsigned :ref:`num_elite_parents<doxid-class_b_r_k_g_a_1_1_brkga_params_1aec9a49fd5bb618908ccc8633c0244157>` {0};
		unsigned :ref:`total_parents<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8c214f59c688ba40baf86b4c4b4377be>` {0};
		:ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>` :ref:`bias_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1af20949ce2efbd51609e5fb6fcf465c2e>` {:ref:`BiasFunctionType::CONSTANT<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a8d6b5cada83510220f59e00ce86d4d92>`};
		unsigned :ref:`num_independent_populations<doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926>` {0};
		unsigned :ref:`pr_number_pairs<doxid-class_b_r_k_g_a_1_1_brkga_params_1a80bb3ed52989c9daf9828c8214d85230>` {0};
		double :ref:`pr_minimum_distance<doxid-class_b_r_k_g_a_1_1_brkga_params_1aec6bb6fc8e77fa719b0522679e912231>` {0.0};
		:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` :ref:`pr_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1ab3cd1ab9a9f434869800bcad07251496>` {:ref:`PathRelinking::Type::DIRECT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76>`};
		:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` :ref:`pr_selection<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8b5eba6b815e9559b296634457e65110>` {:ref:`PathRelinking::Selection::BESTSOLUTION<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`};
		:ref:`PathRelinking::DistanceFunctionType<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941>` :ref:`pr_distance_function_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1aeca229f1073ebe11184634fb9e8378b9>` {:ref:`PathRelinking::DistanceFunctionType::CUSTOM<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a72baef04098f035e8a320b03ad197818>`};
		std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> :ref:`pr_distance_function<doxid-class_b_r_k_g_a_1_1_brkga_params_1af7eec34bf90551ae4553ac031d5dba3c>` {};
		double :ref:`alpha_block_size<doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>` {0.0};
		double :ref:`pr_percentage<doxid-class_b_r_k_g_a_1_1_brkga_params_1a51bb97f66bde623ef8e8826eca78127b>` {0.0};
		unsigned :ref:`num_exchange_individuals<doxid-class_b_r_k_g_a_1_1_brkga_params_1a67743b073653dbeec7a74b055ab2a14c>` {0};
		:ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` :ref:`shaking_type<doxid-class_b_r_k_g_a_1_1_brkga_params_1a29fbf8f57e03e98d6c71ce43751efb2d>` {:ref:`ShakingType::CHANGE<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073>`};
		double :ref:`shaking_intensity_lower_bound<doxid-class_b_r_k_g_a_1_1_brkga_params_1a38c571e712321de7b74429529e272d76>` {0.0};
		double :ref:`shaking_intensity_upper_bound<doxid-class_b_r_k_g_a_1_1_brkga_params_1a3339e258741604f33784e28f2c073759>` {0.0};
		std::function<void(double lower_bound, double upper_bound, std::vector<std::shared_ptr<:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`>>&populations, std::vector<std::pair<unsigned, unsigned>>&shaken)> :ref:`custom_shaking<doxid-class_b_r_k_g_a_1_1_brkga_params_1a4a5db47283d5f03e0605058338476337>` {};
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

	unsigned population_size {0}

Number of elements in the population (> 0).

.. index:: pair: variable; elite_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a3b9a181f00f8675ed88e09f0b9c34571:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double elite_percentage {0.0}

Percentage of individuals to become the elite set (0.0, 1.0].

.. index:: pair: variable; mutants_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a29f0ff6ae4506e2e34ac7bfd633a802c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double mutants_percentage {0.0}

Percentage of mutants to be inserted in the population (0.0, 1.0].

.. index:: pair: variable; num_elite_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec9a49fd5bb618908ccc8633c0244157:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_elite_parents {0}

Number of elite parents for mating (> 0).

.. index:: pair: variable; total_parents
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8c214f59c688ba40baf86b4c4b4377be:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned total_parents {0}

Number of total parents for mating (> 0).

.. index:: pair: variable; bias_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1af20949ce2efbd51609e5fb6fcf465c2e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>` bias_type {:ref:`BiasFunctionType::CONSTANT<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a8d6b5cada83510220f59e00ce86d4d92>`}

Type of bias that will be used.

.. index:: pair: variable; num_independent_populations
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_independent_populations {0}

Number of independent parallel populations (> 0).

.. index:: pair: variable; pr_number_pairs
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a80bb3ed52989c9daf9828c8214d85230:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned pr_number_pairs {0}

Number of pairs of chromosomes to be tested to path relinking (> 0).

.. index:: pair: variable; pr_minimum_distance
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aec6bb6fc8e77fa719b0522679e912231:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double pr_minimum_distance {0.0}

Mininum distance between chromosomes selected to path-relinking. Value range depends on the used distance function.

.. index:: pair: variable; pr_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1ab3cd1ab9a9f434869800bcad07251496:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>` pr_type {:ref:`PathRelinking::Type::DIRECT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76>`}

Path relinking type.

.. index:: pair: variable; pr_selection
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a8b5eba6b815e9559b296634457e65110:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>` pr_selection {:ref:`PathRelinking::Selection::BESTSOLUTION<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`}

Individual selection to path-relinking.

.. index:: pair: variable; pr_distance_function_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1aeca229f1073ebe11184634fb9e8378b9:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`PathRelinking::DistanceFunctionType<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941>` pr_distance_function_type {:ref:`PathRelinking::DistanceFunctionType::CUSTOM<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a72baef04098f035e8a320b03ad197818>`}

Type of the distance function used on path-relinking.

.. index:: pair: variable; pr_distance_function
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1af7eec34bf90551ae4553ac031d5dba3c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::shared_ptr<:ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`> pr_distance_function {}

The distance functor used on path-relinking.

.. index:: pair: variable; alpha_block_size
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double alpha_block_size {0.0}

Defines the block size based on the size of the population (> 0).

.. index:: pair: variable; pr_percentage
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a51bb97f66bde623ef8e8826eca78127b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double pr_percentage {0.0}

Percentage / path size to be computed (0.0, 1.0].

.. index:: pair: variable; num_exchange_individuals
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a67743b073653dbeec7a74b055ab2a14c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_exchange_individuals {0}

Number of elite chromosomes exchanged from each population (> 0).

.. index:: pair: variable; shaking_type
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a29fbf8f57e03e98d6c71ce43751efb2d:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>` shaking_type {:ref:`ShakingType::CHANGE<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073>`}

Type of the shaking procedure.

.. index:: pair: variable; shaking_intensity_lower_bound
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a38c571e712321de7b74429529e272d76:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double shaking_intensity_lower_bound {0.0}

Shaking intensity lower bound (0.0, 1.0].

For default CHANGE and SWAP procedures, this value is a percentage of the chromosome size. If ``shaking_intensity_lower_bound < shaking_intensity_upper_bound``, we uniformly draw a random intensity between these bounds on each shaking call. If ``shaking_intensity_lower_bound = shaking_intensity_upper_bound``, then a fixed intensity is always used. See :ref:`BrkgaParams::shaking_intensity_upper_bound <doxid-class_b_r_k_g_a_1_1_brkga_params_1a3339e258741604f33784e28f2c073759>`.

.. index:: pair: variable; shaking_intensity_upper_bound
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a3339e258741604f33784e28f2c073759:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	double shaking_intensity_upper_bound {0.0}

Shaking intensity upper bound (0.0, 1.0].

For default CHANGE and SWAP procedures, this value is a percentage of the chromosome size. If ``shaking_intensity_lower_bound < shaking_intensity_upper_bound``, we uniformly draw a random intensity between these bounds on each shaking call. If ``shaking_intensity_lower_bound = shaking_intensity_upper_bound``, then a fixed intensity is always used. See :ref:`BrkgaParams::shaking_intensity_lower_bound <doxid-class_b_r_k_g_a_1_1_brkga_params_1a38c571e712321de7b74429529e272d76>`.

.. index:: pair: variable; custom_shaking
.. _doxid-class_b_r_k_g_a_1_1_brkga_params_1a4a5db47283d5f03e0605058338476337:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::function<void(double lower_bound, double upper_bound, std::vector<std::shared_ptr<:ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`>>&populations, std::vector<std::pair<unsigned, unsigned>>&shaken)> custom_shaking {}

This is the custom shaking procedure given by the user.

* Parameters ``lower_bound`` and ``upper_bound`` is the shaking intensity bounds to be applied. Usually, the define a range where the intensity is sampled.

* Parameter ``populations`` are the current :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` populations.

* Parameter ``shaken`` is a list of ``<population index, chromosome index>`` pairs indicating which chromosomes were shaken on which population, so that they got re-decoded.

See :ref:`BRKGA_MP_IPR::setShakingMethod() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169>` for details and examples.

.. note::

	If ``shaken`` is empty, all chromosomes of all populations are re-decoded. This may be slow. Even if you intention is to do so, it is faster to populate ``shaken``.



.. warning::

	This procedure can be very intrusive since it must manipulate the population. So, the user must make sure that :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` invariants are kept, such as chromosome size and population size. Otherwise, the overall functionaly may be compromised.

