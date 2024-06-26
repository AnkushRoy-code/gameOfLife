#include "grid.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Color {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

// Add more colors if needed
Color green = {0, 255, 0, 255};
Color lightGray = {55, 55, 55, 255};
Color white = {255, 255, 255, 255};
Color black = {0, 0, 0, 255};

void Grid::Draw(SDL_Renderer *renderer, float offsetX, float offsetY,
                float scale) {
  for (int row = 0; row < rows; row++) {
    for (int column = 0; column < columns; column++) {
      SDL_Rect rect;

      // I changed this logic so the panning and scaling works.
      rect.x = (column * cellSize + offsetX) * scale;
      rect.y = (row * cellSize + offsetY) * scale;
      rect.w = (cellSize - 1) * scale;
      rect.h = (cellSize - 1) * scale;

      Color color = cells[row][column] ? green : lightGray;

      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(renderer, &rect);
    }
  }
};

void Grid::SetValue(int row, int column, int value) {
  if (row >= 0 && row < rows && column >= 0 && column < columns) {
    cells[row][column] = value;
  }
}

int Grid::GetValue(int row, int column) {
  if (row >= 0 && row < rows && column >= 0 && column < columns) {
    return cells[row][column];
  } else {
    return 0;
  }
}

void Grid::FillRandom() {
  for (int row = 0; row < rows; row++) {
    for (int column = 0; column < columns; column++) {
      int randomValue = rand() % 5;
      cells[row][column] = (randomValue == 4) ? 1 : 0;
    }
  }
}

void Grid::Clear() {
  for (int row = 0; row < rows; row++) {
    for (int column = 0; column < columns; column++) {
      cells[row][column] = 0;
    }
  }
}

void Grid::ToggleCell(int row, int column) {
  if (row >= 0 && row < rows && column >= 0 && column < columns) {
    cells[row][column] = !cells[row][column];
  }
}

void Grid::killCell(int row, int column) {
  if (row >= 0 && row < rows && column >= 0 && column < columns) {
    cells[row][column] = 0;
  }
}

void Grid::reviveCell(int row, int column) {
  if (row >= 0 && row < rows && column >= 0 && column < columns) {
    cells[row][column] = 1;
  }
}

// Function to save the last simulation in a file
void Grid::saveCellsToFile(const std::vector<std::vector<int>> &cells,
                           const std::string &filename) {
  std::ofstream outputFile(filename);

  if (!outputFile.is_open()) {
    std::cerr << "Failed to open file for writing." << std::endl;
    return;
  }

  for (const auto &row : cells) {
    for (size_t i = 0; i < row.size(); ++i) {
      outputFile << row[i];
      if (i != row.size() - 1) {
        outputFile << " ";
      }
    }
    outputFile << "\n";
  }

  outputFile.close();
}

// To load cells from the file
std::vector<std::vector<int>>
Grid::loadCellsFromFile(const std::string &filename) {
  std::vector<std::vector<int>> cells;
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Failed to open file for reading." << std::endl;
    return cells; // Return an empty vector if file cannot be opened
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    std::vector<int> row;
    int num;

    while (iss >> num) {
      row.push_back(num);
      if (iss.peek() == ' ')
        iss.ignore(); // Ignore spaces between numbers
    }

    cells.push_back(row);
  }

  inputFile.close();
  return cells;
}

void Grid::initializeCells() {
  std::ifstream inputFile("data/vec.txt");
  if (inputFile.good()) {
    // File exists, load cells from the file
    cells = loadCellsFromFile("data/vec.txt");
  } else {
    cells = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
  }
}
