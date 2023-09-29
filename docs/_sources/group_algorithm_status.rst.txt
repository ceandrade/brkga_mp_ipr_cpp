.. index:: pair: group; Algorithm status utilities
.. _doxid-group__algorithm__status:

Algorithm status utilities
==========================

.. toctree::
	:hidden:

	class_BRKGA_AlgorithmStatus.rst

Overview
~~~~~~~~




.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
	// classes

	class :ref:`BRKGA::AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`;

	// global functions

	std::ostream& :ref:`BRKGA::operator <<<doxid-group__algorithm__status_1gacd174acb61b2b339b678c6c2020316b0>` (
		std::ostream& output,
		const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`& status
	);

.. _details-group__algorithm__status:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~



Global Functions
----------------

.. index:: pair: function; operator<<
.. _doxid-group__algorithm__status_1gacd174acb61b2b339b678c6c2020316b0:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	std::ostream& BRKGA::operator << (
		std::ostream& output,
		const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`& status
	)

Output streaming operator for :ref:`AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>`.

Todo Clang fails printing chrono times. When they fix it, please, revise this function.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- output

		- the output stream.

	*
		- status

		- the algorithm status.



.. rubric:: Returns:

the output stream.

