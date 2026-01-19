#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>
#include "Camera.h"
#include "GUI.h"
#include "HUD.h"
#include "text.h"

struct Data {
    Object* obj;
    std::string type;
    float x, y, z;
    float r, g, b;
};
struct ProjectionParams {
    float left, right, top, bottom, zNear, zFar;
};
class OpenGLW : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

public:
    GUI gui;
    Camera cam;
    SceneText* text = nullptr;
    HUD* hud = nullptr;
    std::string mode = "2D";

    QMap<QString, QTimer*> timers;
    ProjectionParams getProjectionParams() const;
    std::string getType(const std::string& name);
    std::vector<float> getColors(const std::string& name);

    explicit OpenGLW(QWidget* parent = nullptr);
    ~OpenGLW();

    void addObj(Object* obj, const std::string& name,  const std::string& type,float x, float y, float z, float r, float g, float b);
    void clearScene();
    void changeObj(const std::string& name, float x, float y, float z, float colors[], int turnX,int turnY, int turnZ);
    void removeObj(const std::string& name);
    void setMode(const std::string& m);
    void startMove(const std::string& name, int targetX, int speed);
    std::map<std::string, Data> getObjects() const;
    void moveObj(const std::string& name, const float& x, const float& y, const float z);
    int getX(const std::string& name);
    int getY(const std::string& name);
    int getZ(const std::string& name);
    int getTurnX(const std::string& name);
    int getTurnY(const std::string& name);
    int getTurnZ(const std::string& name);
    void drawGridOpenGL(float spacing, int count);
    void setBackground(std::array<float,3> color);
    void initHUD();
    void initSceneText();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    float backgroundColor[4] = {0.1f, 0.1f, 0.0f, 1.0f};
    int fps = 0;
    std::map<std::string, Data> objects; // List of objects
    //Scene coordinates
    float aspect = 1.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;
    float zNear = 0.0f;
    float zFar = 0.0f;
    QElapsedTimer fpsTimer;
    int fpsFrames = 0;
    int animTargetX = 0;
    int animSpeed = 5;
    QTimer* animTimer = nullptr;
    std::string animName;

private slots:
    void animateMove();
friend class HUD;
friend class SceneText;
};
