//
// Created by simon on 31.03.19.
//

#ifndef CGUE19_THECHOSENFROG_RUNTIMESOLOUD_H
#define CGUE19_THECHOSENFROG_RUNTIMESOLOUD_H

// RuntimeOpenGL.h
#pragma once
#include "RuntimeLinkLibrary.h"
#ifdef _WIN32
    RUNTIME_COMPILER_LINKLIBRARY( "libsoloud.lib");
#elif __APPLE__
    //RUNTIME_COMPILER_LINKLIBRARY("libsoloud.");//TODO apple
#elif __linux
    RUNTIME_COMPILER_LINKLIBRARY("-lsoloud");
#endif


#endif //CGUE19_THECHOSENFROG_RUNTIMESOLOUD_H
