#include "mainwindow.h"

#include "classicalwalker.h"
#include "quantumwalker.h"

#include <QAbstract3DGraph>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    for (auto & graph : {&cGraph, &qGraph}) {
        graph->setFlags(graph->flags() ^ Qt::FramelessWindowHint);
        graph->columnAxis()->setRange(0,6);
        graph->valueAxis()->setRange(0, 1);
        auto graphWidget = QWidget::createWindowContainer(graph, this);
        graph->setSelectionMode(QtDataVisualization::QAbstract3DGraph::SelectionFlags(QtDataVisualization::QAbstract3DGraph::SelectionSlice | QtDataVisualization::QAbstract3DGraph::SelectionRow));
        graphLayout.addWidget(graphWidget);
    }
    int row = 0;
    auto addNumberControl = [&row, this](QSpinBox & spin, QSlider & slider, const int value, const int min, const int max, const QString & prefix, const QString & suffix) {
        QObject::connect(&spin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&slider, this](const int value) {
            if (value != slider.value()) {
                slider.setValue(value);
            }
            simulate();
        });
        QObject::connect(&slider, &QSlider::valueChanged, [&spin, this](const int value) {
            if (value != spin.value()) {
                spin.setValue(value);
            }
        });

        spin.setPrefix(prefix);
        spin.setSuffix(suffix);
        spin.setRange(min, max);
        slider.setRange(spin.minimum(), spin.maximum());
        spin.setValue(value);
        spin.setFixedWidth(spin.sizeHint().width());
        slider.setOrientation(Qt::Vertical);
        controlsLayout.addWidget(&spin, row, 0, Qt::AlignTop);
        controlsLayout.addWidget(&slider, row++, 1);
    };
    addNumberControl(pathLenSpin, pathLenSlider, 7, 1, 100, "length: ", "");
    addNumberControl(stepsSpin, stepsSlider, 3, 1, 1000, "", " steps");
    controlsLayout.setAlignment(Qt::AlignTop);
    mainLayout.addLayout(&graphLayout);
    mainLayout.addLayout(&controlsLayout);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(&mainLayout);
    setMinimumSize(500, 500);
    simulate();
}

void MainWindow::simulate() {
    QuantumWalker qWalker{pathLenSpin.value()};
    ClassicalWalker cWalker{pathLenSpin.value()};
    auto sim = [this](RandomWalker & walker, QtDataVisualization::Q3DBars & graph) {
        for (auto * series : graph.seriesList()) {
            graph.removeSeries(series);
        }
        graph.columnAxis()->setRange(0, pathLenSpin.value() - 1);
        QtDataVisualization::QBar3DSeries * series = new QtDataVisualization::QBar3DSeries();
        QtDataVisualization::QBarDataRow * data = new QtDataVisualization::QBarDataRow();

        walker.step(stepsSpin.value());
        const auto state = walker.getProbabilities();
        double max = 0;
        for (std::size_t i = 0; i < state.rows(); ++i) {
            max = std::max(max, state[i]);
            *data << state[i];
        }
        graph.valueAxis()->setRange(0, max);
        series->dataProxy()->addRow(data);
        graph.addSeries(series);
        graph.seriesList().at(0)->setSelectedBar({0, 0});
    };
    sim(cWalker, cGraph);
    sim(qWalker, qGraph);
}
