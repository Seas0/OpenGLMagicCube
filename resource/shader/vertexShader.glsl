#version 330 core

// aPosition: the origin position of a vertex in local space
// ----------------------------------------------------
layout(location = 0) in vec3 aPosition;

// aColor: the origin position of a vertex in local space
// ----------------------------------------------------
layout(location = 1) in vec3 aColor;

// aTexCoord: the uv position of a vertex
// --------------------------------------
layout(location = 2) in vec2 aTexCoord;

// TexCoord: the uv position of a vertex
// -------------------------------------
out vec2 TexCoord;

// PointColor: the color of this point
out vec3 PointColor;

// model: transform local space to world space
uniform mat4 model;

// view: transform world space to view space
uniform mat4 view;

// projection: transform view space to clip space
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(aPosition, 1.0f);
  TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}