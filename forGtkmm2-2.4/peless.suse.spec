%define _dist %(grep SuSE /etc/SuSE-release)
%define _prefix   /opt/gnome
%define _sharedir %{_prefix}/share
%define boost_version 1.32.0
%define boost_flags --with-boost=/usr --with-boost-version=1_32 --with-boost-include=/usr/include/boost --with-boost-lib-postfix=-gcc-1_32

%if %suse_version == 900
BuildRequires: gtkmm2-devel, gconfmm2-devel, libsigc++2-devel
Requires: gtkmm2 >= 2.2, gconfmm2 >= 2.0, libsigc++2
%endif

%if %suse_version == 910
BuildRequires: gtkmm2-devel, gconfmm-devel, libsigc++2-devel
Requires: gtkmm2 >= 2.2, gconfmm >= 2.0, libsigc++2
%endif

%if %suse_version == 920
BuildRequires: gtkmm24-devel, gconfmm26-devel, libsigc++20-devel
Requires: gtkmm24 >= 2.4, gconfmm26 >= 2.6, libsigc++20
%endif

Name:		peless
Summary:	Text browser

%if %suse_version == 900 || %suse_version == 910
Version:	0.57
Release:	0.040530
%endif
%if %suse_version == 920
Version:	0.63
Release:	0.040120
%endif

Copyright:	GPL
Group:		Productivity/Text/Utilities
Source0:	%{name}-%{version}.tar.bz2
Source1:	peless.png
BuildRoot:	%{_tmppath}/%{name}-root
Packager:	D. Scott Barninger <barninger@fairfieldcomputers.com>
Distribution:	%{_dist}
Prefix:		/opt/gnome
URL:		http://www.io.com/~pelliott/pme/ 
Vendor:		Paul Elliott

BuildRequires: pango-devel, atk-devel, boost-devel, gtk2-devel, freetype2-devel, orbit2-devel
Requires: pango, atk, gtk2, freetype2, orbit2, boost = %{boost_version}

%description
Peless is a simple text file lister. It only displays files and 
never modifies them. It can display multiple files using a tabbed notebook, 
display international characters, and search the files for regular expressions 
or literal expressions. Users can choose the fonts used to display files.

%prep
%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" \
	%configure %{boost_flags}
CCFLAGS=$RPM_OPT_FLAGS make %{?_smp_mflags}

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf "$RPM_BUILD_ROOT"

mkdir -p $RPM_BUILD_ROOT%{_sharedir}/pixmaps
mkdir -p $RPM_BUILD_ROOT%{_sharedir}/applications
cp -p %SOURCE1 $RPM_BUILD_ROOT%{_sharedir}/pixmaps/%{name}.png

make DESTDIR=$RPM_BUILD_ROOT install-strip

echo "[Desktop Entry]" > $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Encoding=UTF-8" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Name=%{name}" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Type=Application" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Exec=%{_prefix}/bin/%{name}" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Icon=%{_sharedir}/pixmaps/%{name}.png" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Terminal=0" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "StartupNotify=true" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop
echo "Categories=Application;Utility;" >> $RPM_BUILD_ROOT%{_sharedir}/applications/%{name}.desktop

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf "$RPM_BUILD_ROOT"
rm -rf $RPM_BUILD_DIR/file.list.%{name}
rm -rf $RPM_BUILD_DIR/file.list.%{name}.libs
rm -rf $RPM_BUILD_DIR/file.list.%{name}.files
rm -rf $RPM_BUILD_DIR/file.list.%{name}.files.tmp
rm -rf $RPM_BUILD_DIR/file.list.%{name}.dirs

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README INTERNATIONALIZATION philo.txt
%{_prefix}/bin/peless
%{_sharedir}/applications/%{name}.desktop
%{_sharedir}/pixmaps/%{name}.png

%changelog
* Mon Jan 24 2005 D. Scott Barninger <barninger@fairfieldcomputers.com>
- add build for 9.0 and 9.1
* Sun Jan 23 2005 D. Scott Barninger <barninger@fairfieldcomputers.com>
- modify for gwdg.de repository
