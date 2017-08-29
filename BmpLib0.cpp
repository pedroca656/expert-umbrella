// **********************************************************************
//  BmpLib0.cpp
//
// **********************************************************************

#include <stdio.h>
#include <malloc.h>
#include "BmpLib0.h"

static unsigned char *red, *green, *blue;
static int width, height;

static imginfoheader imgheader;
static bmpinfoheader bmpheader;

// **********************************************************************
//
//
// **********************************************************************
int getBMPSize(char *name, unsigned int &sizeX, unsigned int &sizeY)
{
   FILE *fptr;
   long x,y;
   unsigned char ch;

   if((fptr=fopen(name,"rb"))==NULL){
       printf("\nFile not found.");
       return 1;
   }
   else
   {
       fread(&bmpheader.fileid,2,1,fptr);
       fread(&bmpheader.filesize,4,1,fptr);
       fread(&bmpheader.reserved1,2,1,fptr);
       fread(&bmpheader.reserved2,2,1,fptr);
       fread(&bmpheader.imgoffset,4,1,fptr);
       fread(&imgheader,sizeof(imgheader),1,fptr);

       if ((bmpheader.fileid != 19778) ||
           (bmpheader.imgoffset != 54) ||
           (imgheader.headersize != 40) ||
           (imgheader.numplanes != 1) ||
           (imgheader.pixeldepth != 24) ||
           (imgheader.compression != 0))
        {
           printf("Error: Invalid bitmap image.");
           fclose(fptr);
           return 1;
       }
       else {
            sizeX = imgheader.imgwidth;
            sizeY = imgheader.imgheight;
            fclose(fptr);
            return 0;
       }
    }
}
   
// **********************************************************************
//
//
// **********************************************************************
int load_bmp (char bmp[], unsigned int &sizeX, unsigned int &sizeY)
{

   FILE *fptr;
   long x,y;
   unsigned int padbytes=0;
   unsigned char ch;

   if((fptr=fopen(bmp,"rb"))==NULL){
       printf("\nFile not found.");
       return 1;
   }else{
       fread(&bmpheader.fileid,2,1,fptr);
       fread(&bmpheader.filesize,4,1,fptr);
       fread(&bmpheader.reserved1,2,1,fptr);
       fread(&bmpheader.reserved2,2,1,fptr);
       fread(&bmpheader.imgoffset,4,1,fptr);
       fread(&imgheader,sizeof(imgheader),1,fptr);

       if (    (bmpheader.fileid != 19778) ||
           (bmpheader.imgoffset != 54) ||
           (imgheader.headersize != 40) ||
           (imgheader.numplanes != 1) ||
           (imgheader.pixeldepth != 24) ||
           (imgheader.compression != 0)
       ){
           printf("Error: Invalid bitmap image.");
           return 1;
       };


       padbytes = (imgheader.imgwidth*3)%4;
       if(padbytes!=0)
           padbytes = 4-padbytes;
       red = (unsigned char *) malloc(imgheader.imgheight*imgheader.imgwidth);
       green = (unsigned char *) malloc(imgheader.imgheight*imgheader.imgwidth);
       blue = (unsigned char *) malloc(imgheader.imgheight*imgheader.imgwidth);

       if (red == NULL || green == NULL || blue == NULL){
           printf("\nOut of memory.");
           return 1;
       }

       for(x=imgheader.imgheight-1;x>-1;x--){
           for(y=0;y<imgheader.imgwidth;y++){
               blue[(x*imgheader.imgwidth+y)] = getc(fptr);
               green[(x*imgheader.imgwidth+y)] = getc(fptr);
               red[(x*imgheader.imgwidth+y)] = getc(fptr);
           }
           for(y=0;y<padbytes;y++)
               getc(fptr);
       }
       sizeX = width = imgheader.imgwidth;
       sizeY = height = imgheader.imgheight;
       fclose(fptr);
   }
   return 0;
}

