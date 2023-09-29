.. index:: pair: page; BRKGA-MP-IPR 3.0 Guide and Documentation
.. _doxid-indexpage:

BRKGA-MP-IPR Guide and Documentation - C++ Version 3.0
*******************************************************************************

:target:`doxid-index_mainpage`

BRKGA-MP-IPR provides a *very easy-to-use* framework for the Multi-Parent
Biased Random-Key Genetic Algorithm with Implict Path Relink
(**BRKGA-MP-IPR**). Assuming that your have a *decoder* to your problem, we can
setup, run, and extract the value of the best solution in less than 5 commands
(obvisiously, you may need few other lines fo code to do a proper test).

This C++ version provides a fast prototyping API using `C++20 standards
<https://en.wikipedia.org/wiki/C%2B%2B20>`__ and libraries. All code was
developed as a header-only library, and have no external dependencies other
than those included in the package. So, you just need to copy/check out the
files and point your compiler's header path to BRKGA-MP-IPR folder (``-I`` on
GCC and Clang).

This framework can use multiple threads of modern CPUs, by setting a single
parameter (assuming that your decoder is thread-safe). This leverage the
parallel decoding nature that BRKGAs offer, compared to other (meta) heuristic
frameworks.

The code can be compiled using `> GCC 9.4 <https://gcc.gnu.org>`__ and
`> Clang 10.0 <https://clang.llvm.org>`__, and it is very probable that it can
be compiled using other modern C++ compilers, such as Intel and Microsoft
compilers. To use multiple threads, your compiler must support `OpenMP
<https://www.openmp.org>`__. The current version has been long developed, and
it is a very mature code used in production in several companies. However, it
lacks of a proper unit and coverage testing.

If C++ is not suitable to you, we may find useful the `Julia version
<https://github.com/ceandrade/brkga_mp_ipr_julia>`__ of this framework. We are
also developing a `Python version
<https://github.com/ceandrade/brkga_mp_ipr_python>`__ which is in its earlier
stages (no IPR yet). However, both Julia and Python versions only support
single-objective optimization at this moment. We have no timeline to implement
multi-objective optimization in such platforms. Also, at this moment, we have
no plans to implement the BRKGA-MP-IPR in other languages such as Java or C#.
But if you want to do so, you are must welcome. But please, keep the API as
close as possible to the C++ API (or Julia API in case you decide go C), and
use the best coding and documentation practices of your chosen
language/framework.

* `Julia version <https://github.com/ceandrade/brkga_mp_ipr_julia>`__

* `Python version <https://github.com/ceandrade/brkga_mp_ipr_python>`__

If you are not familiar with how :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`
works, take a look on `Multi-Parent BRKGA
<https://doi.org/10.1016/j.ejor.2019.11.037>`__. In the future, we will provide
a *Prime on BRKGA-MP* section. If you know what *elite set*, *decoder*, and so
means, we can get to the guts on the :ref:`Guide <doxid-guide>`.

.. admonition:: OVER HERE!

    :ref:`Skip the update and go to the tutorial clicking here! <doxid-guide>`


.. _doxid-index_new_on_version3:

What is new on version 3.0
===============================================================================

.. _doxid-index_new_on_version3_api_enhancements:

API enhancements
-------------------------------------------------------------------------------

On version 2.0, we claimed that BRKGA-MP-IPR was a very easy-to-use framework.
But, few people told me this statement was not even true. The main complaining
was that while the features were very nice, tightening them together was hard,
even using the provided examples.

Now, BRKGA-MP-IPR supplies a method called
``:ref:`BRKGA_MP_IPR::run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```.
It implements the entire pipeline using all framework features in a chain-like
way, similar to the detailed examples. The user may call in this way:

.. code-block:: cpp
    :linenos:

    //...
    auto [brkga_params, control_params] =
          readConfiguration(config_file);

    MyDecoder my_decoder;

    BRKGA_MP_IPR<MyDecoder> algorithm(
        my_decoder, BRKGA::Sense::MINIMIZE, seed, num_chromosomes, brkga_params
    );
    // algorithm.initialize(); // No need anymore :-)

    auto final_status = algorithm.run(control_params, &cout);
    cout << "Final algorithm status\n" << final_status;
    //...

