#ifndef BONSAI_H
#define BONSAI_H

#include <vector>
#include <memory>

#include "asciiarrows.h"

class Branch
{
public:
    static Branch makeRoot(const int x, const int y);
    void grow();
    QString map(const QRect &rect) const;

private:
    static const size_t m_maxChildren = 1;
    static const int m_maxLevel = 20;
    static const int m_maxY = 16;
    static const int m_maxX = 22;
    static const int m_maxXperLevel = -1;

    Branch(const int x, const int y, const int offset, const int level);
    /// returns offset (right-side) or -offset (left-side)
    /// or 0 if no child can be created
    int placeForNextChildren() const;
    inline bool hasChildren() const { return m_children.size() > 0; }
    inline bool canGrowX() const { return m_offset && (std::abs(m_x) < m_maxX + m_maxXperLevel * m_level); }
    inline bool canGrowY() const { return m_y < m_maxY; }
    bool tryGrowChild();
    void fillMap(int *b, const QRect rect, int x0, int y0) const;
    int m_x;
    int m_y;
    int m_offset;
    int m_level;
    std::vector<std::shared_ptr<Branch>> m_children;

};

class Bonsai
{
public:
    Bonsai() = default;
};

#endif // BONSAI_H
