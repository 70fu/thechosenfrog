//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_RUNTIMECLASSES_H
#define CGUE19_THECHOSENFROG_RUNTIMECLASSES_H

#ifndef NDEBUG
    #define RT_VIRTUAL virtual
#else
    #define RT_VIRTUAL
#endif


enum RuntimeClassIds : unsigned char
{
    SOUND_LIST,
    MUSIC_LIST
};

namespace RuntimeClassNames
{
    static constexpr const char* SOUND_LIST = "SoundList";
    static constexpr const char* MUSIC_LIST = "MusicList";
};

#endif //CGUE19_THECHOSENFROG_RUNTIMECLASSES_H
