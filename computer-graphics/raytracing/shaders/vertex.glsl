#version 130

uniform float aspect;
uniform vec3 cameraPosition;

attribute vec3 vPosition;
varying vec3 origin, direction;

void main() {
    gl_Position = vec4(vPosition, 1.0);
    direction = normalize(vec3(vPosition.x*aspect, vPosition.y, -1.0 ));
    origin = cameraPosition;
}
