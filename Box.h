#pragma once

#include "graphicsObject.h"

class Box : public GraphicsObject
{
public:
    Box() {};
    ~Box() {};

    void initGeom();
    int render();



private:
    Vertices    m_vertices;
    Indices		m_indices;
};
