# CppAD: A Package for Differentiation of C++ Algorithms

## Documentation
[users guide](https://cppad.readthedocs.io/en/latest/user_guide.html)

## License
SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later

## Install

- The preferred method to test and
  [install](https://cppad.readthedocs.io/en/latest/Install.html)
  CppAD uses [cmake](https://cmake.org).

- A deprecated
  [autotools](https://cppad.readthedocs.io/en/latest/autotools.html)
  procedure can be used for this purpose, but it will eventually be removed.

- For any sub-directory *dir*,
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
