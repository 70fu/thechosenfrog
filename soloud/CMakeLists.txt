cmake_minimum_required(VERSION 3.12)
project(soloud)

#sources
set(SOLOUD_SOURCES src/core/soloud.cpp src/core/soloud_audiosource.cpp src/core/soloud_bus.cpp src/core/soloud_core_3d.cpp src/core/soloud_core_basicops.cpp src/core/soloud_core_faderops.cpp src/core/soloud_core_filterops.cpp src/core/soloud_core_getters.cpp src/core/soloud_core_setters.cpp src/core/soloud_core_voicegroup.cpp src/core/soloud_core_voiceops.cpp src/core/soloud_fader.cpp src/core/soloud_fft.cpp src/core/soloud_fft_lut.cpp src/core/soloud_file.cpp src/core/soloud_filter.cpp src/core/soloud_queue.cpp src/core/soloud_thread.cpp)
#set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/backend/sdl/soloud_sdl1.cpp src/backend/sdl/soloud_sdl2.cpp src/backend/sdl/soloud_sdl1_dll.c src/backend/sdl/soloud_sdl2_dll.c)

set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/monotone/soloud_monotone.cpp)
set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/openmpt/soloud_openmpt.cpp src/audiosource/openmpt/soloud_openmpt_dll.c)
set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/sfxr/soloud_sfxr.cpp)
set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/vic/soloud_vic.cpp)
set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/vizsn/soloud_vizsn.cpp)
set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/audiosource/wav/soloud_wav.cpp src/audiosource/wav/soloud_wavstream.cpp src/audiosource/wav/dr_impl.cpp src/audiosource/wav/stb_vorbis.c)

set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/filter/soloud_bassboostfilter.cpp src/filter/soloud_biquadresonantfilter.cpp src/filter/soloud_dcremovalfilter.cpp src/filter/soloud_dcremovalfilter.cpp src/filter/soloud_echofilter.cpp src/filter/soloud_fftfilter.cpp src/filter/soloud_flangerfilter.cpp src/filter/soloud_robotizefilter.cpp src/filter/soloud_waveshaperfilter.cpp)

#headers
set(SOLOUD_HEADERS include/soloud.h include/soloud_audiosource.h include/soloud_bus.h include/soloud_error.h include/soloud_fader.h include/soloud_fft.h include/soloud_fftfilter.h include/soloud_file.h include/soloud_file_hack_off.h include/soloud_file_hack_on.h include/soloud_filter.h include/soloud_internal.h include/soloud_queue.h include/soloud_thread.h)

set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_monotone.h)
set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_openmpt.h)
set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_sfxr.h)
set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_vic.h)
set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_vizsn.h)
set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_wav.h include/soloud_wavstream.h)

set(SOLOUD_HEADERS ${SOLOUD_HEADERS} include/soloud_bassboostfilter.h include/soloud_biquadresonantfilter.h include/soloud_dcremovalfilter.h include/soloud_echofilter.h include/soloud_flangerfilter.h include/soloud_robotizefilter.h include/soloud_waveshaperfilter.h)

set(SOLOUD_HEADERS ${SOLOUD_HEADERS} src/audiosource/wav/dr_flac.h src/audiosource/wav/dr_mp3.h src/audiosource/wav/dr_wav.h src/audiosource/wav/stb_vorbis.h)

#platform specific files
if (WIN32)
    set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/backend/winmm/soloud_winmm.cpp)
elseif (APPLE)
    set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/backend/coreaudio/soloud_coreaudio.cpp)
elseif (UNIX)
    #set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/backend/alsa/soloud_alsa.cpp)
    set(SOLOUD_SOURCES ${SOLOUD_SOURCES} src/backend/oss/soloud_oss.cpp)
endif()

#add lib
include_directories(include)
add_library(soloud ${SOLOUD_SOURCES} ${SOLOUD_HEADERS})

#enable correct backends
if (WIN32)
    target_compile_definitions(soloud PRIVATE WITH_WINMM)
elseif (APPLE)
    target_compile_definitions(soloud PRIVATE WITH_COREAUDIO)
elseif (UNIX)
    target_compile_definitions(soloud PRIVATE WITH_OSS)
    #target_compile_definitions(soloud PRIVATE WITH_OSS)
endif()
