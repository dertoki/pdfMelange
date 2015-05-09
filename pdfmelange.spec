Name:           pdfmelange
Version:        0.4.3
Release:        1%{?dist}
Summary:        A program to modify PDF documents

License:        GPLv3+
URL:            http://dertoki.github.io/pdfMelange/
Source0:        %{name}-%{version}.tar.xz

%if 0%{?fedora}
BuildRequires:  gcc-c++,intltool,gtkmm30-devel,poppler-glib-devel,poppler-devel
Requires:       gtkmm30,poppler-glib,poppler
%endif

%if 0%{?suse_version} == 1310
BuildRequires:  gcc-c++,intltool,gtkmm3-devel,libpoppler-glib-devel,libpoppler-devel,update-desktop-files
Requires:       libpoppler-glib8,libpoppler43
%endif
%if 0%{?suse_version} == 1320
BuildRequires:  gcc-c++,intltool,gtkmm3-devel,libpoppler-glib-devel,libpoppler-devel,update-desktop-files
Requires:       libpoppler-glib8,libpoppler46
%endif

%description
A program to modify PDF documents
 + Merging PDF documents
 + Erasing pages
 + Change the page order
 + Modify the orientation of pages by rotating in 90° steps
 + Drag an drop files from your file manager
 + Free and open software

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
%if 0%{?suse_version} >= 1310
%suse_update_desktop_file -r -G 'PDF editor' %{name} Utility DesktopUtility
%endif

%make_install

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/pdfmelange
/usr/share/applications/pdfmelange.desktop
/usr/share/locale/de/LC_MESSAGES/pdfmelange.mo
/usr/share/pixmaps/pdfmelange.png
%dir /usr/doc
%dir /usr/doc/pdfmelange
%doc /usr/doc/pdfmelange/AUTHORS
%doc /usr/doc/pdfmelange/COPYING
%doc /usr/doc/pdfmelange/ChangeLog
%doc /usr/doc/pdfmelange/INSTALL
%doc /usr/doc/pdfmelange/NEWS
%doc /usr/doc/pdfmelange/README

%changelog
* Sun Apr 04 2014 Tobias
-
