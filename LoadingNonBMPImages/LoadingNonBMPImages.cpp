#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <array>

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
std::array<SDL_Surface*, KEY_PRESS_SURFACE_TOTAL> keyPressSurfaces = { NULL };

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
        //Create window:           {WINDOW NAME}           {X Pos}                  {Y Pos}            {X Width}     {Y Width}   {Creation Flags: Display Window Upon Creation}
        window = SDL_CreateWindow("Loading PNGs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        //If successful, SDL_CreateWindow initializes a SDL_Window, otherwise returns NULL
        if (window == NULL)
        {
            std::cerr << "\nWindow could not be created! SDL_ERROR: " << SDL_GetError();
            success = false;
        }
        else
        {   
            /*
            Now we must initialize the SDL_image loading
            */
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
                success = false;
            }
            else
            {
                //Get window surface, since we have already determined that the window isn't NULL, it must therefore have a surface that we can get
                frontBuffer = SDL_GetWindowSurface(window);
            }

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
    keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/Image.png");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    {
        std::cout << "\nFailed to load default image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Images/ImageUp.png");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
    {
        std::cout << "\nFailed to load up image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Images/ImageDown.png");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
    {
        std::cout << "\nFailed to load down image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Images/ImageLeft.png");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
    {
        std::cout << "\nFailed to load left image!";
        success = false;
    }

    //Load default surface
    keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Images/ImageRight.png");
    if (keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
    {
        std::cout << "\nFailed to load right image!";
        success = false;
    }

    exitSurface = loadSurface("Images/Exit.png");
    if (exitSurface == NULL) {
        std::cout << "\nFailed to load exit image!";
        success = false;
    }

    return success;
}

SDL_Surface* loadSurface(std::string path) {

    /*
    optimizedSurface will take the raw image surface and optimize it for blitting
    The reason it optimizes the surface is because, by default, the format of the bmp
    doesn't match the format of the screen (typically .bmp are 24bit but the ones I've been
    using have been 256bit (and now im not even using .bmps anymore!), however a screen is typically 32bit), so every time the screen
    gets blitted, the processor has to convert the surface to match the screen. Instead of
    that, we can just convert the surface beforehand so it doesn't need to be converted each
    time.
    */
    SDL_Surface* optimizedSurface = NULL;

    /*
    IMG_Load simply loads any image file based on the file location within the project directory
    */
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    //Loading the original surface
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image %s! SDL Error: " << path << ", " << IMG_GetError();
    }
    //If the surface is valid, optimize it
    else {
        /*
        SDL_ConvertSurface takes in a source surface and converts its format into another
        We get the frontBuffer's format as it is tied to the window and therefore gets us
        the window's format to use when converting.
        */
        optimizedSurface = SDL_ConvertSurface(loadedSurface, frontBuffer->format, 0);

        if (optimizedSurface == NULL) {
            std::cerr << "Unable to optimize image %s! SDL Error: " << path.c_str() << SDL_GetError();
            /*
            As a backup, if the surface is unable to be optimized we will just return the original
            */
            return loadedSurface;
        }

        //Get rid of old surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return the optimized surface
    return optimizedSurface;
}

void handleEvents() {
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
                if (numberOfQuitEvents == 2) {
                    quit = true;
                }

            }
            else if (e.type = SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
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
        //SDL_BlitSurface(backBuffer, NULL, frontBuffer, NULL);

        /*
        This rectangle defines the dimensions of the scaled image, in this case, it is made to take up half the screen width/height
        and to be displayed centered
        */
        SDL_Rect scaleRect;
        scaleRect.x = SCREEN_WIDTH / 4;
        scaleRect.y = SCREEN_HEIGHT / 4;
        scaleRect.w = SCREEN_WIDTH / 2;
        scaleRect.h = SCREEN_HEIGHT / 2;
        SDL_BlitScaled(backBuffer, NULL, frontBuffer, &scaleRect);


        //Update the surface, the changes made to the surface won't be shown until this function is called
        SDL_UpdateWindowSurface(window);
    }
}

void close() {

    //Deallocate all loaded surfaces
    for (std::size_t i = 0; i < keyPressSurfaces.size(); i++) {
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



