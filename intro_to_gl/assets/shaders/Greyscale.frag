#version 330

uniform sampler2D tex;

const vec3 intensity = vec3(0.299, 0.587, 0.184);

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	vec4 col = texture2D(tex, texCoordOut);
	float grey = dot(col.rgb, intensity);
	colour = vec4(grey);
	colour.r = (col.r * 0.393) + (col.g * 0.769) + (col.b * 0.189);
	colour.g = (col.r * 0.349) + (col.g * 0.686) + (col.b * 0.168);    
    colour.b = (col.r * 0.272) + (col.g * 0.534) + (col.b * 0.131);
	//colour.a = 1.0;
}