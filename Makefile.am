## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

pdfmelangedocdir = ${prefix}/doc/pdfmelange
pdfmelangedoc_DATA = \
	README\
	README.md\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

pdfmelangePixmapdir = $(prefix)/share/pixmaps
pdfmelangePixmap_DATA = extra/pdfmelange.png

pdfmelangeDesktopdir = $(prefix)/share/applications
pdfmelangeDesktop_DATA = extra/pdfmelange.desktop

mingw_FILES = \
	mingw/crossBuild-poppler-win32.sh \
	mingw/get_win32_deps.sh \
	mingw/icons.lst \
	mingw/gtk+-3.10.4-dlls.lst \
	mingw/launcher.c \
	mingw/launcher-resource.rc \
	mingw/licence.txt \
	mingw/Makefile.win32 \
	mingw/pdfMelange.ico \
	mingw/pdfMelange.iss \
	mingw/pdfMelange.png \
	mingw/pdfMelange.xcf \
	mingw/pdfMelange-resource.rc \
	mingw/portable_set-up.cc \
	mingw/portable_set-up_resource.rc

INTLTOOL_FILES = \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(pdfmelangedoc_DATA) \
	$(INTLTOOL_FILES) \
	$(mingw_FILES) \
	$(data_DATA) \
	debian \
	extra \
	distmake.sh \
	pdfmelange-xUbuntu_14.04.dsc \
	pdfmelange-xUbuntu_14.10.dsc \
	pdfmelange-xUbuntu_15.04.dsc \
	pdfmelange.spec \
	minimal.pdf

DISTCLEANFILES = \
	intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(pdfmelangedocdir)
