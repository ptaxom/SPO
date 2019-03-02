#include <gtk/gtk.h>
#include <cairo.h>
#include <string>
#include <iostream>
#include <complex>


int offset_x;
int offset_y;

int height;
int width;
int y = 0;



double x_start = -1.5, y_start = 1, x_end = 1.5, y_end = -1;
double dx, dy;

unsigned char * cache;
int cached_region = 0;
bool cached = false;

#include<random>
std::random_device rd;

gboolean timeout(GtkWidget *widget)
{
    y++;
    if (y >= height)
    {
        cached = true;
        gtk_widget_queue_draw(widget);
        return FALSE;
    }
    if (rd() % 5 == 1)
        y--;
    else
        gtk_widget_queue_draw_area(widget, 0, y, width, 1);
    return TRUE;
}


GdkRGBA parseColor(double i)
{
    GdkRGBA cl = {i / 256, 0, 0, 1};
    return cl;
}

GdkRGBA getColor(int x, int y)
{
    double curX = x_start + (double)x * dx;
    double curY = y_start + (double)y * dy;
    std::complex<double> C(curX, curY), Z(0, 0);
    unsigned char iter = 0;
    while(iter < 255 && (Z.imag() * Z.imag() + Z.real() * Z.real() < 4.0))
    {
        Z = Z * Z + C;
        iter++;
    }
    double i = (double)iter;
    cache[cached_region++] = iter;
    return parseColor(i);
}




void draw(GtkWidget *widget, cairo_t *cr)
{

    GtkWidget *window = gtk_widget_get_toplevel(widget);
    gtk_window_get_size(GTK_WINDOW(window), &width, &height);
    
    if (!cached){
        for (int x = 0; x < width; x++)
        {
            GdkRGBA color = getColor(x, y);
            cairo_rectangle (cr, x, y, 1, 1);
            gdk_cairo_set_source_rgba(cr, &color);
            cairo_fill (cr);
        }
    }
    else
    {
        for(int i = 0; i < cached_region; i++)
            {
                GdkRGBA color = parseColor(cache[i]);
                int x = i % width;
                int y = i / width;
                cairo_rectangle (cr, x, y, 1, 1);
                gdk_cairo_set_source_rgba(cr, &color);
                cairo_fill (cr);
            }   


    }
    
}

void quit_handler(void)
{
    if (cache)
        delete cache;
    gtk_main_quit();        
    int pid = getpid();
    killpg(getpgid(pid), SIGKILL);
}


void initGTK(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *darea;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    darea = gtk_drawing_area_new();

    gtk_window_move(GTK_WINDOW(window), offset_x, offset_y);

    gtk_container_add(GTK_CONTAINER(window), darea);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    cache = new unsigned char[width * height];

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(quit_handler), NULL);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(draw), NULL);

    g_timeout_add(30, (GSourceFunc)timeout, window);

    gtk_widget_show_all(window);
    gtk_main();
    delete cache;
}

int main(int argc, char **argv)
{
    if (argc == 8)
    {

        offset_x = std::stoi(std::string(argv[0]));
        offset_y = std::stoi(std::string(argv[1]));
        width    = std::stoi(std::string(argv[2]));
        height   = std::stoi(std::string(argv[3]));
        x_start  = std::stod(std::string(argv[4]));
        y_start  = std::stod(std::string(argv[5]));
        x_end    = std::stod(std::string(argv[6]));
        y_end    = std::stod(std::string(argv[7]));

        dx = (x_end - x_start) / (double)width;
        dy = (y_end - y_start) / (double)height;
        initGTK(argc, argv);
    }
   
}