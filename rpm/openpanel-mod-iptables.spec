%define version 0.9.3

%define libpath /usr/lib
%ifarch x86_64
  %define libpath /usr/lib64
%endif

Summary: Iptables Firewalling
Name: openpanel-mod-iptables
Version: %version
Release: 1
License: GPLv2
Group: Development
Source: http://packages.openpanel.com/archive/openpanel-mod-iptables-%{version}.tar.gz
Patch1: openpanel-mod-iptables-00-makefile
BuildRoot: /var/tmp/%{name}-buildroot
Requires: openpanel-core >= 0.8.3

%description
Iptables Firewalling
Openpanel user management module

%prep
%setup -q -n openpanel-mod-iptables-%version
%patch1 -p0 -b .buildroot

%build
BUILD_ROOT=$RPM_BUILD_ROOT
./configure
make

%install
BUILD_ROOT=$RPM_BUILD_ROOT
rm -rf ${BUILD_ROOT}
mkdir -p ${BUILD_ROOT}/var/opencore/modules/IPTables.module
cp -rf ./iptablesmodule.app ${BUILD_ROOT}/var/opencore/modules/IPTables.module/
ln -s iptablesmodule.app/exec ${BUILD_ROOT}/var/opencore/modules/IPTables.module/action
cp module.xml ${BUILD_ROOT}/var/opencore/modules/IPTables.module/
cp *.png *.html ${BUILD_ROOT}/var/opencore/modules/IPTables.module/
install -m 755 verify ${BUILD_ROOT}/var/opencore/modules/IPTables.module/verify

%post
mkdir -p /var/opencore/conf/staging/IPTables
chown opencore:authd /var/opencore/conf/staging/IPTables

%files
%defattr(-,root,root)
/
