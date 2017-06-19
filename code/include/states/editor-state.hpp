#ifndef editor_state_hpp
#define editor_state_hpp

#include <stdio.h>

class EditorState : public Gamestate
{
public:
    void init(Engine* game);
    void cleanup();

    void update(Engine* game);
    void handle_events(Engine* game);
    void draw(Engine* game);
};

#endif /* editor_state_hpp */