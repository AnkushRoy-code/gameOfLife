
class zoomAndPanning {
public:
  void worldToScreen(float worldX, float worldY, int &screenX, int &screenY);
  void screenToWorld(int screenX, int screenY, float &worldX, float &worldY);

  void updatePan(float mouseX, float mouseY) {
    startPanX = mouseX;
    startPanY = mouseY;
  }

private:
  float offsetX = 0.0f, offsetY = 0.0f;

  float startPanX = 0.0f, startPanY = 0.0f;
};
