#ifndef CGUE19_THECHOSENFROG_IMAGEDATA_H
#define CGUE19_THECHOSENFROG_IMAGEDATA_H

#include "../util/Color.h"
#include <glad/glad.h>
#include <string>

/**
 * Is an editable Image with fixed size and number of channels in RAM
 * Channel count may be:
 * 1 - gray image
 * 2 - gray image with alpha
 * 3 - rgb
 * 4 - rgba
 */
class ImageData
{
    friend class TextureList;
    friend class Texture;
    friend class CubeMapList;
    friend class CubeMap;
private:
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int channelCount = 0;
    unsigned char* data = nullptr;

    ImageData();

    bool isInBounds(unsigned int x, unsigned int y) const;
    unsigned int getIndex(unsigned int x, unsigned int y) const;

    bool loadFromFile(const std::string& path);
public:
    /**
     * @param width
     * @param height
     * @param channelCount is clamped to 1-4
     */
    ImageData(unsigned int width, unsigned int height, unsigned int channelCount);
    ImageData(const ImageData& other);
    ImageData(ImageData&& other) noexcept;
    virtual ~ImageData();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getChannelCount() const;

    /**
     * @param x >=0 && <width
     * @param y >=0 && <height
     * @return color at given position
     */
    Color getPixel(unsigned int x, unsigned int y) const;
    /**
     * If channelCount is:
     * 1 - color.r is used
     * 2 - color.r and color.a is used
     * 3 - color.r, color.g, color.b is used
     * 4 - all components on color are used
     * @param x >=0 && <width
     * @param y >=0 && <height
     * @param color is the new color at given position
     */
    void setPixel(unsigned int x, unsigned int y, const Color& color);

    /**
     * @return pointer to the bit data
     */
    unsigned char* getData();

    GLenum getInternalFormat() const;
    GLenum getFormat() const;

    ImageData& operator=(const ImageData& other);
    ImageData& operator=(ImageData&& other) noexcept;

    //TODO Nice to have () operator, which returns a ref to a color through which you can change the portion of the array directly
    //Color& operator()(unsigned int x, unsigned int y);
};


#endif //CGUE19_THECHOSENFROG_IMAGEDATA_H
