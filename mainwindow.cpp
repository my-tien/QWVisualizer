#include "mainwindow.h"

#include "walker.h"

#include <QAbstract3DGraph>

using namespace std::complex_literals;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    for (auto & graph : {&cGraph, &qGraph}) {
        graph->setFlags(graph->flags() ^ Qt::FramelessWindowHint);
        graph->columnAxis()->setRange(0,6);
        graph->valueAxis()->setRange(0, 1);
        auto graphWidget = QWidget::createWindowContainer(graph, this);
        graph->setSelectionMode(QtDataVisualization::QAbstract3DGraph::SelectionFlags(QtDataVisualization::QAbstract3DGraph::SelectionSlice | QtDataVisualization::QAbstract3DGraph::SelectionRow));
        graphLayout.addWidget(graphWidget);
    }

    initStateCombo.addItems({tr("tails"), tr("heads"), tr("tails / heads"), tr("tails / imaginary heads")});
    initStateCombo.setCurrentIndex(static_cast<int>(InitState::Tails));
    QObject::connect(&initStateCombo, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](const int){ simulate(); });
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
    controlsLayout.addWidget(&initStateCombo, row++, 0);
    addNumberControl(pathLenSpin, pathLenSlider, 7, 1, 100, "length: ", "");
    addNumberControl(stepsSpin, stepsSlider, 3, 1, 100, "", " steps");
    controlsLayout.setAlignment(Qt::AlignTop);
    mainLayout.addLayout(&graphLayout);
    mainLayout.addLayout(&controlsLayout);
    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(&mainLayout);
    setMinimumSize(500, 500);
}

void MainWindow::init(InitState const s) {
    const auto length = pathLenSpin.value();
    qInitState.setZero(length * 2);
    cInitState.setZero(length * 2);
    switch(s) {
    case InitState::Tails:
        qInitState[length - 1] = 1;
        break;
    case InitState::Heads:
        qInitState[length] = 1;
        break;
    case InitState::TailsHeads:
        qInitState[length - 1] = 1/std::sqrt(2);
        qInitState[length] = 1/std::sqrt(2);
        break;
    case InitState::TailsImagHeads:
        qInitState[length - 1] = 1/std::sqrt(2);
        qInitState[length] = 1i/std::sqrt(2);
        break;
    }
    if (s == InitState::Tails || s == InitState::Heads) {
        cInitState = qInitState;
    } else {
        cInitState[length - 1] = 1;
    }
}

void MainWindow::simulate() {
    init(static_cast<InitState>(initStateCombo.currentIndex()));
    Walker qWalker{qInitState, Walker::hadamardCoin()};
    Walker cWalker{cInitState, Walker::classicCoin()};
    auto sim = [this](Walker & walker, QtDataVisualization::Q3DBars & graph) {
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
