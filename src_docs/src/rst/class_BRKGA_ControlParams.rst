.. index:: pair: class; BRKGA::ControlParams
.. _doxid-class_b_r_k_g_a_1_1_control_params:

class BRKGA::ControlParams
==========================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Represents additional control parameters that can be used outside this framework. :ref:`More...<details-class_b_r_k_g_a_1_1_control_params>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#include <brkga_mp_ipr.hpp>
	
	class ControlParams {
	public:
		// fields
	
		std::chrono::seconds :ref:`maximum_running_time<doxid-class_b_r_k_g_a_1_1_control_params_1af0ce79e47569d1189c9bf74fc66766e0>` {0};
		unsigned :ref:`exchange_interval<doxid-class_b_r_k_g_a_1_1_control_params_1a143277bea05ee5204fbb50b805e6569c>` {0};
		unsigned :ref:`ipr_interval<doxid-class_b_r_k_g_a_1_1_control_params_1acadef8421e45b48b4af9709a5a9125e3>` {0};
		unsigned :ref:`shake_interval<doxid-class_b_r_k_g_a_1_1_control_params_1acf6dbc7ef57a532acebc65fc70d51cf7>` {0};
		unsigned :ref:`reset_interval<doxid-class_b_r_k_g_a_1_1_control_params_1a6471f8cce94062e95cfd035f1d13cd49>` {0};
		unsigned :ref:`stall_offset<doxid-class_b_r_k_g_a_1_1_control_params_1a7f438a8e777a9bee2c8289c0e7d13a3b>` {0};
	};
.. _details-class_b_r_k_g_a_1_1_control_params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Represents additional control parameters that can be used outside this framework.

These parameters are not used directly in the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` nor in the path relinking. However, they are loaded from the configuration file and can be called by the user to perform out-loop controlling.

Fields
------

.. index:: pair: variable; maximum_running_time
.. _doxid-class_b_r_k_g_a_1_1_control_params_1af0ce79e47569d1189c9bf74fc66766e0:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::chrono::seconds maximum_running_time {0}

Maximum running time.

.. index:: pair: variable; exchange_interval
.. _doxid-class_b_r_k_g_a_1_1_control_params_1a143277bea05ee5204fbb50b805e6569c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned exchange_interval {0}

Interval / number of interations without improvement in the best solution at which elite chromosomes are exchanged (0 means no exchange).

.. index:: pair: variable; ipr_interval
.. _doxid-class_b_r_k_g_a_1_1_control_params_1acadef8421e45b48b4af9709a5a9125e3:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned ipr_interval {0}

Interval / number of interations without improvement in the best solution at which the Implicit Path Relink is called (0 means no IPR).

.. index:: pair: variable; shake_interval
.. _doxid-class_b_r_k_g_a_1_1_control_params_1acf6dbc7ef57a532acebc65fc70d51cf7:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned shake_interval {0}

Interval / number of interations without improvement in the best solution at which the populations are shaken (0 means no shake).

.. index:: pair: variable; reset_interval
.. _doxid-class_b_r_k_g_a_1_1_control_params_1a6471f8cce94062e95cfd035f1d13cd49:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned reset_interval {0}

Interval / number of interations without improvement in the best solution at which the populations are reset (0 means no reset).

.. index:: pair: variable; stall_offset
.. _doxid-class_b_r_k_g_a_1_1_control_params_1a7f438a8e777a9bee2c8289c0e7d13a3b:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	unsigned stall_offset {0}

Defines the numbers iterations to stop when the best solution is not improved, i.e., the algorithm converged (0 means don't stop by stall).

