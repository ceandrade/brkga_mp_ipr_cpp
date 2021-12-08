.. index:: pair: page; BRKGA-MP-IPR Guide and Documentation - C++ Version
.. _doxid-indexpage:

BRKGA-MP-IPR Guide and Documentation - C++ Version
===============================================================================

BRKGA-MP-IPR provides a *very easy-to-use* framework for the Multi-Parent
Biased Random-Key Genetic Algorithm with Implict Path Relink
(**BRKGA-MP-IPR**). Assuming that your have a *decoder* to your problem, we can
setup, run, and extract the value of the best solution in less than 5 commands
(obvisiously, you may need few other lines fo code to do a proper test).

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
    <https://en.wikipedia.org/wiki/MOEA_Framework>`_.

If you are not familiar with how BRKGA works, take a look on `Standard BRKGA
<http://dx.doi.org/10.1007/s10732-010-9143-1>`_ and `Multi-Parent BRKGA
<https://doi.org/10.1016/j.ejor.2019.11.037>`_.
If you know what *elite set*, *decoder*,
and so means, we can get to the guts on the :ref:`Guide <doxid-guide>`.


The implementation
-------------------------------------------------------------------------------

This C++ version provides a fast prototyping API using C++17 standards and
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
`Julia version <https://github.com/ceandrade/brkga_mp_ipr_julia>`_.
We are also developing a
`Python version <https://github.com/ceandrade/brkga_mp_ipr_python>`_
which is in its earlier stages.
At this moment, we have no plans to implement the BRKGA-MP-IPR in other
languages such as Java or C#. But if you want to do so, you are must welcome.
But please, keep the API as close as possible to the C++ API (or Julia API in
case you decide go C), and use the best coding and documentation practices of
your chosen language/framework.


License and Citing
-------------------------------------------------------------------------------

BRKGA-MP-IPR uses a permissive BSD-like license and it can be used as it
pleases you. And since this framework is also part of an academic effort, we
kindly ask you to remember to cite the originating paper of this work.
Indeed, Clause 4 estipulates that "all publications, softwares, or any other
materials mentioning features or use of this software (as a whole package or
any parts of it) and/or the data used to test it must cite the following
article explicitly":

.. admonition:: Reference

    C.E. Andrade, R.F. Toso, J.F. Gonçalves, M.G.C. Resende. The Multi-Parent
    Biased Random-key Genetic Algorithm with Implicit Path Relinking. *European
    Journal of Operational Research*, volume 289, number 1, pages 17–30, 2021.
    DOI:
    `10.1016/j.ejor.2019.11.037 <https://doi.org/10.1016/j.ejor.2019.11.037>`_.

If you are using the multi-objective version, you must also cite this paper:

.. admonition:: Reference

    C.E. Andrade, L.S. Pessoa, S. Stawiarski. The Physical Cell Identity
    Assignment Problem: a Multi-objective Optimization Approach.
    *IEEE Transactions on Evolutionary Computation*, volume XXX, number X,
    pages XX–XX, 2022.
    DOI:
    `10.1016/j.ejor.2019.11.037 <https://doi.org/10.1016/j.ejor.2019.11.037>`_.

You may also consider to cite the following papers from people that helped
to find bugs and develop new features for BRKGA-MP-IPR 2.0:

.. admonition:: Reference

    A.F. Kummer N., L.S. Buriol, O.C.B. de Araujo. A biased random key genetic
    algorithm applied to the VRPTW with skill requirements and synchronization
    constraints. *Proceedings of the 2020 Genetic and Evolutionary Computation
    Conference (GECCO '20)*, pages 717-724, 2020.
    DOI:
    `10.1145/3377930.3390209 <https://doi.org/10.1145/3377930.3390209>`_.

You can download all references for this
:download:`Bibtex <../assets/references.bib>`, or this
:download:`RIS <../assets/references.ris>` files.


About the logo
-------------------------------------------------------------------------------

The logo is just a play with 3 chromosomes crossing with each other
(multi-parent) during the mating process. The lines also represent solutions
paths that encounter with each other generating new solutions during the
path-relink.
