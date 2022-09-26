#pragma once

namespace Catalyst
{

    class Image
    {

    public:
        explicit Image(unsigned char* data, size_t length, unsigned int channels);
        ~Image();

        const unsigned char* getData() const;

        const int&  getX() const;
        const int&  getY() const;

        const int&  getChannels() const;

    private:
        unsigned char* m_Data;
        int m_X;
        int m_Y;
        int m_Channels;

    };

}
