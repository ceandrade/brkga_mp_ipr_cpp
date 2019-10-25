.. _doxid-global:
.. index:: pair: namespace; global

API documentation
==============================

.. toctree::
	:hidden:

	namespace_BRKGA.rst
	class_EnumIO.rst










.. index:: pair: function; toUpper
.. _doxid-enum__io_8hpp_1ada3f4837517f607981fce6af2219f67e:
.. _cid-toupper:
.. index:: pair: function; toEnum
.. _doxid-enum__io_8hpp_1a1acb2a2038abbd68664d821dfa0a4fd1:
.. _cid-toenum:
.. index:: pair: function; operator<<
.. _doxid-enum__io_8hpp_1a52bbbf3567d7dbbd725ca9b9bea08a03:
.. _cid-operator<<:
.. index:: pair: function; operator>>
.. _doxid-enum__io_8hpp_1ab3226f85fd90430ed868709445c0896d:
.. _cid-operator>>:




.. ref-code-block:: cpp
	:class: overview-code-block

	
	// namespaces

	namespace :ref:`BRKGA<doxid-namespace_b_r_k_g_a>`;
		namespace :ref:`BRKGA::PathRelinking<doxid-namespace_b_r_k_g_a_1_1_path_relinking>`;

	// classes

	template <typename T>
	class :ref:`EnumIO<doxid-class_enum_i_o>`;

	// global functions

	static std::string toUpper(const std::string& input);

	template  <
		typename T,
		typename std::enable_if< std::is_enum< T >::value >::type * = nullptr
		>
	T toEnum(const std::string& s);

	template  <
		typename T,
		typename std::enable_if< std::is_enum< T >::value >::type * = nullptr
		>
	std::ostream& operator << (
		std::ostream& os,
		const T& t
		);

	template  <
		typename T,
		typename std::enable_if< std::is_enum< T >::value >::type * = nullptr
		>
	std::istream& operator >> (
		std::istream& is,
		T& t
		);

