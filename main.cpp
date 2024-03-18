#include "includes.h"
#include "Camera.h"
#include "Box.h"


#define WIN_WIDTH 512
#define WIN_HEIGHT 512
#define FPS 30
#define UPDATE_OBJ 1000

int window; 

GLint winWidth, winHeight;
Camera cam;
Box box;

void resizeCB(int width, int height) {
	/* use the full view port */
	glViewport(0, 0, width, height);
	winWidth = width;
	winHeight = height;
	/* use 2D  orthographic parallel projection  */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
}

void renderCB(void) {
	//glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	Matrix4f viewMat = Matrix4f::transpose(cam.getViewMatrix(NULL));
	glLoadMatrixf((GLfloat*)viewMat.data());

	glMatrixMode(GL_PROJECTION);
	Matrix4f projMat = Matrix4f::transpose(cam.getProjectionMatrix(NULL));
	glLoadMatrixf((GLfloat*)projMat.data());

	box.render();
	glutSwapBuffers();
}

void keyboardCB(unsigned char key, int x, int y) {
	//std::cout << "Key Pressed: " << key << std::endl;
	printf("Key Pressed: %c\n", key);
}

void specialCB(int key, int x, int y) {
	//std::cout << "Special Key Pressed: " << key << std::endl;
	printf("Special Key Pressed");
}

void passiveMouseCB(int x, int y) {
	return;
}

void timerCB(int op) { return; }

int main(int argc, char* argv[]) {
	winWidth = WIN_WIDTH;
	winHeight = WIN_HEIGHT;

	glutInit(&argc, argv);
	glutInitWindowPosition(10, 10);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winWidth, winHeight);
	window = glutCreateWindow("3009 Project");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glutReshapeFunc(resizeCB);
	glutDisplayFunc(renderCB);
	glutKeyboardFunc(keyboardCB);
	glutSpecialFunc(specialCB);
	glutPassiveMotionFunc(passiveMouseCB);

	glutTimerFunc(FPS, timerCB, UPDATE_OBJ);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	cam.setCamera(Vector3f(30, 30, 100), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	box.initGeom();
	box.setModelScale(12, 12, 12);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error - %s \n", glewGetErrorString(res));
		return (-1);
	}

	glutMainLoop();

	return 0;
}