// **********************************************************************
//
//
// **********************************************************************
void CopyBmpToImageClass(unsigned char* data)
{

    long x,y, i;
    for(x=0;x<width;x++)
    {
        for(y=0;y<height;y++)
        {
            unsigned long addr;
            i = (y*width+ x);
            addr = (unsigned long)( (height-y-1) *(width)* 3 + x * 3 );
            data[addr++] = red[i];
            data[addr++] = green[i];
            data[addr] = blue[i];
        }
    }
}
// **********************************************************************
//
//
// **********************************************************************
void freeBMPMemory()
{
    free (red);
    free (green);
    free (blue);
    red=green=blue;
}

// **********************************************************************
//
//
// **********************************************************************
int CopyImageClassToBmp(unsigned char* data)
{
    long count = 0;
    long x,y, i;
    long tam = height*width;

    red = (unsigned char *) malloc(tam);
    green = (unsigned char *) malloc(tam);
    blue = (unsigned char *) malloc(tam);
    if (red == NULL || green == NULL || blue == NULL){
       printf("\nOut of memory - CopyImageClassToBmp.");
       return 1;
    }

    for(x=0;x<width;x++)
    {
        for(y=0;y<height;y++)
        {
            unsigned long addr;
            i = (y*width+ x);
            addr = (unsigned long)( (height-y-1) *(width)* 3 + x * 3 );
            red[i] = data[addr++];
            green[i] = data[addr++];
            blue[i] = data[addr];
        }
    }
    return 0;
}
// **********************************************************************
//
//
// **********************************************************************
int save_bmp(char bmp[], unsigned char* data, unsigned int sizeX, unsigned int sizeY)
{
   
   FILE *fptr;
   long x,y,c;
   unsigned int padbytes=0;
   unsigned char ch;
   
// copia os dados da ImageClass para o BMP
   width = sizeX;
   height = sizeY;
   if (CopyImageClassToBmp(data))
        return 1; // erro
// ----

   bmpheader.fileid = 19778;
   bmpheader.filesize = width * height + 54;
   bmpheader.reserved1 = 0;
   bmpheader.reserved2 = 0;
   bmpheader.imgoffset = 54;
   imgheader.headersize = 40;
   imgheader.imgwidth = width;
   imgheader.imgheight = height;
   imgheader.numplanes = 1;
   imgheader.pixeldepth = 24;
   imgheader.compression = 0;
   imgheader.bitmapsize = width * height;
   imgheader.hresolution = 0;
   imgheader.vresolution = 0;
   imgheader.usedcolors = 0;
   imgheader.significantcolors = 0;

   if((fptr=fopen(bmp,"wb"))==NULL){
       printf("\nCould not write to file.");
       return 1;
   }else{
       fwrite(&bmpheader.fileid,2,1,fptr);
       fwrite(&bmpheader.filesize,4,1,fptr);
       fwrite(&bmpheader.reserved1,2,1,fptr);
       fwrite(&bmpheader.reserved2,2,1,fptr);
       fwrite(&bmpheader.imgoffset,4,1,fptr);
       fwrite(&imgheader,sizeof(imgheader),1,fptr);
       c=0;

       padbytes = (imgheader.imgwidth*3)%4;
       if(padbytes!=0)
           padbytes = 4-padbytes;
       for(x=imgheader.imgheight-1;x>-1;x--){
           for(y=0;y<imgheader.imgwidth;y++)
           {
               putc(blue[(x*imgheader.imgwidth+y)],fptr);
               putc(green[(x*imgheader.imgwidth+y)],fptr);
               putc(red[(x*imgheader.imgwidth+y)],fptr);
           }
           for(y=0;y<padbytes;y++)
               putc(0,fptr);
       }
       fclose(fptr);
   }
   
   freeBMPMemory();
   return 0;
}




// **********************************************************************
//
//
// **********************************************************************
//void CopyBmpToImageClassOK(unsigned char* data)
//{
//   long count = 0;
//   long x,y, i;
//   for(x=0;x<height;x++)
//   {
//       for(y=0;y<width;y++)
//       {
//            unsigned long addr;
//            i = (x*width + y);
//            addr = (unsigned long)( (height-x-1) *(width)* 3 + y * 3 );
//            data[addr++] = red[i];
//            data[addr++] = green[i];
//            data[addr] = blue[i];
//       }
//    }
//}


