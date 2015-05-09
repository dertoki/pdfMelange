dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(pdfMelange, 0.4.3)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CXX

dnl ***************************************************************************
dnl Conditional Compiling test applications
dnl ***************************************************************************
AC_ARG_ENABLE(testapps,
[  --enable-testapps     Build extra test applications [default=no]],
enable_testapps="$enableval", enable_testapps="no")

AM_CONDITIONAL(ENABLE_TESTAPPS, test "$enable_testapps" = "yes")

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=pdfmelange
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [The domain to use with gettext])

AM_GLIB_GNU_GETTEXT

PROGRAMNAME_LOCALEDIR=[${datadir}/locale]
AC_SUBST(PROGRAMNAME_LOCALEDIR)


LT_INIT






PKG_CHECK_MODULES(POPPLERWRITER_DEPS,poppler
poppler-glib
glibmm-2.4)

PKG_CHECK_MODULES(DRAWINGAREA_DEPS,gtkmm-3.0
poppler-glib)

PKG_CHECK_MODULES(PREF_DIALOG_DEPS,gtkmm-3.0)

PKG_CHECK_MODULES(MELANGE_DEPS,poppler
poppler-glib
gtkmm-3.0)

AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
mingw/Makefile
])
