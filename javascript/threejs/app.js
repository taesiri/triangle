
var alpha = 0.0,
    beta = 0.05,
    zoom = 7.0,
    gridLength = 3.0,
    gridSubdivisions = 10,
    gridLineWidth = 1.0,
    locked = false,
    cursorX = 0,
    cursorY = 0;

var scene = new THREE.Scene();
var aspect = window.innerWidth / window.innerHeight;
var camera = new THREE.PerspectiveCamera(45, aspect, 0.1, 200);
var clock = new THREE.Clock();

var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

var oldX = 0, oldY = 0;
window.addEventListener('mousemove', function(e) {
  if(locked){
    alpha -= (oldX - e.clientX) / 100.0;
    beta -= (oldY - e.clientY) / 100.0;
    oldX = e.clientX;
    oldY = e.clientY;
  }
});
scene.matrixAutoUpdate = true;
window.addEventListener('mousedown', function(e){
  locked = true;
  oldX = e.clientX;
  oldY = e.clientY;
});
window.addEventListener('mouseup', function(){
  locked = false;
});
window.addEventListener('wheel', function(e){
  zoom += e.deltaY / 20.0;
  e.preventDefault();
});

var geometry = new THREE.Geometry();

geometry.colors.push(new THREE.Color(0.9, 0.1, 0.1));
geometry.vertices.push(new THREE.Vector3(0, 2.2, 0));

geometry.colors.push(new THREE.Color(0.1, 0.1, 0.9));
geometry.vertices.push(new THREE.Vector3(-1, 0.2, 0));

geometry.colors.push(new THREE.Color(0.1, 0.9, 0.1));
geometry.vertices.push(new THREE.Vector3(1, 0.2, 0));

geometry.faces.push(new THREE.Face3(0, 1, 2, null, geometry.colors));

var material = new THREE.MeshBasicMaterial({
  vertexColors: THREE.VertexColors
});
material.side = THREE.DoubleSide;

var triangle = new THREE.Mesh(geometry, material);
scene.add(triangle);

var size = gridLength;
var step = size/gridSubdivisions;
var gridHelper = new THREE.GridHelper(size, step);

gridHelper.position = new THREE.Vector3(0, 0, 0);
gridHelper.rotation = new THREE.Euler(0, 0, 0);
gridHelper.color = new THREE.Color(0.2, 1, 0.2);

scene.add(gridHelper);

camera.position.x = 0;
camera.position.y = 1;

// camera.rotateOnAxis(new THREE.Vector3(1,0,0), beta * 2 * Math.Pi / 360.0);
// camera.rotateOnAxis(new THREE.Vector3(0,1,0), alpha * 2 * Math.Pi / 360.0);

function render() {
  scene.rotation.x = beta;
  scene.rotation.y = alpha;
  camera.position.z = zoom;
  requestAnimationFrame(render);
  triangle.rotation.y += clock.getDelta();
  renderer.render(scene, camera);
}
render();
