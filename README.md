
<div align="center">
  <img src="https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/src_docs/src/assets/logo_name_300.png">
</div>

BRKGA-MP-IPR - C++ version 3.1.1
================================================================================

<table>
<tr>
  <td>Documentation</td>
  <td>
    <a href="https://ceandrade.github.io/brkga_mp_ipr_cpp">
    <img src="https://img.shields.io/badge/Tutorial-API-blue.svg" alt="Documentation" />
    </a>
  </td>
</tr>
<tr>
  <td>License</td>
  <td>
    <a href="https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md">
    <img src="https://img.shields.io/badge/license-BSD--like-blue" alt="License" />
    </a>
    <a href="https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md">
    <img src="https://img.shields.io/badge/license-MIT-blue" alt="License" />
    </a>
  </td>
</tr>
</table>

Table of Contents
--------------------------------------------------------------------------------

- :speech_balloon: [Introduction](#speech_balloon-introduction)
- :high_brightness: [What is new in version 3.1.1](#high_brightness-what-is-new-on-version-311)
- :high_brightness: [What is new in version 3.1](#high_brightness-what-is-new-on-version-31)
  - [Bug fix 1: Evolution step used to mess up with the best chromosome](#evolution-step-used-to-mess-up-with-the-best-chromosome)
  - [Bug fix 2: Initial population was incorrect](#initial-population-was-incorrect)
- :high_brightness: [What is new in version 3.0](#high_brightness-what-is-new-on-version-30)
  - [API enhancements](#api-enhancements)
  - [BRKGA and control parameters](#brkga-and-control-parameters)
  - [Code modernizing and speed bump](#code-modernizing-and-speed-bump)
- :high_brightness: [What is new in version 2.0](#high_brightness-what-is-new-on-version-20)
  - [Multiples objectives (Lexicographically)](#multiples-objectives-lexicographically)
  - [Multi-thread mating](#multi-thread-mating)
  - [API changes](#api-changes)
  - [Bug fixes](#bug-fixes)
- :computer: [Installation](#computer-installation)
- :zap: [Usage](#zap-usage---tldr-single-objective)
  - [TL;DR, Single objective](#zap-usage---tldr-single-objective)
  - [TL;DR, Multi objective](#zap-usage---tldr-multi-objective)
- :books: [Tutorial and full documentation](#books-tutorial-and-full-documentation)
- :black_nib: [License and Citing](#black_nib-license-and-citing)
- :construction_worker: [TODO](#construction_worker-todo)
- :pencil2: [Contributing](#pencil2-contributing)


:speech_balloon: Introduction
--------------------------------------------------------------------------------

BRKGA-MP-IPR provides a _very easy-to-use_ framework for the
Multi-Parent Biased Random-Key Genetic Algorithm with Implicit Path Relink
(**BRKGA-MP-IPR**). Assuming that you have a _decoder_ for your problem,
we can set up, run, and extract the value of the best solution in less than
5 commands (obviously, you may need a few other lines of code to do a proper
test).

We provide a very thorough and easy-to-follow tutorial on using
BRKGA effectively. We strongly recommend you read the tutorial to make
your code work at best within BRKGA. Check out the tutorial and API
documentation: https://ceandrade.github.io/brkga_mp_ipr_cpp

This C++ version provides a fast prototyping API using C++20 standards and
libraries. All code was developed as a header-only library, with no
external dependencies other than those included in the package. So, you just
need to copy/check out the files and point your compiler's header path to
BRKGA-MP-IPR folder (`-I` on GCC and Clang).

This framework can use multiple threads of modern CPUs, by setting a single
parameter (assuming that your decoder is thread-safe). This leverages the
parallel decoding nature that BRKGAs offer, compared to other (meta)
heuristic frameworks.

The code can be compiled using [> GCC 9.4](https://gcc.gnu.org) and [> Clang
10.0](https://clang.llvm.org), and it is probable that it can be compiled
using other modern C++ compilers, such as Intel and Microsoft compilers. To
use multiple threads, your compiler must support
[OpenMP](https://www.openmp.org). The current version has been under 
long development, and it is a very mature code used in production 
in several companies. However, it lacks proper unit and
coverage testing. Such tests are in the [TODO](#construction_worker-todo) list.

If C++ is not suitable for you, we may find useful the [**Julia
version**](https://github.com/ceandrade/brkga_mp_ipr_julia) of this framework.
We are also developing a
[**Python version**](https://github.com/ceandrade/brkga_mp_ipr_python)
which is in its earlier stages (no IPR yet).
However, both Julia and Python versions only support single-objective optimization
now. We have no timeline for implementing multi-objective optimization
in such platforms. Also, at this moment, we have no plans to implement
the BRKGA-MP-IPR in other languages such as Java or C#. 
But if you want to do so, you are welcome to do so. 
But please, keep the API as close as possible to the C++ API
(or Julia API in case you decide to go C), and use the best coding and
documentation practices of your chosen language/framework.

- [**Julia version**](https://github.com/ceandrade/brkga_mp_ipr_julia)
- [**Python version**](https://github.com/ceandrade/brkga_mp_ipr_python)

If you are not familiar with how BRKGA works, take a look at
[Multi-Parent BRKGA](https://doi.org/10.1016/j.ejor.2019.11.037).

You may also find it very useful to read these reviews:

- [Biased random-key genetic algorithms: A review](https://doi.org/10.1016/j.ejor.2024.03.030)
- [Early years of Biased Random-Key Genetic Algorithms: A systematic review](https://doi.org/10.1007/s10898-024-01446-5)


:high_brightness: What is new in version 3.1.1
--------------------------------------------------------------------------------

- We have removed some code used for debugging that may cause compilation errors
depending on the `fitness_t` chosen by the user;
- Small polishing for modern C++ brace-initialization syntax.


:high_brightness: What is new in version 3.1
--------------------------------------------------------------------------------

We have two major bug fixes, so we decided to bump the version up to 3.1,
even though we have no new features.

### Evolution step is used to mess up with the best chromosome

On the `evolution()` method, the index handling was mixed with absolute and
relative chromosome position/ranking in the population.
Therefore, offspring may be written in the place of elite individuals.
Although this is a major bug, the overall performance did not drop
significantly, showing how robust BRKGA is, even in the presence of noise
and bugs.

I thank **Pedro H.D.B Hokama** and **Mário C. San Felice** for the long debug session
in finding and fixing this issue.

### Initial population was incorrect

The method `initialize()` is responsible for asserting the correct state of
the algorithm before it runs. This includes initializing the full population
if no initial solution is injected before the algorithm starts, or
if the initial solutions/chromosomes are provided,
completing the first population with random chromosomes.

While working correctly in the presence of initial solutions,
`initialize()` incorrectly creates a population with all alleles equal to zero
when no initial solutions were present. We have fixed that,
and now we guarantee an entirely random initial population in both cases.

Thank **Daniele Ferone** for detecting such a bug.


:high_brightness: What is new in version 3.0
--------------------------------------------------------------------------------

### API enhancements

In version 2.0, we claimed that BRKGA-MP-IPR was a very easy-to-use framework.
But, a few people told me this statement was not even true. The primary complaint
was that while the features were very nice, tightening them together was hard,
even using the provided examples.

Now, BRKGA-MP-IPR supplies a method called `run()`. It implements the entire
pipeline using all framework features in a chain-like way, similar to the
detailed examples. The user may call in this way:

```cpp
    //...
    auto [brkga_params, control_params] =
          BRKGA::readConfiguration(config_file);

    MyDecoder my_decoder;

    BRKGA::BRKGA_MP_IPR<MyDecoder> algorithm(
        my_decoder, BRKGA::Sense::MINIMIZE, seed, num_chromosomes, brkga_params
    );
    // algorithm.initialize(); // No need anymore :-)

    auto final_status = algorithm.run(control_params, &cout);
    cout << "Final algorithm status\n" << final_status;
    //...
```

where `control_params` is an instance of the new class `ControlParams`
(explained further), and an optional stream for logging
(in this example, `cout`).
`run()` returns an `AlgorithmStatus` object with information about the
optimization like total time, iteration counting, and more (check the full
documentation for that).

So, users need no more write fine control loops unless they need/want.
If you like, just set some control parameters 
(and some other callbacks, described below), and you are good to go!

Supporting `run()`, we have three new methods:

- `setStoppingCriteria()`: while method `run()` sets automatically maximum time
  or maximum stalled iterations (without improvement in the best solution) as
  standard stopping criteria, the user can add to these other criteria using
  this method. For example, in a minimization problem, we may want to stop at
  the value within a distance from a lower bound, or when we reach a given number
  of iterations:

  ```cpp
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
  ```

  In this case, the stop criteria become:
  - Is the maximum time reached **OR**
  - Is the maximum stalled iterations reached **OR**
  - `status.best_fitness <= lower_bound * 1.1` **OR**
  - `status.current_iteration == max_iterations`.

  Note that BRKGA-MP-IPR always tests against time and stalled iterations to
  avoid hanging up. However, this behavior can be changed by modifying the
  maximum time and maximum stalled iterations in the control parameters.

  | :memo: Note                |
  |:---------------------------|
  | If you are using implicit path relinking (IPR), which is **very time-consuming**, we **STRONGLY RECOMMEND SETTING A MAXIMUM TIME** since this is the core stopping criterion on IPR.|

  If you really mean to have no maximum time and/or maximum stalled iterations
  set, we recommend using the following code:

  ```cpp
  // After reading your parameters, e.g.,
  // auto [brkga_params, control_params] = readConfiguration("config.conf");

  // You can set the time to max...
  control_params.maximum_running_time = std::chrono::seconds::max();

  // ... and/or the stalled iterations to max.
  control_params.stall_offset = numeric_limits<unsigned>::max();
  ```

- `addNewSolutionObserver()`: This method adds a callback that is triggered
  when the algorithm finds an overall improved solution. It also
  adds stop point if the user finds it useful by returning `true`.
  This is very useful for tracking the evolution of the algorithm, for instance:

  ```cpp
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
  ```

- `setShakingMethod()`: This method adds a custom shaking procedure
  that the user has defined. Please,
  [refer to its documentation for more details](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#providing-custom-shake-procedure).

Less important but still relevant: previously, one must call `initialize()`
before any method that manipulated the population. Also, since `initialize()`
(re)decodes the population, we have to measure its running time too. Now,
the user **does not need to call `initialize()` anymore!!**.
`initialize()` is called on the need by its fellow methods internally. This
leads to fewer error-prone codes.

### BRKGA and control parameters

Although this is part of API enhancement, it deserves special attention. Now,
we include all BRKGA and IPR parameters into `BrkgaParams`, and all (external)
control parameters into `ControlParams` (which was renamed from
`ExternalControlParams`). In doing so, we have a consistent set that can be
fully loaded from configuration files.

Not all parameters are required, and those not are set to their default values.
The new reading function `readConfiguration()` will emit a warning when
no-required parameters are not set.

| :warning: Warning          |
|:---------------------------|
| If you are using IPR, we **STRONGLY RECOMMEND SETTING A MAXIMUM TIME** since this is the core stopping criterion on IPR.

### Code modernizing and speed bump

The code has been modernized using C++20 facilities like concepts and ranges.
Therefore, your compiler must support C++20 now.

One notable change was substituting the custom code in `randInt()` for a
[standard library uniform distribution
utility](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution).
The old code was used when a custom Mersenne Twister RNG code (from
the original BRKGA implementation) was used. Including the Mersenne Twister in
the standard library allows us to use all default utilities. Ad hoc tests show
that the standard library utilities are faster than the old custom code.
However, the speed-up is marginal when considering the full application of
BRKGA. But, when we accumulate hundreds or thousands of calls daily, the time
savings can be considerable in a full year of operation (which usually
translates into energy savings).

:high_brightness: What is new in version 2.0
--------------------------------------------------------------------------------

### Multiple objectives (Lexicographically)

In version 2.0, BRKGA-MP-IPR also deals with multiple objectives in a
lexicographical or priority dominance order. Unlike classical
non-dominance order (using Pareto frontiers), the lexicographical order defines
a _strict preference order among the objective functions._ This leads us to a
partial ordering of the values of the solutions (composed of several values,
each one from one objective function). So, we have the following definition
(abusing a little bit of notation).

| :memo: Definition          |
|:---------------------------|
| Let $A = (f_1, f_2, \ldots, f_n)$ and $A' = (f'_1, f'_2, \ldots, f'_n)$ be two vectors for $n$ functions $f_1, f_2, \ldots, f_n$. $A$ is lexicographical smaller than $A'$, i.e., $A < A'$ if and only if $f_1 < f'_1$, or $f_1 = f'_1$ and $f_2 < f'_2$, or $`\ldots, f_1 = f'_1, \ldots f_{n-1} = f'_{n-1}`$ and $f_n < f'_n$.|

For instance, let's assume we have three minimizing objective functions and
four solutions that are described in the following table:

|Solution|$f_1$      |$f_2$      |$f_3$
|--------|-----------|-----------|----------|
|A       |50         |30         |30
|B       |30         |55         |40
|C       |30         |20         |50
|D       |30         |20         |25

Note that Solution B is better than Solution A because $f_1(A) < f_1(B)$,
even though A has much better values for $f_2$ and $f_3$. Now,
Solution C is better B because, although $f_1(B) = f_1(C)$, we have that
$f_2(B) < f_2(C)$, regardless of the value of $f_3$. Solution D
has the best value for all objective functions.
Therefore $D < C < B < A$.

| :warning: Warning          |
|:---------------------------|
| If you really want an algorithm to produce a **non-dominated set of solutions (Pareto frontier)**, this is **NOT** the right algorithm for you. We recommend taking a look at the [NSGA-II](https://doi.org/10.1109/4235.996017) and [MOAB](https://en.wikipedia.org/wiki/MOEA_Framework).

### Multi-thread mating

One of the nice additions to BRKGA-MP-IPR 2.0 is the capability of performing
the mating in parallel. Such capability speeds up the algorithm substantially,
mainly for large chromosomes and large populations. However, when performing
parallel mating, we have some points regarding reproducibility described in Section
[Multi-thread mating](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#multi-threading)
in the [tutorial](https://ceandrade.github.io/brkga_mp_ipr_cpp).

### API changes

#### New type `fitness_t`

Due to the inclusion of multi-objective optimization capabilities, the user now must
define a type `fitness_t`, and its/decoder must return such a type.
The user can do things like this:

Please, check the details in Sections
["TL;DR - Multi objective"](http://localhost:8000/page_guide.html#tl-dr-multi-objective)
and
["Using BRKGA-MP-IPR on multi-objective mode"](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#using-brkga-mp-ipr-on-multi-objective-mode)
from the [tutorial](https://ceandrade.github.io/brkga_mp_ipr_cpp).

#### New `BRKGA::setInitialPopulation()`

In the previous versions, `setInitialPopulation()` fills up only the first
population, discarding additional warm solutions if the population is full.
Now, `setInitialPopulation()` fills up all populations.
[More details here.](https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a59b05650ede92f5e0107ab606ff6e8b7)

#### Changes in `BRKGA::injectChromosome()`

`injectChromosome()` does not accept an initial fitness value anymore. Now, `injectChromosome()` triggers the decoder in all cases (and therefore, time must be measured on its call).
[More details here.](https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a0347f67b59bfe36856d1c27c95d4b151)

#### Pump to C++ 17

BRKGA-MP-IPR now uses some features of [C++17 standards](https://en.wikipedia.org/wiki/C%2B%2B17).
Therefore, you must change your building tools to support that.

### Bug fixes

- `BrkgaParams::mutants_percentage` should be of type `double` not `unsigned` ([issue #1]( https://github.com/ceandrade/brkga_mp_ipr_cpp/issues/1)). Thanks @sohaibafifi.
- Fix the shorter method call for `BRKGA_MP_IPR::pathRelinking` ([pull #1](https://github.com/ceandrade/brkga_mp_ipr_cpp/pull/2)). Thanks @afkummer.

:computer: Installation
--------------------------------------------------------------------------------

BRKGA-MP-IPR is a header-only framework, which means that you only need to
download the header files and tell your compiler to include the path to where
the files were downloaded.

Quick example (Unix): assume we are in an empty folder. So, we copy/clone
BRKGA-IPR-MP first:

```terminal
  $ git clone https://github.com/ceandrade/brkga_mp_ipr_cpp
  Cloning into 'brkga_mp_ipr_cpp'...
  remote: Enumerating objects: 118, done.
  remote: Counting objects: 100% (118/118), done.
  remote: Compressing objects: 100% (112/112), done.
  remote: Total 118 (delta 24), reused 0 (delta 0)
  Receiving objects: 100% (118/118), 3.50 MiB | 3.66 MiB/s, done.
  Resolving deltas: 100% (24/24), done.
```

Let's write a `test.cpp` with the following code:

```cpp
#include "brkga_mp_ipr.hpp"
#include <iostream>

int main() {
    std::cout << "Testing sense: " << BRKGA::Sense::MINIMIZE;
    return 0;
}
```

Then, let's compile and see if it works:

```terminal
  $ g++ --version
  g++ (MacPorts gcc12 12.3.0_0+stdlib_flag) 12.3.0

  $ g++ -std=c++20 -Ibrkga_mp_ipr/brkga_mp_ipr test.cpp -o test

  $ ./test
  Testing sense: MINIMIZE
```

Note that Git clones the whole repository containing the library code,
documents, and examples. All the examples were built using
[GNU/Make](https://www.gnu.org/software/make) and
[GCC toolchain](https://gcc.gnu.org). However, the code is standard C++,
and we can quickly adapt it to other toolchains such as
[Clang](https://clang.llvm.org),
[Microsoft](https://visualstudio.microsoft.com), or
[Intel](https://software.intel.com/en-us/c-compilers) toolchains.

:zap: Usage - TL;DR, Single objective
--------------------------------------------------------------------------------

The best way to keep it short is to look at the examples on
[the git repo.](https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples)
Let's start solving the traditional single-objective
[Traveling Salesman Problem (TSP)](https://en.wikipedia.org/wiki/Travelling_salesman_problem).
First, we must ensure that
[BRKGA::fitness_t](https://ceandrade.github.io/brkga_mp_ipr_cpp/namespace_BRKGA.html#doxid-namespace-b-r-k-g-a-1ae9551fcbbfd6072b95e5f112e3b1565e)
has the right single-object type. Let's take a look at the trimmed version of
[`fitness_type.hpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/fitness_type.hpp).
This is a trimmed copy:

```cpp
#include <limits>
#include <tuple>

namespace BRKGA {

using fitness_t = double;

//...
} // end namespace BRKGA_MP_IPR
```

Here, `fitness_t` defines the type of the objective function value. In the vast
majority of the cases, `double` suffices. Let's take a look at the main call
[main_minimal.cpp](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_minimal.cpp).
This is a trimmed copy:

```cpp
#include "tsp/tsp_instance.hpp"
#include "decoders/tsp_decoder.hpp"
#include "brkga_mp_ipr.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

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
        ////////////////////////////////////////
        // Read command-line arguments and the instance
        ////////////////////////////////////////

        const unsigned seed = stoi(argv[1]);
        const string config_file = argv[2];
        const string instance_file = argv[4];
        const unsigned num_threads = 4;

        cout << "Reading data..." << endl;
        auto instance = TSP_Instance(instance_file);

        ////////////////////////////////////////
        // Read algorithm parameters
        ////////////////////////////////////////

        cout << "Reading parameters..." << endl;

        auto [brkga_params, control_params] =
            BRKGA::readConfiguration(config_file);

        // Overwrite the maximum time from the config file.
        control_params.maximum_running_time = chrono::seconds {stoi(argv[3])};

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////

        cout << "Building BRKGA data and initializing..." << endl;

        TSP_Decoder decoder(instance);

        BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
            decoder, BRKGA::Sense::MINIMIZE, seed,
            instance.num_nodes, brkga_params, num_threads
        );

        ////////////////////////////////////////
        // Find good solutions / evolve
        ////////////////////////////////////////

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
```

You can identify the following basic steps:

1. Create a data structure to hold your input data. This object should be passed
   to the decoder object/functor (example
   [`tsp/tsp_instance.hpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/tsp/tsp_instance.hpp));

2. Certify that [BRKGA::fitness_t](https://ceandrade.github.io/brkga_mp_ipr_cpp/namespace_BRKGA.html#doxid-namespace-b-r-k-g-a-1ae9551fcbbfd6072b95e5f112e3b1565e)
   has the correct type;

3. Implement a decoder object/functor. This function translates a chromosome
   (an array of numbers in the interval [0, 1]) to a solution for your problem.
   The decoder must return the solution value or cost to be used as fitness
   by BRKGA (example
   [`decoders/tsp_decoder.hpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/decoders/tsp_decoder.hpp));

4. Load the instance and other relevant data;

5. Read the algorithm parameters using `BRKGA::readConfiguration()`; or
   create a `BRKGA::BrkgaParams` and `BRKGA::ControlParams` objects by hand;

6. Create an `BRKGA::BRKGA_MP_IPR` algorithm object;

7. Call `BRKGA::BRKGA_MP_IPR::run()` to optimize.

[`main_minimal.cpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_minimal.cpp)
provides a very minimal example to understand the necessary steps to use the
BRKGA-MP-IPR framework. However,
[`main_complete.cpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/single_obj/main_complete.cpp)
provides a full-featured code, handy for scientific use, such as
experimentation and paper writing. This code allows fine-grained optimization control, 
showing several features of BRKGA-MP-IPR, such as the resets,
chromosome injection, and others. It also logs
all optimization steps, _creating outputs easy to be parsed._ 
**You should use this code for serious business and experimentation.**

These are the basic steps, but I do recommend the entire reading of [the guide](https://ceandrade.github.io/brkga_mp_ipr_cpp).

:zap: Usage - TL;DR, Multi objective
--------------------------------------------------------------------------------

| :warning: Warning          |
|:---------------------------|
| Again, if you really want an algorithm to produce a **non-dominated set of solutions (Pareto frontier)**, this is **NOT** the right algorithm for you. We recommend taking a look at the [NSGA-II](https://doi.org/10.1109/4235.996017) and [MOAB](https://en.wikipedia.org/wiki/MOEA_Framework).

To use BRKGA-MP-IPR in the multi-objective mode, we first must set
[BRKGA::fitness_t](https://ceandrade.github.io/brkga_mp_ipr_cpp/namespace_BRKGA.html#doxid-namespace-b-r-k-g-a-1ae9551fcbbfd6072b95e5f112e3b1565e)
according to the number of objectives we want. In the
[repo example](https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples/tsp/src/multi_obj),
we consider the TSP with two objectives: first, we must minimize the total tour
length, and second, the size of the largest edge in the tour. For that, we must
change the file
[fitness_type.hpp](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/brkga_mp_ipr/fitness_type.hpp)
to reflect such a thing. In this example, we use the standard `std::tuple`:

```cpp
#include <limits>
#include <tuple>

namespace BRKGA {

using fitness_t = std::tuple<double, double>;

//...
} // end namespace BRKGA_MP_IPR
```

In this case, the first component of the tuple holds the tour length, and the
second contains the largest edge. On Section
[Using BRKGA-MP on multi-objective mode](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#using-brkga-mp-ipr-on-multi-objective-mode),
we talk in more detail about multi-objective problems. Remember that
although you could use any type for your `fitness_t`, you should prefer to use
`std::tuple`.

The remaining code is almost identical to the single-objective. The only
differences are computing the largest edge, and printing such information on
the main call. All the steps described briefly in the
[previous section](#zap-usage---tldr-single-objective) are also used here.

:books: Tutorial and full documentation
--------------------------------------------------------------------------------

We provide a very thorough and easy-to-follow tutorial on using BRKGA effectively.
We strongly recommend that you read the tutorial to make sure your code works 
at best within BRKGA.

Check out the tutorial and API documentation:
<https://ceandrade.github.io/brkga_mp_ipr_cpp>

:black_nib: License and Citing
--------------------------------------------------------------------------------

BRKGA-MP-IPR uses a permissive BSD-like license.
And since this framework is also part of an academic effort, 
please remember to cite the originating paper of this work.
Indeed, Clause 4 stipulates that "all publications, softwares, or any other
materials mentioning features or use of this software (as a whole package or
any parts of it) and/or the data used to test it must cite the following
article explicitly":

> C.E. Andrade. R.F. Toso, J.F. Gonçalves, M.G.C. Resende. The Multi-Parent
> Biased Random-key Genetic Algorithm with Implicit Path Relinking. _European
> Journal of Operational Research_, volume 289, number 1, pages 17–30, 2021.
> DOI [10.1016/j.ejor.2019.11.037](https://doi.org/10.1016/j.ejor.2019.11.037)

If you are using the multi-objective version, you must also cite this paper:

> C.E. Andrade, L.S. Pessoa, S. Stawiarski. The Physical Cell Identity
> Assignment Problem: a Multi-objective Optimization Approach.
> _IEEE Transactions on Evolutionary Computation_, to appear, 2022.
> DOI: [10.1109/TEVC.2022.3185927](https://doi.org/10.1109/TEVC.2022.3185927).

[Check it out the full license.](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md)

You may also find it very useful to read these reviews:

- [Biased random-key genetic algorithms: A review](https://doi.org/10.1016/j.ejor.2024.03.030)
- [Early years of Biased Random-Key Genetic Algorithms: A systematic review](https://doi.org/10.1007/s10898-024-01446-5)

You can download all references for this
[:open_file_folder: Bibtex](https://ceandrade.github.io/brkga_mp_ipr_cpp/_downloads/9e03f77296678467b193eb411a9ebc25/references.bib),
or this
[:open_file_folder: RIS](https://ceandrade.github.io/brkga_mp_ipr_cpp/_downloads/89ca66b5c665a51fcb314b01f33d167e/references.ris)
files.

You may also consider citing the following papers from people who helped
to find bugs and develop new features for BRKGA-MP-IPR 2.0:

> A.F. Kummer N., L.S. Buriol, O.C.B. de Araujo. A biased random key genetic
> algorithm applied to the VRPTW with skill requirements and synchronization
> constraints. *Proceedings of the 2020 Genetic and Evolutionary Computation
> Conference (GECCO '20)*, pages 717-724, 2020.
> DOI: [10.1145/3377930.3390209](https://doi.org/10.1145/3377930.3390209).

:muscle: Collaborators
--------------------------------------------------------------------------------

- Alberto Kummer, 2021 (parallel mating).
- Daniele Ferone, 2023, 2015 (bug fix on IPR, bug on initialize()).
- Pedro H.D.B Hokama, 2025 (bug fix on evolution).
- Mário C. San Felice, 2025 (bug fix on evolution).

:construction_worker: TODO
--------------------------------------------------------------------------------

CI and tests side:

- Implement unit tests and certify coverage;
- Configure Travis-Ci correctly, such that we can run tests on Mac OSX and
  Windows too.

:pencil2: Contributing
--------------------------------------------------------------------------------

[Contribution guidelines for this project](CONTRIBUTING.md)
