#!/bin/bash
#
# (c) 2013 Tobias Tandetzki (LGPL-3.0)
# You may view the full copyright text at:
# http://www.opensource.org/licenses/lgpl-3.0.html
#
# DESCRIPTION:
# 
#  Get a working (mingw) runtime environment for "pdfMelange".
#  Dependencies are GTK3, pyhon2.7 and pyPdf1.13.
#  We also need wine and p7zip-full.
#
# Usage: 
#       $ ./get_win32_deps.sh {all|depload|depinst|clean|collect|buildsetup|run}"
#
#                all           : build and execute the windows program.
#                depload       : load dependencies.
#                depinst       : install dependencies.
#                clean         : remove unneeded files.
#                collect       : assemble all files to $M_prefix.
#                buildsetup    : create the setup executable.
#                buildportable : create a portable program version.
#                run           : start the program with wine.
#

##########################################################################################################
# Section Variables
##########################################################################################################

# modify the project name to your needs:
M_project='pdfMelange'
M_version="-$(date --rfc-3339=date)"
M_applauncher='set-up.exe'
M_prefix="$PWD/dist"

# System Root
M_GTK3_prefix='/opt/gtk38'
#M_GTK3_prefix='/usr/i686-w64-mingw32/sys-root/mingw'  # Fedora f20 

##########################################################################################################
# Section Functions
##########################################################################################################

# download the dependencies for windows.
function _download_deps 
{
	if [ ! -f download ] ; then  mkdir download ; fi
	wget "http://www.python.org/ftp/python/2.7.3/python-2.7.3.msi" -P download
	wget "http://www.jrsoftware.org/download.php/is.exe" -P download
	wget "http://pybrary.net/pyPdf/pyPdf-1.13.win32.exe" -P download
}

# install the dependencies for windows.
function _install_deps 
{
	if [ ! -f download ] ; then  mkdir download ; fi

	# a simple check if wine is installed.
	which wine 1> /dev/null
	if [ $? -ne 0 ] ; then 
	   echo -e "\n >>> Error: \"wine\" is needed but NOT installed! \n"
	   exit 1
	fi

	# install python pygtk py2exe and "inno setup" in wine.
	wine msiexec /i download/python-2.7.3.msi
	wine download/pyPdf-1.13.win32.exe
	wine download/is.exe
}

# create a windows setup program. 
function _build_setup 
{
#	if [ ! -f output ] ; then  mkdir output ; fi

	# modify the root of "Inno Setup" if necessarry:
	M_inno_root="C:\Program Files (x86)\Inno Setup 5"

	wineconsole --backend=curses $M_inno_root\\iscc $M_project.iss
	mv -v output/setup.exe output/$M_project$M_version.setup.exe
}

# create a portable program. 
function _build_portable 
{
	if [ ! -f output ] ; then  mkdir output ; fi

	echo "Install:" >  dist/readme.txt
	echo "  Unpack the archiv \"$M_project$M_version.7z\"." >>  dist/readme.txt
	echo "  Open the folder pdfMelange and start $M_applauncher." >>  dist/readme.txt
	echo " " >> dist/readme.txt
	echo "Uninstall:" >> dist/readme.txt
	echo "  Delete desktop link," >> dist/readme.txt
	echo "  Remove the folder \"$M_project\" from your disk." >> dist/readme.txt
	sed -i -e 's/$/\r/' dist/readme.txt              # UNIX to DOS  (adding CRs on Linux based OS that use GNU extensions)

	# Create a archiv file

	mv -v $M_prefix $PWD/$M_project
	7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on $M_project$M_version.7z $M_project
	mv -v $PWD/$M_project $M_prefix
	mv -v $M_project$M_version.7z output/
}

##########################################################################################################
# Section Execute
##########################################################################################################

#
# 1. Collect all necessary files for the project
################################################

