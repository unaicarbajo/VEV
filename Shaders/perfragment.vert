#version 120

attribute vec3 v_position;
attribute vec3 v_normal;
attribute vec2 v_texCoord;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

// NO NORMALIZAR ESTOS VECTORES, no sería correcto (Ej: escalado)
varying vec3 f_position;		// postionEye (posicion del vértice en el SRC)
varying vec3 f_viewDirection;	// V vector que va del vértice a la cámara en el SRC
varying vec3 f_normal;			// normalEye (normal del vértice en el SRC)
varying vec2 f_texCoord;

// NO hace falta calcular interpolación para el cálculo de las normales en píxel,
// el propio motor de OpenGl se ocupa de ello: Lo realiza de dos formas distintas.

void main() {	
	// Hago cambio al sistema de referencia de la cámara los siguientes 3 vectores
	f_position =  (modelToCameraMatrix * vec4(v_position,1.0)).xyz;
	f_viewDirection = (modelToCameraMatrix * vec4((-1.0)*f_position,1)).xyz;
	f_normal = (modelToCameraMatrix * vec4(v_normal,0.0)).xyz;
	
	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);
	f_texCoord = v_texCoord;

}
