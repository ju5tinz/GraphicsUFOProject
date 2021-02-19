//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
#version 330 core

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;

void main(void){
    out_Color = vec4(texture(guiTexture,textureCoords).rgb,1);
}
