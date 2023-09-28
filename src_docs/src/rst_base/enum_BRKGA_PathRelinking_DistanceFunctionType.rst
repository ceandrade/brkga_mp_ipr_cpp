.. index:: pair: enum; DistanceFunctionType
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941:

enum BRKGA::PathRelinking::DistanceFunctionType
===============================================

Overview
~~~~~~~~

Specifies the distance function between two chromosomes during IPR. :ref:`More...<details-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum DistanceFunctionType {
	    :ref:`HAMMING<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a65f8928046bffc8d0f6a445a6ce4f63f>`,
	    :ref:`KENDALLTAU<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a398f810fb39fd62f08e22235fe566ced>`,
	    :ref:`CUSTOM<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a72baef04098f035e8a320b03ad197818>`,
	};

.. _details-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies the distance function between two chromosomes during IPR.

Enum Values
-----------

.. index:: pair: enumvalue; HAMMING
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a65f8928046bffc8d0f6a445a6ce4f63f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	HAMMING

Uses the default Hamming distance calculator.

.. index:: pair: enumvalue; KENDALLTAU
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a398f810fb39fd62f08e22235fe566ced:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	KENDALLTAU

Uses the default KendallTau distance calculator.

.. index:: pair: enumvalue; CUSTOM
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a87d1364dc2c4abffd0fa93bebde97941a72baef04098f035e8a320b03ad197818:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CUSTOM

Indicates a custom function supplied by the user.

