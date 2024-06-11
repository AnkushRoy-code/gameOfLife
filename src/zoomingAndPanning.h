
class zoomAndPanning {
public:
  void worldToScreen(float worldX, float worldY, int &screenX, int &screenY);
  void screenToWorld(int screenX, int screenY, float &worldX, float &worldY);

private:
  float offsetX, offsetY = 0.0f;
};
