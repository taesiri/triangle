#include <stdlib.h>

#include <SDL.h>
#define main SDL_main 

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined (__APPLE__) && defined(__MACH__)
#include <OpenGL\GL.h>
#include <OpenGL\GLU.h>
#else
#include <gl\GL.h>
#include <gl\GLU.h>
#endif

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;

GLfloat rotation_x = 0.0;
GLfloat rotation_y = 0.0f;

GLfloat speed = 1.0f;

Uint8* keys = NULL;

GLvoid initProjectionMatrix(GLsizei width, GLsizei height) {

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height) {

	initProjectionMatrix(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}


GLvoid display(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3.5);

	glRotatef(rotation_x, 1, 0, 0);
	glRotatef(rotation_y, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0., 1., 0.);
	glVertex3f(1., -1., 0.);
	glVertex3f(-1., -1., 0.);
	glEnd();

	glFlush();
	SDL_GL_SwapBuffers();
}




GLboolean checkKeys(GLvoid) {
	static long lastTime = SDL_GetTicks();

	const GLfloat speed = 1.0f;
	const long updateTime = 10;

	long newTime = SDL_GetTicks();

	if (newTime - lastTime > updateTime){
		if (keys[SDLK_ESCAPE])
			return true;

		if (keys[SDLK_LEFT])
			rotation_y -= speed;
		if (keys[SDLK_RIGHT])
			rotation_y += speed;

		if (keys[SDLK_DOWN])
			rotation_x -= speed;
		if (keys[SDLK_UP])
			rotation_x += speed;
	}
	return false;
}

GLvoid TimerLoop(){
	static long lastTime = SDL_GetTicks();

	const GLfloat speed = 1.0f;
	const long updateTime = 10;

	long newTime = SDL_GetTicks();

	if (newTime - lastTime > updateTime){
		rotation_y += speed;
	}
}


int main(int argc, char** args)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError());
		exit(1);
	}

	if (SDL_SetVideoMode(windowWidth, windowHeight, 0, SDL_OPENGL) == NULL) {
		fprintf(stderr, "Unable to create OpenGL scene: %s", SDL_GetError());
		exit(2);
	}

	initGL(windowWidth, windowHeight);

	int done = 0;

	while (!done) {

		display();

		SDL_Event event;

		while (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT) {
				done = 1;
			}

			keys = SDL_GetKeyState(NULL);
		}

		if (checkKeys())
			done = 1;

		TimerLoop();

	}

	SDL_Quit();

	return 1;
}