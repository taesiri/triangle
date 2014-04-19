
import  derelict.glfw3.glfw3;
import derelict.opengl3.gl;
import std.exception, std.stdio, std.math;



auto alpha = 0.0f,
     beta = 5.0f,
     zoom = 7.0f,
     gridLength = 3.0f,
     gridSubdivisions = 10,
     gridLineWidth = 1.0f,
     locked = GL_FALSE,
     cursorX = 0,
     cursorY = 0;


// Replaces gluPerspective. Sets the frustum to perspective mode.
// fovY     - Field of vision in degrees in the y direction
// aspect   - Aspect ratio of the viewport
// zNear    - The near clipping distance
// zFar     - The far clipping distance
// Source: http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar ) nothrow
{
  const GLdouble pi = 3.1415926535897932384626433832795;
  GLdouble fW, fH;
  fH = tan( fovY / 360 * pi ) * zNear;
  fW = fH * aspect;
  glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void main() {
    DerelictGL.load();
    DerelictGLFW3.load();

    glfwSetErrorCallback((code, msg){
      collectException(stderr.writeln(msg));
    });

    enforce(glfwInit());
    scope(exit) glfwTerminate();

    auto window = glfwCreateWindow(500, 500, "Triangle - D GLFW", null, null);
    enforce(window);
    scope(exit) glfwDestroyWindow(window);

    glfwMakeContextCurrent(window);

    DerelictGL.reload(); // Now OpenGL functions can be called.

    glfwSetKeyCallback(window, (win, key, scancode, action, mods){
      if (action != GLFW_PRESS)
        return;

      switch (key)
      {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(win, GL_TRUE);
            break;

        case GLFW_KEY_A:
            gridLength += 1;
            if(gridLength > 12)
                gridLength = 12;
            break;

        case GLFW_KEY_S:
            gridLength -= 1;
            if(gridLength < 2)
                gridLength = 2;
            break;

        case GLFW_KEY_V:
            gridSubdivisions += 2;
            if(gridSubdivisions > 16)
                gridSubdivisions = 16;
            break;

        case GLFW_KEY_B:
            gridSubdivisions -= 2;
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
            if (zoom < 0.0f)
                zoom = 0.0f;
            break;

        case GLFW_KEY_PAGE_DOWN:
            zoom += 0.25f;
            break;

        default:
            break;
      }
    });

    glfwSetFramebufferSizeCallback(window, (win, w, h){
      auto ratio = h > 0 ? w / cast(float)h : 1.0f;

      glViewport(0, 0, w, h);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      perspectiveGL(45.0f, ratio, 0.1f, 200.0f);
    });

    glfwSetMouseButtonCallback(window, (win, button, action, mods){
      if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

      if (action == GLFW_PRESS)
      {
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
      }
      else
      {
        locked = GL_FALSE;
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    });

    glfwSetCursorPosCallback(window, (win, x, y){
      if (locked)
      {
        alpha += cast(GLfloat) (x - cursorX) / 10.0f;
        beta += cast(GLfloat) (y - cursorY) / 10.0f;
      }

      cursorX = cast(int) x;
      cursorY = cast(int) y;
    });

    glfwSetScrollCallback(window, (win, x, y){
      zoom += cast(float) y / 4.0f;
      if (zoom < 0)
        zoom = 0;
    });

    void drawGrid() {
      glColor3f(0.2,1,0.2);

      glLineWidth(gridLineWidth);

      glBegin(GL_LINES);

      float steps= gridLength/gridSubdivisions;

      for (float i = -gridLength; i < gridLength+steps - 0.001; i+= steps ) {
        glVertex3f(i, 0, gridLength);
        glVertex3f(i, 0, -gridLength);

        glVertex3f(gridLength, 0, i);
        glVertex3f(-gridLength, 0, i);
      }
      glEnd();
    }

    void drawScene() {
      float ratio;
      int width, height;

      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / cast(float) height;

      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      perspectiveGL(45.0, ratio, .1f , 200.0f);

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

    }

    while (!glfwWindowShouldClose(window)) {
      drawScene();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
}
