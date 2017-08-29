// *****************************************************************************************
//  ImageClass.h
// *****************************************************************************************

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
/* Para usar o include da GLUT desta forma, adicione as
seguintes clausulas nas configurac›es do Linker:
-framework CoreFoundation
-framework GLUT
-framework OpenGL
*/
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "SOIL/SOIL.h"

class Image {

protected:
	unsigned char *data;
	int sizeX, sizeY, channels;

	bool LoadImageFile(const char *name) {
		data = SOIL_load_image( name, &sizeX, &sizeY, &channels, 0);
//	    TEX *temp;
//		temp = CarregaJPG(name, true);
//		data = temp->data;
//		sizeX = temp->dimx;
//		sizeY = temp->dimy;
//		channels = temp->ncomp;

		if (data == NULL) {
			cout << "Error loading image " << name << "." << endl;
			return false;
		}
		cout << "Image " << name << " loaded !"<< endl;
		cout << "Canais:" << channels << endl;

		return true;
	}
	void FlipY() {

		unsigned long InicioLinhaA, InicioLinhaB, TamLinha;
		unsigned char  *ImgLineTemp=NULL;

		ImgLineTemp = (unsigned char *) malloc(sizeof(unsigned char)* sizeX * channels);

		TamLinha = sizeX * channels;
		InicioLinhaA = 0;
		InicioLinhaB = (sizeY-1) * TamLinha;

		for( unsigned int line = 0; line < sizeY/2; line++ ) {
			memcpy(ImgLineTemp, &data[InicioLinhaA],TamLinha);
			memcpy(&data[InicioLinhaA], &data[InicioLinhaB], TamLinha);
			memcpy(&data[InicioLinhaB], ImgLineTemp, TamLinha);
			InicioLinhaA += TamLinha;
			InicioLinhaB -= TamLinha;
		}
		free (ImgLineTemp);
	}

	Image(int channels=3) {
		data = NULL;
		sizeX = 0;
		sizeY = 0;
		this->channels = channels;

	}
public:
	int SizeX() {
		return sizeX;
	};
	int SizeY() {
		return sizeY;
	};
	int Channels() {
		return channels;
	}

};

class ImageClass: public Image {

private:
	void SetColorMode();
protected:
	int PosX, PosY;
	float zoomH, zoomV;
	GLenum colorMode;

public:

	double Tamanho;
	ImageClass(int channels=3);
	ImageClass(int sizeX, int sizeY, int channels=3);

	int Load(const char *);
	void Save(const char *);
	void Display(void);
	void Delete(void);
	void DrawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	void DrawLineH(int y, int x1, int x2, unsigned char r, unsigned char g, unsigned char b);
	void DrawLineV(int x, int y1, int y2,unsigned char r, unsigned char g, unsigned char b );
	void ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b);
	double GetPointIntensity(int x, int y);
	float GetZoomH() {
		return zoomH;
	};
	float GetZoomV() {
		return zoomV;
	};
	void SetZoomH(float H) {
		zoomH = H;
	};
	void SetZoomV(float V) {
		zoomV = V;
	};
	void CopyTo(ImageClass *i);
	void Clear();

	unsigned char *GetImagePtr();
	void SetSize(int sizeX, int sizeY, int channels=3);

	void SetPos(int X, int Y);

	void DrawBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b);
	void DrawLine(int x0,int y0,int x1, int y1,unsigned char r, unsigned char g, unsigned char b  );
};
