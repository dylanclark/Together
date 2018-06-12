
#ifndef levelstate_hpp
#define levelstate_hpp

// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// include headers
#include <engine.hpp>
#include <char.hpp>
#include <levels.hpp>

class Level1State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level2State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level3State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level4State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level5State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level6State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level7State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level8State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

class Level9State : public Levelstate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void draw(Engine* game);

    void init_objects(Engine* game);
    void interactions(Engine* game);
};

#endif /* levelstate_hpp */
