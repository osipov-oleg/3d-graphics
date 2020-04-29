#include "MyGLWidget.h"
#include <gl/glu.h>

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle("Выделение объектов лучом и отсечение пирамидой с использованием мыши");
    setWindowState(Qt::WindowFullScreen);
}

void MyGLWidget::initializeGL()
{
    // Включение сортировки по глубине
    glEnable(GL_DEPTH_TEST);

    // Задаём такой режим смешивания цветов, чтобы работала прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Инициализация освещения
    initLighting();

    // Инициализация матрицы поворота в виде единичной матрицы
    rotateMatrix.setToIdentity();

    // Создание сфер
    for (int i=0; i < 64; i++)
      spheres[i] = JSphere(i / 8 - 3.5, i % 8 - 3.5, 0, 0.3);
}


void MyGLWidget::initLighting()
{
    // Включение освещения
    glEnable(GL_LIGHTING);

    // Включение автоматической нормализации векторов нормалей (обязательно при использовании операций масштабирования)
    glEnable(GL_NORMALIZE);

    // Освещение обеих сторон полигонов
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Параметры направленного источника (как солнце)
    float light0_diffuse[] = {0.9, 0.3, 0.0}; // Цвет
    float light0_ambient[] = {0.0, 0.0, 0.4, 1.0};
    // У направленного источника 4-ая компонента w = 0, а первые три имеют обратный знак
    float light0_direction[] = {1.0, 0.0, 0.0, 0.0}; // Направление: (-1, 0, 0)
    glEnable(GL_LIGHT0); // Включение источника
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); // Задаём цвет
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction); // Задаём направление
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);    // Цвет рассеянного освещения

    // Параметры точечного источника (как лампа)
    float light1_diffuse[] = {0.4, 0.9, 0.1}; // Цвет

    // У точечного источника 4-ая компонента w = 1
    float light1_position[] = {0.0, 0.0, -2.0, 1.0}; // Координаты центра источника света
    glEnable(GL_LIGHT1); // Включение источника
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);  // Задаём цвет
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position); // Задаём положение

    // Задаём свойства материала
    float specular_color[] = {1.0, 1.0, 1.0, 1}; // Цвет бликов
    float shininess[] = { 30 };
    // Цвет отражённого света
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    // Степень отраженного света
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}


void MyGLWidget::resizeGL(int nWidth, int nHeight)
{
    // Задание области вывода
    glViewport(0, 0, nWidth, nHeight);

    // Обновление матрицы центрального проектирования
    resetProjectionMatrix();

    // Обновление матрицы вида
    resetModelViewMatrix();
}


void MyGLWidget::drawSelectedRectangle()
{
    // Скопируем координаты вершин выделенного прямоугольника в локальные переменные
    float x1 = selectedScreenRect.left();
    float x2 = selectedScreenRect.right();
    float y1 = selectedScreenRect.top();
    float y2 = selectedScreenRect.bottom();

    // Для того, чтобы нарисовать прямоугольник перед всеми объектами прямо на экранной плоскости нужно временно сбросить все матрицы на единичные
    glMatrixMode(GL_MODELVIEW);
    // Сохраним матрицу
    glPushMatrix();
    // Загрузка единичной матрицы аффинных преобразований
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    // Сохраним матрицу
    glPushMatrix();
    // Загрузка единичной матрицы проектирования
    glLoadIdentity();

    glColor4ub(120, 130, 220, 100); // Цвет прямоугольника

    glEnable(GL_BLEND);         // Включим прозрачность для прямоугольника
    glDisable(GL_LIGHTING);     // Отключим освещение

    // Нарисуем выделенную область
    glBegin(GL_QUADS);
      glVertex3f(x1, y1, 0);
      glVertex3f(x1, y2, 0);
      glVertex3f(x2, y2, 0);
      glVertex3f(x2, y1, 0);
    glEnd();

    // Вернём все обратно: включим освещение, выключим прозрачность, восстановим матрицы
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glPopMatrix();      // Восстановление матрицы проектирования

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();      // Восстановление видовой матрицы
}


