#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Grid {
public:
  Grid(int width, int height, int cellSize)
      : rows(height / cellSize), columns(width / cellSize), cellSize(cellSize) {
    initializeCells();
  }

  void Draw(SDL_Renderer *renderer, int offsetX, int offsetY);
  void SetValue(int row, int column, int value);

  int GetValue(int row, int column);
  int GetRows() { return rows; };
  int GetColumns() { return columns; };

  void FillRandom();
  void Clear();
  void ToggleCell(int row, int column);

  void saveCall(const std::string &filename) {
    saveCellsToFile(cells, filename);
  }

  void saveCellsToFile(const std::vector<std::vector<int>> &cells,
                       const std::string &filename);
  std::vector<std::vector<int>> loadCellsFromFile(const std::string &filename);

private:
  int rows, columns, cellSize;
  std::vector<std::vector<int>> cells;

  void initializeCells();
};
