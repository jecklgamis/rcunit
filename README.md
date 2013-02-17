RCUNIT is a small framework for testing C programs. It uses non-local jumps to
emulate exceptions and handles program terminating signals (e.g. SIGILL) 
during test runs. RCUNIT allows creation of test fixtures, either per test
or per test group. RCUNIT is free, you can use it in accordance with the terms
of the MIT License.

Documentation
* See doc/rcunit-docs.txt

Building and Installing RCUNIT (Unix platform)
 
* Build and install Logmoko logging library (git clone git@github.com:jecklgamis/logmoko.git)
* Build and install RCUNIT

```
   $ git clone git@github.com:jecklgamis/rcunit.git
   $ ./autogen.sh
   $ ./configure
   $ make all
   $ sudo make install
```

What Gets Installed?
* RCUNIT static library (librcunit.a) in /usr/local/lib
* RCUNIT header files in /usr/local/include

Note you can change the installation path by re-running ./configure  with --prefix

```
$ ./configure --prefix=/some/dir
```
By default --prefix is ```/usr/local```

Changes from v0.9 to 1.0
~~~~~~~~~~~~~~~~~~~~~~~~

* Signal handling (a major contribution from Tomasz Kantecki at Intel IE). 
* Simplified creation/retrieval of test module and test registry.
* Added assertion macros for bit and byte array testing
* Use of Logmoko, a logging framework based on RCUNIT logging codes itself
* varargs are used for supporting fprintf-like argument passing
* Refined API functions and helper macros
* Removed enabled/disabled flag when adding test or module
* Data types are made lower case to avoid confusion with macros
* All assertions are made implicitly fatal. An executing test will be
  aborted on the first assertion failure

