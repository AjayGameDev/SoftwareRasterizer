#include "Model.h"
#include "FileManager.h"
#include "spdlog/spdlog.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <assimp/mesh.h>
#include <cstdint>

#include <iostream>

Model::Model()
{

}

Model::Model(std::string modelName)
{
    LoadModel(modelName);
}

void Model::LoadModel(std::string modelName)
{
    std::string path = FileManager::GetLocation(FileManager::Location::Model) + modelName; 

    Assimp::Importer importer; 

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

    if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        spdlog::error("Can't load model: {}", importer.GetErrorString() ); 
        return;
    }

    unsigned int numberOfMeshes = scene->mNumMeshes;
    std::cout << "\nNumber of meshes  " << numberOfMeshes;

    uint32_t vertexOffset = 0;
    
    for (int i = 0; i<numberOfMeshes; i++) 
    {
        aiMesh* mesh         = scene->mMeshes[i]; 
        bool hasPosition     = mesh->HasPositions();
        bool hasNormal       = mesh->HasNormals();
        bool hasUV           = mesh->HasTextureCoords(0);
        bool hasTangent      = mesh->HasTangentsAndBitangents();

        unsigned int numberOfVertices = mesh->mNumVertices;
        std::cout << "\nNumber of vertices  " << numberOfVertices;

        for (int j = 0; j<numberOfVertices; j++) 
        {
            Vertex vertex;
            
            if(hasPosition)
                vertex.position = {mesh->mVertices[j].x,mesh->mVertices[j].y,mesh->mVertices[j].z};
            if(hasNormal)
                vertex.normal   = {mesh->mNormals[j].x,mesh->mNormals[j].y,mesh->mNormals[j].z}; 
            if(hasUV)
                vertex.uv       = {mesh->mTextureCoords[0][j].x,mesh->mTextureCoords[0][j].y};
            if(hasTangent)
                vertex.tangent  = {mesh->mTangents[j].x,mesh->mTangents[j].y,mesh->mTangents[j].z}; 

            vertices.push_back(vertex);
        }

        unsigned int numberOfFaces = mesh->mNumFaces; 
        std::cout << "\nNumber of faces  " << numberOfFaces;

        for (int j = 0; j<numberOfFaces; j++) 
        {
            aiFace face = mesh->mFaces[j]; 


            for (int k=0; k<3; k++)
            {
                indices.push_back(face.mIndices[k] + vertexOffset);
            }
        } 

        vertexOffset += numberOfVertices;

    }
    

}