#include <iostream>
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


/*
The arguments for the main() method are specific to SDL2 and must
be added in order to get it to work. It's done to allow for
multi-platform compatibility.
*/
int main(int argc, char* args[])
{
    /*
    The window we'll be rendering to. This is the container for the window itself (aka the border/container)
    */
    SDL_Window* window = NULL;

    /*
    The surface contained by the window. The surface is the actual
    collection of pixels that gets displayed onto the window, this is
    where the window's image actually lies
    */
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_ERROR: %s\n" << SDL_GetError();
    }
    else
    {
        //Create window:              {WINDOW NAME}               {X Pos}                  {Y Pos}            {X Width}     {Y Width}   {Creation Flags: Display Window Upon Creation}
        window = SDL_CreateWindow("Introduction To SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        //If successful, SDL_CreateWindow initializes a SDL_Window, otherwise returns NULL
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL_ERROR: %s\n" << SDL_GetError();
        }
        else
        {
            //Get window surface, since we have already determined that the window isn't NULL, it must therefore have a surface that we can get
            screenSurface = SDL_GetWindowSurface(window);

            //Fill the surface urple by creating a rectangle
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 210, 0, 210));

            //Update the surface, the changes made to the surface won't be shown until this function is called
            SDL_UpdateWindowSurface(window);

            //Hack to get window to stay up
            SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
        }
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}
