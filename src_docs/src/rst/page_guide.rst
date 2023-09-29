.. index:: pair: page; Guide / Tutorial
.. _doxid-guide:

*******************************************************************************
Guide / Tutorial
*******************************************************************************

This tutorial is based on the single objective function usage of BRKGA.
However, almost all information also applies to the multi-objective mode with
minimal changes, as explained in Section
:ref:`Using BRKGA-MP on multi-objective mode <doxid-guide_guide_multi_obj>`.

If you desire to understand the deeps of the multi-parenting and the implicity
path relink, read :cite:p:`Andrade2021:BRKGA_MP_IPR`.


.. _doxid-guide_installation:

Installation
===============================================================================

BRKGA-MP-IPR is a header-only framework, which means that you only need to
download the header files and tell your compiler to include the path to where
the files were downloaded.

Quick example (unix): assume we are in an empty folder. So, we copy/clone
BRKGA-IPR-MP first:

.. code-block::

    $ git clone --depth 1 https://github.com/ceandrade/brkga_mp_ipr_cpp
    Cloning into 'brkga_mp_ipr_cpp'...
    remote: Enumerating objects: 283, done.
    remote: Counting objects: 100% (283/283), done.
    remote: Compressing objects: 100% (222/222), done.
    remote: Total 283 (delta 86), reused 180 (delta 55), pack-reused 0
    Receiving objects: 100% (283/283), 6.77 MiB | 5.88 MiB/s, done.
    Resolving deltas: 100% (86/86), done.

Let's write a ``test.cpp`` with the following code:

.. code-block:: cpp
    :linenos:

    #include "brkga_mp_ipr.hpp"
    #include <iostream>

    int main() {
        std::cout << "Testing sense: " << BRKGA::Sense::MINIMIZE;
        return 0;
    }

Then, let's compile with `GCC <https://gcc.gnu.org>`__ and see it works:

.. code-block::

    $ g++ --version
    g++ (MacPorts gcc12 12.3.0_0+stdlib_flag) 12.3.0
    Copyright (C) 2022 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    $ g++ -std=c++20 -Ibrkga_mp_ipr_cpp/brkga_mp_ipr test.cpp -o test

    $ ./test
    Testing sense: MINIMIZE

With `Clang <https://clang.llvm.org>`__ (note that we have to use the flag
``-fopenmp``, otherwise we get compilation errors):

.. code-block::

    $ clang++ --version
    clang version 16.0.6
    Target: x86_64-apple-darwin22.6.0
    Thread model: posix
    InstalledDir: /opt/local/libexec/llvm-16/bin

    $ clang++ -std=c++20 -fopenmp -Ibrkga_mp_ipr_cpp/brkga_mp_ipr test.cpp -o test

    $ ./test
    Testing sense: MINIMIZE

Note the Git clones the whole repository that contains the library code,
documents, and examples. All the examples were built using `GNU/Make
<https://www.gnu.org/software/make>`_, and `GCC toolchain
<https://gcc.gnu.org>`_ or `LLVM/Clang toolchain <https://clang.llvm.org>`__.
However, the code is standard C++, and we can
quickly adapt it to other toolchains such as `Microsoft
<https://visualstudio.microsoft.com>`_, or `Intel
<https://software.intel.com/en-us/c-compilers>`_ toolchains.


.. _doxid-guide_guide_tldr_single_obj:

TL;DR - Single objective
===============================================================================

The best way to keep it short is to look in the on examples on `the git repo.
<https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples>`_
Let's start solving the traditional single-objective `Traveling Salesman
Problem (TSP) <https://en.wikipedia.org/wiki/Travelling_salesman_problem>`_.
First, we must ensure that
``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
has the right single-object type. Let's
take a look at the trimmed version of
`fitness_type.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/fitness_type.hpp>`_.
This is a trimmed copy:

.. code-block:: cpp
    :linenos:

    namespace BRKGA {

    using fitness_t = double;

    //...
    } // end namespace BRKGA_MP_IPR

Here,
``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
defines the type of the objective function value. In the vast
majority of the cases, ``double`` suffices. Let's take a look into the main call
`main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_minimal.cpp>`_.
This is a trimmed copy:

.. code-block:: cpp
    :linenos:

    int main(int argc, char* argv[]) {
        if(argc < 4) {
            cerr
            << "Usage: " << argv[0]
            << " <seed> <config-file> <maximum-running-time>"
            << " <tsp-instance-file>"
            << endl;
            return 1;
        }

        try {
            // Read command-line arguments and the instance
            const unsigned seed = stoi(argv[1]);
            const string config_file = argv[2];
            const string instance_file = argv[4];
            const unsigned num_threads = 4;

            cout << "Reading data..." << endl;
            auto instance = TSP_Instance(instance_file);

            // Read algorithm parameters
            cout << "Reading parameters..." << endl;

            auto [brkga_params, control_params] =
                BRKGA::readConfiguration(config_file);

            // Overwrite the maximum time from the config file.
            control_params.maximum_running_time = chrono::seconds {stoi(argv[3])};

            // Build the BRKGA data structure
            cout << "Building BRKGA data and initializing..." << endl;

            TSP_Decoder decoder(instance);

            BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
                decoder, BRKGA::Sense::MINIMIZE, seed,
                instance.num_nodes, brkga_params, num_threads
            );

            // Find good solutions / evolve
            cout << "Running for " << control_params.maximum_running_time << "..."
                 << endl;

            const auto final_status = algorithm.run(control_params, &cout);

            cout
            << "\nAlgorithm status: " << final_status
            << "\n\nBest cost: " << final_status.best_fitness
            << endl;
        }
        catch(exception& e) {
            cerr
            << "\n" << string(40, '*') << "\n"
            << "Exception Occurred: " << e.what()
            << "\n" << string(40, '*')
            << endl;
            return 1;
        }
        return 0;
    }

You can identify the following basic steps:

#. Create a data structure to hold your input data. This object should be
   passed to the decoder object/functor (example
   `tsp/tsp_instance.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/tsp/tsp_instance.hpp>`_);

#. Certify that
   ``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
   has the correct type;

#. Implement a decoder object/functor. This function translates a chromosome
   (array of numbers in the interval [0, 1)) to a solution for your problem.
   The decoder must return the solution value or cost to be used as fitness by
   BRKGA (example `decoders/tsp_decoder.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/decoders/tsp_decoder.hpp>`_);

#. Load the instance and other relevant data;

#. Read the algorithm parameters using
   ``:ref:`BRKGA::readConfiguration() <doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>```;
   or create
   ``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` and
   ``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
   objects by hand;

#. Create an ``:ref:`BRKGA::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` algorithm object;

#. Call ``:ref:`BRKGA::BRKGA_MP_IPR::run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
   to optimize;

#. Check the resulting ``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
   for optimization information.

`main_minimal.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_minimal.cpp>`__
provides a very minimal example to understand the necessary steps to use the
BRKGA-MP-IPR framework. However,
`main_complete.cpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_complete.cpp>`_
provides a full-featured code, handy for scientific use, such as
experimentation and paper writing. This code allows fine-grained control of
the optimization, shows several features of BRKGA-MP-IPR such as the resets,
chromosome injection, and others. It also logs
all optimization steps, *creating outputs easy to be parsed.* **You should use
this code for serious business and experimentation.**

These are the basic steps, but I do recommend the reading of this guide.


.. _doxid-guide_guide_tldr_multi_obj:

TL;DR - Multi objective
===============================================================================

.. warning::

    Remember, BRKGA-MP-IPR multi-objective mode produces **lexicographical
    dominated solutions** but **no non-dominated solutions** (Pareto frontier).
    Please, see the details in the :ref:`introduction <doxid-indexpage>`.


To use BRKGA-MP-IPR in the multi-objective mode, we first must set
``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
according to the number of objectives we want. In the
`repo example, <https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples/tsp/src/multi_obj>`_
we consider the TSP with two objectives: first, we must minimize the total tour
length, and second, the size of the largest edge in the tour. For that, we must
change the file
`fitness_type.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/fitness_type.hpp>`_
to reflect such a thing. In this example, we use the standard
`std::tuple <https://en.cppreference.com/w/cpp/utility/tuple>`_.

.. code-block:: cpp
    :linenos:

    namespace BRKGA {

    using fitness_t = std::tuple<double, double>;

    //...
    } // end namespace BRKGA_MP_IPR

In this case, the first component of the tuple holds the tour length, and the
second contains the largest edge. On Section
:ref:`Using BRKGA-MP on multi-objective mode <doxid-guide_guide_multi_obj>`,
we talk with more details about multi-objective problems. Just keep in mind,
although you could use any type for your `fitness_t`, you should prefer to use
`std::tuple <https://en.cppreference.com/w/cpp/utility/tuple>`_.

The remaining code is almost identical to the single-objective. The only
differences are in computing the largest edge, and printing such information on
the main call. All the steps described briefly in the
:ref:`previous section <doxid-guide_guide_tldr_single_obj>`
are also used here.


.. _doxid-guide_getting_started:

Getting started
===============================================================================

BRKGA-MP-IPR is pretty simple, and you must provide one required *decoder*
object to translate chromosomes to solutions. In general, such decoder uses
the problem information to map a vector of real numbers in the interval [0, 1)
to a (valid) solution. In some cases, even though a valid solution cannot be
found, library users apply penalization factors and push the BRKGA to find
valid solutions.

Before you go further, please take a look at the ``examples`` folder in `the
git repo <https://github.com/ceandrade/brkga_mp_ipr_cpp>`_.
Until version 2.0, we maintained a folder with code to solve
combinatorial auction problems :cite:p:`Andrade2015:BRKGA_CA`.
But due to maintanance burden,
we drop the that code from the repo. However, the classical
`Traveling Salesman Problem (TSP)
<https://en.wikipedia.org/wiki/Travelling_salesman_problem>`_ was kept.
In the TSP, we have a set of cities and the distances between them (full
weighted undirect graph). One must find a minimum-cost tour among all cities,
such that each city is visited only once (i.e., find a Hamiltonian cycle of
minimum cost). The folder has the following structure:

* ``src`` subdir: contains all the code;

* ``instances`` subdir: folder containing some TSP instances for testing;

The ``src`` subdir contains all the code to solve TSP both for single and
multi-objective. This is its structure:

* ``tsp`` subdir: contains code to load and build data for TSP;

* ``decoders`` subdir: contains the TSP decoder;

* ``heuristics`` subdir: contains a simple heuristic that computes a greedy
  tour;

* ``main_minimal.cpp`` file: minimal code useful to understand and test the
  framework. **You should start here!** Please take a look on this file before
  continue this tutorial;

* ``main_complete.cpp`` file: full-featured code, handy for scientific use,
  such as experimentation and paper writing. This code allows fine-grained
  control of the optimization, shows several features of BRKGA-MP-IPR such as
  the path-relinking calls, resets, chromosome injection, and others. It also
  logs all optimization steps, *creating outputs easy to be parsed.*
  **You should use this code for serious business and experimentation;**
  Note that this version was much simplified from version 2.0. Still, it offers
  a lot of control for experimentation through parameter tuning. However,
  it is still possible to decompose it by inspecting the code on method
  ``:ref:`BRKGA::BRKGA_MP_IPR::run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```.

* ``config.conf`` file: example of parameter settings;

* ``Makefile`` file: the makefile used to build the executables;

* ``third_part`` subdir: contains the `docopt <https://github.com/docopt/docopt.cpp>`_
  dependence for ``main_complete.cpp``. This is not strictly necessary for
  BRKGA-MP-IPR, but it adds a nice command line interface. If you have problems
  with that, we can change ``main_complete.cpp`` using traditional argument
  handling, or another library you prefer.

The first step is to build the code. Here, we are using `GNU/Make
<https://www.gnu.org/software/make>`__ and `GCC toolchain
<https://gcc.gnu.org>`_. You may change for the toolchain of your choice.
You may need to edit this file according to your compiler version and
settings. The first thing to note in the makefile is the parameter ``OPT``
that, when set ``OPT=opt``, it turns on aggressive optimization flags (for
G++). If the flag is not set, aggressive debug options are set. For serious
implementations, and debugging, we do recommend to use such setup. However,
if your compiler does not like such settings, please, go ahead and change
accordingly. By typing just ``make``, you build both minimal and complete
versions. We also can just type ``make main_minimal`` or ``make
main_complete`` to build one or other version independently.
Typing ``make clean`` will clean up the compilation.

When you call the executables ``main_minimal`` or ``main_complete`` without
arguments, they show the usage. For example, assuming you are using a
terminal:

