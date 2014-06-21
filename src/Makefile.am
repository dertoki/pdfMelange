
pdfmelange_CXXFLAGS = -std=c++0x 
## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(MELANGE_DEPS_CFLAGS) \
	$(PREF_DIALOG_DEPS_CFLAGS) \
	$(DRAWINGAREA_DEPS_CFLAGS) \
	$(TREEVIEW_DEPS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

if ENABLE_TESTAPPS
TEST_APPS = gtkbuilder2ustring preferencesDialog passwordDialog drawingArea treeView pyPdfWriter
else
TEST_APPS =
endif

bin_PROGRAMS = pdfmelange $(TEST_APPS)

pdfmelange_SOURCES =  \
	pythonPdfWriter.h \
	pythonPdfWriter.cc \
	melangeWindow.h \
	melangeWindow.cc \
	melangeListStore.h \
	melangeListStore.cc \
	melangeDrawingArea.h \
	melangeDrawingArea.cc \
	melangeAboutDialog.h \
	melangeAboutDialog.cc \
	melange.ui.h \
	main.cc \
	melange.ui \
	../po/de.po \
	melange.ui-string.h \
	melangeKeyFile.cc \
	melangeKeyFile.h \
	melangePreferences.cc \
	melangePreferences.h \
	melangePreferences.ui-string.h \
	melangePreferences.ui.h \
	melangeLogging.cc \
	melangeLogging.h \
	melangePassword.cc \
	melangePassword.h \
	melange_pixbuf-inline.h \
	melangeTreeView.cc \
	melangeTreeView.h

pdfmelange_LDFLAGS = 

pdfmelange_LDADD = $(MELANGE_DEPS_LIBS) 

gtkbuilder2ustring_SOURCES =  \
	gtkbuilder2ustring.cc

gtkbuilder2ustring_LDADD = $(MELANGE_DEPS_LIBS) 

# preferencesDialog test scenario

preferencesDialog_SOURCES =  \
	melangePreferences.cc \
	melangePreferences.h \
	melangePreferences.ui \
	melangePreferences.ui-string.h \
	melangePreferences.ui.h \
	../po/de.po \
	melangeKeyFile.cc \
	melangeKeyFile.h

preferencesDialog_CPPFLAGS = -DTEST_PREF \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"\
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(PREF_DIALOG_DEPS_CFLAGS)

preferencesDialog_CXXFLAGS = -std=c++0x 

preferencesDialog_LDADD = $(PREF_DIALOG_DEPS_LIBS) 

# passwordDialog test scenario

passwordDialog_SOURCES =  \
	melangePassword.cc \
	melangePassword.h

passwordDialog_CPPFLAGS = -DTEST_PASSWORD \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(PREF_DIALOG_DEPS_CFLAGS)

passwordDialog_CXXFLAGS = -std=c++0x 

passwordDialog_LDADD = $(PREF_DIALOG_DEPS_LIBS) 

# drawingArea test scenario

drawingArea_SOURCES =  \
	melangeDrawingArea.cc \
	melangeDrawingArea.h

drawingArea_CPPFLAGS = -DTEST_DRAWINGAREA \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(DRAWINGAREA_DEPS_CFLAGS)

drawingArea_CXXFLAGS = -std=c++0x 

drawingArea_LDADD = $(DRAWINGAREA_DEPS_LIBS) 

# treeView test scenario

treeView_SOURCES =  \
	melangeTreeView.cc \
	melangeTreeView.h \
	melangeListStore.cc \
	melangeListStore.h \
	pythonPdfWriter.h \
	pythonPdfWriter.cc \
	melangePassword.cc \
	melangePassword.h

treeView_CPPFLAGS = -DTEST_TREEVIEW \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(TREEVIEW_DEPS_CFLAGS)

treeView_CXXFLAGS = -std=c++0x 

treeView_LDADD = $(TREEVIEW_DEPS_LIBS) 

# pyPdfWriter test scenario

pyPdfWriter_SOURCES =  \
	pythonPdfWriter.h \
	pythonPdfWriter.cc 

pyPdfWriter_CPPFLAGS = -DTEST_PYTHONPDFWRITER \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(PYTHONPDFWRITER_DEPS_CFLAGS)

pyPdfWriter_CXXFLAGS = -std=c++0x 

pyPdfWriter_LDADD = $(PYTHONPDFWRITER_DEPS_LIBS) 


