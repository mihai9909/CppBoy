#include "PPU.h"

PPU::PPU(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

PPU::~PPU() {}

void PPU::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void PPU::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void PPU::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
