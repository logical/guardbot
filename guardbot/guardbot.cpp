//g++ -rdynamic -Wall -std=gnu99  -g guardbot.cpp -o guardbot -lm  -lgattlib/build/gattlib.pc `pkg-config --cflags --libs gtk+-3.0` `pkg-config  --libs --cflags opencv`

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/videoio/videoio_c.h"
#include <gtk/gtk.h>
#include <vector>
#include <glib/gprintf.h>
#include <stdio.h>
#include <unistd.h>
#include "gattlib/include/gattlib.h"
#include "ui.h"
#define UI_FILE "ui.ui"

using namespace std;
using namespace cv;


//// Global variables
CvCapture* capture = 0;
cairo_surface_t *cairoimage;
gatt_connection_t* connection;

Mat prev_frame, next_frame, current_frame;

double SENSITIVITY = 50;
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
#define VWIDTH 480
#define VHEIGHT 320
#define MOTION_THRESHHOLD 5
#define WARNCOUNT 4
#define ALARMCOUNT 12
#define PASSCODE "ABCDEFGH"

int last = 0;



GtkWidget *video,*activatebutton;
gboolean warn =FALSE;
gboolean alrm=FALSE;
gboolean quit=FALSE;
gboolean active=FALSE;
int startdelay=20;
int triggercount=0;
//// Callbacks
void camon( GtkWidget *widget , gpointer data);
void update_mhi( IplImage* img, IplImage* dst, int diff_threshold );
void destroy (GtkWidget *widget, gpointer data);
void openfile(GtkWidget *widget, gpointer data);



void drawcvtogtk(GtkWidget* area,IplImage *im);
gboolean start_time_handler(GtkWidget *widget);
gboolean frame_time_handler(GtkWidget *widget);




gboolean cam;

void exposevideo(GtkWidget *widget, GdkEventExpose *ev,gpointer data){	
    cairo_t* cr=gdk_cairo_create(gtk_widget_get_window(widget));
    cairo_set_source_surface(cr,cairoimage, 0, 0);
    cairo_paint(cr);
    cairo_destroy (cr);
}
	
GtkWidget* create_window (void)
{
  GtkWidget *window;
  GtkBuilder *builder;
  GError* error = NULL;
  
  builder = gtk_builder_new ();
  gtk_builder_add_from_string(builder,(const gchar*)interface, strlen(interface),&error);


  if (!builder)
  {
	  g_warning ("Couldn't load builder file: %s", error->message);
	  g_error_free (error);
  }

//  gtk_builder_connect_signals (builder, NULL);
  window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  //filedialog = GTK_WIDGET (gtk_builder_get_object (builder, "filechooserdialog1"));
  video = GTK_WIDGET (gtk_builder_get_object (builder, "drawingarea1"));
  //alarmcheck = GTK_WIDGET (gtk_builder_get_object (builder, "checkbutton1"));
  //warncheck = GTK_WIDGET (gtk_builder_get_object (builder, "checkbutton2"));
  activatebutton = GTK_WIDGET (gtk_builder_get_object (builder, "button1"));

  //GtkRange *sensitivity=(GtkRange *)GTK_WIDGET (gtk_builder_get_object (builder, "hscale1"));
  //GtkAdjustment *adjustment=gtk_range_get_adjustment(sensitivity);
  //gtk_adjustment_set_lower(adjustment,10);
  //gtk_adjustment_set_upper(adjustment,200);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  g_signal_connect (video, "draw", G_CALLBACK (exposevideo), NULL);
  g_signal_connect (activatebutton, "clicked", G_CALLBACK (camon), NULL);
  
  g_object_unref (builder);
  
  return window;
}

gboolean start_time_handler(GtkWidget *widget){
  
//turn on camera
  
  gchar label[32];
  startdelay--;
  if(!startdelay){
    active=TRUE;
    g_sprintf(label, "CLICK HERE TO DEACTIVATE");
    gtk_button_set_label(GTK_BUTTON(activatebutton),label);
    return FALSE;
  }
  g_sprintf(label, "You have %d seconds to leave",startdelay);
  gtk_button_set_label(GTK_BUTTON(activatebutton),label);
  return TRUE;  
}  
  


gboolean alarm_time_handler( GtkWidget *widget) {
    if(triggercount>=ALARMCOUNT){
      system("./alarm&");
    }
    warn=FALSE;
    triggercount=0;
    startdelay=20;
    return FALSE;
}

void notification_handler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data) {
	if(strncmp(PASSCODE,(const char*)data,8)==0){
	system("aplay beep.wav&");
	if(active==TRUE)active=FALSE;
	else active=TRUE;
	}

}

