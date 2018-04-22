#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/xmlwriter.h>


typedef struct customerData{
    char place[20];
    char name[20];
    char surname[20];
    char hour[10];
}customerData;


#define MY_ENCODING "UTF-8"

int streamFile(const char *filename,char *directoryPath,char *finalPath,customerData *data);
static void processNode(xmlTextReaderPtr reader,customerData *data,int* k,int* m,char *directoryPath,char *finalPath);


xmlTextWriterPtr new_xmlTextWriter(const char *uri);
int new_xmlTextWriterStartDocument(xmlTextWriterPtr writer);
int new_xmlTextWriterStartElement(xmlTextWriterPtr writer, xmlChar *name);
int new_xmlTextWriterWriteFormatElement(xmlTextWriterPtr writer, xmlChar *name, char *customer, char *location);
int new_xmlTextWriterEndElement(xmlTextWriterPtr writer);
int new_xmlTextWriterEndDocument(xmlTextWriterPtr writer);
int readXMLFile(const char *fileName);
void xmlWriterFilename(const char *uri, customerData *data);
xmlDocPtr parseDoc(char *fileName, customerData *data);

int main()
{
    DIR* rep = NULL;
    struct dirent* file = NULL;
    customerData *data = NULL;

    data = malloc(sizeof(customerData));

    char initialPath[150] = "D:\\Dossier de stephane\\cours\\2i\\PROJET_ANNUEL\\dirent\\gen_xml";

    char finalPath[150]="";
    char filename[150]="";
    char dateDay[3]="";
    char dateMon[3]="";
    char dateYea[3]="";
    char directoryPath[20]="";



    rep = opendir(initialPath);
    if(rep==NULL)
        return 1 ;

    file = readdir(rep);
    file = readdir(rep);

    /*start to file directly*/
    while ((file = readdir(rep)) != NULL){
//    printf("Le fichier lu s'appelle '%s'\n", file->d_name);
    if(strlen(file->d_name)== 12)
        /*File ok*/
        sprintf(filename,"%s\\%s",initialPath,file->d_name);
        sprintf(dateDay,"%c%c",file->d_name[2],file->d_name[3]);
        sprintf(dateMon,"%c%c",file->d_name[4],file->d_name[5]);
        sprintf(dateYea,"%c%c",file->d_name[6],file->d_name[7]);
        sprintf(directoryPath,"%s%s",dateMon,dateYea);


//        printf("FICHIER : %s\n",filename);
//        printf("jour : %s \n mois : %s\n annee : %s \n",dateDay,dateMon,dateYea);

        sprintf(finalPath,"%s%s\\%s.xml",dateMon,dateYea,dateDay);

        printf("FICHIER : %s\n",finalPath);

        /**get informations**/

        streamFile(filename,directoryPath,finalPath,data);
          xmlCleanupParser();
          xmlMemoryDump();



    }

    free(data);

    if(closedir(rep)==-1)
        return -1;

    return 0;
}




xmlTextWriterPtr new_xmlTextWriter(const char *uri){
    xmlTextWriterPtr writer;

    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
    }

    return writer;
}

int new_xmlTextWriterStartDocument(xmlTextWriterPtr writer){
    int rc;

    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if(rc < 0)
        printf("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");

    return rc;
}

int new_xmlTextWriterStartElement(xmlTextWriterPtr writer, xmlChar *name){
    int rc;

    rc = xmlTextWriterStartElement(writer, BAD_CAST name);
    if(rc < 0)
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");

    return rc;
}

int new_xmlTextWriterWriteFormatElement(xmlTextWriterPtr writer, xmlChar *name, char *customer, char *location){
    int rc;

    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST name, customer, location);
    if(rc < 0)
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");

    return rc;
}

int new_xmlTextWriterEndElement(xmlTextWriterPtr writer){
    int rc;

    rc = xmlTextWriterEndElement(writer);
    if(rc < 0)
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");

    return rc;
}

int new_xmlTextWriterEndDocument(xmlTextWriterPtr writer){
    int rc;

    rc = xmlTextWriterEndDocument(writer);
    if(rc < 0)
        printf("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");

    return rc;
}


int readXMLFile(const char *fileName){

    int     error;
    xmlTextReaderPtr reader;
    reader = xmlReaderForFile(fileName, NULL, 0);
    if(reader == NULL){
        error = 1;
        printf("\nLe fichier n'existe pas!\n");
    }else{
        error = 0;
        printf("\nLe fichier existe!\n");
    }
    return error;

}

void xmlWriterFilename(const char *uri, customerData *data){

    xmlTextWriterPtr    writer;

    writer = xmlNewTextWriterFilename(uri, 0);
    if(writer == NULL) {
        printf("xmlWriterFilename: Error creating the xml writer\n");
        return;
    }

        xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
         new_xmlTextWriterStartElement(writer, BAD_CAST "MAIN");
            new_xmlTextWriterStartElement(writer, BAD_CAST "Info");
                xmlTextWriterStartElement(writer, BAD_CAST "Entree");
                    new_xmlTextWriterStartElement(writer, (xmlChar *)"Personne");
                        new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"nom", "%s", data->name);
                        new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"prenom", "%s", data->surname);
                        new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"heure", "%s", data->hour);
                        new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"lieu", "%s", data->place);
                    new_xmlTextWriterEndElement(writer);
                xmlTextWriterEndElement(writer);
            xmlTextWriterEndElement(writer);
        xmlTextWriterEndElement(writer);

    xmlFreeTextWriter(writer);

}


