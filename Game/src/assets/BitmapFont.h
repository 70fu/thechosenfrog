#ifndef CGUE19_THECHOSENFROG_BITMAPFONT_H
#define CGUE19_THECHOSENFROG_BITMAPFONT_H


#include "Assets.h"
#include "Texture.h"
#include <stb_truetype.h>

/**
 * See stb_truetype documentation
 */
struct BitmapFontParameter
{
    TextureParameters bitmapParams={};

    unsigned int width=512;
    unsigned int height=512;
    unsigned int hOversample = 1;
    unsigned int vOversample = 1;
    float fontSize=64;//height in pixels
    unsigned char firstChar = ' ';
    unsigned char numChars = '~'-' ';
};

/**
 * Can allocate a bitmap texture on the GPU and stores character information.
 *
 * Usage:
 * 1) generateBitmap
 * 2) use
 * 3) cleanup
 * 4) repeat
 */
class BitmapFont
{
    friend class BitmapFontList;
private:
    static constexpr unsigned int MAX_CHARS = 128;
    TextureAsset bitmap;
    BitmapFontParameter parameters;
    stbtt_packedchar charData[MAX_CHARS];
    stbtt_fontinfo fontInfo;
    int ascent;
    int descent;
    int lineGap;

    bool bitmapGenerated = false;

    /**
     * Generates a bitmap with current from the contents of a ttf file
     * Ownership is taken of the fontData pointer
     * @param fontData
     * @param params
     */
    void generateBitmapFromTTF(unsigned char* fontData);
public:
    virtual ~BitmapFont();
    /**
     * Generates a bitmap with given parameters from the contents of a ttf file
     * Ownership is taken of the fontData pointer
     * @param fontData
     * @param params
     */
    void generateBitmapFromTTF(unsigned char *fontData, BitmapFontParameter params);
    void cleanup();

    const TextureAsset& getBitmap() const;
    const BitmapFontParameter& getParameters() const;

    /**
     *
     * @param text
     * @param quads, array of quads that is filled by the method
     * @param quadSize , size of quads
     * @param x
     * @param y
     * @param lineHeight amount of space between lines
     * @param wrapWidth, text is wrapped after this width, if negative, then no wrapping is applied
     * @return quadSize if quadSize>text.length(), otherwise text.length()
     */
    unsigned int fillQuadBuffer(const std::string& text, stbtt_aligned_quad* quads, unsigned int quadSize,float x, float y, float lineHeight, float wrapWidth);

};


#endif //CGUE19_THECHOSENFROG_BITMAPFONT_H
