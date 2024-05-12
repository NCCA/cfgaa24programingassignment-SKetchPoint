#ifndef EMITTER_H
#define EMITTER_H

#include <vector>
#include <random>
#include "Block.h"

class Emitter {
public:
    Emitter(int numScoops, float minSpeed, float maxSpeed, float minX, float maxX, float minY, float maxY);
    std::vector<Block> emitScoops();

private:
    int m_numScoops;
    float m_minSpeed, m_maxSpeed;
    float m_minX, m_maxX, m_minY, m_maxY;
    std::random_device m_rd;
    std::mt19937 m_rng;
};

#endif // EMITTER_H