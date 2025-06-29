#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
	vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

};
uniform Light light; 

in vec3 Normal; 
in vec3 FragPos; 
in vec2 TexCoords;

uniform int isSpot;
uniform int ID;
uniform vec3 viewPos;

void main()
{
	if(ID == 1){
		FragColor = vec4(1.0);
	}else{
			
		vec3 lightDir = normalize(light.position - FragPos); 

		// Ambient
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

		// Diffuse
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb; 

		// Specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

		if(isSpot == 1){
			
			float theta = dot(lightDir, normalize(-light.direction)); 
			float epsilon = (light.cutOff - light.outerCutOff);
			float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
			diffuse  *= intensity;
			specular *= intensity;
    
		}
		// attenuation
			float distance    = length(light.position - FragPos);
			float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
			ambient  *= attenuation; 
			diffuse   *= attenuation;
			specular *= attenuation;

			vec3 result = ambient + diffuse + specular;
			FragColor = vec4(result, 1.0);
	}
};