#include "mainwindow.h"
#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), materialEditor(new MaterialEditor(this)), materialPreview(new MaterialPreview(this)) {

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(materialEditor);
    splitter->addWidget(materialPreview);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    setCentralWidget(splitter);

    connect(materialEditor, &MaterialEditor::materialParametersChanged, materialPreview, &MaterialPreview::updateMaterial);
}

MainWindow::~MainWindow() {}
