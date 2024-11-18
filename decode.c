#include <stdio.h>
#include "decode.h" // Contains decoding files
#include "types.h"  // Contains types
#include<string.h>
#include "common.h" // Contains Magic string info
#include<stdlib.h>

/* Decoding Starts  */
Status do_decoding(DecodeInfo *decInfo)
{
    /* open file and create file ponters */
    if(open_file(decInfo) == e_success)
    {
	printf("\tINFO:Files are opened successfully.\n\n");
    }
    else
    {
	printf("\tError:Files are not opend successfully :( \n");
	return e_failure;
    }

    /* Decode magic string */
    if(!decode_magic_string(decInfo->fptr_output_image,decInfo->magic_str))
    {
	/* Compare decoded magic string with actual magic string */
	if((strcmp(MAGIC_STRING,decInfo->magic_str) == 0))
	{
	    printf("\tINFO:Magic string decoded and validated.\n\n");
	}
	else
	{
	    printf("\tError: Decoded magic string is worng.\n");
	}
    }
    else
    {
	return e_failure;
    }

    /* Decode file extention size  */
    if(!decode_secret_file_extn_size(decInfo->fptr_output_image,&decInfo->secret_file_extn_size))
    {
	printf("\tINFO:Screte file extention size decoded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file extention size not decoded.\n");
	return e_failure;
    }

    /* Decode file extention name */
    if(!decode_secret_file_extn_name(decInfo->fptr_output_image, decInfo->secret_file_extn_name,decInfo->secret_file_extn_size))
    {
	printf("\tINFO:Secret file extention name decode successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file extention name unable to decode.\n");
    }

    /* Contact secret file name with extention */
    if(!concat_extn(decInfo))
    {
	printf("\tINFO:File extention concatinated successfully.\n\n");
    }
    else
    {
	printf("\tError: File extention not concatinated.\n");
	return e_failure;
    }

    /* Open secret file and creat file pointer */
    if(!open_secret_file(decInfo))
    {
	printf("\tINFO:Secret file opened successfully.\n\n");
    }
    else
    {
	printf("\tError: Secrete file unable to open.\n");
	return e_failure;
    }

    /* Decode secrete file data size  */
    if(!decode_secret_file_data_size(decInfo->fptr_output_image,&decInfo->secret_data_size))
    {
	printf("\tINFO:Secrete file data size decoded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret file data size unable to decode.\n");
	return e_failure;
    }

    /* Decode secret data */
    if(!decode_secret_data(decInfo->secret_file_data, decInfo->fptr_output_image, decInfo->fptr_secret_file, decInfo->secret_data_size))
    {
	printf("\tINFO:Secret data decoded successfully.\n\n");
    }
    else
    {
	printf("\tError: Secret data unable to decode.\n");
	return e_failure;
    }

    /* Write secrete data to secret file  */
    if(!write_secrete_data(decInfo->secret_file_data,decInfo->secret_data_size,decInfo->fptr_secret_file))
    {
	printf("\tINFO:Secret data written to secret file.\n\n");
    }
    else
    {
	printf("Error: Secret data unable to write to secret file.\n");
    }

    /* Close file pointers */
    fclose(decInfo->fptr_output_image);

    /* Print promt that decoding completed  */
    printf("\t  -----Decoding completed successfully.------  :)\n");
    return e_success;
}

Status open_file(DecodeInfo *decInfo)
{
    /* 1.Open and create file ponter to sourse image */
    /* 2.Check file ponter if created successfully or not  */
    /* 3.Print stadard error if files not opened */
    decInfo->fptr_output_image = fopen(decInfo->output_image_name, "r");
    
    if (decInfo->fptr_output_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_image_name);
	return e_failure;
    }
    return e_success;
}

