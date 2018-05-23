#g++ -rdynamic -Wall -std=c++14  -g guardbot.cpp -o guardbot -lm  -Lgattlib/build/dbus -lgattlib  `pkg-config --cflags --libs gtk+-3.0` `pkg-config  --libs --cflags opencv`


CPP=g++
CC= gcc

LD=ld
CFILES=

CFLAGS= 
CLIBS=-lm\
 -lstdc++\
 -lgtk-3\
 -lgdk-3\
 -lpangocairo-1.0\
 -lpango-1.0\
 -latk-1.0\
 -lcairo-gobject\
 -lcairo\
 -lgdk_pixbuf-2.0\
 -lgio-2.0\
 -lgobject-2.0\
 -lglib-2.0\
 -lopencv_shape\
 -lopencv_stitching\
 -lopencv_superres\
 -lopencv_videostab\
 -lopencv_aruco\
 -lopencv_bgsegm\
 -lopencv_bioinspired\
 -lopencv_ccalib\
 -lopencv_datasets\
 -lopencv_dpm\
 -lopencv_face\
 -lopencv_freetype\
 -lopencv_fuzzy\
 -lopencv_hdf\
 -lopencv_line_descriptor\
 -lopencv_optflow\
 -lopencv_video\
 -lopencv_plot\
 -lopencv_reg\
 -lopencv_saliency\
 -lopencv_stereo\
 -lopencv_structured_light\
 -lopencv_phase_unwrapping\
 -lopencv_rgbd\
 -lopencv_viz\
 -lopencv_surface_matching\
 -lopencv_text\
 -lopencv_ximgproc\
 -lopencv_calib3d\
 -lopencv_features2d\
 -lopencv_flann\
 -lopencv_xobjdetect\
 -lopencv_objdetect\
 -lopencv_ml\
 -lopencv_xphoto\
 -lopencv_highgui\
 -lopencv_videoio\
 -lopencv_imgcodecs\
 -lopencv_photo\
 -lopencv_imgproc\
 -lopencv_core\
 -lgattlib

CPATHS=-I/usr/include/gtk-3.0\
 -I/usr/include/at-spi2-atk/2.0\
 -I/usr/include/at-spi-2.0\
 -I/usr/include/dbus-1.0\
 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include\
 -I/usr/include/gtk-3.0\
 -I/usr/include/gio-unix-2.0/\
 -I/usr/include/cairo\
 -I/usr/include/pango-1.0\
 -I/usr/include/harfbuzz\
 -I/usr/include/pango-1.0\
 -I/usr/include/atk-1.0\
 -I/usr/include/cairo\
 -I/usr/include/pixman-1\
 -I/usr/include/freetype2\
 -I/usr/include/libpng16\
 -I/usr/include/gdk-pixbuf-2.0\
 -I/usr/include/libpng16\
 -I/usr/include/glib-2.0\
 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include\
 -I/usr/include/opencv

	
CPPFILES=guardbot.cpp

CPPFLAGS = -rdynamic -Wall -std=c++14  -g -pthread

OFILES= guardbot.o 

all:

	$(CPP) -c $(CPPFLAGS) $(CPATHS)  $(CPPFILES)
#	$(CC) -c $(CFLAGS) $(CFILES) $(CPATHS)
#	$(CPP) $(OFILES) $(CLIBS)  -o guardbot
	$(CC) -Wl,-rpath=./gattlib/build/dbus -L./gattlib/build/dbus $(OFILES) $(CLIBS)  -o guardbot

clean: 
	rm -f *.o guardbot


