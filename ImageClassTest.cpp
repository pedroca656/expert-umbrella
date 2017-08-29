// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
//
// Programa de testes para manipulação de Imagens
//
//  Este programa deve ser compilador junto com a classe "ImageClass",
//  que está implementada no arquivo "ImageClass.cpp"
//
//  - para compilar no Visual C ou Visual Studio acrescente as seguintes
//        bibliotecas:
//      opengl32.lib glu32.lib glaux.lib glut32.lib
//
//  - para compilar no DEVCPP (ou no g++) acrescente as seguintes
//        bibliotecas:
//        -lopengl32 -lglut32 -lglu32 -lglaux
//
//  - para alterar a imagem que é carregada pelo programa, olhe a
//    rotina 'init' e altere a linha:
//       r = Image->Load("    ");
//
// Marcio Sarroglia Pinho
//
// pinho@inf.pucrs.br
// **********************************************************************

#include <iostream>
using namespace std;

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GL/glut.h>
#endif

#include "SOIL/SOIL.h"

#include "ImageClass.h"

ImageClass *Image, *NovaImagem;

#define LIMIAR 100
#define LARGURA_JAN 600
#define ALTURA_JAN 600
// **********************************************************************
//  void ConvertBlackAndWhite()
//
//
// **********************************************************************
void ConvertBlackAndWhite()
{
    // Tarefa 1:
    //  Mude o valor do LIMIAR para números pequenos como 5 ou 10

	unsigned char r,g,b;
    int x,y;
    int i;
    cout << "Iniciou Black & White....";
    //NovaImagem->DrawPixel(20, 1,100,255,0,0 );

    for(x=0;x<Image->SizeX();x++)
    {
        for(y=0;y<Image->SizeY();y++)
        {
            i = Image->GetPointIntensity(x,y);
			Image->ReadPixel(x,y,r,g,b);

            // printf("Intens Fora: %5d\n",i);
            if (i < LIMIAR)
            {
                NovaImagem->DrawPixel(x, y,0,0,0);
            }
			else NovaImagem->DrawPixel(x, y, 255,0,0);

        }
    }
    cout << "Concluiu Black & White.\n";

}


// **********************************************************************
// void DetectImageBorders()
//
//
// **********************************************************************
void DetectImageBorders()
{

    // varredura vertical

    // varredura horizontal

}

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
//
// **********************************************************************
void init(void)
{

    Image = new ImageClass();
    int r;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto

    r = Image->Load("775.png");


    if (!r)
        exit(1);
    else
        cout << ("Imagem carregada!\n");

    // Instacia o objeto que irá exibir a nova imagem
    // Caso precise alterar o tamanho da nova imagem, mude os parâmetros
    // da construtura, na chamada abaixo
    NovaImagem = new ImageClass(Image->SizeX(), Image->SizeY(), Image->Channels());

}

// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    gluOrtho2D(0,w,0,h);

    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
// Ajusta o ZOOM da imagem para que apareca na metade da janela
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/Image->SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT)/2.0)/Image->SizeY();
    Image->SetZoomH(zoomH);
    Image->SetZoomV(zoomV);

// posiciona a imagem nova na metada da direita da janela
    NovaImagem->SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, 0);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
    NovaImagem->SetZoomH(zoomH);
    NovaImagem->SetZoomV(zoomV);

// Desenha uma Linha Vermelha
    Image->DrawLine(0,0,1000,1000, 255,0,0);
// Coloca as imagens na tela
    Image->Display();
    NovaImagem->Display();

// Mostra a tela OpenGL
    glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

    switch ( key )
    {
        case 27: // Termina o programa qdo
            NovaImagem->Delete();
            Image->Delete();
            exit ( 0 );   // a tecla ESC for pressionada
        break;
        case '2':
            ConvertBlackAndWhite();
            glutPostRedisplay();    // obrigatório para redesenhar a tela
        break;
        default:
        break;
    }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
    switch ( a_keys )
    {
        case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
        break;
        case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...

        break;
        default:
        break;
    }
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (10,10);

    // Define o tamanho da janela gráfica do programa
    glutInitWindowSize  ( LARGURA_JAN, ALTURA_JAN);
    glutCreateWindow    ( "Image Loader" );

    init ();

    glutDisplayFunc ( display );
    glutReshapeFunc ( reshape );
    glutKeyboardFunc ( keyboard );
    glutSpecialFunc ( arrow_keys );
    //glutIdleFunc ( display );

    glutMainLoop ( );

    cout << "FIM" << endl;

    return 0;
}


