#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1, ourTexture2;

void main(){
    FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
    // FragColor = texture(textureBox, TexCoord);
    // FragColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
}
