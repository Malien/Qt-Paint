#include "PaintWindow.hpp"
#include <QDebug>
#include <QFileDialog>
#include "./ui_PaintWindow.h"

PaintWindow::PaintWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::PaintWindow) {
    ui->setupUi(this);

    connect(
            ui->actionSave,
            &QAction::triggered,
            this,
            [this]() {
                const auto file = QFileDialog::getSaveFileName(
                        this,
                        tr("Save file"),
                        "~/untitled.jpg",
                        tr("Images (*.png *.jpg)"));
                const auto image = ui->graphicsView->grab();
                image.save(file, "jpg");
            });

    connect(
            ui->actionClear,
            &QAction::triggered,
            this,
            [this]() {
                ui->graphicsView->clear();
            });

    const auto connectSlider = [this](QSlider *slider, void (QColor::*setter)(int)) {
        connect(slider, &QSlider::valueChanged, this, [this, setter](int newValue) {
            (color.*(setter))(newValue);
            ui->colorSwatch->setStyleSheet("background-color: " + color.name());
            ui->graphicsView->setColor(color);
        });
    };

    connectSlider(ui->rSlider, &QColor::setRed);
    connectSlider(ui->gSlider, &QColor::setGreen);
    connectSlider(ui->bSlider, &QColor::setBlue);
    ui->colorSwatch->setStyleSheet("background-color: " + color.name());
    ui->graphicsView->setColor(color);

    const auto connectTool = [this](QRadioButton *button, ToolType tool, bool unlocksBrush = false) {
        connect(button, &QRadioButton::toggled, this, [this, tool, unlocksBrush](bool checked) {
           if (checked) ui->graphicsView->setTool(tool);
            ui->brushLabel->setVisible(unlocksBrush);
            ui->brushSlider->setVisible(unlocksBrush);
        });
    };

    connectTool(ui->rectButton, ToolType::Rectangle);
    connectTool(ui->circleButton, ToolType::Circle);
    connectTool(ui->triangleButton, ToolType::Triangle);
    connectTool(ui->brushButton, ToolType::Brush, true);
    ui->graphicsView->setTool(ToolType::Brush);

    connect(
            ui->brushSlider,
            &QSlider::valueChanged,
            this,
            [this](int newValue) {
                ui->graphicsView->setBrushSize(static_cast<float>(newValue));
            });
    ui->graphicsView->setBrushSize(static_cast<float>(ui->brushSlider->value()));
}

PaintWindow::~PaintWindow() {
    delete ui;
}
