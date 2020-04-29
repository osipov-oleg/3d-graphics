#include "JPlane.h"
#include "math.h"

JPlane::JPlane()
{
    A = B = C = D = 0;
}

// Конструктор плоскости
JPlane::JPlane(float _A, float _B, float _C, float _D) : A(_A), B(_B), C(_C), D(_D)
{
    double R = sqrt(A*A + B*B + C*C);

    A /= R;
    B /= R;
    C /= R;
    D /= R;
}

float JPlane::distanceTo(const QVector3D& V) const
{
    return A * V.x() + B * V.y() + C * V.z() + D;
}

JPlane JPlane::operator -() const
{
    return JPlane(-A, -B, -C, -D);
}
