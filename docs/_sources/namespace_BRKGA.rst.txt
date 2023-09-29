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
	class_BRKGA_Population.rst

Overview
~~~~~~~~

This namespace contains all facilities related to :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` Multi Parent with Implicit Path Relinking. :ref:`More...<details-namespace_b_r_k_g_a>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
	namespace BRKGA {

	// namespaces

	namespace :ref:`BRKGA::PathRelinking<doxid-namespace_b_r_k_g_a_1_1_path_relinking>`;

	// typedefs

	typedef std::vector<double> :ref:`Chromosome<doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`;
	typedef double :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`;

	// enums

	enum :ref:`BiasFunctionType<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>`;
	enum :ref:`Sense<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>`;
	enum :ref:`ShakingType<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>`;

	// classes

	class :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`;

	template <class Decoder>
	class :ref:`BRKGA_MP_IPR<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>`;

	class :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`;
	class :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`;
	class :ref:`DistanceFunctionBase<doxid-class_b_r_k_g_a_1_1_distance_function_base>`;
	class :ref:`HammingDistance<doxid-class_b_r_k_g_a_1_1_hamming_distance>`;
	class :ref:`KendallTauDistance<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>`;
	class :ref:`Population<doxid-class_b_r_k_g_a_1_1_population>`;

	// global variables

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`FITNESS_T_MIN<doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb>` = FITNESS_T_MIN_TEMPLATE<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`>;
	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` :ref:`FITNESS_T_MAX<doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f>` = FITNESS_T_MAX_TEMPLATE<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`>;
	constexpr double :ref:`EQUALITY_THRESHOLD<doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a>` = 1e-6;

	// global functions

	template <class T>
	constexpr bool :ref:`close_enough<doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>`(
		T a,
		T b
	);

	template <
		size_t I = 0,
		typename T,
		typename... Ts
	>
	constexpr bool :ref:`close_enough<doxid-group__utility__functions_1ga9e9de825b93fe16fe20dfee5b91c9690>`(
		std::tuple<T, Ts...> a,
		std::tuple<T, Ts...> b
	);

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> :ref:`readConfiguration<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>`(
		std::istream& input,
		std::ostream& logger = std::cout
	);

	INLINE std::pair<:ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`> :ref:`readConfiguration<doxid-group__brkga__control__params_1gac3cde9b5ca694e672bc52408f6381c4c>`(
		const std::string& filename,
		std::ostream& logger = std::cout
	);

	INLINE std::ostream& :ref:`operator <<<doxid-group__brkga__control__params_1gac2e90febdb3186e786a71ef60950f04f>` (
		std::ostream& os,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params
	);

	INLINE std::ostream& :ref:`operator <<<doxid-group__brkga__control__params_1ga1eb95713847bf695dd70c5cb4edc5aab>` (
		std::ostream& os,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params
	);

	INLINE void :ref:`writeConfiguration<doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>`(
		std::ostream& output,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	);

	INLINE void :ref:`writeConfiguration<doxid-group__brkga__control__params_1ga03609eb993f479e801c4b30d794b6203>`(
		const std::string& filename,
		const :ref:`BrkgaParams<doxid-class_b_r_k_g_a_1_1_brkga_params>`& brkga_params,
		const :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`& control_params = :ref:`ControlParams<doxid-class_b_r_k_g_a_1_1_control_params>`{}
	);

	std::ostream& :ref:`operator <<<doxid-group__algorithm__status_1gacd174acb61b2b339b678c6c2020316b0>` (
		std::ostream& output,
		const :ref:`AlgorithmStatus<doxid-class_b_r_k_g_a_1_1_algorithm_status>`& status
	);

	} // namespace BRKGA
.. _details-namespace_b_r_k_g_a:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

This namespace contains all facilities related to :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` Multi Parent with Implicit Path Relinking.

Typedefs
--------

.. index:: pair: typedef; Chromosome
.. _doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	typedef std::vector<double> Chromosome

Chromosone representation.

The chromosome is represented using a vector in the unitary hypercube, i.e., :math:`v \in [0,1]^n` where :math:`n` is the size of the array (dimensions of the hypercube). We use double precision because float precision maybe not be enough for some applications.

We could use ``std::vector<double>`` directly. However, using an alias, we can add additional capabilities to the Chromosome class in the future, such as parenting track. For example, we may want to do this:

.. ref-code-block:: cpp

	class :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`: public std::vector<double> {
	    public:
	        :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`() :
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

	std::vector<double>* pt = new :ref:`Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>`();     // Bad idea
	delete pt;      // Delete does not call the Chromosome destructor

.. index:: pair: typedef; fitness_t
.. _doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	typedef double fitness_t

Fitness type representation.

In general, fitness_t is a single number for single-objective problems. For instance:

.. ref-code-block:: cpp

	using :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` = double;

For multi-objective problems (with dominance/lexicographical sorting), we need to use multiple values. We can either use a class, structure, or ``std::tuple``. Using a custom class or structure, we must provide copy assignment (``operator=``), comparison operators (``operator<``, ``operator>``, and ``operator==``), and the streaming operator ``std::ostream& operator<<(std::ostream& os, const CustomFitness& fitness)`` where ``CustomFitness`` is your fitness structure. We have all these perks using ``std:tuple``. For example, assume we have three minimization objective functions. Then we may have:

.. ref-code-block:: cpp

	using :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` = std::tuple<double, double, double>;

.. note::

	We do recommend use ``std::tuple``.

Internally, BRKGA-MP-IPR doesn't use ``operator==`` directly. :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` implements the custom function ``:ref:`close_enough() <doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>```. For fundamental numerical types, it compares the absolute difference with a given :ref:`EQUALITY_THRESHOLD <doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a>`, i.e., two numbers :math:`a` and :math:`b` equal if :math:`|a - b| < EQUALITY\_THRESHOLD`. For all other types (except ``std::tuple``), we use ``operator==``. For ``std::tuple``, we have a specialized function ``:ref:`close_enough() <doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>``` that iterates over each element of the tuples calling the base ``:ref:`close_enough() <doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>```.

.. warning::

	If you are using custom class other than fundamental types or tuples with fundamental types, you must also provide two const template expressions :ref:`FITNESS_T_MIN <doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb>` and :ref:`FITNESS_T_MAX <doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f>`.

Global Variables
----------------

.. index:: pair: variable; FITNESS_T_MIN
.. _doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` FITNESS_T_MIN = FITNESS_T_MIN_TEMPLATE<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`>

The actual minimal value to ``fitness_t``.

.. index:: pair: variable; FITNESS_T_MAX
.. _doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	static constexpr :ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>` FITNESS_T_MAX = FITNESS_T_MAX_TEMPLATE<:ref:`fitness_t<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`>

The actual Maximum value to ``fitness_t``.

.. index:: pair: variable; EQUALITY_THRESHOLD
.. _doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	constexpr double EQUALITY_THRESHOLD = 1e-6

This constant is used to compare floating-point numbers to equality. Therefore, we consider two numbers :math:`a` and :math:`b` equal if :math:`|a - b| < EQUALITY\_THRESHOLD`.

Currently, this constant is only used during IPR procedures to compare fitness with fundamental types (int, flot, char, etc), either single type or embedded in a tuple. If your ``fitness_t`` has a custom type, this is not applied, as explained in :ref:`fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>`.

