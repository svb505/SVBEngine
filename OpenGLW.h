#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>
#include <QPainter>
#include "Camera.h"
#include "GUI.h"
#include "HUD.h"
#include "text.h"
#include "Animation.h"

enum class RenderMode {
    _2D,
    _3D
};
struct Data {
    Object* obj;
    std::string type;
    float x, y, z;
    float r, g, b;
    std::string parent;
};

struct ProjectionParams {
    float left, right, top, bottom, zNear, zFar;
};
class OpenGLW : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;
private:
    float backgroundColor[4] = { 0.1f, 0.1f, 0.0f, 1.0f };
    int fps = 0;
    std::vector<TextData> texts; // Users texts in scene
    std::unordered_map<std::string, Data> objects; // List of objects
    //Scene coordinates
    float aspect = 1.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;
    float zNear = 0.0f;
    float zFar = 0.0f;
    //FPS
    QElapsedTimer fpsTimer;
    int fpsFrames = 0;
    //Classes 
    GUI gui;
    Camera cam;
    SceneText* text = nullptr;
    AnimationSystem* animSys;
    HUD* hud = nullptr;
    //Other
    RenderMode mode = RenderMode::_2D;
    QPainter painter;
public:
    QMap<QString, QTimer*> timers;
   
    explicit OpenGLW(QWidget* parent = nullptr);
    ~OpenGLW();

    void clearScene();

    void addObj(Object* obj, const std::string& name, const std::string& type, float x, float y, float z,
        float r, float g, float b, std::string parent);
    void changeObj(const std::string& name, float x, float y, float z, float colors[], int turnX,int turnY, int turnZ);
    void removeObj(const std::string& name);

    void setMode(const RenderMode& m);
    void setBackground(std::array<float, 3> color);

    void moveObj(const std::string& name, const float& x, const float& y, const float z);
    
    std::unordered_map<std::string, Data> getObjects() const;
    int getX(const std::string& name);
    int getY(const std::string& name);
    int getZ(const std::string& name);
    int getTurnX(const std::string& name);
    int getTurnY(const std::string& name);
    int getTurnZ(const std::string& name);
    RenderMode getMode();
    ProjectionParams getProjectionParams() const;
    std::string getType(const std::string& name);
    std::vector<float> getColors(const std::string& name);

    void initHUD();
    void initSceneText();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
friend class HUD;
friend class SceneText;
friend class GUI;
friend class AnimationSystem;
};
