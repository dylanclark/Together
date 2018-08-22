#ifndef utils_hpp
#define utils_hpp

#include <SDL2/SDL.h>

/********************/
/*   OPENGL UTILS   */
/********************/

void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

/******************/
/*   COLLISIONS   */
/******************/

class Vector
{
public:
    // init
    Vector();
    Vector(int new_x, int new_y) : x(new_x), y(new_y) { }

    // coords
    int x;
    int y;
};

bool check_collision(SDL_Rect a, SDL_Rect b, Vector* overlap);
bool check_point_in_triangle(int x, int y, bool color, SDL_Rect triangle, bool down, Vector* repos);
bool check_point_in_rect(int x, int y, bool color, SDL_Rect rect, Vector* repos);
bool check_grounded(SDL_Rect a, SDL_Rect b, bool a_color);
bool check_full_overlap(SDL_Rect a, SDL_Rect b);
bool check_in_platform(SDL_Rect a, SDL_Rect b);
bool check_touching_vert(SDL_Rect a, SDL_Rect b);

#endif /* utils_hpp */
