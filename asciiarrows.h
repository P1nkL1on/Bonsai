#ifndef ASCIIARROWS_H
#define ASCIIARROWS_H

#include <QPoint>
#include <QString>
#include <QRect>

namespace AsciiArrows {

void clearBuffer(int *b, const QSize &size);
void drawArrow(int *b, const QRect &rect, const QPoint &start, const int length, const Qt::Orientation orientation);
QString bufferToString(int *b, const QSize &size);

}

#endif // ASCIIARROWS_H
