<div align="center">
  <img src="https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/src_docs/src/assets/logo_name_300.png">
</div>

BRKGA-MP-IPR - C++ version
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

BRKGA-MP-IPR provides a _very easy-to-use_ framework for the
Multi-Parent Biased Random-Key Genetic Algorithm with Implict Path Relink
(**BRKGA-MP-IPR**). Assuming that your have a _decoder_ to your problem,
we can setup, run, and extract the value of the best solution in less than
5 commands (obvisiously, you may need few other lines fo code to do a proper
test).

This C++ version provides a fast prototyping API using C++14 standards and
libraries. All code was developed as a header-only library, and have no
external dependencies other than those included in the package. So, you just
need to copy/check out the files and point your compiler's header path to
BRKGA-MP-IPR folder (`-I` on G++ and CLANG++).

This framework can use multiple threads of modern CPUs, by setting a single
parameter (assuming that your decoder is thread-safe). This leverage the
parallel decoding nature that BRKGAs offer, compared to other (meta)
heuristic frameworks.

The code can be compiled using [> GCC 7.2](https://gcc.gnu.org) and [> Clang
6.0](https://clang.llvm.org), and it is very probable that it can be compiled
using other modern C++ compilers, such as Intel and Microsoft compilers. To
use multiple threads, your compiler must support
[OpenMP](https://www.openmp.org). The current version has been long developed,
and it is a very mature code used in production in several companies.
However, it lacks of a proper unit and coverage testing. Such tests are in
the TODO list.

If C++ is not suitable to you, we may find useful the [**Julia
version**](https://github.com/ceandrade/brkga_mp_ipr_julia) of this framework.
We are also developing a
[**Python version**](https://github.com/ceandrade/brkga_mp_ipr_python)
which is in its earlier stages.
At this moment, we have no plans to implement the BRKGA-MP-IPR in other
languages such as Java or C#. But if you want to do so, you are
must welcome. But please, keep the API as close as possible to the C++ API
(or Julia API in case you decide go C), and use the best coding and
documentation practices of your chosen language/framework.

- [**Julia version**](https://github.com/ceandrade/brkga_mp_ipr_julia)
- [**Python version**](https://github.com/ceandrade/brkga_mp_ipr_python)

If you are not familiar with how BRKGA works, take a look on
[Standard BRKGA](http://dx.doi.org/10.1007/s10732-010-9143-1) and
[Multi-Parent BRKGA](http://dx.doi.org/xxx).
In the future, we will provide a _Prime on BRKGA-MP_
section.

:computer: Installation
--------------------------------------------------------------------------------

BRKGA-MP-IPR is a header-only framework, which means that you only need to
download the header files and tell your compiler to include the path to where
the files were downloaded.

Quick example (unix): assume we are in an empty folder. So, we copy/clone
BRKGA-IPR-MP first:

    $ git clone https://github.com/ceandrade/brkga_mp_ipr_cpp
    Cloning into 'brkga_mp_ipr_cpp'...
    remote: Enumerating objects: 118, done.
    remote: Counting objects: 100% (118/118), done.
    remote: Compressing objects: 100% (112/112), done.
    remote: Total 118 (delta 24), reused 0 (delta 0)
    Receiving objects: 100% (118/118), 3.50 MiB | 3.66 MiB/s, done.
    Resolving deltas: 100% (24/24), done.

Let's write a `test.cpp` with the following code:

```cpp
#include "brkga_mp_ipr.hpp"
#include <iostream>

int main() {
    std::cout << "Testing sense: " << BRKGA::Sense::MINIMIZE;
    return 0;
}
```

Then, let's compile and see it works:

    $ g++ --version
    g++ (MacPorts gcc8 8.2.0_3) 8.2.0

    $ g++ -std=c++14 -Ibrkga_mp_ipr/brkga_mp_ipr test.cpp -o test

    $ ./test
    Testing sense: MINIMIZE

Note the Git clones the whole repository that contains the library code,
documents, and examples. All the examples were built using
[GNU/Make](https://www.gnu.org/software/make/) and
[GCC toolchain](https://gcc.gnu.org). However, the code is standard C++,
and we can quickly adapt it to other toolchains such as
[Clang](https://clang.llvm.org),
[Microsoft](https://visualstudio.microsoft.com), or
[Intel](https://software.intel.com/en-us/c-compilers) toolchains.
To build this documentation, we also need
[Doxygen](http://www.doxygen.nl).

:zap: Usage - TL;DR
--------------------------------------------------------------------------------

The best way to keep it short is to look in the
on examples on [the git repo.](https://github.com/ceandrade/brkga_mp_ipr_cpp/tree/master/examples)
Let's take a look into
[`main_minimal.cpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_minimal.cpp),
which solves the
[Traveling Salesman Problem (TSP)](https://en.wikipedia.org/wiki/Travelling_salesman_problem).
This is a trimmed copy:

```cpp
#include "tsp/tsp_instance.hpp"
#include "decoders/tsp_decoder.hpp"
#include "brkga_mp_ipr.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 4) {
        cerr << "Usage: "<< argv[0]
             << " <seed> <config-file> <num-generations>"
                " <tsp-instance-file>" << endl;
        return 1;
    }

    const unsigned seed = stoi(argv[1]);
    const string config_file = argv[2];
    const unsigned num_generations = stoi(argv[3]);
    const string instance_file = argv[4];

    auto instance = TSP_Instance(instance_file);

    auto params = BRKGA::readConfiguration(config_file);
    auto& brkga_params = params.first;

    TSP_Decoder decoder(instance);

    BRKGA::BRKGA_MP_IPR<TSP_Decoder> algorithm(
            decoder, BRKGA::Sense::MINIMIZE, seed,
            instance.num_nodes, brkga_params);

    algorithm.initialize();

    algorithm.evolve(num_generations);

    auto best_cost = algorithm.getBestFitness();
    cout << "Best cost: " << best_cost;

    return 0;
}
```

You can identify the following basic steps:

1. Create a data structure to hold your input data. This object should be passed
   to the decoder object/functor (example
   [`tsp/tsp_instance.hpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/tsp/tsp_instance.hpp));

2. Implement a decoder object/functor. This function translates a chromosome
   (array of numbers in the interval [0,1]) to a solution for your problem. The
   decoder must return the solution value or cost to be used as fitness by BRKGA
   (example
   [`decoders/tsp_decoder.hpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/decoders/tsp_decoder.hpp));

3. Load the instance and other relevant data;

4. Read the algorithm parameters using `BRKGA::readConfiguration()`; or
   create a `BRKGA::BrkgaParams` object by hand;

5. Create an `BRKGA::BRKGA_MP_IPR` algorithm object;

6. Call `BRKGA::BRKGA_MP_IPR::initialize()` to init the BRKGA state;

7. Call `BRKGA::BRKGA_MP_IPR::evolve()` to optimize;

8. Call `BRKGA::BRKGA_MP_IPR::getBestFitness()` and/or
   `BRKGA::BRKGA_MP_IPR::getBestChromosome()` to retrieve the best solution.

[`main_minimal.cpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_minimal.cpp)
provides a very minimal example to understand the necessary steps to use the
BRKGA-MP-IPR framework. However,
[`main_complete.cpp`](https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/examples/tsp/src/main_complete.cpp)
provides a full-featured code, handy for scientific use, such as
experimentation and paper writing. This code allows fine-grained control of
the optimization, shows several features of BRKGA-MP-IPR such as the resets,
chromosome injection, and others. It also logs
all optimization steps, _creating outputs easy to be parsed._ **You should use
this code for serious business and experimentation.**

:books: Tutorial and full documentation
--------------------------------------------------------------------------------

Check out the tutorial and API documentation:
https://ceandrade.github.io/brkga_mp_ipr_cpp

:black_nib: License and Citing
--------------------------------------------------------------------------------

BRKGA-MP-IPR uses a permissive BSD-like license and it can be used as it
pleases you. And since this framework is also part of an academic effort, we
kindly ask you to remember to cite the originating paper of this work.
Indeed, Clause 4 estipulates that "all publications, softwares, or any other
materials mentioning features or use of this software (as a whole package or
any parts of it) and/or the data used to test it must cite the following
article explicitly:":

> C.E. Andrade. R.F. Toso, J.F. Gonçalves, M.G.C. Resende. The Multi-Parent
> Biased Random-key Genetic Algorithm with Implicit Path Relinking. _European
> Jornal of Operational Research_, To appear, 2019.
> DOI https://doi.org/10.1016/j.ejor.2019.11.037

[Check it out the full license.](https://github.com/ceandrade/brkga_mp_ipr_julia/blob/master/LICENSE.md)

:construction_worker: TODO
--------------------------------------------------------------------------------

CI and tests side:

- Implement unit tests and certify coverage;

- Configure Travis-Ci correctly, such that we can run tests on Mac OSX and
  Windows too.

:pencil2: Contributing
--------------------------------------------------------------------------------

[Contribution guidelines for this project](CONTRIBUTING.md)
