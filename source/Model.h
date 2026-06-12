#pragma once

#include "Maths.h"
#include "vector"
#include <cstdint>
#include <string>

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector2 uv;
};


class Model
{
    

    public:
            std::vector<Vertex> vertices; 
            std::vector<uint32_t> indices;

            Model();
            Model(std::string modelName);
            void LoadModel(std::string modelName);
};  



