/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * This file is part of pdfMelange.
 * Copyright (C) Tobias Tandetzki 2014 <tobzki@googlemail.com>
 *
 * PDFmelange is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pdfMelange is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <glib.h>
#include <glib/gstdio.h>
#include <gtkmm.h>
#include <unistd.h>
#include <windows.h>

size_t __cdecl
/*ic_*/strnlen (const char *s, size_t n)
{
    size_t i;

    for (i = 0; i < n && *s != 0; i++) ++s;

    return i;
}

/**
 * \class MyWindow
 * 
 * \brief A small setup program.
 *
 * This class only works for Windows OSes.
 * 
 * Workflow:
 *  1. print some info and instructions.
 *  2. create buttons for install, uninstall and cancel.
 *
 */
class MyWindow : public Gtk::Window
{
	public:
		MyWindow(char* argv_null);
		~MyWindow();

	protected:
		void on_button_install_clicked();
		void on_button_uninst_clicked();
		void on_button_cancle_clicked();

		Gtk::Button    m_button_install;
		Gtk::Button    m_button_uninst;
		Gtk::Button    m_button_cancle;
		Gtk::Label     m_label;
		Gtk::Box       m_vbox;
		Gtk::Box       m_hbox1;
		Gtk::ButtonBox m_hbox2;
		Gtk::Image     m_image;

		std::string m_sLnkFileName;
		std::string m_sPrgPath;
		std::string m_sDirPath;

		const char *m_cPrgName;
		const char *m_cAppName;
};

/** 
 * \brief Constructor
 *
 * Workflow:
 *  1. create a horizontal box, insert an icon and text.
 *  2. create a second horizontal box, insert buttons.
 *  3. create a vertical box, insert the horizontal boxes.
 *  3. find all pathes, e.g. installation directory, full program path ...
 *
 * \param argv_null: application path, passed by main argv[0].
 * \return void
 */
MyWindow::MyWindow(char* argv_null): Gtk::Window(),
    m_button_install("Install"),
    m_button_uninst("Uninstall"),
    m_button_cancle(" Cancel "),
    m_label("This is a minimal installation for portable use\n"
            "that does not require administrator rights.\n"
            "Use this version only if you know exactly what you are doing.\n\n"
            "Please use the setup version for a standard installation,\n"
            "see http://dertoki.github.io/pdfMelange.\n"),

    m_vbox(Gtk::ORIENTATION_VERTICAL),
    m_hbox1(Gtk::ORIENTATION_HORIZONTAL),
    m_hbox2(Gtk::ORIENTATION_HORIZONTAL)
{
    m_cPrgName = "pdfMelange.exe";
    m_cAppName = "pdfMelange";

    set_title("pdfMelange Setup");

    m_label.set_use_markup(true);
    m_image.set_from_icon_name("pdfMelange", Gtk::ICON_SIZE_DIALOG);

    m_hbox2.pack_start(m_button_install);
    m_hbox2.pack_start(m_button_uninst);
    m_hbox2.pack_start(m_button_cancle);
    m_hbox2.set_layout(Gtk::BUTTONBOX_END);

    m_button_install.set_tooltip_text("Create a desktop entry icon.");
    m_button_uninst.set_tooltip_text("Remove the desktop entry icon\n and installation directory.");
    m_button_install.signal_clicked().connect(sigc::mem_fun(*this,&MyWindow::on_button_install_clicked));
    m_button_uninst.signal_clicked().connect(sigc::mem_fun(*this,&MyWindow::on_button_uninst_clicked));
    m_button_cancle.signal_clicked().connect(sigc::mem_fun(*this,&MyWindow::on_button_cancle_clicked));

    m_hbox1.set_spacing(15);
    m_hbox1.pack_start(m_image);
    m_hbox1.pack_end(m_label);

    m_vbox.pack_start(m_hbox1);
    m_vbox.pack_start(m_hbox2);
    m_vbox.set_border_width(5);

    add(m_vbox);

    set_default_size(200,50);
    show_all_children(true);

    const gchar *cdesktop = g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP);;
    std::string pwd = g_get_current_dir();

    m_sLnkFileName.append(cdesktop);
    m_sLnkFileName.append("\\pdfMelange.url");

    m_sPrgPath.append(pwd);
    if (g_file_test("bin\\pdfMelange.exe", G_FILE_TEST_EXISTS))
    {
        m_sPrgPath.append("\\bin\\pdfMelange.exe");
    }
    else if (g_file_test(".\\pdfMelange.exe", G_FILE_TEST_EXISTS))
    {
        m_sPrgPath.append("\\pdfMelange.exe");
    }
    else
    {
        std::string messagetext;
        messagetext.append("Can't find <b>pdfMelange.exe</b>\n\n");
        messagetext.append("<b>");
        messagetext.append(pwd);
        messagetext.append("</b>\n");
        messagetext.append("is the wrong location to call <b>");
        messagetext.append(g_path_get_basename(argv_null));
        messagetext.append("</b>");
        Gtk::MessageDialog messagedialog(*this,
                                         messagetext.c_str(),
                                         true /* use_markup */,
                                         Gtk::MESSAGE_ERROR,
                                         Gtk::BUTTONS_CLOSE);
        messagedialog.run();
        hide();
        exit(0);
    };

    std::string::size_type found = pwd.find(m_cAppName);
    if (found != std::string::npos){
        m_sDirPath = pwd.substr(0, found).append(m_cAppName);
    }

    std::cout << "         pwd: " <<  pwd << std::endl;
    std::cout << "     DirPath: " <<  m_sDirPath << std::endl;
    std::cout << "     PrgPath: " <<  m_sPrgPath << std::endl;
    std::cout << "   argv_null: " <<  argv_null << std::endl;
    std::cout << " LnkFileName: " <<  m_sLnkFileName << std::endl;
}

