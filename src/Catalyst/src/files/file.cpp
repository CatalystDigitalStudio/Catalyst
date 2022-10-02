#include "pch.h"
#include "file.h"

#include <streambuf>

namespace Catalyst
{

    Path Path::construct(std::string path)
    {
        static size_t lastSlash = path.find_last_of('/');
        static size_t lastDot = path.find_last_of('.');
        static Path result = {};

        result.m_FullPath = path;
        result.m_Path = path.substr(0, lastSlash);
        result.m_Name = path.substr(lastSlash, lastDot);
        result.m_Extention = path.substr(lastDot);

        return result;
    }

    File::File(const std::string& path, Openmode mode)
    {
        m_Path = path;

        m_Stream.open(path, (std::ios::openmode)mode);
    }

    File::~File()
    {
        if (m_Stream.is_open())
        {
            m_Stream.flush();
            m_Stream.close();
        }
    }

    const std::string File::read()
    {
        if (m_Stream.is_open())
        {
            return std::string((std::istreambuf_iterator<char>(m_Stream)),
                std::istreambuf_iterator<char>());
        }
        else
        {
            return std::string();
        }
    }
    void File::write(const std::string& data)
    {
        if (m_Stream.is_open())
        {
            m_Stream.write(data.c_str(), data.size());
        }
    }

    std::fstream& File::get()
    {
        return m_Stream;
    }

    const std::string File::getFullPath() const
    {
        return m_Path.string();
    }
    const std::string File::getPath() const
    {
        return m_Path.relative_path().string();
    }
    const std::string File::getName() const
    {
        return m_Path.filename().string();
    }
    const std::string File::getExtention() const
    {
        return m_Path.extension().string();
    }


}
