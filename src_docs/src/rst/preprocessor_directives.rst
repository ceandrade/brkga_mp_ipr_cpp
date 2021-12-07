.. index:: pair: BRKGA_MULTIPLE_INCLUSIONS; INLINE

.. _doxid-brkga__mp__ipr_8hpp_preprocessor_directives:

Preprocessor directives
=======================


.. _doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116:

BRKGA_MULTIPLE_INCLUSIONS
-------------------------

Overview
~~~~~~~~

If supplied, ``BRKGA_MULTIPLE_INCLUSIONS`` inlines the functions and template
specializations. Please, refer to Section
:ref:`Known issues <doxid-guide_1guide_known_issues>`
for more details.

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#ifdef BRKGA_MULTIPLE_INCLUSIONS
	#define :ref:`INLINE<doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116>` inline
	#else
	#define INLINE
	#endif


.. _doxid-brkga__mp__ipr_8hpp_mating_flags:

Multi-thread Mating flags
-------------------------

Overview
~~~~~~~~

These preprocessor flags determine how the mating process will happen
regarding reproducibility. One of the following options should be used.
If more than one is given, ``MATING_FULL_SPEED`` takes priority over
``MATING_SEED_ONLY``, which takes priority over ``MATING_SEQUENTIAL``.
If no option is supplied, BRKGA-MP-IPR assume ``MATING_FULL_SPEED``.
Please, refer to Section
:ref:`Multi-thread mating <doxid-guide_1guide_tips_multi_thread_mating>`
for more details.

.. ref-code-block:: cpp
	:class: doxyrest-overview-code-block

	#if defined(MATING_FULL_SPEED)
		#undef MATING_SEQUENTIAL
		#undef MATING_SEED_ONLY
	#elif defined(MATING_SEED_ONLY)
		#undef MATING_SEQUENTIAL
		#undef MATING_FULL_SPEED
	#elif defined(MATING_SEQUENTIAL)
		#undef MATING_SEQUENTIAL
		#undef MATING_FULL_SPEED
	#else
		#define MATING_FULL_SPEED
	#endif


.. _doxid-brkga__mp__ipr_8hpp_mating_full_speed:

MATING_FULL_SPEED
~~~~~~~~~~~~~~~~~

Using ``MATING_FULL_SPEED``, the mating process is done in parallel, using
independent pseudo-random number generators.

.. warning::

	The results are reproducible **if and only if** you use the same seed
	and the same number of threads.


.. _doxid-brkga__mp__ipr_8hpp_mating_seed_only:

MATING_SEED_ONLY
~~~~~~~~~~~~~~~~~

Using ``MATING_SEED_ONLY``, the mating is still parallel, but each pseudo-random
number generator is seeded on each mating. This is a little bit slower than
``MATING_FULL_SPEED``, but we depend only on the seed, regardless of the number
of threads. I.e., Runs with a different number of threads, but the same seed
should result in the same sequence of decisions.


.. _doxid-brkga__mp__ipr_8hpp_mating_sequential:

MATING_SEQUENTIAL
~~~~~~~~~~~~~~~~~

Using ``MATING_SEQUENTIAL``, the mating process is completely sequential, as in
the original BRKGA. The reproducibility is guaranteed with only the same
seed. This option can be very slow for large chromosomes and large
populations. But it makes debugging easier.
