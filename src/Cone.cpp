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
float Cone::distanceBetweenVec(ngl::Vec3 &_point1, ngl::Vec3 _point2)
{
    // Calculate squared distances for each dimension
    float sqrX = (_point2.m_x - _point1.m_x) * (_point2.m_x - _point1.m_x);
    float sqrY = (_point2.m_y - _point1.m_y) * (_point2.m_y - _point1.m_y);
    float sqrZ = (_point2.m_z - _point1.m_z) * (_point2.m_z - _point1.m_z);
    float distanceSqr = sqrX + sqrY + sqrZ;
    // Calculate and return the actual distance
    return sqrtf(distanceSqr);
}

bool Cone::checkCollision(ngl::Vec3 _blockPosition, int _blockType, int _pointVal, ngl::Vec3 _playerPosition)
{
    //implement AABB
    bool isColliding = false;
    if (_blockType == 0) {
        // AABB collision for a 1*1*1 box centered at _blockPosition
        float halfBoxSize = 0.5f;
        float playerYOffset = 0.5f;
        float playerRadius = 0.5f;
        // Check for overlap on each axis
        isColliding = (std::abs(_blockPosition.m_x - _playerPosition.m_x) <= halfBoxSize + playerRadius) &&
                      (std::abs(_blockPosition.m_y - (_playerPosition.m_y + playerYOffset)) <= halfBoxSize + playerRadius) &&
                      (std::abs(_blockPosition.m_z - _playerPosition.m_z) <= halfBoxSize + playerRadius);
    } else {
        // Sphere-sphere collision
        float blockRadius = 0.5f;
        float playerYOffset = 0.5f;
        float playerRadius = 0.5f;

        float distance =distanceBetweenVec(_blockPosition, _playerPosition + ngl::Vec3(0.0f, playerYOffset, 0.0f));
        isColliding = distance <= blockRadius + playerRadius;
    }
    if (isColliding)
    {
        // Print cone position for debugging (optional)
        std::cout << "caught TRUE " << std::endl;

        // Update score and lives based on block type
        switch (_blockType)
        {
            case 0: // Trash
                updateScoreAndLives(_pointVal, -1);
                break;
            case 1: // Scoop
                updateScoreAndLives(_pointVal, 0);
                break;
            case 2: // Bonus scoop
                updateScoreAndLives(_pointVal, 1);
                break;
            default:
                std::cerr << "ERROR - Invalid block type: " <<_pointVal << std::endl;
                break;
        }
        return true;
    }
    else
    {
        return false;
    }
}