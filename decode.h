#include "types.h" // Contains user defined types
#include<stdio.h>

   /* stucture for decode */

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *output_image_name;
    FILE *fptr_output_image;

    /* Magic string  */
    char magic_str[5];

    /* Secret file info */
    char secret_file_name[100];
    FILE *fptr_secret_file;
    uint secret_file_extn_size;
    char secret_file_extn_name[10];
    uint secret_data_size;
    char secret_file_data[100];

} DecodeInfo;

    /* Function protype  */

/* Perform do decoding  */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers to sourse file  */
Status open_secret_file(DecodeInfo *decInfo);

/* Get file pointers to secret file */
Status open_file(DecodeInfo *decInfo);

/* Decode Magic strig  */
Status decode_magic_string(FILE *fptr_output_image,char *pass);

/* Decode data from image  */
Status decode_image_to_data(int len,char *pass,FILE *fptr_output_image);

/* Get one byte from lsb  */
Status decode_byte_from_lsb(char *data,char *ch);

/* Decode secrete file extention size */
Status decode_secret_file_extn_size(FILE *fptr_output_image,uint *size);

/* Get size from lsb */
Status decode_size_from_lsb(char *data,unsigned int *size);

/* Decode and get secret file extention name */
Status decode_secret_file_extn_name(FILE *fptr_output_image,char *extn_name,uint size);

/* Concat secret file name with extention  */
Status concat_extn(DecodeInfo *decInfo);

/* Get secret data size */
Status decode_secret_file_data_size(FILE *fptr_output_image,uint *size);

/* Get secret data */
Status decode_secret_data(char *str,FILE *fptr_output_image,FILE *fptr_secret_file, uint size);

/* Write secret data to secret file */
Status write_secrete_data(char *data,uint size,FILE *fptr_secret_file);
