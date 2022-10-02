#pragma once

#include <unordered_map>
#include <fstream>

#include <filesystem>

namespace Catalyst
{

    enum class Openmode
    {
        app = std::ios::app,
        in = std::ios::in,
        out = std::ios::out,
        ate = std::ios::ate,
        trunc = std::ios::trunc,
        nocreate = std::ios::_Nocreate,
        noreplace = std::ios::_Noreplace,
        binary = std::ios::binary,
    };

    struct Path
    {
        std::string m_FullPath;
        std::string m_Path;
        std::string m_Name;
        std::string m_Extention;

        static Path construct(std::string path);
    };

    struct File
    {

    public:
        File() = default;
        File(const std::string&, Openmode);
        ~File();

        const std::string read();
        void write(const std::string&);

        std::fstream& get();

        CATALYST_LOGIC_DISCARD const std::string getFullPath()  const;
        CATALYST_LOGIC_DISCARD const std::string getPath()      const;
        CATALYST_LOGIC_DISCARD const std::string getName()      const;
        CATALYST_LOGIC_DISCARD const std::string getExtention() const;

    private:
        std::fstream m_Stream;

    private:
        std::filesystem::path m_Path;

    private:
        template<typename T>
        friend class IFactory;

    };

}
