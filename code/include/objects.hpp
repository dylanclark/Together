
#ifndef object_hpp
#define object_hpp

// using SDL and standard IO
#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include header files
#include <textures.hpp>
#include <char.hpp>
#include <engine.hpp>

// TODO
class Object;

typedef enum _buttonstatus {
    BUTT_INACTIVE = 0,
    BUTT_ACTIVATE = 1,
    BUTT_ACTIVE = 2,
    BUTT_INACTIVATE = 3
} buttonstatus;

static const int BUTT_ANIMATION_LENGTH = 4;

typedef enum _directions {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
} directions;

typedef enum _cratetype {
    FOUR_BY_TWO = 0,
    FOUR_BY_ONE = 1,
    TWO_BY_TWO = 2,
    THREE_BY_TWO = 3
} cratetype;

// max number of tiles that this crate will need to render
static const int MAX_BORDER = 12;

typedef enum _springstatus {
    BOARD_INACTIVE = 0,
    BOARD_ACTIVATE = 1,
    BOARD_ACTIVE = 2,
    BOARD_INACTIVATE = 3
} springstatus;

static const int BOARD_ANIMATION_LENGTH = 7;
static const int SPRING_X_VEL = 0;
static const int SPRING_Y_VEL = 10;

// direction
typedef enum _flipdir {
    FLIP_LEFT = 0,
    FLIP_RIGHT = 1
} flipdir;

#endif /* object_hpp */