// Второй способ более трудоёмкий, так как использует умножение матрицы на вектора
void MyGLWidget::drawSelectedRectangle_2()
{
    // Скопируем координаты вершин выделенного пользователем прямоугольника в локальные переменные
    float x1 = selectedScreenRect.left();
    float x2 = selectedScreenRect.right();
    float y1 = selectedScreenRect.top();
    float y2 = selectedScreenRect.bottom();

    // Умножив координаты вершин прямоугольника, заданные в экранной системе координат OpenGL, на обратную матрицу всех преобразований,
    // получим однородные координаты прямоугольника в мировой системе координат OpenGL
    QVector4D c1 = IQ * QVector4D(x1, y1, 0, 1);
    QVector4D c2 = IQ * QVector4D(x1, y2, 0, 1);
    QVector4D c3 = IQ * QVector4D(x2, y2, 0, 1);
    QVector4D c4 = IQ * QVector4D(x2, y1, 0, 1);

    // Преобразуем однородные координаты вершин в обычные
    QVector3D r1 = c1.toVector3DAffine();
    QVector3D r2 = c2.toVector3DAffine();
    QVector3D r3 = c3.toVector3DAffine();
    QVector3D r4 = c4.toVector3DAffine();

    glColor4ub(120, 130, 220, 100); // Цвет прямоугольника

    glEnable(GL_BLEND);         // Включим прозрачность для прямоугольника
    glDisable(GL_LIGHTING);     // Отключим освещение

    // Нарисуем выделенную область
    glBegin(GL_QUADS);
      glVertex3f(r1.x(), r1.y(), r1.z());
      glVertex3f(r2.x(), r2.y(), r2.z());
      glVertex3f(r3.x(), r3.y(), r3.z());
      glVertex3f(r4.x(), r4.y(), r4.z());
    glEnd();

    glEnable(GL_LIGHTING);      // Включим освещение
    glDisable(GL_BLEND);        // Отключим прозрачность
}

void MyGLWidget::paintGL()
{
    // Очистка буфера глубины и буфера цвета
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING); // Включим освещение

    // Рисование сфер
    GLUquadric* sphereObj = gluNewQuadric();
    for (int i=0; i < 64; i++)
    {
        glPushMatrix();
        JSphere S = spheres[i];
        glTranslatef(S.x0(), S.y0(), S.z0());

        // Изменяем цвет сферы в зависимости от того, выделена он или нет
        if (S.isSelected) // Если выделена
        {
          float color[] = { 0.0, 162/255.0, 232/255.0, 1};
          glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }
        else // Если не выделена
        {
          float color[] = { 1 - i * 0.03, 0.1, i * 0.03, 1};
          glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }

        // Рисование сферы
        gluSphere(sphereObj, spheres[i].radius(), 50, 50);

        glPopMatrix();
    }
    gluDeleteQuadric(sphereObj);


    // Рисование выделенной области
    if (!selectedScreenRect.isNull())
    {
        drawSelectedRectangle();
    }

    // Вывод на экран текста
    glColor3f(1, 1, 1);
    QFont text_font("Cambria", 10, QFont::Bold);
    renderText(10, 20, "Колесо мыши - удалить/приблизить", text_font);
    renderText(10, 35, "Левая кнопка мыши - выделить", text_font);
    renderText(10, 50, "Правая кнопка мыши - повернуть сцену", text_font);
    QStringList words = output_text.split('\n');
    for (int i=0; i < words.length(); i++)
     renderText(10, 65 + i*15, words[i], text_font);
}



void MyGLWidget::resetProjectionMatrix()
{
    // Переключение на матрицу проектирования
    glMatrixMode(GL_PROJECTION);
    // Загрузка единичной матрицы проектирования
    glLoadIdentity();
    // Инициализация перспективной проекции
    gluPerspective(30.0, (float)width() / height(), 0.1, 20);
}


