#version 330 core

// FragColor: color output
out vec4 FragColor;

// TexCoord: texture coordinate
in vec2 TexCoord;

in vec3 FragPos;

in vec3 Normal;

// texture samplers
uniform sampler2D Texture;

// lighting system
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

// color mask
uniform vec3 mask;



void main() {
  
  // basic phong light system
  // ------------------------
  
  // ambient
  float ambientStrength = 0.01;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse 
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // specular(Blinn-Phong)
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32);
  vec3 specular = lightColor * spec; 
  
  /*
  // specular(Phong)
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  
  */

  // apply mask & light on the texture
  FragColor =
      texture(Texture, TexCoord)
      * vec4((ambient + diffuse + specular), 1.0f)
      * vec4(vec3(1.0f, 1.0f, 1.0f) - mask, 1.0f);
}
