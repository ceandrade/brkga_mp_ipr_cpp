.. index:: pair: class; BRKGA::AlgorithmStatus
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status:

class BRKGA::AlgorithmStatus
============================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Defines the current status of the algorithm for a given :ref:`BRKGA_MP_IPR::run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>` call. :ref:`More...<details-class_b_r_k_g_a_1_1_algorithm_status>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class AlgorithmStatus {
	public:
		// fields
	
		:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`best_fitness<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a887c715ed54dff66352eb0a32fbe0f48>` {};
		:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>` :ref:`best_chromosome<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a995293c074aca45226d1ea7e5d76b452>` {};
		unsigned :ref:`current_iteration<doxid-class_b_r_k_g_a_1_1_algorithm_status_1aabb512737ee4afb79588e6aedc52804c>` {0};
		unsigned :ref:`last_update_iteration<doxid-class_b_r_k_g_a_1_1_algorithm_status_1ab4a57225aa33542913c06eba73ef8c6e>` {0};
		std::chrono::duration<double> :ref:`current_time<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a9e852356cac7e0fa545d270f478c94ac>` {0.0};
		std::chrono::duration<double> :ref:`last_update_time<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a344737b4912d545006f1ca34cf061870>` {0.0};
		unsigned :ref:`largest_iteration_offset<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a1bf7132617bf8da9fb3beaf0ccd243b1>` {0};
		unsigned :ref:`stalled_iterations<doxid-class_b_r_k_g_a_1_1_algorithm_status_1ae127fbc7999461babb5a969abf3514db>` {0};
		std::chrono::duration<double> :ref:`path_relink_time<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a7829b7fef2aa8ab57729d86869e2089b>` {0.0};
		unsigned :ref:`num_path_relink_calls<doxid-class_b_r_k_g_a_1_1_algorithm_status_1ab5ef744e5489535631226a576801487f>` {0};
		unsigned :ref:`num_homogenities<doxid-class_b_r_k_g_a_1_1_algorithm_status_1acd79069531a3d1acfb65f0800a05c453>` {0};
		unsigned :ref:`num_best_improvements<doxid-class_b_r_k_g_a_1_1_algorithm_status_1adabd42094c6879a6d08fde76777e25ce>` {0};
		unsigned :ref:`num_elite_improvements<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a2593e0b8c44afd2faa1a3f377123e342>` {0};
		unsigned :ref:`num_exchanges<doxid-class_b_r_k_g_a_1_1_algorithm_status_1abc49ffcc5bb59ad0c3522a1ac896dd21>` {0};
		unsigned :ref:`num_shakes<doxid-class_b_r_k_g_a_1_1_algorithm_status_1a7b3b54627bee59d53fc74b986a29100b>` {0};
		unsigned :ref:`num_resets<doxid-class_b_r_k_g_a_1_1_algorithm_status_1aa4e621bdbcac58515cf23d0a0174ca55>` {0};
	};
.. _details-class_b_r_k_g_a_1_1_algorithm_status:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Defines the current status of the algorithm for a given :ref:`BRKGA_MP_IPR::run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>` call.

.. note::

	We use ``std::chrono::duration<double>`` instead ``std::chrono::seconds`` for keep better precision.

Fields
------

.. index:: pair: variable; best_fitness
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a887c715ed54dff66352eb0a32fbe0f48:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` best_fitness {}

Current best fitness value.

.. index:: pair: variable; best_chromosome
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a995293c074aca45226d1ea7e5d76b452:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>` best_chromosome {}

A pointer to the current best chromosome.

.. index:: pair: variable; current_iteration
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1aabb512737ee4afb79588e6aedc52804c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned current_iteration {0}

The current algorithm iteration / generation.

.. index:: pair: variable; last_update_iteration
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1ab4a57225aa33542913c06eba73ef8c6e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned last_update_iteration {0}

The iteration of the last update.

.. index:: pair: variable; current_time
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a9e852356cac7e0fa545d270f478c94ac:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::chrono::duration<double> current_time {0.0}

The time of the last update (in seconds).

.. index:: pair: variable; last_update_time
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a344737b4912d545006f1ca34cf061870:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::chrono::duration<double> last_update_time {0.0}

The time of the last update (in seconds).

.. index:: pair: variable; largest_iteration_offset
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a1bf7132617bf8da9fb3beaf0ccd243b1:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned largest_iteration_offset {0}

The largest number of iterations between two updates.

.. index:: pair: variable; stalled_iterations
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1ae127fbc7999461babb5a969abf3514db:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned stalled_iterations {0}

Last number of iterations without improvement in the best solution.

.. index:: pair: variable; path_relink_time
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a7829b7fef2aa8ab57729d86869e2089b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::chrono::duration<double> path_relink_time {0.0}

Total time spent on path relinking so far (in seconds).

.. index:: pair: variable; num_path_relink_calls
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1ab5ef744e5489535631226a576801487f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_path_relink_calls {0}

Number of call to path relinking.

.. index:: pair: variable; num_homogenities
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1acd79069531a3d1acfb65f0800a05c453:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_homogenities {0}

Number of homogenities, i.e., number of times that the populations are too homogenious and the path reliking could not be performed.

.. index:: pair: variable; num_best_improvements
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1adabd42094c6879a6d08fde76777e25ce:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_best_improvements {0}

Number of improvements in the best solution by the path reliking.

.. index:: pair: variable; num_elite_improvements
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a2593e0b8c44afd2faa1a3f377123e342:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_elite_improvements {0}

Number of improvements in elite individuals, but not the best one, by path reliking.

.. index:: pair: variable; num_exchanges
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1abc49ffcc5bb59ad0c3522a1ac896dd21:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_exchanges {0}

Number of exchange between populations performed (not number of individuals).

.. index:: pair: variable; num_shakes
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1a7b3b54627bee59d53fc74b986a29100b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_shakes {0}

Number of shakes performed.

.. index:: pair: variable; num_resets
.. _doxid-class_b_r_k_g_a_1_1_algorithm_status_1aa4e621bdbcac58515cf23d0a0174ca55:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_resets {0}

Number of population resets performed.

