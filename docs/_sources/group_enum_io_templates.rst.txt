.. index:: pair: group; Template specializations for enum I/O
.. _doxid-group__enum__io__templates:

Template specializations for enum I/O
=====================================

.. toctree::
	:hidden:

	class_EnumIO.rst


Using slightly modified template class provided by Bradley Plohr (`https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header <https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header>`__) we specialize that template to enums in the :ref:`BRKGA <doxid-namespace_b_r_k_g_a>` namespace.

The :ref:`EnumIO <doxid-class_enum_i_o>` class helps to read and write enums from streams directly, saving time in coding custom solutions. Please, see ``third_part/enum_io.hpp`` for complete reference and examples.

.. note::

	Due to some weird scope resolution within clang compilers, we must imbue the enum facilities into namespace :ref:`BRKGA <doxid-namespace_b_r_k_g_a>`. This does not happen with g++. We have not test to MSVC.



.. warning::

	The specialization must be inline-d to avoid multiple definitions issues across different modules. However, this can cause "inline" overflow, and compromise your code. If you include this header only once along with your code, it is safe to remove the ``inline`` s from the specializations. But, if this is not the case, you should move these specializations to a module you know is included only once, for instance, the ``main()`` module.