function _collect_dlls 
{
	# expand this list of additional libraries when you get error messages complaining missing libraries. 
#	M_GTK3dll="libatk-1.0-0.dll libatkmm-1.6-1.dll libcairo-2.dll libcairo-gobject-2.dll libcairomm-1.0-1.dll libffi-6.dll libfontconfig-1.dll  libfreetype-6.dll libgdk-3-0.dll libgdkmm-3.0-1.dll libgdk_pixbuf-2.0-0.dll libgio-2.0-0.dll libgiomm-2.4-1.dll libglib-2.0-0.dll libglibmm-2.4-1.dll libgmodule-2.0-0.dll libgobject-2.0-0.dll libgtk-3-0.dll libgtkmm-3.0-1.dll libiconv-2.dll libintl-8.dll libjpeg-9.dll libpango-1.0-0.dll libpangocairo-1.0-0.dll libpangomm-1.4-1.dll libpangowin32-1.0-0.dll libpixman-1-0.dll libpng16-16.dll libpoppler-44.dll libpoppler-glib-8.dll libsigc-2.0-0.dll libtiff-5.dll libxml2-2.dll zlib1.dll"
	# Fedora f20 DLL collection	
	M_GTK3dll="libatk-1.0-0.dll libatkmm-1.6-1.dll libcairo-2.dll libcairo-gobject-2.dll libcairomm-1.0-1.dll libffi-6.dll libfontconfig-1.dll  libfreetype-6.dll libgdk-3-0.dll libgdkmm-3.0-1.dll libgdk_pixbuf-2.0-0.dll libgio-2.0-0.dll libgiomm-2.4-1.dll libglib-2.0-0.dll libglibmm-2.4-1.dll libgmodule-2.0-0.dll libgobject-2.0-0.dll libgtk-3-0.dll libgtkmm-3.0-1.dll libintl-8.dll libpango-1.0-0.dll libpangocairo-1.0-0.dll libpangomm-1.4-1.dll libpangowin32-1.0-0.dll libpixman-1-0.dll libpng16-16.dll  libpoppler-glib-8.dll libsigc-2.0-0.dll libtiff-5.dll zlib1.dll libpoppler-glib-8.dll iconv.dll libintl-8.dll libglib-2.0-0.dll libwinpthread-1.dll libgcc_s_sjlj-1.dll libffi-6.dll libgobject-2.0-0.dll libpoppler-44.dll libbz2-1.dll libjpeg-62.dll liblcms2-2.dll libopenjpeg-1.dll libexpat-1.dll"


	# copy GTK3 libraries. 

	if [ ! -f $M_prefix/bin ] ; then  mkdir -vp $M_prefix/bin ; fi
	for i in $M_GTK3dll 
	do
		if [ -e $M_GTK3_prefix/bin/$i ] ; then
			cp -v $M_GTK3_prefix/bin/$i $M_prefix/bin
		else 
			echo -e "\n >>> Error: \"$i\" not found! Expected to find \"$i\" in \"$M_GTK3_prefix/bin\""
			exit 1
		fi
	done

	# copy gcc libraries. 

	mkdir -vp $M_prefix/bin
	cp -v $(find '/usr/lib/gcc/i686-w64-mingw32/' -name libgcc_s_sjlj-1.dll) $M_prefix/bin
	cp -v $(find '/usr/lib/gcc/i686-w64-mingw32/' -name libstdc++-6.dll) $M_prefix/bin
	cp -v $(find '/usr/i686-w64-mingw32/' -name libwinpthread-1.dll) $M_prefix/bin
}

