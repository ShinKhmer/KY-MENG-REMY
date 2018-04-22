/*
 * QR Code generator demo (C)
 *
 * Run this command-line program with no arguments. The program
 * computes a demonstration QR Codes and print it to the console.
 *
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qrcodegen.h"



// Function prototypes
static void doBasicDemo(char **mee);
static void printQr(const uint8_t qrcode[]);

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height);

// The main application program.
int main(int argc,char** argv) {
printf("%s", argv[1]);
	doBasicDemo(argv[1]);

	return EXIT_SUCCESS;
}



/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(char **mee) {
	const char *text = mee;  // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode);
}

/*---- Utilities ----*/

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[]) {
	int size = qrcodegen_getSize(qrcode);
	printf("%d\n",size);

    int p=-1;
    int l=0;
    int color=0;
     unsigned x2, y2;
	const char* filename = "test2.png";
  unsigned width = 168, height = 168;
  unsigned char* image = malloc(width * height * 4);


	unsigned char c = 254;
	bool plop;
	for (int y = 0; y < size ; y++) {
		for (int x = 0; x < size ; x++) {
			//fputs((qrcodegen_getModule(qrcode, x, y) ? "\17\17" : "  "), stdout);
                plop =qrcodegen_getModule(qrcode, x, y);
			if(plop)
                {
                    p++;
                    color=1;
                    printf("%c%c",c,c);
                }
                else{
                    p++;
                    color=0;
                    printf("  ");
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
		fputs("\n", stdout);
	}

  encodeOneStep(filename, image, width, height);

  free(image);

	fputs("\n", stdout);
}


/*----- ZONE TEST-------*/

void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

