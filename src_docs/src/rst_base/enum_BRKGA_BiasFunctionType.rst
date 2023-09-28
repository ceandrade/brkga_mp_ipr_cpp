.. index:: pair: enum; BiasFunctionType
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1:

enum BRKGA::BiasFunctionType
============================

Overview
~~~~~~~~

Specifies a bias function type when choosing parents to mating (``r`` is a given parameter). :ref:`More...<details-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum BiasFunctionType {
	    :ref:`CONSTANT<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a8d6b5cada83510220f59e00ce86d4d92>`,
	    :ref:`CUBIC<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1accd681e34e5e40fbce74618c3ccffcff>`,
	    :ref:`EXPONENTIAL<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adcd5fc33e211f31cef0cd7cb36518d31>`,
	    :ref:`LINEAR<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1aaac544aacc3615aada24897a215f5046>`,
	    :ref:`LOGINVERSE<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adca762bd1443afdcf03af352da1c9184>`,
	    :ref:`QUADRATIC<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1aa6ef2e8b7424bfa9d56315aca733861c>`,
	    :ref:`CUSTOM<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a72baef04098f035e8a320b03ad197818>`,
	};

.. _details-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies a bias function type when choosing parents to mating (``r`` is a given parameter).

This function substitutes the ``rhoe`` parameter from the original :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`.

Enum Values
-----------

.. index:: pair: enumvalue; CONSTANT
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a8d6b5cada83510220f59e00ce86d4d92:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CONSTANT

:math:`\frac{1}{\text{num. parents for mating}}` (all individuals have the same probability).

.. index:: pair: enumvalue; CUBIC
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1accd681e34e5e40fbce74618c3ccffcff:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CUBIC

:math:`r^{-3}`

.. index:: pair: enumvalue; EXPONENTIAL
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adcd5fc33e211f31cef0cd7cb36518d31:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	EXPONENTIAL

:math:`e^{-r}`

.. index:: pair: enumvalue; LINEAR
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1aaac544aacc3615aada24897a215f5046:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	LINEAR

:math:`1/r`

.. index:: pair: enumvalue; LOGINVERSE
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1adca762bd1443afdcf03af352da1c9184:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	LOGINVERSE

:math:`\frac{1}{\log(r + 1)}` (usually works better than other functions).

.. index:: pair: enumvalue; QUADRATIC
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1aa6ef2e8b7424bfa9d56315aca733861c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	QUADRATIC

:math:`r^{-2}`

.. index:: pair: enumvalue; CUSTOM
.. _doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1a72baef04098f035e8a320b03ad197818:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CUSTOM

Indicates a custom bias function supplied by the user.

