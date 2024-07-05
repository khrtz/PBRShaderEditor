#ifndef MATERIALEDITOR_H
#define MATERIALEDITOR_H

#include <QWidget>
#include <QSlider>
#include <QLabel>

class MaterialEditor : public QWidget {
    Q_OBJECT

public:
    MaterialEditor(QWidget *parent = nullptr);

signals:
    void materialParametersChanged(float roughness, float metallic);

private slots:
    void onParametersChanged();

private:
    QSlider *roughnessSlider;
    QSlider *metallicSlider;
};

#endif // MATERIALEDITOR_H
