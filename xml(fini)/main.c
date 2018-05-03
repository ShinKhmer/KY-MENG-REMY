#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>
#include <libxml/xmlwriter.h>

#define MY_ENCODING "UTF-8"

void search_location(char *locationName, char *tempo);

void testXmlwriterFilename(const char *uri,int location,char* dateAsked);
xmlChar *ConvertInput(const char *in, const char *encoding);
xmlTextWriterPtr new_xmlTextWriter(const char *uri);
int new_xmlTextWriterStartDocument(xmlTextWriterPtr writer);
int new_xmlTextWriterStartElement(xmlTextWriterPtr writer, xmlChar *name);
int new_xmlTextWriterWriteFormatElement(xmlTextWriterPtr writer, xmlChar *name, char *customer, char *location);
int new_xmlTextWriterEndElement(xmlTextWriterPtr writer);
int new_xmlTextWriterEndDocument(xmlTextWriterPtr writer);

int main()
{
    char tempo[30];
    char dateAsked[]="070318";
    char nameFile[20];
    int location = 1;
    int i = 1;
    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM location");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while ((row = mysql_fetch_row(result)))
        {
            i++;
        }
        mysql_free_result(result);

    }
    else
    {
        return 98;
    }
    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT CURRENT_DATE");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while ((row = mysql_fetch_row(result)))
        {
            strcpy(tempo,row[0]);
        }
        mysql_free_result(result);
        mysql_close(mysql);
    }
    else
    {
        return 7;
    }

    sprintf(dateAsked,"%c%c%c%c%c%c",tempo[8],tempo[9],tempo[5],tempo[6],tempo[2],tempo[3]);


    for(;location<i;location++)
    {
        sprintf(nameFile,"%d_%s.xml",location,dateAsked);
        testXmlwriterFilename(nameFile,location,dateAsked);
        xmlCleanupParser();
        xmlMemoryDump();
    }
    printf("DONE SUCCESSFULLY ! \n");
    return 0;
}


void
testXmlwriterFilename(const char *uri,int location,char* dateAsked)
{
    int rc;

    xmlTextWriterPtr writer;
    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    char tempo[250] ;
    sprintf(tempo,"%d",location);
    char locationName[50]="";
    char customerName[100]="";
    char customerSurname[100]="";
    char hour[50]="";
    char now[10]="";

    /* Create a new XmlWriter for uri, with no compression. */
    writer = new_xmlTextWriter(uri);
    if(writer == NULL)
        return;


    /* Start the document with the xml default for the version,*/
    rc = new_xmlTextWriterStartDocument(writer);
    if(rc < 0)
        return;

    rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"MAIN");
    if(rc < 0)
        return;

    rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Lieu");
    if(rc < 0)
        return;


    search_location(locationName, tempo);

    rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"nom", "%s", locationName);
    if(rc < 0)
        return;

    rc = new_xmlTextWriterEndElement(writer);
    if(rc < 0)
        return;


    /* Start an element named "Info" as child of Lieu. */
    rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Info");
    if(rc < 0)
        return;

    /* Start an element named "HEADER" as child of ORDER. */
    rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Entree");
    if(rc < 0)
        return;

    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT date_entry,name_customer,last_name_customer FROM customers,history WHERE id_location = %d AND history.id_customer = customers.id_customer",location);
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while ((row = mysql_fetch_row(result)))
        {
            strcpy(customerName,row[1]);
            strcpy(customerSurname,row[2]);
            strcpy(hour,row[0]);
            sprintf(now,"%c%c%c%c%c%c",hour[8],hour[9],hour[5],hour[6],hour[2],hour[3]);
            sprintf(hour,"%c%c:%c%c",hour[11],hour[12],hour[14],hour[15]);
            if(strcmp(now,dateAsked)==0){
                rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Personne");
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"nom", "%s", customerName);
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"prenom", "%s", customerSurname);
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"heure", "%s", hour);
                if(rc < 0)
                    return;

                /* Close the element named personne. */
                rc = new_xmlTextWriterEndElement(writer);
                if(rc < 0)
                    return;
            }
        }
        mysql_free_result(result);
    }
    else
    {
        return;
    }
     /* Start an element named "ENTRY" as child of ENTRIES. */

    /* Close the element named Entree. */
    rc = new_xmlTextWriterEndElement(writer);
    if(rc < 0)
        return;

    /* Start an element named "HEADER" as child of ORDER. */
    rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Sorties");
    if(rc < 0)
        return;


 if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT date_exit,name_customer,last_name_customer FROM customers,history WHERE id_location = %d AND history.id_customer = customers.id_customer",location);
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);

        while ((row = mysql_fetch_row(result)))
        {
            strcpy(customerName,row[1]);
            strcpy(customerSurname,row[2]);
            strcpy(hour,row[0]);
            sprintf(now,"%c%c%c%c%c%c",hour[8],hour[9],hour[5],hour[6],hour[2],hour[3]);
            sprintf(hour,"%c%c:%c%c",hour[11],hour[12],hour[14],hour[15]);

            if(strcmp(now,dateAsked)==0){
                rc = new_xmlTextWriterStartElement(writer, (xmlChar *)"Personne");
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"nom", "%s", customerName);
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"prenom", "%s", customerSurname);
                if(rc < 0)
                    return;

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = new_xmlTextWriterWriteFormatElement(writer, (xmlChar *)"heure", "%s", hour);
                if(rc < 0)
                    return;

                /* Close the element named personne. */
                rc = new_xmlTextWriterEndElement(writer);
                if(rc < 0)
                    return;
            }
        }
        mysql_free_result(result);
    }
    else
    {
        return;
    }

    /* Close the element named personne. */
    rc = new_xmlTextWriterEndElement(writer);
    if(rc < 0)
        return;

    /* Close the element named Entree. */
    rc = new_xmlTextWriterEndElement(writer);
    if(rc < 0)
        return;



    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = new_xmlTextWriterEndDocument(writer);
    if(rc < 0)
        return

    xmlFreeTextWriter(writer);
}


xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}






void search_location(char *locationName, char *tempo){
    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM location");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while ((row = mysql_fetch_row(result)))
        {
            if(strcmp(tempo,row[0])==0)
            {strcpy(locationName,row[1]);
            break;}
        }
        mysql_free_result(result);
    }
    else
    {
        printf("Connection error with the database !\n");
    }
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
