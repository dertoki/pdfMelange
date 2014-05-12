static Glib::ustring ui_builder =
"<?xml version='1.0' encoding='UTF-8'?>"
"<!-- Generated with glade 3.16.1 -->"
"<interface>"
"  <requires lib='gtk+' version='3.0'/>"
"  <object class='GtkAction' id='about'>"
"    <property name='label' translatable='yes'>About ...</property>"
"    <property name='tooltip' translatable='yes'>About PdfMelange ...</property>"
"    <property name='stock_id'>gtk-about</property>"
"    <property name='icon_name'>help-about</property>"
"  </object>"
"  <object class='GtkAccelGroup' id='accelgroup1'/>"
"  <object class='GtkAction' id='append'>"
"    <property name='label' translatable='yes'>Append a file ...</property>"
"    <property name='short_label' translatable='yes'>Append ...</property>"
"    <property name='tooltip' translatable='yes'>Append a file ...</property>"
"    <property name='stock_id'>gtk-add</property>"
"    <property name='icon_name'>list-add</property>"
"  </object>"
"  <object class='GtkAction' id='copy'>"
"    <property name='label' translatable='yes'>_Copy</property>"
"    <property name='short_label' translatable='yes'>Copy</property>"
"    <property name='tooltip' translatable='yes'>Copy selected pages</property>"
"    <property name='stock_id'>gtk-copy</property>"
"    <property name='icon_name'>edit-copy</property>"
"  </object>"
"  <object class='GtkAction' id='cut'>"
"    <property name='label' translatable='yes'>Cu_t</property>"
"    <property name='short_label' translatable='yes'>Cut</property>"
"    <property name='tooltip' translatable='yes'>Cut selected pages</property>"
"    <property name='stock_id'>gtk-cut</property>"
"    <property name='icon_name'>edit-cut</property>"
"  </object>"
"  <object class='GtkAction' id='delete'>"
"    <property name='label' translatable='yes'>_Delete</property>"
"    <property name='short_label' translatable='yes'>Del</property>"
"    <property name='tooltip' translatable='yes'>Delete selected pages</property>"
"    <property name='stock_id'>gtk-remove</property>"
"    <property name='icon_name'>list-remove</property>"
"  </object>"
"  <object class='GtkAction' id='insert'>"
"    <property name='label' translatable='yes'>Insert a file ...</property>"
"    <property name='short_label' translatable='yes'>Insert ...</property>"
"    <property name='tooltip' translatable='yes'>Insert a file ...</property>"
"    <property name='stock_id'>gtk-add</property>"
"    <property name='icon_name'>document-save-as</property>"
"  </object>"
"  <object class='GtkAction' id='move_page_down'>"
"    <property name='label' translatable='yes'>Move element down</property>"
"    <property name='short_label' translatable='yes'>down</property>"
"    <property name='tooltip' translatable='yes'>Move element down</property>"
"    <property name='stock_id'>gtk-go-down</property>"
"    <property name='icon_name'>go-down</property>"
"  </object>"
"  <object class='GtkAction' id='move_page_up'>"
"    <property name='label' translatable='yes'>Move element down</property>"
"    <property name='short_label' translatable='yes'>down</property>"
"    <property name='tooltip' translatable='yes'>Move element up</property>"
"    <property name='stock_id'>gtk-go-up</property>"
"    <property name='icon_name'>go-up</property>"
"  </object>"
"  <object class='GtkAction' id='open'>"
"    <property name='label' translatable='yes'>O_pen</property>"
"    <property name='short_label' translatable='yes'>O_pen</property>"
"    <property name='tooltip' translatable='yes'>Open a file ...</property>"
"    <property name='stock_id'>gtk-open</property>"
"    <property name='icon_name'>document-open</property>"
"  </object>"
"  <object class='GtkAction' id='paste'>"
"    <property name='label' translatable='yes'>_Paste</property>"
"    <property name='short_label' translatable='yes'>paste</property>"
"    <property name='tooltip' translatable='yes'>Paste selected pages</property>"
"    <property name='stock_id'>gtk-paste</property>"
"    <property name='icon_name'>edit-paste</property>"
"  </object>"
"  <object class='GtkAction' id='preferences'>"
"    <property name='label' translatable='yes'>Pr_eferences</property>"
"    <property name='tooltip' translatable='yes'>User preferendes dialog.</property>"
"    <property name='stock_id'>gtk-properties</property>"
"    <property name='icon_name'>preferences-system</property>"
"  </object>"
"  <object class='GtkAction' id='quit'>"
"    <property name='label' translatable='yes'>Quit</property>"
"    <property name='short_label' translatable='yes'>Quit</property>"
"    <property name='tooltip' translatable='yes'>Quit PDFmelange</property>"
"    <property name='stock_id'>gtk-quit</property>"
"    <property name='icon_name'>application-exit</property>"
"  </object>"
"  <object class='GtkAction' id='rotate_ccw'>"
"    <property name='label'>Rotate CCW</property>"
"    <property name='short_label' translatable='yes'>CCW</property>"
"    <property name='tooltip' translatable='yes'>Rotate counter-clockwise</property>"
"    <property name='icon_name'>object-rotate-left</property>"
"  </object>"
"  <object class='GtkAction' id='rotate_cw'>"
"    <property name='label'>Rotate CW</property>"
"    <property name='short_label' translatable='yes'>CW</property>"
"    <property name='tooltip' translatable='yes'>Rotate clockwise</property>"
"    <property name='icon_name'>object-rotate-right</property>"
"  </object>"
"  <object class='GtkAction' id='save'>"
"    <property name='label' translatable='yes'>Save ...</property>"
"    <property name='short_label' translatable='yes'>Save ...</property>"
"    <property name='tooltip' translatable='yes'>Save ...</property>"
"    <property name='stock_id'>gtk-save</property>"
"    <property name='icon_name'>document-save</property>"
"  </object>"
"  <object class='GtkAction' id='save_as'>"
"    <property name='label' translatable='yes'>Save as ...</property>"
"    <property name='short_label' translatable='yes'>Save as ...</property>"
"    <property name='tooltip' translatable='yes'>Save as ...</property>"
"    <property name='stock_id'>gtk-save-as</property>"
"    <property name='icon_name'>document-save-as</property>"
"  </object>"
"  <object class='GtkAction' id='select_all'>"
"    <property name='label' translatable='yes'>Unselect all</property>"
"    <property name='short_label' translatable='yes'>Select all</property>"
"    <property name='tooltip' translatable='yes'>Select all</property>"
"  </object>"
"  <object class='GtkAction' id='systeminfo'>"
"    <property name='label' translatable='yes'>Systeminfo ...</property>"
"    <property name='tooltip' translatable='yes'>System information ...</property>"
"    <property name='stock_id'>gtk-properties</property>"
"    <property name='icon_name'>applications-system</property>"
"  </object>"
"  <object class='GtkAction' id='unselect_all '>"
"    <property name='label' translatable='yes'>Unselect all</property>"
"    <property name='short_label' translatable='yes'>Unselect all</property>"
"    <property name='tooltip' translatable='yes'>Unselect all</property>"
"  </object>"
"  <object class='GtkAction' id='zoom_fit'>"
"    <property name='label' translatable='yes'>_Zoom fit</property>"
"    <property name='short_label' translatable='yes'>Zoom fit</property>"
"    <property name='tooltip' translatable='yes'>Zoom fit</property>"
"    <property name='stock_id'>gtk-zoom-fit</property>"
"  </object>"
"  <object class='GtkAction' id='zoom_in'>"
"    <property name='label' translatable='yes'>_Zoom in</property>"
"    <property name='short_label' translatable='yes'>Zoom in</property>"
"    <property name='tooltip' translatable='yes'>Zoom in</property>"
"    <property name='stock_id'>gtk-zoom-in</property>"
"  </object>"
"  <object class='GtkAction' id='zoom_out'>"
"    <property name='label' translatable='yes'>_Zoom out</property>"
"    <property name='short_label' translatable='yes'>Zoom out</property>"
"    <property name='tooltip' translatable='yes'>Zoom out</property>"
"    <property name='stock_id'>gtk-zoom-out</property>"
"  </object>"
"  <object class='GtkWindow' id='main_window'>"
"    <property name='visible'>True</property>"
"    <property name='can_focus'>False</property>"
"    <property name='title'>pdfMelange</property>"
"    <property name='default_width'>620</property>"
"    <property name='default_height'>675</property>"
"    <child>"
"      <object class='GtkVBox' id='vbox1'>"
"        <property name='visible'>True</property>"
"        <property name='can_focus'>False</property>"
"        <child>"
"          <object class='GtkMenuBar' id='menubar1'>"
"            <property name='visible'>True</property>"
"            <property name='can_focus'>False</property>"
"            <child>"
"              <object class='GtkMenuItem' id='menuitem1'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>_File</property>"
"                <property name='use_underline'>True</property>"
"                <child type='submenu'>"
"                  <object class='GtkMenu' id='menu1'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem-open'>"
"                        <property name='related_action'>open</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='o' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem-save'>"
"                        <property name='related_action'>save</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='s' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem-save-as'>"
"                        <property name='related_action'>save_as</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='s' signal='activate' modifiers='GDK_SHIFT_MASK | GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkSeparatorMenuItem' id='separatormenuitem1'>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem-quit'>"
"                        <property name='related_action'>quit</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='q' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                  </object>"
"                </child>"
"              </object>"
"            </child>"
"            <child>"
"              <object class='GtkMenuItem' id='menuitem2'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>_Edit</property>"
"                <property name='use_underline'>True</property>"
"                <child type='submenu'>"
"                  <object class='GtkMenu' id='menu2'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_copy'>"
"                        <property name='related_action'>copy</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='c' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_cut'>"
"                        <property name='related_action'>cut</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='x' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_paste'>"
"                        <property name='related_action'>paste</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='v' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_delete'>"
"                        <property name='related_action'>delete</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <accelerator key='Delete' signal='activate'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_append'>"
"                        <property name='related_action'>append</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='tooltip_text' translatable='yes'>Append a file ...</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='a' signal='activate' modifiers='GDK_SHIFT_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_insert'>"
"                        <property name='related_action'>insert</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='i' signal='activate' modifiers='GDK_SHIFT_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkSeparatorMenuItem' id='menuline'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_preferences'>"
"                        <property name='related_action'>preferences</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                      </object>"
"                    </child>"
"                  </object>"
"                </child>"
"              </object>"
"            </child>"
"            <child>"
"              <object class='GtkMenuItem' id='menuitem3'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>_View</property>"
"                <property name='use_underline'>True</property>"
"                <child type='submenu'>"
"                  <object class='GtkMenu' id='menu4'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <child>"
"                      <object class='GtkMenuItem' id='select_all '>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='related_action'>select_all</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='has_tooltip'>True</property>"
"                        <property name='tooltip_markup'>_Unselect all</property>"
"                        <property name='tooltip_text' translatable='yes'>Unselect all</property>"
"                        <property name='right_justified'>True</property>"
"                        <property name='label' translatable='yes'>_Select all</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='a' signal='activate' modifiers='GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='unselect_all'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='related_action'>unselect_all </property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='tooltip_text' translatable='yes'>Unselect all</property>"
"                        <property name='label' translatable='yes'>_Unselect all</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='a' signal='activate' modifiers='GDK_SHIFT_MASK | GDK_CONTROL_MASK'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkSeparatorMenuItem' id='separatormenuitem2'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='zoom_all'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='related_action'>zoom_fit</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='label' translatable='yes'>Zoom all</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='a' signal='activate'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='zoom_in1'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='related_action'>zoom_in</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='label' translatable='yes'>Zoom in</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='plus' signal='activate'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='zoom_out1'>"
"                        <property name='use_action_appearance'>False</property>"
"                        <property name='related_action'>zoom_out</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='label' translatable='yes'>Zoom out</property>"
"                        <property name='use_underline'>True</property>"
"                        <accelerator key='minus' signal='activate'/>"
"                      </object>"
"                    </child>"
"                  </object>"
"                </child>"
"              </object>"
"            </child>"
"            <child>"
"              <object class='GtkMenuItem' id='menuitem4'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>_Help</property>"
"                <property name='use_underline'>True</property>"
"                <child type='submenu'>"
"                  <object class='GtkMenu' id='menu3'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_about'>"
"                        <property name='related_action'>about</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='tooltip_text' translatable='yes'>About PdfMelange</property>"
"                        <accelerator key='F1' signal='activate'/>"
"                      </object>"
"                    </child>"
"                    <child>"
"                      <object class='GtkMenuItem' id='menuitem_systeminfo'>"
"                        <property name='related_action'>systeminfo</property>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                      </object>"
"                    </child>"
"                  </object>"
"                </child>"
"              </object>"
"            </child>"
"          </object>"
"          <packing>"
"            <property name='expand'>False</property>"
"            <property name='fill'>True</property>"
"            <property name='position'>0</property>"
"          </packing>"
"        </child>"
"        <child>"
"          <object class='GtkToolbar' id='toolbar1'>"
"            <property name='visible'>True</property>"
"            <property name='can_focus'>False</property>"
"            <property name='toolbar_style'>icons</property>"
"            <property name='icon_size'>1</property>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_open'>"
"                <property name='related_action'>open</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton1</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_save'>"
"                <property name='related_action'>save</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton1</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkSeparatorToolItem' id='separatortoolitem1'>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>False</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_quit'>"
"                <property name='related_action'>quit</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton1</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkSeparatorToolItem' id='separatortoolitem2'>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>False</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_rot_cw'>"
"                <property name='related_action'>rotate_cw</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton2</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_rot_ccw'>"
"                <property name='related_action'>rotate_ccw</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton3</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_move_page_up'>"
"                <property name='related_action'>move_page_up</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton2</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_move_page_down'>"
"                <property name='related_action'>move_page_down</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton3</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_insert'>"
"                <property name='related_action'>insert</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton1</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_delete'>"
"                <property name='related_action'>delete</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton1</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkSeparatorToolItem' id='separatortoolitem3'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>False</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_zoom_in'>"
"                <property name='related_action'>zoom_in</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton2</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_zoom_out'>"
"                <property name='related_action'>zoom_out</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton2</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolButton' id='toolbutton_zoom_fit'>"
"                <property name='related_action'>zoom_fit</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <property name='label' translatable='yes'>toolbutton2</property>"
"                <property name='use_underline'>True</property>"
"              </object>"
"              <packing>"
"                <property name='expand'>False</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkToolItem' id='toolbutton1'>"
"                <property name='use_action_appearance'>False</property>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>False</property>"
"                <child>"
"                  <object class='GtkSpinner' id='spinner1'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                  </object>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name='expand'>True</property>"
"                <property name='homogeneous'>True</property>"
"              </packing>"
"            </child>"
"          </object>"
"          <packing>"
"            <property name='expand'>False</property>"
"            <property name='fill'>True</property>"
"            <property name='position'>1</property>"
"          </packing>"
"        </child>"
"        <child>"
"          <object class='GtkHPaned' id='hpaned11111'>"
"            <property name='visible'>True</property>"
"            <property name='can_focus'>True</property>"
"            <property name='border_width'>2</property>"
"            <property name='position'>180</property>"
"            <property name='position_set'>True</property>"
"            <child>"
"              <object class='GtkScrolledWindow' id='scrolledwindow1'>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>True</property>"
"                <child>"
"                  <object class='GtkTreeView' id='treeview1'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>True</property>"
"                    <child internal-child='selection'>"
"                      <object class='GtkTreeSelection' id='treeview-selection1'/>"
"                    </child>"
"                  </object>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name='resize'>False</property>"
"                <property name='shrink'>True</property>"
"              </packing>"
"            </child>"
"            <child>"
"              <object class='GtkNotebook' id='notebook1'>"
"                <property name='visible'>True</property>"
"                <property name='can_focus'>True</property>"
"                <child>"
"                  <object class='GtkDrawingArea' id='drawingareaPDF'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                  </object>"
"                </child>"
"                <child type='tab'>"
"                  <object class='GtkLabel' id='label1'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <property name='label' translatable='yes'>PDF View</property>"
"                  </object>"
"                  <packing>"
"                    <property name='tab_fill'>False</property>"
"                  </packing>"
"                </child>"
"                <child>"
"                  <object class='GtkScrolledWindow' id='scrolledwindow2'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>True</property>"
"                    <property name='hscrollbar_policy'>always</property>"
"                    <property name='vscrollbar_policy'>always</property>"
"                    <property name='shadow_type'>out</property>"
"                    <child>"
"                      <object class='GtkViewport' id='viewport1'>"
"                        <property name='visible'>True</property>"
"                        <property name='can_focus'>False</property>"
"                        <property name='shadow_type'>etched-in</property>"
"                        <child>"
"                          <object class='GtkLabel' id='labelFileInfo'>"
"                            <property name='visible'>True</property>"
"                            <property name='can_focus'>False</property>"
"                            <property name='xalign'>0</property>"
"                            <property name='yalign'>0</property>"
"                          </object>"
"                        </child>"
"                      </object>"
"                    </child>"
"                  </object>"
"                  <packing>"
"                    <property name='position'>1</property>"
"                  </packing>"
"                </child>"
"                <child type='tab'>"
"                  <object class='GtkLabel' id='label2'>"
"                    <property name='visible'>True</property>"
"                    <property name='can_focus'>False</property>"
"                    <property name='label' translatable='yes'>File Info</property>"
"                  </object>"
"                  <packing>"
"                    <property name='position'>1</property>"
"                    <property name='tab_fill'>False</property>"
"                  </packing>"
"                </child>"
"              </object>"
"              <packing>"
"                <property name='resize'>True</property>"
"                <property name='shrink'>True</property>"
"              </packing>"
"            </child>"
"          </object>"
"          <packing>"
"            <property name='expand'>True</property>"
"            <property name='fill'>True</property>"
"            <property name='position'>2</property>"
"          </packing>"
"        </child>"
"        <child>"
"          <object class='GtkStatusbar' id='statusbar'>"
"            <property name='visible'>True</property>"
"            <property name='can_focus'>False</property>"
"            <property name='spacing'>2</property>"
"          </object>"
"          <packing>"
"            <property name='expand'>False</property>"
"            <property name='fill'>True</property>"
"            <property name='position'>3</property>"
"          </packing>"
"        </child>"
"      </object>"
"    </child>"
"  </object>"
"</interface>";
