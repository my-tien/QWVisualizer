#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Q3DBars>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

#include <complex>
#include <eigen3/Eigen/Dense>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT
    Eigen::VectorXcd qInitState;
    Eigen::VectorXcd cInitState;
    QHBoxLayout mainLayout;
    QVBoxLayout graphLayout;
    QtDataVisualization::Q3DBars qGraph;
    QtDataVisualization::Q3DBars cGraph;

    QGridLayout controlsLayout;
    enum class InitState { Tails, Heads, TailsHeads, TailsImagHeads };
    QComboBox initStateCombo;
    QSpinBox pathLenSpin;
    QSlider pathLenSlider;
    QSpinBox stepsSpin;
    QSlider stepsSlider;

    void init(InitState const s);
    void simulate();
public:
    explicit MainWindow(QWidget *parent = 0);
};

#endif // MAINWINDOW_H
