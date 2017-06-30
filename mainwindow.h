#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Q3DBars>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

    QHBoxLayout mainLayout;
    QVBoxLayout graphLayout;
    QtDataVisualization::Q3DBars qGraph;
    QtDataVisualization::Q3DBars cGraph;

    QGridLayout controlsLayout;
    QSpinBox pathLenSpin;
    QSlider pathLenSlider;
    QSpinBox stepsSpin;
    QSlider stepsSlider;

    void simulate();
public:
    explicit MainWindow(QWidget *parent = 0);
};

#endif // MAINWINDOW_H
