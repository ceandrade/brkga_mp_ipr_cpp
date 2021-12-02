.. index:: pair: page; Todo List
.. _doxid-todo:

Todo List
===============================================================================

Major TODOs
-------------------------------------------------------------------------------

Implement unit tests and cover tests for all functionalities, mimicking the
Julia version.


Minor TODOs
-------------------------------------------------------------------------------

.. list-table::
	:widths: 20 80

	*
		- :target:`doxid-todo_1_todo000003` Member :ref:`BRKGA::KendallTauDistance::affectSolution <doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a4638274b38b50051eaba3f399b1b9c5f>` (std::vector< double >::const_iterator v1_begin, std::vector< double >::const_iterator v2_begin, const std::size_t block_size)

		- (ceandrade): implement this properly.

	*
		- :target:`doxid-todo_1_todo000001` Member :ref:`BRKGA::readConfiguration <doxid-namespace_b_r_k_g_a_1ad212f0711891038e623f4d882509897e>` (const std::string &filename)

		- (ceandrade) This method can beneficiate from introspection tools from C++17. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/BrkgaMpIpr.jl>`__.

	*
		- :target:`doxid-todo_1_todo000002` Member :ref:`BRKGA::writeConfiguration <doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>` (const std::string &filename, const :ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>` &brkga_params, const :ref:`ExternalControlParams <doxid-class_b_r_k_g_a_1_1_external_control_params>` &control_params=ExternalControlParams())

		- (ceandrade) This method can beneficiate from introspection tools from C++17. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/BrkgaMpIpr.jl>`__.

