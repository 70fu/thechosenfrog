#ifndef CGUE19_THECHOSENFROG_IMAGEDATA_H
#define CGUE19_THECHOSENFROG_IMAGEDATA_H

#include "../util/Color.h"

//TODO documentation
class ImageData
{
    //TODO check if channel count is valid (1-4)
    friend class TextureList;
    friend class Texture;
private:
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int channelCount = 0;
    unsigned char* data = nullptr   ;

    ImageData();

    bool isInBounds(unsigned int x, unsigned int y) const;
    unsigned int getIndex(unsigned int x, unsigned int y) const;
public:
    ImageData(unsigned int width, unsigned int height, unsigned int channelCount);
    ImageData(const ImageData& other);
    ImageData(ImageData&& other) noexcept;
    virtual ~ImageData();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getChannelCount() const;

    Color getPixel(unsigned int x, unsigned int y) const;
    void setPixel(unsigned int x, unsigned int y, const Color& color);

    ImageData& operator=(const ImageData& other);
    ImageData& operator=(ImageData&& other) noexcept;

    //TODO Nice to have () operator, which returns a ref to a color through which you can change the portion of the array directly
    //Color& operator()(unsigned int x, unsigned int y);
};


#endif //CGUE19_THECHOSENFROG_IMAGEDATA_H
