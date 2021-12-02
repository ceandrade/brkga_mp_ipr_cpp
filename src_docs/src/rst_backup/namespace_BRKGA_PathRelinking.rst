.. index:: pair: namespace; BRKGA::PathRelinking
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking:
.. _cid-brkga.pathrelinking:

namespace BRKGA::PathRelinking
==============================

.. toctree::
	:hidden:

	enum_BRKGA_PathRelinking_PathRelinkingResult.rst
	enum_BRKGA_PathRelinking_Selection.rst
	enum_BRKGA_PathRelinking_Type.rst

Overview
~~~~~~~~

Holds the enumerations for Path Relinking algorithms. :ref:`More...<details-doxid-namespace_b_r_k_g_a_1_1_path_relinking>`












.. ref-code-block:: cpp
	:class: overview-code-block

	
	namespace PathRelinking {

	// enums

	enum :ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`;
	enum :ref:`Selection<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>`;
	enum :ref:`Type<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>`;

	// global functions

	:ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`& :ref:`operator|=<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a12cb77f0dc67b1484f7f36bd5a3da903>` (:ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`& lhs, :ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` rhs);

	} // namespace PathRelinking
.. _details-doxid-namespace_b_r_k_g_a_1_1_path_relinking:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Holds the enumerations for Path Relinking algorithms.

Global Functions
----------------

.. index:: pair: function; operator|=
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a12cb77f0dc67b1484f7f36bd5a3da903:
.. _cid-brkga.pathrelinking.operator:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`& operator|= (:ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>`& lhs, :ref:`PathRelinkingResult<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>` rhs)

Perform bitwise ``OR`` between two ``PathRelinkingResult`` returning the highest rank ``PathRelinkingResult``.

For example

* ``TOO_HOMOGENEOUS | NO_IMPROVEMENT == NO_IMPROVEMENT``

* ``NO_IMPROVEMENT | ELITE_IMPROVEMENT == ELITE_IMPROVEMENT``

* ``ELITE_IMPROVEMENT | BEST_IMPROVEMENT == BEST_IMPROVEMENT``

