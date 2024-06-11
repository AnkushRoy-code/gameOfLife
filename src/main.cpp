#include <SDL2/SDL.h>
#include <SDL_mouse.h>
#include <cstdlib>
#include <iostream>

#include "simulation.h"

const int DESIRED_FPS = 12;
const int FRAME_PERIOD = 1000 / DESIRED_FPS;

int main() {
  int i = 0;

  std::cout << "\033[2J\033[1;1H";
  std::cout << "-----[GameOfLife]\n";
  std::cout // please excuse my formatter
      << "\n  This is a simulation of Conway's Game Of Life implemented by "
         "Ankush "
         "Roy.... by following a tutorial from youtube.\n  But! the youtuber "
         "made it using python and pygame, and I made it using C++ and "
         "SDL2! and moreover I also added some different features to the "
         "game!\n\n\n";

  std::cout
      << "  Press [1] and [ENTER] to Continue\n  Or Press [0] and [ENTER] "
         "to quit(safely)\n  > ";
  std::cin >> i;

  if (i == 0) {
    std::cout << "\033[2J\033[1;1H";
    exit(0);
  }

  std::cout << "\033[2J\033[1;1H";
  std::cout << "-----[CONTROLS]\n\n";
  std::cout << "  Lets talk about the controls as you "
               "won't know them if you didn't read it and no where else is the "
               "controls mentioned except the source code\n\n";
  std::cout << "      [q]                -> Quit the Simulation\n";
  std::cout << "      [Enter]            -> Run the Simulation\n";
  std::cout << "      [SpaceBar]         -> Pause the Simulation\n";
  std::cout << "      [r]                -> Render fill the Simulation { Can "
               "only be done if "
               "paused!! }\n";
  std::cout << "      [c]                -> Clear the Simulation { Can only "
               "done if paused !! }\n";
  std::cout
      << "      [MouseLeftClick]   -> Toggle the cell nearest to mouse in the "
         "Simulation\n";
  std::cout << "\n      { Unfortunately dragging doesn't work} \n\n";

  std::cout
      << "  Press [1] and [ENTER] to Continue\n  Or Press [0] and [ENTER] "
         "to quit(safely)\n  > ";

  std::cin >> i;

  if (i == 0) {
    std::cout << "\033[2J\033[1;1H";
    exit(0);
  }

  srand(time(NULL));
  const int CELL_SIZE = 20;
  const int WINDOW_HEIGHT = 700;
  const int WINDOW_WIDTH = 700;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_UTILITY);

  window = SDL_CreateWindow("GameOfLife", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            window_flags);

  if (window == nullptr) {
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr) {
    std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  SDL_Event event;
  bool quit = false;

  Uint32 frameStart, frameTime;

  Simulation simulation(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);

  while (!quit) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
      // event handling
      if (event.type == SDL_QUIT) {
        quit = true;
      }

      else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          int row = event.button.y / CELL_SIZE;
          int column = event.button.x / CELL_SIZE;
          simulation.ToggleCell(row, column);
        }
        if (event.button.button == SDL_BUTTON_MIDDLE) {
          // todo
        }
      }

      else if (event.type == SDL_KEYDOWN) {

        switch (event.key.keysym.sym) {

        case SDLK_RETURN:
          simulation.Start();
          SDL_SetWindowTitle(window, "GameOfLife");
          break;

        case SDLK_SPACE:
          simulation.Stop();
          SDL_SetWindowTitle(window, "GameOfLife - Paused");
          break;

        case SDLK_q:
          quit = true;
          break;

        case SDLK_r:
          simulation.CreateRandomState();
          break;

        case SDLK_c:
          simulation.ClearGrid();

        default:
          break;
        }
      }
    }

    simulation.Update();

    SDL_SetRenderDrawColor(renderer, 29, 29, 29, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    int offsetX = 0, offsetY = 0;

    simulation.Draw(renderer, offsetX, offsetY);
    // rendering stuff
    SDL_RenderPresent(renderer);

    frameTime = SDL_GetTicks() - frameStart;

    if (frameTime < FRAME_PERIOD) {
      SDL_Delay(FRAME_PERIOD - frameTime);
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  std::cout << "\033[2J\033[1;1H";
  return 0;
}
