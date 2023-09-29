.. index:: pair: enum; ShakingType
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494:

enum BRKGA::ShakingType
=======================

Overview
~~~~~~~~

Specifies the type of shaking to be performed. :ref:`More...<details-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum ShakingType {
	    :ref:`CHANGE<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073>`,
	    :ref:`SWAP<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a46fc23bc4e4d57e5469a39658a6dd3e8>`,
	    :ref:`CUSTOM<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a72baef04098f035e8a320b03ad197818>`,
	};

.. _details-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies the type of shaking to be performed.

Enum Values
-----------

.. index:: pair: enumvalue; CHANGE
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CHANGE

Applies the following perturbations:

#. Inverts the value of a random chosen, i.e., from ``value`` to ``1 - value``;

#. Assigns a random value to a random key.

.. index:: pair: enumvalue; SWAP
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a46fc23bc4e4d57e5469a39658a6dd3e8:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	SWAP

Applies two swap perturbations:

#. Swaps the values of a randomly chosen key ``i`` and its neighbor ``i + 1``;

#. Swaps values of two randomly chosen keys.

.. index:: pair: enumvalue; CUSTOM
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a72baef04098f035e8a320b03ad197818:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	CUSTOM

Indicates a custom shaking procedure supplied by the user.

