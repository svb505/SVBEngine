# SVBEngine

**SVBEngine** is a mini-game engine with OpenGL rendering and a Qt user interface.

## Features
- 2D/3D rendering
- Data-driven object behavior control via reusable scenarios
- Qt-based user interface for debugging and tooling
- Logging and monitoring

## Architecture
- Object behavior is implemented using a **composition-based approach**
- Rendering, movement, and gameplay logic are processed in independent systems
- Scenarios define reusable, data-driven gameplay behaviors

## Technical Features
- Explicit object lifetime and ownership management
- Clear separation of gameplay and rendering phases
- Scalable behavior logic without deep inheritance hierarchies

## Purpose
This project was developed to explore game system design,
object behavior modeling, and engine-level architecture
relevant to complex real-time games.

## Technologies
- C++ (RAII, STL, smart pointers)
- OpenGL
- Qt (UI tools)

## Capabilities
- Fully customizable 2D/3D objects in scenes
- Scene import/export
- Data-driven scenarios for defining object behaviors such as movement, state transitions, and interactions

## Demo
![2D](images/Engine2D.png)
![3D](images/Engine3D.png)
![SCENARIOS](images/EngineScenarios.png)
![OBJECTS](images/EngineObjects.png)

Thanks for reading!
