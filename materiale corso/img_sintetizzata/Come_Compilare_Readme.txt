Gli esempi vanno compilati linkando le librerie:
OpenGL
GLU (normalmente e' parte integrante della OpenGL)
glut (o freeglut)

Ad esempio, su linux la compilazione va eseguita come:
g++ <file da compilare> -lGL -lGLU -lglut

Su OS X e' necessario linkare i framework OpenGL e GLUT

Su Windows e' necessario installare la freeglut e linkare con opengl32.dll, glu32.dll e (appunto) la dll della freeglut


