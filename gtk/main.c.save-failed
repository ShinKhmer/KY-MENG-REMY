#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <gtk/gtk.h>
#include "lib/gtk_functions.c"


void createWinGTK(GtkWidget *);
void test();


// INIT GTK
GtkWidget *window;
GtkWidget *grid;
GtkWidget *vbox;
GtkWidget *notebook;
GtkWidget *vbox2;
GtkWidget *vbox3;
GtkWidget *vbox4;
GtkWidget *vbox5;
GtkWidget *array;
GtkWidget *button;
GtkWidget *pTabLabel;
GtkWidget *image;
gchar *sTabLabel;


int main(int argc, char **argv){

    // INITIALISATION BDD
    MYSQL *database = NULL;
    database = mysql_init(database);

    if(database == NULL){
        printf("\nProblem while initializing database !\n");
        return  0;
    }

    mysql_options(database, MYSQL_READ_DEFAULT_GROUP, "option");

    // CONNEXION BDD
    if(mysql_real_connect(database, "localhost", "root", "", "worknshare", 0, NULL, 0)){


        /** INITIALISATION DE GTK **/
        gtk_init(&argc, &argv);

        /** ETAPE 1 **/
        /** CREATION DE LA FENETRE **/
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        createWinGTK(window);


        /** ETAPE 2 **/
        vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); /** CREATION DU BOX ORIENTEE VERTICALEMENT (OU HORIZONTALEMENT SELON VOTRE CHOIX) **/
        gtk_container_add(GTK_CONTAINER(window), vbox); /** AJOUT DE LA VBOX DANS LA FENETRE **/

        /** ETAPE 3 **/
        notebook = gtk_notebook_new(); /** CREATION D'UN NOTEBOOK **/
        gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0); /** AJOUT DU NOTEBOOK DANS LA VBOX **/
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP); /** DEFINITION DE LA POSITION DES ONGLETS: top, bottom, left, right POSSIBLES **/

        gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);


        /** VOTRE CONTENU ICI **/
        grid = gtk_grid_new();

        button = gtk_button_new_with_label("Test");

        gtk_widget_set_hexpand (button, TRUE);
        gtk_widget_set_halign (button, GTK_ALIGN_FILL);
        gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);
        gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 100);

        /** JOUEZ AVEC LES POSITIONS DES LIGNES ET DES COLONNES POUR LE POSITIONNEMENT DE VOS BOUTONS **/

        //gtk_table_attach(GTK_TABLE(array), button, 0, 1, 0, 1, !GTK_EXPAND, !GTK_FILL, 0, 0);
        g_signal_connect(button,"clicked", G_CALLBACK (test), NULL);



        /** AFFICHAGE DE TOUS LES ELEMENTS DANS LA FENETRE **/
        gtk_widget_show_all(window);

        gtk_main();

    }else{
        printf("Problem while connecting to database !\n");
    }

    return 0;
}


void createWinGTK(GtkWidget *window){
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); /** LA FENETRE SERA POSITIONNEEE AU MILIEU **/
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400); /** DIMENSION DE LA FENETRE **/
    gtk_window_set_title(GTK_WINDOW(window), "Code validation"); /** TITRE DE LA FENETRE **/
}

void test(){
    printf("Hello, it's working !!\n");
}
