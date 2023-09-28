.. index:: pair: page; Todo List
.. _doxid-todo:

Todo List
=========

.. list-table::
	:widths: 20 80

	*
		- Member :ref:`BRKGA::KendallTauDistance::affectSolution <doxid-class_b_r_k_g_a_1_1_kendall_tau_distance_1a06d79321c14a511195daa4c1d143ed7e>` (Chromosome::const_iterator v1_begin, Chromosome::const_iterator v2_begin, const std::size_t block_size) override

		- :target:`doxid-todo_1_todo000006` This is the same implementation of Hamming. We need to figure out how to do it properly.

	*
		- Member :ref:`BRKGA::operator\<\< <doxid-group__algorithm__status_1gacd174acb61b2b339b678c6c2020316b0>` (std::ostream &output, const :ref:`AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>` &status)

		- :target:`doxid-todo_1_todo000005` Clang fails printing chrono times. When they fix it, please, revise this function.

	*
		- Member :ref:`BRKGA::readConfiguration <doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>` (std::istream &input, std::ostream &logger=std::cout)

		- :target:`doxid-todo_1_todo000003` This method can beneficiate from introspection tools. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.

	*
		- Member :ref:`BRKGA::writeConfiguration <doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>` (std::ostream &output, const :ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>` &brkga_params, const :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>` &control_params= :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>` {})

		- :target:`doxid-todo_1_todo000004` This method can beneficiate from introspection tools. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.

	*
		- Module :ref:`chrono_helpers <doxid-group__chrono__helpers>`

		- :target:`doxid-todo_1_todo000001` These functions may not be necessary for C++23, since streaming operators for chrono types are defined there.

	*
		- Member :ref:`operator\<\< <doxid-group__chrono__helpers_1gaf5b85af7b48112d0af6f04c27e6dbbb7>` (std::ostream &os, const std::chrono::duration< double > &dur)

		- :target:`doxid-todo_1_todo000002` Remove this when clang/libc get the support.