int linkble(char *argv[]){
//this is specific for hm-10
	int ret;

	uuid_t hm10_uuid;
	hm10_uuid.type=SDP_UUID16;
	hm10_uuid.value.uuid16=(0xffe1);


	connection = gattlib_connect(NULL, argv[1], BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	if (connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}

	gattlib_register_notification(connection, notification_handler, NULL);

	ret = gattlib_notification_start(connection, &hm10_uuid);
	if (ret) {
		fprintf(stderr, "Fail to start notification %d \n",ret);
		return 1;
	}
	return 0;
}

int main( int   argc,char *argv[]){
  GtkWidget *window;
  if(argc > 1){	
	  if(linkble(argv)){
		printf("no bluetooth found\n");
	  }		
  }
  gtk_init (&argc, &argv);
  window = create_window ();
  gtk_widget_show (window);
  cairoimage=cairo_image_surface_create(CAIRO_FORMAT_RGB24,VWIDTH,VHEIGHT);
  gtk_widget_set_size_request( video, VWIDTH, VHEIGHT);
  capture = cvCaptureFromCAM( CV_CAP_ANY );
  if(!capture){
	  printf("cam not working\n");
	  return FALSE;
  }
  current_frame=cvarrToMat(cvQueryFrame(capture));
  cvtColor(current_frame,current_frame, CV_RGB2GRAY);
  next_frame=current_frame.clone();

  g_timeout_add(250,( GSourceFunc )frame_time_handler,NULL);
  gtk_main();
  
	if(capture)cvReleaseCapture( &capture );
	if(connection)gattlib_disconnect(connection);
  return 0;

}


/*
void sensitivitycb( GtkWidget *widget , gpointer data){
    GtkAdjustment *adjust=gtk_range_get_adjustment((GtkRange *)widget);
    SENSITIVITY=gtk_adjustment_get_value(adjust);
    printf("adjust sensitivity %f\n",SENSITIVITY);
}
*/

void camon( GtkWidget *widget , gpointer data) {

  if(active){
    gtk_main_quit();
    return;
  }
  g_timeout_add_seconds(1,( GSourceFunc )start_time_handler,NULL);

}


void destroy (GtkWidget *widget, gpointer data)
{
  	gtk_main_quit ();
}

// Check if there is motion in the result matrix
// count the number of changes and return.
inline int detectMotion(const Mat & motion, int max_deviation)
{

	int x_start = 0;
	int y_start = 0;
	int x_stop = VWIDTH;
	int y_stop = VHEIGHT;

    // calculate the standard deviation
    Scalar mean, stddev;
    meanStdDev(motion, mean, stddev);
    // if not to much changes then the motion is real (neglect agressive snow, temporary sunlight)
    if(stddev[0] < max_deviation)
    {
        int number_of_changes = 0;
 //       int min_x = motion.cols, max_x = 0;
 //       int min_y = motion.rows, max_y = 0;
        // loop over image and detect changes
        for(int j = y_start; j < y_stop; j+=2){ // height
            for(int i = x_start; i < x_stop; i+=2){ // width
                // check if at pixel (j,i) intensity is equal to 255
                // this means that the pixel is different in the sequence
                // of images (prev_frame, current_frame, next_frame)
                if(static_cast<int>(motion.at<uchar>(j,i)) == 255)
                {
                    number_of_changes++;
//                    if(min_x>i) min_x = i;
//                    if(max_x<i) max_x = i;
//                    if(min_y>j) min_y = j;
//                    if(max_y<j) max_y = j;
                }
            }
        }
        return number_of_changes;
    }
    return 0;
}


gboolean frame_time_handler(GtkWidget *widget){

  Mat d1, d2,motion;
// Maximum deviation of the image, the higher the value, the more motion is allowed
  int max_deviation = 50	;
  int number_of_changes=0;
// Take a new image
  Mat clr_frame = cvarrToMat(cvQueryFrame(capture));
	cairo_surface_destroy(cairoimage);
	cairoimage=cairo_image_surface_create_for_data ( clr_frame.data, CAIRO_FORMAT_RGB24,VWIDTH,VHEIGHT,clr_frame.step);
	gtk_widget_queue_draw (video);

  if(capture){
        current_frame = next_frame.clone();
        cvtColor(clr_frame,next_frame, CV_RGB2GRAY);
		absdiff(current_frame, next_frame, motion);  
      	threshold(motion, motion, 40, 255, CV_THRESH_BINARY);
//        cvtColor(motion,clr_frame, CV_GRAY2RGB);

        
       number_of_changes = detectMotion(motion, max_deviation);
//        g_printf("%d\n",number_of_changes);
// If a lot of changes happened, we assume something changed.
        if(number_of_changes>=MOTION_THRESHHOLD)
        {
			if(active){

			  if(!warn){
						if(triggercount>=WARNCOUNT){
							system("./warning&");
							warn=TRUE;
							//I lied, you only have 5 seconds
							g_timeout_add_seconds( 10, ( GSourceFunc )alarm_time_handler, NULL );			    
						}
			  }

				char name[16];
				IplImage im=IplImage(clr_frame);
				sprintf(name,"%s.jpg",g_date_time_format(g_date_time_new_now_local(),"%m-%d-%T"));
				printf("%s\n",name);
				cvSaveImage(name,&im,0);
				g_usleep(50000);
				triggercount++;
				g_printf(" %d triggers \n",triggercount);
			}
  
  		}

  }
    return TRUE;
}






