#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Engine.h"

class Engine : public QMainWindow
{
    Q_OBJECT

public:
    Engine(QWidget *parent = nullptr);
    ~Engine();

private:
    Ui::EngineClass ui;
};