void MyGLWidget::resetModelViewMatrix()
{
    // Переключение на видовую матрицу
    glMatrixMode(GL_MODELVIEW);
    // Инициализация видовой матрицы в виде единичной матрицы
    glLoadIdentity();
    // Перенос вглубь экрана
    glTranslatef(0, 0, -zoffset);
    // Поворот объекта
    glMultMatrixf(rotateMatrix.data());
    // Масштабирование
    glScalef(0.3, 0.3, 0.3);

    // Получим текущие матрицы OpenGL для проектирования и аффинных преобразований
    QMatrix4x4 projectionMatrix, modelViewMatrix;
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.data());
    glGetFloatv(GL_MODELVIEW_MATRIX,  modelViewMatrix.data());

    // Умножим матрицы проектирования и видовую матрицы
    Q = projectionMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
}



void MyGLWidget::initSelectingFrustumPlanes()
{
    // Исправим координаты прямоугольника, чтобы его длина и высота не имели отрицательных значений
    QRectF rect = selectedScreenRect.normalized();

    // Скопируем координаты вершин выделенного прямоугольника в локальные переменные
    float x1 = rect.left();
    float x2 = rect.right();
    float y1 = rect.top();
    float y2 = rect.bottom();

    // Координаты ближней и дальней плоскостей отсечения
    float z1 = -1, z2 = 1;
    // z1 соответствует глубине zNear, z2 - zFar параметров функции gluPerspective

    // Будем рассматривать матрицу Q размером 4x4 как одномерный массив из 16 чисел
    float* M = Q.data();

    JPlane px1(M[0] - x1*M[3], M[4] - x1*M[7], M[8] - x1*M[11], M[12] - x1*M[15]);  // Левая плоскость отсечения
    JPlane px2(M[0] - x2*M[3], M[4] - x2*M[7], M[8] - x2*M[11], M[12] - x2*M[15]);  // Правая плоскость отсечения

    JPlane py1(M[1] - y1*M[3], M[5] - y1*M[7], M[9] - y1*M[11], M[13] - y1*M[15]);  // Нижняя плоскость отсечения
    JPlane py2(M[1] - y2*M[3], M[5] - y2*M[7], M[9] - y2*M[11], M[13] - y2*M[15]);  // Верхняя плоскость отсечения

    JPlane pz1(M[2] - z1*M[3], M[6] - z1*M[7], M[10] - z1*M[11], M[14] - z1*M[15]);  // Ближняя плоскость отсечения z = -1
    JPlane pz2(M[2] - z2*M[3], M[6] - z2*M[7], M[10] - z2*M[11], M[14] - z2*M[15]);  // Дальняя плоскость отсечения z = 1

    // Вектора нормалей к плоскостям должны быть повернуты вглубь пирамиды выделения, поэтому
    // в уравнениях плоскостей px2, py2, pz2 меняем знаки на противоположные
    JPlane planes[6] = {px1, -px2, py1, -py2, pz1, -pz2};
    memcpy(selectingPlanes, planes, 6 * sizeof(JPlane));
}


// Обработчик события перемещения указателя мыши (событие происходит при зажатой кнопке мыши)
void MyGLWidget::mouseMoveEvent(QMouseEvent* m_event)
{
    QPoint dp;

    switch (clickedButton)
    {
        // Если зажата правая кнопка мыши, то поворачиваем сцену
    case Qt::RightButton:

        // Вычислим, на сколько переместился курсор мыши между двумя событиями mouseMoveEvent
        dp = m_event->pos() - mousePosition;

        // Изменим матрицу поворота в соответствии с тем, как пользователь переместил курсор мыши
        changeRotateMatrix(rotateMatrix.data(), dp.x(), dp.y());

        // Сохраним текущую позицию мыши
        mousePosition = m_event->pos();

        // Обновим текущую матрицу вида
        resetModelViewMatrix();
        break;

        // Если зажата левая кнопка мыши, то изменяем параметры плоскостей отсечения
    case Qt::LeftButton:

        // Вычислим координаты указателя мыши в экранной системе координат OpenGL
        QPointF A = toOpenGLScreen(m_event->pos());

        // Запомним координату второго угла выделенной прямоугольной области
        selectedScreenRect.setTopRight(A);

        output_text = "";
        if (!selectedScreenRect.isNull())
        {
            // Вычислим уравнения плоскостей отсечения
            initSelectingFrustumPlanes();
            for (int i=0; i < 64; i++)
            {
                // Если сферы внутри усечённой пирамиды, то отмечаем, что они выделены
                spheres[i].isSelected = spheres[i].is_inside(selectingPlanes, 6);
                if (spheres[i].isSelected)
                {
                    output_text = output_text + QString("Отсечение (выделение) пирамидой  сферы №%1\n").arg(i+1);
                }
            }
        }
        break;
    }
    updateGL(); // Перерисовать окно
}


