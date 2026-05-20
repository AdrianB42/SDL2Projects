#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = nullptr;

/*
Rather than using front and back buffers, the double buffer system will be handled
entirely by one GPU renderer
*/
SDL_Renderer* renderer = nullptr;

/*
SDL_Texture is how we are going to create an image to send to the renderer
*/
SDL_Texture* texture = nullptr;

bool Init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
void close();
void handleEvents();

/*
Initializes the window, renderer, and image loading
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
        if (window == nullptr)
        {
            std::cerr << "\nWindow could not be created! SDL_ERROR: " << SDL_GetError();
            success = false;
        }
        else
        {
            /*
            Now we initialize the renderer
            */
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == nullptr) {
                std::cerr << "\nRenderer could not be created! SDL Error: " << SDL_GetError();
                success = false;

            }
            else {
                //Initialize render color (initial window color)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                /*
                Finally we must initialize the SDL_image loading
                */
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cerr << "\nSDL_image could not initialize! SDL_image Error: " << IMG_GetError();
                    success = false;
                }
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
            std::cerr << "\nError loding media";
        }
        else {
            handleEvents();
        }
    }
    close();

    return 0;
}

SDL_Texture* loadTexture(std::string path) {
    

    /*
    This is the texture that will be returned
    */
    SDL_Texture* newTexture = nullptr;

    /*
    IMG_Load simply loads any image file based on the file location within the project directory
    */
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    //Loading the original surface
    if (loadedSurface == nullptr)
    {
        std::cout << "\nUnable to load image %s! SDL Error: " << path << ", " << IMG_GetError();
    }
    //If the surface is valid, convert to a texture
    else {
        /*
        SDL_CreateTextureFromSurface 
        */
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

        if (newTexture == nullptr) {
            std::cerr << "\nUnable to create texture! SDL Error: " << path.c_str() << SDL_GetError();
        }

        //Get rid of old surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return the texture
    return newTexture;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load PNG texture
    texture = loadTexture("Images/Image.png");
    if (texture == nullptr)
    {
        std::cerr << "\nFailed to load texture image!";
        success = false;
    }

    return success;
}

void handleEvents() {
    /*
    Event handling loop. each event gets read into e
    */
    SDL_Event e;
    bool quit = false;
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
                quit = true;
            }
        }

        //Clear Screen
        SDL_RenderClear(renderer);

        //Render texture to screen
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        //Update screen
        SDL_RenderPresent(renderer);
    }
}

void close() {

    //Destroy texture
    SDL_DestroyTexture(texture);
    texture = nullptr;

    //Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    //Destroy window
    SDL_DestroyWindow(window);
    window = nullptr;

    //Quit SDL subsystems
    SDL_Quit();
    IMG_Quit();
}