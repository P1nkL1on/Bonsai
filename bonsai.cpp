#include "bonsai.h"

#include <QtCore>

#include <random>
#include <iostream>

std::random_device rd;
std::mt19937 mt(rd());

inline int random(const int max)
{
    return std::uniform_int_distribution<int>(0, max - 1)(mt);
}

Branch Branch::makeRoot(const int x, const int y)
{
    return Branch(x, y, 0, 0);
}

void Branch::grow()
{
    if (random(4) && hasChildren())
        return m_children[random(m_children.size())]->grow();
    if (random(3) && canGrowX()) {
        m_x += m_x > 0 ? 1 : -1;
        return;
    }
    if (random(2) && canGrowY()) {
        m_y += 1;
        return;
    }
    tryGrowChild();
}

QString Branch::map(const QRect &rect) const
{
    const QSize size = rect.size();
    int b[size.width() * size.height()];
    AsciiArrows::clearBuffer(b, size);
    fillMap(b, rect, 0, 0);
    return AsciiArrows::bufferToString(b, size);
}

void Branch::fillMap(int *b, const QRect rect, int x0, int y0) const
{
    y0 += m_offset;
    if (m_x)
        AsciiArrows::drawArrow(b, rect, {x0, y0}, m_x, Qt::Horizontal);
    if (m_y)
        AsciiArrows::drawArrow(b, rect, {x0 + m_x, y0}, m_y, Qt::Vertical);

    for (const std::shared_ptr<Branch> &child : m_children)
        child->fillMap(b, rect, x0 + m_x, y0);
}

Branch::Branch(const int x, const int y, const int offset, const int level) :
    m_x(x),
    m_y(y),
    m_offset(offset),
    m_level(level)
{
    Q_ASSERT(y >= 0);
    Q_ASSERT(offset >= 0);
    Q_ASSERT(level >= 0);
}

int Branch::placeForNextChildren() const
{
    if (m_children.size() >= m_maxChildren)
        return 0;
    if (m_y == 0)
        return 0;

    const int side = random(2) * 2 - 1;
    if (!hasChildren())
        // any side at any height
        return side * random(m_y);

    const int lastOffset = m_children[m_children.size() - 1]->m_offset;
    if (lastOffset == m_y - 1)
        // stop growth on last
        return 0;

    const int lastsSide = m_children[m_children.size() - 1]->m_x > 0 ? 1 : -1;
    const int offset = random(m_y - lastOffset) + lastOffset;
    if (offset == lastOffset)
        // on the other side
        return -lastsSide * offset;

    // on any side on next height
    return offset * side;
}

bool Branch::tryGrowChild()
{
    if (m_level >= m_maxLevel)
        return false;
    const int placeNext = placeForNextChildren();
    if (placeNext == 0)
        return false;
    const int offset = std::abs(placeNext);
    const int side = placeNext > 0 ? 1 : -1;
    m_children.push_back(std::shared_ptr<Branch>(new Branch(side, 0, offset, m_level + 1)));
    return true;
}
