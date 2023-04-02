#pragma once

#include "mtllib.h"

namespace Catalyst
{
    namespace files
    {
        struct obj
        {

            ///==========================================================================================
            ///  Basic OBJ Types
            ///==========================================================================================
            typedef struct 
            {
                float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
            } verticie;

            typedef struct
            {
                float u = 0.0f, v = 0.0f, w = 0.0f;
            } texture;

            typedef struct
            {
                float x = 0.0f, y = 0.0f, z = 0.0f;
            } normal;


            ///==========================================================================================
            ///  OBJ Object Construction
            ///==========================================================================================

            typedef struct
            {
                unsigned int v_index = 0;
                unsigned int t_index = 0;
                unsigned int n_index = 0;
            } face;


            ///==========================================================================================
            ///  OBJ Object
            ///==========================================================================================

            typedef struct
            {
                std::string name;

                verticie* verticies;
                texture* texcoords;
                normal* normals;

                face* faces;

            } object;

            static obj parseFile(const char*);
            static obj::object parseObject(std::string);
            static void parseFloat4(std::string data, float& x, float& y, float& z, float& w);

            object* objects;
            mtl* materials;

            ~obj();
        };
    }
}
