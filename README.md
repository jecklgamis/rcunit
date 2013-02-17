
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

Note you can change the installation path by re-running ./configure with the
--prefix=<dir> option. By default it uses /usr/local.
