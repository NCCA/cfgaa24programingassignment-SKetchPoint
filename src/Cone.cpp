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
ngl::Vec3 Cone::getPosition()
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
    m_speed = _speed;
}
void Cone::draw(const std::string &_shader,float _r,float _g,float _b)
{

    ngl::ShaderLib::use(_shader);
    ngl::ShaderLib::setUniform("albedo", _r, _g,_b);
    ngl::VAOPrimitives::draw("cone");
}
//limitations on xyz movement on cone based on walls
//const float boundary=5;
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
    setPoints(m_points+_scoreChange);
    setLives(m_lives+_livesChange);
}
void Cone::checkCollision(std::unique_ptr<Block>&_scoop)
{
    // Implementation for collision checking
    //if area of block hits player, it is caught, points/lives added or deducted accordingly
    if(_scoop->isCaught(getPosition()))
    {
        std::cout << "Cone position: " << std::endl;
        switch (_scoop->getType())
        {
            //update lives and score for the player, then since it's caught, have the block disappear
            case 0: // Trash
                updateScoreAndLives(_scoop->getPointVal(),-1);
                break;
            case 1: // Scoop
                updateScoreAndLives(_scoop->getPointVal(),0);
                break;
            case 2: // Bonus scoop
                updateScoreAndLives(_scoop->getPointVal(),1);
                break;
            default:
                std::cerr << "ERROR - Invalid block type: " << _scoop->getType() << std::endl;
                break;
        }
        _scoop->setIsAlive(false);
    }
}
