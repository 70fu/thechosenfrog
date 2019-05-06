#include "BitmapFont.h"
#include "../logger/imguial_log.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

using namespace ImGuiAl;

BitmapFont::~BitmapFont()
{
    if(bitmapGenerated)
    {
        Log::getInstance().Warn("cleanup has not been called on bitmap font, call ignored");
        cleanup();
    }
}

void BitmapFont::generateBitmapFromTTF(unsigned char *fontData)
{
    if(bitmapGenerated)
    {
        Log::getInstance().Warn("generateBitmapFromTTF called even though bitmap has already been generated, call ignored");
        return;
    }

    //load font info
    if(!stbtt_InitFont(&fontInfo,fontData,0))
    {
        Log::getInstance().Error("Failed to init font");
        return;
    }

    //load some font metrics
    stbtt_GetFontVMetrics(&fontInfo,&ascent,&descent,&lineGap);

    //scale font metrics
    float scale = stbtt_ScaleForPixelHeight(&fontInfo,parameters.fontSize);
    ascent*=scale;
    descent*=scale;
    lineGap*=scale;

    //check if all characters can be stored
    if(parameters.numChars>MAX_CHARS)
    {
        Log::getInstance().Warn("%d characters have been requested to load, but only %d can be loaded into one bitmap",parameters.numChars,MAX_CHARS);
        parameters.numChars = MAX_CHARS;
    }

    //initialize image
    bitmap.data=ImageData(parameters.width,parameters.height,1);

    //pack bitmap
    stbtt_pack_context context;
    if(!stbtt_PackBegin(&context,bitmap.data.getData(),parameters.width,parameters.height,0,1,nullptr))
    {
        Log::getInstance().Error("Failed to initialize font packing");
        return;
    }
    stbtt_PackSetOversampling(&context,parameters.hOversample,parameters.vOversample);
    if(!stbtt_PackFontRange(&context,fontData,0,parameters.fontSize,parameters.firstChar,parameters.numChars,&charData[0]))
    {
        Log::getInstance().Error("Failed to pack font");
        return;
    }
    stbtt_PackEnd(&context);

    //generate bitmap texture
    bitmap.allocateOnGPU(parameters.bitmapParams);

    bitmapGenerated = true;
}

void BitmapFont::generateBitmapFromTTF(unsigned char *fontData, BitmapFontParameter params)
{
    parameters = params;

    generateBitmapFromTTF(fontData);
}

void BitmapFont::cleanup()
{
    if(!bitmapGenerated)
    {
        Log::getInstance().Warn("Cannot cleanup a bitmap, which has not been generated yet, call ignored");
        return;
    }

    bitmap.cleanup();
    delete[] fontInfo.data;

    bitmapGenerated = false;
}

const TextureAsset& BitmapFont::getBitmap() const
{
    return bitmap;
}

const BitmapFontParameter &BitmapFont::getParameters() const
{
    return parameters;
}

unsigned int
BitmapFont::fillQuadBuffer(const std::string &text, stbtt_aligned_quad *quads, unsigned int quadSize, float x, float y,
                           float lineHeight, float wrapWidth)
{
    if(wrapWidth<0)
        wrapWidth = std::numeric_limits<float>::max();

    float currentX = x;
    float currentY = y;
    unsigned int quadIndex = 0;
    for(unsigned int i = 0;quadIndex<quadSize && i<text.length();++i)
    {
        //wrapping
        if(currentX >= wrapWidth || text[i]=='\n')
        {
            currentX = x;
            currentY+=lineHeight+parameters.fontSize;

            if(text[i]=='\n')
                continue;
        }

        stbtt_GetPackedQuad(charData,parameters.width,parameters.height,text[i]-parameters.firstChar,&currentX,&currentY,&quads[quadIndex],0);
        ++quadIndex;
    }

    return quadIndex;
}


