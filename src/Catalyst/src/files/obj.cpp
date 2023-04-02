#include "pch.h"
#include "obj.h"

#include <string>
#include <fstream>
#include <streambuf>

namespace Catalyst 
{
    namespace files
    {

        obj obj::parseFile(const char* path)
        {


            obj object = {};
            std::string data;

            ///==========================================================================================
            ///  Read OBJ File
            ///==========================================================================================
            {

                std::ifstream t("file.txt");
                t.seekg(0, std::ios::end);
                data.reserve(t.tellg());
                t.seekg(0, std::ios::beg);

                data.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

                t.close();

            }

            ///==========================================================================================
            ///  OBJ Count Objects
            ///==========================================================================================

            unsigned int objects = count_KMP(data, "o ");

            ///==========================================================================================
            ///  OBJ Count Materials
            ///==========================================================================================

            unsigned int materials = count_KMP(data, "mtllib ");

            ///==========================================================================================
            ///  OBJ Allocate
            ///==========================================================================================

            object.objects = new obj::object[objects];
            object.materials = new mtl[materials];

            ///==========================================================================================
            ///  OBJ 
            ///==========================================================================================

            std::string o = data.substr(data.find("o "));

            while (data.find("o ") != data.npos)
            {
                parseObject(data.substr(data.find("o ")));
            }

            return object;
        }

        obj::~obj()
        {

            if (objects)
            {
                delete[] objects;
            }

            if (materials)
            {
                delete[] materials;
            }

        }

        obj::object obj::parseObject(std::string data)
        {
            obj::object object = {};

            unsigned int verticies = count_KMP(data, "v ");
            unsigned int texcoords = count_KMP(data, "vt ");
            unsigned int normals   = count_KMP(data, "vn ");
            unsigned int faces     = count_KMP(data, "f ");

            object.verticies = new obj::verticie[verticies];
            object.texcoords = new obj::texture[texcoords];
            object.normals   = new obj::normal[normals];
            object.faces     = new obj::face[faces];

            std::stringstream ss;
            ss >> data;

            std::string line;

            while (std::getline(ss, line))
            {

                if (line.starts_with("o "))
                {
                    object.name = std::string(line.data() + line.find(' '), line.size());
                } 
                else if (line.starts_with("v "))
                {

                } 
                else if (line.starts_with("vt "))
                {

                } 
                else if (line.starts_with("vn "))
                {

                } 
                else if (line.starts_with("usemtl "))
                {

                } 
                else if (line.starts_with("f "))
                {

                } 
                else if (line.starts_with("g "))
                {

                }
                else
                {
                    //ignore
                }

            }

            return object;
        }

    }
}