Status decode_magic_string(FILE *fptr_output_image,char *str)
{
    /* 1.Seek offset to 54 possition */
    /* 2.Find magic string lenth */
    /* 3.Call image to data function */
    int len;
    fseek(fptr_output_image,54,SEEK_SET);
    len=strlen(MAGIC_STRING);
    if(!decode_image_to_data(len, str,fptr_output_image))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status decode_image_to_data(int len,char *str,FILE *fptr_output_image)
{
    /* 1.Declare requied and variable */
    /* 2.Read 8 byte from from sourse image */
    /* 3.Call decode_byte_from_lsb */
    /* 4.Repeat step 2 length times */
    char image_buffer[8];
    unsigned char ch;
    int i=0;

    for(i=0; i<len; i++)
    {
	ch=0;
	fread(image_buffer,8,1,fptr_output_image);
	decode_byte_from_lsb(image_buffer,&ch);
	str[i]=ch;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data,char *ch)
{
    /* 1.Get 1 bit from lsb of image data */
    /* 2.Clear ith bit and Put that bit to character variable */
    /* 3.Repeat step:1 8 times */
    for(int i=0;i<8;i++)
    {
	*ch=( ((data[i] & 1)<<i) | *ch );
    }
    return e_success;
}

Status decode_secret_file_extn_size(FILE *fptr_output_image,uint *size)
{
    /* 1.Read 32 bytes of data from sourse image and check validation */
    /* 2.Call decode size from lsb function */
    char image_buffer[32];
    if(fread(image_buffer,32,1,fptr_output_image))
    {
	if(!decode_size_from_lsb(image_buffer,size))
	{
	    return e_success;
	}
	else
	{
	    return e_failure;
	}
    }
    else
    {
	return e_failure;
    }
}

Status decode_size_from_lsb(char *data, uint *size)
{
    /* 1.Get 1 bit from lsb of image data */
    /* 2.Clear ith bit and Put that bit to size */
    /* 3.Repeat step:1 8 times */
    for(int i=0;i<32;i++)
    {
	*size = *size | ((data[i] & 1 )<<i);
    }
    return e_success;
}

Status decode_secret_file_extn_name(FILE *fptr_output_image,char *extn_name,uint size)
{
    /* 1.Call image to data function and check for validation */
    if(!decode_image_to_data(size,extn_name,fptr_output_image))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

Status concat_extn(DecodeInfo *decInfo)
{
    /* 1.Copy desired name to secret file name variable */
    /* 2.Concatinate secret file name with extention name */
    strcpy(decInfo->secret_file_name,"output");
    strcat(decInfo->secret_file_name,decInfo->secret_file_extn_name);

    return e_success;
}


Status open_secret_file(DecodeInfo *decInfo)
{
    /* 1.Open and create file ponter to secret data file */
    /* 2.Check file ponter if created successfully or not  */
    decInfo->fptr_secret_file = fopen(decInfo->secret_file_name, "w");
    if (decInfo->fptr_secret_file == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_file_name);
	return e_failure;
    }
    return e_success;
}

Status decode_secret_file_data_size(FILE *fptr_output_image,uint *size)
{
    /* 1.Read 32 bytes from sourse image */
    /* 2.Call decode size from lsb funtion and check validation */
    char image_buffer[32];
    if(fread(image_buffer,32,1,fptr_output_image))
    {
	if(!decode_size_from_lsb(image_buffer,size))
	{
	    return e_success;
	}
	else
	{
	    return e_failure;
	}
    }
    else
    {
	return e_failure;
    }
}

Status decode_secret_data(char *str,FILE *fptr_output_image,FILE *fptr_secret_file,uint size)
{
    /* 1.Call image to data function and check for validation */
    char image_buffer[size*8];
    if(!decode_image_to_data(size,str,fptr_output_image))
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}
Status write_secrete_data(char *data,uint size,FILE *fptr_secret_file)
{
    /* 1.Write secret data to secret file */
    /* 2.Close file ponter to secret file */
    if(fwrite(data,size,1,fptr_secret_file))
    {
	fclose(fptr_secret_file);
	return e_success;
    }
    else
    {
	return e_failure;
    }
}

