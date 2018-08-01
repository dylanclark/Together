
// include headers
#include <engine.hpp>
#include <menu.hpp>
#include <states/menustate.hpp>

void MainMenuState::init()
{
    // create title
    title = new class Title(game->screen_width / 2,  game->screen_height / 4, 1600, 200);
    if (!title->tex.load_object(128, 16, "menu/title.png"))
    {
        printf("failed to load title texture!\n");
        return;
    }

    fade_in = new class FadeIn(100);

    // create buttons
    items.push_back(new class NewGameButton(game->screen_width / 2, game->screen_height / 2 - 40, 480, 80));
    items.push_back(new class ContinueButton(game->screen_width / 2, game->screen_height / 2 + 50, 480, 80));
    items.push_back(new class OptionsButton(game->screen_width / 2, game->screen_height / 2 + 140, 320, 80));
    items.push_back(new class QuitButton(game->screen_width / 2, game->screen_height / 2 + 230, 160, 80));

    // start bg music
    Mix_PlayMusic(game->sound->menu_music, -1);
}

void NewGameMenuState::init()
{
    title = new class Title (game->screen_width / 2,  game->screen_height / 3, 1280, 160);
    if (!title->tex.load_object(256, 32, "menu/new_game_title.png")) {
        printf("Failed to load new game title object!\n");
        return;
    }

    items.push_back(new class YesNewGameButton(game->screen_width / 2, game->screen_height / 2, 160, 80));
    items.push_back(new class NoButton(game->screen_width / 2, game->screen_height / 2 + 90, 160, 80));
}

void LevelSelectMenuState::init()
{
    items.push_back(new class LevelSlider(game->read_save(), false, game->screen_width / 2, game->screen_height / 2 - 45, 480, 80));
    items.push_back(new class BackButton(game->screen_width / 2, game->screen_height / 2 + 45, 320, 80));
}

void OptionsMenuState::init()
{
    items.push_back(new class VolumeSlider(5, true, game->screen_width / 2, game->screen_height / 2 - 135, 640, 80));
    items.push_back(new class SfxSlider(5, true, game->screen_width / 2, game->screen_height / 2 - 45, 640, 80));
    items.push_back(new class ControlsButton(game->screen_width / 2, game->screen_height / 2 + 45, 480, 80));
    items.push_back(new class BackButton(game->screen_width / 2, game->screen_height / 2 + 135, 320, 80));
}

void PauseMenuState::init()
{
    items.push_back(new class ResumeButton(game->screen_width / 2, game->screen_height / 2 - 135, 320, 80));
    items.push_back(new class LevelSelectButton(game->screen_width / 2, game->screen_height / 2 - 45, 640, 80));
    items.push_back(new class OptionsButton(game->screen_width / 2, game->screen_height / 2 + 45, 320, 80));
    items.push_back(new class SaveAndQuitButton(game->screen_width / 2, game->screen_height / 2 + 135, 640, 80));
}

void QuitMenuState::init()
{
    title = new class Title (game->screen_width / 2,  game->screen_height / 3, 640, 80);
    if (!title->tex.load_object(128, 16, "menu/quit_title.png")) {
        printf("Failed to load quit title object!\n");
    }

    items.push_back(new class YesQuitButton(game->screen_width / 2, game->screen_height / 2, 160, 80));
    items.push_back(new class NoButton(game->screen_width / 2, game->screen_height / 2 + 90, 160, 80));
}