where ``control_params`` is an instance of the new class
``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
(explained further), and an optional stream for logging (in this example,
``cout``).
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
returns an
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
object with information about the optimization like total time, iteration
counting, and more (check the full documentation for that).

So, users need no more write fine control loops unless they need/want. Just set
some control parameters (and some other callbacks, described below, if you
like), and you are good to go!

Supporting
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```,
we have three new methods:

* ``:ref:`setStoppingCriteria() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1adee8fc8410a56e71b2af84ed6f4f2a7c>```:
  while method
  ``:ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
  sets automatically maximum time or maximum stalled iterations (without
  improvement in the best solution) as standard stopping criteria, the user can
  add to these other criteria using this method. For instance, the following
  lambda function tests if the best solution reached a given value:

  .. code-block:: cpp
    :linenos:

      fitness_t my_magical_solution = 10;
      algorithm.setStoppingCriteria(
          [&](const AlgorithmStatus& status) {
              return status.best_fitness == my_magical_solution;
          }
      );

  In this case, the stop criteria become: Is maximum time reached **OR** Is
  maximum stalled iterations reached **OR** ``best_fitness == my_magical_solution``.

  .. warning::

    While we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** (mainly when using
    IPR), if you really rmean to have no maximum time or maximum stalled
    iterations set, we recommend to use the following code:

  .. code-block:: cpp
    :linenos:

      // After reading your parameters, e.g.,
      // auto [brkga_params, control_params] = readConfiguration("config.conf");
      // You can set to the max.
      control_params.maximum_running_time = std::chrono::seconds::max();
      control_params.stall_offset = numeric_limits<decltype(control_params.stall_offset)>::max();

* ``:ref:`addNewSolutionObserver() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa6cf3aca1879ffd4dc0c986340163254>```:
  This method adds a callback that is triggered when an overall improved
  solution is found by the algorithm. It also adds an additional stop point if
  the users finds it useful by return ``true``. This is very useful for
  tracking the evolution of the algorithm, for instance:

  .. code-block:: cpp
    :linenos:

      algorithm.addNewSolutionObserver(
          [](const AlgorithmStatus& status) {
              std::cout
              << "> Iter: " << status.current_iteration
              << " | solution: " << status.best_fitness
              << " | time: " << status.current_time
              << std::endl;
              return false; // Dont' stop the optimization.
           }
      );

* ``:ref:`setShakingMethod() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169>```:
  This method adds a custom shaking procedure defined by the user. Please,
  refer to its documentation for more details.

Less important but still relevant: previously, one must call ``initialize()``
before any method that manipulated the population. Also, since ``initialize()``
(re)decodes the population, we have to measure its running time too. Now, the
user do not need to call ``initialize()`` anymore!!! ``initialize()`` is
called on the need by its fellow methods internally. This leads to fewer
error-prone codes.


.. _doxid-index_new_on_version3_parameters:

BRKGA and control parameters
-------------------------------------------------------------------------------

Although this is part of API enhancement, it deserves special attention. Now,
we include all :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` and IPR parameters into
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>```, and all
(external) control parameters into ``:ref:`BRKGA::ControlParams
<doxid-class_b_r_k_g_a_1_1_control_params>``` (which was renamed from
``ExternalControlParams``). In doing so, we have a consistent set that can be
fully loaded from configuration files.

Not all parameters are required, and those not are set to their default values.
The new reading function ``:ref:`BRKGA::readConfiguration()
<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>```
will emit a warning when no-required parameters are not set.

.. warning::

    If you are using IPR, we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** since
    this is the core stopping criteria on IPR.


.. _doxid-index_new_on_version3_modernization:

Code modernizing and speed bump
-------------------------------------------------------------------------------

The code has been modernized using `C++20 facilities
<https://en.wikipedia.org/wiki/C%2B%2B20>`__ like concepts and ranges.
Therefore, your compiler must support C++20 now.

