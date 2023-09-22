.. index:: pair: group; Functions for equality comparisons
.. _doxid-group__utility__functions:

Functions for equality comparisons
==================================

.. toctree::
	:hidden:

Overview
~~~~~~~~

These are helper functions that, at compiler time, detect if the ``fitness_t`` is a floating point type, and applies the absolute diference. :ref:`More...<details-group__utility__functions>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
	// global functions

	template <class T>
	constexpr bool :ref:`BRKGA::close_enough<doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>`(
		T a,
		T b
	);

	template <
		size_t I = 0,
		typename T,
		typename... Ts
	>
	constexpr bool :ref:`BRKGA::close_enough<doxid-group__utility__functions_1ga9e9de825b93fe16fe20dfee5b91c9690>`(
		std::tuple<T, Ts...> a,
		std::tuple<T, Ts...> b
	);

.. _details-group__utility__functions:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

These are helper functions that, at compiler time, detect if the ``fitness_t`` is a floating point type, and applies the absolute diference. Otherwise, the compiler generates the equality comparison.

Global Functions
----------------

.. index:: pair: function; close_enough
.. _doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	template <class T>
	constexpr bool BRKGA::close_enough(
		T a,
		T b
	)

Compares two values to equality.

If these values are real numbers, we compare their absolute difference with ``EQUALITY_THRESHOLD``, i.e., :math:`|a - b| < EQUALITY\_THRESHOLD`. In other cases (except tuples, which have a specialization), we use the ``operator==`` directly. Therefore, ``fitness_t`` must define ``operator==``.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- a

		- First item to be compared.

	*
		- b

		- Second item to be compared.



.. rubric:: Returns:

true if a and b are equal.

.. index:: pair: function; close_enough
.. _doxid-group__utility__functions_1ga9e9de825b93fe16fe20dfee5b91c9690:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	template <
		size_t I = 0,
		typename T,
		typename... Ts
	>
	constexpr bool BRKGA::close_enough(
		std::tuple<T, Ts...> a,
		std::tuple<T, Ts...> b
	)

Compares two tuples to equality.

This specialization iterates, recursively, of each tuples' members and compares them. Note that this is done in compilation time, with no impact at running time.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- a

		- First tuple to be compared.

	*
		- b

		- Second tuple to be compared.



.. rubric:: Returns:

true if a and b are equal.

