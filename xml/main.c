#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>
#include <libxml/xmlwriter.h>


//#if defined(LIBXML_WRITER_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

#define MY_ENCODING "UTF-8"

void testXmlwriterFilename(const char *uri,int location,char* dateAsked);
xmlChar *ConvertInput(const char *in, const char *encoding);

int main()
{
/*   char dateAsked[]="190218";
    char nameFile[20];
    int location = 1;
    sprintf(nameFile,"%d_%s.xml",location,dateAsked);
    testXmlwriterFilename(nameFile,location,dateAsked);
*/



      xmlCleanupParser();
         xmlMemoryDump();
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
    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,*/
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "MAIN");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Lieu");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

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
        return;
    }
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "nom", "%s",
                                         locationName);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "Info" as child of Lieu. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Info");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Start an element named "HEADER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Entree");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

 if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT date_entry,name_customer,surname_customer FROM customer,history WHERE id_location = %d AND history.id_customer = customer.id_customer",location);
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
                rc = xmlTextWriterStartElement(writer, BAD_CAST "personne");
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
                    return;
                }

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "nom", "%s",
                                                     customerName);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }
                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "prenom", "%s",
                                                     customerSurname);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }
                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "heure", "%s",
                                                     hour);
                if (rc < 0) {
                    printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }

                /* Close the element named personne. */
                rc = xmlTextWriterEndElement(writer);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
                    return;
                }
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
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }
    /* Start an element named "HEADER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "Sorties");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }


 if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT date_exit,name_customer,surname_customer FROM customer,history WHERE id_location = %d AND history.id_customer = customer.id_customer",location);
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
                rc = xmlTextWriterStartElement(writer, BAD_CAST "personne");
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
                    return;
                }

                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "nom", "%s",
                                                     customerName);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }
                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "prenom", "%s",
                                                     customerSurname);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }
                /* Add an attribute with name "version" and value "1.0" to ORDER. */
                rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "heure", "%s",
                                                     hour);
                if (rc < 0) {
                    printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
                    return;
                }

                /* Close the element named personne. */
                rc = xmlTextWriterEndElement(writer);
                if (rc < 0) {
                    printf
                        ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
                    return;
                }
            }
        }
        mysql_free_result(result);
    }
    else
    {
        return;
    }

    /* Close the element named personne. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Close the element named Entree. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }



    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }

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
