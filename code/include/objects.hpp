
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

typedef enum _ObjectType {
    OBJECT_SPRING = 0,
    OBJECT_BLOCK,
} ObjectType;

typedef enum _SpringStatus {
    SPRING_IDLE,
    SPRING_SPRUNG,
    SPRING_INACTIVE
} SpringStatus;

class Object
{
public:
    Object() { };

    virtual void render(Engine* game, SDL_Rect cam_rect, bool active_color) = 0;

    ObjectType get_type() { return m_type; }
    SDL_Rect get_rect() { return m_rect; }

    SDL_Rect m_rect;
    ObjectType m_type;
    Texture m_tex;
    bool m_color;
};

class Spring : public Object
{
public:
    Spring(Engine* game, int x, int y, bool color, float y_vel, SDL_Color palette);
    void render(Engine* game, SDL_Rect cam_rect, bool active_color);
    float get_yvel() { return m_yvel; }
    void spring();

private:
    SpringStatus m_status;
    float m_yvel;
    int spring_start;
};

#endif /* object_hpp */
