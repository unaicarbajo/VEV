#version 120

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient; // Scene ambient light

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

uniform struct material_t {
	vec3  diffuse;
	vec3  specular;
	float alpha;
	float shininess;
} theMaterial;

uniform sampler2D texture0;

varying vec3 f_position;      // camera space  positionEye
varying vec3 f_viewDirection; // camera space  V vector que va del vértice a la cámara en el SRC
varying vec3 f_normal;        // camera space  normalEye (normal del vértice en el SRC)
varying vec2 f_texCoord;

vec4 f_color;


// Dado un vector de una luz y la normal obtener la aportación lambertiana
// Recordar: el vector de la luz va del revés
float lambert_factor(vec3 n, vec3 l){
	// El dot puede ser negativo
	return max(0.0,dot(n,l));
}

// Cálculo difusa
vec3 diffuse_vec(const in int i, const in vec3 lightDirection, 
					const in vec3 normal){
	float difFactor;
	difFactor = lambert_factor(normal, lightDirection);
	return theLights[i].diffuse * difFactor;
}

// Cálculo especular
vec3 specular_vec(const in int i,const in vec3 lightDirection, 
					const in vec3 normal, const in vec3 V){

	float specFactor;
	vec3 R = 2*dot(normal,lightDirection)*normal - lightDirection;
	float RoV = dot(normalize(R),normalize(V));
	// Se comprueba si el pow va a ser viable, equivalente al max
	float powRovS;
	if (RoV>0.0 && theMaterial.shininess>0.0){
		powRovS = pow(RoV,theMaterial.shininess);
	}
	else{
		powRovS = 0.0;
	}
	specFactor = dot(normal,lightDirection)*powRovS;
	return theLights[i].specular * specFactor;
}
float attenuation_factor(const in int i, const float distance){
	// Si se normalizara theLights[i].attenuation se le quitaría importancia a la distancia
	float att_inv = theLights[i].attenuation.x+theLights[i].attenuation.y*distance + theLights[i].attenuation.z*pow(distance,2);
	if (att_inv != 0.0)
		return 1/att_inv;
	return 1.0;
}

float cspot_factor(const in float LoS, int i){
	float sExp = theLights[i].exponent;
	if (LoS>0.0 && sExp>0.0){
		return  pow(LoS,sExp);
	}
	return 0.0;
	}
void directional_light(inout vec3 diffuse_color, vec3 specular_color,const  in int i, vec3 normalEye, vec3 positionEye){
	// El vector L será -posicion_luz, normalizado
	diffuse_color += diffuse_vec(i,-normalize(vec3(theLights[i].position)),normalEye);
	specular_color += specular_vec(i,-normalize(vec3(theLights[i].position)),normalEye,(-1.0)*positionEye);
}

void positional_light(inout vec3 diffuse_color, vec3 specular_color,const  in int i, vec3 normalEye, vec3 positionEye){
	// El vetor L será el vector que une los puntos
	// positionEye y position(luz), normalizado
	vec3 L = theLights[i].position.xyz - positionEye;
	float distanceL = length(L);
	if (distanceL > 0.0){
		vec3 L_normal = L/distanceL;
		float attenuation = attenuation_factor(i,distanceL);
		diffuse_color += diffuse_vec(i,L_normal,normalEye) * attenuation;
		specular_color += specular_vec(i,L_normal,normalEye,(-1.0)*positionEye) * attenuation;
	}
}

void spotlight(inout vec3 diffuse_color, vec3 specular_color,const in int i, vec3 normalEye, vec3 positionEye){
	vec3 L = normalize(theLights[i].position.xyz - positionEye);
	vec3 spotDirEye = normalize(theLights[i].spotDir);
	float cosAlpha = max(0.0,dot(-L,spotDirEye));
	if (cosAlpha >= theLights[i].cosCutOff){
		float cspot = cspot_factor(dot(-L,spotDirEye),i);
		diffuse_color += diffuse_vec(i,L,normalEye) * cspot;
		specular_color += specular_vec(i,L,normalEye,(-1.0)*positionEye) * cspot;
	}
}

void main() {
	vec3 normalEye, positionEye, diffuse_color, specular_color;
	normalEye = normalize(f_normal);
	diffuse_color = vec3(0.0,0.0,0.0);
	specular_color = vec3(0.0,0.0,0.0);
	for (int i = 0; i < active_lights_n; i++){
		//////////// DIRECCIONAL /////////////
		if (theLights[i].position[3] == 0){
			directional_light(diffuse_color, specular_color, i, normalEye, f_position);
		}
		//////////// POSICIONAL /////////////
		// el ángulo (cutoff) es 90
		else if (theLights[i].position[3] == 1 && theLights[i].cosCutOff == 0.0){
			positional_light(diffuse_color, specular_color, i, normalEye, f_position);
		}
		//////////// SPOTLIGHT /////////////
		else{
			spotlight(diffuse_color, specular_color, i, normalEye, f_position);
		}
	}

	f_color.rgb = scene_ambient + diffuse_color * theMaterial.diffuse + specular_color * theMaterial.specular;
	f_color.a = theMaterial.alpha; 	// 1.0 = canal alpha (total transparencia)
									// 0.0 = totalmente opaco

	vec4 texColor = texture2D(texture0,f_texCoord);
	gl_FragColor = texColor * f_color;
}
