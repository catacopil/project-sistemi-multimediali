/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "GL/glut.h"

GLubyte Texture1[256 * 256 * 3];
GLubyte Texture2[256 * 256 * 3];

void CambiaDim(int, int);
void DisegnaTutto();
void Azione(unsigned char key, int x, int y);
void DisegnaCubo(GLfloat);


void CambiaDim(int w, int h)
{
	//GLfloat rat;
	
	if(h == 0) h = 1;
	
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-1, 1, -1, 1, 1, -1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void DisegnaCubo(GLfloat R)
{
	glBegin(GL_QUADS);
	
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1);
	glVertex3f(R, R, R);
	glTexCoord2f(0, 1);
	glVertex3f(-R, R, R);
	glTexCoord2f(0, 0);
	glVertex3f(-R, -R, R);
	glTexCoord2f(1, 0);
	glVertex3f(R, -R, R);
	
	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-R, R, -R);
	glTexCoord2f(0, 1);
	glVertex3f(R, R, -R);
	glTexCoord2f(0, 0);
	glVertex3f(R, -R, -R);
	glTexCoord2f(1, 0);
	glVertex3f(-R, -R, -R);
	
	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(R, R, -R);
	glTexCoord2f(0, 1);
	glVertex3f(R, R, R);
	glTexCoord2f(0, 0);
	glVertex3f(R, -R, R);
	glTexCoord2f(1, 0);
	glVertex3f(R, -R, -R);
	
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-R, R, R);
	glTexCoord2f(0, 1);
	glVertex3f(-R, R, -R);
	glTexCoord2f(0, 0);
	glVertex3f(-R, -R, -R);
	glTexCoord2f(1, 0);
	glVertex3f(-R, -R, R);
	
	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(R, R, -R);
	glTexCoord2f(0, 1);
	glVertex3f(-R, R, -R);
	glTexCoord2f(0, 0);
	glVertex3f(-R, R, R);
	glTexCoord2f(1, 0);
	glVertex3f(R, R, R);
	
	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-R, -R, -R);
	glTexCoord2f(0, 1);
	glVertex3f(R, -R, -R);
	glTexCoord2f(0, 0);
	glVertex3f(R, -R, R);
	glTexCoord2f(1, 0);
	glVertex3f(-R, -R, R);
	
	glEnd();
}

GLfloat a, b;

void DisegnaTutto()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 2, 1000);
	glMatrixMode(GL_MODELVIEW);
	
	GLfloat ambiente[4] = { 1, 1, 1, 1 };
	GLfloat direttiva[4] = { 1, 1, 1, 1 };
	
	GLfloat brillante_l[4] = { 1, 1, 1, 1 };
	GLfloat brillante_m[4] = { 0, 0, 0, 1 };
	
	glMateriali(GL_FRONT, GL_SHININESS, 32);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, direttiva);
	
	glEnable(GL_TEXTURE_2D);
	
	glLoadIdentity();
	glTranslatef(0, 0, -40);
	glRotatef(a, 0, 1, 0);
	glRotatef(b, 1, 0, 0);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, brillante_l);
	glBindTexture(GL_TEXTURE_2D, 1);
	DisegnaCubo(8);
	
	glTranslatef(18, 0, 0);
    
	glMaterialfv(GL_FRONT, GL_SPECULAR, brillante_m);
	glBindTexture(GL_TEXTURE_2D, 2);
	DisegnaCubo(8);
	
	glutSwapBuffers();
    
}

char Stringa[80];

void Azione(unsigned char t, int , int )
{
	switch(t) {
		case 'a': a += 5; break;
		case 's': a -= 5; break;
		case 'q': b += 5; break;
		case 'w': b -= 5; break;
	}
	
	glutPostRedisplay();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(450, 450);
	glutCreateWindow("Texture Binding");
	
	{
		FILE *fHan = fopen("legno_256.raw", "rb");
		if(fHan == NULL) return(0);
		fread(Texture1, 256 * 256, 3, fHan);
		fclose(fHan);
		
		fHan = fopen("mattoni.raw", "rb");
		if(fHan == NULL) return(0);
		fread(Texture2, 256 * 256, 3, fHan);
		fclose(fHan);
	}
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	
	GLfloat black[4] = { 0, 0, 0, 1 };
	GLfloat aLite[4] = { 0.3, 0.3, 0.3, 1 };
	GLfloat dLite[4] = { 1.0, 1.0, 1.0, 1 };
	GLfloat sLite[4] = { 1, 1.0, 1.0, 1 };
	
	GLfloat PosLite[4] = { 1, 1, 1, 0 };
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, PosLite);
	
	glEnable(GL_LIGHT0);
	
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture1);
    
 	glBindTexture(GL_TEXTURE_2D, 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture2);
	
	glutReshapeFunc(CambiaDim);
	glutKeyboardFunc(Azione);
	glutDisplayFunc(DisegnaTutto);
	
	glutMainLoop();
	
	return(0);
}

