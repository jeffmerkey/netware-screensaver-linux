%define _build_id_links none
%define debug_package %{nil}

Summary:          Netware SMP Style Screensaver for Linux
License:          LGPL
Name:             netware-screensaver
Version:          1.16
Release:          1%{?dist}

URL:              https://www.github.com/jeffmerkey/netware-screensaver-linux
Source0:          %{name}-%{version}.tar.gz

Requires:         ncurses
BuildRequires:    ncurses-devel

%description 
The %{name} package contains the Netware SMP screensaver for Linux.

%prep
%setup -q -n %{name}-%{version}

%build
%{__make} 

%install
[ -n "%{buildroot}" -a "%{buildroot}" != "/" ] && %{__rm} -rf %{buildroot}
%{__mkdir_p} %{buildroot}%{_sbindir}
%{__mkdir_p} %{buildroot}%{_bindir}
%{__mkdir_p} %{buildroot}%{_includedir}
%{__mkdir_p} %{buildroot}/usr/lib
%{__make} \
	DESTDIR=%{buildroot} \
	install

%pre

%post

%preun

%postun

%files
%defattr(-,root,root)
%{_bindir}/netware-worms
%{_includedir}/netware-worms.h
/usr/lib/libnetware-screensaver.a
/usr/lib/libnetware-screensaver.so

%changelog
