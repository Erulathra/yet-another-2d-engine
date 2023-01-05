#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in mat4 transform;
layout(location = 5) in ivec2 tileCoord;

layout(std140, binding = 0) uniform TransformationMatrices {
    mat4 projection;
    mat4 view;
};

uniform int tileSize;
uniform int tileMapSize;

out VS_OUT {
    vec2 texCoord;
    flat ivec2 tileCoord;
    float uvTileSize;
} vs_out;

void main() {
    vs_out.tileCoord = tileCoord;
    vs_out.uvTileSize = 1.f / (tileMapSize/tileSize);

    gl_Position = projection * view * transform * vec4(position, 1.0);

    vec2 fTileCoord;
    fTileCoord.x = float(tileCoord.x);
    fTileCoord.y = float(-tileCoord.y);

    vec2 texCoord = position.xy + vec2(0.5, 0.5) + fTileCoord;
    texCoord.y = 1 - texCoord.y;

    vs_out.texCoord = texCoord * vs_out.uvTileSize;
    vs_out.texCoord.y = 1 - vs_out.texCoord.y;
}
