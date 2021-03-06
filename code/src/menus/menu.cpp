
// using SDL and standard IO
#include <stdio.h>
#include <glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>

// include headers
#include <engine.hpp>
#include <menu.hpp>
#include <textures.hpp>

MenuButton::MenuButton(int x, int y, int w, int h)
{
    type = MENU_BUTTON;
    selected = false;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void MenuButton::render()
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {tex.get_width(), 0, tex.get_width(), tex.get_height()};
    SDL_Rect inactive_clip = {0, 0, tex.get_width(), tex.get_height()};

    if (selected) {
        // tex.render_button(&rect, &active_clip);
    } else {
        // tex.render_button(&rect, &inactive_clip);
    }
}

MenuSlider::MenuSlider(int num_states, bool is_permanent, int x, int y, int w, int h)
{
    type = MENU_SLIDER;
    selected = false;
    frames = num_states;
    cur_frame = 0;
    permanent = is_permanent;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void MenuSlider::render()
{
    // determine which chunk of the texture to render
    SDL_Rect active_clip = {tex.get_width() * (cur_frame * 2 + 1), 0, tex.get_width(), tex.get_height()};
    SDL_Rect inactive_clip = {tex.get_width() * (cur_frame * 2), 0, tex.get_width(), tex.get_height()};

    // render based on char status
    if (selected) {
        // tex.render_button(&rect, &active_clip);
    } else {
        // tex.render_button(&rect, &inactive_clip);
    }
}

Title::Title(int x, int y, int w, int h)
{
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void Title::render()
{
    SDL_Rect active_clip = {0, 0, tex.get_width(), tex.get_height()};
    // tex.render_button(&rect, &active_clip);
}

FadeIn::FadeIn(int time)
{
    rect.x = 0;
    rect.y = 0;
    rect.w = game->screen_width;
    rect.h = game->screen_height;
    alpha = 255;

    timer = time;
    if (tex.load_object(16, 16, "white/background/white_back.png")) {
        printf("failed to load fade-in object!\n");
    }
    // tex.set_blend_mode(SDL_BLENDMODE_BLEND);
}

void FadeIn::render()
{
    SDL_Rect clip = {0, 0, tex.get_width(), tex.get_height()};
    SDL_Rect render_rect = {rect.x + rect.w / 2, rect.y + rect.h / 2, rect.w, rect.h};
    // tex.render_button(&render_rect, &clip);
}

void FadeIn::update()
{
    timer -= 1;

    if (timer <= 0) {
        if (alpha > 0) {
            alpha -= 1;
        } else {
            alpha = 0;
        }
    }
    // tex.set_alpha(alpha);
}

Menu::Menu()
{
    // cursor for selecting starts at the top
    selector = 0;

    // initialize these so our checks in draw work
    title = NULL;
    fade_in = NULL;

    // controls
    controller = new class Controller;
    up = down = right = left = false;
}

void Menu::handle_events()
{
    // event handler
    SDL_Event e;

    // handle those events, bruh
    while (SDL_PollEvent(&e))
    {
        if (fade_in) {
            if (fade_in->timer > 0) {
                continue;
            }
        }

        switch (e.type)
        {
            case SDL_QUIT:
                game->quit();
                break;
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
                        items[selector]->select();
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        Mix_ResumeMusic();
                        Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
                        game->pop_state();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                switch (e.cbutton.button)
                {
                    case SDL_CONTROLLER_BUTTON_A:
                        items[selector]->select();
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
                    case SDL_CONTROLLER_BUTTON_B:
                        Mix_ResumeMusic();
                        Mix_PlayChannel(-1, game->sound->menu_select_snd, 0);
                        game->pop_state();
                        break;
                }
                break;
        }
    }
}

void Menu::update()
{
    // change selector based on controller inputs
    if (down || up) {
        selector = (selector + (down - up)) % items.size();
        Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        up = down = false;
    }
    if (right || left) {
        if (items[selector]->type == MENU_SLIDER) {
            MenuSlider* slider = static_cast<MenuSlider*>(items[selector]);
            slider->cur_frame = (slider->cur_frame + slider->frames + (right - left)) % slider->frames;
            if (slider->permanent) {
                slider->select();
            }
            Mix_PlayChannel(-1, game->sound->menu_choose_snd, 0);
        }
        left = right = false;
    }

    // clear the window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // update fade
    if (fade_in) {
        fade_in->update();
    }

    // make sure each button/slider knows whether it's selected or not
    for (int i = 0; i < items.size(); i++) {
        items[i]->selected = (i == selector);
    }
}

void Menu::draw()
{
    if (title) {
        title->render();
    }

    for (int i = 0; i < items.size(); i++) {
        items[i]->render();
    }

    if (fade_in) {
        fade_in->render();
    }

    SDL_GL_SwapWindow(game->window);
}

void Menu::cleanup()
{

}
