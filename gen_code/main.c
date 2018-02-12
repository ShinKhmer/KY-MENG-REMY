#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>

void generate_code(char* code);
int check_code(char *code_string);


int main(int argc, char ** argv)
{

    char code[11]="";
    do{
            generate_code(code);
    }while(check_code(code));
    printf("client code : %s\n",code);

    return 0;
}


void generate_code(char* code){

    char salt[62]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char result[11]="";
    int letter = 0;
    int i=0;
    time_t t;

    srand(time(&t));

    for(i=0;i<10;i++){
        letter = rand()%62;
        result[i]=salt[letter];
    }

    strcpy(code,result);
    return;
}


int check_code(char *code_string){

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
        while ((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[5],code_string)== 0)
            {
                return 99;
            }
        }
        mysql_free_result(result);
        return 0;
    }
    else {
            return 99;
    }
    return 99;
}

