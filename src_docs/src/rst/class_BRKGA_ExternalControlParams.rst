.. index:: pair: class; BRKGA::ExternalControlParams
.. _doxid-class_b_r_k_g_a_1_1_external_control_params:
.. _cid-brkga.externalcontrolparams:

class BRKGA::ExternalControlParams
==================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Represents additional control parameters that can be used outside this framework. :ref:`More...<details-doxid-class_b_r_k_g_a_1_1_external_control_params>`











.. index:: pair: function; ~ExternalControlParams
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a66bdd4268523ddeca03bc3a02ba80423:
.. _cid-brkga.externalcontrolparams.~externalcontrolparams:

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class ExternalControlParams
	{
	public:
		// fields
	
		unsigned :ref:`exchange_interval<doxid-class_b_r_k_g_a_1_1_external_control_params_1a9f8c1c5b517d319c0d5713b9c77efa95>`;
		unsigned :ref:`num_exchange_indivuduals<doxid-class_b_r_k_g_a_1_1_external_control_params_1a43d831d93698b1f982b6d0cdbed0ae12>`;
		unsigned :ref:`reset_interval<doxid-class_b_r_k_g_a_1_1_external_control_params_1ae9be7649e2a9c0803ed58385c9208dc3>`;

		// methods
	
		:ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_external_control_params_1a5bcad4c748fddaec8f5213259c4ee734>` (const :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`&);
	};
.. _details-doxid-class_b_r_k_g_a_1_1_external_control_params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Represents additional control parameters that can be used outside this framework.

These parameters are not used directly in the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` nor in the path relinking. However, they are loaded from the configuration file and can be called by the user to perform out-loop controlling.

Fields
------

.. index:: pair: variable; exchange_interval
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a9f8c1c5b517d319c0d5713b9c77efa95:
.. _cid-brkga.externalcontrolparams.exchange_interval:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned exchange_interval

Interval at which elite chromosomes are exchanged (0 means no exchange).

.. index:: pair: variable; num_exchange_indivuduals
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a43d831d93698b1f982b6d0cdbed0ae12:
.. _cid-brkga.externalcontrolparams.num_exchange_indivuduals:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned num_exchange_indivuduals

Number of elite chromosomes exchanged from each population.

.. index:: pair: variable; reset_interval
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1ae9be7649e2a9c0803ed58385c9208dc3:
.. _cid-brkga.externalcontrolparams.reset_interval:

.. ref-code-block:: cpp
	:class: title-code-block

	unsigned reset_interval

Interval at which the populations are reset (0 means no reset).

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a5bcad4c748fddaec8f5213259c4ee734:
.. _cid-brkga.externalcontrolparams.operator:

.. ref-code-block:: cpp
	:class: title-code-block

	:ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`& operator = (const :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`&)

Assignment operator for complaince.

