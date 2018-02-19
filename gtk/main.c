#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <gtk/gtk.h>
#include "lib/gtk_functions.c"


void print_hello(GtkWidget *, gpointer);
void activate (GtkApplication *, gpointer);
void window_create(GtkApplication *);
void button_create(char*, int, int, int, int, GtkWidget *, gpointer, int);


GtkWidget *window;
GtkWidget *notebook;
GtkWidget *vbox;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *pTabLabel;
gchar *sTabLabel;


int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);     // Object destruction

  return status;
}



void print_hello(GtkWidget *widget, gpointer data)
{
  g_print ("Hello World\n");
}

void activate(GtkApplication *app, gpointer user_data)
{

    window_create(app);

    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

    gtk_container_add(GTK_CONTAINER(window), notebook);

    vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    sTabLabel = g_strdup_printf("Onglet 1");
    pTabLabel = gtk_label_new(sTabLabel);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, pTabLabel);


    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();

    /* Pack the container in the window */
    gtk_container_add(GTK_CONTAINER (vbox), grid);

    button_create("Je n'ai pas de code", 0, 0, 1, 1, print_hello, NULL, 0);
    button_create("J'ai un code", 0, 1, 1, 1, print_hello, NULL, 0);
    button_create("Quitter", 0, 2, 1, 1, gtk_widget_destroy, window, 1);

    gtk_grid_set_row_spacing(GTK_GRID(grid), 50);
    gtk_grid_set_column_spacing (GTK_GRID(grid), 50);



    /* Now that we are done packing our widgets, we show them all
    * in one go, by calling gtk_widget_show_all() on the window.
    * This call recursively calls gtk_widget_show() on all widgets
    * that are contained in the window, directly or indirectly.
    */
    gtk_widget_show_all (window);

}

void window_create(GtkApplication *app){
    window = gtk_application_window_new (app);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); /** LA FENETRE SERA POSITIONNEEE AU MILIEU **/
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200); /** DIMENSION DE LA FENETRE **/
    gtk_window_set_title(GTK_WINDOW(window), "Code validation"); /** TITRE DE LA FENETRE **/
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
}

void button_create(char* name, int x, int y, int length, int width, GtkWidget *callback, gpointer data, int swapped){

    button = gtk_button_new_with_label(name);
    gtk_widget_set_hexpand (button, TRUE);
    if(swapped == 1){
        g_signal_connect_swapped(button, "clicked", G_CALLBACK (callback), data);
    }
    else{
        g_signal_connect(button, "clicked", G_CALLBACK (callback), data);
    }
    gtk_grid_attach (GTK_GRID (grid), button, x, y, length, width);
}
