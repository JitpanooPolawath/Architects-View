#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 

in vec3 Normal; 
in vec3 FragPos; 

uniform int ID;
uniform vec3 viewPos;

void main()
{
	if(ID == 1){
		FragColor = vec4(1.0);
	}else{

		// Ambient
		vec3 ambient = light.ambient * material.ambient;

		// Diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos); 
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * material.diffuse);

		// Specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
		vec3 specular = light.specular * (spec * material.specular);  

		// Combined
		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0);
	}
};