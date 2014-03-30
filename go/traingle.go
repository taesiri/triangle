package main

import (
    "fmt"
    "github.com/go-gl/gl"
    "github.com/go-gl/glu"
    glfw "github.com/go-gl/glfw3"
     )

const (
	Title  = "Triangle - GO GLFW"
	Width  = 500
	Height = 500
  )

var (
    rotation_x, rotation_y float32
  )

func errorCallback(err glfw.ErrorCode, desc string) {
      fmt.Printf("%v: %v\n", err, desc)
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

	window.MakeContextCurrent()

	glfw.SwapInterval(1)

	gl.Init()

  initScene();

	for !window.ShouldClose() {
		displayScene()
		window.SwapBuffers()
		glfw.PollEvents()
	}

}

func initProjectionMatrix(){
  gl.Viewport(0,0,Width,Height)
  gl.MatrixMode(gl.PROJECTION)
  gl.LoadIdentity()
  glu.Perspective(45.,float64(Width)/float64(Height), .1, 200.)
}

func initScene() {
  initProjectionMatrix()

  gl.ShadeModel(gl.SMOOTH)
  gl.ClearColor(0.,0.,0.,1.)
  gl.Enable(gl.DEPTH_TEST)
  gl.DepthFunc(gl.LEQUAL)
}

func displayScene() {
  gl.Clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT)

  gl.MatrixMode(gl.MODELVIEW)
  gl.LoadIdentity()
  gl.Translatef(0,0,-3.5)

  rotation_y +=1

  gl.Rotatef(rotation_x,1,0,0)
  gl.Rotatef(rotation_y,0,1,0)

  gl.Begin(gl.TRIANGLES)
    gl.Vertex3f(0,1,0)
    gl.Vertex3f(1,-1,0)
    gl.Vertex3f(-1,-1,0)
  gl.End()

  return
}
