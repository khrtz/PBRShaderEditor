#ifndef MATERIALPREVIEW_H
#define MATERIALPREVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

class MaterialPreview : public QOpenGLWidget {
    Q_OBJECT

public:
    MaterialPreview(QWidget *parent = nullptr);
    ~MaterialPreview();

public slots:
    void updateMaterial(float roughness, float metallic);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    QOpenGLShaderProgram *program;
    float roughness;
    float metallic;
};

#endif // MATERIALPREVIEW_H
