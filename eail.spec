%define MODULEDIR "/usr/lib/elementary/modules/eail/linux-gnu-i686-1.7.99"
Name: eail
Summary: The Accessibility Toolkit for elementary
Version: 1.0.0
Release: 1
License: LGPL
Group: Development/Libraries
Source: %{name}-%{version}.tar.xz
URL: http://www.gtk.org

Requires: glib2
Requires: elementary
Requires: atk
BuildRequires: glib2-devel
BuildRequires: elementary-devel
BuildRequires: pkgconfig
BuildRequires: atk-devel
BuildRequires: at-spi2-atk-devel

BuildRoot: %{buildroot}

%description
Handy library of accessibility functions. Development libs and headers
are in eail-devel.

%package devel
Summary: Header, docs and development files to to develop applications using eail.
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Header, docs and development libraries for eail.

%prep
%setup -q

%build
(if ! test -x configure; then ./autogen.sh; fi;
 %configure --with-elementary-modules=%{MODULEDIR} $CONFIGFLAGS)
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README
%{_libdir}/libeail*
%{_libdir}/elementary/modules/eail/linux-gnu-i686-1.7.99/*.so

%files devel
%defattr(-, root, root)
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/eail/*.h

%changelog
* Thu Jul 11 2013 packager@comarch.com
- Add eail as elementary module
* Wed Apr 17 2013 packager@comarch.com
- Fix spec file
* Mon Mar 25 2013 packager@comarch.com
- Initial (2.2.02.0)


