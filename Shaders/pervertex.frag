#version 120

varying vec4 f_color;
varying vec2 f_texCoord;

uniform sampler2D texture0;

void main() {
	vec4 texColor;
	texColor = texture2D(texture0, f_texCoord);
	// Color del final del pixel

	// color (interpolado) * color_textura
	gl_FragColor = f_color; // * texColor;
}
