#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "Objects.h"
#include <QElapsedTimer>
#include "Camera.h"

struct Data {
    Object* obj;
    std::string type;
    float x, y, z;
    float r, g, b;
};
struct ProjectionParams {
    float left, right, top, bottom, zNear, zFar;
};
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

public:
    Camera cam;
    QMap<QString, QTimer*> timers;
    ProjectionParams getProjectionParams() const;
    std::string mode = "2D";
    std::string getType(const std::string& name);
    std::vector<float> getColors(const std::string& name);
    explicit MyOpenGLWidget(QWidget* parent = nullptr);
    void addObj(Object* obj, const std::string& name, const std::string& type, const float& x,
        const float& y, const float z,const float r, const float g, const float b);
    void clearScene();
    void changeObj(const std::string& name, float x, float y, float z, float colors[],int turnX,
        int turnY, int turnZ);
    void removeObj(const std::string& name);
    void setMode(const std::string& m);
    void startMove(const std::string& name, int targetX, int speed);
    std::map <std::string, Data> getObjects();
    void moveObj(const std::string& name, const float& x, const float& y, const float z);
    int getX(const std::string& name);
    int getY(const std::string& name);
    int getZ(const std::string& name);
    int getTurnX(const std::string& name);
    int getTurnY(const std::string& name);
    int getTurnZ(const std::string& name);
    void drawGrid(float spacing, int count,QPainter& painter);
    void drawText(QPainter& painter,int x, int y,int text);
protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override; 
    void resizeGL(int w,int h) override; 
    void paintGL() override;
    QPointF worldToScreen(float x, float y) const;
private:
    QElapsedTimer fpsTimer;
    int fpsFrames = 0;
    //Widget coordinates
    float aspect = 1.0f;
    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;
    float zNear = 0.0f;
    float zFar = 0.0f;
    int animTargetX = 0;
    int animSpeed = 5;
    QTimer* animTimer = nullptr;
    std::string animName;
    std::map < std::string, Data > objects;// list of all objects
private slots:
    void animateMove();
};
