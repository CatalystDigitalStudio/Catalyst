#include "pch.h"
#include "mtllib.h"

#include <fstream>
#include <vector>

namespace Catalyst
{
    namespace files
    {

        static unsigned int getMaterialCount(std::ifstream& file)
        {
            std::string line;

            std::getline(file, line);
            std::getline(file, line);

            auto count = line.find(':');

            return (unsigned int)std::atoi(line.data() + count + 1);
        }

        void mtl::parseMtl(const std::string& path, std::unordered_map<std::string, mtl>& materials)
        {
            std::ifstream file("E:/Nova/ColtMcG1/Catalyst/src/Reactor/assets/" + path);

            if (!file.is_open())
            {
                CATALYST_CORE_INFO("Material file not found. Path : {0}", path.c_str());
                return;
            }

            std::string line;

            materials.reserve(getMaterialCount(file));

            std::stringstream buffer;
            buffer << file.rdbuf();

            std::string data = buffer.str();

            size_t cIndex = data.find("newmtl", 0), nIndex = 0, index = 0;;

            for (;nIndex != data.size();)
            {
                nIndex = data.find("newmtl", cIndex+1);

                if (nIndex > data.size())
                {
                    nIndex = data.size();
                    index++;
                }

                auto lines = split({ &data[cIndex], &data[nIndex] }, '\n');
                mtl material = {};

                for (auto line : lines)
                {
                    auto items = split(line, ' ');

                    if (items.size() < 2)
                    {
                        continue;
                    }

                    std::string key = items[0];

                    items.erase(items.begin());

                    if (key.compare("newmtl") == 0)
                    {
                        material.name = items[0];
                    } 
                    else if (key.compare("Ka") == 0)
                    {
                        parseFloats(items.data(), items.size(), material.Ka);
                    }
                    else if (key.compare("Kd") == 0)
                    {
                        parseFloats(items.data(), items.size(), material.Kd);
                    }
                    else if (key.compare("Ks") == 0)
                    {
                        parseFloats(items.data(), items.size(), material.Ks);
                    }
                    else if (key.compare("Ns") == 0)
                    {
                        parseFloats(items.data(), items.size(), material.Ns);
                    }
                    else if (key.compare("d") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.d);
                    }
                    else if (key.compare("Tf") == 0)
                    {
                        parseFloats(items.data(), items.size(), material.Tf);
                    }
                    else if (key.compare("Ni") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Ni);
                    }
                    else if (key.compare("illum") == 0)
                    {
                        parseInts(items.data(), items.size(), &material.illum);
                    }
                    else if (key.compare("Pr") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Pr);
                    }
                    else if (key.compare("Pm") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Pm);
                    }
                    else if (key.compare("Ps") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Ps);
                    }
                    else if (key.compare("Pc") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Pc);
                    }
                    else if (key.compare("Prc") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Pcr);
                    }
                    else if (key.compare("Ke") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.Ke);
                    }
                    else if (key.compare("aniso") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.aniso);
                    }
                    else if (key.compare("anisor") == 0)
                    {
                        parseFloats(items.data(), items.size(), &material.anisor);

                    }
                    else
                    {
                        CATALYST_CORE_INFO("Textures not supported yet!");
                        __debugbreak();
                    }

                }

                materials[material.name] = material;
                cIndex = nIndex;
            }

            file.close();

        }
    }
}

