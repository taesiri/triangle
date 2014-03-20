#include <iostream>
#include <GLUT/GLUT.h>

#define GLUT_KEY_ESCAPE 27

const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;

const char* windowName = "Traingle - C++ GLUT";

GLfloat rotation_x = 0.0f;
GLfloat rotation_y = 0.0f;

bool keys[255];

GLfloat speed = 1.0f;


GLvoid initProjectionMatrix(GLsizei width, GLsizei height) {
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 0.1f, 200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height){
    initProjectionMatrix(width,height);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.,0.,0.,1.);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

GLvoid display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-3.5);

    glRotatef(rotation_x,1,0,0);
    glRotatef(rotation_y,0,1,0);

    glBegin(GL_TRIANGLES);
    glVertex3f(0.,1.,0.);
    glVertex3f(1.,-1.,0.);
    glVertex3f(-1.,-1.,0.);
    glEnd();

    glFlush();
    glutSwapBuffers();
}


GLboolean checkKeys(){
    if(keys[GLUT_KEY_ESCAPE]){
        return true;
    }

    return false;
}

GLvoid timerLoop(int value){
    if(checkKeys()){
        exit(0);
    }

    glutPostRedisplay();
    glutTimerFunc(1, timerLoop, 0);

    rotation_y += speed;

}

GLvoid keyboardDownCallback(unsigned char key,int x,int y){
    keys[key] = true;
}

GLvoid keyboardUpCallback(unsigned char key,int x,int y){
    keys[key] = false;
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow(windowName);

    initGL(windowWidth,windowHeight);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboardDownCallback);
    glutKeyboardUpFunc(keyboardDownCallback);

    glutTimerFunc(1,timerLoop,0);
    glutMainLoop();

    return 0;
}
