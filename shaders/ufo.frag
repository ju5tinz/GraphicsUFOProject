#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 strength;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 FragColor;

void main()
{
/*
	// ambient
	vec3 ambient = strength.x * lightColor;
  	
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = strength.y * diff * lightColor;
    
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = strength.z * spec * lightColor;  
        
	// light attenuation
	float intensity = 15.0;
	intensity = intensity/(length(lightPos - FragPos));

	vec3 result = intensity * (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
*/

	vec3 result = (normalize(Normal) + vec3(1.0, 1.0, 1.0))/2;
	FragColor = vec4(result, 1.0);
}