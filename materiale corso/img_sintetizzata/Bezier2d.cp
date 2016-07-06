/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "GL/glut.h"


GLint nNumPoints = 4;
GLfloat ctrlPoints[4][3] = { 	{ -4.0f, 0.0f, 0.0f },
				{ -6.0f, 4.0f, 0.0f },
				{ 6.0f, -4.0f, 0.0f },
				{ 4.0f, 0.0f, 0.0f } };

void CambiaDim(int, int);
void DisegnaTutto();
void AzioneTasto(unsigned char Tasto, int x, int y);

void CambiaDim(int w, int h)
{
	glViewport(0, 0, w, h);
	
	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, 80, -80);
}

void DisegnaTutto()
{
	int i;
	
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0, 0, 0);
	
	// per visualizzare i punti di controllo
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(i = 0;i < nNumPoints;i++) glVertex3fv(ctrlPoints[i]);
	glEnd();
	
	// potrebbe essere impostata da fuori
	glMap1f(GL_MAP1_VERTEX_3, 0.0f, 100.0f, 3, nNumPoints, &ctrlPoints[0][0]);
			
	// abilita l'evaluator
	glEnable(GL_MAP1_VERTEX_3);
	
	//glMapGrid1d(100, 0.0, 100.0);
	//glEvalMesh1(GL_LINE, 0, 100);
	
	// creiamo una line-strip
	glBegin(GL_LINE_STRIP);
	for(i = 0;i <= 100;i++) {
		glEvalCoord1f((GLfloat) i);
	}
	glEnd();
	
	
	glutSwapBuffers();
}

void AzioneTasto(unsigned char Tasto, int, int)
{
	
	switch(Tasto) {
		case 'o': ctrlPoints[1][0] += 1; break;
		case 'p': ctrlPoints[1][0] -= 1; break;
	}
	
	
	glutPostRedisplay();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(450, 450);
	glutCreateWindow("Bezier 2d");
	
	glutReshapeFunc(CambiaDim);
	glutDisplayFunc(DisegnaTutto);
	glutKeyboardFunc(AzioneTasto);
	
	glutMainLoop();
	
	return(0);
}

