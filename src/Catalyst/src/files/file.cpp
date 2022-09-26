#include "phc.h"
#include "file.h"

#include <streambuf>

namespace Catalyst
{
    File::File(const std::string& path, Openmode mode)
    {
        size_t lastSlash = path.find_last_of('/');
        size_t lastDot = path.find_last_of('.');

        m_FullPath = path;
        m_Path = path.substr(0, lastSlash);
        m_Name = path.substr(lastSlash, lastDot);
        m_Extention = path.substr(lastDot);

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
        return std::string((std::istreambuf_iterator<char>(m_Stream)),
            std::istreambuf_iterator<char>());
    }
    void File::write(const std::string& data)
    {
        m_Stream.write(data.c_str(), data.size());
    }

    std::fstream& File::get()
    {
        return m_Stream;
    }

    constexpr const std::string& File::getFullPath() const
    {
        return m_FullPath;
    }
    constexpr const std::string& File::getPath() const
    {
        return m_Path;
    }
    constexpr const std::string& File::getName() const
    {
        return m_Name;
    }
    constexpr const std::string& File::getExtention() const
    {
        return m_Extention;
    }
}
