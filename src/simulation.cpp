#include "simulation.h"

#include <utility>
#include <vector>

void Simulation::Draw(SDL_Renderer *renderer, float offsetX, float offsetY,
                      float scale) {
  grid.Draw(renderer, offsetX, offsetY, scale);
}

void Simulation::SetCellValue(int row, int column, int value) {
  grid.SetValue(row, column, value);
}

int Simulation::CountLiveNeighbors(int row, int column) {

  int liveNeighbors = 0;

  std::vector<std::pair<int, int>> neighborOffsets = {
      {-1, 0},  // Directly above
      {1, 0},   // Directly below
      {0, -1},  // To the left
      {0, 1},   // To the right
      {-1, -1}, // Diagonal upper left
      {-1, 1},  // Diagonal upper right
      {1, -1},  // Diagonal lower left
      {1, 1}    // Diagonal lower right
  };

  for (const auto &offset : neighborOffsets) {
    int neighborRow = ((row + offset.first) + grid.GetRows()) % grid.GetRows();
    int neighborColumn =
        ((column + offset.second) + grid.GetColumns()) % grid.GetRows();
    liveNeighbors += grid.GetValue( // GetValue returns 1 if neighbor else 0
        neighborRow, neighborColumn);
  }
  return liveNeighbors;
}

void Simulation::Update() {
  if (isRunning()) {
    for (int row = 0; row < grid.GetRows(); row++) {
      for (int column = 0; column < grid.GetColumns(); column++) {
        int liveNeighbors = CountLiveNeighbors(row, column);
        int cellValue = grid.GetValue(row, column);

        if (cellValue == 1) {
          if (liveNeighbors > 3 || liveNeighbors < 2) {
            tempGrid.SetValue(row, column, 0);
          } else {
            tempGrid.SetValue(row, column, 1);
          }
        } else {
          if (liveNeighbors == 3) {
            tempGrid.SetValue(row, column, 1);
          } else {
            tempGrid.SetValue(row, column, 0);
          }
        }
      }
    }
    grid = tempGrid;
  }
}

void Simulation::ClearGrid() {
  if (!isRunning()) {
    grid.Clear();
  }
}

void Simulation::CreateRandomState() {
  if (!isRunning()) {
    grid.FillRandom();
  }
}

void Simulation::ToggleCell(int row, int column) {
  if (!isRunning()) {
    grid.ToggleCell(row, column);
  }
}

void Simulation::killCell(int row, int column) {
  if (!isRunning()) {
    grid.killCell(row, column);
  }
}

void Simulation::reviveCell(int row, int column) {
  if (!isRunning()) {
    grid.reviveCell(row, column);
  }
}

void Simulation::saveCells(const std::string &filename) {
  grid.saveCall(filename);
}
