#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

uniform sampler2D texture;
uniform vec3 cameraPos;

void main()
{
	FragColor = vec4(1.0f, 0, 0, 1.0f);

	//vec3 result = (normalize(Normal) + vec3(1.0, 1.0, 1.0))/2;
    //FragColor = vec4(result, 1.0);

	//FragColor = texture(texture, TexCoord);

	//float ratio = 1.00 / 1.52;
    //vec3 I = normalize(Position - cameraPos);
    //vec3 R = refract(I, normalize(Normal), ratio);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
