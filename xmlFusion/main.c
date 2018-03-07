#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>


#define MY_ENCODING "UTF-8"

static void processNode(xmlTextReaderPtr reader);
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

static void processNode(xmlTextReaderPtr reader)
 {
    const xmlChar *name, *value;

    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);

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
    }
}

static void streamFile(const char *filename) {
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForFile(filename, NULL, 0);
    if (reader != NULL) {
        ret = xmlTextReaderRead(reader);
        while (ret == 1) {
            processNode(reader);
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
