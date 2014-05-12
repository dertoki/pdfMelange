Name:           pdfmelange
Version:        0.4
Release:        1%{?dist}
Summary:        A program to modify PDF documents.

License:        GPLv3+
URL:            http://dertoki.github.io/pdfMelange/
Source0:        %{name}-%{version}.tar.gz


%if %{defined suse_version}
BuildRequires:  gcc-c++,intltool,gtkmm3-devel,poppler-glib-devel,python2-devel
Requires:       gtkmm3,poppler-glib,python,pyPdf
%endif

%if %{defined fedora}
BuildRequires:  gcc-c++,intltool,gtkmm30-devel,libpoppler-glib-devel,python2-devel
Requires:       gtkmm30,libpoppler-glib,python,pyPdf
%endif

%description
A program to modify PDF documents.
 + Merging PDF documents.
 + Erasing pages.
 + Change the page order.
 + Modify the orientation of pages by rotating in 90° steps.
 + Drag an drop files from your file manager.
 + Free and open software.

%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%files
/usr/bin/pdfmelange
/usr/doc/pdfmelange/AUTHORS
/usr/doc/pdfmelange/COPYING
/usr/doc/pdfmelange/ChangeLog
/usr/doc/pdfmelange/INSTALL
/usr/doc/pdfmelange/NEWS
/usr/doc/pdfmelange/README
/usr/share/applications/pdfmelange.desktop
/usr/share/locale/de/LC_MESSAGES/pdfmelange.mo
/usr/share/pixmaps/pdfmelange.png

%changelog
* Sun Apr 13 2014 Tobias
- 
