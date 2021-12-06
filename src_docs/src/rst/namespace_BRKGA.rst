.. index:: pair: namespace; BRKGA
.. _doxid-namespace_b_r_k_g_a:

namespace BRKGA
===============

.. toctree::
	:hidden:

	namespace_BRKGA_PathRelinking.rst
	enum_BRKGA_BiasFunctionType.rst
	enum_BRKGA_Sense.rst
	enum_BRKGA_ShakingType.rst
	class_BRKGA_BRKGA_MP_IPR.rst
	class_BRKGA_BrkgaParams.rst
	class_BRKGA_DistanceFunctionBase.rst
	class_BRKGA_ExternalControlParams.rst
	class_BRKGA_HammingDistance.rst
	class_BRKGA_KendallTauDistance.rst
	class_BRKGA_Population.rst

Overview
~~~~~~~~

This namespace contains all stuff related to :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` Multi Parent with Implicit Path Relinking. :ref:`More...<details-namespace_b_r_k_g_a>`

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	namespace BRKGA {

	// namespaces

	namespace :ref:`BRKGA::PathRelinking<doxid-namespace_b_r_k_g_a_1_1_path_relinking>`;

	// typedefs

	typedef std::vector<double> :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`;
	typedef double :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>`;

	// enums

	enum :ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>`;
	enum :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>`;
	enum :ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>`;

	// classes

	template <class Decoder>
	class :ref:`BRKGA_MP_IPR<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>`;

	class :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`;
	class :ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`;
	class :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`;
	class :ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance>`;
	class :ref:`KendallTauDistance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`;
	class :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`;

	// global variables

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` :ref:`FITNESS_T_MIN<doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb>`;
	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` :ref:`FITNESS_T_MAX<doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f>`;
	constexpr double :ref:`EQUALITY_THRESHOLD<doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a>`;

	// global functions

	:ref:`INLINE<doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116>` std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`> :ref:`readConfiguration<doxid-namespace_b_r_k_g_a_1a6ea1575c98d23be6abbc2a497f31529f>`(const std::string& filename);

	:ref:`INLINE<doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116>` void :ref:`writeConfiguration<doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4>`(
		const std::string& filename,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`& control_params = :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`()
		);

	} // namespace BRKGA
.. _details-namespace_b_r_k_g_a:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

This namespace contains all stuff related to :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` Multi Parent with Implicit Path Relinking.

Typedefs
--------

.. index:: pair: typedef; Chromosome
.. _doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	typedef std::vector<double> Chromosome

Chromosone representation.

The chromosome is represented using a vector in the unitary hypercube, i.e., :math:`v \in [0,1]^n` where :math:`n` is the size of the array (dimensions of the hypercube). We use double precision because float precision maybe not be enough for some applications.

We could use ``std::vector<double>`` directly. However, using ``typedef``, we can add additional capabilities to the Chromosome class in the future, such as parenting track. For example, we may want to do this:

.. ref-code-block:: cpp

	class :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`: public std::vector<double> {
	    public:
	        :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`() :
	            std::vector<double>(), my_personal_data(0.0)
	            {}
	    public:
	        double my_personal_data;
	};

to keep track of some data within a specifc chromosome.

In general, most people do not recommend to inherit publicly from ``std::vector`` because it has no virtual destructor. However, we may do that as long as:

a) We remember that every operation provided by ``std::vector`` must be a semantically valid operation on an object of the derived class;

b) We avoid creating derived class objects with dynamic storage duration;

c) We **DO AVOID** polymorphism:

.. ref-code-block:: cpp

	std::vector<double>* pt = new :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1ac1d4eb0799f47b27004f711bdffeb1c4>`();     // Bad idea
	delete pt;      // Delete does not call the Chromosome destructor

.. index:: pair: typedef; fitness_t
.. _doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	typedef double fitness_t

Fitness type representation.

In general, fitness_t is a single number for single-objective problems. For instance:

.. ref-code-block:: cpp

	typedef double :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>`;

For multi-objective problems (with dominance/lexicographical sorting), we need to use multiple values. We can either use a class, structure, or ``std::tuple``. Using a custom class or structure, we must provide copy assignment (``operator=``) and comparison operators (``operator<``, ``operator>``, and ``operator==``). We have all these perks using ``std:tuple``. For example, assume we have three minimization objective functions. Then we may have:

.. ref-code-block:: cpp

	typedef std::tuple<double, double, double> :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>`;

We do recommend use ``std::tuple``.

Internally, BRKGA-MP-IPR doesn't use ``operator==`` directly. :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` implements the custom function ``close_enough()``. For fundamental numerical types, it compares the absolute difference with a given :ref:`EQUALITY_THRESHOLD <doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a>`, i.e., two numbers :math:`a` and :math:`b` equal if :math:`|a - b| < EQUALITY\_THRESHOLD`. For all types (except ``std::tuple``), we use operator==. For ``std::tuple``, we have a specialized ``close_enough()`` that iterates over each element of the tuples calling the base ``close_enough()``.

If you are using custom class other than fundamental types or tuples with fundamental types, you must also provide two const template expressions :ref:`FITNESS_T_MIN <doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb>` and :ref:`FITNESS_T_MAX <doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f>`.

Global Variables
----------------

.. index:: pair: variable; FITNESS_T_MIN
.. _doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` FITNESS_T_MIN

The actual minimal value to ``fitness_t``.

.. index:: pair: variable; FITNESS_T_MAX
.. _doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>` FITNESS_T_MAX

The actual Maximum value to ``fitness_t``.

.. index:: pair: variable; EQUALITY_THRESHOLD
.. _doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	constexpr double EQUALITY_THRESHOLD

This constant is used to compare floating-point numbers to equality. Therefore, we consider two numbers :math:`a` and :math:`b` equal if :math:`|a - b| < EQUALITY\_THRESHOLD`.

Currently, this constant is only used during IPR procedures to compare fitness with fundamental types (int, flot, char, etc), either single type or embedded in a tuple. If your ``fitness_t`` has a custom type, this is not applied, as explained in :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae9551fcbbfd6072b95e5f112e3b1565e>`.

Global Functions
----------------

.. index:: pair: function; readConfiguration
.. _doxid-namespace_b_r_k_g_a_1a6ea1575c98d23be6abbc2a497f31529f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`INLINE<doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116>` std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`> readConfiguration(const std::string& filename)

Reads the parameters from a configuration file.

Todo (ceandrade) This method can beneficiate from introspection tools from C++17. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.



.. rubric:: Parameters:

.. list-table::
	:widths: 20 80

	*
		- filename

		- the configuration file.

	*
		- std::fstream::failure

		- in case of errors in the file.



.. rubric:: Returns:

a tuple containing the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and external control parameters.

.. index:: pair: function; writeConfiguration
.. _doxid-namespace_b_r_k_g_a_1a01bade43afee725ca73c3f45a76012c4:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	:ref:`INLINE<doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116>` void writeConfiguration(
		const std::string& filename,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`& control_params = :ref:`ExternalControlParams<doxid-class_b_r_k_g_a_1_1_external_control_params>`()
		)

Writes the parameters into a file..

Todo (ceandrade) This method can beneficiate from introspection tools from C++17. We would like achieve a code similar to the `Julia counterpart <https://github.com/ceandrade/brkga_mp_ipr_julia>`__.



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

