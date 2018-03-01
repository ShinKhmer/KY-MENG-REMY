#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>
#include <gtk/gtk.h>

typedef struct code_generate{
    int id_customer;
    char code_customer[11];
    struct code_generate *next;
}code_generate;

void generate_code(GtkWidget *button, gpointer data);
void check_code(GtkWidget *button, gpointer data);
int log_in_out(int id_location, const gchar * code_customer);
void activate(GtkApplication *, gpointer);
void quick_message (GtkWindow *, gchar *);
void window_create(GtkApplication *);
void button_create(char*, int, int, int, int, GtkWidget *, gpointer, int);
void text_entry_create(int, int, int, int, int);
code_generate *profile_new(code_generate *);
code_generate *profile_free(code_generate *);


GtkWidget *window;
GtkWidget *dropdownList;
GtkWidget *notebook;
GtkWidget *vbox;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *label;
GtkWidget *entry;
GtkWidget *pTabLabel;
gchar *sTabLabel;


int main(int argc, char ** argv)
{
    code_generate *start = NULL;

    start = profile_new(start);

    /** ==== GTK ===== **/
    GtkApplication *app;

    app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(activate), start);
    g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);     // Object destruction

    printf("A bientot !");

    // Free struct !

    return 0;
}




/* --- GTK --- */
void activate(GtkApplication *app, gpointer user_data){

    gint id_location;
    int i = 0;

    /* Create a window */
    window_create(app);

    /* Dialog box */
    quick_message(GTK_WINDOW(window), "Veuillez choisir le lieu :");

    /* Create a notebook */
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);

    gtk_container_add(GTK_CONTAINER(window), notebook);

    vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    sTabLabel = g_strdup_printf("Code");
    pTabLabel = gtk_label_new(sTabLabel);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, pTabLabel);


    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing (GTK_GRID(grid), 30);

    gtk_grid_attach(GTK_GRID(grid), dropdownList, 0, 0, 1, 1);

    /* Pack the container in the window */
    gtk_container_add(GTK_CONTAINER (vbox), grid);

    /* Label creation */
    label = gtk_label_new("Veuillez insérer votre code client (10 caractères)");
    gtk_widget_set_hexpand (label, TRUE);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);

    /* Text Entry creation */
    text_entry_create(10, 0, 2, 1, 1);

    /* Buttons creation */
    //button_create("Valider", 0, 3, 1, 1, check_code, vbox, 0);

    /* No code label */
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span color=\"blue\">Pas de code : Veuillez vous inscrire sur le site</span>");
    gtk_widget_set_hexpand (label, TRUE);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 4, 1, 1);

    /* Exit button */
    button_create("Quitter", 0, 6, 1, 1, exit, window, 1);




    /* RECUPERATION DU TEXTE
    Fonction récupération du texte => copy_code
    Paramètres : gchar
    Fonction appellant check_code
    */

    gtk_widget_show_all (window);

}

void quick_message (GtkWindow *parent, gchar *message)
{
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new();
    /* Dropdown list => DO FUNCTION ! */
    dropdownList = gtk_combo_box_text_new();
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 0, "Bastille" );
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 1, "République" );
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 2, "Odéon" );
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 3, "Place d'Italie" );
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 4, "Ternes" );
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX(dropdownList), 5, "Beaubourg" );
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropdownList), 0);

    /*content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new (message);*/

    // Ensure that the dialog box is destroyed when the user responds
    g_signal_connect_swapped (dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

    gtk_widget_show_all (dialog);
}

void window_create(GtkApplication *app){
    window = gtk_application_window_new (app);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); /** LA FENETRE SERA POSITIONNEEE AU MILIEU **/
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200); /** DIMENSION DE LA FENETRE **/
    gtk_window_set_title(GTK_WINDOW(window), "Code validation"); /** TITRE DE LA FENETRE **/
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
}

void notebook_tab_change(GtkWidget *Notebook, gpointer data){
    gtk_notebook_set_current_page(data, 2);
}

void button_create(char* name, int x, int y, int length, int width, GtkWidget *callback, gpointer data, int swapped){
    button = gtk_button_new_with_label(name);
    gtk_widget_set_hexpand (button, TRUE);
    gtk_grid_attach (GTK_GRID (grid), button, x, y, length, width);

    if(swapped == 1){
        g_signal_connect_swapped(button, "clicked", G_CALLBACK(callback), data);
    }
    else{
        g_signal_connect(button, "clicked", G_CALLBACK(callback), data);
    }
}


void text_entry_create(int length_text, int x, int y, int length, int width){
    entry = gtk_entry_new();
    gtk_entry_set_max_length (GTK_ENTRY (entry), length_text);
    gtk_grid_attach (GTK_GRID (grid), entry, x, y, length, width);
}


code_generate *profile_new(code_generate *start){
    code_generate *profile;

    profile->id_customer = 0;
    profile->code_customer[0] = '\0';
    profile->next = start;

    return profile;
}

code_generate *profile_free(code_generate *start){
    code_generate *del = start;
    if(del != NULL){
        start = start->next;
        free(del);
    }

    return start;
}
