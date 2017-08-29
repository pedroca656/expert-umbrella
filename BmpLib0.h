#ifndef BmpLib0_DEF
#define BmpLib0_DEF

int load_bmp(char bmp[], unsigned int &sizeX, unsigned int &sizeY);
void CopyBmpToImageClass (unsigned char* data);
int save_bmp(char bmp[], unsigned char* data, unsigned int sizeX, unsigned int sizeY);
void freeBMPMemory();

int getBMPSize(char *name, unsigned int &sizeX, unsigned int &sizeY);

typedef struct {
       unsigned short int fileid;
       unsigned long filesize;
       unsigned short int reserved1;
       unsigned short int reserved2;
       unsigned long imgoffset;
} bmpinfoheader ;

typedef struct {
       unsigned long headersize;
       long imgwidth;
       long imgheight;
       unsigned short int numplanes;
       unsigned short int pixeldepth;
       unsigned long compression;
       unsigned long bitmapsize;
       long hresolution;
       long vresolution;
       unsigned long usedcolors;
       unsigned long significantcolors;
} imginfoheader;


#endif
