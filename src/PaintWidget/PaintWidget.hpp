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
    explicit PaintWidget(QWidget *parent = nullptr) noexcept: QGraphicsView(parent) {
        setViewportUpdateMode(SmartViewportUpdate);
        setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        scene.setStickyFocus(false);
        setScene(&scene);
    }

public slots:

    void clear() {
        scene.clear();
    };

    void setTool(const ToolType tool) {
        if (currentTool == ToolType::Triangle && triangleCount != 3) {
            scene.removeItem(currentTriangle);
        }
        currentTool = tool;
    }

    void setColor(const QColor &color) {
        currentColor = color;
    }

    void setBrushSize(float size) {
        brushSize = size;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        dragStart = mapToScene(event->pos());
        switch (currentTool) {

            case ToolType::Rectangle:
                currentRect = scene.addRect(
                        dragStart->x(),
                        dragStart->y(),
                        0,
                        0,
                        noBorder,
                        QBrush(currentColor));
                currentRect->setVisible(true);
                break;

            case ToolType::Circle:
                currentCircle = scene.addEllipse(
                        dragStart->x(),
                        dragStart->y(),
                        0,
                        0,
                        noBorder,
                        QBrush(currentColor));
                currentCircle->setVisible(true);
                break;

            case ToolType::Triangle:
                if (currentTriangle && triangleCount < 3) {
                    auto polygon = currentTriangle->polygon();
                    polygon.push_back(*dragStart);
                    currentTriangle->setPolygon(polygon);
                    triangleCount++;
                } else {
                    currentTriangle = scene.addPolygon(QPolygonF({*dragStart}), noBorder, QBrush(currentColor));
                    triangleCount = 1;
                }
                break;

            default:
                break;
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (dragStart) {
            switch (currentTool) {
                case ToolType::Rectangle:
                    if (currentRect) {
                        const auto mappedPosition = mapToScene(event->pos());
                        const auto[left, right] = std::minmax(mappedPosition.x(), dragStart->x());
                        const auto[top, bottom] = std::minmax(mappedPosition.y(), dragStart->y());
                        const QRectF rect(left, top, right - left, bottom - top);
                        currentRect->setRect(rect);
                    }
                    break;

                case ToolType::Circle:
                    if (currentCircle)
                        currentCircle->setRect(QRectF(*dragStart, mapToScene(event->pos())));
                    break;

                case ToolType::Brush: {
                    const auto currentPosition = mapToScene(event->pos());
                    scene.addLine(
                            dragStart->x(),
                            dragStart->y(),
                            currentPosition.x(),
                            currentPosition.y(),
                            QPen(
                                    currentColor,
                                    brushSize,
                                    Qt::SolidLine,
                                    Qt::RoundCap,
                                    Qt::RoundJoin
                            )
                    );
                    dragStart = currentPosition;
                } break;

                default:
                    break;
            }
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        dragStart = std::nullopt;
        currentRect = nullptr;
        currentCircle = nullptr;
    }

};

#endif //PAINT_PAINTWIDGET_HPP
