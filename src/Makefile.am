
pdfmelange_CXXFLAGS = -std=c++0x 
## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(MELANGE_DEPS_CFLAGS) 

AM_CFLAGS =\
	 -Wall\
	 -g

if ENABLE_TESTAPPS
TEST_APPS = gtkbuilder2ustring preferencesDialog passwordDialog saveAsDialog drawingArea treeView PopplerWriter
else
TEST_APPS =
endif

bin_PROGRAMS = pdfmelange $(TEST_APPS)

pdfmelange_SOURCES =  \
	melangePdfWriter.h \
	melangePdfWriter.cc \
	melangePopplerWriter.h \
	melangePopplerWriter.cc \
	melangeWindow.h \
	melangeWindow.cc \
	melangeListStore.h \
	melangeListStore.cc \
	melangeDrawingArea.h \
	melangeDrawingArea.cc \
	melangeAboutDialog.h \
	melangeAboutDialog.cc \
	melangeSaveAsDialog.h \
	melangeSaveAsDialog.cc \
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
	melangeTreeView.h \
	copyfile.cc \
	copyfile.h

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

# saveAsDialog test scenario

saveAsDialog_SOURCES =  \
	melangeSaveAsDialog.cc \
	melangeSaveAsDialog.h \
	melangeKeyFile.cc \
	melangeKeyFile.h

saveAsDialog_CPPFLAGS = -DTEST_SAVEAS \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(PREF_DIALOG_DEPS_CFLAGS)

saveAsDialog_CXXFLAGS = -std=c++0x 

saveAsDialog_LDADD = $(PREF_DIALOG_DEPS_LIBS) 

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
    melangeKeyFile.h \
    melangeKeyFile.cc \
    copyfile.h \
    copyfile.cc \
	melangePdfWriter.h \
	melangePdfWriter.cc \
	melangePopplerWriter.h \
	melangePopplerWriter.cc \
	melangePassword.cc \
	melangePassword.h

treeView_CPPFLAGS = -DTEST_TREEVIEW \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(MELANGE_DEPS_CFLAGS)

treeView_CXXFLAGS = -std=c++0x 

treeView_LDADD = $(MELANGE_DEPS_LIBS) 

# PopplerWriterWriter test scenario

PopplerWriter_SOURCES =  \
    copyfile.h \
    copyfile.cc \
	melangePdfWriter.h \
	melangePdfWriter.cc \
	melangePopplerWriter.h \
	melangePopplerWriter.cc 

PopplerWriter_CPPFLAGS = -DTEST_PDFPOPPLERWRITER \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPROGRAMNAME_LOCALEDIR=\""${PROGRAMNAME_LOCALEDIR}"\" \
	$(POPPLERWRITER_DEPS_CFLAGS)

PopplerWriter_CXXFLAGS = -std=c++0x 

PopplerWriter_LDADD = $(POPPLERWRITER_DEPS_LIBS) 


