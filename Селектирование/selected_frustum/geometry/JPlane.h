#ifndef JPLANE_H
#define JPLANE_H

#include <QVector3D>

///
/// \brief Класс JPlane используется для работы с плоскостями
/// Уравнение плоскости записывается в виде Ax + By + Cz + D = 0
///
class JPlane
{
    float A, B, C, D;

public:

    JPlane();

    JPlane(float _A, float _B, float _C, float _D);

    ///
    /// \brief distanceTo
    /// \param V - координаты точки
    /// \return Возвращает значение выражения
    /// A * V.x + B * V.y + C * V.z + D - расстояние от точки V до плоскости
    ///
    float distanceTo(const QVector3D& V) const;

    ///
    /// \brief operator -
    /// \return Возвращает уравнение плоскости, у которой вектор нормали повернут на 180 градусов относительно текущей
    /// Используется для поворота вектора нормали внутрь пирамиды отсечения
    JPlane operator-() const;
};

#endif // PLANE_H
