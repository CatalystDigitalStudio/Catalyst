#include "pch.h"
#include "image.h"

#include "files/file.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"

namespace Catalyst
{
    Image::Image(const std::string& path, unsigned int channels)
    {
        //m_Data = stbi_load(path.c_str(), &m_X, &m_Y, &m_Channels, channels);
    }
    Image::Image(unsigned char* data, size_t length, unsigned int channels)
    {

//        m_Data = stbi_load_from_memory(data, (int)length, &m_X, &m_Y, &m_Channels, channels);

    }
    Image::~Image()
    {
        free(m_Data);
    }

    const unsigned char* Image::getData() const
    {
        return m_Data;
    }

    const int& Image::getX() const
    {
        return m_X;
    }
    const int& Image::getY() const
    {
        return m_Y;
    }
    const int& Image::getChannels() const
    {
        return m_Channels;
    }
}