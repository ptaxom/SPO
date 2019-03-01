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

gboolean timeout(GtkWidget *widget)
{
    y++;
    if (y >= height)
        return FALSE;
    gtk_widget_queue_draw_area(widget, 0, y, width, 1);
    return TRUE;
}

GdkRGBA getColor(int x, int y)
{
    double curX = x_start + (double)x * dx;
    double curY = y_start + (double)y * dy;
    std::complex<double> C(curX, curY), Z(0, 0);
    int iter = 0;
    while(iter < 255 && (Z.imag() * Z.imag() + Z.real() * Z.real() < 4.0))
    {
        Z = Z * Z + C;
        iter++;
    }
    double i = (double)iter;
    GdkRGBA cl = {i / 256, 0, 0, 1};
    return cl;
}


void draw(GtkWidget *widget, cairo_t *cr)
{

    GtkWidget *window = gtk_widget_get_toplevel(widget);
    gtk_window_get_size(GTK_WINDOW(window), &width, &height);

    for (int x = 0; x < width; x++)
    {
        GdkRGBA color = getColor(x, y);
        cairo_rectangle (cr, x, y, 1, 1);
        gdk_cairo_set_source_rgba(cr, &color);
        cairo_fill (cr);
    }

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

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(draw), NULL);

    g_timeout_add(30 + 10 * (rand() % 10), (GSourceFunc)timeout, window);

    gtk_widget_show_all(window);
    gtk_main();
}

int main(int argc, char **argv)
{
    // for(int i = 0; i < 8; i++)
    //     std::cout << "Arg " << std::string(argv[i]) << std::endl;
        // printf("%s\n", argv[i]);
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

        std::cout << getpid() << " offset x: " << offset_x << std::endl;
        std::cout << getpid() << " offset y: " << offset_y << std::endl;

        dx = (x_end - x_start) / (double)width;
        dy = (y_end - y_start) / (double)height;
        initGTK(argc, argv);
    }
   
}