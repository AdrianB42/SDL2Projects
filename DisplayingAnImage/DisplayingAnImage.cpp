#include <iostream>
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Surface* windowSurface = NULL;
SDL_Surface* imageSurface = NULL;


bool Init();
bool loadMedia();
void close();

/*
This Init() function just takes the initialization code created in "IntroductionToSDL2" and abstracts it for better modularity
Returns: true if initialization was successful; false if not
*/
bool Init() {
    bool success = true;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_ERROR: %s\n" << SDL_GetError();
        success = false;
    }
    else
    {
        //Create window:              {WINDOW NAME}               {X Pos}                  {Y Pos}            {X Width}     {Y Width}   {Creation Flags: Display Window Upon Creation}
        window = SDL_CreateWindow("Displaying An Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        //If successful, SDL_CreateWindow initializes a SDL_Window, otherwise returns NULL
        if (window == NULL)
        {
            std::cout << "Window could not be created! SDL_ERROR: %s\n" << SDL_GetError();
            success = false;
        }
        else
        {
            //Get window surface, since we have already determined that the window isn't NULL, it must therefore have a surface that we can get
            windowSurface = SDL_GetWindowSurface(window);
        }
    }
    
    return success;
}

int main(int argc, char* args[])
{
    if (!Init()) {
        std::cerr << "Error Initializing Window";
    } 
    else {
        if (!loadMedia()) {
            std::cerr << "Failed to load media :(";
        }
        else {
            /*
            SDL_BlitSurface pastes the copy of one surface onto another
            In this case, since we want the windowSurface to be the "main" surface (as it is connected to the window)
            we blit all other surfaces onto it before then updating the window (which reads & displays the data on the main surface)
            */
            SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        }


        //Update the surface, the changes made to the surface won't be shown until this function is called
        SDL_UpdateWindowSurface(window);

        //Hack to get window to stay up
        SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
    }

    close();


    return 0;
}

bool loadMedia() {
    bool success = true;

    /*
    SDL_LoadBMP simply loads a bitmap based on the file location within the project directory
    */
    imageSurface = SDL_LoadBMP("Images/Image.bmp");

    if (imageSurface == NULL)
    {
        std::cout<< "Unable to load image %s! SDL Error: %s\n" << "Images/Image.bmp" << SDL_GetError();
        success = false;
    }
    return success;
}

void close() {

    //Deallocate imageSurface
    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    /*
    We dont have to worry about deallocating the windowSurface Surface since it is attached to the window, therefore SDL_DestroyWindow() will handle that
    */

    //Quit SDL subsystems
    SDL_Quit();
}