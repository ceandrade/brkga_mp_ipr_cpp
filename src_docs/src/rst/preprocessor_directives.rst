.. index:: pair: BRKGA_MULTIPLE_INCLUSIONS; INLINE
.. _doxid-brkga__mp__ipr_8hpp_1a2eb6f9e0395b47b8d5e3eeae4fe0c116:

Preprocessor directive ``BRKGA_MULTIPLE_INCLUSIONS``
====================================================

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
