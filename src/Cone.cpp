#include "Cone.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>

Cone::Cone(int _lives, int _points, float _speed, ngl::Vec3 _position, ngl::Vec3 _colour)
{
    m_lives=_lives;
    m_points=_points;
    m_speed=_speed;
    m_pos=_position;
    m_colour=_colour;

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

void Cone::draw(const std::string &_shader,  const ngl::Mat4 &_view , const ngl::Mat4 &_project)
{
    //drawing out the cone that is user controlled
    ngl::ShaderLib::use(_shader);
    m_transform.setPosition(m_pos);
    m_transform.setRotation(90.0f,0.0f,0.0f);
    ngl::Mat4 MVP= _project*_view*m_transform.getMatrix() ;
    ngl::ShaderLib::setUniform("MVP",MVP);
    ngl::VAOPrimitives::draw("cone");
}
//limitations on xyz movement on cone based on walls
const float boundary=5;
//controls for how much to move the cone and sets it
void Cone::move(float _x, float _y, float _z)
{
    float currentPosY=m_pos.m_y;
    float currentPosZ=m_pos.m_z;
    //checking boundaries before update
    if(m_pos.m_x+_x<= boundary && m_pos.m_x + _x >= -boundary)
    {
        //current pos with changes hits boundary of plane, position isn't updated
        m_pos.m_x+=_x;
    }
    if(m_pos.m_y + _y > boundary|| m_pos.m_y+_y<0)
    {
        //if y jump isn't too high or below 0(ground), updates
        m_pos.m_y+=_y;
    }
    if(m_pos.m_z + _z <= boundary && m_pos.m_z + _z >= -boundary)
    {
        //current pos with changes hits boundary of plane, position isn't updated
        m_pos.m_z+=_z;
    }
}
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