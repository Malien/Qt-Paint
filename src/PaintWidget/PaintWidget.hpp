//
// Created by Yaroslav on 19.06.2020.
//

#ifndef PAINT_PAINTWIDGET_HPP
#define PAINT_PAINTWIDGET_HPP

#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QGraphicsSceneMoveEvent>
#include <iostream>
#include <QDebug>
#include <memory>
#include <optional>
#include <src/model/Tools.hpp>
#include <src/model/Drawable.hpp>

class PaintWidget : public QGraphicsView {
Q_OBJECT

    ToolType currentTool = ToolType::Rectangle;
    QColor currentColor = QColor::fromRgb(0, 0, 0);
    std::optional<QPointF> dragStart = std::nullopt;
    QGraphicsRectItem *currentRect = nullptr;
    QGraphicsEllipseItem *currentCircle = nullptr;
    QGraphicsPolygonItem *currentTriangle = nullptr;
    unsigned char triangleCount = 0;
    float brushSize = 10.0f;

    QGraphicsScene scene = QGraphicsScene(rect(), this);

    static inline QPen noBorder = QPen(QColor::fromRgb(0, 0, 0, 0));

public:
    explicit PaintWidget(QWidget *parent = nullptr) noexcept;

public slots:

    void clear() { scene.clear(); };

    void setTool(ToolType tool);

    void setColor(const QColor &color) { currentColor = color; }

    void setBrushSize(float size) { brushSize = size; }

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif //PAINT_PAINTWIDGET_HPP
