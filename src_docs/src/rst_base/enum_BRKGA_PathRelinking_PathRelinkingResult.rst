.. index:: pair: enum; PathRelinkingResult
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253:

enum BRKGA::PathRelinking::PathRelinkingResult
==============================================

Overview
~~~~~~~~

Specifies the result type/status of path relink procedure. :ref:`More...<details-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum PathRelinkingResult {
	    :ref:`TOO_HOMOGENEOUS<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253afc79eaa94186dcf0eda5c1df7bd87001>`   = 0,
	    :ref:`NO_IMPROVEMENT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae7339868191fd122c45d9abb0dcb87e9>`    = 1,
	    :ref:`ELITE_IMPROVEMENT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae523f249a5e460a70f2ae8ac7d7a959b>` = 3,
	    :ref:`BEST_IMPROVEMENT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253a44ea02fc8a02805c8ddfe8d37d101a39>`  = 7,
	};

.. _details-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies the result type/status of path relink procedure.

Enum Values
-----------

.. index:: pair: enumvalue; TOO_HOMOGENEOUS
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253afc79eaa94186dcf0eda5c1df7bd87001:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	TOO_HOMOGENEOUS

The chromosomes among the populations are too homogeneous and the path relink will not generate improveded solutions.

.. index:: pair: enumvalue; NO_IMPROVEMENT
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae7339868191fd122c45d9abb0dcb87e9:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	NO_IMPROVEMENT

Path relink was done but no improveded solution was found.

.. index:: pair: enumvalue; ELITE_IMPROVEMENT
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae523f249a5e460a70f2ae8ac7d7a959b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	ELITE_IMPROVEMENT

An improved solution among the elite set was found, but the best solution was not improved.

.. index:: pair: enumvalue; BEST_IMPROVEMENT
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253a44ea02fc8a02805c8ddfe8d37d101a39:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	BEST_IMPROVEMENT

The best solution was improved.

