/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "GL/glut.h"


void CambiaDim(int, int);
void DisegnaTutto();
void AzioneTasto(unsigned char Tasto, int x, int y);

GLubyte Texrgb[256 * 256 * 3];
//GLubyte Texrgba[256 * 256 * 4];


void CambiaDim(int w, int h)
{
	glViewport(0, 0, w, h);
	
	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, +1, -1, +1, 2, 200);
	
}

GLfloat X, Y, A;

GLfloat S, T;

void DisegnaTutto()
{
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(X, 0, Y - 5);
	glRotatef(A, 0, 1, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, Texrgb);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, Texrgb);
	
	glEnable(GL_TEXTURE_2D);
    
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1, -1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1, -1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1, 1);
	glEnd();
	
	glutSwapBuffers();
}

void AzioneTasto(unsigned char t, int, int)
{
	if(t == 'a') X -= 0.1f;
	if(t == 's') X += 0.1f;
	if(t == 'w') Y += 0.1f;
	if(t == 'z') Y -= 0.1f;
	if(t == 'y') A += 10.0f;
	if(t == 'u') A -= 10.0f;
	
	if(t == 'j') S -= 0.01f;
	if(t == 'k') S += 0.01f;
	if(t == 'i') T += 0.01f;
	if(t == 'm') T -= 0.01f;
	
	glutPostRedisplay();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(450, 450);
	glutCreateWindow("Texture: Mipmapping + anisotropico");
	
	glutReshapeFunc(CambiaDim);
	glutDisplayFunc(DisegnaTutto);
	glutKeyboardFunc(AzioneTasto);
	
	{
		FILE *fHan;
		
		fHan = fopen("rosso.raw", "rb");
		if(fHan == NULL) return(0);
		fread(Texrgb, 256 * 256, 3, fHan);
		fclose(fHan);
	}
	
	glutMainLoop();
	
	return(0);
}

