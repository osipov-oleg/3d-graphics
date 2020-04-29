#ifndef JRAY_H
#define JRAY_H

#include <QVector3D>

///
/// \brief JRay - луч в пространстве
/// Луч в пространстве задаётся начальной точкой A, конечной - B и направлением D
class JRay
{
    QVector3D _A, _B;

    ///
    /// \brief Единичный вектор направления
    ///
    QVector3D _D;
public:

    JRay();

    ///
    /// \brief JRay - конструктор луча по двум точкам в пространстве
    /// \param A - координаты первой точки в пространстве
    /// \param B - координаты второй точки в пространстве
    ///
    JRay(QVector3D A, QVector3D B);

    ///
    /// \brief A
    /// \return Возвращает координаты начальной точки луча
    ///
    QVector3D A() const;

    ///
    /// \brief B
    /// \return Возвращает координаты второй точки луча
    ///
    QVector3D B() const;

    ///
    /// \brief direction
    /// \return Возвращает единичный вектор направления
    ///
    QVector3D direction() const;

    bool isNull();
};

#endif // JRAY_H
