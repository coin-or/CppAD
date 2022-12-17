# Title
CppAD: A Package for Differentiation of C++ Algorithms

# Links

- [Documentation](https://coin-or.github.io/CppAD)

- [News](https://coin-or.github.io/CppAD/html/whats_new.html)

- [Install](https://coin-or.github.io/CppAD/html/install.html)

# License
<pre>
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
</pre>


# Autotools
The preferred method to test and install CppAD uses [CMake](https://cmake.org).
The deprecated Autotools procedure can be used for this purpose,
but it will eventually be removed.
For any sub-directory *dir*,
files of the form *dir*/`makefile.am` and *dir*/`makefile.in`
are used to support the Autotools test and install procedure.
In addition,
the following files, in this directory, are also for this purpose:
`compile`,
`config.guess`,
`config.sub`,
`configure.ac`,
`depcomp`,
`install-sh`,
`missing`.


# Copyright
See the file `authors` in this directory.
