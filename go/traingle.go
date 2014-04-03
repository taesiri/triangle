package main

import (
    "fmt"
    "github.com/go-gl/gl"
    "github.com/go-gl/glu"
    glfw "github.com/go-gl/glfw3"
     )

const (
	Title  = "Triangle - GO GLFW"
	Width  = 1000
	Height = 1000
  )

var (
    cursorX, cursorY, alpha,beta int
    zoom, gridLength, gridSubdivisions, gridLineWidth float32
    locked bool
  )




//========================================================================
// Error Handling
//========================================================================

func errorCallback(err glfw.ErrorCode, desc string) {
      fmt.Printf("%v: %v\n", err, desc)
}

//========================================================================
// Handle key strokes
//========================================================================

func key_callback(window *glfw.Window, key glfw.Key, scancode int, action glfw.Action, mods glfw.ModifierKey) {
  if action != glfw.Press {
    return
  }

  switch glfw.Key(key) {

    case glfw.KeyEscape:
      window.SetShouldClose(true)

    case glfw.KeyA:
            gridLength += 1
            if gridLength > 12 {
                gridLength = 12
            }

    case glfw.KeyS:
        gridLength -= 1
        if gridLength < 2 {
            gridLength = 2
        }

    case glfw.KeyV:
        gridSubdivisions += 2
        if gridSubdivisions > 16 {
            gridSubdivisions = 16
        }

    case glfw.KeyB:
         gridSubdivisions -= 2
        if gridSubdivisions < 2 {
            gridSubdivisions = 2
          }

    case glfw.KeyLeft:
        alpha += 5

    case glfw.KeyRight:
        alpha -= 5

    case glfw.KeyUp:
        beta -= 5
        break;
    case glfw.KeyDown:
        beta += 5
        break;
    case glfw.KeyPageUp:
        zoom -= 0.25
        if zoom < 0. {
            zoom = 0.
        }
        break;
    case glfw.KeyPageDown:
        zoom += 0.25

    default:
      return
    }

}

//========================================================================
// Callback function for mouse button events
//========================================================================

func mouse_button_callback(window *glfw.Window, button glfw.MouseButton, action glfw.Action, mods glfw.ModifierKey) {
    if button != glfw.MouseButtonLeft{
        return
    }

    if action == glfw.Press {
        window.SetInputMode(glfw.Cursor, glfw.CursorDisabled)
        locked = true
    } else {
        locked = false
        window.SetInputMode(glfw.Cursor, glfw.CursorNormal)
    }
}


//========================================================================
// Callback function for cursor motion events
//========================================================================

func cursor_position_callback(window *glfw.Window , x float64, y float64) {
    if locked {
        alpha +=  int((x - float64(cursorX)) / 10.)
        beta +=  int((y - float64(cursorY)) / 10.)
    }

    cursorX = int(x)
    cursorY = int(y)
}


//========================================================================
// Callback function for scroll events
//========================================================================

func scroll_callback(window *glfw.Window , x float64,  y float64) {
    zoom += float32(y) / 4.
    if zoom < 0 {
        zoom = 0;
    }
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================

func framebuffer_size_callback(window *glfw.Window , width int, height int) {
    ratio := 1.;

    if height > 0{
        ratio = float64(width) / float64(height);
    }

    gl.Viewport(0, 0, width, height)
    gl.MatrixMode(gl.PROJECTION)
    gl.LoadIdentity()
    glu.Perspective(45.0, ratio, .1 , 200.0)
}



func main() {

  glfw.SetErrorCallback(errorCallback)

	if !glfw.Init() {
		panic("Can't init glfw!")
	}

	defer glfw.Terminate()

	window, err := glfw.CreateWindow(Width, Height, Title, nil, nil)
	if err != nil {
		panic(err)
	}

	window.SetKeyCallback(key_callback)
  window.SetMouseButtonCallback(mouse_button_callback)
  window.SetFramebufferSizeCallback(framebuffer_size_callback)
  window.SetCursorPositionCallback(cursor_position_callback)
  window.SetScrollCallback(scroll_callback)

	glfw.SwapInterval(1)


  window.MakeContextCurrent()


	gl.Init()

  initScene(window);


	for !window.ShouldClose() {
		displayScene()
		window.SwapBuffers()
		glfw.PollEvents()
	}

}

func initProjectionMatrix(window *glfw.Window) {
  width, height := window.GetFramebufferSize()
  ratio := float64(width) / float64(height);

  gl.Viewport(0,0,width,height)
  gl.MatrixMode(gl.PROJECTION)
  gl.LoadIdentity()
  glu.Perspective(45., ratio, .1, 200.)
}

func initScene(window *glfw.Window) {
  initProjectionMatrix(window)

  gl.ShadeModel(gl.SMOOTH)
  gl.ClearColor(0.,0.,0.,1.)
  gl.Enable(gl.DEPTH_TEST)
  gl.DepthFunc(gl.LEQUAL)

  gridLength = 3
  gridSubdivisions = 10
  gridLineWidth = 1.0
  zoom = 7.0
  locked = false
  beta = 5.
}

func drawGrid(){
  gl.Color3f(.2,1,.2)
  gl.LineWidth(gridLineWidth)

  gl.Begin(gl.LINES);

  steps := gridLength/gridSubdivisions;

  for i:= -gridLength; i < gridLength+steps - .001; i+=steps {
    gl.Vertex3f(i, 0, gridLength);
    gl.Vertex3f(i, 0, -gridLength);

    gl.Vertex3f(gridLength, 0, i);
    gl.Vertex3f(-gridLength, 0, i);
  }
  gl.End();
}

func displayScene() {
  gl.Clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT)

  gl.MatrixMode(gl.MODELVIEW)
  gl.LoadIdentity()

  gl.Translatef(0,0,-zoom)

  gl.Rotatef(float32(beta), 1.0, 0.0, 0.0);
  gl.Rotatef(float32(alpha), 0.0, 1.0, 0.0);

  gl.Begin(gl.TRIANGLES)
    gl.Color3f(.9,.1,.1)
    gl.Vertex3f(0, 2.2, 0)
    gl.Color3f(.1,.1,.9)
    gl.Vertex3f(-1, 0.2, 0)
    gl.Color3f(.1,.9,.1)
    gl.Vertex3f(1, 0.2, 0)
  gl.End();


  drawGrid()


}