.. code-block::

    $./main_minimal
    Usage: ./main_minimal <seed> <config-file> <maximum-running-time> <tsp-instance-file>

    $ ./main_complete
    Arguments did not match expected patterns

    Usage:
      main_complete
            --config <config_file>
            --seed <seed>
            --stop_rule <stop_rule>
            --stop_arg <stop_arg>
            --maxtime <max_time>
            --instance <instance_file>
            [--threads <num_threads>]
            [--no_evolution]
      main_complete (-h | --help)

    Options:
      --config <arg>     Text file with the BRKGA-MP-IPR parameters.
      --seed <arg>       Seed for the random number generator.
      --stop_rule <arg>  Stop rule where:
                         - (G)enerations: number of evolutionary generations.
                         - (I)terations: maximum number of generations without
                           improvement in the solutions.
      --stop_arg <arg>     Argument value for the stopping rule.
      --maxtime <arg>      Maximum time in seconds.
      --instance <arg>     Instance file.
      --threads <arg>    Number of threads to be used during parallel decoding.
                         It must in the range [1, 64] [default: 1].
      --no_evolution     If supplied, no evolutionary operators are applied.
                         So, the algorithm becomes a simple multi-start algorithm.
      -h --help          Produce help message.

So, this is a possible output whe calling ``main_minimal`` :

.. code-block::

    $ ./main_minimal 27000001 config.conf 100 ../instances/brazil58.dat
    Reading data...
    Reading parameters...
    Building BRKGA data and initializing...
    Running for 30s...
    Custom stopping criteria not supplied by the user. Using max. time = 30s and max. stall_offset = 1000
    Using 4 threads for decoding
    Exchanged 1 solutions from each population. Iteration 432. Current time: 7.88959s
    Exchanged 1 solutions from each population. Iteration 556. Current time: 10.1435s
    Exchanged 1 solutions from each population. Iteration 735. Current time: 13.1668s
    Path relink at 835 iteration. Block size: 45. Type: DIRECT. Distance: KENDALLTAU. Current time: 13.1668s
    - No improvement found. Current time: 14.7002s
    Exchanged 1 solutions from each population. Iteration 835. Current time: 14.7006s
    Exchanged 1 solutions from each population. Iteration 935. Current time: 16.3598s
    Shaking with intensity 0.70553. Type SWAP. Iteration 935. Current time: 16.3598s
    Path relink at 1035 iteration. Block size: 45. Type: DIRECT. Distance: KENDALLTAU. Current time: 16.3598s
    - Improvement on the elite set but not in the best individual. Current time: 18.0657s
    Exchanged 1 solutions from each population. Iteration 1035. Current time: 18.0661s
    Exchanged 1 solutions from each population. Iteration 1135. Current time: 20.1444s
    Reset population after 500 iterations without improvement. Iteration 1135. Current time: 20.1551s
    Path relink at 1235 iteration. Block size: 45. Type: DIRECT. Distance: KENDALLTAU. Current time: 20.1551s
    - No improvement found. Current time: 22.0121s
    Exchanged 1 solutions from each population. Iteration 1235. Current time: 22.0125s
    Shaking with intensity 0.635298. Type SWAP. Iteration 1235. Current time: 22.0125s
    Exchanged 1 solutions from each population. Iteration 1335. Current time: 23.5627s
    Path relink at 1435 iteration. Block size: 45. Type: DIRECT. Distance: KENDALLTAU. Current time: 23.5627s
    - No improvement found. Current time: 25.0922s
    Exchanged 1 solutions from each population. Iteration 1435. Current time: 25.0926s
    Exchanged 1 solutions from each population. Iteration 1535. Current time: 26.6116s
    Shaking with intensity 0.447978. Type SWAP. Iteration 1535. Current time: 26.6116s

    Algorithm status:
    best_fitness: 27895
    current_iteration: 1635
    last_update_iteration: 635
    current_time: 28.1722s
    last_update_time: 11.6496s
    largest_iteration_offset: 159
    stalled_iterations: 1000
    path_relink_time: 0.076028s
    num_path_relink_calls: 4
    num_homogenities: 0
    num_best_improvements: 0
    num_elite_improvements: 1
    num_exchanges: 11
    num_shakes: 3
    num_resets: 1

    Best cost: 27895

For ``main_complete``, the output is more verbose, since we want to capture
as much information as possible to do some statistical analysis. The output
should be something close to this:

.. code-block::

    $./main_complete --config config.conf --seed 2700001 --stop_rule I \
        --stop_arg 1000 --maxtime 30 --threads 4 --instance ../../instances/brazil58.dat

    [Tue Sep 26 22:13:30 2023] Experiment started
    > Instance: '../../instances/brazil58.dat'
    > Loading config file: 'config.conf'
    > Algorithm parameters:
    population_size 2000
    elite_percentage 0.30
    mutants_percentage 0.15
    num_elite_parents 2
    total_parents 3
    bias_type LOGINVERSE
    num_independent_populations 3
    pr_number_pairs 0
    pr_minimum_distance 0.15
    pr_type DIRECT
    pr_selection BESTSOLUTION
    pr_distance_function_type KENDALLTAU
    alpha_block_size 1.00
    pr_percentage 1.00
    num_exchange_individuals 1
    shaking_type SWAP
    shaking_intensity_lower_bound 0.25
    shaking_intensity_upper_bound 0.75
    > Control parameters:
    maximum_running_time 30s
    exchange_interval 100
    shake_interval 300
    ipr_interval 200
    reset_interval 500
    stall_offset 1000
    > Seed: 2700001
    > Stop rule: Improvement
    > Stop argument: 1000
    > Number of threads for decoding: 4

    [Tue Sep 26 22:13:30 2023] Reading TSP data
    Number of nodes: 58

    [Tue Sep 26 22:13:30 2023] Generating initial tour
    Initial cost: 30774.00

    [Tue Sep 26 22:13:30 2023] Building BRKGA
    New population size: 580
    Chromosome size: 58

    [Tue Sep 26 22:13:30 2023] Injecting initial solution

    [Tue Sep 26 22:13:30 2023] Optimizing...
    * Iteration | Cost | CurrentTime
    Custom stopping criteria not supplied by the user. Using max. time = 30s and max. stall_offset = 1000
    Using 4 threads for decoding
    * 1 | 30774.00 | 0.00s
    * 46 | 30365.00 | 0.12s
    * 47 | 29956.00 | 0.12s
    * 53 | 29618.00 | 0.14s
    * 54 | 29343.00 | 0.14s
    * 71 | 29332.00 | 0.19s
    * 115 | 29304.00 | 0.31s
    * 145 | 29215.00 | 0.39s
    * 156 | 29206.00 | 0.42s
    * 167 | 29172.00 | 0.44s
    Exchanged 1 solutions from each population. Iteration 267. Current time: 0.71s
    * 364 | 29060.00 | 0.95s
    * 370 | 28910.00 | 0.96s
    Exchanged 1 solutions from each population. Iteration 470. Current time: 1.22s
    * 474 | 28865.00 | 1.23s
    * 477 | 28859.00 | 1.24s
    * 554 | 28773.00 | 1.43s
    * 596 | 28763.00 | 1.54s
    * 647 | 28699.00 | 1.67s
    * 675 | 28671.00 | 1.74s
    * 746 | 28585.00 | 1.91s
    * 760 | 28575.00 | 1.95s
    Exchanged 1 solutions from each population. Iteration 860. Current time: 2.21s
    * 932 | 28301.00 | 2.48s
    Exchanged 1 solutions from each population. Iteration 1032. Current time: 2.81s
    * 1080 | 28075.00 | 2.98s
    * 1113 | 27945.00 | 3.12s
    * 1165 | 27709.00 | 3.30s
    * 1181 | 27571.00 | 3.35s
    * 1184 | 27352.00 | 3.36s
    * 1209 | 27294.00 | 3.44s
    * 1220 | 27289.00 | 3.48s
    Exchanged 1 solutions from each population. Iteration 1320. Current time: 3.79s
    Path relink at 1420 iteration. Block size: 25. Type: DIRECT. Distance: KENDALLTAU. Current time: 3.79s
    - No improvement found. Current time: 4.09s
    Exchanged 1 solutions from each population. Iteration 1420. Current time: 4.09s
    Exchanged 1 solutions from each population. Iteration 1520. Current time: 4.42s
    Shaking with intensity 0.65. Type SWAP. Iteration 1520. Current time: 4.42s
    Path relink at 1620 iteration. Block size: 25. Type: DIRECT. Distance: KENDALLTAU. Current time: 4.42s
    - No improvement found. Current time: 4.73s
    Exchanged 1 solutions from each population. Iteration 1620. Current time: 4.73s
    Exchanged 1 solutions from each population. Iteration 1720. Current time: 5.04s
    Reset population after 500 iterations without improvement. Iteration 1720. Current time: 5.04s
    Path relink at 1820 iteration. Block size: 25. Type: DIRECT. Distance: KENDALLTAU. Current time: 5.04s
    - No improvement found. Current time: 5.36s
    Exchanged 1 solutions from each population. Iteration 1820. Current time: 5.36s
    Shaking with intensity 0.73. Type SWAP. Iteration 1820. Current time: 5.36s
    Exchanged 1 solutions from each population. Iteration 1920. Current time: 5.70s
    Path relink at 2020 iteration. Block size: 25. Type: DIRECT. Distance: KENDALLTAU. Current time: 5.70s
    - No improvement found. Current time: 5.98s
    Exchanged 1 solutions from each population. Iteration 2020. Current time: 5.98s
    Exchanged 1 solutions from each population. Iteration 2120. Current time: 6.26s
    Shaking with intensity 0.26. Type SWAP. Iteration 2120. Current time: 6.26s

    [Tue Sep 26 22:13:37 2023] End of optimization

    > Final status:
    best_fitness: 27289.00
    current_iteration: 2220
    last_update_iteration: 1220
    current_time: 6.54s
    last_update_time: 3.48s
    largest_iteration_offset: 197
    stalled_iterations: 1000
    path_relink_time: 0.01s
    num_path_relink_calls: 4
    num_homogenities: 0
    num_best_improvements: 0
    num_elite_improvements: 0
    num_exchanges: 13
    num_shakes: 3
    num_resets: 1

    % Best tour cost: 27289
    % Best tour: 21 7 0 29 12 39 24 8 31 19 52 49 3 17 43 23 57 4 26 42 11 56 22 54 53 1 47 40 34 9 51 50 46 48 2 20 35 16 25 18 5 27 13 36 14 33 45 55 44 32 28 38 10 15 41 30 6 37

    Instance,Seed,Cost,NumNodes,TotalIterations,LastUpdateIteration,TotalTime,LastUpdateTime,LargestIterationOffset,StalledIterations,PRTime,PRCalls,PRNumHomogenities,PRNumPrImprovBest,PRNumImprovElite,NumExchanges,NumShakes,NumResets
    brazil58,2700001,27289,58,2220,1220,6.54,3.48,197,1000,0.01,4,0,0,0,13,3,1

Note that your can extract only the last line (e.g., using ``tail -n1``)
from the log, and add it to a table in a CSV file. In this way, you can load
such table in your favorite statistic tools.

I hope by now you got your system set up and running. Let's see the essential
details on how to use the BRKGA-MP-IPR.


.. _doxid-guide_decoder_development:

First things first
===============================================================================


.. _doxid-guide_decoder_function:

The decoder function
-------------------------------------------------------------------------------

The core of the BRKGA algorithm is the definition of a decoder
function/object. The decoder maps the chromosomes (vectors of real numbers in
the interval [0, 1)) to solutions of the problem. In some sense, a decoder is
similar to a `kernel function from Support Vector Machines
<https://en.wikipedia.org/wiki/Kernel_method>`_: both functions are used to
project solutions/distances in different spaces.

Here, we have a small difference between the C++/Python and the Julia
implementations. In the Julia version, you must define a data container
inherit from `AbstractInstance
<https://ceandrade.github.io/BrkgaMpIpr.jl/guide/#First-things-first:-basic-data-structures-and-decoder-function-1>`_,
and a decoder function. The reason you must do that is because structs in
Julia have no methods (but constructors), and the decoder function must take
both chromosome and input data in the call. In C++/Python, we can encapsulate the
input data into the decoder object, resulting in a much more clear API.

The basic form of a decoder should be:

.. code-block:: cpp
    :linenos:

    class Decoder {
    public:
        BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool rewrite);
    };

