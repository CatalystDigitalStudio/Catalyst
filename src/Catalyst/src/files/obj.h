#pragma once

#include "mtllib.h"

namespace Catalyst
{
    namespace files
    {
        ///==========================================================================================
        ///  OBJ File Parsing
        ///==========================================================================================
        /// 
        /// # Blender v2.80 (sub 74) OBJ File: 'example.blend'  <-- Header
        /// # www.blender.org                                   <-- Vendor
        /// mtllib casa.mtl                                     <-- Material File
        /// o PAREDES_Cubo                                      <-- Object Tag + Name
        /// v -1.0 -1.0 0.0                                     <-- Verticies (x y z [w=1.0])
        /// v 1.0 -1.0 0.0                                      <--
        /// v 1.0 1.0 0.0                                       <--
        /// v -1.0 1.0 0.0                                      <--
        /// vt -1.0 -1.0 0.0                                    <-- Tex-Coords (u [v w])
        /// vt 1.0 -1.0 0.0                                     <--
        /// vt 1.0 1.0 0.0                                      <--
        /// vt -1.0 1.0 0.0                                     <--
        /// vn 0.0 0.0 1.0                                      <-- Normals (x y z)
        /// vn 0.0 0.0 1.0                                      <--
        /// vn 0.0 0.0 1.0                                      <--
        /// vn 0.0 0.0 1.0                                      <--
        /// usemtl pared                                        <-- Material
        /// s off                                               <-- Smooth Shading (off/on)
        /// f 0/0/0 1/1/0 2/2/0 3/3/0 2/2/0 1/1/0               <-- Faces/Indicies (v/vt/vn v/vt/vn v/vt/vn)
        /// o Cubo_0.002                                        <-- Object Tag + Name
        /// ...
        /// 
        ///==========================================================================================
        /// 
        /// ASYNC
        /// File is read asynchronously w/ 4 workers
        /// File data is copied line by line by each worker and delt with accordingly.
        /// Worker 1: Verticie
        /// Worker 2: TexCoord
        /// Worker 3: Normal
        /// Worker 4: Face
        /// 
        
        struct obj
        {

            ///==========================================================================================
            ///  Basic OBJ Types
            ///==========================================================================================
            struct verticie
            {
                float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
            };

            struct texture
            {
                float u = 0.0f, v = 0.0f, w = 0.0f;
            };

            struct normal
            {
                float x = 0.0f, y = 0.0f, z = 0.0f;
            };


            ///==========================================================================================
            ///  OBJ Object Construction
            ///==========================================================================================

            struct face
            {
                int v_index = 0;
                int t_index = 0;
                int n_index = 0;
            };


            ///==========================================================================================
            ///  OBJ Object
            ///==========================================================================================

            struct object
            {
                std::string name;

                std::vector<face> faces;

                mtl material;

            };

            static obj parseFile(const char*);

            std::vector<object> objects = {};
            std::unordered_map<std::string, mtl> materials = {};

            std::vector<verticie> verticies;
            std::vector<texture> texcoords;
            std::vector<normal> normals;
        };
    }
}
