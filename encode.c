#include <stdio.h>
#include "encode.h" // Contains encoding files
#include "types.h"  // Contains types
#include<string.h>
#include "common.h" // Contains Magic string info

Status do_encoding(EncodeInfo *encInfo)
{

    /* Open files */
    if(open_files(encInfo) == e_success)
    {
	printf("\tINFO:Files are opened successfully :) \n\n");
    }
    else
    {
	printf("\tError:Files are not opend successfully :( \n");
	return e_failure;
    }

    /* Check sourse image capacity */
    if(!check_capacity(encInfo))
    {
	printf("\tINFO:Capacity checked successfully.\n\n");
    }
    else
    {
	printf("\tError: There is no space to encode.\n");
	return e_failure;
    }

    /* Copy bmp header */
    if(!copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image))
    {
	printf("\tINFO:BMP header copied successfully.\n\n");
    }
    else
    {
	printf("\tError: BMP header unable to copy.\n");
	return e_failure;
    }

    /* Encode magic string to stego imgae */
    if(!encode_magic_string(MAGIC_STRING,encInfo->fptr_src_image,encInfo->fptr_stego_image))
    {
	printf("\tINFO:Magic string encodded successfully.\n\n");
    }
    else
    {
	printf("\tError: Magic string unable to encode.\n");
	return e_failure;
    }	

    /* Encode secret file extention size */
    if(!encode_secret_file_size(encInfo->secret_file_exnt_size,encInfo->fptr_src_image,encInfo->fptr_stego_image))
    {
	printf("\tINFO:Secret file size encodded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file size unable to encode.\n\n");
	return e_failure;
    }


    /* Encode secret file extention name */
    if(!encode_secret_file_extn(encInfo->secret_file_exnt_name,encInfo->fptr_src_image,encInfo->fptr_stego_image))
    {
	printf("\tINFO:Secret file extention name encodded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file extention name unable to encode.\n");
    }

    /* Encode secret data size */
    if(!encode_secret_file_size(encInfo->secret_file_data_size,encInfo->fptr_src_image,encInfo->fptr_stego_image))
    {
	printf("\tINFO:Secret file data size encodded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file data size unable to encode.\n");
	return e_failure;
    }

    /* Encode secret data */
    if(!encode_secret_file_data(encInfo->secret_file_data_size,encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo->fptr_secret))
    {
	printf("\tINFO:Secret file data encoded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file data unable to encode.\n");
	return e_failure;
    }

    /* Copy ramaining data */
    if(!copy_remaining_img_data(encInfo->fptr_stego_image,encInfo->fptr_src_image))
    {
	printf("\tINFO:Remaining data copied successfully.\n\n");
    }
    else
    {
	printf("Error: Remaining data unable to copy.\n");
	return e_failure;
    }

    printf("\t  -----Encoding completed successfully.-----\n");

}

/* Function Definitions */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    /* Get image size
     * Input: Image file ptr
     * Output: width * height * bytes per pixel (3 in our case)
     * Description: In BMP Image, width is stored in offset 18,
     * and height after that. size is 4 bytes */

    uint width,height;
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);
    fread(&height, sizeof(int), 1, fptr_image);

    return width * height * 3;
}