The decoder **must** contain a public **decode()** method that receives a
``:ref:`BRKGA::Chromosome <doxid-namespace_b_r_k_g_a_1a8ae7fc2da08d2d93a0628f346e72fab6>```
reference and an ``boolean``, and returns a
``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```.
But before going further, let's talk about the chromosome.


.. _doxid-guide_the_chromosome:

The chromosome or vector of doubles
-------------------------------------------------------------------------------

Note that all long the BRKGA discussion, the chromosome is represented as a
vector of real numbers in the interval [0, 1). Indeed, we could use
straightforward ``std::vector<double>``. However, sometimes is interesting to
keep more information inside the chromosome for further analysis, such as,
other solution metrics that not the main fitness value. For example, in a
scheduling problem, we may choose to keep both makespan and total completion
time metrics. Therefore, we chose to make the chromosome a "generic" data
structure in our design.

File
`chomosome.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/chromosome.hpp>`_
shows the basic represetation of a chromosome:

.. code-block:: cpp

    using Chromosome = std::vector<double>;

If this enough for you, you go already and use such a definition.
**We do recommend to import and use the definition from**
`chomosome.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/chromosome.hpp>`_,
instead to redefine in your own code, since it is the same definition
the main BRKGA-MP-IPR algorithm uses.

However, if you need more information to be tracked during the optimization,
you can redefine the chromosome. First, your definition must complain with
the ``std::vector`` interface. The easiest way to do that is to inherit
from the ``std::vector``. For instance, assume we want to keep track of the
makespan and the total completion time for a scheduling problem. We can do
the following:

.. code-block:: cpp
    :linenos:

    class Chromosome: public std::vector<double> {
    public:
        Chromosome() :
            std::vector<double>(), makespan(0.0), total_completion_time(0.0)
            {}

        Chromosome(unsigned _size, double _value = 0.0)
            std::vector<double>(_size, value),
            makespan(0.0), total_completion_time(0.0)
            {}

        Chromosome(const Chromosome& chr) = default;

    public:
        double makespan;
        double total_completion_time;
    };

In general, most people do not recommend to inherit publicly from
``std::vector`` because it has no virtual destructor. However, we may do
that as long as:

a) We remember that every operation provided by ``std::vector`` must be a
   semantically valid operation on an object of the derived class;

b) We avoid creating derived class objects with dynamic storage duration;

c) We **DO AVOID** polymorphism:

.. code-block:: cpp
    :linenos:

    std::vector<double>* pt = new Chromosome();     // Bad idea. Don't do that!
    delete pt;      // Delete does not call the Chromosome destructor.


.. _doxid-guide_back_to_the_decoder:

Back to the decoder
-------------------------------------------------------------------------------

Again, **the decoder is the heart of a BRKGA.** An easy way to keep the API clean
is to define a decoder that has a reference for the input data. This is a TSP
decoder defined on file `decoders/tsp_decoder.hpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/decoders/tsp_decoder.hpp>`__:

.. code-block:: cpp
    :linenos:

    #include "tsp/tsp_instance.hpp"
    #include "brkga_mp_ipr/fitness_type.hpp"
    #include "brkga_mp_ipr/chromosome.hpp"

    class TSP_Decoder {
    public:
        TSP_Decoder(const TSP_Instance& instance);
        BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool rewrite);

    public:
        const TSP_Instance& instance;
    };

Note that ``TSP_Decoder`` get a const reference to ``TSP_Instance``, that holds
the input data. Therefore, ``TSP_Decoder`` has direct access to the data for
optimization. This approach also benefits cache efficiency, mainly when
multiple threads are used for decoding, i.e., several threads can use the same
read-only data already in the cache, which speeds up the optimization.

The decode method also has a ``rewrite`` argument that indicates if the decoder
should rewrite the chromosome, in case of local search / local improvements be
performed during the decoder process. This flag is critical if you intend to
use the Implicit Path Relink (details on
``:ref:`BRKGA::BRKGA_MP_IPR::pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>```
Even though you do not rewrite the chromosome in your decoder, you must provide
such signature for API compatibility.

The decoder must return a
``:ref:`BRKGA::fitness_t <doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
that is used as the **fitness** to rank the chromosomes. In general, fitness is
the cost/value of the solution, but you may want to use it to penalize
solutions that violate the problem constraints, for example.

