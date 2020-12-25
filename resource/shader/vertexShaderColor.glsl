#version 330 core

// aPosition: the origin position of a vertex in local space
// ----------------------------------------------------
layout(location = 0) in vec3 aPosition;

// aNormal: the normal vector of a vertex
layout (location = 1) in vec3 aNormal;

// aColor: the origin position of a vertex in local space
// ----------------------------------------------------
layout(location = 2) in vec3 aColor;

// aTexCoord: the uv position of a vertex
// --------------------------------------
layout(location = 3) in vec2 aTexCoord;

// PointColor: the color of this point
out vec3 PointColor;

out vec3 FragPos;

out vec3 Normal;

// model: transform local space to world space
uniform mat4 model;

// view: transform world space to view space
uniform mat4 view;

// projection: transform view space to clip space
uniform mat4 projection;

void main() {
  FragPos = vec3(model * vec4(aPosition, 1.0));
  Normal = mat3(transpose(inverse(model))) * aNormal;  
  gl_Position = projection * view * model * vec4(aPosition * 0.993f, 1.0f);
  PointColor = aColor;
}
