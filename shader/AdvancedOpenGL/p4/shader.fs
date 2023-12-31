#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;

void main() {
  vec4 texColor = texture(texture0, TexCoords);
  if (texColor.a < 0.1)
    discard;
  FragColor = texColor;
}