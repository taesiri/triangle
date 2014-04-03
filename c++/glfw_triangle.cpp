#include <GLFW/glfw3.h>
#include <OPENGL/glu.h>
#include <stdlib.h>
#include <stdio.h>


GLfloat alpha = 0.0f, beta = 5.0f;
GLfloat zoom = 7.f;
GLboolean locked = GL_FALSE;

int cursorX;
int cursorY;

float gridLength = 3;
float gridSubdivisions = 10;
GLfloat gridLineWidth = 1.0;


//========================================================================
// Draw Grid
//========================================================================

void drawGrid() {
    glColor3f(.2,1,.2);

    glLineWidth(gridLineWidth);

    glBegin(GL_LINES);

    float steps= gridLength/gridSubdivisions;

    for (float i = -gridLength; i < gridLength+steps - .001; i+= steps ) {
        glVertex3f(i, 0, gridLength);
        glVertex3f(i, 0, -gridLength);

        glVertex3f(gridLength, 0, i);
        glVertex3f(-gridLength, 0, i);
    }
    glEnd();
}


//========================================================================
// Draw Scene
//========================================================================

void drawScene(GLFWwindow* window) {
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, ratio, .1f , 200.0f);

    glBegin(GL_TRIANGLES);
    glColor3f(.9,.1,.1);
    glVertex3f(0, 2.2, 0);
    glColor3f(.1,.1,.9);
    glVertex3f(-1, 0.2, 0);
    glColor3f(.1,.9,.1);
    glVertex3f(1, 0.2, 0);
    glEnd();

    drawGrid();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,-1,-zoom);

    glRotatef(beta, 1.0, 0.0, 0.0);
    glRotatef(alpha, 0.0, 1.0, 0.0);

    glfwSwapBuffers(window);
    glfwPollEvents();

}


//========================================================================
// Error Handling
//========================================================================

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

//========================================================================
// Handle key strokes
//========================================================================

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_A:
            gridLength+=1;
            if(gridLength > 12)
                gridLength = 12;
            break;
        case GLFW_KEY_S:
            gridLength-=1;
            if(gridLength < 2)
                gridLength = 2;
            break;

        case GLFW_KEY_V:
            gridSubdivisions+=2;
            if(gridSubdivisions > 16)
                gridSubdivisions = 16;
            break;

        case GLFW_KEY_B:
             gridSubdivisions-=2;
            if(gridSubdivisions < 2)
                gridSubdivisions = 2;

            break;
        case GLFW_KEY_LEFT:
            alpha += 5;
            break;
        case GLFW_KEY_RIGHT:
            alpha -= 5;
            break;
        case GLFW_KEY_UP:
            beta -= 5;
            break;
        case GLFW_KEY_DOWN:
            beta += 5;
            break;
        case GLFW_KEY_PAGE_UP:
            zoom -= 0.25f;
            if (zoom < 0.f)
                zoom = 0.f;
            break;
        case GLFW_KEY_PAGE_DOWN:
            zoom += 0.25f;
            break;
        default:
            break;
    }

}

//========================================================================
// Callback function for mouse button events
//========================================================================

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
    }
    else
    {
        locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================

void cursor_position_callback(GLFWwindow* window, double x, double y) {
    if (locked)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;
    }

    cursorX = (int) x;
    cursorY = (int) y;
}


//========================================================================
// Callback function for scroll events
//========================================================================

void scroll_callback(GLFWwindow* window, double x, double y) {
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;

    if (height > 0)
        ratio = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, .1f , 200.0f);

}

int main(int argc, char** argv) {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(500, 500, "Traingle - C++ GLFW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);


    while (!glfwWindowShouldClose(window))
    {

        drawScene(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
