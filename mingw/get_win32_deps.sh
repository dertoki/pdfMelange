#!/bin/bash
#
# (c) 2013 Tobias Tandetzki (LGPL-3.0)
# You may view the full copyright text at:
# http://www.opensource.org/licenses/lgpl-3.0.html
#
# DESCRIPTION:
# 
#  Get a working (mingw) runtime environment for "pdfMelange".
#  Dependencies are GTK3, wine and p7zip-full.
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
#M_GTK3_prefix='/opt/mingw'
#M_GTK3_prefix='/usr/i686-w64-mingw32/sys-root/mingw'  # Fedora f20 

##########################################################################################################
# Section Functions
##########################################################################################################

# download the dependencies for windows.
function _download_deps 
{
    if [ ! -f download ] ; then  mkdir download ; fi
    wget "http://www.jrsoftware.org/download.php/is.exe" -P download
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
    wine download/is.exe
}

# create a windows setup program. 
function _build_setup 
{
#   if [ ! -f output ] ; then  mkdir output ; fi

    # modify the root of "Inno Setup" if necessarry:
    M_inno_root="C:\Program Files (x86)\Inno Setup 5"

    wineconsole --backend=curses $M_inno_root\\iscc $M_project.iss
    mv -v output/setup.exe output/$M_project$M_version.setup.exe
    
    md5sum    --tag output/$M_project$M_version.setup.exe >  output/hash
    sha1sum   --tag output/$M_project$M_version.setup.exe >> output/hash
    sha256sum --tag output/$M_project$M_version.setup.exe >> output/hash
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
    M_GTK3dll=$(cat gtk+-3.10.4-dlls.lst)

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

function _collect_GSettingsSchemas 
{
    M_GSettingsSchemas_path='/share/glib-2.0/schemas/'

    mkdir -vp $M_prefix$M_GSettingsSchemas_path
    cp -v $M_GTK3_prefix$M_GSettingsSchemas_path/* $M_prefix$M_GSettingsSchemas_path
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
            _collect_dlls
            _collect_GSettingsSchemas
            _collect_std_icons
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
            _collect_dlls
            _collect_GSettingsSchemas
            _collect_std_icons
            make -f Makefile.win32 melange
            _build_setup
            wine $M_prefix/bin/$M_project.exe
            ;;
        *)
            echo $"Usage: $0 {all|depload|depinst|clean|collect|buildsetup|run}"
            echo $"         - all : build and execute the windows program."
            echo $"         - depload : load dependencies."
            echo $"         - depinst : install dependencies."
            echo $"         - clean : remove unneeded files."
            echo $"         - collect :  assemble all files to $M_prefix."
            echo $"         - buildsetup :  create the setup executable."
            echo $"         - buildportable :  create a portable program version."
            echo $"         - run :  start the program with wine."
            exit 1
esac

