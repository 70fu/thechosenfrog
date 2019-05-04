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
	EVENT_MANAGER,
	IMGUI_DEBUG_GUI,
	MESH_LIST,
	MATERIAL_LIST,
	SHADER_LIST,
	SHADER_PROGRAM_LIST,
	GAME_RENDERER,
	GAME_UPDATER,
	MAIN_SCENE,
	TEXTURE_LIST,
	CUBE_MAP_LIST,
	BITMAP_FONT_LIST
};

/**
 * These names must be exactly the class name
 */
namespace RuntimeClassNames
{
    static constexpr const char* SOUND_LIST = "SoundList";
    static constexpr const char* MUSIC_LIST = "MusicList";
	static constexpr const char* EVENT_MANAGER = "EventManager";
	static constexpr const char* IMGUI_DEBUG_GUI = "ImGuiDebugGUI";
	static constexpr const char* MESH_LIST = "MeshList";
	static constexpr const char* MATERIAL_LIST = "MaterialList";
	static constexpr const char* SHADER_LIST = "ShaderList";
	static constexpr const char* SHADER_PROGRAM_LIST = "ShaderProgramList";
	static constexpr const char* GAME_RENDERER = "GameRenderer";
	static constexpr const char* GAME_UPDATER = "GameUpdater";
	static constexpr const char* MAIN_SCENE = "MainScene";
	static constexpr const char* TEXTURE_LIST = "TextureList";
    static constexpr const char* CUBE_MAP_LIST = "CubeMapList";
    static constexpr const char* BITMAP_FONT_LIST = "BitmapFontList";
};

#endif //CGUE19_THECHOSENFROG_RUNTIMECLASSES_H
