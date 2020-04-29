#ifndef JFIGURE_H
#define JFIGURE_H

#include <QColor>
#include "JPlane.h"
#include "JRay.h"

class JSphere
{
    // Координаты центра сферы
    QVector3D _center;

    // Радиус сферы
    float _radius;

    // Цвет сферы
    QColor _color;

public:

    // Флаг, определяющий, выделена ли сфера мышкой
    bool isSelected;

    JSphere();

    JSphere(float x0, float y0, float z0, float radius);

    ///
    /// \brief is_inside
    /// \param planes - уравнения плоскостей отсечения
    /// \param nPlanes - количество плоскостей отсечения
    /// \return Возвращает истину, если сфера находится внутри пирамиды отсечения, задаваемой плоскостями planes, иначе ложь
    ///
    bool is_inside(const JPlane* planes, int nPlanes) const;

    ///
    /// \brief radius
    /// \return Возвращает радиус сферы
    ///
    float radius() const;

    // Координаты центра сферы
    float x0() const;

    float y0() const;

    float z0() const;

    ///
    /// \brief center
    /// \return Возвращает координаты центра сферы
    ///
    QVector3D center() const;

    ///
    /// \brief Вычисляет координаты точек пересечения луча ray со сферой
    /// \param ray - параметры луча
    /// \param R - массив точек пересечения из 0, 1, или 2 элементов
    /// \return Возвращает количество (0, 1, или 2) точек пересечения луча ray со сферой
    ///
    int intersects(const JRay& ray, QVector3D* R) const;

    ///
    /// \brief isCrossed
    /// \param ray - параметры луча
    /// \return Возвращает истину, если луч ray пересекает сферу, иначе ложь
    ///
    bool isCrossed(const JRay& ray);
};

#endif // JFIGURE_H
