#ifndef CGUE19_THECHOSENFROG_SIGNGENERATOR_H
#define CGUE19_THECHOSENFROG_SIGNGENERATOR_H

#include <string>
#include <vec3.hpp>

class Game;
class TransformComponent;
namespace SignGenerator
{
    struct SignParameters
    {
        glm::vec3 translation={0,0,0};
        glm::vec3 rotation={0,0,0};
        std::string text="";
    };

    TransformComponent& makeSign(Game& game, SignParameters params);
}


#endif //CGUE19_THECHOSENFROG_SIGNGENERATOR_H
