#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qrcodegen.h"


static void doBasicDemo(char **str_code,char** name_of_file);
static void printQr(const uint8_t qrcode[],char* name_of_file);

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height);


int main(int argc,char** argv) {

FILE *file = NULL;
char name[100];
if(argc == 3)
{
    doBasicDemo(argv[1],argv[2]);
    sprintf(name,"%s.png",argv[2]);
    file = fopen(name,"rb");
    if(file != NULL){
        fclose(file);
        return 0;//qr created successfully
    }

    return 98;//file doesn't exist/not created
}
else if(argc <= 2)
    return 100;//random error/unknown situation
else if(argc > 3)
    return 101;//random error/unknown situation

return 99;

}



/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(char **str_code,char** name_of_file) {

	const char *text = str_code;  // User-supplied text

	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode,name_of_file);
}

/*---- Utilities ----*/

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[], char* name_of_file) {
	int size = qrcodegen_getSize(qrcode);

    int p=-1;
    int l=0;
    int color=1;
     unsigned x2, y2;
	char *filename = NULL;
    filename = malloc(sizeof(char)*strlen(name_of_file)+5);
    sprintf(filename,"%s.png",name_of_file);

  unsigned width = 168, height = 168;
  unsigned char* image = malloc(width * height * 4);


	unsigned char c = 254;
	bool plop;
	for (int y = 0; y < size ; y++) {
		for (int x = 0; x < size ; x++) {
                plop =qrcodegen_getModule(qrcode, x, y);
			if(plop)
                {
                    p++;
                    color=0;
                }
                else{
                    p++;
                    color=1;

                }

            if(p>=21){
                p=0;
                l++;
            }
            for(y2 = (l*7); y2 < (l*7)+7; y2++)
            {
                for(x2 = (p*7); x2 < (p*7)+7; x2++)
                {
                     if(color){
                    image[4 * width * y2 + 4 * x2 + 0] = 255;//R
                    image[4 * width * y2 + 4 * x2 + 1] = 255;//G
                    image[4 * width * y2 + 4 * x2 + 2] = 255;//B
                    image[4 * width * y2 + 4 * x2 + 3] = 255;//A

                    }
                    else{
                    image[4 * width * y2 + 4 * x2 + 0] = 0;//R
                    image[4 * width * y2 + 4 * x2 + 1] = 0;//G
                    image[4 * width * y2 + 4 * x2 + 2] = 0;//B
                    image[4 * width * y2 + 4 * x2 + 3] = 255;//A
                    }
                }
            }
		}
	}
  encodeOneStep(filename, image, width, height);
  free(filename);
  free(image);

	//fputs("\n", stdout);
}


/*----- ZONE TEST-------*/

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}
