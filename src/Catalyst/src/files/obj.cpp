#include "pch.h"
#include "obj.h"

#include <string>
#include <fstream>
#include <streambuf>

namespace Catalyst 
{
    namespace files
    {

        static std::unordered_map<std::string, std::vector<std::string>> mapData(std::ifstream& file, std::unordered_map<std::string, std::tuple<unsigned int, unsigned int, std::string>>& object_faces)
        {
            std::unordered_map<std::string, std::vector<std::string>> data = { {"o", {}},{"mtllib", {}},{"v", {}},{"vt", {}},{"vn", {}},{"s", {}},{"f", {}} };
            std::string line;
            std::string postfix;
            std::string prefix;
            std::string currentName;
            unsigned int pindex = 0, index = 0;

            while (std::getline(file, line))
            {
                auto fs = line.find(' ');
                postfix = line.substr(fs+1);
                prefix = { line.begin(), line.begin()+fs};
                auto& pool = data[prefix];
                
                if (prefix == "o")
                {
                    currentName = postfix;
                    object_faces[currentName] = { pindex, index, "" };
                    pindex = index;

                    pool.push_back(postfix);
                }
                else if (prefix == "f")
                {
                    auto items = split(postfix, ' ');

                    for (auto& item : items)
                    {
                        pool.push_back(item);
                    }

                    index += items.size();
                }
                else if (prefix == "usemtl")
                {
                    std::get<2>(object_faces[currentName]) = postfix;
                }
                else
                {
                    pool.push_back(postfix);
                }
            }

            return data;
        }

        obj obj::parseFile(const char* path)
        {


            obj object = {};

            ///==========================================================================================
            ///  Read OBJ File
            ///==========================================================================================

            std::ifstream file(path);

            if (!file.is_open())
            {
                __debugbreak();
            }
            std::unordered_map<std::string, std::tuple<unsigned int, unsigned int, std::string>> objf;
            std::unordered_map<std::string, std::vector<std::string>> data = mapData(file, objf);

            ///==========================================================================================
            ///  OBJ Count Objects
            ///==========================================================================================
            object.objects.reserve(  data.count("o" ));
            object.verticies.reserve(data.count("v" ));
            object.texcoords.reserve(data.count("vt"));
            object.normals.reserve(  data.count("vn"));

            ///==========================================================================================
            ///  OBJ Materials
            ///==========================================================================================

            mtl::parseMtl(data["mtllib"][0], object.materials);
            
            ///==========================================================================================
            ///  OBJ 
            ///==========================================================================================

            // Verticies
            auto parseVerticies = [&]() {
                verticie verticie;
                for (auto& item : data["v"])
                {
                    auto items = split(item, ' ');
                    parseFloats(items.data(), items.size(), (float*)&verticie);
                    object.verticies.push_back(verticie);
                    verticie = { 0.0f, 0.0f, 0.0f, 1.0f };
                }
            };
            auto vFuture = std::async(std::launch::async, parseVerticies);

            // Texture Coordinates
            auto parseTexcoords = [&]() {
                texture tex;
                for (auto& item : data["vt"])
                {
                    auto items = split(item, ' ');
                    parseFloats(items.data(), items.size(), (float*)&tex);
                    object.texcoords.push_back(tex);
                    tex = { 0.0f };
                }
            };
            auto tFuture = std::async(std::launch::async, parseTexcoords);

            // Normals
            auto parseNormals = [&]() {
                normal normals;
                for (auto& item : data["vn"])
                {
                    auto items = split(item, ' ');
                    parseFloats(items.data(), items.size(), (float*)&normals);
                    object.normals.push_back(normals);
                    normals = { 0.0f, 0.0f, 0.0f};
                }
            };
            auto nFuture = std::async(std::launch::async, parseNormals);

            /*
            * data  -> { "o", { "Obj1", "Obj2" } }
            * faces -> { "f", { "0/0/0", "1/1/1", "0/0/0", "1/1/1" } }
            * objf  -> { "Obj1", {0, 2} }, { "Obj2", {3, 4} }
            * 
            * name = data["o"] -> "Obj1"
            * index = objf[name] -> {0, 4}
            * faces = { "0/0/0", "1/1/1" }
            */

            unsigned int index = 0;
            for (auto& name : data["o"])
            {
                obj::object objt = { name, {}, {} };

                auto& faces = data["f"];
                auto indexs = objf[name];

                objt.material = object.materials[std::get<2>(indexs)];

                face face;
                for (unsigned int i = std::get<0>(indexs); i < std::get<1>(indexs); i++)
                {
                    auto items = split(faces[i], '/');

                    parseInts(items.data(), items.size(), (int*)&face);
                    objt.faces.push_back(face);
                    face = { 0 };
                }

                object.objects.push_back(objt);

                ++index;
            }

            ///==========================================================================================
            ///  FINISH 
            ///==========================================================================================
            
            vFuture.wait();
            tFuture.wait();
            nFuture.wait();

            file.close();

            return object;
        }

    }
}

