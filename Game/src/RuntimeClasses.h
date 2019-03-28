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

/**
 * Do not change order!!!
 */
enum RuntimeClassIds : unsigned char
{
    SOUND_LIST,
    MUSIC_LIST,
	EVENT_MANAGER
};

/**
 * These names must be exactly the class name
 */
namespace RuntimeClassNames
{
    static constexpr const char* SOUND_LIST = "SoundList";
    static constexpr const char* MUSIC_LIST = "MusicList";
	static constexpr const char* EVENT_MANAGER = "EventManager";
};

#endif //CGUE19_THECHOSENFROG_RUNTIMECLASSES_H
