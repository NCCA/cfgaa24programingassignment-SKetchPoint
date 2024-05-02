#include "Cone.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

Cone::Cone(int _lives, int _points, float _speed, ngl::Vec3 _position, ngl::Vec3 _colour)
{
    m_lives=_lives;
    m_points=_points;
    m_speed=_speed;
    m_position=_position;
    m_colour=_colour;

}
//const prevents changes when it's intended to retrieve info
ngl::Vec3 Cone::getPosition() const
{
    return m_position;
}

void Cone::setPosition(const ngl::Vec3 _pos)
{
    m_position = _pos;
}

int Cone::getLives() const
{
    return m_lives;
}

void Cone::setLives(int _lives)
{
    m_lives = _lives;
}

int Cone::getPoints() const
{
    return m_points;
}

void Cone::setPoints(int _points)
{
    m_points = _points;
}

//void Cone::moveForward(float _speed)
//{
//    m_position.m_z -= _speed;
//}
//
//void Cone::moveBackward(float _speed)
//{
//    m_position.m_z += _speed;
//}
//
//void Cone::moveLeft(float _speed)
//{
//    m_position.m_x -= _speed;
//}
//
//void Cone::moveRight(float _speed)
//{
//    m_position.m_x += _speed;
//}

bool Cone::checkCollision(const std::vector<ngl::Vec3> &_blockPositions)
{
    // Implementation for collision checking
    //if area of block hits rim of cone & block does not hit ground, return true, else false
    return false; // Placeholder
}

void Cone::updateScoreAndLives(int _scoreChange, int _livesChange)
{
    //score will always change
    // positive when successful catch good block
    //negative when good block hits ground or cone collides with bad block
    //if catch bad block, lives also change
    m_points += _scoreChange;
    m_lives += _livesChange;
}