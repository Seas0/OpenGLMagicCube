#version 330 core

// FragColor: color output
out vec4 FragColor;

// TexCoord: texture coordinate
in vec2 TexCoord;

// PointColor: color of the point 
in vec3 PointColor;

// texture samplers
uniform sampler2D Texture;

// lighting system
uniform vec3 ambient;

// color mask
uniform vec3 mask;

void main() {
  // apply the mask on the texture
  FragColor =
      texture(Texture, TexCoord) *
      vec4(vec3(1.0f, 1.0f, 1.0f) - mask, 1.0f);
}