In our TSP example, we have a very simple decoder that generates a permutation
of nodes, and compute the cost of the cycle from that permutation
(note that we don't use the flag ``rewrite`` in this example):

.. code-block:: cpp
    :linenos:

    BRKGA::fitness_t TSP_Decoder::decode(Chromosome& chromosome, bool /* not-used */) {
        vector<pair<double, unsigned>> permutation(instance.num_nodes);
        for(unsigned i = 0; i < instance.num_nodes; ++i)
            permutation[i] = make_pair(chromosome[i], i);

        sort(permutation.begin(), permutation.end());

        BRKGA::fitness_t cost = instance.distance(permutation.front().second,
                                                  permutation.back().second);

        for(unsigned i = 0; i < instance.num_nodes - 1; ++i)
            cost += instance.distance(permutation[i].second,
                                      permutation[i + 1].second);
        return cost;
    }

With the instance data and the decoder ready, we can build the BRKGA data
structures and perform the optimization.

.. warning::

    When using multiple threads, **you must guarantee that the decoder is
    thread-safe.** You may want to create all read-write data structures on each
    call or create a separate storage space for each thread. Section
    :ref:`Multi-thread decoding <doxid-guide_guide_tips_multi_thread_decoding>`
    brings some tips.

.. warning::

    **The decoder must be a function,** i.e., given a chromosome, it must output
    the same solution/fitness in any call. In other words, the decoder must be a
    deterministic (or, at most, pseudo-random) procedure.

Indeed, this is an essential aspect of the decoder: **it must produce the exact
solution for the same chromosome.** If the decoder cannot do it, we will see a
substantial degradation in the BRKGA performance regarding convergence. BRKGA
cannot learn well with non-deterministic decoders. Moreover, non-deterministic
decoders do not allow reproducibility, impairing their utility for production
and academic environments.

However, there are several situations where we must toss a coin to break a tie.
In this case, we must guarantee that such a coin always results in the same
sequence of values for a given chromosome. In other words, we must ensure that
our decoder is pseudo-random or pseudo-non-deterministic. We could create a
Random Number Generator (RNG) inside each decoding call with a fixed seed. But
this strategy may not explorer the solution space as needed since the seed is
the same for all decoding.

We can use several strategies to mitigate such situations, but the most used is
to create an (n+1)-sized chromosome such that one allele (in general, the first
or the last) is used as a seed to the RNG. In this way, the chromosome also
carries the information for breaking ties, and therefore, we can reproduce the
solution. This is an example:

.. code-block:: cpp
    :linenos:

    typedef std::mt19937::result_type seed_t;

    // This just reinterprets the bits as they are. This is the safest way to
    // guarantee reproducibility since we only use the bits. However, since we
    // are converting the range [0.0, 1.0] from a double, we may have a skewed
    // list of seeds. We are missing the integer part and negative numbers bits.
    // Still, for most applications, this should be good enough.
    auto seed1 = *(reinterpret_cast<seed_t*>(&chromosome[n]))

    // This version may grab all the seed's domain. However, we may face
    // numerical issues with precision here. In some cases, the same double may
    // generate two different seeds (depending on the platform), and we will
    // lose reproducibility. We only recommend using this if you really need a
    // very diverse set of seeds to generate millions of random numbers in the
    // decoder.
    auto seed2 = seed_t(numeric_limits<seed_t>::max() * chromosome[n]);

    // Just instantiate a local random number generator. Tip: this can hit your
    // performance. Better allocate the RNG before. If you use multiple threads,
    // please read the Section :ref:`Multi-thread decoding <doxid-guide_guide_tips_multi_thread_decoding>`.
    std::mt19937 my_local_rng(seed1);


.. _doxid-guide_brkga_object:

Building BRKGA-MP-IPR algorithm object
===============================================================================

``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` is the main object that
implements all BRKGA-MP-IPR algorithms such as evolution, path relink, and
other auxiliary procedures. Note that ``:ref:`BRKGA::BRKGA_MP_IPR
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>``` is a template
parametrized by the decoder type. This strategy avoids runtime polymorphism,
drastically improving the performance of the code.

The first step is to call the algorithm constructor that has the following
signature:

.. code-block:: cpp
    :linenos:

    BRKGA_MP_IPR(
        Decoder& decoder_reference,
        const Sense sense,
        const unsigned seed,
        const unsigned chromosome_size,
        const BrkgaParams& params,
        const unsigned max_threads = 1,
        const bool evolutionary_mechanism_on = true
    );

The first argument is the decoder object that must implement the ``decode()``
method as discussed before. You also must indicate whether you are minimizing
or maximizing through parameter ``:ref:`BRKGA::Sense
<doxid-namespace_b_r_k_g_a_1af28538be111c8320b2fec44b77ec5e9b>```.

A good seed also must be provided for the (pseudo) random number generator
(according to :cite:p:`Matsumoto2007:Common_defects_rngs`).
BRKGA-MP-IPR uses the `Mersenne Twister
engine <https://en.wikipedia.org/wiki/Mersenne_Twister>`_
from the
`standard C++ library <http://www.cplusplus.com/reference/random/mt19937>`_.

The ``chromosome_size`` also must be given. It indicates the length of each
chromosome in the population. In general, this size depends on the instance and
how the decoder works. The constructor also takes a ``:ref:`BRKGA::BrkgaParams
<doxid-class_b_r_k_g_a_1_1_brkga_params>``` object that holds several
parameters. We will take about that later.

``max_threads`` defines how many threads the algorithm should use for decoding
and some other operations. As said before, **you must guarantee that the
decoder is thread-safe** when using two or more threads. See
:ref:`Multi-thread decoding <doxid-guide_guide_tips_multi_thread_decoding>`
for more information.

Another common argument is ``evolutionary_mechanism_on`` which is enabled by
default. When disabled, no evolution is performed. The algorithm only decodes
the chromosomes and ranks them. On each generation, all population is replaced
excluding the best chromosome. This flag helps on implementations of simple
multi-start algorithms.

All BRKGA and Path Relink hyper-parameters are stored in a
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` object.
Such objects can be read and write from plain text files or can be created by
hand by the user. There is also a companion
``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
object that stores extra control parameters that can be used outside the
BRKGA-MP-IPR to control several aspects of the optimization. For instance,
interval to apply path relink, reset the population, perform population
migration, among others. This is how a configuration file looks like (see
`config.conf
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/config.conf>`_
for a commented version):

.. code-block::
    :linenos:

    # BRKGA and IPR parameters
    population_size 2000
    elite_percentage 0.30
    mutants_percentage 0.15
    num_elite_parents 2
    total_parents 3
    bias_type LOGINVERSE
    num_independent_populations 3
    pr_number_pairs 0
    pr_minimum_distance 0.15
    pr_type DIRECT
    pr_selection BESTSOLUTION
    pr_distance_function_type KENDALLTAU
    alpha_block_size 1.0
    pr_percentage 1.0
    num_exchange_individuals 1
    shaking_type SWAP
    shaking_intensity_lower_bound 0.25
    shaking_intensity_upper_bound 0.75

    # Control parameters
    maximum_running_time 60
    exchange_interval 100
    ipr_interval 200
    shake_interval 300
    reset_interval 500
    stall_offset 100

To read this file, you can use the function
``:ref:`BRKGA::readConfiguration()
<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>```
which returns a
``:ref:`std::pair<https://en.cppreference.com/w/cpp/utility/pair>`<:ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>`, :ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>`>``.
When reading such
file, the function ignores all blank lines, and lines starting with ``#``. As
commented before,
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>```
contains all hyper-parameters regarding BRKGA and IPR methods and
``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
contains extra control parameters, and they are not mandatory to
the BRKGA-MP-IPR itself.

Let's take a look in the example from
`main_minimal.cpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_minimal.cpp>`__:

.. code-block:: cpp
    :linenos:

    const unsigned seed = stoi(argv[1]);
    const string config_file = argv[2];
    const string instance_file = argv[4];
    const unsigned num_threads = 4;

    auto instance = TSP_Instance(instance_file);

    auto [brkga_params, control_params] =
        BRKGA::readConfiguration(config_file);

    control_params.maximum_running_time = chrono::seconds {stoi(argv[3])};

    TSP_Decoder decoder(instance);

    BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
        decoder, BRKGA::Sense::MINIMIZE, seed,
        instance.num_nodes, brkga_params, num_threads
    );

This code gets some arguments from the command line and loads a TSP instance.
After that, it reads the BRKGA parameters from the configuration file.
Here, instead of using the maximum time given in the config file, we overwrite
it with the maximum time passed by the user through the command line. We then
build the decoder object, and the BRKGA algorithm. Since we are looking for
cycles of minimum cost, we ask for the algorithm ``MINIMIZE``. The starting
seed is also given. Since ``TSP_Decode`` considers each chromosome key as a
node/city, the length of the chromosome must be the number of nodes, i.e.,
``instance.num_nodes``. Finally, we also pass the BRKGA parameters.

Now, we have a
``:ref:`BRKGA::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>```
algorithm/object which will be used to call all other functions during the
optimization. Note that we can build several
``:ref:`BRKGA::BRKGA_MP_IPR <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r>```
objects using different parameters, decoders, or instance data. These
structures can be evolved in parallel and mixed-and-matched at your will. Each
one holds a self-contained BRKGA state including populations, fitness
information, and a state of the random number generator.


.. _doxid-guide_opt:

It's optimization time
===============================================================================

Until version 2.0, the user was responsible for creating the main optimization
loop. While this strategy gives fine control over the algorithm's flow, he/she
must call the BRKGA-MP-IPR features, such as IPR, shaking, population reset,
and others, individually. That generates cumbersome code, which usually takes a
lot of time for the developer to make it right.

In version 3.0, we abstract all these details, creating a single method
``:ref:`BRKGA_MP_IPR::run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
containing the complete optimization loop, which may use all the features
provided by this library. In this way, we provide a comprehensive and
easy-to-use single-entry point, like this:

.. code-block:: cpp

    const auto final_status = algorithm.run(control_params, &cout);

``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
takes a ``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
object which contains several control parameters in how the main loop behaves.
It is through these control parameters that the user can control the maximum
optimization time and when features like IPR, shaking, etc, are called.
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
also takes an output stream to log some information along the
optimization.

Once done,
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
returns a
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
object that brings all the details about the
optimization itself, such as the number of iterations, running time, number of
calls for each method, and others. Most importantly,
``:ref:`AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
also brings the fitness and the chromosome, representing the best solution
found during the optimization (note that it is not the best chromosome in the
current population because it may be fully reset and has lost the best
solution).

The main loop should be like this:

.. code-block:: cpp
    :linenos:

    while(!must_stop) {
        evolve(); // One generation.
        if(best solution improvement) {
            Save best solution;
            Call observer callbacks;
        }

        if(!must_stop && ipr_interval > 0 && stalled_iterations > 0 &&
           stalled_iterations % ipr_interval == 0) {
             pathRelink();
             if(best solution improvement) {
                 Save best solution;
                 Call observer callbacks;
             }
         }

        if(!must_stop && exchange_interval > 0 && stalled_iterations > 0 &&
           stalled_iterations % exchange_interval == 0) {
             exchangeElite();
        }

        if(!must_stop && shake_interval > 0 && stalled_iterations > 0 &&
           stalled_iterations % shake_interval == 0) {
             shake();
        }

        if(!must_stop && reset_interval > 0 && stalled_iterations > 0 &&
           stalled_iterations % reset_interval == 0) {
             reset();
        }
    }

Therefore, note that the order that
``:ref:`pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>```,
``:ref:`exchangeElite()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>```,
``:ref:`shake()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>```,
and
``:ref:`reset()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>```
are called, depends on the thresholds defined in
``:ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```.

For path relinking, the block size is computed by
:math:`\lceil \alpha \times \sqrt{p} \rceil` where
:math:`\alpha` is
``:ref:`BrkgaParams::alpha_block_size
<doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>```
and :math:`p` is
``:ref:`BrkgaParams::population_size
<doxid-class_b_r_k_g_a_1_1_brkga_params_1a8a4b0592480f32a3cf186b6d1759f571>```.
If the size is larger than the chromosome size, the size is set to half of the
chromosome size. For more details on that, refer to
``:ref:`pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>```.

.. note::

    The algorithm always test against maximum running time and for the maximum
    stalled iterations/generations given by
    ``:ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
    indenpendently of the stopping criteria function supplied by the user. This
    is especially important when activating the implicit path reliking which is
    **very timing consuming**. If you are using IPR, we **STRONGLY RECOMMEND TO
    SET A MAXIMUM TIME** since this is the core stopping criteria on IPR.


.. warning::

    The decoding is done in parallel using threads, and the user **must
    guarantee that the decoder is THREAD-SAFE.** If such property cannot be
    held, we suggest using a single thread for optimization.


.. _doxid-guide_before_opt:

Options before optimization starts
-------------------------------------------------------------------------------

While we can call
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
right away, some options can enhance the pipeline before running the
optimization. We can:

* Set solution observer callbacks that are called when the best solution is
  updated;
* Set a custom stopping-criteria function other than solely time and stalled
  iterations;
* Set a custom shake procedure instead of using the canonical BRKGA-MP-IPR
  shaking options;
* Set custom bias function for chromosome ranking;
* Provide warmstart solutions to the algorithm to improve general solution
  quality and convergence.

We will explore such options in the following sections.


.. _doxid-guide_setting_observer_callbacks:

Setting solution observers / callbacks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Usually, tracking the algorithm's convergence is a good idea.
``:ref:`run()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
provides a callback mechanism activated when the best solution found so far
during the optimization is improved. This is done by calling functions provided
by the user. For instance:

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

adds a callback function that prints the current iteration, the value of the
current best solution, and the time it was found. In this example, we use a
lambda function. Obviously, you can define a named function outside this scope
and add it as a callback, too.

You have noted that we use the method
``:ref:`BRKGA_MP_IPR::addNewSolutionObserver()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa6cf3aca1879ffd4dc0c986340163254>```
to add the callback function which must have the following signature:

.. code-block:: cpp

    bool observer_callback_name(const AlgorithmStatus& status);

where
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
provides the current optimization status, such as current
time, number of iterations, best solution values, best chromosome, and many
other statistics. Indeed,
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
is the primary way to track the algorithm's convergence. Then, this function
returns a ``boolean`` that, if ``true``, aborts the optimization immediately.
This is useful when one wants only to obtain a solution with a particular value
or characteristic and stop to save time.

You can add as many observers as you want. They will be called in the order
they are added.

One interesting usage of such callbacks is to perform (expensive) local search
from the best solution when this cannot be done during the decoder process.
Once the local search is done, we can inject the improved solution/chromosome
back into the population using method
``:ref:`BRKGA_MP_IPR::injectChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>```.
Please, see section
:ref:`Injecting solutions / chromosome into the population <doxid-guide_inject_chromosome>`
for more details.


.. _doxid-guide_setting_stopping_criteria:

Defining custom stopping criteria
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the algorithm always test for **the maximum running time** and for
**the maximum stalled iterations/generations** given by
``:ref:`ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```.
However, in some situations, the user may want to evaluate additional criteria
to determine whether the optimization must stop or not. For example, in a
minimization problem, we may want to stop the value within a distance from a
lower bound or when we reach a given number of iterations, as shown below:

.. code-block:: cpp
    :linenos:

    fitness_t lower_bound = compute_lower_bound();
    unsigned max_iterations = 100;

    algorithm.setStoppingCriteria(
        [&](const AlgorithmStatus& status) {
            return
                status.best_fitness <= lower_bound * 1.1; // 10% from the lower bound
                ||
                status.current_iteration == max_iterations;
        }
    );


For that, we use the method
``:ref:`BRKGA_MP_IPR::setStoppingCriteria() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1adee8fc8410a56e71b2af84ed6f4f2a7c>```
which takes a function with the signature

.. code-block:: cpp

    bool stopping_callback_name(const AlgorithmStatus& status);

Similar to observer callbacks, the function must take a reference to a
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```
object and return ``true`` when the optimization must stop or ``false`` otherwise.

.. warning::

    If you are using implicit path relinking (IPR), which is **very timing
    consuming**, we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME** since this is
    the core stopping criteria on IPR.

If you really mean to have no maximum time and/or maximum stalled iterations
set, we recommend to use the following code:

.. code-block:: cpp
    :linenos:

    // After reading your parameters, e.g.,
    // auto [brkga_params, control_params] = readConfiguration("config.conf");

    // You can set the time to max...
    control_params.maximum_running_time = std::chrono::seconds::max();

    // ... and/or the stalled iterations to max.
    control_params.stall_offset = numeric_limits<unsigned>::max();


.. _doxid-guide_providing_custom_shake:

Providing custom shake procedure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

BRKGA-MP-IPR supplies two canonical ways to perturb the population called
shaking procedures (:ref:`more details here <doxid-guide_guide_shaking>`).
Shaking was introduced by :cite:p:`Andrade2019:PFSP_brkga_shaking`, and the
canonical shaking procedures are effective in most cases. However, there are
situation that calls for a custom (maybe more effective) procedure (e.g.,
:cite:p:`Higino2018:BRKGA_VRP_Private`). In such cases, one can use method

``:ref:`BRKGA_MP_IPR::setShakingMethod()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a4b5019c33a563d1906f0b7d0a8304169>```
which sets a shaking function with the signature:

.. code-block:: cpp
    :linenos:

    void custom_shaking(
        double lower_bound,
        double upper_bound,
        std::vector<std::shared_ptr<Population>>& populations,
        std::vector<std::pair<unsigned, unsigned>>& shaken
    );

We have that:

* Parameters ``lower_bound`` and ``upper_bound`` is the shaking intensity
  bounds to be applied. Usually, the define a range where the intensity is
  sampled;
* Parameter ``populations`` are the current BRKGA populations;
* Parameter ``shaken`` is a list of ``<population index, chromosome index>``
  pairs indicating which chromosomes were shaken on which population,
  so that they got re-decoded.

.. note::

    If ``shaken`` is empty, all chromosomes of all populations are
    re-decoded. This may be slow. Even if you intention is to do so,
    it is faster to populate ``shaken``.

.. warning::

    This procedure can be **very intrusive** since it must manipulate
    the population. So, the user must make sure that BRKGA invariants
    are kept, such as chromosome size and population size.
    Otherwise, the overall functionaly may be compromised.

In the example below, we implement the standard mutation for vanilla
genetic algorithms to the elite population. Note that we kept the random number
generator outside, to make sure we generate different sequences on each call:

.. code-block:: cpp
    :linenos:

    // A random number generator.
    std::mt19937 rng(2700001);
    rng.discard(rng.state_size);

    // Determines whether we change the allele or not.
    std::bernoulli_distribution must_change(0.50);

     algorithm.setShakingMethod(
         [&](double lower_bound, double upper_bound,
             std::vector<std::shared_ptr<Population>>& populations,
             std::vector<std::pair<unsigned, unsigned>>& shaken) {

             // Determines the value of the allele.
             std::uniform_real_distribution<> allele_value(lower_bound, upper_bound);

             for(unsigned pop_idx = 0; pop_idx < populations.size(); ++pop_idx) {
                 auto& population = populations[0]->population;

                 for(unsigned chr_idx = 0; chr_idx < population.size(); ++chr_idx) {
                     auto& chromosome = population[chr_idx];

                     bool change = false;
                     for(unsigned i = 0; i < chromosome.size(); ++i) {
                         if(must_change(rng)) {
                             chromosome[i] = allele_value(rng);
                             change = true;
                         }
                     }

                     if(change)
                         shaken.push_back({pop_idx, chr_idx});
                 } // chr for
             } // pop for
         }; // lambda
     ); // setShakingMethod


.. _doxid-guide_setting_custom_bias_function:

Setting custom bias function
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The bias function controls how alleles are chosen from the (multi) parents
during mating. While BRKGA-MP-IPR framework already provides an extensive set
of functions through
``:ref:`BRKGA::BiasFunctionType
<doxid-namespace_b_r_k_g_a_1af0ede0f2a7123e654a4e3176b5539fb1>```,
one may want to change that behavior using a
custom function (e.g., to
:ref:`simulate the vanilla BRKGA <doxid-guide_guide_standard_brkga>`).
This is done using method
``:ref:`BRKGA_MP_IPR::setBiasCustomFunction()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>```,
where the user supplies the desired **positive non-increasing function** with
the signature

.. code-block:: cpp

    double bias_function(const unsigned r);

.. warning::

    The bias function must be a **positive non-increasing function**, i.e.
    :math:`f: \mathbb{N}^+ \to \mathbb{R}^+` such that :math:`f(i) \ge 0` and
    :math:`f(i) \ge f(i+1)` for :math:`i \in [1, \ldots, total\_parents]`.
    This is requirement to produce the right probabilities.

Note that
``:ref:`setBiasCustomFunction()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>```
tests the function and throw a
`std::runtime_error <https://en.cppreference.com/w/cpp/error/runtime_error>`_
in case the funtion is not positive non-increasing.

For instance, the code below sets the inverse quadratic function as bias:

.. code-block:: cpp
    :linenos:

    algorithm.setBiasCustomFunction(
        [](const unsigned x) {
            return 1.0 / (x * x);
        }
    );


.. _doxid-guide_injecting_warm_start_solutions:

Injecting warm-start solutions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

One good strategy is to bootstrap the main optimization algorithm with good
solutions from fast heuristics
(:cite:p:`Lopes2016:Heuristics_hub_location`,
:cite:p:`Pessoa2018:Heuristics_flowshop_stepwise`,
:cite:p:`Andrade2019:PFSP_brkga_shaking`)
or even from relaxations of integer linear programming models
:cite:p:`Andrade2015:BRKGA_CA`
or constraint programming models
:cite:p:`Andrade2022:PCI_MO`.

Since BRKGA-MP-IPR does not know the problem structure, you must *encode* the
warm-start solution as chromosomes (vectors in the interval [0, 1)). In other
words, you must do the inverse process that your decoder does. For instance,
this is a piece of code from `main_complete.cpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_complete.cpp>`__
showing this process:

.. code-block:: cpp
    :linenos:

    auto initial_solution = greedy_tour(instance);
    //...

    std::mt19937 rng(seed);
    vector<double> keys(instance.num_nodes); // It should be == chromosome_size.
    for(auto& key : keys)
        key = generate_canonical<double,
                                 numeric_limits<double>::digits>(rng);

    sort(keys.begin(), keys.end());

    BRKGA::Chromosome initial_chromosome(instance.num_nodes);
    auto& initial_tour = initial_solution.second;
    for(size_t i = 0; i < keys.size(); i++)
        initial_chromosome[initial_tour[i]] = keys[i];

    algorithm.setInitialPopulation(
        vector<BRKGA::Chromosome>(1, initial_chromosome)
    );

Here, we create one incumbent solution using the greedy heuristic ``greedy_tour()``
`found here <https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples/tsp/src/single_obj/heuristics>`_.
It gives us ``initial_solution`` which is a ``std::pair<double, std::vector<unsigned>>``
containing the cost of the tour and the tour itself which is a sequence of
nodes to be visited. In the next lines, we encode ``initial_solution``. First,
we create a vector of sorted random ``keys``. For that, we create a new random
number generator ``rng``, the vector ``keys``, and fill up ``keys`` with random
numbers in the interval [0, 1), using C++ standard library function
``:ref:`generate_canonical<>() <https://en.cppreference.com/w/cpp/numeric/random/generate_canonical>```
Once we have the keys, we sort them as
``TSP_Decoder::decode()`` does. We then create the ``initial_chromosome``, and
fill it up with ``keys`` according to the nodes' order in ``initial_solution``.
Finally, we use
``:ref:`BRKGA_MP_IPR::setInitialPopulation()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```
to assign the incumbent to the initial population. Note that we enclose the
initial solution inside a vector of chromosomes, since
``:ref:`setInitialPopulation()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a59b05650ede92f5e0107ab606ff6e8b7>```
may take more than one starting solution. See its signature:

.. code-block:: cpp

    void setInitialPopulation(const std::vector<Chromosome>& chromosomes);

Indeed, you can have as much warm-start solutions as you like, limited to the
size of the populations.


.. _doxid-guide_fine_optimization_control:

DIY: building an optimization loop for fine control
===============================================================================

While version 3.0 greatly enhances how to utilize all BRKGA-MP-IPR features
transparently, one may want to change the algorithm's flow. For instance, we
are developing a hyperheuristic using BRKGA, IPR, and VND so that each
algorithm is called when some condition happens. For that, we kept all public
interfaces from version 2.0 to version 3.0. The following sections describe
each one of these features in detail.


.. _doxid-guide_evolving_population:

Evolving the population
-------------------------------------------------------------------------------

The core aspect of the BRKGA-MP-IPR is the genetic algorithm itself. The
evolution of each generation/iteration is performed by method
``:ref:`BRKGA_MP_IPR::evolve()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>```, which
takes the number of generations we must evolve. The call is pretty simple:

.. code-block:: cpp

    algorithm.evolve(num_generations);

``:ref:`evolve()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>```
evolves all populations for ``num_generations``. If ``num_genertions`` is
omitted, only one generation is evolved.

Note that
``:ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
calls
``:ref:`evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>```
for one generation evolution per iteration of the main loop. In a custom setting,
one may evolve several generations per main loop iteration, if it make sense
for that scenario.

.. note::

    ``:ref:`evolve() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aee1828c2ca506f18b896f1fc75ceafcb>```
    does not check the stopping criteria and only stops when the given
    iterations are done. Therefore, we must be careful when evolving multiple
    generations at once. Please, check Section
    :ref:`Complex decoders and timing. <doxid-guide_complex_decoders_and_timing>`

.. warning::

    Again, the decoding of each chromosome is done in parallel if multi-thread
    is enabled. Therefore, **we must guarantee that the decoder is
    THREAD-SAFE.** If such property cannot be held, we suggest using a single
    thread.


.. _doxid-guide_access_solutions:

Accessing solutions/chromosomes
-------------------------------------------------------------------------------

BRKGA-MP-IPR offers several mechanisms to access a variety of data during
the optimization. Most common, we want to access the best chromosome of the
current population after some iterations. You can use the companion methods:

.. code-block:: cpp

    double BRKGA_MP_IPR::getBestFitness() const;
    const Chromosome& BRKGA_MP_IPR::getBestChromosome() const;

``:ref:`BRKGA_MP_IPR::getBestFitness()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0499e65fbddae20a97b276504fe72e39>```
returns the value/fitness of the best chromosome across all current populations.
``:ref:`BRKGA_MP_IPR::getBestChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa4b0396a4780fde3be8d284c535b600e>```
returns a *reference* of the best chromosome across all current populations.
You may want to extract an actual solution from such chromosome, i.e.,
to apply a decoding function that returns the actual solution instead only
its value.

You may have noticed that we insist on the term **current population.**
This is because all getting methods use the current chromosomes, i.e., the
population whose state can change after any procedure such as IPR, shaking, or
reset. Usually, IPR preserves the best solution when manipulating the
population. However, the shaking and reset procedures often perturb and/or
deconstruct the population. *Therefore, there is a big chance that we will lose
the best solution.*

.. warning::

    All the get methods return information (fitness and chromosome) from the
    current population, not the best solution found overall. Only method
    ``:ref:`run() <doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1acb361f402797d3c09390f852326fc7b8>```
    keeps the best solution overall.


You may also want to get a reference of specific chromosome and its fitness
for a given population using
``:ref:`BRKGA_MP_IPR::getChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1abfe4eccfd47a8eb88fc920e640f8513f>```
and
``:ref:`BRKGA_MP_IPR::getFitness()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a9d6636a50f519bf0f1e85257282c6065>```.

.. code-block:: cpp

    const Chromosome& getChromosome(unsigned population_index,
                                    unsigned position) const;

    fitness_t getFitness(unsigned population_index,
                         unsigned position) const;

For example, you can get the 3rd best chromosome (and it fitness)
from the 2nd population using

.. ref-code-block:: cpp

    third_best_chr = algorithm.getChromosome(1, 2);
    third_best_fitness = algorithm.getFitness(1, 2);

.. note::
  Just remember that C++ is zero-indexed. So, the first population index is 0
  (zero), the second population index is 1 (one), and so forth. The same happens
  for the chromosomes.


.. _doxid-guide_inject_chromosome:

Injecting solutions / chromosome into the population
-------------------------------------------------------------------------------

Now, suppose you get such chromosome or chromosomes and apply a quick local
search procedure on them. It may be useful to reinsert such new solutions in
the BRKGA population for the next evolutionary cycles. You can do that using
``:ref:`BRKGA_MP_IPR::injectChromosome()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a0347f67b59bfe36856d1c27c95d4b151>```.

.. code-block:: cpp

    void injectChromosome(
        const Chromosome& chromosome,
        unsigned population_index,
        unsigned position
    )

Note that the chromosome is put in a specific position of a given population.
The new chromosome replaces the old one, and the decoder is triggered to
compute the new fitness. Once done, the population is re-sorted according to
the chromosomes’ fitness.

For example, assuming the ``algorithm`` is your
BRKGA-MP-IPR object and ``brkga_params`` is your ``BrkgaParams`` object, the
following code injects the random chromosome ``keys`` into the population #1 in
the last position (``population_size - 1``), i.e., it will replace the worst
solution by a random one:

.. code-block:: cpp
    :linenos:

    std::mt19937 rng(seed);
    Chromosome keys(instance.num_nodes);
    for(auto& key : keys)
        key = generate_canonical<double, numeric_limits<double>::digits>(rng);

    algorithm.injectChromosome(keys, 0, brkga_params.population_size - 1);


.. _doxid-guide_guide_ipr:

Implicit Path Relink
-------------------------------------------------------------------------------

The Implicit Path Relinking (IPR) is a nice addition to the standard BRKGA
framework, and it provides an excellent way to create hybrid heuristics and
push the optimization further. The good thing about IPR is that you do not
need to worry about the path relink implementation, which can be long and
tedious if done by hand or customized per problem.

BRKGA-MP-IPR provides a friendly interface to use IPR directly from the BRKGA
population, and you only must provide a few functions and arguments to have a
Path Relink algorithm ready to go. These are the two main signatures:
``:ref:`BRKGA_MP_IPR::pathRelink(/*long args call*/)
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>```
and
``:ref:`BRKGA_MP_IPR::pathRelink(/*short args call*/)
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ae9c15595bc46c3554b3eb5656ab08a53>```,
and these are their signatures:

.. code-block:: cpp
    :linenos:

    // Long list of arguments.
    PathRelinking::PathRelinkingResult pathRelink(
        PathRelinking::Type pr_type,
        PathRelinking::Selection pr_selection,
        std::shared_ptr<DistanceFunctionBase> dist,
        unsigned number_pairs,
        double minimum_distance,
        std::size_t block_size = 1,
        std::chrono::seconds max_time = std::chrono::seconds{0},
        double percentage = 1.0
    )

    // Short list of arguments.
    PathRelinking::PathRelinkingResult pathRelink(
        std::shared_ptr<DistanceFunctionBase> dist,
        std::chrono::seconds max_time = std::chrono::seconds{0}
    )

The first argument defines the type of implicit path relink to be performed
``:ref:`BRKGA::PathRelinking::Type
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>```.
The
``:ref:`DIRECT
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76>```
path relink exchanges the keys of two chromosomes directly, and
it is usually more suitable to or threshold representations, i.e., where the
key values are used to some kind of discretization, such as *"if x < 0.5, then
0, otherwise 1."*
The
``:ref:`PERMUTATION
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a48deaef68056f516e0091a15c1db3daa>```
path relink switches the order of a key
according to its position in the other chromosome. Usually, this kind of path
relink is more suitable to permutation representations, where the chromosome
induces an order or permutation. For example, chromosome ``[0.4, 0.7, 0.1]``
may induce the increasing order ``(3, 1, 2)``. More details about threshold and
permutation representations in :cite:p:`Andrade2021:BRKGA_MP_IPR`.

``:ref:`BRKGA::PathRelinking::Selection
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076fa>```
defines how the algorithm picks the chromosomes for relinking.
``:ref:`BESTSOLUTION
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa6a169dcc4781fa0dc8c448d550be9d39>```
selects, in the order, the best solution of each population.
``:ref:`RANDOMELITE
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a3ce8f0aeb5c0063aab2e8cbaee3076faa80e0b7674eebae1977705eed127c6ee8>```
chooses uniformly random solutions from the elite sets.

The next argument is a pointer to a functor object used to compute the distance
between two chromosomes, and determine if changes in a given (block) of alleles
change the solution. This object must inherit from
``:ref:`BRKGA::DistanceFunctionBase
<doxid-class_b_r_k_g_a_1_1_distance_function_base>```, which has the following
signature:

.. code-block:: cpp
    :linenos:

    class DistanceFunctionBase {
    public:
        virtual double distance(
            const Chromosome& v1,
            const Chromosome& v2
        ) = 0;

        virtual bool affectSolution(
            const Chromosome::value_type key1,
            const Chromosome::value_type key2
        ) = 0;

        virtual bool affectSolution(
            Chromosome::const_iterator v1_begin,
            Chromosome::const_iterator v2_begin,
            const std::size_t block_size
        ) = 0;
    };

Note that ``:ref:`BRKGA::DistanceFunctionBase
<doxid-class_b_r_k_g_a_1_1_distance_function_base>``` is an abstract interface,
and children classes must implement all methods.

If the value returned by method ``distance()`` is greater than or equal to
``minimum_distance`` (on ``pathRelink()`` arguments), the algorithm will perform
the path relink between the two chromosomes. Otherwise, it will look for another
pair of chromosomes. The algorithm will try ``number_pairs`` chromosomes before
gives up. In the presence of multiple populations, the path relinking is
performed between elite chromosomes from different populations, in a circular
fashion. For example, suppose we have 3 populations. The framework performs 3
path relinkings: the first between individuals from populations 1 and 2, the
second between populations 2 and 3, and the third between populations 3 and 1.
In the case of just one population, both base and guiding individuals are
sampled from the elite set of that population.

Note that in traditional path relink algorithms, method ``distance()`` depends
on the problem structure. On IPR, you can use a generic distance function, or
provide one that incorporates more knowledge about the problem. BRKGA-MP-IPR
provides a class/functor to compute the (modified)
`Hamming distance <https://en.wikipedia.org/wiki/Hamming_distance>`_
for threshold representations (``:ref:`BRKGA::HammingDistance
<doxid-class_b_r_k_g_a_1_1_hamming_distance>```),
and a class/functor that computes the
`Kendall Tau distance <https://en.wikipedia.org/wiki/Kendall_tau_distance>`_
distance for permutation representations (``:ref:`BRKGA::KendallTauDistance
<doxid-class_b_r_k_g_a_1_1_kendall_tau_distance>```). Again, details about
threshold and permutation representations in :cite:p:`Andrade2021:BRKGA_MP_IPR`.

As a simple example, suppose you are using a threshold representation where
each chromosome key can represent one of 3 different values (a ternary
threshold representation). So, one possible way to compute the distance between
two chromosomes can be:

.. code-block:: cpp
    :linenos:

    class TernaryHammingDistance: public DistanceFunctionBase {
    protected:
        double value(const double key) const {
            return key < 0.33 ? 0.0 : (key < 0.66 ? 1.0 : 2.0);
        }

    public:
        explicit TernaryHammingDistance() {}
        virtual ~TernaryHammingDistance() {}

        virtual double distance(const std::vector<double>& vector1,
                                const std::vector<double>& vector2) override {
            double dist = 0.0;
            for(std::size_t i = 0; i < vector1.size(); ++i)
                dist += std::fabs(value(vector1[i]) - value(vector2[i]));
            return dist;
        }

        virtual bool affectSolution(const double key1, const double key2) override {
            return std::fabs(value(key1) - value(key2)) > 0.0;
        }

        virtual bool affectSolution(std::vector<double>::const_iterator v1_begin,
                                    std::vector<double>::const_iterator v2_begin,
                                    const std::size_t block_size) override {
            for(std::size_t i = 0; i < block_size;
                ++i, ++v1_begin, ++v2_begin) {
                if(std::fabs(value(*v1_begin) - value(*v2_begin)) > 0.0)
                    return true;
            }
            return false;
        }
    };

To avoid changes that do not lead to new solutions, we must verify if such key
exchanges affect the solution. For that, the distance functor object must
implement the methods ``affectSolution()``, which come with two signatures,
depicted in the previous example.

In the first version, ``affectSolution()`` takes two keys and check whether the
exchange of ``key1`` by ``key2`` could change the solution. If so, the method
returns ``true``. The second version takes two iterators for two chromosomes
and checks ``block_size`` keys from those iterators. The idea is, instead to
check only individual keys, we check an entire block of keys. This is very
usual for path relinks that exchange blocks of keys instead individual ones.
For instance, suppose that the alleles/keys are used as threshold such that
values > 0.5 activate a feature. Suppose we have ``chromosome1 = [0.3, 0.4,
0.1, 0.8]`` and ``chromosome2 = [0.6, 0.1, 0.2, 0.9]``. If the key blocks start
on the first keys, and ``block_size = 2``, ``affectSolution()`` will return
``true`` since the very first keys have different activation value. However, if
we start from the 3rd keys and exchange blocks of 2 keys (``[0.4, 0.1]`` by
``[0.1, 0.2]``), nothing changes since both values have the same activation
level (< 0.5). The blocks can hold only one key/allele, sequential key blocks,
or even the whole chromosome.

.. note::
  Note that ``affectSolution()`` is crucial to the IPR performance since this
  function helps to avoid exploring regions already surveyed. Also, note that
  ``affectSolution()`` can incorporate some problem knowledge.

.. note::
  The current implementation of permutation path relink does not make use of
  ``affectSolution()``. However, ``pathRelink()`` requires the function.
  Therefore, we can implement simple constant methods:

  .. code-block:: cpp
    :linenos:

      virtual bool affectSolution(const double, const double) override {
          return true;
      }

      virtual bool affectSolution(std::vector<double>::const_iterator,
                                  std::vector<double>::const_iterator,
                                  const std::size_t) override {
          return true;
      }

``block_size`` defines the number of keys / size of the chromosome block to be
exchanged during the direct path relink. This parameter is also critical for
IPR performance since it avoids too many exchanges during the path building.
Usually, we can compute this number based on the size of the chromosome by some
factor (
``:ref:`BrkgaParams::alpha_block_size <doxid-class_b_r_k_g_a_1_1_brkga_params_1a40ff7da597d2c4eadabf3f62b3b9196d>```
in the configuration file), chosen by you.
Again, details in :cite:p:`Andrade2021:BRKGA_MP_IPR`.

.. note::

  Experiments have shown that a good choice is
  :math:`block\_size = alpha\_block\_size \times \sqrt{size~of~chromosome}`

The last two parameters are stopping criteria. The algorithm stops either when
``max_time`` seconds is reached or ``percentage%`` of the path is built.

.. warning::

  **IPR is a very time-intensive process. You must set the stopping criteria
  accordingly.**

Let's see how can we call IPR. As example, take the TSP for which we use
the permutation-based IPR, and the Kendall Tau distance functions.

.. code-block:: cpp
    :linenos:

    std::shared_ptr<DistanceFunctionBase> dist_func {new KendallTauDistance};

    auto result = algorithm.pathRelink(
        brkga_params.pr_type,
        brkga_params.pr_selection,
        dist_func,
        brkga_params.pr_number_pairs,
        1, // block_size doesn't not matter for permutation.
        max_time - elapsedFrom(start_time),
        brkga_params.pr_percentage
    );

Note that most parameters come from ``brkga_params``. The maximum IPR time is
set to the remaining time for optimization (global ``maximum_time`` minus the
elapsed time).
``:ref:`pathRelink()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1aa8da5193248d23ced19e68483aca31a5>```
returns a
``:ref:`BRKGA::PathRelinking::PathRelinkingResult
<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253>```
object which defines the status of the IPR optimization. Four situation may
happen:

* ``:ref:`TOO_HOMOGENEOUS
  <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253afc79eaa94186dcf0eda5c1df7bd87001>```
  The chromosomes among the populations are too homogeneous and the path relink
  will not generate improveded solutions. This status is directly linked to
  the chosen distance function and minimum distance. If the minimum distance is
  too large, IPR may not be able to find a pair of chromosomes far enough for
  path relink;

* ``:ref:`NO_IMPROVEMENT
  <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae7339868191fd122c45d9abb0dcb87e9>```
  Path relink was done but no improveded solution was found;

* ``:ref:`ELITE_IMPROVEMENT
  <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253ae523f249a5e460a70f2ae8ac7d7a959b>```
  An improved solution among the elite set was found, but the best solution was
  not improved;

* ``:ref:`BEST_IMPROVEMENT
  <doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a64da27c4c7ed94712c1547d972de6253a44ea02fc8a02805c8ddfe8d37d101a39>```
  The best solution was improved.

If the found solution is the best solution found so far, IPR replaces the worst
solution by it. Otherwise, IPR computes the distance between the found solution
and all other solutions in the elite set, and replaces the worst solution by it
if and only if the found solution is, at least, ``minimum_distance`` from all
them.

Important notes about IPR
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

IPR will call ``decode()`` function always with ``writeback = false``. The
reason is that if the decoder rewrites the chromosome, the path between
solutions is lost and inadvertent results may come up. Note that at the end of
the path relinking, the method calls the decoder with ``writeback = true`` in
the best chromosome found to guarantee that this chromosome is re-written to
reflect the best solution found.

.. warning::

    Make sure your decoder does not rewrite the chromosome when called with the
    argument ``writeback = false``.

BRKGA-MP-IPR ``pathRelink()`` implementation is multi-threaded. Instead of to
build and decode each chromosome one at a time, the method builds a list of
candidates, altering the alleles/keys according to the guide solution, and then
decode all candidates in parallel. Note that
:math:`O(chromosome\_size^2~/~block\_size)` additional memory is necessary to
build the candidates, which can be costly if the ``chromosome_size`` is very
large.

.. warning::

    As it is in method ``evolve()``, the decoding is done in parallel using
    threads, and the user **must guarantee that the decoder is THREAD-SAFE.**
    If such property cannot be held, we suggest using a single thread for
    optimization.


.. _doxid-guide_guide_shaking:

Shaking
-------------------------------------------------------------------------------

Sometimes, BRKGA gets stuck, converging to local maxima/minima, for several
iterations. When such a situation happens, it is a good idea to perturb the
population, or even restart from a new one completely new. BRKGA-MP-IPR offers
``:ref:`BRKGA_MP_IPR::shake()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>```,
an improved variation of the original version proposed in
:cite:p:`Andrade2019:PFSP_brkga_shaking`. This is the signature:

.. code-block:: cpp
    :linenos:

    void shake(
        unsigned intensity,
        ShakingType shaking_type,
        unsigned population_index = std::numeric_limits<unsigned>::infinity()
    )

``:ref:`shake()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3721a91ed9d3fcbdc57fbcee2e20ac66>```,
method gets an ``intensity`` parameter that measures how many times
the perturbation is applied on the elite set for a given ``population_index``
(if not given, all populations are shaken). This method offers two
canonial/generic
``:ref:`BRKGA::ShakingType
<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494>```'s.
With
``:ref:`CHANGE
<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a421cfd143e450c3f5814a0495409e073>```,
direct modifications are done in the keys/alleles. This kind of
shaking is recommended when the chromosome uses direct or threshold
representations.
``:ref:`SWAP
<doxid-namespace_b_r_k_g_a_1a616e3d7dedad5ff4e6a2961cda1ea494a46fc23bc4e4d57e5469a39658a6dd3e8>```
exchanges keys/alleles inducing new permutations.
For representational definitions, please refer to
:cite:p:`Andrade2021:BRKGA_MP_IPR`. For instance, the following code shakes
all populations using 10 swap moves:

.. code-block:: cpp

    algorithm.shake(10, BRKGA::ShakingTypeSWAP);

Sometimes, the provided shaking are not appropriated to the problem being solved.
In this case, the user can provide a custom shaking procedure. Please, take a
look on section
:ref:`Providing custom shake procedure <doxid-guide_providing_custom_shake>`.


.. _doxid-guide_guide_resetting:

Resetting
-------------------------------------------------------------------------------

Sometimes, even shaking the populations does not help to escape from local
maxima/minima. So, we need a drastic measure, restarting from scratch the role
population. This can be easily accomplished with
``:ref:`BRKGA::BRKGA_MP_IPR::reset()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a3bfe66221dd2f9c755a65ed7df14e350>```.

.. ref-code-block:: cpp

    algorithm.reset();

.. note::
  When using ``reset()``, all warm-start solutions provided by
  ``setInitialPopulation()`` are discarded. You may use ``injectChromosome()``
  to insert those solutions again.

.. warning::
  Again, the decoding of each chromosome is done in parallel if multi-thread is
  enabled. Therefore, **we must guarantee that the decoder is THREAD-SAFE.** If
  such property cannot be held, we suggest using a single thread..


.. _doxid-guide_guide_migration:

Multi-population and migration
-------------------------------------------------------------------------------

Multi-population or *island model* was introduced in genetic algorithms in
:cite:p:`Whitley1998:Island_model`.
The idea is to evolve parallel and independent populations and, once a while,
exchange individuals among these populations. In several scenarios, this
approach is very beneficial for optimization.

BRKGA-MP-IPR is implemented using such island idea from the core. If you read
the guide until here, you may notice that several methods take into account
multiple populations. To use multiple populations, you must set
``:ref:`BrkgaParams::num_independent_populations
<doxid-class_b_r_k_g_a_1_1_brkga_params_1a9a4a99536f6b755ceb07b54d784f8926>```
with 2 or more populations, and build the BRKGA algorithm from such parameters.

The immigration process is implemented by method
``:ref:`BRKGA_MP_IPR::exchangeElite()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>```
which has the following signature:

.. code-block:: cpp

    void exchangeElite(unsigned num_immigrants);

``:ref:`BRKGA_MP_IPR::exchangeElite()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1ab89298e6c633a81bf8c0462fb40ddd15>```
copies ``num_immigrants`` from one population to another, replacing the worst
``num_immigrants`` individuals from the recipient population. Note that the
migration is done for all pairs of populations. For instance, the following
code exchanges 3 best individuals from each population:

.. code-block:: cpp

    algorithm.exchangeElite(3);


.. _doxid-guide_guide_standard_brkga:

Simulating the standard BRKGA
===============================================================================

Sometimes, it is a good idea to test how the standard BRKGA algorithm performs
for a problem. You can use BRKGA-MP-IPR framework to quickly implement and test
a standard BRKGA.

First, you must guarantee that, during the crossover, the algorithm chooses
only one elite individual and only one non-elite individual. This is easily
accomplished setting ``num_elite_parents = 1`` and ``total_parents = 2``. Then,
you must set up a bias function that ranks the elite and no-elite individual
according to the original BRKGA bias parameter :math:`\rho` (rho).

You can use
``:ref:`BRKGA_MP_IPR::setBiasCustomFunction()
<doxid-class_b_r_k_g_a_1_1_b_r_k_g_a___m_p___i_p_r_1a8616c89626ca3c8e8d3b5adb1da24c92>```
for that task. The given function should receive the index of the chromosome and
returns a ranking for it. Such ranking is used in the roulette method to choose
the individual from which each allele comes to build the new chromosome. Since
we have one two individuals for crossover in the standard BRKGA, the bias
function must return the probability to one or other individual. In the
following code, we do that with a simple ``if...else`` lambda function.

.. ref-code-block:: cpp
    :linenos:

    // Create brkga_params by hand or reading from a file,
    // then set the following by hand.
    brkga_params.num_elite_parents = 1;
    brkga_params.total_parents = 2;

    // This is the original parameter rho form the vanilla BRKGA.
    const double rho = 0.75;

    algorithm.setBiasCustomFunction!(
        [&](const unsigned x) {
            return x == 1 ? rho : 1.0 - rho;
        }
    );

Here, we first set the ``num_elite_parents = 1`` and ``total_parents = 2`` as
explained before. Following, we set a variable ``rho = 0.75``. This is the
:math:`\rho` from standard BRKGA, and you may set it as you wish. Then, we set
the bias function as a very simple lambda function (note that we must use
``[&]`` to capture ``rho`` in the outside context). So, if the index of the
chromosome is 1 (elite individual), it gets a 0.75 rank/probability. If the
index is 2 (non-elite individual), the chromosome gets 0.25 rank/probability.

.. warning::

    Note that we consider the index 1 as the elite individual instead index 0,
    and index 2 to the non-elite individual opposed to index 1. The reason
    for this is that, internally, BRKGA always pass ``r > 0`` to the bias
    function to avoid division-by-zero exceptions.


.. _doxid-guide_guide_parameters:

Reading and writing parameters
===============================================================================

Although we can build the BRKGA algorithm data by set up a
``:ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` object
manually, the easiest way to do so is to read such parameters from a
configuration file. For this, we can use
``:ref:`BRKGA::readConfiguration()
<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>```
that reads a simple plain text file and returns a tuple of
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>```
and
``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```
objects. For instance,

.. code-block:: cpp

    auto [brkga_params, control_params] = BRKGA::readConfiguration("tuned_conf.txt");

The configuration file must be plain text such that contains pairs of
parameter name and value.
In `examples folder
<https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples/tsp>`_, we
have `config.conf
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/config.conf>`_
that looks like this:

.. code-block::
    :linenos:

    # BRKGA and IPR parameters
    population_size 2000
    elite_percentage 0.30
    mutants_percentage 0.15
    num_elite_parents 2
    total_parents 3
    bias_type LOGINVERSE
    num_independent_populations 3
    pr_number_pairs 0
    pr_minimum_distance 0.15
    pr_type DIRECT
    pr_selection BESTSOLUTION
    pr_distance_function_type KENDALLTAU
    alpha_block_size 1.0
    pr_percentage 1.0
    num_exchange_individuals 1
    shaking_type SWAP
    shaking_intensity_lower_bound 0.25
    shaking_intensity_upper_bound 0.75

    # Control parameters
    maximum_running_time 60
    exchange_interval 100
    ipr_interval 200
    shake_interval 300
    reset_interval 500
    stall_offset 100

It does not matter whether we use lower or upper cases. Blank lines and lines
starting with ``#`` are ignored. The order of the parameters should not
matter either. And, finally, this file should be readable for both C++, Julia,
and Python framework versions (when all come to the same version number).

In some cases, you define some of the parameters at the running time, and you
may want to save them for debug or posterior use. To do so, you can use
``:ref:`BRKGA::writeConfiguration()
<doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>```,
call upon a ``:ref:`BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>``` object.

.. code-block:: cpp
    :linenos:

    BRKGA::writeConfiguration("test.conf", brkga_params);
    //or
    BRKGA::writeConfiguration ("test.conf", brkga_params, control_params);

If ``control_params`` is not given, default values are used in its place.

.. note::

    ``:ref:`BRKGA::writeConfiguration()
    <doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>```
    rewrites the given file. So, watch out to not lose previous configurations.


.. _doxid-guide_guide_multi_obj:

Using BRKGA-MP-IPR on multi-objective mode
===============================================================================

As stated in the introduction, BRKGA-MP-IPR also deals with multiple objectives
in a lexicographical or priority dominance order. Differing from classical
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

In many problems in the real-life, the users usually require a particular
priority order among several objective functions, and therefore, the
lexicographical approach is very appropriate. However, if the objective
functions have no apparent order in your scenario, you may need to use a
non-dominated approach.

.. warning::

    If you really want an algorithm to produce a **non-dominated set of
    solutions (Pareto frontier)**, this is **not** the right algorithm for you.
    We recommend taking a look at the `NSGA-II
    <https://doi.org/10.1109/4235.996017>`_ and `MOAB
    <https://en.wikipedia.org/wiki/MOEA_Framework>`_.

Note that we could use the single-objective version of the BRKGA, by doing a
linear (or affine) comnination of the objective function like this:

.. math::

    \sum_{i = 1}^n \alpha_i f_i

where :math:`n` is the number of objective functions.
In the minimization case, to guarantee that
:math:`f_i` is more important than :math:`f_{i + 1}`, we must choose
:math:`\alpha_i > \sup(D_{i + 1})`, the supremum of set :math:`D_{i+1}` which is
the image of :math:`f_{i+1}`, i.e., the value :math:`f_{i+1}` can generate.
In other words, :math:`\alpha_i` must be larger than the highest value
:math:`f_{i+1}` can take.

For instance, suppose we have two objective integer functions :math:`f_1` and
:math:`f_2`, for a minimization problem. Function :math:`f_1` values vary from 0
to 10, and :math:`f_2` varies from 100 to 500. To guarantee that a single unit
change in :math:`f_1` is more important than all :math:`f_2`, we must make
alpha1 = 501. Therefore, if we reduce :math:`f_1` off one unit, this impact in
the combined objective function will be 501 units, which is larger than the
largest possible value of :math:`f_2`.

The problem with this approach is that it can lead to numerical issues too fast
and too frequent for lots of practical applications. Depending on the domain of
our functions, precision errors and overflow can occur, impairing the
optimization process at all. Second, this is harder to debug and gets more work
from the developer to break the total value on every single objective value.

That said, to use BRKGA-MP-IPR in the native multi-objective mode,
we first must set
``:ref:`BRKGA::fitness_t
<doxid-namespace_b_r_k_g_a_1ae212772a5d4bb9b7055e30791b494514>```
according to the number of objectives we want. For that, we must
change the file
`fitness_type.hpp <https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/fitness_type.hpp>`_
to reflect such a thing. In this example, we use the standard
`std::tuple <https://en.cppreference.com/w/cpp/utility/tuple>`_:

.. code-block:: cpp
    :linenos:

    namespace BRKGA {
    using fitness_t = std::tuple<double, double>;
    } // end namespace BRKGA_MP_IPR

In theory, we can use any custom structure or class, providing it implements
the comparison operators (`operator<`, `operator>`, and `operator==`), and the
streaming operator `std::ostream& operator<<(std::ostream& os, const
CustomFitness& fitness)` where `CustomFitness` is your fitness structure.

Internally, BRKGA-MP-IPR doesn't use ``operator==`` directly. BRKGA implements
the custom function
``:ref:`BRKGA::close_enough()
<doxid-group__utility__functions_1gae3eb0d56b561126f23ebd26ec556ac29>```.
For fundamental numerical types, it compares the absolute difference with a given
``:ref:`BRKGA::EQUALITY_THRESHOLD
<doxid-namespace_b_r_k_g_a_1a8d1d184901bb4f34c71c7bb73a86a84a>```
i.e., two numbers :math:`a` and :math:`b` equal if :math:`|a - b| <
EQUALITY\_THRESHOLD`. For all other types (except ``std::tuple``), we use
``operator==``. For ``std::tuple``, we have a specialized ``close_enough()``
that iterates over each element of the tuples calling the base
``close_enough()``.

Once defined your ``fitness_t``, you must also provide
``:ref:`BRKGA::FITNESS_T_MIN
<doxid-namespace_b_r_k_g_a_1a27f915fd21c02aee1097135954420ebb>```
and
``:ref:`BRKGA::FITNESS_T_MAX
<doxid-namespace_b_r_k_g_a_1aa4eaa93f02c949d7af918598c606402f>```,
if your ``fitness_t`` is not a fundamental type or a tuple of
fundamental types. The following is an example of a custom ``fitness_t`` should
looks like.

.. code-block:: cpp
    :linenos:

    class MyCrazyFitness {
    public:
        constexpr MyCrazyFitness() = default;


        constexpr MyCrazyFitness(const double _main_part,
                                 const double _secondary_part,
                                 const double _threshold):
            main_part(_main_part),
            secondary_part(_secondary_part),
            threshold(_threshold)
            {}

        bool operator<(const MyCrazyFitness& second) const {
            if(this->main_part < (second.main_part - threshold))
                return true;
            if(this->secondary_part < second.secondary_part)
                return true;
            return false;
        }

        bool operator>(const MyCrazyFitness& second) const {
            if(this->main_part > (second.main_part + threshold))
                return true;
            if(this->secondary_part > second.secondary_part)
                return true;
            return false;
        }

        bool operator==(const MyCrazyFitness& second) const {
            return !((*this < second) || (*this > second));
        }

    public:
        double main_part;
        double secondary_part;
        double threshold;
    };

    std::ostream& operator<<(std::ostream& os, const MyCrazyFitness& fitness) {
        os
        << "(main_part: " << fitness.main_part << ", "
        << "secondary_part: " << fitness.secondary_part << ", "
        << "threshold: " << fitness.threshold << ")";
        return os;
    }

    // The following three definitions are mandatory!
    using fitness_t = MyCrazyFitness;

    static constexpr fitness_t FITNESS_T_MIN {-100.0, -10.0, 1e-16};

    static constexpr fitness_t FITNESS_T_MAX {1000.0, 100.0, 1e-1};



.. warning::

    Unless we really need a custom ``fitness_t``, you should use ``std::tuple``.


.. _doxid-guide_guide_tips:

(Probable Valuable) Tips
===============================================================================

.. _doxid-guide_algorithm_warmup:

Algorithm warmup
-------------------------------------------------------------------------------

While in Julia framework version is primordial to do a dry-run to precompile
all functions (and a good idea on Python version), in C++ and Python this
warmup is not necessary. However, few dry-runs can help the OS/processor with
cache locality and give some speedup.

Besides the dry-runs, I would recommend the pre-allocation of all
resource/memory that you need, if you know in advance how much will be
necessary. This pre-allocation speeds the decoding process dramatically.
There is
`some argument <https://github.com/ceandrade/brkga_mp_ipr_cpp/issues/3#issuecomment-986107579>`_.
whether or not we should pre-allocate some data
structures since it might incur
`false sharing <https://en.wikipedia.org/wiki/False_sharing>`_.
Therefore, more experimentation and fine-tuning are needed in this space.


.. _doxid-guide_complex_decoders_and_timing:

Complex decoders and timing
-------------------------------------------------------------------------------

Some problems require complex decoders while for others, the decoder contains
local search procedures, that can be time-consuming. In general, the decoding
is the most time-expensive component of a BRKGA algorithm, and it may skew some
stopping criteria based on running time. Therefore, if your decoder is
time-consuming, it is a good idea to implement a timer or chronometer kind of
thing inside the decoder.

Testing for stopping time uses several CPU cycles, and you need to be careful
when/where to test it, otherwise, you spend all the optimization time doing
system calls to the clock.

IMHO, the most effective way to do it is to test time at the very begining of
the decoding. If the current time is larger than the maximum time allowed,
simple return ``Inf`` or ``-Inf`` according to your optimization direction. In
this way, we make the solution **invalid** since it violates the maximum time
allowed. The BRKGA framework takes care of the rest.


.. _doxid-guide_guide_tips_multi_thread_decoding:

Multi-thread decoding
-------------------------------------------------------------------------------

Since `Moore's law <https://en.wikipedia.org/wiki/Moore%27s_law>`_ is not
holding its status anymore, we, simple mortals, must appeal to the wonders of
multi-threading. This paradigm can be tricky to code, and `Amdahl's law
<https://en.wikipedia.org/wiki/Amdahl%27s_law>`_ plays against us. Several
genetic algorithms, and in particular, BRKGA, can use parallel solution
evaluation (or decoding), which makes the use of multi-threading relatively
straightforward. BRKGA-MP-IPR C++ framework is not different, and it uses
`OpenMP <https://www.openmp.org>`_ capabilities to do so.

First, as commented several times in this guide, **the decoder must be
THREAD-SAFE.** So, each thread must have its own read/write data structures and
may share other read-only data. The simplest way to do it is to create those
structures inside the decoder (like most people do). **But be aware**, this
strategy slows down the algorithm significantly depending on the size and
format of the structures, and *I do not recommend it*.

IMHO, the best way to do that is to preallocate the data structure per thread
and pass them to the decoder through the problem instance. Then, inside the
decoder, you can use `omp_get_thread_num()
<https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf>`_
and recover the memory you want to use.

Let's see a simple example considering the TSP example. ``TSP_Decode`` uses a
single array to create the permutation of nodes. Let's pre-allocate its memory
per thread. To keep things separeted and easy to understand, we created a new
class
`TSP_Decoder_pre_allocating
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/decoders/tsp_decoder_pre_allocating.hpp>`_
so that we allocate, for each thread, a vector to hold the permutation during
the decoding:

.. code-block:: cpp
    :linenos:

    class TSP_Decoder_pre_allocating {
    public:
        TSP_Decoder_pre_allocating(const TSP_Instance& instance,
                                   const unsigned num_threads = 1);

        double decode(BRKGA::Chromosome & chromosome, bool rewrite = true);

    public:
        const TSP_Instance& instance;

    protected:
        using Permutation = std::vector<std::pair<double, unsigned>>;
        std::vector<Permutation> permutation_per_thread;
    };

Note that the constructor has one more argument indicating how many threads we
are using. We also define a type ``Permutation`` with is a simple vector of
key-node pairs. The important structure is ``permuration_per_thread`` which is
a simple vector of the size of the number of threads where we allocate the
permutation vectors for each thread.

Then, in the implementation, we allocate all memory in the constructor
(`RAII principle <https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization>`_).
In ``decode``, we use `omp_get_thread_num()
<https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf>`_
to identify which thread called the decoder, and retrieve the respective data
strucuture.

.. code-block:: cpp
    :linenos:

    #include <omp.h>

    TSP_Decoder_pre_allocating::TSP_Decoder_pre_allocating(
                const TSP_Instance& _instance, const unsigned num_threads):
        instance(_instance),
        // Pre-allocate space for permutations for each thread.
        permutation_per_thread(num_threads, Permutation(instance.num_nodes))
    {}

    double TSP_Decoder_pre_allocating::decode(Chromosome& chromosome,
                                              bool /* not-used */) {
        // If you have OpenMP available, get the allocated memory per thread ID.
        #ifdef _OPENMP
        auto& permutation = permutation_per_thread[omp_get_thread_num()];
        #else
        auto& permutation = permutation_per_thread[0];
        #endif

        for(unsigned i = 0; i < instance.num_nodes; ++i)
            permutation[i] = make_pair(chromosome[i], i);

        sort(permutation.begin(), permutation.end());

        double cost = instance.distance(permutation.front().second,
                                        permutation.back().second);

        for(unsigned i = 0; i < instance.num_nodes - 1; ++i)
            cost += instance.distance(permutation[i].second,
                                      permutation[i + 1].second);

        return cost;
    }

.. note::

    Pre-allocation and multi-threading only make sense for large data structures
    and time-consuming decoders. Otherwise, the code spends too much time on
    context switching and system calls.

.. warning::

    Multi-threading consumes many resources of the machine and have diminishing
    returns. I recommend using **at most the number of physical cores** (may -1)
    to avoid racing and too much context switching. You must test which is the
    best option for your case. In my experience, complex decoders benefit more
    from multi-threading than simple and fast decoders.


.. _doxid-guide_guide_tips_multi_thread_mating:

Multi-thread mating
-------------------------------------------------------------------------------

One of the nice additions to BRKGA-MP-IPR 2.0 is the capability of performing
the mating in parallel. Such capability speeds up the algorithm substantially,
mainly for large chromosomes and large populations. However, when performing
parallel mating, we have some points regarding reproducibility described below.
The parallel mating is controlled in compilation time by the preprocessing flags
``:ref:`MATING_FULL_SPEED <doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7>```,
``:ref:`MATING_SEED_ONLY <doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22>```,
and
``:ref:`MATING_SEQUENTIAL <doxid-group__compiler__directives_1ga5766b15b52c6201d58440c53462fdf24>```,

Compiling your code with
``:ref:`MATING_SEQUENTIAL <doxid-group__compiler__directives_1ga5766b15b52c6201d58440c53462fdf24>```,
enabled will remove the parallel mating at all, and the algorithm will behave
like the previous versions. This option can be very slow for large chromosomes
and large populations. But it makes debugging easier. Following we have a quick
example using the TSP decoder with 4 threads, for 1,000 generations. We only
run the evolutionary portion, disabling all other features
(see `main_maximum_iterations.cpp
<https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_maximum_iterations.cpp>`_).
Note that the average total time (real time) is 4m 46s.

.. code-block::

    $ make
    ...
    ...
    g++ -DMATING_SEQUENTIAL -std=c++20 -O3 -fomit-frame-pointer -funroll-loops \
        -ftracer -fpeel-loops -fprefetch-loop-arrays -pthread -fopenmp \
        -I. -I./brkga_mp_ipr  -c main_maximum_iterations.cpp -o main_maximum_iterations.o
    ...

    $ for i in 1 2 3; do time ./main_maximum_iterations 270001 config.conf 1000 ../../instances/rd400.dat > /dev/null; done

    real    4m48.649s
    user    6m58.208s
    sys     0m0.432s

    real    4m42.132s
    user    6m48.032s
    sys     0m0.418s

    real    4m46.317s
    user    6m53.796s
    sys     0m0.243s


Setting
``:ref:`MATING_SEED_ONLY <doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22>```,
BRKGA will perform the mating in parallel, however, in
a more controlled way. On each evolutionary step, the algorithm creates a
sequence of seeds (one per to-be-generated offspring). Such seeds are fed to a
pseudo-random number generator (RNG) for each thread: when starting the mating,
the RNG is seeded before being used. This occurs in each iteration. In this way,
all generators have their state controlled by the main generator, and therefore,
**the unique seed** supplied by the user. This is another example, running the
same machine and also 4 threads. The average total time is 1m 58s, a whooping
reduction of ~59%.

.. code-block::

    $ make
    ...
    ...
    g++ -DMATING_SEED_ONLY -std=c++20 -O3 -fomit-frame-pointer -funroll-loops \
        -ftracer -fpeel-loops -fprefetch-loop-arrays -pthread -fopenmp \
        -I. -I./brkga_mp_ipr  -c main_maximum_iterations.cpp -o main_maximum_iterations.o
    ...

    $ for i in 1 2 3; do time ./main_maximum_iterations 270001 config.conf 1000 ../../instances/rd400.dat > /dev/null; done

    real    1m58.493s
    user    7m34.826s
    sys     0m2.335s

    real    1m57.311s
    user    7m38.627s
    sys     0m0.941s

    real    1m57.403s
    user    7m47.691s
    sys     0m0.950s

Finally,
``:ref:`MATING_FULL_SPEED <doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7>```,
enables parallel mating at full speed. In this case,
each thread has a unique RNG previously seeded at the beginning of the algorithm
(BRKGA constructor). This initialization is done in a chain: the first RNG is
seeded with the seed provided by the user. For the following, the algorithm uses
the previous RNG state as seed. In this way, we guarantee that all RNGs have
different states, but all depend on the user seed. However, in this case, **the
reproducibility depends on both the seed given by the user and the number of
threads**. This is because to guarantee reproducibility, we must ensure that the
same chromosome region is handled by the same thread (id) since we have an RNG
per thread. In other words, the random states must be the same for each thread,
on different runs.  When we increase or decrease the number of threads,
different threads will handle the chromosomes. However, this is the fastest
method. Here is another example, using the same conditions as before.
The average here is 1m 56s, a marginal improve regarding to
``:ref:`MATING_SEED_ONLY <doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22>```,

.. ref-code-block::

    $ make
    ...
    ...
    g++ -DMATING_FULL_SPEED -std=c++20 -O3 -fomit-frame-pointer -funroll-loops \
        -ftracer -fpeel-loops -fprefetch-loop-arrays -pthread -fopenmp \
        -I. -I./brkga_mp_ipr  -c main_maximum_iterations.cpp -o main_maximum_iterations.o
    ...

    $ for i in 1 2 3; do time ./main_maximum_iterations 270001 config.conf 1000 ../../instances/rd400.dat > /dev/null; done

    real    1m55.840s
    user    7m40.191s
    sys     0m1.382s

    real    1m56.824s
    user    7m44.251s
    sys     0m0.845s

    real    1m56.293s
    user    7m41.350s
    sys     0m2.114s


.. warning::

    Remember, when using
    ``:ref:`MATING_FULL_SPEED <doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7>```,
    the results depend on both **seed** and **number of threads.**
    Multiple runs with the same seed and number of threads should produce the
    same results. Changing one or other, the results will change.

Of course, these results depend on the problem and decoder implementation. But
you can see that, or simple/fast decoders, both parallel options represent a
very significant improvement over the serial version.

.. note::

    We thank `Alberto F. Kummer <https://github.com/afkummer>`_
    very much for the first version of the parallel mating and the fruitful
    discussions about it and other topics. Please, also consider citing his
    paper :cite:p:`Kummer2020:BRKGA_VRPTW_Synch`.


.. _doxid-guide_known_issues:

Known issues
===============================================================================

Since BRKGA-MP-IPR is header-only, we may have compilation/linking issues if we
include the BRKGA-MP-IPR header in different modules (translation units on C++
jargon) and compile them separately (which normally we do). For example,
suppose we have two pieces of code, module_a.cpp and module_b.cpp, such that we
include BRKGA in both (i.e., ``#include "brkga_mp_ipr"`` in both files.

File `module_a.cpp`:

.. code-block:: cpp
    :linenos:

    #include "brkga_mp_ipr.hpp"
    int main() {
        auto params = BRKGA::readConfiguration("config.conf");
        return 0;
    }

File `module_b.cpp`:

.. code-block:: cpp
    :linenos:

    #include "brkga_mp_ipr.hpp"
    void test() {
        auto params = BRKGA::readConfiguration("config.conf");
    }

Let's compile each one with GCC and link them:

.. code-block::
    :linenos:

    $ g++ -std=c++20 -I../brkga_mp_ipr -c module_a.cpp -o module_a.o

    $ g++ -std=c++20 -I../brkga_mp_ipr -c module_b.cpp -o module_b.o

    $ g++ module_a.o module_b.o -o test

    duplicate symbol '__ZN5BRKGA6EnumIOINS_13PathRelinking9SelectionEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA6EnumIOINS_5SenseEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA6EnumIOINS_16BiasFunctionTypeEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA6EnumIOINS_13PathRelinking20DistanceFunctionTypeEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA6EnumIOINS_11ShakingTypeEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA6EnumIOINS_13PathRelinking4TypeEE10enum_namesB5cxx11Ev' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA17readConfigurationERSiRSo' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA17readConfigurationERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERSo' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGAlsERSoRKNS_15AlgorithmStatusE' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGAlsERSoRKNS_13ControlParamsE' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA18writeConfigurationERSoRKNS_11BrkgaParamsERKNS_13ControlParamsE' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGA18writeConfigurationERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERKNS_11BrkgaParamsERKNS_13ControlParamsE' in:
        module_a.o
        module_b.o
    duplicate symbol '__ZN5BRKGAlsERSoRKNS_11BrkgaParamsE' in:
        module_a.o
        module_b.o
    ld: 13 duplicate symbols for architecture x86_64
    collect2: error: ld returned 1 exit status

Now, we try with Clang:

.. code-block::
    :linenos:

    $ clang++ -std=c++20 -fopenmp -I../brkga_mp_ipr -c module_a.cpp -o module_a.o

    $ clang++ -std=c++20 -fopenmp -I../brkga_mp_ipr -c module_b.cpp -o module_b.o

    $ clang++ -std=c++20 -fopenmp module_a.o module_b.o -o test

    duplicate symbol 'BRKGA::EnumIO<BRKGA::PathRelinking::Selection>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::EnumIO<BRKGA::Sense>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::EnumIO<BRKGA::BiasFunctionType>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::EnumIO<BRKGA::PathRelinking::DistanceFunctionType>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::EnumIO<BRKGA::ShakingType>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::EnumIO<BRKGA::PathRelinking::Type>::enum_names()' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::operator<<(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, BRKGA::AlgorithmStatus const&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::writeConfiguration(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, BRKGA::BrkgaParams const&, BRKGA::ControlParams const&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::writeConfiguration(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, BRKGA::BrkgaParams const&, BRKGA::ControlParams const&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::operator<<(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, BRKGA::ControlParams const&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::operator<<(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, BRKGA::BrkgaParams const&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::readConfiguration(std::__1::basic_istream<char, std::__1::char_traits<char>>&, std::__1::basic_ostream<char, std::__1::char_traits<char>>&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'BRKGA::readConfiguration(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&, std::__1::basic_ostream<char, std::__1::char_traits<char>>&)' in:
        module_a.o
        module_b.o
    duplicate symbol 'operator<<(std::__1::basic_ostream<char, std::__1::char_traits<char>>&, std::__1::chrono::duration<double, std::__1::ratio<1l, 1l>> const&)' in:
        module_a.o
        module_b.o
    ld: 14 duplicate symbols for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)


So, note that we have several duplicated symbols (which varies a little bit
per compiler), including the EnumIO utilities, output stream operators,
``:ref:`BRKGA::readConfiguration()
<doxid-group__brkga__control__params_1ga1c8b456ad75a3b522d315d4167546ae6>```,
``:ref:`BRKGA::writeConfiguration()
<doxid-group__brkga__control__params_1ga758c489d2f6291cf80a78ca6765b856e>```,
even the definitions of
``:ref:`BRKGA::AlgorithmStatus <doxid-class_b_r_k_g_a_1_1_algorithm_status>```,
``:ref:`BRKGA::BrkgaParams <doxid-class_b_r_k_g_a_1_1_brkga_params>```,
and
``:ref:`BRKGA::ControlParams <doxid-class_b_r_k_g_a_1_1_control_params>```.

To avoid such a situation, we have to ``inline`` these functions and template
specializations. We can do that passing the compiler directive
``:ref:`BRKGA_MULTIPLE_INCLUSIONS
<doxid-group__compiler__directives_1ga608f9e381594efac6848d962c0110e91>```,
which inlines the functions and template specializations properly.

.. code-block::

    $ g++ -std=c++20 -I../brkga_mp_ipr -DBRKGA_MULTIPLE_INCLUSIONS -c module_a.cpp -o module_a.o

    $ g++ -std=c++20 -I../brkga_mp_ipr -DBRKGA_MULTIPLE_INCLUSIONS -c module_b.cpp -o module_b.o

    $ g++ module_a.o module_b.o -o test

However, we have two side-effects. First, such inlining can make the object
code grows large since we include several copies of the same functions (which
are I/O functions which already are large by their nature). Second, the
compiler may complain about too many inline functions, if you are already using
several inline functions.

.. warning::

    Avoid including ``brkga_mp_ip.hpp`` in several files/translation units.
    If unavoidable, use the compiler directive ``:ref:`BRKGA_MULTIPLE_INCLUSIONS
    <doxid-group__compiler__directives_1ga608f9e381594efac6848d962c0110e91>```.

But now, suppose we must use multiple inclusions of BRKGA header, and our
compiler finds issues on inline such functions. The last resource is to move
the implementation of such functions to a separated translation unit (.cpp)
and compile them isolated, adding to the linking stage.


.. _doxid-guide_references:

References
===============================================================================

.. bibliography::
