Name:           rcunit
Version:        2.0.0
Release:        0.1.rc1%{?dist}
Summary:        Unit testing framework for C programs
License:        Apache-2.0
URL:            https://github.com/jecklgamis/rcunit
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  autoconf
BuildRequires:  automake
BuildRequires:  gcc
BuildRequires:  make

%description
RCUNIT is a small framework for testing C programs. It uses non-local
jumps to emulate exceptions and handles program-terminating signals
(e.g. SIGILL) during test runs. RCUNIT allows creation of test
fixtures, either per test or per test group.

%package devel
Summary:        Development files for rcunit
%description devel
This package contains the static library and header files needed
to develop software using RCUNIT.

%prep
%autosetup

%build
./autogen.sh
%configure
%make_build

%install
%make_install

%files devel
%license LICENSE
%doc README.md
%{_libdir}/librcunit.a
%{_includedir}/rcunit*.h

%changelog
* Thu May 01 2026 Jerrico Gamis <jecklgamis@gmail.com> - 2.0.0-0.1.rc1
- Release candidate 2.0.0-rc1
- Fix buffer overflows in assert and mtrace
- Fix broken random number generator formula
- Remove duplicate declarations and macros in rcunit_api.h
- Standardize license headers to Apache 2.0

* Sat Apr 26 2026 Jerrico Gamis <jecklgamis@gmail.com> - 1.0.0-1
- Initial release
