#ifndef POINT_H
#define POINT_H

class Imgpoint {
public:
    Imgpoint(float x = 0.0f, float y = 0.0f); // Ĭ�Ϲ��캯����ͬʱ�������� x �� y �ĳ�ʼֵ
    float getX() const; // ��ȡ x �����ֵ
    float getY() const; // ��ȡ y �����ֵ
    void setX(float x); // ���� x �����ֵ
    void setY(float y); // ���� y �����ֵ
private:
    float m_x; // x ����
    float m_y; // y ����
};

#endif // POINT_H
#pragma once
