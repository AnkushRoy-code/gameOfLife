#pragma once
#include "grid.h"
#include <SDL2/SDL.h>

class Simulation {

public:
  Simulation(int width, int height, int cellSize)
      : grid(width, height, cellSize), tempGrid(width, height, cellSize),
        run{false} {};
  void Draw(SDL_Renderer *renderer, float offsetX, float offsetY);
  void SetCellValue(int row, int column, int value);

  int CountLiveNeighbors(int row, int column);
  void Update();

  bool isRunning() { return run; };
  void Start() { run = true; };
  void Stop() { run = false; };

  void ClearGrid();
  void CreateRandomState();
  void ToggleCell(int row, int column);
  void killCell(int row, int column);
  void reviveCell(int row, int column);

  void saveCells(const std::string &filename);

  int getValue(int row, int column) { return grid.GetValue(row, column); }

private:
  Grid grid;
  Grid tempGrid;
  bool run;
};