function _collect_theme_adwaita 
{
	# copy theme

	M_theme='/Adwaita'
	M_themeengine='libadwaita.dll'

	M_relativpath_theme='/share/themes'
	M_relativpath_themeengine='/lib/gtk-3.0/3.0.0/theming-engines'

	mkdir -p $M_prefix$M_relativpath_theme$M_theme
	cp -vr $M_GTK3_prefix$M_relativpath_theme$M_theme/* $M_prefix$M_relativpath_theme$M_theme
	rm -vr $M_prefix$M_relativpath_theme$M_theme/gtk-2.0

	mkdir -p $M_prefix$M_relativpath_themeengine
	cp -v $M_GTK3_prefix$M_relativpath_themeengine/$M_themeengine $M_prefix$M_relativpath_themeengine
}
 
function _collect_icons_adwaita 
{
	# copy icons

	M_main_icon='pdfMelange.png'
	M_icons_theme='oxygen'
	M_relativpath_icons="/share/icons/$M_icons_theme/32x32"
	M_icons=$(cat icons.lst)

	M_path=$PWD

	mkdir -vp $M_prefix$M_relativpath_icons

	cd $M_GTK3_prefix$M_relativpath_icons
	for i in $M_icons 
	do
		#echo "cp -v --parents .$i $M_prefix$M_relativpath_icons"
		cp -v --parents .$i $M_prefix$M_relativpath_icons
	done
	cd $M_path

	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/mimetypes/gtk-file.png $M_prefix$M_relativpath_icons/mimetypes
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/list-add.png $M_prefix$M_relativpath_icons/actions/list-add-symbolic.png
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/list-remove.png $M_prefix$M_relativpath_icons/actions/list-remove-symbolic.png
	cp -v $M_path/$M_main_icon $M_prefix/share/icons

	# update icon cache
	cp -v $M_GTK3_prefix/share/icons/$M_icons_theme/index.theme $M_prefix/share/icons/$M_icons_theme
	gtk-update-icon-cache -t $M_prefix/share/icons/oxygen
}

function _collect_std_icons 
{
	# copy icons

	M_main_icon='pdfMelange.png'

	if [ ! -f $M_prefix/share/icons ] ; then  mkdir -vp $M_prefix//share/icons ; fi
	
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/mimetypes/text-x-generic.png $M_prefix/share/icons/gtk-file.png
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/places/folder.png $M_prefix/share/icons/gtk-directory.png
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/list-add.png $M_prefix/share/icons/list-add-symbolic.png
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/list-remove.png $M_prefix/share/icons/list-remove-symbolic.png
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/object-rotate-left.png $M_prefix/share/icons
	cp -v $M_GTK3_prefix/share/icons/gnome/32x32/actions/object-rotate-right.png $M_prefix/share/icons
	cp -v $M_main_icon $M_prefix/share/icons
}

function _collect_settings 
{
	# update settings
	# edit /etc/gtk-3.0/settings.ini
	mkdir -vp $M_prefix/etc/gtk-3.0
	echo '[Settings]' 			>  $M_prefix/etc/gtk-3.0/settings.ini
	echo 'gtk-theme-name = Adwaita'  	>> $M_prefix/etc/gtk-3.0/settings.ini
	echo 'gtk-icon-theme-name = oxygen'  	>> $M_prefix/etc/gtk-3.0/settings.ini
}

function _collect_GSettingsSchemas 
{
	M_GSettingsSchemas_path='/share/glib-2.0/schemas/'

	mkdir -vp $M_prefix$M_GSettingsSchemas_path
	cp -v $M_GTK3_prefix$M_GSettingsSchemas_path/* $M_prefix$M_GSettingsSchemas_path
}

function _collect_pythons 
{

	# modify the destination of your windows-python installation if necessary:
	M_python_root="$HOME/.wine/drive_c/Python27"

	# the python runtime library:
	M_python_dll="python27.dll"
	M_python_dll_local=$(find $HOME/.wine/ -name $M_python_dll | tail -n 1)

	M_python_libs_main='_abcoll.py abc.py codecs.py copy.py copy_reg.py decimal.py functools.py __future__.py genericpath.py hashlib.py linecache.py locale.py ntpath.py numbers.py os.py re.py site.py sre_compile.py sre_constants.py sre_parse.py stat.py struct.py sysconfig.py traceback.py types.py UserDict.py warnings.py weakref.py _weakrefset.py fnmatch.py shutil.py tempfile.py collections.py keyword.py heapq.py bisect.py random.py'
	M_python_libs_encodings='aliases.py ascii.py cp1252.py cp850.py utf_16.py cp437.py hex_codec.py __init__.py'
	M_python_libs_pypdf='filters.py filters.py generic.py generic.py __init__.py __init__.py pdf.py pdf.py utils.py utils.py xmp.py xmp.py'

	# copy python runtime library.

	mkdir -vp $M_prefix/bin
	if [ -z $M_python_dll_local ]; then 
		# file not found!
		echo -e "\n >>> Error: \"$M_python_dll\" not found! Expected to find \"$M_python_dll\" in \"$HOME/.wine/\""
		exit 1
	else
		cp -v $M_python_dll_local $M_prefix/bin
	fi

	# copy python main libraries. 

	mkdir -vp $M_prefix/bin/Lib
	for i in $M_python_libs_main
	do
		cp -v $M_python_root/Lib/$i $M_prefix/bin/Lib
	done

	# copy python encodings libraries.

	mkdir -vp $M_prefix/bin/Lib/encodings
	for i in $M_python_libs_encodings
	do
		cp -v $M_python_root/Lib/encodings/$i $M_prefix/bin/Lib/encodings
	done

	# copy python pyPdf libraries.

	mkdir -vp $M_prefix/bin/Lib/pyPdf
	for i in $M_python_libs_pypdf
	do
		cp -v $M_python_root/Lib/site-packages/pyPdf/$i $M_prefix/bin/Lib/pyPdf
	done
}


function _collect_copyright_notes 
{
	# pyPdf copyright notes.

	mkdir -vp $M_prefix/share/doc/python-pypdf
	cp -v /usr/share/doc/python-pypdf/copyright $M_prefix/share/doc/python-pypdf

	# python copyright notes.

	mkdir -vp $M_prefix/share/doc/libpython
	cp -v /usr/share/doc/libpython2.7/copyright $M_prefix/share/doc/libpython

	# GTKmm copyright notes.

	mkdir -vp $M_prefix/share/doc/libgtkmm-3
	cp -v /usr/share/doc/libgtkmm-3.0-1/copyright $M_prefix/share/doc/libgtkmm-3

	# poppler copyright notes.

	mkdir -vp $M_prefix/share/doc/libpoppler
	cp -v /usr/share/doc/libpoppler-glib8/copyright $M_prefix/share/doc/libpoppler

	# glib copyright notes.

	mkdir -vp $M_prefix/share/doc/libglib
	cp -v /usr/share/doc/libglib2.0-0/copyright $M_prefix/share/doc/libglib

	# cairo copyright notes.

	mkdir -vp $M_prefix/share/doc/libcairo
	cp -v /usr/share/doc/libcairo2/copyright $M_prefix/share/doc/libcairo
}

case "$1" in
        collect)
#		_collect_theme_adwaita
#		_collect_icons_adwaita
#		_collect_settings
		_collect_dlls
		_collect_GSettingsSchemas
		_collect_std_icons
		_collect_pythons
		_collect_copyright_notes 
		;;
        buildsetup)
		make -f Makefile.win32 melange
		_build_setup
		;;
        buildportable)
		make -f Makefile.win32 portable
		_build_portable
		;;
        depinst)
		_install_deps
		;;
        depload)
		_download_deps
		;;
        clean)
		rm -rv output
		rm -rv dist
		;;
        run)
		wine $M_prefix/bin/$M_project.exe
		;;
	all)
#		_collect_theme_adwaita
#		_collect_icons_adwaita
#		_collect_settings
		_collect_dlls
		_collect_GSettingsSchemas
		_collect_std_icons
		_collect_pythons 
		make -f Makefile.win32 melange
		_build_setup
		wine $M_prefix/bin/$M_project.exe
		;;
        *)
		echo $"Usage: $0 {all|depload|depinst|clean|collect|buildsetup|run}"
		echo $"			- all : build and execute the windows program."
		echo $"			- depload : load dependencies."
		echo $"			- depinst : install dependencies."
		echo $"			- clean : remove unneeded files."
		echo $"			- collect :  assemble all files to $M_prefix."
		echo $"			- buildsetup :  create the setup executable."
		echo $"			- buildportable :  create a portable program version."
		echo $"			- run :  start the program with wine."
		exit 1
esac

