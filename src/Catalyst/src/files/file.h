#pragma once

#include <unordered_map>
#include <fstream>

#include "core/internal.h"

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

    class File
    {

    protected:
        explicit File(const std::string&, Openmode);

    public:
        ~File();

        const std::string read();
        void write(const std::string&);

        std::fstream& get();

        CATALYST_LOGIC_DISCARD constexpr const std::string& getFullPath()  const;
        CATALYST_LOGIC_DISCARD constexpr const std::string& getPath()      const;
        CATALYST_LOGIC_DISCARD constexpr const std::string& getName()      const;
        CATALYST_LOGIC_DISCARD constexpr const std::string& getExtention() const;

    private:
        std::fstream m_Stream;

    private:
        std::string m_FullPath;
        std::string m_Path;
        std::string m_Name;
        std::string m_Extention;
    };

    class FileManager
    {

    public:
        static File& open(const std::string& path, Openmode);

        static bool isOpen(const std::string& path);

    private:
        static std::unordered_map<std::string, File> m_Files;

    };

}
