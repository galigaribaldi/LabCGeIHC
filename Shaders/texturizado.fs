#version 330 core

in vec2 out_uv;
out vec4 color;
//unidad de Textura de tipo sampler2D
uniform sampler2D ourTexture;

void main(){
	//obtiene color de la textura en base a al unidad de textura y las coordenadas
	color = texture(ourTexture, out_uv);
	if(color.a < 0.8)
		discard;
	//color = vec4(0.3, 0.5, 0.4, 1.0);
}