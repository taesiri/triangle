
from pyglet.gl import *
from pyglet.window import key


config = Config(sample_buffers=1, samples=4,
                depth_size=16, double_buffer=True)

window = pyglet.window.Window(width = 500, height=500,
        caption="Triangle - Python pyglet", config=config)
window.set_exclusive_mouse(True)

# to log all events
# window.push_handlers(pyglet.window.event.WindowEventLogger())


class Stats:
    def __init__(self, alpha, beta, zoom,
            gridLength, gridSubdivisions, gridLineWidth):
        self.alpha = alpha
        self.beta  = beta
        self.zoom  = zoom
        self.gridLength = gridLength
        self.gridSubdivisions = gridSubdivisions
        self.gridLineWidth = gridLineWidth
        self.time = 0

    def addGridLength(self, n):
        self.gridLength = self.gridLength + n
        if self.gridLength > 12:
            self.gridLength = 12
        if self.gridLength < 2:
            self.gridLength = 2

    def addGridSubdivision(self, n):
        self.gridSubdivision = self.gridSubdivision + n
        if self.gridSubdivision > 16:
            self.gridSubdivision = 16
        if self.gridSubdivision < 2:
            self.gridSubdivision = 2

    def addAlpha(self, n):
        self.alpha += n

    def addBeta(self, n):
        self.beta += n

    def addZoom(self, n):
        self.zoom += n
        if self.zoom < 0:
            self.zoom = 0

stats = Stats(0.0, 5.0, 7.0, 3.0, 10, 1.0)



def update(dt):
    stats.time += dt

pyglet.clock.schedule(update)

def drawGrid():
    steps = stats.gridLength / stats.gridSubdivisions

    glColor3f(0.2, 1, 0.2)
    glLineWidth(stats.gridLineWidth)

    glBegin(GL_LINES)

    i = -stats.gridLength
    while i <= stats.gridLength:
        glVertex3f(i, 0, stats.gridLength)
        glVertex3f(i, 0, -stats.gridLength)

        glVertex3f(stats.gridLength, 0, i)
        glVertex3f(-stats.gridLength, 0, i)

        i += steps

    glEnd()

@window.event
def on_draw():
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glTranslatef(0.0, -1.0, -stats.zoom)
    glRotatef(stats.beta, 1.0, 0.0, 0.0)
    glRotatef(stats.alpha, 0.0, 1.0, 0,0)

    drawGrid()

    glRotatef(stats.time * 100.0, 0.0, 1.0, 0.0)

    glBegin(GL_TRIANGLES)
    glColor3f(0.9, 0.1, 0.1)
    glVertex3f(0.0, 2.2, 0.0)
    glColor3f(0.1, 0.1, 0.9)
    glVertex3f(-1.0, 0.2, 0.0)
    glColor3f(0.1, 0.9, 0.1)
    glVertex3f(1.0, 0.2, 0)
    glEnd()

@window.event
def on_resize(width, height):
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, width / float(height), .1, 200)

    glEnable(GL_DEPTH_TEST)
    return pyglet.event.EVENT_HANDLED

@window.event
def on_key_press(symbol, modifiers):
    actions = {
            key.ESCAPE: lambda: window.close(),
            key.A:      lambda: stats.addGridLength(1),
            key.S:      lambda: stats.addGridLength(-1),
            key.V:      lambda: stats.addGridSubdivision(2),
            key.B:      lambda: stats.addGridSubdivision(-2),
            key.LEFT:   lambda: stats.addAlpha(5),
            key.RIGHT:  lambda: stats.addAlpha(-5),
            key.UP:     lambda: stats.addBeta(-5),
            key.DOWN:   lambda: stats.addBeta(5),
            key.PAGEUP: lambda: stats.addZoom(0.25),
            key.PAGEDOWN: lambda: stats.addZoom(-0.25)
            }

    if symbol in actions:
        actions[symbol]()

@window.event
def on_mouse_motion(x, y, dx, dy):
    stats.alpha += dx / 10.0
    stats.beta  += dy / 10.0

@window.event
def on_mouse_scroll(x, y, dx, dy):
    stats.addZoom(dy / 4.0)


if __name__ == '__main__':
    pyglet.app.run()
