.. _global:
.. index:: pair: namespace; global

API documentation
=================

.. toctree::
	:hidden:

	namespace_BRKGA.rst
	group_main_algorithms.rst
	group_brkga_control_params.rst
	group_algorithm_status.rst
	group_distance_functions.rst
	group_utility_functions.rst
	group_chrono_helpers.rst
	group_enum_io_templates.rst
	group_compiler_directives.rst


Overview
~~~~~~~~

All BRKGA-MP-IPR algorithms and facilities are within
namespace ``:ref:`BRKGA<doxid-namespace_b_r_k_g_a>```. Some helper are in
the general scope, such general streaming operator and enumaratio manipulation.


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block


	// namespaces

	namespace :ref:`BRKGA<doxid-namespace_b_r_k_g_a>`;
		namespace :ref:`BRKGA::PathRelinking<doxid-namespace_b_r_k_g_a_1_1_path_relinking>`;

	// classes

	template <typename T>
	class :ref:`EnumIO<doxid-class_enum_i_o>`;

	// global functions

	template <DurationType durantion_t>
	INLINE std::istream& :ref:`operator >><doxid-group__chrono__helpers_1ga79cf8ed7aa9f3f585935caba988d60c8>` (
		std::istream& is,
		durantion_t& duration
	);

	INLINE std::ostream& :ref:`operator <<<doxid-group__chrono__helpers_1gaf5b85af7b48112d0af6f04c27e6dbbb7>` (
		std::ostream& os,
		const std::chrono::duration<double>& dur
	);

	template <typename... Ts>
	std::ostream& :ref:`operator <<<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` (
		std::ostream& os,
		const std::tuple<Ts...>& tp
	);

	static std::string :target:`toUpper<doxid-enum__io_8hpp_1ada3f4837517f607981fce6af2219f67e>`(const std::string& input);

	template <EnumType T>
	T :target:`toEnum<doxid-enum__io_8hpp_1afea8619f6e8b531411980659ada31f4a>`(const std::string& s);

	template <EnumType T>
	std::ostream& :target:`operator <<<doxid-enum__io_8hpp_1a04b7b755bb1d0514802003ff47aaa868>` (
		std::ostream& os,
		const T& t
	);

	template <EnumType T>
	std::istream& :target:`operator >><doxid-enum__io_8hpp_1a398b74584e926b4c900380606f800e83>` (
		std::istream& is,
		T& t
	);

	// macros

	#define :ref:`BRKGA_MULTIPLE_INCLUSIONS<doxid-group__compiler__directives_1ga608f9e381594efac6848d962c0110e91>`
	#define :ref:`MATING_FULL_SPEED<doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7>`
	#define :ref:`MATING_SEED_ONLY<doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22>`
	#define :ref:`MATING_SEQUENTIAL<doxid-group__compiler__directives_1ga5766b15b52c6201d58440c53462fdf24>`
