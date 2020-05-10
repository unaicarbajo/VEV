#version 120

// Bump mapping with many lights.

// all attributes in model space

// Fuentes:
// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
// https://fabiensanglard.net/bumpMapping/index.php
// http://www.opengl-tutorial.org/es/intermediate-tutorials/tutorial-13-normal-mapping/#normal-textures

// Se realiza en un espacio tangente, ya que para poder realizar bien todos los trabajos
// con luces puede haber problemas con las orientaciones de las normales del bump map,
// de manera que todas las luces se trabajaran sobre el mismo espacio de coordenadas,
// donde los vectores del bump map siempre estén apuntando al eje z (positivo). Habilitando
// el uso del bump map sin importar su orientación.

// Se mandan al fragment shader aquellas variables de luces importantes
// representadas en el espacio tangente

attribute vec3 v_position;
attribute vec3 v_normal;	// vector de normal
attribute vec2 v_texCoord;
attribute vec3 v_TBN_t;		// vector de tangente
attribute vec3 v_TBN_b;		// vector de bitangente

uniform mat4 modelToCameraMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToClipMatrix;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

// All bump computations are performed in tangent space; therefore, we need to
// convert all light (and spot) directions and view directions to tangent space
// and pass them the fragment shader.

varying vec2 f_texCoord;
varying vec3 f_viewDirection;     // tangent space
varying vec3 f_lightDirection[4]; // tangent space
varying vec3 f_spotDirection[4];  // tangent space

void main() {

	gl_Position = modelToClipMatrix * vec4(v_position, 1.0);

	vec3 T,B,N;

	N = normalize((modelToCameraMatrix * vec4(v_normal,0.0)).xyz);
	T = normalize((modelToCameraMatrix * vec4(v_TBN_t,0.0)).xyz);
	// Se reortogonaliza
	T = normalize(T - dot(T,N) * N);
	B = cross(N,T);
	// o B = normalize((modelToCameraMatrix * vec4(v_TBN_b,0.0)).xyz);

	// matriz para pasar a espacio tangente
	mat3 TBN = transpose(mat3(T,B,N));	// mat3(columna0,columna1,columna2)

	f_viewDirection	= normalize(TBN * ((-1.0)*v_position));

	f_texCoord = v_texCoord;


	for (int i=0; i < active_lights_n; i++){
		//if (theLights[i].position[3] == 0)
			f_lightDirection[i] = normalize(TBN * vec4(theLights[i].position.xyz-v_position,0).xyz) ;
		//else
		//	f_lightDirection[i] = normalize(TBN * (modelToCameraMatrix * (-1.0)*theLights[i].position).xyz) ;
		f_spotDirection[i]  = normalize(TBN * (modelToCameraMatrix * vec4(theLights[i].spotDir,0)).xyz);
	}
}
