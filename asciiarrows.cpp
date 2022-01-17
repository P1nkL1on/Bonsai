#include "asciiarrows.h"

#include <QtDebug>

namespace AsciiArrows {

using VV = std::vector<char>;

// m Code {_a, _b, _c, _d, _e, _f, _g, _h, _i, _k, _l, _m, _n, _o, _p, _r, _n2, _o2, _p2, _r2, _err};
///         ░,  ─,  │,  └,  ┘,  ┌,  ┐,  ├,  ┤,  ┴,  ┬,  ┼,  ↓,  →,  ←,  ↑,  *↓,  *→,  *←,  *↑,   *
VV __u    { 0,  0,  1,  1,  1,  0,  0,  1,  1,  1,  0,  1,  1,  0,  0,  0,   2,   0,   0,   0,   0};
VV __s    {0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0, 0,0, 2,0, 0,2, 0,0, 0,0,};
VV __d    { 0,  0,  1,  0,  0,  1,  1,  1,  1,  0,  1,  1,  0,  0,  0,  1,   0,   0,   0,   2,   0};

// u is up part, s is left and right accordingly, d is down part

enum Code {
    _a = 0b00000, //░
    _r = 0b00001, //↑
    _p = 0b00010, //←
    _f = 0b00011, //┌
    _o = 0b00100, //→
    _g = 0b00101, //┐
    _b = 0b00110, //─
    _l = 0b00111, //┬
    _n = 0b01000, //↓
    _c = 0b01001, //│
    _d = 0b01010, //└
    _h = 0b01011, //├
    _e = 0b01100, //┘
    _i = 0b01101, //┤
    _k = 0b01110, //┴
    _m = 0b01111, //┼
    _r2= 0b10001, //↑ if start
    _p2= 0b10010, //← if start
    _o2= 0b10100, //→ if start
    _n2= 0b11000, //↓ if start
};

inline int removeStartByte(int c)
{
    // check fifth bit
    if ((c >> 4) & 1U) {
        c ^= 1U << 4;
        // arrow but an end type (should be shown as - | accordingly)
        if (c == 0b0001)
            c |= 1U << 3;
        if (c == 0b1000)
            c |= 1U << 0;
        if (c == 0b0010)
            c |= 1U << 2;
        if (c == 0b0100)
            c |= 1U << 1;
    }
    return c;
}

void clearBuffer(int *b, const QSize &size)
{
    for (int i = 0; i < size.width() * size.height(); ++i)
        b[i] = 0b0;
}

int *__bb;
QSize __ss;

template <typename F>
void p(
        const int vstart, const int vother, // x and y (or y and x)
        const int smin, const int smax,     // screen clamp min max
        const int length,
        const int bitInv, const int bitFrw, // set bit to 1 if inv / fwd / both if middle
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
    int max = es;
    int min = ss;
    if (max < min)
        std::swap(max, min);

    for (int v = min; v <= max; ++v) {
        const bool isInv = v == min;
        const bool isFwd = v == max;
        int bit = 0b0000;
        if (!isFwd)
            bit |= 1U << bitInv;
        if (!isInv)
            bit |= 1U << bitFrw;
        // add a start bit in case its a beginning
        if ((isFwd || isInv) && (max > min) && (v == s))
            bit |= 1U << 4;
        f(v, vother, bit);
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
        return p(start.x(), start.y(), rect.left(), rect.right(), length, 1, 2, [b, w](int x, int y, int bit){
            b[y * w + x] |= bit;
        });
    return p(start.y(), start.x(), rect.top(), rect.bottom(), length, 3, 0, [b, w](int y, int x, int bit){
        b[y * w + x] |= bit;
    });
}

QString bufferToString(int *b, const QSize &size, const int mode)
{
//    const QString s =  ".─│└┘┌┐├┤┴┬┼↓→←↑│──│*";
//    const QString s2 = " bcdefghiklmnoprMNOP*";
    const QString s  = ".↑←┌→┐─┬↓│└├┘┤┴┼";
    const QString s2 = "arpfogblncdheikm";
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
            l += source->at(removeStartByte(c));
        }
        r += (isBorder ? (border[4] + l + border[4]) : (l)) + '\n';
    }
    r += isBorder ? QString(border[2]).leftJustified(w + 1, border[5]).append(border[3]).append('\n') : "";
    return r;
}

}
