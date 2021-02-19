//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

out vec4 out_Color;

// projective reflection -- water reflection
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
// dudvMap -- water rippling
uniform sampler2D dudvMap;
// normal map -- specular reflection
uniform sampler2D normalMap;
// depth map
uniform sampler2D depthMap;
// color of the "sun"
uniform vec3 lightColor;
//adds a variable component that depends on time for where we sample the texture
uniform float moveFactor;

const float waveStrength = 0.04;
const float shineDamper = 20.0;
const float reflectivity = 0.5;

void main(void) {
    //normalized device coordinates - screen space coordinates
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    
    //depth map (calculates the water depth)
    float near = 0.1; float far = 10000.0; //near and far planes are from window projection matrix
    float depth = texture(depthMap, refractTexCoords).r; //depth info stored in r in range 0-1

    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    float waterDepth = floorDistance - waterDistance;
    
    //calculate the water distortion & refraction amount
    vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
    distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth/20.0, 0.0, 1.0);
    
    //recalculate the texCoords including distortion now
    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords,0.001, 0.999);
    
    reflectTexCoords -= totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.0001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

    //calculate the reflect and refract colors
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);
    
    //normal map
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0); // the *2-1 make the values of r and g between -1 and 1 so that the normals are not all positive (for a more realistic effect). increase y component to make the water look less "bumpy"
    normal = normalize(normal);
    
    //fresnel effect
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5); //the higher the exponent, the more reflective the water is
    
    //specular lighting for the water (normal map)
    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth/5.0, 0.0, 1.0);
        
    //resulting color
    out_Color = mix(reflectColor, refractColor, refractiveFactor);
    out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0); //give water a bluish tint & specular highlights
    out_Color.a = clamp(waterDepth/5.0, 0.0, 1.0); //want alpha value to be closer to 0 (more transparent) when depth is shallow and 1 when depth is deep (in this case depth of 5). alpha value should be in range [0,1]
}
