/* */#include <stdio.h>#include <stdlib.h>#include <time.h>#include <math.h>#include "GL/glut.h"void CambiaDim(int, int);void DisegnaTutto();void AzioneTasto(unsigned char Tasto, int x, int y);void Niente();void CambiaDim(int w, int h){	glViewport(0, 0, w, h);		// matrice	glMatrixMode(GL_PROJECTION);	glLoadIdentity();	glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 1000);}#define NUM_TRI	100float X[NUM_TRI];float Z[NUM_TRI];static void InitTri(){	int i;		for(i = 0;i < NUM_TRI;i++) {		X[i] = -100 + (rand() % 200);		Z[i] = -100 + (rand() % 200);	}}static void DisegnaTri(){	int i;		glBegin(GL_TRIANGLES);	for(i = 0;i < NUM_TRI;i++) {		glVertex3f(X[i], -1, Z[i]);		glVertex3f(X[i] + 1, -1, Z[i]);		glVertex3f(X[i], 1, Z[i]);	}	glEnd();}float OssX, OssZ, OssA;static void DisegnaGriglia(){	int i, j;		glBegin(GL_LINES);	for(i = -100;i < 100;i++) {		for(j = -100;j < 100;j++) {			glVertex3f(i, -1, j);			glVertex3f(i + 1, -1, j);			glVertex3f(i, -1, j);			glVertex3f(i, -1, j + 1);		}	}	glEnd();}float Alfa = 1.0f;void DisegnaTutto(){	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		glFogi(GL_FOG_MODE, GL_EXP);	GLfloat Col[4] = { 0.8f, 0.8f, 0.8f, 1.0f };	glFogfv(GL_FOG_COLOR, Col);	glFogf(GL_FOG_DENSITY, 0.1f);	glEnable(GL_FOG);		glMatrixMode(GL_MODELVIEW);	glLoadIdentity();	glRotatef(-OssA, 0, 1, 0);	glTranslatef(-OssX, 0, -OssZ);		glColor3f(0, 1, 0);	DisegnaGriglia();		glColor4f(1, 0, 0, Alfa);	DisegnaTri();		glutSwapBuffers();}void AzioneTasto(unsigned char Tasto, int, int){	switch(Tasto) {		case 'w':			OssZ = OssZ - 0.2f * cos(OssA / 57.3f);			OssX = OssX - 0.2f * sin(OssA / 57.3f);		break;		case 'z':			OssZ = OssZ + 0.2f * cos(OssA / 57.3f);			OssX = OssX + 0.2f * sin(OssA / 57.3f);		break;					case 'a': OssA += 10; break;		case 's': OssA -= 10; break;				case 'f': OssX -= 0.2; break;		case 'g': OssX += 0.2; break;				case 't': OssZ -= 0.2; break;		case 'v': OssZ += 0.2; break;				case '1': Alfa -= 0.1; break;		case '2': Alfa += 0.1; break;			}		glutPostRedisplay();}int main(int argc, char **argv){	glutInit(&argc, argv);		glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);	glutInitWindowPosition(100, 100);	glutInitWindowSize(450, 450);	glutCreateWindow("GL Fog");		glEnable(GL_DEPTH_TEST);	glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		glutReshapeFunc(CambiaDim);	glutDisplayFunc(DisegnaTutto);	glutKeyboardFunc(AzioneTasto);		InitTri();		glutMainLoop();		return(0);}