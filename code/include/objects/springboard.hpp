
#ifndef springboard_hpp
#define springboard_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
<<<<<<< HEAD
<<<<<<< HEAD:together/springboard.hpp
#include "characters.hpp"
#include "initialization.hpp"
#include "textures.hpp"
#include "collisions.hpp"
#include "tiles.hpp"
#include "camera.hpp"
#include "button.hpp"
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
#include <characters/characters.hpp>
#include <engine/init.hpp>
#include <textures/textures.hpp>
#include <characters/collisions.hpp>
#include <levels/tiles.hpp>
#include <levels/camera.hpp>

<<<<<<< HEAD
>>>>>>> 201bf90... gigantic makefile revamp:code/include/objects/springboard.hpp
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca


// button status constants
const int BOARD_INACTIVE = 0;
const int BOARD_ACTIVATE = 1;
const int BOARD_ACTIVE = 2;
const int BOARD_INACTIVATE = 3;

// animation length
const int BOARD_ANIMATION_LENGTH = 7;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
// direction
const int FLIP_LEFT = 0;
const int FLIP_RIGHT = 1;

const int LOCATION = 0;
const int VELOCITY = 1;

<<<<<<< HEAD
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca

class springboard
{
public:
    // initialize traits
    springboard();

    // render the item on the screen
    void render(SDL_Rect* camera, engine* game);

    // collision rectangle
    SDL_Rect col_rect;

    // texture
    texture tex;

    // check for collision with dot
    bool check(SDL_Rect dot_rect);

    // activated or not
    bool activated;
<<<<<<< HEAD
<<<<<<< HEAD:together/springboard.hpp
    
=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca

    // shown or not
    bool show;

<<<<<<< HEAD
>>>>>>> 201bf90... gigantic makefile revamp:code/include/objects/springboard.hpp
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    // direction
    int direction;

    // animation status
    int status;

    // frame of animation
    int frame;

    // type. can be LOCATION for defualt jumping or VELOCITY for jumping the
    // abount of the dot doing the springing
    int type;

    // angle to flip
    double angle = 0.0;

    // flip axis point
    SDL_Point* center = NULL;

    // flip type
<<<<<<< HEAD
<<<<<<< HEAD
    SDL_RendererFlip flip = SDL_FLIP_NONE;

=======
=======
>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
    SDL_RendererFlip flip_type;

    // spring velocity
    float x_spring;

    float y_spring;

    // default velocity and acceleration constants
    const float SPRING_X_VEL = 0;
    const float SPRING_Y_VEL = 10;

    // cross-layer spring logic
    void cross_spring(dot* springer, dot* springee, int type);
<<<<<<< HEAD
<<<<<<< HEAD:together/springboard.hpp
>>>>>>> 250bb44... cross layer, bug fixes, levels 1 and 2,
    
=======

>>>>>>> 201bf90... gigantic makefile revamp:code/include/objects/springboard.hpp
=======

>>>>>>> ec65ad696ba84fae2cc740a44722ec668b7193ca
};

#endif /* springboard_hpp */

