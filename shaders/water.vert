//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
#version 330 core

in vec2 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float tiling = 2.0;

void main(void) {
    vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
    toCameraVector = cameraPosition - worldPosition.xyz;
    fromLightVector = worldPosition.xyz - lightPosition;
    
    textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
    clipSpace = gl_Position;
}
