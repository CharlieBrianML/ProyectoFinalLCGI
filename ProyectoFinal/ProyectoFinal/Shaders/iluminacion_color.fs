#version 330 core

//uniform vec4 ourColor; 
//out vec4 color;
in vec3 our_normal;
in vec3 fragPos;

 struct LightPosition{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
 };

 uniform vec4 ourColor;
 uniform LightPosition light;
 uniform vec3 viewPos;//11
 out vec4 color;

void main()
{
    vec3 ambient = light.ambient*vec3(ourColor);
	vec3 Normal = normalize(our_normal);///10
	vec3 lightDirection = normalize(light.position - fragPos);//10
	float diff = max(dot(Normal, lightDirection),0.0);//10
	vec3 diffuse = diff*light.diffuse*vec3(ourColor);//10

	vec3 r = reflect(-lightDirection,Normal);//11
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = pow(max(dot(r,viewDir),0.0),32.0);//11
	vec3 specular = spec * light.specular * vec3(ourColor);

	//color = vec4(ambient+diffuse,1.0);//10
	color = vec4(ambient+diffuse+specular,1.0);//11
}
