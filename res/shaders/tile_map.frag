#version 430 core

uniform sampler2D texture_diffuse;

out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

void main() {

    FragColor = texture(texture_diffuse, fs_in.TexCoord);

}
