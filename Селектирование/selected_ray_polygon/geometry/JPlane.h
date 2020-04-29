// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#ifndef JPLANE_H
#define JPLANE_H

#include <QVector3D>
#include "JRay.h"

class JPlane
{
    float A, B, C, D;

public:

    JPlane();

    JPlane(float _A, float _B, float _C, float _D);

    float distanceTo(const QVector3D& V) const;

    ///
    /// \brief intersects
    /// \param ray - параметры луча в трёхмерном пространстве
    /// \return Возвращает точку пересечения плоскости с лучом ray
    ///
    QVector3D intersects(const JRay& ray) const;

    ///
    /// \brief normal
    /// \return Возвращает вектор нормали к плоскости
    ///
    QVector3D normal() const;

    JPlane operator-() const;
};

#endif // PLANE_H
