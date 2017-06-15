
// using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <menu.hpp>
#include <textures.hpp>

MenuButton::MenuButton(bool select, int x, int y, int w, int h)
{
    selected = select;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void MenuButton::render(SDL_Renderer* rend)
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {tex->get_width(), 0, tex->get_width(), tex->get_height()};
    SDL_Rect inactive_clip = {0, 0, tex->get_width(), tex->get_height()};

    // render based on char status
    if (selected)
    {
        tex->render_button(&rect, &active_clip, rend);
    }
    else
    {
        tex->render_button(&rect, &inactive_clip, rend);
    }
}

MenuSlider::MenuSlider(bool select, int length, bool perm, int x, int y, int w, int h)
{
    selected = select;
    frames = length;
    cur_frame = 0;
    permanent = perm;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

MenuSlider::~MenuSlider()
{

}

void MenuSlider::render(SDL_Renderer* rend)
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {tex->get_width() * (cur_frame * 2 + 1), 0, tex->get_width(), tex->get_height()};
    SDL_Rect inactive_clip = {tex->get_width() * (cur_frame * 2), 0, tex->get_width(), tex->get_height()};

    // render based on char status
    if (selected)
    {
        tex->render_button(&rect, &active_clip, rend);
    }
    else
    {
        tex->render_button(&rect, &inactive_clip, rend);
    }
}

Title::Title(int x, int y, int w, int h)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void Title::render(SDL_Renderer* rend)
{
    SDL_Rect active_clip = {0, 0, tex->get_width(), tex->get_height()};

    tex->render_button(&rect, &active_clip, rend);
}

FadeIn::FadeIn(int w, int h, int time)
{
    rect.x = 0;
    rect.y = 0;
    rect.w = w;
    rect.h = h;
    alpha = 255;

    timer = time;
}

FadeIn::~FadeIn()
{
    tex->free();
}

void FadeIn::render(SDL_Renderer* rend)
{
    SDL_Rect clip = {0, 0, tex->get_width(), tex->get_height()};
    SDL_Rect render_rect = {rect.x + rect.w / 2, rect.y + rect.h / 2, rect.w, rect.h};

    tex->render_button(&render_rect, &clip, rend);
}

void FadeIn::update()
{
    timer -= 1;

    if (timer <= 0)
    {
        if (alpha > 0)
            alpha -= 1;
        else
            alpha = 0;
    }

    tex->set_alpha(alpha);
}

Menu::Menu(int w, int h, int timer)
{
    selector = 0;

    controller = new class Controller;

    fade_in = NULL;
    fade_in = new class FadeIn(w, h, timer);

    up = false;
    down = false;
    right = false;
    left = false;

    up_up = true;
    down_up = true;
    right_up = true;
    left_up = true;
}

bool Menu::handle_event(SDL_Event& e, Engine* game)
{


    switch (e.type)
    {
        case SDL_KEYDOWN:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_DOWN:
                    down = true;
                    break;
                case SDL_SCANCODE_UP:
                    up = true;
                    break;
                case SDL_SCANCODE_RIGHT:
                    right = true;
                    break;
                case SDL_SCANCODE_LEFT:
                    left = true;
                    break;
                case SDL_SCANCODE_SPACE:
                case SDL_SCANCODE_RETURN:
                    if (selector < sliders.size())
                    {
                        sliders[selector]->select(game);
                    }
                    else
                    {
                        buttons[selector - sliders.size()]->select(game);
                    }
                    break;
                case SDL_SCANCODE_ESCAPE:
                    return false;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.scancode)
            {
                case SDL_SCANCODE_DOWN:
                    down_up = true;
                    down = false;
                    break;
                case SDL_SCANCODE_UP:
                    up_up = true;
                    up = false;
                    break;
                case SDL_SCANCODE_LEFT:
                    left_up = true;
                    left = false;
                    break;
                case SDL_SCANCODE_RIGHT:
                    right_up = true;
                    right = false;
                    break;
                default:
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                    if (selector < sliders.size())
                    {
                        sliders[selector]->select(game);
                    }
                    else
                    {
                        buttons[selector - sliders.size()]->select(game);
                    }
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    up = true;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    down = true;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    right = true;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    left = true;
                    break;
                case SDL_CONTROLLER_BUTTON_START:
                    return false;
                    break;
            }
            break;
        case SDL_CONTROLLERBUTTONUP:
            switch (e.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    up_up = true;
                    up = false;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    down_up = true;
                    down = false;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    right_up = true;
                    right = false;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    left_up = true;
                    left = false;
                    break;
            }
            break;
    }
    return true;
}

void Menu::update(Engine* game)
{
    if (up && up_up)
    {
        selector = (selector + size - 1) % size;
        Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        up_up = false;
    }
    if (down && down_up)
    {
        selector = (selector + 1) % size;
        Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        down_up = false;
    }
    if (right && right_up)
    {
        if (selector < sliders.size())
        {
            sliders[selector]->cur_frame = (sliders[selector]->cur_frame + 1) % sliders[selector]->frames;
            if (sliders[selector]->permanent)
            {
                sliders[selector]->select(game);
            }
            Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        }
        right_up = false;
    }
    if (left && left_up)
    {
        if (selector < sliders.size())
        {
            sliders[selector]->cur_frame = (sliders[selector]->cur_frame + sliders[selector]->frames - 1) % sliders[selector]->frames;
            if (sliders[selector]->permanent)
            {
                sliders[selector]->select(game);
            }
            Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        }
        left_up = false;
    }
}