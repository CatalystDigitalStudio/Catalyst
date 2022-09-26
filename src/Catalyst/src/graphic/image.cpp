#include "phc.h"
#include "image.h"

#include "stb_image.h"

#include "files/file.h"

namespace Catalyst
{
    Image::Image(unsigned char* data, size_t length, unsigned int channels)
    {

        m_Data = stbi_load_from_memory(data, length, &m_X, &m_Y, &m_Channels, channels);

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