#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Q3DBars>
#include <QBar3DSeries>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

    QHBoxLayout mainLayout;
    QtDataVisualization::Q3DBars graph;

    QGridLayout controlsLayout;
    QSpinBox pathLenSpin;
    QSpinBox stepsSpin;

    void simulate();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() {}
};

#endif // MAINWINDOW_H
