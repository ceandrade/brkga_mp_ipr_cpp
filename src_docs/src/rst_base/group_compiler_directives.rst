.. index:: pair: group; Compiler directives
.. _doxid-group__compiler__directives:

Compiler directives
===================

.. toctree::
	:hidden:

Overview
~~~~~~~~

Changes the behavior of the code according to the user supplied directives. :ref:`More...<details-group__compiler__directives>`


.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	
	// macros

	#define :ref:`BRKGA_MULTIPLE_INCLUSIONS<doxid-group__compiler__directives_1ga608f9e381594efac6848d962c0110e91>`
	#define :ref:`MATING_FULL_SPEED<doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7>`
	#define :ref:`MATING_SEED_ONLY<doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22>`
	#define :ref:`MATING_SEQUENTIAL<doxid-group__compiler__directives_1ga5766b15b52c6201d58440c53462fdf24>`

.. _details-group__compiler__directives:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Changes the behavior of the code according to the user supplied directives. Usually, pass by flag ``-D`` (in GCC and Clang). For instance,

$ gcc -DBRKGA_MULTIPLE_INCLUSIONS code.cpp -o code.o

Macros
------

.. index:: pair: define; BRKGA_MULTIPLE_INCLUSIONS
.. _doxid-group__compiler__directives_1ga608f9e381594efac6848d962c0110e91:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	#define BRKGA_MULTIPLE_INCLUSIONS

Allows inclusion within multiple translation units.

If we need to include this file in multiple translation units (files) that are compiled separately, we have to ``inline`` some functions and template definitions to avoid multiple definitions and linking problems. However, such inlining can make the object code grows large. In other cases, the compiler may complain about too many inline functions, if you are already using several inline functions.

.. index:: pair: define; MATING_FULL_SPEED
.. _doxid-group__compiler__directives_1ga232080cffcccb7698dd33155c0a534d7:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	#define MATING_FULL_SPEED

Enables full speed parallel mating.

At full speed (MATING_FULL_SPEED), the mating process is done in parallel, using independent RNGs. The results are reproducible if and only if you use the same seed and the same number of threads.

.. index:: pair: define; MATING_SEED_ONLY
.. _doxid-group__compiler__directives_1ga68b3cb7d49905ea1096c35a4467b6c22:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	#define MATING_SEED_ONLY

Enables Seed-only parallel mating.

Using MATING_SEED_ONLY, the mating is still parallel, but each RNG is seeded on each mating. This is a little bit slower than full speed, but we depend only on the seed, regardless of the number of threads. I.e., Runs with a different number of threads, but the same seed should result in the same sequence of decisions.

.. index:: pair: define; MATING_SEQUENTIAL
.. _doxid-group__compiler__directives_1ga5766b15b52c6201d58440c53462fdf24:

.. ref-code-block:: cpp
	:class: doxyrest-title-code-block

	#define MATING_SEQUENTIAL

Enables sequential mating.

Using MATING_SEQUENTIAL, the mating process is completely sequential, as in the original :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`. The reproducibility is guaranteed with only the same seed. This option can be very slow for large chromosomes and large populations. But it makes debugging easier.

