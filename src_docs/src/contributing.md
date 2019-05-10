Contributing
=======================

Although the code is an experimental one, we aim to write an efficient and
consistent code. So, you should keep in mind the balance between memory
utilization and code efficiency and pay special attention to cache utilization.
This aspect is very important when using multi-threads applications with shared
memory.


Style
-----------------------

Please, follow the general coding style. Since it is too long to describe all
details here, study the code already written. However, in general, 

- Name classes, methods, and variables as clear and meaningful as possible;

- Write short commentaries on the code flow to reading more accessible and
  faster;

- Use Doxygen to properly document the code, especially the classes and methods
  definitions;

- No trailing spaces, no tabs, Unix/POSIX end of line. Try to keep line
  within 80 columns and do not exceed 120 columns;

- Always compile your code using all warning flags activated.
  **DO NOT IGNORE the warnings: fix them**;

- Always use [cppcheck](http://cppcheck.sourceforge.net) to check for flaws
  and improve the code quality;

- Use [Valgrind](http://valgrind.org) to check for memory leaks and other
  improvements;

- Do not use system specific code/headers. Your code must compile in several
  systems with minimum change.

