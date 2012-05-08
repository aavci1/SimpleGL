#version 330 core

// gbuffer textures
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
// screen size and camera position
uniform vec2 viewportSize;
uniform vec3 cameraPos;
// light properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightDiffuseIntensity;
uniform float lightSpecularIntensity;
uniform float lightAttenuationRange;
uniform float lightAttenuationConstant;
uniform float lightAttenuationLinear;
uniform float lightAttenuationQuadratic;
// outputs
out vec4 _color;

void main() {
	vec2 texCoord = gl_FragCoord.xy / viewportSize;
	// get color, normal and position
	vec3 color = texture(texture0, texCoord).xyz;
	vec3 normal = (texture(texture1, texCoord).xyz - 0.5) * 2.0;
	vec3 position = texture(texture2, texCoord).xyz;
	// get specular parameters
	float specularIntensity = texture(texture0, texCoord).w;
	float specularPower = texture(texture1, texCoord).w;
	// discard fragment if not within radius
	vec3 lightVector = position - lightPos;
	float dist = length(lightVector);
	if (dist > lightAttenuationRange)
		discard;
	// discard fragment if facing away
	vec3 lightDir = normalize(lightVector);
	float diffuseFactor = dot(normal, -lightDir);
	if (diffuseFactor <= 0)
		discard;
	// calculate attenuation
	float k = dist / lightAttenuationRange;
	float attenuation = lightAttenuationConstant + lightAttenuationLinear * k + lightAttenuationQuadratic * k * k;
	attenuation = clamp(1.0 - attenuation, 0.0, 1.0);
	// calculate diffuse light contribution
    vec3 lightContrib = lightColor * lightDiffuseIntensity * diffuseFactor;
	// calculate specular light contribution
	vec3 eyeDir = normalize(cameraPos - position);
	vec3 reflectionDir = normalize(reflect(lightDir, normal));
	float specularFactor = pow(dot(eyeDir, reflectionDir), specularPower);
	if (specularFactor > 0)
		lightContrib += lightColor * lightSpecularIntensity * specularIntensity * specularFactor;
	// calculate final color
    _color = vec4(color * lightContrib * attenuation, 1.0);
}
