#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QGLWidget>
#include <QMouseEvent>
#include <QMatrix4x4>
#include "geometry/JPlane.h"
#include "geometry/JSphere.h"

class MyGLWidget : public QGLWidget
{

public:
    MyGLWidget(QWidget *);
private:

    // Глубина объекта
    float zoffset = 3;

    // Матрица поворота, используемая для поворота сцены, когда пользователь зажимает кнопку мыши
    QMatrix4x4 rotateMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;

    // Позиция указателя мыши
    QPoint mousePosition;

    // Нажатая кнопка мыши
    Qt::MouseButton clickedButton;

    // Координаты прямоугольной области выделения в экранной системе координат
    QRectF selectedScreenRect;

    // Усечённая пирамида для выделения объектов, задаваемая 6 плоскостями
    JPlane selectingPlanes[6];

    // Параметры селектирующего луча
    JRay selectionRay;

    // Сферы
    JSphere spheres[64];

    // Текст (отладочная информация), который выводится на экране
    QString output_text;

    // Инициализация освещения
    void initLighting();

    // Процедура для обновления матрицы проектирования
    void resetProjectionMatrix();

    // Процедура для обновления видовой матрицы
    void resetModelViewMatrix();

    ///
    /// \brief toOpenGLScreen
    /// \param pos - позиция указателя мыши в оконной системе координат
    /// \return Возвращает координаты указателя мыши в экранной системе координат OpenGL (от -1 до 1), соответствующую точке pos
    ///
    QPointF toOpenGLScreen(QPoint pos);

    ///
    /// \brief drawSelectedRectangle рисует выделенный пользователем прямоугольник на плоскости z = 0
    /// Это оптимальный способ
    void drawSelectedRectangle();

    ///
    /// \brief drawSelectedRectangle_2 делает то же самое, но с вычислением произведения матриц
    /// Это неоптимальный способ
    void drawSelectedRectangle_2();


    ///
    /// \brief initSelectingFrustumPlanes вычисляет параметры плоскостей отсечения, которые образуют усечённую пирамиду.
    /// Вызывается при изменении координат вершин прямоугольника, который изображает выделенное пространство
    ///
    void initSelectingFrustumPlanes();

    // Процедура для изменения матрицы поворота
    static void changeRotateMatrix(float* rotateMatrix, float dx, float dy);

    // Процедура для инициализации параметров OpenGL
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mouseMoveEvent(QMouseEvent* m_event);
    void wheelEvent(QWheelEvent* w_event);
    void keyPressEvent(QKeyEvent* key_event);

    // Обработчик нажатия кнопок мыши
    void mousePressEvent(QMouseEvent* m_event);

    // Обработчик отпускания кнопок мыши
    void mouseReleaseEvent(QMouseEvent* m_event);
};

#endif // MYGLWIDGET_H
