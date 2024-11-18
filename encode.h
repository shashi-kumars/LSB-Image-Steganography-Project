#include "types.h" // Contains user defined types
#include<stdio.h>

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */
/*
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
*/
typedef struct _EncodeInfo
{
    /* Source Image Info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint source_imge_size;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char *secret_file_exnt_name;
    uint secret_file_exnt_size;
    uint secret_file_data_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Magic string Info */
    uint magic_string_len;

} EncodeInfo;


/* Encoding function prototype */

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *extn, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file size */
Status encode_secret_file_size( int extn_size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode secret file data*/
Status encode_secret_file_data(int size,FILE *fptr_src_image, FILE *fptr_stego_image,FILE *fptr_secret);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb( char ch,char *image_data);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_stego_image,FILE *fptr_src_image);

/* encoding size_to_lsb */
Status encode_size_to_leb(int size, char *arr);