xmlDocPtr parseDoc(char *fileName, customerData *data) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(fileName);
    if(doc == NULL) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return (NULL);
    }
    cur = xmlDocGetRootElement(doc);
    if(cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return (NULL);
    }
//    printf("\ncurrentName = %s", cur->name);
    if(strcmp((char*)cur->name, "MAIN") != 0) {
        fprintf(stderr,"document of the wrong type, root node != MAIN \n");
        xmlFreeDoc(doc);
        return (NULL);
    }else{
        cur=cur->last;
    }
    if(strcmp((char*)cur->name, "Info") != 0) {
        fprintf(stderr,"document of the wrong type, root node != Info \n");
        xmlFreeDoc(doc);
        return (NULL);
    }else{
        cur=cur->last;
    }
    if(strcmp((char*)cur->name, "Entree") != 0) {
        fprintf(stderr,"document of the wrong type, root node != Entree \n");
        xmlFreeDoc(doc);
        return (NULL);
    }else{
        printf("\nJe parse le fichier!\n");
        xmlNewTextChild(cur, NULL, (const xmlChar*)"Personne", NULL);
    }
    cur = cur->last;
    if(strcmp((char*)cur->name, "Personne") != 0) {
        fprintf(stderr,"document of the wrong type, root node != Personne \n");
        xmlFreeDoc(doc);
        return (NULL);
    }else{

        printf("\nJe parse le fichier!\n");
        xmlNewTextChild(cur, NULL, (const xmlChar*)"nom", (const xmlChar*)data->name);
        xmlNewTextChild(cur, NULL, (const xmlChar*)"prenom", (const xmlChar*)data->surname);
        xmlNewTextChild(cur, NULL, (const xmlChar*)"heure", (const xmlChar*)data->hour);
        xmlNewTextChild(cur, NULL, (const xmlChar*)"lieu", (const xmlChar*)data->place);
    }
    cur = cur->last;

    if(doc != NULL){

        xmlSaveFormatFile(fileName, doc, 0); //N'ECRASE PAS LE FICHIER LORS DE LA MODIFICATION
        xmlFreeDoc(doc);
    }

    return(doc);
}




int streamFile(const char *filename,char *directoryPath,char *finalPath,customerData *data) {
    xmlTextReaderPtr reader;
    int ret;

    int k=0;
    int m=0;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader,data,&k,&m, directoryPath, finalPath);

            ret = xmlTextReaderRead(reader);
        }
        xmlFreeTextReader(reader);
        if (ret != 0) {
            fprintf(stderr, "%s : failed to parse\n", filename);
            return 99;
        }
    } else {
        fprintf(stderr, "Unable to open %s\n", filename);
        return 99;
    }
    return 0;
}

static void processNode(xmlTextReaderPtr reader,customerData *data,int* k,int* m,char *directoryPath,char *finalPath) {
    const xmlChar *name, *value;
    xmlDocPtr doc = NULL;
    int error;

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);

    if (xmlTextReaderDepth(reader)==2)
        {(*k)++;
        printf("plplplpppll\n");
        }

    if((*k)==1 && xmlTextReaderDepth(reader)==3)
        sprintf(data->place,"%s",value);
        printf("depth : %d\n",xmlTextReaderDepth(reader));
        printf("k : %d\n",*k);
    if((*k)==3 && xmlTextReaderDepth(reader)==5 && (*m)==0)
        {printf("plop\n");
            strcpy(data->name,(const char*)value);
            (*m)++;
            return;
        }
        if((*k)==3 && xmlTextReaderDepth(reader)==5 && (*m)==1)
        {
            strcpy(data->surname,(const char*)value);
            (*m)++;
            return ;
        }
    if((*k)==3 && xmlTextReaderDepth(reader)==5 && (*m)==2)
        {
            strcpy(data->hour,(const char*)value);
            (*m)=0;

            printf("data name: %s\n",data->name);
            printf("data surname: %s\n",data->surname);
            printf("data hour: %s\n",data->hour);
            printf("data place: %s\n",data->place);
                    /**check existence of the document**/

        error = readXMLFile(finalPath);

        if(error == 0){ //SI LE FICHIER EXISTE DEJA, REECRITURE DU FICHIER
            doc = parseDoc(finalPath,data);

        }else{ //SINON CREATION DU NOUVEAU FICHIER
        //input informations

            CreateDirectory(directoryPath,NULL);
            xmlWriterFilename(finalPath, data);
            doc = parseDoc(finalPath,data);
        }

            return;
        }

    /*
    printf("%d %d %s %d %d",
	    xmlTextReaderDepth(reader),
	    xmlTextReaderNodeType(reader),
	    name,
	    xmlTextReaderIsEmptyElement(reader),
	    xmlTextReaderHasValue(reader));
    if (value == NULL)
	printf("\n");
    else {
        if (xmlStrlen(value) > 40)
            printf(" %.40s...\n", value);
        else
	    printf(" %s\n", value);
    */

    }

