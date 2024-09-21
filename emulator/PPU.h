#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

// Picture Processing Unit responsible for rendering graphics inside a window
class PPU : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    PPU(QWidget* parent = nullptr);
    ~PPU();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
