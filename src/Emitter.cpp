#include "Emitter.h"
#include <random>

Emitter::Emitter(int _numScoops, float _minSpeed, float _maxSpeed, float _minX, float _maxX, float _minY, float _maxY)
{
    // Set initial values
    m_numScoops = _numScoops;
    m_minSpeed = _minSpeed;
    m_maxSpeed = _maxSpeed;
    m_minX = _minX;
    m_maxX = _maxX;
    m_minY = _minY;
    m_maxY = _maxY;

    // Initialize random number generator
    std::random_device m_rd;
    m_rng = std::mt19937(m_rd());
}

std::vector<Block> Emitter::emitScoops()
{
    std::vector<Block> scoops;
    // Generate random properties for each scoop
    for (int i = 0; i < m_numScoops; ++i)
    {
        // Random scoop type (0: trash, 1: scoop, 2: bonus)
        int type = std::uniform_int_distribution<int>(0, 2)(m_rng);
        // Random initial position within the emitter's range
        float x = std::uniform_real_distribution<float>(m_minX, m_maxX)(m_rng);
        float y = m_minY;
        float z = std::uniform_real_distribution<float>(-1.0f, 1.0f)(m_rng);
        // Random speed within the specified range
        float speed = std::uniform_real_distribution<float>(m_minSpeed, m_maxSpeed)(m_rng);
        // Create a new block object with these properties
        scoops.push_back(Block(type, true, ngl::Vec3(x, y, z), speed));
    }

    return scoops;
}