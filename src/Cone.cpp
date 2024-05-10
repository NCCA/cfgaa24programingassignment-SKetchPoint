#include "Cone.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <iostream>
Cone::Cone(int _lives, int _points, float _speed, ngl::Vec3 _position)
{
    m_lives=_lives;
    m_points=_points;
    m_speed=_speed;
    m_pos=_position;

}
//note: const prevents changes when it's intended to retrieve info
ngl::Vec3 Cone::getPosition() const
{
    return m_pos;
}

void Cone::setPosition(const ngl::Vec3 _pos)
{
    m_pos = _pos;
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
float Cone::getSpeed() const
{
    return m_speed;
}

void Cone::setSpeed(int _speed)
{
    m_lives = _speed;
}

void Cone::draw(const std::string &_shader,float _r,float _g,float _b)
{

    ngl::ShaderLib::use(_shader);
    ngl::ShaderLib::setUniform("albedo", _r, _g,_b);
    ngl::VAOPrimitives::draw("cone");
}
//limitations on xyz movement on cone based on walls
///const float boundary=5;
//controls for how much to move the cone and sets it
void Cone::move(float _x, float _y, float _z, float _boundary)
{
    float axis[]={_x,_y,_z};
    for (int i=0;i<3;i++)
    {
        //checking limitations based on boundary of walls for cone movement
        if (m_pos[i] + axis[i] <= _boundary && m_pos[i] + axis[i] >= -_boundary)
        {
            m_pos[i] += axis[i];
        }
    }
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
bool Cone::checkCollision(const std::vector<ngl::Vec3> &_blockPositions)
{
    // Implementation for collision checking
    //if area of block hits rim of cone & block does not hit ground, return true, else false
    return false; // Placeholder
}
