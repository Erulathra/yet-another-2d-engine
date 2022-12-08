#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& Vertices, const std::vector<GLuint>& Indices,
           const std::vector<Texture>& Textures) : Vertices(Vertices), Indices(Indices), Textures(Textures),
           Vao(Vertices, Indices, Textures)
{
}

void Mesh::Draw(ShaderWrapper& Shader) const
{
    BindTextures(Shader);

    Vao.Draw();
}

void Mesh::BindTextures(const ShaderWrapper& Shader) const
{
    uint16_t TextureIndex = 0;
    uint16_t DiffuseIndex = 0;
    uint16_t SpecularIndex = 0;

//    for (const Texture & Item : Textures)
//    {
//        glActiveTexture(GL_TEXTURE0 + TextureIndex);
//
//        std::string Number;
//        if(Item.TextureType == "texture_diffuse")
//            Number = std::to_string(DiffuseIndex++);
//        else if(Item.TextureType == "texture_specular")
//            Number = std::to_string(SpecularIndex++);
//        else if (Item.TextureType == "texture_normalmap")
//            Number = std::to_string(0);
//
//        Shader.SetFloat(Item.TextureType + Number, TextureIndex);
//        glBindTexture(GL_TEXTURE_2D, Item.Id);
//        TextureIndex++;
//    }
//    glActiveTexture(GL_TEXTURE0);
}

const VAOWrapper& Mesh::GetVao() const
{
    return Vao;
}
