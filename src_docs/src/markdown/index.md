BRKGA-MP-IPR Guide and Documentation - C++ Version 3.0 {#mainpage}
================================================================================

BRKGA-MP-IPR provides a _very easy-to-use_ framework for the
Multi-Parent Biased Random-Key Genetic Algorithm with Implict Path Relink
(**BRKGA-MP-IPR**). Assuming that your have a _decoder_ to your problem,
we can setup, run, and extract the value of the best solution in less than
5 commands (obvisiously, you may need few other lines fo code to do a proper
test).

We provide a very thorough and easy-to-follow tutorial on using
BRKGA effectively. We strongly recommend you read the tutorial to make
your code works at best within BRKGA. Check out the tutorial and API
documentation: https://ceandrade.github.io/brkga_mp_ipr_cpp

This C++ version provides a fast prototyping API using
[C++20 standards](https://en.wikipedia.org/wiki/C%2B%2B20)
and libraries. All code was developed as a header-only library, and have no
external dependencies other than those included in the package. So, you just
need to copy/check out the files and point your compiler's header path to
BRKGA-MP-IPR folder (`-I` on GCC and Clang).

This framework can use multiple threads of modern CPUs, by setting a single
parameter (assuming that your decoder is thread-safe). This leverage the
parallel decoding nature that BRKGAs offer, compared to other (meta)
heuristic frameworks.

The code can be compiled using [> GCC 9.4](https://gcc.gnu.org) and [> Clang
10.0](https://clang.llvm.org), and it is very probable that it can be compiled
using other modern C++ compilers, such as Intel and Microsoft compilers. To
use multiple threads, your compiler must support
[OpenMP](https://www.openmp.org). The current version has been long developed,
and it is a very mature code used in production in several companies.
However, it lacks of a proper unit and coverage testing.

If C++ is not suitable to you, we may find useful the
[Julia version](https://github.com/ceandrade/brkga_mp_ipr_julia)
of this framework. We are also developing a
[Python version](https://github.com/ceandrade/brkga_mp_ipr_python)
which is in its earlier stages (no IPR yet).
However, both Julia and Python versions only support single-objective optimization
at this moment. We have no timeline to implement multi-objective optimization
in such platforms. Also, at this moment, we have no plans to implement
the BRKGA-MP-IPR in other
languages such as Java or C#. But if you want to do so, you are
must welcome. But please, keep the API as close as possible to the C++ API
(or Julia API in case you decide go C), and use the best coding and
documentation practices of your chosen language/framework.

- [Julia version](https://github.com/ceandrade/brkga_mp_ipr_julia)
- [Python version](https://github.com/ceandrade/brkga_mp_ipr_python)

If you are not familiar with how BRKGA works, take a look on
[Multi-Parent BRKGA](https://doi.org/10.1016/j.ejor.2019.11.037).
In the future, we will provide a _Prime on BRKGA-MP_
section. If you know what _elite set_, _decoder_, and so means,
we can get to the guts on the [Guide](@ref guide).

What is new on version 3.0
--------------------------------------------------------------------------------

### API enhancements

On version 2.0, we claimed that BRKGA-MP-IPR was a very easy-to-use framework.
But, few people told me this statement was not even true. The main complaining
was that while the features were very nice, tightening them together was hard,
even using the provided examples.

Now, BRKGA-MP-IPR supplies a method called `BRKGA::BRKGA_MP_IPR::run()`. It
implements the entire pipeline using all framework features in a chain-like
way, similar to the detailed examples. The user may call in this way:

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

where `control_params` is an instance of the new class `BRKGA::ControlParams`
(explained further), and an optional stream for logging
(in this example, `cout`).
`run()` returns an `BRKGA::AlgorithmStatus` object with information about the
optimization like total time, iteration counting, and more (check the full
documentation for that).

So, users need no more write fine control loops unless they need/want.
Just set some control parameters (and some other callbacks, described below,
if you like), and you are good to go!

Supporting `run()`, we have three new methods:

- `BRKGA::BRKGA_MP_IPR::setStoppingCriteria()`: while method `run()` sets
  automatically maximum time or maximum stalled iterations (without improvement
  in the best solution) as standard stopping criteria, the user can add to
  these other criteria using this method. For instance, the following lambda
  function tests if the best solution reached a given value:

  ```cpp
  fitness_t my_magical_solution = 10;
  algorithm.setStoppingCriteria(
      [&](const AlgorithmStatus& status) {
          return status.best_fitness == my_magical_solution;
      }
  );
  ```

  In this case, the stop criteria become: _Is maximum time reached **OR** Is
  maximum stalled iterations reached **OR**
  `best_fitness == my_magical_solution`._

  **(FIX THIS NOTE ON RST)** While we **STRONGLY RECOMMEND TO SET A MAXIMUM TIME**
  (mainly when using IPR), if you really rmean to have no maximum time or
  maximum stalled iterations set, we recommend to use the following code:

  ```cpp
  // After reading your parameters, e.g.,
  // auto [brkga_params, control_params] = readConfiguration("config.conf");
  // You can set to the max.
  control_params.maximum_running_time = std::chrono::seconds::max();
  control_params.stall_offset = numeric_limits<decltype(control_params.stall_offset)>::max();
  ```

- `BRKGA::BRKGA_MP_IPR::addNewSolutionObserver()`: This method adds a callback
  that is triggered when an overall improved solution is found by the
  algorithm. It also adds an additional stop point if the users finds it useful
  by return `true`. This is very useful for tracking the evolution of the
  algorithm, for instance:

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

- `BRKGA::BRKGA_MP_IPR::setShakingMethod()`: This method adds a custom shaking
  procedure defined by the user. Please, refer to its documentation for more
  details.

Less important but still relevant: previously, one must call `initialize()`
before any method that manipulated the population. Also, since `initialize()`
(re)decodes the population, we have to measure its running time too. Now,
the user **do not need to call `initialize()` anymore!!!**.
`initialize()` is called on the need by its fellow methods internally. This
leads to fewer error-prone codes.

### BRKGA and control parameters

Although this is part of API enhancement, it deserves special attention. Now,
we include all BRKGA and IPR parameters into `BRKGA::BrkgaParams`, and all
(external) control parameters into `BRKGA::ControlParams` (which was renamed
from `ExternalControlParams`). In doing so, we have a consistent set that can
be fully loaded from configuration files.

Not all parameters are required, and those not are set to their default values.
The new reading function `BRKGA::readConfiguration()` will emit a warning when
no-required parameters are not set.

**(FIX THIS WANING ON RST)** If you are using IPR, we **STRONGLY RECOMMEND TO
SET A MAXIMUM TIME** since this is the core stopping criteria on IPR.

### Code modernizing and speed bump

The code has been modernized using
[C++20 facilities](https://en.wikipedia.org/wiki/C%2B%2B20)
like concepts and ranges. Therefore, your compiler must support C++20 now.

One notable change was substituting the custom code in `randInt()` for a
[standard library uniform distribution
utility](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution).
The old code was used when a custom Mersenne Twister RNG code was used (from
the original BRKGA implementation). The inclusion of the Mersenne Twister in
the standard library allows us to use all default utilities. Ad hoc tests show
that the standard library utilities are faster than the old custom code.
However, the speed-up is marginal when considering the full application of
BRKGA. But, when we accumulate hundreds or thousands of calls daily, the time
savings can be considerable in a full year of operation (which usually
translates into energy savings).

What is new on version 2.0
--------------------------------------------------------------------------------

### Multiples objectives (Lexicographically)

In version 2.0, BRKGA-MP-IPR also deals with multiple objectives in a
lexicographical or priority dominance order. Differing from classical
non-dominance order (using Pareto frontiers), the lexicographical order defines
a _strict preference order among the objective functions._ This leads us to a
partial ordering of the values of the solutions (composed of several values,
each one from one objective function). So, we have the following definition
(abusing a little bit of notation).

**(FIX THIS DEFINITION ON RST)** Let `A = (f_1, f_2, ..., f_n)` and
`A' = (f'_1, f'_2, .., f'_n)` be two vectors for `n` functions
`f_1, f_2, ..., f_n`. `A` is lexicographical smaller than `A'`, i.e.,`A < A'`
if and only if `f_1 < f'_1`, or `f_1 = f'_1` and `f_2 < f'_2`, or
`..., f_1 = f'_1, ..., f_{n-1} = f'_{n-1}` and `f_n < f'_n`.|

For instance, let's assume we have three minimizing objective functions and
four solutions described in the following table:

|Solution|`f_1`      |`f_2`      |`f_3`
|--------|-----------|-----------|----------|
|A       |50         |30         |30
|B       |30         |55         |40
|C       |30         |20         |50
|D       |30         |20         |25

Note that Solution B is better than Solution A because `f_1(A) < f_1(B),`
even though A has much better values for `f_2` and `f_3`. Now,
Solution C is better B because, although `f_1(B) = f_1(C),` we have that
`f_2(B) < f_2(C),` regardless of the value of `f_3.` Solution D
has the best value for all objective functions. Therefore `D < C < B <
A.`

**(FIX WARNING NOTE ON RST)**
If you really want an algorithm to produce a **non-dominated set of solutions (Pareto frontier)**,
this is **NOT** the right algorithm for you. We recommend taking a look at
the [NSGA-II](https://doi.org/10.1109/4235.996017) and
[MOAB](https://en.wikipedia.org/wiki/MOEA_Framework).

### Multi-thread mating

One of the nice additions to BRKGA-MP-IPR 2.0 is the capability of performing
the mating in parallel. Such capability speeds up the algorithm substantially,
mainly for large chromosomes and large populations. However, when performing
parallel mating, we have some points regarding reproducibility described in Section
[Multi-thread mating](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#multi-threading)
in the [tutorial](https://ceandrade.github.io/brkga_mp_ipr_cpp).

### API changes

#### New type `BRKGA::fitness_t`

Due to the inclusion of multi-objective optimization capabilities, the user now must
define a type `BRKGA::fitness_t`, and his/her decoder must return such a type.
The user can do things like this:

Please, check for details in Sections
["TL;DR - Multi objective"](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#tl-dr-multi-objective)
and
["Using BRKGA-MP-IPR on multi-objective mode"](https://ceandrade.github.io/brkga_mp_ipr_cpp/page_guide.html#using-brkga-mp-ipr-on-multi-objective-mode)
from the [tutorial](https://ceandrade.github.io/brkga_mp_ipr_cpp).

#### New `BRKGA::BRKGA_MP_IPR::setInitialPopulation()`

In the previous versions, `BRKGA::BRKGA_MP_IPR::setInitialPopulation()` fills
up only the first population, discarding additional warm solutions if the
population is full. Now, `setInitialPopulation()` fills up all populations.
[More details
here.](https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a59b05650ede92f5e0107ab606ff6e8b7)

#### Changes in `BRKGA::BRKGA_MP_IPR::injectChromosome()`

`BRKGA::BRKGA_MP_IPR::injectChromosome()` does not accept initial fitness value
anymore. Now, `injectChromosome()` triggers the decoder in all cases (and
therefore, time must be measured on its call). [More details
here.](https://ceandrade.github.io/brkga_mp_ipr_cpp/class_BRKGA_BRKGA_MP_IPR.html#doxid-class-b-r-k-g-a-1-1-b-r-k-g-a-m-p-i-p-r-1a0347f67b59bfe36856d1c27c95d4b151)

#### Pump to C++ 17

BRKGA-MP-IPR now uses some features of [C++17 standards](https://en.wikipedia.org/wiki/C%2B%2B17).
Therefore, you must change your building tools to support that.

### Bug fixes

- `BRKGA::BrkgaParams::mutants_percentage` should be of type `double` not `unsigned` ([issue #1]( https://github.com/ceandrade/brkga_mp_ipr_cpp/issues/1)). Thanks `@sohaibafifi`.
- Fix the shorter method call for `BRKGA::BRKGA_MP_IPR::pathRelinking()` ([pull #1](https://github.com/ceandrade/brkga_mp_ipr_cpp/pull/2)). Thanks `@afkummer`.

License and Citing
--------------------------------------------------------------------------------

BRKGA-MP-IPR uses a permissive BSD-like license and it can be used as it
pleases you. And since this framework is also part of an academic effort, we
kindly ask you to remember to cite the originating paper of this work.
Indeed, Clause 4 estipulates that "all publications, softwares, or any other
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

You may also consider to cite the following papers from people that helped
to find bugs and develop new features for BRKGA-MP-IPR 2.0:

> A.F. Kummer N., L.S. Buriol, O.C.B. de Araujo. A biased random key genetic
> algorithm applied to the VRPTW with skill requirements and synchronization
> constraints. *Proceedings of the 2020 Genetic and Evolutionary Computation
> Conference (GECCO '20)*, pages 717-724, 2020.
> DOI: [10.1145/3377930.3390209](https://doi.org/10.1145/3377930.3390209).

You can download all references for this
[:open_file_folder: Bibtex](https://ceandrade.github.io/brkga_mp_ipr_cpp/_downloads/9e03f77296678467b193eb411a9ebc25/references.bib),
or this
[:open_file_folder: RIS](https://ceandrade.github.io/brkga_mp_ipr_cpp/_downloads/89ca66b5c665a51fcb314b01f33d167e/references.ris)
files.

Collaborators
--------------------------------------------------------------------------------

- Alberto Kummer, 2021 (parallel mating).
- Daniele Ferone, 2023 (bug fix on IPR).

About the logo
--------------------------------------------------------------------------------

The logo is just a play with 3 chromosomes crossing with each other
(multi-parent) during the mating process. The lines also represent solutions
paths that encounter with each other generating new solutions during the
path-relink.
