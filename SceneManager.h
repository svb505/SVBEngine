#pragma once
#include <map>
#include <string>
#include "Objects.h"
#include <QPointF>
#include <QPainter>

struct Data {
    Object* obj;
    std::string type;
    float x, y, z;
    float r, g, b;
};

class Scene {    
public:

    std::map<std::string, Data> objects; // List of objects
    void addObj(const std::string& name, Object* obj, const std::string& type,
        float x, float y, float z, float r, float g, float b)
    {
        objects[name] = { obj, type, x, y, z, r, g, b };
    }
    void removeObj(const std::string& name) {
        objects.erase(name);
    }
    void clearScene() {
        objects.clear();
    }
    Data* getObj(const std::string& name) {
        auto it = objects.find(name);
        return (it != objects.end()) ? &it->second : nullptr;
    }
    const std::map<std::string, Data>& getObjects() const {
        return objects;
    }

};
