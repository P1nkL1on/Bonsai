#ifndef ASCIIARROWS_TESTS_H
#define ASCIIARROWS_TESTS_H

#include <QObject>

namespace AsciiArrows {

class Tests : public QObject
{
    Q_OBJECT
private slots:
    void arrows_data();
    void arrows();
    void rotate90_data();
    void rotate90();
    void rotateArrow90_data();
    void rotateArrow90();
};

}

#endif // ASCIIARROWS_TESTS_H
