.. index:: pair: enum; Selection
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa:

enum BRKGA::PathRelinking::Selection
====================================

Overview
~~~~~~~~

Specifies which individuals used to build the path. :ref:`More...<details-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum Selection {
	    :ref:`BESTSOLUTION<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`,
	    :ref:`RANDOMELITE<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa80e0b7674eebae1977705eed127c6ee8>`,
	};

.. _details-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies which individuals used to build the path.

Enum Values
-----------

.. index:: pair: enumvalue; BESTSOLUTION
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	BESTSOLUTION

Selects, in the order, the best solution of each population.

.. index:: pair: enumvalue; RANDOMELITE
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa80e0b7674eebae1977705eed127c6ee8:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	RANDOMELITE

Chooses uniformly random solutions from the elite sets.

