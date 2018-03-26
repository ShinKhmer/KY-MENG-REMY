#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>


#define MY_ENCODING "UTF-8"

static void processNode(xmlTextReaderPtr reader, int *i, int *k);
static void streamFile(const char *filename);
void get_date(char* output);
void transfo_date(char* date);

int main()
{
    char date[10]="";
    char dateDay[3]="";
    char dateMon[3]="";
    char dateYea[3]="";
    transfo_date(date);
    sprintf(dateDay,"%c%c",date[0],date[1]);
    sprintf(dateMon,"%c%c",date[2],date[3]);
    sprintf(dateYea,"%c%c",date[4],date[5]);
    printf("%s\n",date);

    streamFile("1_020318.xml");
    xmlCleanupParser();
    xmlMemoryDump();
    return 0;
}

static void processNode(xmlTextReaderPtr reader, int * i,int * k)
 {

    const xmlChar *name, *value;
    char ***tab = NULL;
    char ***tab_temp = NULL;
    int i = 0;
    int j = 0;
    int k = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    int line = 0;
    int column = 3;




    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);

    if(xmlTextReaderDepth() == 2)
        i++;

    if (i==3) { //début d'entrée

        if(xmlTextReaderDepth(reader) == 4)
        {


            if(k==0) // nom
            {
                /**ALLOCATION D'UNE LIGNE DE PLUS**/
                // line found
                line++;

                if(line > 1){
                    /** Malloc tab_temp **/
                    tab_temp = malloc(sizeof(char **) * (line - 1));

                    // Malloc 2 : column
                    for(x=0; x<line; x++){
                        tab_temp[x] = malloc(sizeof(char *) * column);
                        // Malloc 3 : characters
                        for(y=0; y<50; y++){    // length characters = 50
                            tab_temp[x][y] = malloc(sizeof(char) * 50);
                        }
                    }

                    /** Copy tab in tab_temp **/
                    for(x=0; x<line; x++){
                        for(y=0; y<column; y++){
                            for(z=0; z<50; z++){
                                tab_temp[x][y][z] = tab[x][y][z];
                            }
                        }
                    }

                    /** Free tab **/
                    for(x=0; x<line; x++){
                        for(y=0; y<column; y++){
                            free tab[x][y];
                        }
                        free(tab[x]);
                    }
                }

                /** Malloc tab **/
                // Malloc 1 : line
                tab = malloc(sizeof(char **) * line);

                // Malloc 2 : column
                for(x=0; x<line; x++){
                    tab[x] = malloc(sizeof(char *) * column);
                    // Malloc 3 : characters
                    for(y=0; y<50; y++){    // length characters = 50
                        tab[x][y] = malloc(sizeof(char) * 50);
                    }
                }

                /** Paste tab_temp to tab **/
                if(tab > 1){
                    for(x=0; x<line; x++){
                        for(y=0; y<column; y++){
                            for(z=0; z<50; z++){
                                tab[x][y][z] = tab_temp[x][y][z];
                            }
                        }
                    }
                }


                /** Free tab_temp **/
                for(x=0; x<line; x++){
                    for(y=0; y<column; y++){
                        free tab[x][y];
                    }
                    free(tab[x]);
                }

                sprintf(tab[line][k],"%s",value);
                k++;
            }
            if(k==1) // prenom
            {
                sprintf(tab[line][k],"%s",value);
                k++;
            }
            if(k==2) // heure
            {
                sprintf(tab[line][k],"%s",value);
                k=0;
            }
        }
    }


    if (i==5) { //début de sortie
        if(xmlTextReaderDepth(reader) == 4)
        { /**ALLOCATION D'UNE LIGNE DE PLUS**/

            if(k==0) // nom
            {
                sprintf(tab[k][],"%s",value);
                k++;
            }
            if(k==1) // prenom
            {
                sprintf(tab[k][],"%s",value);
                k++
            }
            if(k==2) // heure
            {
                sprintf(tab[k][],"%s",value);
                k=0;
            }
        }
    }
 }

static void streamFile(const char *filename) {
    xmlTextReaderPtr reader;
    int ret;

    int i = 0;
    int k = 0;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader,i,k);
            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
    }
}


void get_date(char* output)
{

    time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  sprintf(output, "[%d/%d/%d]",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year -100);
    return;
}


void transfo_date(char* date)
{
    char tempo[20] = "";
    int i = 1;
    int k = 0;
    int n=0;
    get_date(tempo);
    for(;n<strlen(tempo);n++)
    {
        if(tempo[i+1]=='/')
        {
            date[k]='0';
            date[k+1]=tempo[i];
            k+=2;
            i+=2;
        }
        else if(tempo[i]=='[' || tempo[i]==']')
        {
            i++;
        }
        else
        {
            date[k]=tempo[i];
            i++;
            k++;
        }
    }
return;
}



