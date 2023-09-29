.. index:: pair: group; BRKGA and Control Parameters
.. _doxid-group__brkga__control__params:

BRKGA and Control Parameters
============================

.. toctree::
	:hidden:

	class_BRKGA_BrkgaParams.rst
	class_BRKGA_ControlParams.rst

Overview
~~~~~~~~




.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
	// classes

	class :ref:`BRKGA::BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`;
	class :ref:`BRKGA::ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`;

	// global functions

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> :ref:`BRKGA::readConfiguration<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>`(
		std::istream& input,
		std::ostream& logger = std::cout
	);

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> :ref:`BRKGA::readConfiguration<doxid-group__brkga__control__params_1gac3cde9b5ca694e672bc52408f6381c4c>`(
		const std::string& filename,
		std::ostream& logger = std::cout
	);

	INLINE std::ostream& :ref:`BRKGA::operator <<<doxid-group__brkga__control__params_1gac2e90febdb3186e786a71ef60950f04f>` (
		std::ostream& os,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params
	);

	INLINE std::ostream& :ref:`BRKGA::operator <<<doxid-group__brkga__control__params_1ga1eb95713847bf695dd70c5cb4edc5aab>` (
		std::ostream& os,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params
	);

	INLINE void :ref:`BRKGA::writeConfiguration<doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>`(
		std::ostream& output,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	);

	INLINE void :ref:`BRKGA::writeConfiguration<doxid-group__brkga__control__params_1ga03609eb993f479e801c4b30d794b6203>`(
		const std::string& filename,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	);

.. _details-group__brkga__control__params:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~



Global Functions
----------------

.. index:: pair: function; readConfiguration
.. _doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> BRKGA::readConfiguration(
		std::istream& input,
		std::ostream& logger = std::cout
	)

Reads the parameters from an input stream.

Todo This method can beneficiate from introspection tools. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- input

		- the input stream. It can be a file or a string stream.

	*
		- logger

		- a output stream to log some information such as missing no-required parameters. It does not log errors.

	*
		- std::fstream::failure

		- in case of errors in the file.



.. rubric:: Returns:

a tuple containing the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and external control parameters.

.. index:: pair: function; readConfiguration
.. _doxid-group__brkga__control__params_1gac3cde9b5ca694e672bc52408f6381c4c:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> BRKGA::readConfiguration(
		const std::string& filename,
		std::ostream& logger = std::cout
	)

Reads the parameters from a configuration file.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- filename

		- the configuration file.

	*
		- logger

		- a output stream to log some information such as missing no-required parameters. It does not log errors.

	*
		- std::fstream::failure

		- in case of errors in the file.



.. rubric:: Returns:

a tuple containing the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and external control parameters.

.. index:: pair: function; operator<<
.. _doxid-group__brkga__control__params_1gac2e90febdb3186e786a71ef60950f04f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE std::ostream& BRKGA::operator << (
		std::ostream& os,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params
	)

Output stream operator for :ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>`.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- os

		- the output stream.

	*
		- brkga_params

		- the parameters.



.. rubric:: Returns:

the output stream.

.. index:: pair: function; operator<<
.. _doxid-group__brkga__control__params_1ga1eb95713847bf695dd70c5cb4edc5aab:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE std::ostream& BRKGA::operator << (
		std::ostream& os,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params
	)

Output stream operator for :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>`.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- os

		- the output stream.

	*
		- control_params

		- the parameters.



.. rubric:: Returns:

the output stream.

.. index:: pair: function; writeConfiguration
.. _doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE void BRKGA::writeConfiguration(
		std::ostream& output,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	)

Writes the parameters into a output stream.

.. note::

	All floating point parameters are written with two point precision.

Todo This method can beneficiate from introspection tools. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- output

		- the output stream.

	*
		- brkga_params

		- the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` parameters.

	*
		- control_params

		- the external control parameters. Default is an empty object.

	*
		- std::fstream::failure

		- in case of errors in the file.

.. index:: pair: function; writeConfiguration
.. _doxid-group__brkga__control__params_1ga03609eb993f479e801c4b30d794b6203:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	INLINE void BRKGA::writeConfiguration(
		const std::string& filename,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	)

Writes the parameters into a configuration file.

.. note::

	All floating point parameters are written with two point precision.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- filename

		- the configuration file.

	*
		- brkga_params

		- the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` parameters.

	*
		- control_params

		- the external control parameters. Default is an empty object.

	*
		- std::fstream::failure

		- in case of errors in the file.

