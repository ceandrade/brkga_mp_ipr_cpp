.. index:: pair: enum; Type
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132:
.. _cid-brkga.pathrelinking.type:

enum BRKGA::PathRelinking::Type
===============================

Overview
~~~~~~~~

Specifies type of path relinking. :ref:`More...<details-doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132>`

.. ref-code-block:: cpp
	:class: overview-code-block

	#include <brkga_mp_ipr.hpp>

	enum BRKGA::PathRelinking::Type
	{
	    :ref:`DIRECT<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76>`,
	    :ref:`PERMUTATION<doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a48deaef68056f516e0091a15c1db3daa>`,
	};

.. _details-doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132:

Detailed Documentation
~~~~~~~~~~~~~~~~~~~~~~

Specifies type of path relinking.

Enum Values
-----------

.. index:: pair: enumvalue; DIRECT
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a4c5d06b02c97731aaa976179c62dcf76:
.. _cid-brkga.pathrelinking.type.direct:

.. ref-code-block:: cpp
	:class: title-code-block

	DIRECT

Changes each key for the correspondent in the other chromosome.

.. index:: pair: enumvalue; PERMUTATION
.. _doxid-namespace_b_r_k_g_a_1_1_path_relinking_1a79247d22aeb1fa9ab7611488e8137132a48deaef68056f516e0091a15c1db3daa:
.. _cid-brkga.pathrelinking.type.permutation:

.. ref-code-block:: cpp
	:class: title-code-block

	PERMUTATION

Switches the order of a key for that in the other chromosome.

