#!/bin/bash
#
# (c) 2013 Tobias Tandetzki (LGPL-3.0)
# You may view the full copyright text at:
# http://www.opensource.org/licenses/lgpl-3.0.html
#
# DESCRIPTION:
# 
#  Get a working (mingw) runtime environment for "pdfMelange".
#  Dependencies are gtk3, wine and p7zip-full.
#
# Usage: 
#       $ ./get_win32_deps.sh {all|depload|depinst|clean|collect|buildsetup|run}"
#
#                all           : build and execute the windows program.
#                depload       : load dependencies.
#                depinst       : install dependencies.
#                clean         : remove unneeded files.
#                collect       : assemble all files to $my_prefix.
#                buildsetup    : create the setup executable.
#                buildportable : create a portable program version.
#                run           : start the program with wine.
#

##########################################################################################################
# Section Variables
##########################################################################################################

# modify the project name to your needs:
my_project='pdfMelange'
my_version="-$(date --rfc-3339=date)"
my_applauncher='set-up.exe'
my_prefix="$PWD/dist"
my_main_icon='pdfMelange.png'
icons_list_file='icons.lst'
dll_list_file='gtk+-3.18.6-i686-dlls.lst'
#dll_list_file='gtk+-3.18.6-x64-dlls.lst'

# System Root
gtk3_prefix='/usr/i686-w64-mingw32/sys-root/mingw'  # Fedora f23 
#gtk3_prefix='/usr/x86_64-w64-mingw32/sys-root/mingw'  # Fedora f23 

gtk3_icons_dir="/share/icons/Adwaita/32x32/"

##########################################################################################################
# Section Functions
##########################################################################################################

# Copy files, listed in file $1, from dir $2 to dir $2.
#
# List file $1 may have one or two arguments per line.
#    With one argument per line, the source- and target-name are the same.    
#    With two arguments per line, the first argument is the source-name and
#        the second argument is the target-name.    
#    Returns "1" on error.
function _list_copy 
{
     local list_file="$1"
     local source_dir="$2"
     local target_dir="$3"

     if [ ! -f "$target_dir" ] ; then  mkdir -vp "$target_dir" ; fi

     cat "$list_file" | while read source_file target_file; do
          if [ "$target_file" ] ; then   # If the length of "$target_file" is non-zero.
               if [ -e "$source_dir$source_file" ] ; then   # If file exists
                    cp -v "$source_dir$source_file"  "$target_dir$target_file"               
               else 
                    echo -e "\n >>> Error: \"""$source_file""\" not found!"
                    echo -e " >>>        Expected to find \"""$source_file""\" in \"""$source_dir""\""
                    return 1
               fi
          elif [ "$source_file" ] ; then # If the length of "$source_file" is non-zero.
               if [ -e "$source_dir$source_file" ] ; then   # If file exists
                    cp -v "$source_dir$source_file"  "$target_dir"               
               else 
                    echo -e "\n >>> Error: \"""$source_file""\" not found!"
                    echo -e " >>>        Expected to find \"""$source_file""\" in \"""$source_dir""\""
                    return 1
               fi
          fi
     done
}

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
    my_inno_root='C:\Program Files (x86)\Inno Setup 5'

    wineconsole --backend=curses $my_inno_root\\iscc "$my_project".iss
    mv -v output/setup.exe output/"$my_project$my_version".setup.exe
    
    md5sum    --tag output/$my_project$my_version.setup.exe >  output/hash
    sha1sum   --tag output/$my_project$my_version.setup.exe >> output/hash
    sha256sum --tag output/$my_project$my_version.setup.exe >> output/hash
}

# create a portable program. 
function _build_portable 
{
    if [ ! -f output ] ; then  mkdir output ; fi

    echo "Install:" >  dist/readme.txt
    echo "  Unpack the archiv \"$my_project$my_version.7z\"." >>  dist/readme.txt
    echo "  Open the folder pdfMelange and start $my_applauncher." >>  dist/readme.txt
    echo " " >> dist/readme.txt
    echo "Uninstall:" >> dist/readme.txt
    echo "  Delete desktop link," >> dist/readme.txt
    echo "  Remove the folder \"$my_project\" from your disk." >> dist/readme.txt
    sed -i -e 's/$/\r/' dist/readme.txt              # UNIX to DOS  (adding CRs on Linux based OS that use GNU extensions)

    # Create a archiv file

    mv -v $my_prefix $PWD/$my_project
    7za a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on $my_project$my_version.7z $my_project
    mv -v $PWD/$my_project $my_prefix
    mv -v $my_project$my_version.7z output/
}

