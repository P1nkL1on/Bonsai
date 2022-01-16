#include "asciiarrows.h"

#include <QtDebug>

namespace AsciiArrows {

enum Code {_a, _b, _c, _d, _e, _f, _g, _h, _i, _k, _l, _m, _n, _o, _p, _r, _n2, _o2, _p2, _r2, _err};
///         ░,  ─,  │,  └,  ┘,  ┌,  ┐,  ├,  ┤,  ┴,  ┬,  ┼,  ↓,  →,  ←,  ↑,  *↓,  *→,  *←,  *↑,   *

void clearBuffer(int *b, const QSize &size)
{
    for (int i = 0; i < size.width() * size.height(); ++i)
        b[i] = 0;
}

int *__bb;
QSize __ss;

template <typename F>
void p(
        const int vstart, const int vother, // x and y (or y and x)
        const int smin, const int smax,     // screen clamp min max
        const int length,
        F &&f)                              // f() called on x, y, isStart, isEnd, isInverted
{
    Q_ASSERT(smin <= smax);
    const int l = length - ((length > 0) ? 1 : -1);
    const int s = vstart;
    const int e = vstart + l;
    if (((s < smin) && (e < smin)) || ((s > smax) && (e > smax)))
        return;
    const int ss = std::max(smin, std::min(smax, s));
    const int es = std::max(smin, std::min(smax, e));
    const int isInv = length < 0;
    int max = es;
    int min = ss;
    if (max < min)
        std::swap(max, min);

    for (int v = min; v <= max; ++v) {
        const bool isStr = v == s;
        const bool isEnd = v == e;
        f(v, vother, isStr, isEnd, isInv);

        // qDebug().noquote().nospace() << "\n" << v << "\n" << bufferToString(__bb, __ss, 3);
    }
}

void drawArrow(int *b, const QRect &rect, const QPoint &start, const int length, const Qt::Orientation orientation)
{
    if (!length)
        return;
    __bb = b;
    __ss = rect.size();
    const int w = rect.width();
    if (orientation == Qt::Horizontal)
        return p(start.x(), start.y(), rect.left(), rect.right(), length, [b, w](int x, int y, bool isStr, bool isEnd, bool isLft){
            int *bb = b + y * w + x;
            switch (*bb) {
            case _a: /*  ░  */           *bb = isEnd ? (isLft ? _p : _o) : isStr ? (isLft ? _p2 : _o2) : _b; break;
            case _c: /*  │  */           *bb = isEnd ? (isLft ? _h : _i) : isStr ? (isLft ? _i : _h) : _m; break;
            case _p: /*  ←  */           *bb = isEnd &&  isLft ? _p : _b; break;
            case _o: /*  →  */           *bb = isEnd && !isLft ? _o : _b; break;
            case _r: /*  ↑  */           *bb = isEnd ? (isLft ? _g : _e) : isStr ? (isLft ? _g : _f) : _l; break;
            case _r2:/*  ↑2 */           *bb = isEnd ? (isLft ? _d : _e) : isStr ? (isLft ? _e : _d) : _l; break;
            case _b: case _p2: case _o2: *bb = _b; break;
            default:                     /*qDebug() << "-- unknown" << int(*bb); */*bb = _err; break;
            }
        });

    return p(start.y(), start.x(), rect.top(), rect.bottom(), length, [b, w](int y, int x, bool isStr, bool isEnd, bool isDwn){
        int *bb = b + y * w + x;
        switch (*bb) {
        case _a: /*  ░  */           *bb = isEnd ? (isDwn ? _n : _r) : isStr ? (isDwn ? _n2 : _r2) : _c; break;
        case _b: /*  ─  */           *bb = isEnd ? (isDwn ? _k : _l) : isStr ? (isDwn ? _l : _k) : _m; break;
        case _n: /*  ↓  */           *bb = isEnd &&  isDwn ? _n : _c; break;
        case _r: /*  ↑  */           *bb = isEnd && !isDwn ? _r : _c; break;
        case _o: /*  →  */           *bb = isEnd ? (isDwn ? _f : _e) : isStr ? (isDwn ? _g : _e) : _h; break;
        case _o2:/*  →2 */           *bb = isEnd ? (isDwn ? _d : _f) : isStr ? (isDwn ? _f : _d) : _h; break;
        case _c: case _n2: case _r2: *bb = _c; break;
        default:                     /*qDebug() << "| unknown" << int(*bb); */*bb = _err; break;
        }
    });
}

QString bufferToString(int *b, const QSize &size, const int mode)
{
    const QString s =  ".─│└┘┌┐├┤┴┬┼↓→←↑│──│*";
    const QString s2 = " bcdefghiklmnoprMNOP*";
    // const QString border = "╔╗╚╝║═";
    const QString border = "####|=";

    const int w = size.width();
    const int h = size.height();
    const bool isCodes = mode % 2;
    const bool isBorder = mode / 2;
    const QString *source = isCodes ? &s2 : &s;
    QString r = isBorder ? QString(border[0]).leftJustified(w + 1, border[5]).append(border[1]).append('\n') : "";
    for (int y = 0; y < h; ++y) {
        QString l;
        for (int x = 0; x < w; ++x) {
            const int c = b[(h - 1 - y) * w + x];
            l += source->at(c);
        }
        r += (isBorder ? (border[4] + l + border[4]) : (l)) + '\n';
    }
    r += isBorder ? QString(border[2]).leftJustified(w + 1, border[5]).append(border[3]).append('\n') : "";
    return r;
}

}
