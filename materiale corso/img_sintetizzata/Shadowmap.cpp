/** Shadow Maps
 **/

#include "GL/glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DimX, DimY;

static void CambiaDim(int w, int h)
{
	DimX = w;
	DimY = h;
	glViewport(0, 0, w, h);
}


GLfloat LuceProMatr[16];
GLfloat LucePosMatr[16];

GLfloat BiasMatr[16] = {
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f
};
GLfloat Riga[4];	

GLfloat LucePos[4] = { 3, 9, 3, 1 };
GLfloat TelecPos[4] = { 0, 0, 10, 1 };

static GLfloat aMat[] = { 0.4f, 0.4f, 0.4f, 1.0f };
static GLfloat dMat[] = { 0.4f, 0.4f, 0.4f, 1.0f };
static GLfloat sMat[] = { 0.1f, 0.1f, 0.1f, 1.0f };

static GLfloat aMat2[] = { 0.8f, 0.1f, 0.1f, 1.0f };
static GLfloat dMat2[] = { 0.8f, 0.1f, 0.1f, 1.0f };
static GLfloat sMat2[] = { 0.1f, 0.1f, 0.1f, 1.0f };

static GLfloat aMat3[] = { 0.1f, 0.8f, 0.1f, 1.0f };
static GLfloat dMat3[] = { 0.1f, 0.8f, 0.1f, 1.0f };
static GLfloat sMat3[] = { 0.1f, 0.1f, 0.1f, 1.0f };


static GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat aLite[] = { 80.0f / 255.0f, 90.0f / 255.0f, 110.0f / 255.0f, 1.0f };
static GLfloat dLite[] = { 175.0f / 255.0f, 165.0f / 255.0f, 145.0f / 255.0f, 1.0f };
static GLfloat sLite[] = { 255.0f / 255.0f, 250.0f / 255.0f, 240.0f / 255.0f, 1.0f };

static GLuint TextureShadow;

#define DIM_TEXT	512


GLfloat PosX = 2;
GLfloat PosZ = 2;

bool Tutto = true;

static void DisegnaMondo()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, aMat2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dMat2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sMat2);
	glMateriali(GL_FRONT, GL_SHININESS, 32);
	
	//glPushMatrix();
	//glTranslatef(0, 0, -4);
	glutSolidSphere(3, 50, 50);
	//glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, aMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sMat);
	
	glPushMatrix();
	glTranslatef(PosX, 3, PosZ);
	
	glutSolidSphere(0.5f, 50, 50);
	
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.5, -1, 3);
	glutSolidCube(1);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, aMat3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dMat3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sMat3);
	
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5, -3, 5);
	glNormal3f(0, 1, 0);
	glVertex3f(5, -3, 5);
	glNormal3f(0, 1, 0);
	glVertex3f(5, -3, -5);
	glNormal3f(0, 1, 0);
	glVertex3f(-5, -3, -5);
	glEnd();
}