##########################################################################################################
# Section Execute
##########################################################################################################

#
# 1. Collect all necessary files for the project
################################################

# Collect libraries
function _collect_dlls 
{
     # copy gtk3 libraries. 

     local dll_source_dir="$gtk3_prefix/bin/"
     local dll_target_dir="$my_prefix/bin/"

     if [ ! -f "$dll_target_dir" ] ; then  mkdir -vp "$dll_target_dir" ; fi

     _list_copy $dll_list_file $dll_source_dir $dll_target_dir
     if [ "$?" -eq 1 ] ; then  exit 1 ; fi

}

# Collect icons
function _collect_std_icons 
{
     local icons_source_dir="$gtk3_prefix$gtk3_icons_dir"
     local icons_target_dir="$my_prefix/share/icons/"

     if [ ! -f "$icons_target_dir" ] ; then  mkdir -vp "$icons_target_dir" ; fi

     cp -v $my_main_icon $my_prefix/share/icons
     _list_copy $icons_list_file $icons_source_dir $icons_target_dir
     if [ "$?" -eq 1 ] ; then  exit 1 ; fi
}

function _collect_GSettingsSchemas 
{
    my_GSettingsSchemas_path='/share/glib-2.0/schemas/'

    mkdir -vp $my_prefix$my_GSettingsSchemas_path
    cp -v $gtk3_prefix$my_GSettingsSchemas_path/* $my_prefix$my_GSettingsSchemas_path
}

function _collect_copyright_notes 
{
    # pyPdf copyright notes.

    mkdir -vp $my_prefix/share/doc/python-pypdf
    cp -v /usr/share/doc/python-pypdf/copyright $my_prefix/share/doc/python-pypdf

    # python copyright notes.

    mkdir -vp $my_prefix/share/doc/libpython
    cp -v /usr/share/doc/libpython2.7/copyright $my_prefix/share/doc/libpython

    # GTKmm copyright notes.

    mkdir -vp $my_prefix/share/doc/libgtkmm-3
    cp -v /usr/share/doc/libgtkmm-3.0-1/copyright $my_prefix/share/doc/libgtkmm-3

    # poppler copyright notes.

    mkdir -vp $my_prefix/share/doc/libpoppler
    cp -v /usr/share/doc/libpoppler-glib8/copyright $my_prefix/share/doc/libpoppler

    # glib copyright notes.

    mkdir -vp $my_prefix/share/doc/libglib
    cp -v /usr/share/doc/libglib2.0-0/copyright $my_prefix/share/doc/libglib

    # cairo copyright notes.

    mkdir -vp $my_prefix/share/doc/libcairo
    cp -v /usr/share/doc/libcairo2/copyright $my_prefix/share/doc/libcairo
}

case "$1" in
        collect)
            _collect_dlls
            _collect_GSettingsSchemas
            _collect_std_icons
#            _collect_copyright_notes 
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
            wine $my_prefix/bin/$my_project.exe
            ;;
        all)
            _collect_dlls
            _collect_GSettingsSchemas
            _collect_std_icons
            make -f Makefile.win32 melange
            _build_setup
            wine $my_prefix/bin/$my_project.exe
            ;;
        *)
            cat pdfmelange.ascii
            echo $"Usage: $0 {all|depload|depinst|clean|collect|buildsetup|run}"
            echo $"         - all : build and execute the windows program."
            echo $"         - depload : load dependencies."
            echo $"         - depinst : install dependencies."
            echo $"         - clean : remove unneeded files."
            echo $"         - collect :  assemble all files to $my_prefix."
            echo $"         - buildsetup :  create the setup executable."
            echo $"         - buildportable :  create a portable program version."
            echo $"         - run :  start the program with wine."
            exit 1
esac

