.. index:: pair: enum; ShakingType
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494:
.. _cid-brkga.shakingtype:

enum BRKGA::ShakingType
=======================

Overview
~~~~~~~~

Specifies the type of shaking to be performed. :ref:`More...<details-doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>`

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum BRKGA::ShakingType
	{
	    :ref:`CHANGE<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073>` = 0,
	    :ref:`SWAP<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a46fc23bc4e4d57e5469a39658a6dd3e8>`   = 1,
	};

.. _details-doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies the type of shaking to be performed.

Enum Values
-----------

.. index:: pair: enumvalue; CHANGE
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073:
.. _cid-brkga.shakingtype.change:

.. ref-code-block:: cpp
	:class: title-code-block

	CHANGE

Applies the following perturbations:

#. Inverts the value of a random chosen, i.e., from ``value`` to ``1 - value``;

#. Assigns a random value to a random key.

.. index:: pair: enumvalue; SWAP
.. _doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a46fc23bc4e4d57e5469a39658a6dd3e8:
.. _cid-brkga.shakingtype.swap:

.. ref-code-block:: cpp
	:class: title-code-block

	SWAP

Applies two swap perturbations:

#. Swaps the values of a randomly chosen key ``i`` and its neighbor ``i + 1``;

#. Swaps values of two randomly chosen keys.

