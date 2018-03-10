# Title
CppAD: A Package for Differentiation of C++ Algorithms

# Links

- [docmentation](https://coin-or.github.io/CppAD/doc)

- [News](https://coin-or.github.io/CppAD/doc/whats_new.htm)

- [Install](https://coin-or.github.io/CppAD/doc/install.htm)

- [Directories](https://coin-or.github.io/CppAD/doc/directory.htm)

- [Coin-OR Download](https://www.coin-or.org/download/source/CppAD/)


# License
<pre>
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
</pre>


# Autotools
The preferred method to test and install CppAD uses cmake.
The deprecated autotools procedure can be used for this purpose,
but it will eventually be removed.
For any sub-directory *dir*,
files of the form *dir*/`makefile.am` and *dir*/`makefile.in`
are used to support the autotools test and install procedure.
In addition,
the following files, in this directory, are also for this purpose:
`compile`,
`config.guess`,
`config.sub`,
`configure.ac`,
`depcomp`,
`install-sh`,
`missing`.


# Copyright:
See the file `authors` in this directory.