// Процедура предназначена для изменения матрицы поворота, чтобы сцена поворачивалась в нужном направлении строго вслед за указателем мыши.
// Вызывается, когда пользователь изменил положение указателя мыши при зажатой кнопке (мыши)
void MyGLWidget::changeRotateMatrix(float* rotateMatrix, float dx, float dy)
{
    QMatrix4x4 R(rotateMatrix);     // Создание матрицы R из вещественного массива длиной 16
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
    R.copyDataTo(rotateMatrix);     // Копирование матрицы R в исходный массив
}


// Обработчик события прокрутки колеса мыши
void MyGLWidget::wheelEvent(QWheelEvent* w_event)
{
    // При прокрутке колеса мыши изменяем глубину объекта
    zoffset -= w_event->delta() / 500.0;
    resetModelViewMatrix(); // Обновление матрицы вида
    updateGL(); // Перерисовать окно
}


QPointF MyGLWidget::toOpenGLScreen(QPoint pos)
{
    float mx = -1.0f + 2.0f * (double) pos.x() / width();
    float my =  1.0f - 2.0f * (double) pos.y() / height();
    return QPointF(mx, my);
}


void MyGLWidget::mousePressEvent(QMouseEvent* m_event)
{
    clickedButton = m_event->button(); // Нажатая конопка мыши
    mousePosition = m_event->pos();    // Текущая позиция указателя мыши

    // Вычислим координаты указателя мыши в экранной системе координат OpenGL
    QPointF P = toOpenGLScreen(mousePosition);

    // Запомним координату первого угла выделенной прямоугольной области
    selectedScreenRect.setCoords(P.x(), P.y(), P.x(), P.y());

    // Вычислим параметры селектирующего луча
    // Для этого нужно взять две точки, перпендикулярные экранной плоскости
    // Первая точка A должна находится на ближней плоскости отсечения (z = -1), вторая точка B - на дальней плоскости отсечения (z = 1)
    QVector3D A = (IQ * QVector4D(P.x(), P.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (IQ * QVector4D(P.x(), P.y(),  1, 1)).toVector3DAffine();
    selectionRay = JRay(A, B);

    // Проверим, какие объекты лежат на пути селектирующего луча
    output_text = "";
    for (int i=0; i < 64; i++)
    {
        QVector3D R[2];
        spheres[i].isSelected = false;
        int n = spheres[i].intersects(selectionRay, R);
        if (n > 0)
        {
          spheres[i].isSelected = true;
          output_text += QString("Пересечение с лучом сферы №%1: (%2, %3, %4), ").arg(i+1).arg(R[0].x()).arg(R[0].y()).arg(R[0].z());
          output_text += QString("(%1, %2, %3)\n").arg(R[1].x()).arg(R[1].y()).arg(R[1].z());
        }
        //spheres[i].isSelected = spheres[i].isCrossed(selectionRay);
    }
}


void MyGLWidget::mouseReleaseEvent(QMouseEvent *m_event)
{
    clickedButton = Qt::NoButton;
    updateGL(); // Перерисовать окно
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}
