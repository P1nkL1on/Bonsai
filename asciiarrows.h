#ifndef ASCIIARROWS_H
#define ASCIIARROWS_H

#include <QPoint>
#include <QString>
#include <QRect>

namespace AsciiArrows {

void clearBuffer(int *b, const QSize &size);
void drawArrow(int *b, const QRect &rect, const QPoint &start, const int length, const Qt::Orientation orientation);
// todo: remove mode later (only exists because qDebug can't handle ASCII properly)
QString bufferToString(int *b, const QSize &size, const int mode = 0);

}

#endif // ASCIIARROWS_H
