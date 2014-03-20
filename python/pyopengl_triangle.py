from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys

GL_KEY_ESCAPE = 27

windowName = "Triangle - PyOpenGL"

windowWidth = 500
windowHeight = 500

rotation_x = 0.
rotation_y = 0.

keys = [False] * 255

speed = 1.

def main():
  glutInit(sys.argv)

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
  glutInitWindowSize(windowWidth,windowHeight)
  glutCreateWindow(windowName)

  initGL(windowWidth,windowHeight)

  glutDisplayFunc(display)

  glutKeyboardFunc(keyboardDownCallback)
  glutKeyboardUpFunc(keyboardDownCallback)

  glutTimerFunc(1,timerLoop,0)
  glutMainLoop()
  return


def initProjectionMatrix(width,height):
  glViewport(0,0,width,height)
  glMatrixMode(GL_PROJECTION)
  glLoadIdentity()
  gluPerspective(45.,width/height, .1, 200.)
  return

def initGL(width,height):
  initProjectionMatrix(width,height)

  glShadeModel(GL_SMOOTH)
  glClearColor(0.,0.,0.,1.)
  glEnable(GL_DEPTH_TEST)
  glDepthFunc(GL_LEQUAL)
  return

def display():

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glTranslatef(0,0,-3.5)

    glRotatef(rotation_x,1,0,0)
    glRotatef(rotation_y,0,1,0)

    glBegin(GL_TRIANGLES)
    glVertex3f(0.,1.,0.)
    glVertex3f(1.,-1.,0.)
    glVertex3f(-1.,-1.,0.)
    glEnd()

    glFlush()
    glutSwapBuffers()
    return

def timerLoop(value):
  if(checkKeys()):
    sys.exit(0)

  glutPostRedisplay()
  glutTimerFunc(1, timerLoop, 0)

  global rotation_y
  rotation_y += speed

  return

def checkKeys():
  if(keys[GL_KEY_ESCAPE]):
    return True

  return False

def keyboardDownCallback(key, x, y):
  keys[ord(key)] = True
  return

def keyboardUpCallback(key, x, y):
  keys[ord(key)] = False
  return


# running Main
main()
