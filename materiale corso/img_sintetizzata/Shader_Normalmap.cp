/*
 */

#define GL_GLEXT_PROTOTYPES 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "GL/glut.h"


void CambiaDim(int, int);
void DisegnaTutto();
void AzioneTasto(unsigned char Tasto, int x, int y);

// Texture

GLubyte Texture1[256 * 256 * 4];	// RGBA
GLubyte Texture2[256 * 256 * 4];

// Shader

const GLchar *SorgenteVertex =
"varying vec3 N;"
"varying vec3 T;"
"varying vec3 B;"
"varying vec4 O;"
"void main()"
"{"
"   gl_Position = ftransform();"
"   N = gl_NormalMatrix * gl_Normal;"
"   T = gl_NormalMatrix * gl_MultiTexCoord1.xyz;"
"   B = gl_NormalMatrix * gl_MultiTexCoord2.xyz;"
"   O = gl_ModelViewMatrix * gl_Vertex;"
"   gl_TexCoord[0] = gl_MultiTexCoord0;"
"}"
;

const GLchar *SorgenteFragm = 
"uniform sampler2D Text1;"
"uniform sampler2D Norm1;"
"varying vec3 N;"
"varying vec3 T;"
"varying vec3 B;"
"varying vec4 O;"
"void main()"
"{"
"   vec4 ca;"
"   vec4 cd;"
"   vec4 cs;"
"   vec4 t;"
"   vec3 n1;"
"   vec3 n2;"
"	vec3 n;"
"   ca = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;"
"   cd = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;"
"   cs = gl_FrontMaterial.specular * gl_LightSource[0].specular;"
"   t = texture2D(Text1, gl_TexCoord[0].st);"
"   n = texture2D(Norm1, gl_TexCoord[0].st).xyz - vec3(0.5);"
"   ca *= t;"
"   cd *= t;"
"   n1 = N * n.z;"
"   n1 += T * n.x;"
"   n1 += B * n.y;"
"   n1 = normalize(n1);"
"   n2 = normalize(gl_LightSource[0].position.xyz);"
"   cd *= max(dot(n1, n2), 0.0);"
"   ca += cd;"
"	n2 -= normalize(O.xyz);"
"	n2 = normalize(n2);"
"	float val = pow(max(dot(n1, n2), 0.0), 32.0);"
"	ca += val * cs;"
"	gl_FragColor = ca;"
"}"
;

GLhandleARB ShaderVertex;
GLhandleARB ShaderFragm;
GLhandleARB ShaderProg;

#define MAX_SHADER_LOG	2048
static GLchar ShaderLog[MAX_SHADER_LOG];

static bool CreaShaderVertex()
{
	GLint Ok;
	GLint PosTex;
	
	ShaderVertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	glShaderSourceARB(ShaderVertex, 1, (const GLchar **) &SorgenteVertex, NULL);
	glCompileShaderARB(ShaderVertex);
	glGetObjectParameterivARB(ShaderVertex, GL_OBJECT_COMPILE_STATUS_ARB, &Ok);
	glGetInfoLogARB(ShaderVertex, MAX_SHADER_LOG, &PosTex, ShaderLog);
	if(PosTex > 0) {
		printf("log vertex: %s\n", ShaderLog);
	}
	
	return(Ok == 1);
}

static bool CreaShaderFragm()
{
	GLint Ok;
	GLint PosTex;
	
	ShaderFragm = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	glShaderSourceARB(ShaderFragm, 1, (const GLchar **) &SorgenteFragm, NULL);
	glCompileShaderARB(ShaderFragm);
	glGetObjectParameterivARB(ShaderFragm, GL_OBJECT_COMPILE_STATUS_ARB, &Ok);
	glGetInfoLogARB(ShaderFragm, MAX_SHADER_LOG, &PosTex, ShaderLog);
	if(PosTex > 0) {
		printf("log fragment: %s\n", ShaderLog);
	}
	
	return(Ok == 1);
}

