#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in mat4 Transform;
layout(location = 5) in ivec2 TileCoord;

layout(std140, binding = 0) uniform TransformationMatrices {
    mat4 Projection;
    mat4 View;
};

uniform int tileSize;
uniform int tileMapSize;

out VS_OUT {
    vec2 TexCoord;
} vs_out;

void main() {
    gl_Position = Projection * View * Transform * vec4(Position, 1.0);

    vec2 fTileCoord;
    fTileCoord.x = float(TileCoord.x);
    fTileCoord.y = float(-TileCoord.y);

    vec2 texCoord = Position.xy + vec2(0.5, 0.5) + fTileCoord;
    texCoord.y = 1 - texCoord.y;

    vs_out.TexCoord = texCoord / (tileMapSize / tileSize);
    vs_out.TexCoord.y = 1 - vs_out.TexCoord.y;
}
