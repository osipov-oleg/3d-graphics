// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#include "JRay.h"

JRay::JRay()
{
}


JRay::JRay(QVector3D A, QVector3D B)
{
    _A = A;
    _B = B;
    _D = (_A - _B).normalized();
}


QVector3D JRay::A() const
{
    return _A;
}


QVector3D JRay::B() const
{
    return _B;
}


QVector3D JRay::direction() const
{
    return _D;
}


bool JRay::isNull()
{
    return _A.isNull() && _B.isNull();
}