Status open_files(EncodeInfo *encInfo)
{
    /* Get File pointers for i/p and o/p files
     * Inputs: Src Image file, Secret file and
     * Stego Image file
     * Output: FILE pointer for above files
     * Return Value: e_success or e_failure, on file errors */

    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");

    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    /* Check source image capacity
     * Inputs: EncodeInfo Structue
     * Output: e_success, e_failure based on capacity check
     * Return Value: e_success or e_failure, on file errors */

    /* 1.Find magic string length */
    /* 2.Call source image size funtion and get sourse image size */
    /* 3.Find secret file extention size */
    /* 4.Find secret data size */
    /* 5.Check sourse image size is greater then sum of all encoding data sizes. */

    encInfo->magic_string_len=strlen(MAGIC_STRING);

    rewind(encInfo->fptr_src_image);
    encInfo->source_imge_size=get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->secret_file_exnt_name = strstr(encInfo->secret_fname,".");
    encInfo->secret_file_exnt_size=strlen(encInfo->secret_file_exnt_name);


    fseek(encInfo->fptr_secret, 0, SEEK_END);
    encInfo->secret_file_data_size=ftell(encInfo->fptr_secret);

    if( encInfo->source_imge_size >= ((encInfo->magic_string_len * 8)+sizeof(int)+(encInfo->secret_file_exnt_size * 8)+sizeof(int)+(encInfo->secret_file_data_size * 8) ) )
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /* 1.Rewind sourse image file pointer */
    /* 2.Read 54 byte from soure and store in char array */
    /* 3.Write that 54 bytes to stego image */
    char temp[54];
    rewind(fptr_src_image);
    if(fread(temp,54,1,fptr_src_image))
    {
	fwrite(temp,54,1,fptr_stego_image);
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status encode_byte_to_lsb(char ch, char *arr)
{
    /* 1.Clear lsb bit of image data  */
    /* 2.Get a bit and put put a ith bit of characater */
    /* 3.Run step:1 8 times */
    for(int i=0;i<8;i++)
    {
	arr[i]= (arr[i] & ( ~1) ) | ( (ch & (1<<i))>>i);
    }
    return e_success;
}

Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /* 1.Find length of string data */
    /* 2.Initialize 1 byte string data to character variable */
    /* 3.Read 8bytes of data from sourse image */
    /* 4.Call encode byte to lsb function */
    /* 5.Write the encoded byte to stego image */
    /* 6.Repeat step:2 length times */

    char image_buffer[8];
    unsigned char ch;

    int len=strlen(data);

    for(int i=0;i<len;i++)
    {
	ch=data[i];

	if(fread(image_buffer,8,1,fptr_src_image))
	{
	    encode_byte_to_lsb(ch,image_buffer);

	    fwrite(image_buffer,8,1,fptr_stego_image);
	}
    }
    return e_success;
}

Status encode_magic_string(char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /* 1.Call encode data to image and pass magic string */
    if(!encode_data_to_image(magic_string,fptr_src_image,fptr_stego_image))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status encode_size_to_leb(int size, char *arr)
{
    /* 1.Clear lsb bit of data */
    /* 2.Get a bit from size */
    /* 3.Put that bit to lsb of data */
    /* 4.Repeat the step:1 32 times */

    for(int i=0;i<32;i++)
    {
	arr[i]= (arr[i] & ( ~1) ) | ( (size & (1<<i))>>i);
    }
    return e_success;
}

Status encode_secret_file_extn(char *extn, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /* 1.Call encode data to image and pass extention */
    if(!encode_data_to_image(extn,fptr_src_image,fptr_stego_image))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status encode_secret_file_size(int secret_file_exnt_size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    /* 1.Read 32 byte of data from sourse image */
    /* 2.Call encode size to lsb and pass that data and size */
    /* 3.Write the encoded data to stego image */

    char image_buffer[32];
    if(fread(image_buffer,32,1,fptr_src_image))
    {
	encode_size_to_leb(secret_file_exnt_size,image_buffer);
	fwrite(image_buffer,32,1,fptr_stego_image);
	return e_success;
    }
    else
	return e_failure;
}

Status encode_secret_file_data(int size,FILE *fptr_src_image, FILE *fptr_stego_image,FILE *fptr_secret)
{
    /* 1.Rewind the secret data file */
    /* 2.Read secret data size number of bytes from secret data file */
    /* 3.Call encode data to image and the secret data and check validation*/

    char data[size];
    rewind(fptr_secret);

    if(fread(data,size,1,fptr_secret))
    {
	encode_data_to_image(data,fptr_src_image,fptr_stego_image);
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status copy_remaining_img_data(FILE *fptr_stego_image,FILE *fptr_src_image)
{
    /* 1.Read 1 byte from sourse image */
    /* 2.write that 1 byte to stego image */
    /* 3.Repeat step:1 till end of file */
    char ch;
    while(fread(&ch,1,1,fptr_src_image) > 0)
    {
	fwrite(&ch,1,1,fptr_stego_image);
    }
    return e_success;
}

