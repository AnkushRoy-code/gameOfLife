#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "simulation.h"

const int DESIRED_FPS = 12;
const int FRAME_PERIOD = 1000 / DESIRED_FPS;

bool checkLineContent(const std::string &filename, int lineNumber,
                      const std::string &targetString);

int main() {
  if (!checkLineContent("data/NotFirstTime.txt", 2, "true")) {
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
    std::cout
        << "  Lets talk about the controls as you "
           "won't know them if you didn't read it and no where else is the "
           "controls mentioned except the source code\n\n";
    std::cout << "      [q]                -> Quit the Simulation\n";
    std::cout << "      [Enter]            -> Run the Simulation\n";
    std::cout << "      [SpaceBar]         -> Pause the Simulation\n";
    std::cout << "      [m]                -> Centering the simulation after "
                 "panning\n";
    std::cout << "      [r]                -> Render fill the Simulation { Can "
                 "only be done if "
                 "paused!! }\n";
    std::cout << "      [c]                -> Clear the Simulation { Can only "
                 "done if paused !! }\n";
    std::cout << "      [MouseLeftClick]   -> Toggle the cell nearest to mouse "
                 "in the Simulation\n";
    std::cout << "      [MouseMiddleClick] -> For panning the simulation\n";
    std::cout << "  \n\nPress [1] and [ENTER] to Continue\n  Or Press [0] and "
                 "[ENTER] "
                 "to quit(safely)\n  > ";

    std::cin >> i;

    if (i == 0) {
      std::cout << "\033[2J\033[1;1H";
      exit(0);
    }
  }

  // the logic starts here
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

  float offsetX = 0.0f, offsetY = 0.0f;
  float offsetEndX = 0.0f, offsetEndY = 0.0f;

  float scale = 1.0f;

  bool leftMouseButtonPressed = false;
  bool middleMouseButtonPressed = false;
  bool alive;

  float mouseStartPanX = 0.0f, mouseStartPanY = 0.0f;

  while (!quit) {
    frameStart = SDL_GetTicks();

    while (SDL_PollEvent(&event) != 0) {
      // event handling
      if (event.type == SDL_QUIT) {
        quit = true;
      }

      else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
          leftMouseButtonPressed = true;
          int x, y;
          SDL_GetMouseState(&x, &y); // Get current mouse position

          int row = ((y / scale) - offsetY) / CELL_SIZE;
          int column = ((x / scale) - offsetX) / CELL_SIZE;

          if (simulation.getValue(row, column) == 0) {
            alive = true;
          } else {
            alive = false;
          }
        } else if (event.button.button == SDL_BUTTON_MIDDLE) {
          middleMouseButtonPressed = true;
          int x, y;
          SDL_GetMouseState(&x, &y); // Get current mouse position

          mouseStartPanX = x / scale;
          mouseStartPanY = y / scale;
        }
      } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_MIDDLE) {
          middleMouseButtonPressed = false;
          offsetEndX = offsetX;
          offsetEndY = offsetY;
        } else if (event.button.button == SDL_BUTTON_LEFT) {
          leftMouseButtonPressed = false;
        }
      }

      else if (event.type == SDL_MOUSEWHEEL) {
        if (event.wheel.y > 0) {
          scale *= 1.05f;

        } else if (event.wheel.y < 0) {
          scale *= 0.95f;
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
          break;

        case SDLK_m:
          offsetY = 0.0f;
          offsetX = 0.0f;
          offsetEndY = 0.0f;
          offsetEndX = 0.0f;
          break;
        case SDLK_z:
          scale = 1.0f;
          break;

        default:
          break;
        }
      }
    }

    if (leftMouseButtonPressed) {
      int x, y;
      SDL_GetMouseState(&x, &y); // Get current mouse position

      int row = ((y / scale) - offsetY) / CELL_SIZE;
      int column = ((x / scale) - offsetX) / CELL_SIZE;

      if (alive) {
        simulation.reviveCell(row, column);
      } else if (!alive) {
        simulation.killCell(row, column);
      }
    }

    if (middleMouseButtonPressed) {
      int x, y;
      SDL_GetMouseState(&x, &y); // Get current mouse position

      offsetY = offsetEndY + ((y / scale) - mouseStartPanY);
      offsetX = offsetEndX + ((x / scale) - mouseStartPanX);
    }
    simulation.Update();

    SDL_SetRenderDrawColor(renderer, 29, 29, 29, 255);
    SDL_RenderClear(renderer);

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    simulation.Draw(renderer, offsetX, offsetY, scale);
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

  simulation.saveCells("data/vec.txt");

  std::cout << "\033[2J\033[1;1H";
  return 0;
}

bool checkLineContent(const std::string &filename, int lineNumber,
                      const std::string &targetString) {
  std::ifstream file(filename);
  std::string line;
  int currentLine = 0;

  if (!file.is_open()) {
    std::ofstream writeFile(filename, std::ios::app);
    if (writeFile.is_open()) {
      writeFile << "Line 1\n"; // Write a placeholder for the first line
      writeFile << "true\n";   // Write "true" on the second line
      writeFile.close();
    }
    return false; // Return false if unable to open the file
  }

  while (std::getline(file, line)) {
    currentLine++;
    if (currentLine == lineNumber) {
      return line ==
             targetString; // Compare the line content with the target string
    }
  }

  file.close();
  return false; // Return false if the line number exceeds the total lines in
                // the file
}
