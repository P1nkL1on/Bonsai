#include "asciiarrows.h"

namespace AsciiArrows {

enum Code {_a, _b, _c, _d, _e, _f, _g, _h, _i, _k, _l, _m, _n, _o, _p, _r, _err};
///         ░,  ─,  │,  └,  ┘,  ┌,  ┐,  ├,  ┤,  ┴,  ┬,  ┼,  →,  ←,  ↑,  ↓,  *
const QString s = ".─│└┘┌┐├┤┴┬┼→←↑↓*";

void clearBuffer(int *b, const QSize &size)
{
    for (int i = 0; i < size.width() * size.height(); ++i)
        b[i] = 0;
}

template <typename F>
void p(
        const int vstart, const int vother, // x and y (or y and x)
        const int smin, const int smax,     // screen clamp min max
        const int length,
        F &&f)                              // f() called on x, y, isStart, isEnd, isInverted
{
    Q_ASSERT(smin < smax);
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
    }
}

void drawArrow(int *b, const QRect &rect, const QPoint &start, const int length, const Qt::Orientation orientation)
{
    if (!length)
        return;
    const int w = rect.width();
    if (orientation == Qt::Horizontal)
        return p(start.x(), start.y(), rect.left(), rect.right(), length, [b, w](int x, int y, bool isStr, bool isEnd, bool isLft){
            int *bb = b + y * w + x;
            switch (*bb) {
            case _a:                   *bb = isEnd ? (isLft ? _o : _n) : _b; break;
            case _b: case _n: case _o: *bb = _b; break;
            default:                   *bb = _err; break;
            }
        });

    return p(start.y(), start.x(), rect.top(), rect.bottom(), length, [b, w](int y, int x, bool isStr, bool isEnd, bool isDwn){
        int *bb = b + y * w + x;
        switch (*bb) {
        case _a:                   *bb = isEnd ? (isDwn ? _r : _p) : _c; break;
        case _c: case _p: case _r: *bb = _c; break;
        default:                   *bb = _err; break;
        }
    });
}

QString bufferToString(int *b, const QSize &size)
{
    const int w = size.width();
    const int h = size.height();
    QString r;
    for (int y = 0; y < h; ++y) {
        QString l;
        for (int x = 0; x < w; ++x) {
            const int c = b[(h - 1 - y) * w + x];
            l +=  s[c];
        }
        r += l + "\n";
    }
    return r;
}

}
