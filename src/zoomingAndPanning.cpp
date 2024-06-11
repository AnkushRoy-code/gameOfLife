#include "zoomingAndPanning.h"

void zoomAndPanning::worldToScreen(float worldX, float worldY, int &screenX,
                                   int &screenY) {
  screenX = (int)(worldX - offsetX);
  screenY = (int)(worldY - offsetY);
}

void zoomAndPanning::screenToWorld(int screenX, int screenY, float &worldX,
                                   float &worldY) {
  worldX = (float)(screenX) + offsetX;
  worldY = (float)(screenY) + offsetY;
}
