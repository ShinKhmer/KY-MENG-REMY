#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock.h>
#include <MYSQL/mysql.h>
#include <winsock2.h>

void generate_code(char* code);
int check_code(char *code_string);
int log_in_out(int *id_location, char * code_customer);



int main(int argc, char ** argv)
{
    /** ==== PART CODE GENERATOR =====   **/
    /*
    char code[11]="";
    do{
            generate_code(code);
    }while(check_code(code));
    printf("client code : %s\n",code);
    */
    /** ==== PART INSERT DATABASE ===== **/





    printf("plop\n");
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
        mysql_close(mysql);
        return 0;
    }
    else {
            return 99;
    }
    return 99;
}

int log_in_out(int *id_location, char * code_customer){

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
        while ((row = mysql_fetch_row(result)))
        {
            if(strcmp(row[5],code_customer)== 0)
            {
                id_customer = row[0];
                inside = row[6];
            }
        }
        mysql_free_result(result);
        if(id_customer != 0 && inside = 0)
        {
            sprintf(request,"INSERT INTO `history`(`id_history`, `date_entry`, `date_exit`, `id_customer`, `id_location`) VALUES (NULL,[value-2],NULL,%d,%d)",id_customer,id_location);
            mysql_query(mysql,request);
            sprintf(request,"UPDATE `customer` SET inside = 1 WHERE id_customer = %d",id_customer);
            mysql_query(mysql,request);

        }
        else if(id_customer != 0 && inside = 1)
        {

            sprintf(request,"UPDATE `history` SET `date_exit` = [value] WHERE id_customer = %d AND date_exit = NULL",id_customer);
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


void sign_in(){
    MYSQL *mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
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
