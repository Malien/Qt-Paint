//
// Created by Yaroslav on 19.06.2020.
//

#ifndef PAINT_DRAWABLE_HPP
#define PAINT_DRAWABLE_HPP

#include <QGraphicsScene>

struct Drawable {
    virtual ~Drawable() = default;

    virtual void draw(QGraphicsScene& scene) const noexcept = 0;
};

#endif //PAINT_DRAWABLE_HPP
