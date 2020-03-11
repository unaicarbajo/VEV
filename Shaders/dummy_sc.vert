#version 120

uniform mat4 modelToCameraMatrix; // M
uniform mat4 cameraToClipMatrix;  // P

attribute vec3 v_position;

varying vec4 f_color;

uniform float sc;
void main() {

	f_color = vec4(sc,1,1,1);
	vec3 pos = v_position + sc;
	gl_Position = cameraToClipMatrix * modelToCameraMatrix * vec4(pos, 1);

}
