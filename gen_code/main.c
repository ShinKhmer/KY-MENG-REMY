#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generate_code(char* code);



int main(int argc, char ** argv)
{
    char code[11]="";
    generate_code(code);
    printf("%s",code);

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


int check_code(char code_string){


}

