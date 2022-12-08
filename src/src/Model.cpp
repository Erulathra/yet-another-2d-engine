#include "Model.h"

#include <assimp/Importer.hpp>
#include <filesystem>

#include "LoggingMacros.h"
#include "stb_image.h"

void Model::Draw()
{
    for (std::shared_ptr<Mesh> Item: Meshes)
    {
//        Item->Draw(*GetShader());
    }
}

//std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* Material, aiTextureType Type, const std::string& TypeName)
//{
//    std::vector<Texture> Textures;
//    for (uint32_t i = 0; i < Material->GetTextureCount(Type); i++)
//    {
//        aiString Path;
//        Material->GetTexture(Type, i, &Path);
//        Texture Texture;
//        Texture.Id = TextureFromFile(Path.C_Str());
//        Texture.TextureType = TypeName;
//        Texture.TexturePath = Path.C_Str();
//        Textures.push_back(Texture);
//    }
//    return Textures;
//}

//GLuint Model::TextureFromFile(const std::string& Path)
//{
//
//    unsigned int TextureID;
//    glGenTextures(1, &TextureID);
//
//    std::filesystem::path PathFromExecutable = std::filesystem::path{ModelPath}.parent_path() / Path;
//    SPDLOG_DEBUG("Loading texture at path: {}", PathFromExecutable.string());
//
//    int Width, Height, NumberOfComponents;
//    stbi_set_flip_vertically_on_load(true);
//    uint8_t* ImageData = stbi_load(PathFromExecutable.string().c_str(), &Width, &Height, &NumberOfComponents, 0);
//    if (ImageData)
//    {
//        GLenum ColorFormat;
//        if (NumberOfComponents == 1)
//            ColorFormat = (GL_RED);
//        else if (NumberOfComponents == 3)
//            ColorFormat = (GL_RGB);
//        else if (NumberOfComponents == 4)
//            ColorFormat = (GL_RGBA);
//
//        glBindTexture(GL_TEXTURE_2D, TextureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, ColorFormat, Width, Height, 0, ColorFormat, GL_UNSIGNED_BYTE, ImageData);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(ImageData);
//    }
//    else
//    {
//        SPDLOG_ERROR("Failed to load texture at path: {}", PathFromExecutable.string());
//        stbi_image_free(ImageData);
//    }
//
//    return TextureID;
//}
