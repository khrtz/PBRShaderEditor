#include "materialeditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MaterialEditor::MaterialEditor(QWidget *parent)
    : QWidget(parent), roughnessSlider(new QSlider(Qt::Horizontal, this)), metallicSlider(new QSlider(Qt::Horizontal, this)) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *roughnessLayout = new QHBoxLayout();
    QLabel *roughnessLabel = new QLabel("Roughness", this);
    roughnessLayout->addWidget(roughnessLabel);
    roughnessLayout->addWidget(roughnessSlider);
    layout->addLayout(roughnessLayout);

    QHBoxLayout *metallicLayout = new QHBoxLayout();
    QLabel *metallicLabel = new QLabel("Metallic", this);
    metallicLayout->addWidget(metallicLabel);
    metallicLayout->addWidget(metallicSlider);
    layout->addLayout(metallicLayout);

    connect(roughnessSlider, &QSlider::valueChanged, this, &MaterialEditor::onParametersChanged);
    connect(metallicSlider, &QSlider::valueChanged, this, &MaterialEditor::onParametersChanged);
}

void MaterialEditor::onParametersChanged() {
    float roughness = roughnessSlider->value() / 100.0f;
    float metallic = metallicSlider->value() / 100.0f;
    emit materialParametersChanged(roughness, metallic);
}
