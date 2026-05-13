#include <iostream>
#include <SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


/*
An enum is used for sake of programmer readability
*/
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT, //0
    KEY_PRESS_SURFACE_UP, //1
    KEY_PRESS_SURFACE_DOWN, //2
    KEY_PRESS_SURFACE_LEFT, //3
    KEY_PRESS_SURFACE_RIGHT, //4
    KEY_PRESS_SURFACE_TOTAL //5
};

SDL_Window* window = NULL;
//Front buffer is what is displayed to the screen
SDL_Surface* frontBuffer = NULL;
/*
Back buffer handles drawing the images as they comes before blitting it to the front buffer
This method prevents the user from seeing half-rendered frames.
*/
SDL_Surface* backBuffer = NULL;

/*
Array that stores a surface corresponding to each image
*/
SDL_Surface* keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL] = {NULL};

// Surface for the exit image
SDL_Surface* exitSurface = NULL;



bool Init();
bool loadMedia();
SDL_Surface* loadSurface(std::string path);
void close();
void handleEvents();

/*
This Init() function just takes the initialization code created in "IntroductionToSDL2" and abstracts it for better modularity
Returns: true if initialization was successful; false if not
*/
bool Init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "\nSDL could not initialize! SDL_ERROR: " << SDL_GetError();
        success = false;
    }
    else
    {
        //Create window:            {WINDOW NAME}            {X Pos}                  {Y Pos}            {X Width}     {Y Width}   {Creation Flags: Display Window Upon Creation}
        window = SDL_CreateWindow("Handling Inputs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        //If successful, SDL_CreateWindow initializes a SDL_Window, otherwise returns NULL
        if (window == NULL)
        {
            std::cerr << "\nWindow could not be created! SDL_ERROR: " << SDL_GetError();
            success = false;
        }
        else
        {
            //Get window surface, since we have already determined that the window isn't NULL, it must therefore have a surface that we can get
            frontBuffer = SDL_GetWindowSurface(window);
        }
    }

    return success;
}

int main(int argc, char* args[])
{
    if (!Init()) {
        std::cerr << "\nError Initializing Window";
    }
    else {
        if (!loadMedia()) {
            std::cerr << "\nFailed to load media :(";
        }
        else {
            handleEvents();
        }

    }

    close();


    return 0;
}


/*
Loads all of the images to their corresponding surfaces
*/
bool loadMedia() {
    bool success = true;

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/Image.bmp");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    {
        std::cout<< "\nFailed to load default image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Images/ImageUp.bmp");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
    {
        std::cout << "\nFailed to load up image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Images/ImageDown.bmp");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
    {
        std::cout << "\nFailed to load down image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Images/ImageLeft.bmp");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
    {
        std::cout << "\nFailed to load left image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Images/ImageRight.bmp");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
    {
        std::cout << "\nFailed to load right image!";
        success = false;
    }

    exitSurface = loadSurface("Images/Exit.bmp");
    if (exitSurface == NULL) {
        std::cout << "\nFailed to load exit image!";
        success = false;
    }

    return success;
}

SDL_Surface* loadSurface(std::string path) {

    /*
    SDL_LoadBMP simply loads a bitmap based on the file location within the project directory
    */
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image %s! SDL Error: " << path << ", " << SDL_GetError();
    }
    return loadedSurface;
}

void handleEvents(){
    /*
    Event handling loop. each event gets read into e
    */
    SDL_Event e;
    bool quit = false;
    short numberOfQuitEvents = 0;
    //basic quit (game) loop
    while (quit == false) {
        /*
        SDL_PollEvent() reads in the last event that was recorded by the program
        This loop is saying "While the number of queued events is != 0..."
        This prevents the loop from running excessively in the case that there are
        no events to handle
        */
        while (SDL_PollEvent(&e) != 0) {
            /*
            SDL_QUIT means hitting the "X" button on the window, if the quit flag isn't
            set to true, the program wont be able to close by clicking it, which is slightly
            humorous.
            */
            if (e.type == SDL_QUIT) {
                numberOfQuitEvents++;
                backBuffer = exitSurface;
                if (numberOfQuitEvents == 2){
                    quit = true;
                }
                
            }
            else if (e.type = SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_UP:
                        backBuffer = keyPressSurfaces[KEY_PRESS_SURFACE_UP];
                        break;
                    case SDLK_DOWN:
                        backBuffer = keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                        break;
                    case SDLK_LEFT:
                        backBuffer = keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                        break;
                    case SDLK_RIGHT:
                        backBuffer = keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                        break;
                    default:
                        backBuffer = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                        break;


                }
            }
        }

        /*
        SDL_BlitSurface pastes the copy of one surface onto another
        In this case, since we want the windowSurface to be the "main" surface (as it is connected to the window)
        we blit all other surfaces onto it before then updating the window (which reads & displays the data on the main surface)
        */
        SDL_BlitSurface(backBuffer, NULL, frontBuffer, NULL);

        //Update the surface, the changes made to the surface won't be shown until this function is called
        SDL_UpdateWindowSurface(window);
    }
}

void close() {

    //Deallocate all loaded surfaces
    for (std::size_t i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
        SDL_FreeSurface(keyPressSurfaces[i]);
        keyPressSurfaces[i] = NULL;
    }
    
    //Deallocate exit surface
    SDL_FreeSurface(exitSurface);
    exitSurface = NULL;

    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;
    /*
    We dont have to worry about deallocating the windowSurface Surface since it is attached to the window, therefore SDL_DestroyWindow() will handle that

    The back buffer doesn't need to be deallocated since it doesn't own any of the surfaces itself
    All surfaces are owned by their own independent variables (either KeyPressSurfaces[] or exitSurface)
    */
   

    //Quit SDL subsystems
    SDL_Quit();
}