static bool LinkaProgram()
{
	GLint Ok;
	GLint PosTex;
	
	ShaderProg = glCreateProgramObjectARB();
	glAttachObjectARB(ShaderProg,ShaderVertex);
	glAttachObjectARB(ShaderProg,ShaderFragm);
	glLinkProgramARB(ShaderProg);
	glGetObjectParameterivARB(ShaderProg, GL_OBJECT_LINK_STATUS_ARB, &Ok);
	glGetInfoLogARB(ShaderProg, MAX_SHADER_LOG, &PosTex, ShaderLog);
	if(PosTex > 0) {
		printf("log link: %s\n", ShaderLog);
	}
	
	return(Ok == 1);
}

static void DisegnaCubo(GLfloat R)
{
	glBegin(GL_QUADS);
	
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1);
	// queste andrebbero ripetute per ogni faccia per definire correttamente tangente e bitangente
	glMultiTexCoord3fARB(GL_TEXTURE0 + 1, 1, 0, 0);
	glMultiTexCoord3fARB(GL_TEXTURE0 + 2, 0, 1, 0);
	glVertex3f(R, R, R);
	glTexCoord2f(0, 1);
	glVertex3f(-R, R, R);
	glTexCoord2f(0, 0);
	glVertex3f(-R, -R, R);
	glTexCoord2f(1, 0);
	glVertex3f(R, -R, R);
	
	glNormal3f(0, 0, -1);
	// per semplicita' non le definiamo, ma cosi' solo la prima faccia e' illuminata correttamente!
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

static GLfloat aMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat dMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat sMat[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static void DisegnaOggetto(float x, float y, float z, float a, float b)
{
	glTranslatef(x, y, z);
	glRotatef(a, 0, 1, 0);
	glRotatef(b, 0, 0, 1);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, aMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sMat);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	
	// metto le texture nelle texture units
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, 2);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 1);
	
	glUseProgramObjectARB(ShaderProg);
	DisegnaCubo(1);
	glUseProgramObjectARB(0);
}

void CambiaDim(int w, int h)
{
	glViewport(0, 0, w, h);
	
	// matrice
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, +1, -1, +1, 2, 200);
	
}

GLfloat A = 0, B = 0;

static GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat aLite[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLfloat dLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat sLite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat LucePos[4] = { 1, 1, 1, 0 };

void DisegnaTutto()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, aLite);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dLite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sLite);
	glLightfv(GL_LIGHT0, GL_POSITION, LucePos);

	DisegnaOggetto(0, 0, -5, A, B);
			
	glutSwapBuffers();
}

void AzioneTasto(unsigned char t, int, int)
{
	if(t == 'a') A += 10;
	if(t == 's') A -= 10;
	if(t == 'w') B += 10;
	if(t == 'z') B -= 10;
		
	glutPostRedisplay();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(450, 450);
	glutCreateWindow("Luce + Texture + Normalmap");
	
	{
		FILE *fHan = fopen("cassa.raw", "rb");
		if(fHan == NULL) return(0);
		fread(Texture1, 256 * 256, 4, fHan);
		fclose(fHan);
		
		fHan = fopen("cassa_norm2.raw", "rb");
		if(fHan == NULL) return(0);
		fread(Texture2, 256 * 256, 4, fHan);
		fclose(fHan);
	}
	
	// Z buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// luci
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	glEnable(GL_LIGHT0);
	
	// texture
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Texture1);
    
	glBindTexture(GL_TEXTURE_2D, 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Texture2);
	
	// shader
	if(!CreaShaderVertex()) return(0);
	if(!CreaShaderFragm()) return(0);
	if(!LinkaProgram()) return(0);
	
	// collegamento shader-texture
	glUseProgramObjectARB(ShaderProg);
	GLint p1 = glGetUniformLocationARB(ShaderProg, "Text1");
	if(p1 > -1) glUniform1i(p1, 0);
	else printf("Errore texture 1\n");
	p1 = glGetUniformLocationARB(ShaderProg, "Norm1");
	if(p1 > -1) glUniform1i(p1, 1);
	else printf("Errore texture 2\n");
	
	// callback
	glutReshapeFunc(CambiaDim);
	glutDisplayFunc(DisegnaTutto);
	glutKeyboardFunc(AzioneTasto);
	
	glutMainLoop();
	
	return(0);
}

