.. index:: pair: class; BRKGA::ExternalControlParams
.. _doxid-class_b_r_k_g_a_1_1_external_control_params:

class BRKGA::ExternalControlParams
==================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Represents additional control parameters that can be used outside this framework. :ref:`More...<details-class_b_r_k_g_a_1_1_external_control_params>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class ExternalControlParams {
	public:
		// fields
	
		unsigned :ref:`exchange_interval<doxid-class_b_r_k_g_a_1_1_external_control_params_1a9f8c1c5b517d319c0d5713b9c77efa95>`;
		unsigned :ref:`num_exchange_indivuduals<doxid-class_b_r_k_g_a_1_1_external_control_params_1a43d831d93698b1f982b6d0cdbed0ae12>`;
		unsigned :ref:`reset_interval<doxid-class_b_r_k_g_a_1_1_external_control_params_1ae9be7649e2a9c0803ed58385c9208dc3>`;

		// methods
	
		ExternalControlParams& :ref:`operator =<doxid-class_b_r_k_g_a_1_1_external_control_params_1a8aa8eeab7c828ea34add9686a6c90730>` (const ExternalControlParams&);
	};
.. _details-class_b_r_k_g_a_1_1_external_control_params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Represents additional control parameters that can be used outside this framework.

These parameters are not used directly in the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` nor in the path relinking. However, they are loaded from the configuration file and can be called by the user to perform out-loop controlling.

Fields
------

.. index:: pair: variable; exchange_interval
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a9f8c1c5b517d319c0d5713b9c77efa95:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned exchange_interval

Interval at which elite chromosomes are exchanged (0 means no exchange).

.. index:: pair: variable; num_exchange_indivuduals
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a43d831d93698b1f982b6d0cdbed0ae12:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned num_exchange_indivuduals

Number of elite chromosomes exchanged from each population.

.. index:: pair: variable; reset_interval
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1ae9be7649e2a9c0803ed58385c9208dc3:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned reset_interval

Interval at which the populations are reset (0 means no reset).

Methods
-------

.. index:: pair: function; operator=
.. _doxid-class_b_r_k_g_a_1_1_external_control_params_1a8aa8eeab7c828ea34add9686a6c90730:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	ExternalControlParams& operator = (const ExternalControlParams&)

Assignment operator for complaince.

