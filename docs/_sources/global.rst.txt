.. _global:
.. index:: pair: namespace; global

API documentation
==============================

.. toctree::
	:hidden:

	namespace_BRKGA.rst
	class_EnumIO.rst
	preprocessor_directives.rst

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block


	// namespaces

	namespace :ref:`BRKGA<doxid-namespace_b_r_k_g_a>`;
	namespace :ref:`BRKGA::PathRelinking<doxid-namespace_b_r_k_g_a_1_1_path_relinking>`;

	// classes

	template <typename T>
	class :ref:`EnumIO<doxid-class_enum_i_o>`;

	// global functions

	static std::string :target:`toUpper<doxid-enum__io_8hpp_1ada3f4837517f607981fce6af2219f67e>`(const std::string& input);

	template <
		typename T,
		typename std::enable_if<std::is_enum<T>::value>::type* = nullptr
		>
	T :target:`toEnum<doxid-enum__io_8hpp_1a1acb2a2038abbd68664d821dfa0a4fd1>`(const std::string& s);

	template <
		typename T,
		typename std::enable_if<std::is_enum<T>::value>::type* = nullptr
		>
	std::ostream& :target:`operator <<<doxid-enum__io_8hpp_1a6656bb4c91dcb19d6d089791bcfd4d5e>` (
		std::ostream& os,
		const T& t
		);

	template <
		typename T,
		typename std::enable_if<std::is_enum<T>::value>::type* = nullptr
		>
	std::istream& :target:`operator >><doxid-enum__io_8hpp_1a2600831426fc570b21237e48b2c467fd>` (
		std::istream& is,
		T& t
		);

