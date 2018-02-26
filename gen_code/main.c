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
int log_in_out(int id_location, char * code_customer);
void activate(GtkApplication *, gpointer);
void window_create(GtkApplication *);
void button_create(char*, int, int, int, int, GtkWidget *, gpointer, int);
void text_entry_create(int, int, int, int, int);
void gtk_exit();
code_generate *profile_new(code_generate *);
code_generate *profile_free(code_generate *);


GtkWidget *window;
GtkWidget *notebook;
GtkWidget *vbox;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *label;
GtkWidget *entry;
GtkWidget *pTabLabel;
gchar *sTabLabel;
const gchar *uri;




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

    return 0;
}

void check_code(GtkWidget *button, gpointer data){

    const gchar *sText;

    sText = gtk_entry_get_text(GTK_ENTRY(entry));

    printf("Code a verifier : %s\n", sText);

    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];

    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM customer");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[5],sText) == 0)
            {
                printf("Code valide");
                log_in_out(1, sText);
            }else{
                printf("Code non valide\n");
            }
            printf("Code(s) existant(s) dans la BDD : %s\n", row[5]);
        }
        mysql_free_result(result);
        mysql_close(mysql);
    }
    else
    {
        printf("Erreur lors de la connexion !");
    }
}

int log_in_out(int id_location, char * code_customer){

    int id_customer = 0;
    int inside;

    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];

    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM customer");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[5],code_customer)== 0)
            {
                id_customer = atoi(row[0]);
                inside = atoi(row[6]);
            }
        }
        mysql_free_result(result);
        if(id_customer != 0 && inside == 0)
        {
            sprintf(request,"INSERT INTO `history`(`id_history`, `date_entry`, `date_exit`, `id_customer`, `id_location`) VALUES (NULL,CURRENT_TIMESTAMP,NULL,%d,%d)",id_customer,id_location);
            mysql_query(mysql,request);
            sprintf(request,"UPDATE `customer` SET inside = 1 WHERE id_customer = %d",id_customer);
            mysql_query(mysql,request);

        }
        else if(id_customer != 0 && inside == 1)
        {

            sprintf(request,"UPDATE `history` SET `date_exit` = CURRENT_TIMESTAMP WHERE id_customer = %d AND date_exit IS NULL",id_customer);
            mysql_query(mysql,request);
            sprintf(request,"UPDATE `customer` SET inside = 0 WHERE id_customer = %d",id_customer);
            mysql_query(mysql,request);
        }
        else
        {
            mysql_close(mysql);
            return 99;
        }
        mysql_close(mysql);
    }
    else {
            return 99;
    }
    return 99;
}

/*
int sign_in(){
    MYSQL *mysql;
    char request[150];
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"INSERT INTO `customer`( `name_customer`, `surname_customer`, `phone_number`, `email`, `code_customer`, `inside`, `id_subscription`) VALUES ([value-2],[value-3],[value-4],[value-5],[value-6],0,0)");
        mysql_query(mysql,request);
    }
    else {
            return 99;
    }
    return 99;
}
*/


/* --- GTK --- */
void activate(GtkApplication *app, gpointer user_data){

    // Create a window
    window_create(app);

    // Create a notebook
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

    /* Pack the container in the window */
    gtk_container_add(GTK_CONTAINER (vbox), grid);

    /* Label creation */
    label = gtk_label_new("Veuillez insérer votre code client (10 caractères)");
    gtk_widget_set_hexpand (label, TRUE);
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);

    /* Text Entry creation */
    text_entry_create(10, 0, 2, 1, 1);

    /* Buttons creation */
    button_create("Valider", 0, 3, 1, 1, check_code, vbox, 0);

    /* URL - DON'T WORKING :/
    button = gtk_link_button_new("Google");
    gtk_link_button_set_uri(GTK_LINK_BUTTON(button), "http://google.com/");
    gtk_grid_attach (GTK_GRID (grid), button, 0, 4, 1, 1);
    */

    /* No code label */
    label = gtk_label_new(NULL);
    gtk_label_set_markup(label, "<span color=\"blue\">Pas de code : Veuillez vous inscrire sur le site</span>");
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
