#include "mainwindow.h"
#include "randomwalker.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    graph.setFlags(graph.flags() ^ Qt::FramelessWindowHint);
    graph.columnAxis()->setRange(0,6);
    graph.valueAxis()->setRange(0, 1);
    pathLenSpin.setValue(7);
    stepsSpin.setValue(3);
    pathLenSpin.setPrefix("length: ");
    stepsSpin.setSuffix(" steps");
    auto graphWidget = QWidget::createWindowContainer(&graph, this);
    graphWidget->setMinimumSize(500, 500);
    mainLayout.addWidget(graphWidget);

    int row = 0;
    controlsLayout.setAlignment(Qt::AlignTop);
    controlsLayout.addWidget(&pathLenSpin, row++, 0);
    controlsLayout.addWidget(&stepsSpin, row++, 0);
    mainLayout.addLayout(&controlsLayout);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(&mainLayout);
    simulate();

    QObject::connect(&pathLenSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int &) { simulate(); });
    QObject::connect(&stepsSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](const int &) { simulate(); });
}

void MainWindow::simulate() {
    for (auto * series : graph.seriesList()) {
        graph.removeSeries(series);
    }
    graph.columnAxis()->setRange(0, pathLenSpin.value() - 1);
    QtDataVisualization::QBar3DSeries * series = new QtDataVisualization::QBar3DSeries();
    QtDataVisualization::QBarDataRow * data = new QtDataVisualization::QBarDataRow();
    RandomWalker walker{pathLenSpin.value()};
    walker.step(stepsSpin.value());
    const auto state = walker.getState();
    for (std::size_t i = 0; i < state.rows(); ++i) {
        *data << state[i];
    }
    series->dataProxy()->addRow(data);
    graph.addSeries(series);
}
