// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#ifndef JRAY_H
#define JRAY_H

#include <QVector3D>

///
/// \brief JRay - луч в пространстве
///
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

    QVector3D A() const;

    QVector3D B() const;

    QVector3D direction() const;

    bool isNull();
};

#endif // JRAY_H
