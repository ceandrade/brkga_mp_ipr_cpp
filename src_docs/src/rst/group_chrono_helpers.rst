.. index:: pair: group; I/O chrono help functions
.. _doxid-group__chrono__helpers:

I/O chrono help functions
=========================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Defines some helpers for chrono types. :ref:`More...<details-group__chrono__helpers>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
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

.. _details-group__chrono__helpers:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Defines some helpers for chrono types.

Todo These functions may not be necessary for C++23, since streaming operators for chrono types are defined there.

Global Functions
----------------

.. index:: pair: function; operator>>
.. _doxid-group__chrono__helpers_1ga79cf8ed7aa9f3f585935caba988d60c8:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	template <DurationType durantion_t>
	INLINE std::istream& operator >> (
		std::istream& is,
		durantion_t& duration
	)

Input stream operator for std::chrono durations.

Until C++20, we have no operator>> for reading std::chrono. So, the following definition is only for reading seconds. We first read the integer representation of a second, then convert it to the duration.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- durantion_t

		- a chrono::duration type.

	*
		- is

		- the input stream.

	*
		- duration

		- the duration



.. rubric:: Returns:

the input stream.

.. index:: pair: function; operator<<
.. _doxid-group__chrono__helpers_1gaf5b85af7b48112d0af6f04c27e6dbbb7:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE std::ostream& operator << (
		std::ostream& os,
		const std::chrono::duration<double>& dur
	)

Output stream operator for chrono durations (and seconds) for Clang/libc.

To the date, libc (clang) has no support to stream chrono objects. This is a working around only for seconds.

Todo Remove this when clang/libc get the support.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- os

		- the output operator.

	*
		- dur

		- the duration.



.. rubric:: Returns:

the output operator.

