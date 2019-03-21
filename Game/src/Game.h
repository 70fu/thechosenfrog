//
// Created by simon on 21.03.19.
//

#ifndef CGUE19_THECHOSENFROG_GAME_H
#define CGUE19_THECHOSENFROG_GAME_H


class Game {
public:
    void init();
    /**
     * @return true if game has not finished
     */
    bool update();
    void render();
    void cleanup();
};


#endif //CGUE19_THECHOSENFROG_GAME_H
