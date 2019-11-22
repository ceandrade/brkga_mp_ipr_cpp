.. index:: pair: page; BRKGA-MP-IPR Guide and Documentation - C++ Version
.. _doxid-indexpage:

BRKGA-MP-IPR Guide and Documentation - C++ Version
===============================================================================

BRKGA-MP-IPR provides a *very easy-to-use* framework for the Multi-Parent
Biased Random-Key Genetic Algorithm with Implict Path Relink
(**BRKGA-MP-IPR**). Assuming that your have a *decoder* to your problem, we can
setup, run, and extract the value of the best solution in less than 5 commands
(obvisiously, you may need few other lines fo code to do a proper test).

This C++ version provides a fast prototyping API using C++14 standards and
libraries. All code was developed as a header-only library, and have no
external dependencies other than those included in the package. So, you just
need to copy/check out the files and point your compiler's header path to
BRKGA-MP-IPR folder (``-I`` on G++ and CLANG++).

This framework can use multiple threads of modern CPUs, by setting a single
parameter (assuming that your decoder is thread-safe). This leverage the
parallel decoding nature that BRKGAs offer, compared to other (meta) heuristic
frameworks.

The code can be compiled using `> GCC 7.2 <https://gcc.gnu.org>`_ and `> Clang
6.0 <https://clang.llvm.org>`_, and it is very probable that it can be
compiled using other modern C++ compilers, such as Intel and Microsoft
compilers. To use multiple threads, your compiler must support `OpenMP
<https://www.openmp.org>`_. The current version has been long developed, and
it is a very mature code used in production in several companies. However, it
lacks of a proper unit and coverage testing. Such tests are in the TODO list.

If C++ is not suitable to you, we may find useful the
`Julia version <https://github.com/ceandrade/brkga_mp_ipr_julia>`_
of this framework
or the `Python version <https://github.com/ceandrade/brkga_mp_ipr_python>`_
which is in its earlier stages.
At this moment, we have no plans to implement the BRKGA-MP-IPR in other
languages such as Java or C#. But if you want to do so, you are
must welcome. But please, keep the API as close as possible to the C++ API
(or Julia API in case you decide go C), and use the best coding and
documentation practices of your chosen language/framework

If you are not familiar with how BRKGA works, take a look on `Standard BRKGA
<http://dx.doi.org/10.1007/s10732-010-9143-1>`_ and `Multi-Parent BRKGA
<http://dx.doi.org/xxx>`_. In the future, we will provide a *Prime on
BRKGA-MP* section. If you know what *elite set*, *decoder*, and so means, we
can get to the guts on the :ref:`Guide <doxid-guide>`.


License and Citing
-------------------------------------------------------------------------------

BRKGA-MP-IPR uses a permissive BSD-like license and it can be used as it
pleases you. And since this framework is also part of an academic effort, we
kindly ask you to remember to cite the originating paper of this work.
Indeed, Clause 4 estipulates that "all publications, softwares, or any other
materials mentioning features or use of this software (as a whole package or
any parts of it) and/or the data used to test it must cite the following
article explicitly:":

  C.E. Andrade. R.F. Toso, J.F. Gonçalves, M.G.C. Resende. The Multi-Parent
  Biased Random-key Genetic Algorithm with Implicit Path Relinking and its
  real-world applications.  *European Journal of Operational Research*,
  volume XX, issue X, pages xx-xx, 2019.
  DOI `to be determined <http://dx.doi.org/xxx>`_


About the logo
-------------------------------------------------------------------------------

The logo is just a play with 3 chromosomes crossing with each other
(multi-parent) during the mating process. The lines also represent solutions
paths that encounter with each other generating new solutions during the
path-relink.

