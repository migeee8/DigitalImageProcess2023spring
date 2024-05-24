#ifndef POINT_H
#define POINT_H

class Imgpoint {
public:
    Imgpoint(float x = 0.0f, float y = 0.0f); // 默认构造函数，同时可以设置 x 和 y 的初始值
    float getX() const; // 获取 x 坐标的值
    float getY() const; // 获取 y 坐标的值
    void setX(float x); // 设置 x 坐标的值
    void setY(float y); // 设置 y 坐标的值
private:
    float m_x; // x 坐标
    float m_y; // y 坐标
};

#endif // POINT_H
#pragma once
