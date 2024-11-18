
/*
Name : Shashi kumar S
Date : 23-05-2024
Description : LSB image stegnograpy
 */

/* Including all required header files  */

#include<stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

/* Main function starts  */

int main()
{
    /* variables declaration and initialization */
    char opt;
    int i=1;

    while(i)
    {
	/* User promt to choose encoding or decoding option */
	printf("'e' - Encode.\n'd' - Decode.\nEnter the option:- ");
	scanf(" %c",&opt);
	printf("\n");

	/* Run switch case based on user option */
	switch(opt)
	{
	    case 'e': {
			  /* User chosen encoding  */
			  printf("\t Encoding is started:....\n\n");
			  EncodeInfo encInfo;

			  /* Initialize sourse,destination and secret file names */
			  encInfo.src_image_fname="beautiful.bmp";
			  encInfo.stego_image_fname="output.bmp";
			  encInfo.secret_fname="secret.txt";
			  
			  /* Calling do encoding function */
			  do_encoding(&encInfo);
			  break;
		      }

	    case 'd': {
			  /* User chosen docoding  */
			  printf("\t Decoding is started:.....\n\n");
			  DecodeInfo decInfo;

			  /* Initialize stegged image name   */
			  decInfo.output_image_name="output.bmp";

			  /* Calling do decoding function  */
			  do_decoding(&decInfo);
			  break;
		      }

	    default:  /* Printing promt error fi user entered other then encoding or decoding option */
		      printf("Please, Enter valide option ['e' - endode / 'd' - decode ]\n");
		      break;
	}
	/* Ask user if he wants to continue  */
	printf("\nYou still want to continue [1/0] : ");
	scanf("%d",&i);
    }
    return 0;
} 

