# rcunit

[![Build](https://github.com/jecklgamis/rcunit/actions/workflows/build.yaml/badge.svg)](https://github.com/jecklgamis/rcunit/actions/workflows/build.yaml)

rcunit is a small framework for testing C programs. It uses non-local jumps to
emulate exceptions and handles program-terminating signals (e.g. SIGSEGV,
SIGILL) during test runs. rcunit supports test fixtures per test module, and is free to use under the
[Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

## Documentation

See [doc/rcunit-user-guide.md](doc/rcunit-user-guide.md).

## Building and Installing

```sh
git clone https://github.com/jecklgamis/rcunit.git
cd rcunit
./autogen.sh
./configure
make
sudo make install
```

This installs the static library (`librcunit.a`) in `/usr/local/lib` and the
headers in `/usr/local/include`. To change the install prefix:

```sh
./configure --prefix=/some/dir
```

## Running Tests

```sh
make check
```
