#version 430 core

uniform sampler2D texture_diffuse;

out vec4 FragColor;

uniform int tileSize;
uniform int tileMapSize;

in VS_OUT {
    vec2 texCoord;
    flat ivec2 tileCoord;
    float uvTileSize;
} fs_in;

void main() {
    vec2 minTexCoord = fs_in.tileCoord * fs_in.uvTileSize;
    vec2 maxTexCoord = minTexCoord + fs_in.uvTileSize;

    vec2 clampedTexCoord = fs_in.texCoord;
    clampedTexCoord.x = clamp(clampedTexCoord.x, minTexCoord.x + 0.001, maxTexCoord.x - 0.001);
    clampedTexCoord.y = clamp(clampedTexCoord.y,  1 - (maxTexCoord.y - 0.001), 1 - (minTexCoord.y + 0.001));

    FragColor = texture(texture_diffuse, clampedTexCoord);

}
