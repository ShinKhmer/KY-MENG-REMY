#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>


#define PLACE 1//Bastille


int check(char** code);
int log_in_out(int id_location,char* code_customer);

int main(int argc,char**argv)
{
    int value;

    value = check(argv[1]);
    return value;
}


int check(char** code){

    int flag=0;
    int callback = 99;
    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");
    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM customers");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[8],code) == 0)
            {
                /* Pop up */
                //printf("Code valide\n");
                callback = log_in_out(PLACE, code);
                flag = 1;
            }else{
                printf("Code non valide\n");
            }
            //printf("Code(s) existant(s) dans la BDD : %s\n", row[8]);
        }
        mysql_free_result(result);
        mysql_close(mysql);
    }
    else
    {
        printf("Erreur lors de la connexion !");
        return 99;
    }
    if(flag){
        return callback;
    }
    else
        return 20;
}


int log_in_out(int id_location,char* code_customer){

    int id_customer = 0;
    int inside = 0;

    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    char request[150];

    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if(mysql_real_connect(mysql, "127.0.0.1", "root","", "worknshare", 0, NULL, 0))
    {
        sprintf(request,"SELECT * FROM customers");
        mysql_query(mysql,request);
        result = mysql_use_result(mysql);
        while((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[8],code_customer)== 0)
            {
                id_customer = atoi(row[0]);
                inside = atoi(row[10]);
                //printf("%d",id_customer);
            }
        }
        mysql_free_result(result);
        if(id_customer != 0 && inside == 0)
        {
            sprintf(request,"INSERT INTO `history`(`id_history`, `date_entry`, `date_exit`, `id_customer`, `id_location`) VALUES (NULL,CURRENT_TIMESTAMP,NULL,%d,%d)",id_customer,id_location);
            mysql_query(mysql,request);
            sprintf(request,"UPDATE `customers` SET inside = 1 WHERE id_customer = %d",id_customer);
            mysql_query(mysql,request);
            mysql_close(mysql);
            return 1;
        }
        else if(id_customer != 0 && inside == 1)
        {

            sprintf(request,"UPDATE `history` SET `date_exit` = CURRENT_TIMESTAMP WHERE id_customer = %d AND date_exit IS NULL",id_customer);
            mysql_query(mysql,request);
            sprintf(request,"UPDATE `customers` SET inside = 0 WHERE id_customer = %d",id_customer);
            mysql_query(mysql,request);
            mysql_close(mysql);
            return 2;
        }
        else
        {
            mysql_close(mysql);
            return 90;
        }
    }
    else {
            return 100;
    }
    return 666;
}
