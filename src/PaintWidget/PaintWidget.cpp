//
// Created by Yaroslav on 19.06.2020.
//

#include "PaintWidget.hpp"

PaintWidget::PaintWidget(QWidget *parent) noexcept: QGraphicsView(parent) {
    setViewportUpdateMode(SmartViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    scene.setStickyFocus(false);
    setScene(&scene);
}

void PaintWidget::setTool(ToolType tool) {
    if (currentTool == ToolType::Triangle && triangleCount != 3) {
        scene.removeItem(currentTriangle);
    }
    currentTool = tool;
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {
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

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {
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

void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {
    dragStart = std::nullopt;
    currentRect = nullptr;
    currentCircle = nullptr;
}
