/* */#include <stdio.h>#include <stdlib.h>#include <time.h>#include <math.h>#include "GL/glut.h"void CambiaDim(int, int);void DisegnaTutto();void Azione(unsigned char key, int x, int y);void DisegnaCubo(GLfloat R);float AngoloX = 0;float AngoloY = 0;GLubyte Texture1[512 * 512 * 3];GLubyte Texture2[512 * 512 * 3];void DisegnaCubo(GLfloat R){	glBegin(GL_QUADS);		glNormal3f(0, 0, 1);	glTexCoord2f(1, 1);	glVertex3f(R, R, R);	glTexCoord2f(0, 1);	glVertex3f(-R, R, R);	glTexCoord2f(0, 0);	glVertex3f(-R, -R, R);	glTexCoord2f(1, 0);	glVertex3f(R, -R, R);		glNormal3f(0, 0, -1);	glTexCoord2f(1, 1);	glVertex3f(-R, R, -R);	glTexCoord2f(0, 1);	glVertex3f(R, R, -R);	glTexCoord2f(0, 0);	glVertex3f(R, -R, -R);	glTexCoord2f(1, 0);	glVertex3f(-R, -R, -R);		glNormal3f(1, 0, 0);	glTexCoord2f(1, 1);	glVertex3f(R, R, -R);	glTexCoord2f(0, 1);	glVertex3f(R, R, R);	glTexCoord2f(0, 0);	glVertex3f(R, -R, R);	glTexCoord2f(1, 0);	glVertex3f(R, -R, -R);		glNormal3f(-1, 0, 0);	glTexCoord2f(1, 1);	glVertex3f(-R, R, R);	glTexCoord2f(0, 1);	glVertex3f(-R, R, -R);	glTexCoord2f(0, 0);	glVertex3f(-R, -R, -R);	glTexCoord2f(1, 0);	glVertex3f(-R, -R, R);		glNormal3f(0, 1, 0);	glTexCoord2f(1, 1);	glVertex3f(R, R, -R);	glTexCoord2f(0, 1);	glVertex3f(-R, R, -R);	glTexCoord2f(0, 0);	glVertex3f(-R, R, R);	glTexCoord2f(1, 0);	glVertex3f(R, R, R);		glNormal3f(0, -1, 0);	glTexCoord2f(1, 1);	glVertex3f(-R, -R, -R);	glTexCoord2f(0, 1);	glVertex3f(R, -R, -R);	glTexCoord2f(0, 0);	glVertex3f(R, -R, R);	glTexCoord2f(1, 0);	glVertex3f(-R, -R, R);		glEnd();}void CambiaDim(int w, int h){	glViewport(0, 0, w, h);}GLfloat PosLite[4] = { 2, 2, 2, 1 };void DisegnaTutto(){	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// matrici	glMatrixMode(GL_PROJECTION);	glLoadIdentity();	glFrustum(-1, 1, -1, 1, 2, 200);	glMatrixMode(GL_MODELVIEW);	glLoadIdentity();		// luci	GLfloat aLite[4] = { 0.0, 0.0, 0.0, 1 };	GLfloat dLite[4] = { 1.0, 1.0, 1.0, 1 };	GLfloat sLite[4] = { 0, 0, 0, 1 };		GLfloat d2Lite[4] = { 0.0, 0.0, 1.0, 1 };	GLfloat Pos2Lite[4] = { -2, 2, 2, 1 };		glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);		glLightfv(GL_LIGHT1, GL_AMBIENT, aLite);	glLightfv(GL_LIGHT1, GL_DIFFUSE, d2Lite);	glLightfv(GL_LIGHT1, GL_SPECULAR, sLite);		glLightfv(GL_LIGHT0, GL_POSITION, PosLite);		glLightfv(GL_LIGHT1, GL_POSITION, Pos2Lite);		//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0f);	//glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0f);		// materiale	GLfloat ambiente[4] = { 1, 1, 1, 1 };	GLfloat direttiva[4] = { 1, 1, 1, 1 };	GLfloat brillante[4] = { 0, 0, 0, 1 };		glMateriali(GL_FRONT, GL_SHININESS, 32);		glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);	glMaterialfv(GL_FRONT, GL_DIFFUSE, direttiva);	glMaterialfv(GL_FRONT, GL_SPECULAR, brillante);		// Texture	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture1);	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, Texture1);	glEnable(GL_TEXTURE_2D);		// oggetto	glTranslatef(0, -0.5f, -3);		glRotatef(AngoloX, 1, 0, 0);	glRotatef(AngoloY, 0, 1, 0);		DisegnaCubo(0.5f);		glEnd();		glutSwapBuffers();    }void Azione(unsigned char a, int , int ){	if(a == 'a') AngoloY += 10;	if(a == 's') AngoloY -= 10;		if(a == 'w') AngoloX += 10;	if(a == 'z') AngoloX -= 10;		if(a == 'y') PosLite[2] += 0.5f;	if(a == 'b') PosLite[2] -= 0.5f;		glutPostRedisplay();}int main(int argc, char **argv){	glutInit(&argc, argv);	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);	glutInitWindowPosition(100, 100);	glutInitWindowSize(450, 450);	glutCreateWindow("LightStudio + Texture");		glEnable(GL_DEPTH_TEST);	glClearColor(0, 0, 0, 1.0f);	glEnable(GL_CULL_FACE);		glEnable(GL_LIGHTING);	glEnable(GL_LIGHT0);	glEnable(GL_LIGHT1);		GLfloat black[4] = { 0, 0, 0, 1 };		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);		{		FILE *fHan = fopen("legno.raw", "rb");		if(fHan == NULL) return(0);		fread(Texture1, 512 * 512, 3, fHan);		fclose(fHan);	}		glutReshapeFunc(CambiaDim);	glutKeyboardFunc(Azione);	glutDisplayFunc(DisegnaTutto);		glutMainLoop();		return(0);}