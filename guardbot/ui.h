const char *interface=
"<?xml version='1.0' encoding='UTF-8'?>\
<!-- Generated with glade 3.22.0 -->\
<interface>\
  <requires lib='gtk+' version='3.0'/>\
  <object class='GtkWindow' id='window1'>\
    <property name='can_focus'>False</property>\
    <signal name='destroy' handler='destroy' swapped='no'/>\
    <child>\
      <object class='GtkVBox' id='vbox1'>\
        <property name='visible'>True</property>\
        <property name='can_focus'>False</property>\
        <child>\
          <object class='GtkMenuBar' id='menubar1'>\
            <property name='visible'>True</property>\
            <property name='can_focus'>False</property>\
            <child>\
              <object class='GtkMenuItem' id='menuitem4'>\
                <property name='visible'>True</property>\
                <property name='can_focus'>False</property>\
                <property name='label' translatable='yes'>_Help</property>\
                <property name='use_underline'>True</property>\
                <child type='submenu'>\
                  <object class='GtkMenu' id='menu3'>\
                    <property name='visible'>True</property>\
                    <property name='can_focus'>False</property>\
                    <child>\
                      <object class='GtkImageMenuItem' id='imagemenuitem10'>\
                        <property name='label'>gtk-about</property>\
                        <property name='visible'>True</property>\
                        <property name='can_focus'>False</property>\
                        <property name='use_underline'>True</property>\
                        <property name='use_stock'>True</property>\
                      </object>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name='expand'>False</property>\
            <property name='fill'>True</property>\
            <property name='position'>0</property>\
          </packing>\
        </child>\
        <child>\
          <object class='GtkDrawingArea' id='drawingarea1'>\
            <property name='visible'>True</property>\
            <property name='can_focus'>False</property>\
          </object>\
          <packing>\
            <property name='expand'>True</property>\
            <property name='fill'>True</property>\
            <property name='position'>1</property>\
          </packing>\
        </child>\
        <child>\
          <object class='GtkVBox' id='vbox2'>\
            <property name='visible'>True</property>\
            <property name='can_focus'>False</property>\
            <child>\
              <placeholder/>\
            </child>\
            <child>\
              <placeholder/>\
            </child>\
            <child>\
              <placeholder/>\
            </child>\
            <child>\
              <object class='GtkButton' id='button1'>\
                <property name='label' translatable='yes'>You have 20 seconds to leave!</property>\
                <property name='visible'>True</property>\
                <property name='can_focus'>True</property>\
                <property name='receives_default'>True</property>\
                <signal name='clicked' handler='camon' swapped='no'/>\
              </object>\
              <packing>\
                <property name='expand'>True</property>\
                <property name='fill'>True</property>\
                <property name='position'>3</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name='expand'>True</property>\
            <property name='fill'>True</property>\
            <property name='position'>2</property>\
          </packing>\
        </child>\
      </object>\
    </child>\
    <child type='titlebar'>\
      <placeholder/>\
    </child>\
  </object>\
</interface>";
