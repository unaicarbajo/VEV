#version 120

attribute vec3 v_position;
attribute vec3 v_normal;
attribute vec2 v_texCoord;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

uniform mat4 worldToShadowCameraClip;   // Matriz de cambio del sistema de coordenadas del mundo
                                        // al sistema de coordendas de la luz que genera sombras
                                        // (camara situada en dicha luz)

varying vec3 f_position;
varying vec3 f_viewDirection;
varying vec3 f_normal;
varying vec2 f_texCoord;

varying vec3 f_c;   // coordenadas del vertice en el espacio de la camara colocada
                    // en la posici´on de la fuente de luz

void main() {
	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);

	f_position =  (modelToCameraMatrix * vec4(v_position,1.0)).xyz;
	f_viewDirection = (-1.0)*f_position;
	f_normal = (modelToCameraMatrix * vec4(v_normal,0.0)).xyz;
	f_texCoord = v_texCoord;

    f_c = (worldToShadowCameraClip * modelToWorldMatrix * vec4(v_position, 1.0)).xyz;

}