static void DisegnaTutto()
{

	// primo passo: disegno la scena dal punto di vista della luce
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 100);
	glGetFloatv(GL_PROJECTION_MATRIX, LuceProMatr);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(LucePos[0], LucePos[1], LucePos[2], 0, 0, 0, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, LucePosMatr);
	
	glViewport(0, 0, DIM_TEXT, DIM_TEXT);
	
	glCullFace(GL_FRONT);		// inverto, disegno il dietro
	glDisable(GL_LIGHTING);
	glColorMask(0, 0, 0, 0);
	
	DisegnaMondo();
	
	glBindTexture(GL_TEXTURE_2D, TextureShadow);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, DIM_TEXT, DIM_TEXT);

	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glColorMask(1, 1, 1, 1);
	
	
	// secondo passo: disegno la scena dal punto di vista della telecamera
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// telecamera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1.5, 200);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	gluLookAt(TelecPos[0], TelecPos[1], TelecPos[2], 0, 0, 0, 0, 1, 0);
	
	glViewport(0, 0, DimX, DimY);
	
	// luce - solo ambientale
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, black);		// BLACK - niente contributo diffuso
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);		// BLACK - niente contributo speculare
	glLightfv(GL_LIGHT0, GL_POSITION, LucePos);
	
	// disegno
	DisegnaMondo();
	
	
	// terzo passo: disegno le zone illuminate
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	
	// costruzione della matrice di proiezione sulla shadowmap
	glPushMatrix();
	glLoadMatrixf(BiasMatr);
	glMultMatrixf(LuceProMatr);
	glMultMatrixf(LucePosMatr);
	glGetFloatv(GL_MODELVIEW_MATRIX, LucePosMatr);
	glPopMatrix();
	
	// generazione delle coordinate di shadowmap
	Riga[0] = LucePosMatr[0]; Riga[1] = LucePosMatr[4]; Riga[2] = LucePosMatr[8]; Riga[3] = LucePosMatr[12]; 
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, Riga);
	glEnable(GL_TEXTURE_GEN_S);

	Riga[0] = LucePosMatr[1]; Riga[1] = LucePosMatr[5]; Riga[2] = LucePosMatr[9]; Riga[3] = LucePosMatr[13]; 
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, Riga);
	glEnable(GL_TEXTURE_GEN_T);

	Riga[0] = LucePosMatr[2]; Riga[1] = LucePosMatr[6]; Riga[2] = LucePosMatr[10]; Riga[3] = LucePosMatr[14]; 
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, Riga);
	glEnable(GL_TEXTURE_GEN_R);

	Riga[0] = LucePosMatr[3]; Riga[1] = LucePosMatr[7]; Riga[2] = LucePosMatr[11]; Riga[3] = LucePosMatr[15]; 
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, Riga);
	glEnable(GL_TEXTURE_GEN_Q);
	
	glBindTexture(GL_TEXTURE_2D, TextureShadow);
	glEnable(GL_TEXTURE_2D);
	
	// scartiamo il fragment se fallisce il test con la shadowmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LESS);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_ALPHA);

	glAlphaFunc(GL_GEQUAL, 0.5f);
	glEnable(GL_ALPHA_TEST);
	
	// disegno - lo switch 'Tutto' permette di vedere il contributo della luce
	if(Tutto) DisegnaMondo();
	
	glDisable(GL_ALPHA_TEST);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	
	glFlush();
	
}

static void Azione(unsigned char key, int , int )
{
	if(key == 'w') PosZ -= 0.1f;
	if(key == 'z') PosZ += 0.1f;
	if(key == 'a') PosX -= 0.1f;
	if(key == 's') PosX += 0.1f;
	
	if(key == 'r') LucePos[2] -= 0.1f;
	if(key == 'c') LucePos[2] += 0.1f;
	if(key == 'd') LucePos[0] -= 0.1f;
	if(key == 'f') LucePos[0] += 0.1f;
	
	if(key == 'u') TelecPos[2] -= 0.1f;
	if(key == 'n') TelecPos[2] += 0.1f;
	if(key == 'h') TelecPos[0] -= 0.1f;
	if(key == 'j') TelecPos[0] += 0.1f;
	
	if(key == ' ') Tutto = !Tutto;
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(550, 550);
	glutCreateWindow("Shadowmap");	
	
	glClearColor(1, 1, 1, 1);
	glColor3f(0, 0, 0);
	
	// Z buffer
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// luci
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	
	glEnable(GL_LIGHT0);
	
	// ombra
	glGenTextures(1, &TextureShadow);
	glBindTexture(GL_TEXTURE_2D, TextureShadow);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIM_TEXT, DIM_TEXT, 0,
					GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// glut
	glutReshapeFunc(CambiaDim);
	glutKeyboardFunc(Azione);
	glutDisplayFunc(DisegnaTutto);
	
	glutMainLoop();
	
	return(0);
}