/** 
 * \brief Destructor
 */
MyWindow::~MyWindow()
{
}

/** 
 * \brief do the installation.
 *
 * Simply create a link file on the windows desktop and give a message about what happend.
 *
 */
void MyWindow::on_button_install_clicked()
{
    std::ofstream lnkFile;
    lnkFile.open (m_sLnkFileName);
    lnkFile << "[InternetShortcut]\n";
    lnkFile << "URL=file:///" << m_sPrgPath << "\n";
    lnkFile << "WorkingDirectory=" << m_sDirPath << "\\bin\n";
    lnkFile << "IconIndex=0\n";
    lnkFile << "IconFile=" << m_sPrgPath << "\n";
    lnkFile.close();

    Gtk::MessageDialog messagedialog(*this,
                                     "A desktop icon has been created successfully.\n"
                                     "Double-click the icon <b>pdfMelange</b> to start ...",
                                     true /* use_markup */,
                                     Gtk::MESSAGE_INFO,
                                     Gtk::BUTTONS_CLOSE);
    messagedialog.run();
    hide();
}

/** 
 * \brief do the uninstallation.
 *
 * Workflow:
 *  1. print a warning with an option to cancel.
 *  2. remove the desktop icon.
 *  3. remove the program folder with all its files.
 *
 */
void MyWindow::on_button_uninst_clicked()
{
    std::string messageText("Do you really want to delete:\n");
    gboolean icon_exists = FALSE;
    gboolean  dir_exists = FALSE;

    icon_exists = g_file_test(m_sLnkFileName.c_str(), G_FILE_TEST_EXISTS);
    if (icon_exists){
            messageText.append("The desktop icon <b>pdfMelange</b>.\n");
    };

    std::string::size_type found = m_sDirPath.find(m_cAppName);
    if (found != std::string::npos
        && g_path_is_absolute(m_sDirPath.c_str())
        && g_file_test(m_sDirPath.c_str(), G_FILE_TEST_EXISTS)
        && g_file_test(m_sDirPath.c_str(), G_FILE_TEST_IS_DIR)
       ){
        dir_exists = TRUE;
        messageText.append("Directory: <b>");
        messageText.append(m_sDirPath);
        messageText.append("</b>\n");
    }

    if (dir_exists || icon_exists){
        Gtk::MessageDialog messagedialog(*this,
                                         messageText.c_str(),
                                         true /* use_markup */,
                                         Gtk::MESSAGE_WARNING,
                                         Gtk::BUTTONS_OK_CANCEL);
        //messagedialog.set_secondary_text("File already exists. Replace the old one?");

        int result = messagedialog.run();

        //Handle the response:
        switch(result)
        {
            case(Gtk::RESPONSE_OK):
            {
                if (icon_exists) g_remove(m_sLnkFileName.c_str());
				//
				// This is extremly dangerous, because "rmdir /s /q <m_sDirPath>" is called in the background.
				// It's neccessary to test "m_sDirPath" on correctnes und existance.
				//
                if (dir_exists){
                    STARTUPINFO si = {0};
                    PROCESS_INFORMATION pi = {0};
                    std::string cmd("cmd.exe /C "
                                     "ping 1.1.1.1 -n 1 -w 3000 > Nul & "
                                     "rmdir /s /q ");
                    cmd.append(m_sDirPath);

                    CreateProcess(NULL, (LPSTR) cmd.c_str(), NULL, NULL, FALSE,
                                  CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
                }
                break;
            }
            case(Gtk::RESPONSE_CANCEL):
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    std::cout << "Bye!" << std::endl;
    hide();
}

/** 
 * \brief cancel and exit the dialog.
 */
void MyWindow::on_button_cancle_clicked()
{
    hide();
    std::cout << "Bye!" << std::endl;
}

/** 
 * \brief Entry point of the program.
 * 
 * Instantiate a gtk main window.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings, 
 *                 representing the arguments that were entered on the command line 
 *                 when the program was started.
 * \return -
 */
int main(int argc, char **argv)
{
    Gtk::Main main(argc,argv);

    MyWindow window(argv[0]);

    main.run(window);

    return 0;
}