One notable change was substituting the custom code in ``randInt()`` for a
`standard library uniform distribution utility
<https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution>`__.
The old code was used when a custom Mersenne Twister RNG code was used (from
the original :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` implementation). The
inclusion of the Mersenne Twister in the standard library allows us to use all
default utilities. Ad hoc tests show that the standard library utilities are
faster than the old custom code. However, the speed-up is marginal when
considering the full application of :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`.
But, when we accumulate hundreds or thousands of calls daily, the time savings
can be considerable in a full year of operation (which usually translates into
energy savings).


.. _doxid-index_new_on_version2:

What is new on version 2.0
===============================================================================

In version 2.0, BRKGA-MP-IPR also deals with multiple objectives in a
lexicographical or priority dominance order. Differing from classical
non-dominance order (using Pareto frontiers), the lexicographical order defines
a *strict preference order among the objective functions.* This leads us to a
partial ordering of the values of the solutions (composed of several values,
each one from one objective function). So, we have the following definition
(abusing a little bit of notation).

.. admonition:: Definition

    Let :math:`A = (f_1, f_2, \ldots, f_n)` and
    :math:`A' = (f'_1, f'_2, \ldots, f'_n)`
    be two vectors for :math:`n` functions :math:`f_1, f_2, \ldots, f_n`.
    :math:`A` is lexicographical smaller than :math:`A'`, i.e.,
    :math:`A < A'` if and only if
    :math:`f_1 < f'_1`, or
    :math:`f_1 = f'_1` and :math:`f_2 < f'_2`, or
    :math:`\ldots, f_1 = f'_1, \ldots, f_{n-1} = f'_{n-1}`
    and :math:`f_n < f'_n`.

For instance, let's assume we have three minimizing objective functions and
four solutions described in the following table:

.. table::

    ======== =========== =========== ===========
    Solution :math:`f_1` :math:`f_2` :math:`f_3`
    ======== =========== =========== ===========
    A        50          30          30
    B        30          55          40
    C        30          20          50
    D        30          20          25
    ======== =========== =========== ===========

Note that Solution B is better than Solution A because :math:`f_1(A) < f_1(B),`
even though A has much better values for :math:`f_2` and :math:`f_3`. Now,
Solution C is better B because, although :math:`f_1(B) = f_1(C),` we have that
:math:`f_2(B) < f_2(C),` regardless of the value of :math:`f_3.` Solution D
has the best value for all objective functions. Therefore :math:`D < C < B <
A.`

.. warning::

    If you really want an algorithm to produce a **non-dominated set of
    solutions (Pareto frontier)**, this is **not** the right algorithm for you.
    We recommend taking a look at the `NSGA-II
    <https://doi.org/10.1109/4235.996017>`_ and `MOAB
    <https://en.wikipedia.org/wiki/MOEA_Framework>`_


.. _doxid-index_new_on_versio2_mating:

Multi-thread mating
-------------------------------------------------------------------------------

One of the nice additions to BRKGA-MP-IPR 2.0 is the capability of performing
the mating in parallel. Such capability speeds up the algorithm substantially,
mainly for large chromosomes and large populations. However, when performing
parallel mating, we have some points regarding reproducibility described in
Section `Multi-thread mating
<https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#multi-threading>`__
in the `tutorial <https://ceandrade.github.io/brkga_mp_ipr_cpp>`__.


.. _doxid-index_new_on_versio2_api:

API changes
-------------------------------------------------------------------------------

.. _doxid-index_new_on_versio2_api_fitness:

New type ``BRKGA::fitness_t``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Due to the inclusion of multi-objective optimization capabilities, the user now
must define a type ``:ref:`BRKGA::fitness_t
<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```, and his/her
decoder must return such a type. The user can do things like this:

Please, check for details in Sections `"TL;DR - Multi objective"
<https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#tl-dr-multi-objective>`__
and `"Using BRKGA-MP-IPR on multi-objective mode"
<https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#using-brkga-mp-ipr-on-multi-objective-mode>`__
from the `tutorial <https://ceandrade.github.io/brkga_mp_ipr_cpp>`__.


.. _doxid-index_new_on_versio2_api_setinitpop:

New ``BRKGA_MP_IPR::setInitialPopulation()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the previous versions, ``:ref:`BRKGA::BRKGA_MP_IPR::setInitialPopulation()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```
fills up only the first population, discarding additional warm solutions if the
population is full. Now, ``setInitialPopulation()`` fills up all populations.
`More details here.
<https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a59b05650ede92f5e0107ab606ff6e8b7>`__


.. _doxid-index_new_on_versio2_api_inject_chr:

Changes in ``BRKGA_MP_IPR::injectChromosome()``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``:ref:`BRKGA::BRKGA_MP_IPR::injectChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>```
does not accept initial fitness value anymore. Now, ``injectChromosome()``
triggers the decoder in all cases (and therefore, time must be measured on its
call). `More details here.
<https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a0347f67b59bfe36856d1c27c95d4b151>`__


.. _doxid-index_new_on_versio2_api_cpp17:

Pump to C++ 17
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

BRKGA-MP-IPR now uses some features of `C++17 standards
<https://en.wikipedia.org/wiki/C%2B%2B17>`__. Therefore, you must change your
building tools to support that.


.. _doxid-index_new_on_versio2_bugfix:

Bug fixes
-------------------------------------------------------------------------------

* ``:ref:`BRKGA::BrkgaParams::mutants_percentage
  <doxid-class_b_r_k_g_a_1_1_brkga_params_1a29f0ff6ae4506e2e34ac7bfd633a802c>```
  should be of type ``double`` not ``unsigned`` (`issue #1
  <https://github.com/ceandrade/brkga_mp_ipr_cpp/issues/1>`__). Thanks
  ``@sohaibafifi``.

* Fix the shorter method call for ``BRKGA::BRKGA_MP_IPR::pathRelinking()``
  (`pull #1 <https://github.com/ceandrade/brkga_mp_ipr_cpp/pull/2>`__). Thanks
  ``@afkummer``.


.. _doxid-index_license:

License and Citing
===============================================================================

BRKGA-MP-IPR uses a permissive BSD-like license and it can be used as it
pleases you. And since this framework is also part of an academic effort, we
kindly ask you to remember to cite the originating paper of this work.
Indeed, Clause 4 estipulates that "all publications, softwares, or any other
materials mentioning features or use of this software (as a whole package or
any parts of it) and/or the data used to test it must cite the following
article explicitly":

.. container:: colored-container

    C.E. Andrade, R.F. Toso, J.F. Gonçalves, M.G.C. Resende. The Multi-Parent
    Biased Random-key Genetic Algorithm with Implicit Path Relinking. *European
    Journal of Operational Research*, volume 289, number 1, pages 17–30, 2021.
    DOI:
    `10.1016/j.ejor.2019.11.037 <https://doi.org/10.1016/j.ejor.2019.11.037>`_.

If you are using the multi-objective version, you must also cite this paper:

.. container:: colored-container

    C.E. Andrade, L.S. Pessoa, S. Stawiarski. The Physical Cell Identity
    Assignment Problem: a Multi-objective Optimization Approach.
    *IEEE Transactions on Evolutionary Computation*, to appear, 2022.
    DOI:
    `10.1016/j.ejor.2019.11.037 <https://doi.org/10.1016/j.ejor.2019.11.037>`_.

You may also consider to cite the following papers from people that helped
to find bugs and develop new features for BRKGA-MP-IPR 2.0:

.. container:: colored-container

    A.F. Kummer N., L.S. Buriol, O.C.B. de Araujo. A biased random key genetic
    algorithm applied to the VRPTW with skill requirements and synchronization
    constraints. *Proceedings of the 2020 Genetic and Evolutionary Computation
    Conference (GECCO '20)*, pages 717-724, 2020.
    DOI:
    `10.1145/3377930.3390209 <https://doi.org/10.1145/3377930.3390209>`_.

You can download all references for this
:download:`Bibtex <../assets/references.bib>`, or this
:download:`RIS <../assets/references.ris>` files


.. _doxid-index_collaborators:

Collaborators
===============================================================================

* Alberto Kummer, 2021 (parallel mating).

* Daniele Ferone, 2023 (bug fix on IPR).


.. _doxid-index_logo:

About the logo
===============================================================================

The logo is just a play with 3 chromosomes crossing with each other
(multi-parent) during the mating process. The lines also represent solutions
paths that encounter with each other generating new solutions during the
path-relink.
