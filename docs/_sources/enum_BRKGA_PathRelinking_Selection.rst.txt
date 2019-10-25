.. index:: pair: enum; Selection
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa:
.. _cid-brkga.pathrelinking.selection:

enum BRKGA::PathRelinking::Selection
====================================

Overview
~~~~~~~~

Specifies which individuals used to build the path. :ref:`More...<details-doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum BRKGA::PathRelinking::Selection
	{
	    :ref:`BESTSOLUTION<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>`,
	    :ref:`RANDOMELITE<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa80e0b7674eebae1977705eed127c6ee8>`,
	};

.. _details-doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies which individuals used to build the path.

Enum Values
-----------

.. index:: pair: enumvalue; BESTSOLUTION
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39:
.. _cid-brkga.pathrelinking.selection.bestsolution:

.. ref-code-block:: cpp
	:class: title-code-block

	BESTSOLUTION

Selects, in the order, the best solution of each population.

.. index:: pair: enumvalue; RANDOMELITE
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa80e0b7674eebae1977705eed127c6ee8:
.. _cid-brkga.pathrelinking.selection.randomelite:

.. ref-code-block:: cpp
	:class: title-code-block

	RANDOMELITE

Chooses uniformly random solutions from the elite sets.

