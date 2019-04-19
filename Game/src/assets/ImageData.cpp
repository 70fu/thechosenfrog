#include <cstring>
#include "ImageData.h"
#include "../logger/imguial_log.h"

bool ImageData::isInBounds(unsigned int x, unsigned int y) const
{
    return x>=0 && x<width && y>=0 && y<height;
}

unsigned int ImageData::getIndex(unsigned int x, unsigned int y) const
{
    return y*width+x;
}

ImageData::ImageData()
{
}

ImageData::ImageData(unsigned int width, unsigned int height, unsigned int channelCount):width(width),height(height),channelCount(channelCount)
{
    //TODO only in debug mode
    if(channelCount==0)
    {
        ImGuiAl::Log::getInstance().Warn(
                "Cannot create image with channel count 0, creating with channel count 1 instead");
        this->channelCount = channelCount = 1;
    }
    if(channelCount>4)
    {
        ImGuiAl::Log::getInstance().Warn(
                "Cannot create image with channel count %d, creating with channel count 4 instead",channelCount);
        this->channelCount = channelCount = 4;
    }

    data = new unsigned char[width*height*channelCount];
}

ImageData::ImageData(const ImageData &other) : width(other.width), height(other.height), channelCount(other.channelCount)
{
    //deep copy array
    size_t size = width*height*channelCount;
    data = new unsigned char[size];
    std::memcpy(data,other.data,size);
}

ImageData::ImageData(ImageData &&other) noexcept : width(other.width), height(other.height), channelCount(other.channelCount), data(other.data)
{
    other.data = nullptr;
}

ImageData::~ImageData()
{
    delete[] data;
}

unsigned int ImageData::getWidth() const
{
    return width;
}

unsigned int ImageData::getHeight() const
{
    return height;
}

unsigned int ImageData::getChannelCount() const
{
    return channelCount;
}

Color ImageData::getPixel(unsigned int x, unsigned int y) const
{
    //TODO only in debug mode?
    if(!isInBounds(x,y))
    {
        ImGuiAl::Log::getInstance().Warn("Image read access at X: %d Y: %d is out of bounds (Width: %d, Height: %d)",x,y,width,height);
        return Color();
    }

    int idx = getIndex(x,y);
    Color c;
    switch(channelCount)
    {
        case 1:
            c.r = data[idx];
            c.g = c.r;
            c.b = c.r;
            c.a = 255;
        case 2:
            c.r = data[idx];
            c.g = c.r;
            c.b = c.r;
            c.a = data[idx+1];
        case 3:
            c.r = data[idx];
            c.g = data[idx+1];
            c.b = data[idx+2];
            c.a = 255;
        case 4:
            c.r = data[idx];
            c.g = data[idx+1];
            c.b = data[idx+2];
            c.a = data[idx+3];
        default:
            ImGuiAl::Log::getInstance().Warn("Image with channel count %d cannot be read",channelCount);
    }

    return c;
}

void ImageData::setPixel(unsigned int x, unsigned int y, const Color& c)
{
    if(!isInBounds(x,y))
    {
        ImGuiAl::Log::getInstance().Warn("Image write access at X: %d Y: %d is out of bounds (Width: %d, Height: %d)",x,y,width,height);
        return;
    }

    int idx = getIndex(x,y);
    switch(channelCount)
    {
        case 1:
            data[idx] = c.r;
            break;
        case 2:
            data[idx] = c.r;
            data[idx+1] = c.a;
            break;
        case 3:
            data[idx] = c.r;
            data[idx+1] = c.g;
            data[idx+2] = c.b;
            break;
        case 4:
            data[idx] = c.r;
            data[idx+1] = c.g;
            data[idx+2] = c.b;
            data[idx+3] = c.a;
            break;
        default:
            ImGuiAl::Log::getInstance().Warn("Image with channel count %d cannot be written to",channelCount);
    }
}

ImageData &ImageData::operator=(const ImageData &other)
{
    width=other.width;
    height=other.height;
    channelCount=other.channelCount;

    //deep copy
    size_t size = width*height*channelCount;
    unsigned char* newData = new unsigned char[size];
    std::memcpy(newData,other.data,size);

    //delete old array
    delete[] data;

    //assign new
    data = newData;

    return *this;

}

ImageData &ImageData::operator=(ImageData &&other) noexcept
{
    if(this!=&other)
    {
        width=other.width;
        height=other.height;
        channelCount=other.channelCount;

        //delete old value
        delete[] data;

        //move data
        data = other.data;

        //make data inaccessible in other object
        other.data = nullptr;
    }
    return *this;
}
