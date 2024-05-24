#include "Point.h"

Imgpoint::Imgpoint(float x, float y) : m_x(x), m_y(y) {}

float Imgpoint::getX() const {
    return m_x;
}

float Imgpoint::getY() const {
    return m_y;
}

void Imgpoint::setX(float x) {
    m_x = x;
}

void Imgpoint::setY(float y) {
    m_y = y;
}
