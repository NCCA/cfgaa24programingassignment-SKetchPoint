#include "Block.h"
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

Block::Block(int _type, bool _isAlive, const ngl::Vec3 &_position, float _initialSpeed)
{
    m_type= _type;
    m_isAlive= _isAlive;
    // Set point value based on block type
    switch (_type)
    {
        case 0: // Trash
            m_pointVal =-1;
            break;
        case 1: // Scoop
            m_pointVal =1;
            break;
        case 2: // Bonus scoop
            m_pointVal =3;
            break;
        default:
            std::cerr << "ERROR - Invalid block type: " << _type << std::endl;
            break;
    }
    m_initialSpeed= _initialSpeed;
    m_position= _position;
}

int Block::getType() const
{
    //Grabs to see if its trash, scoop, or bonus scoop
    //helpful to determine if life of cone needs to be reduced
    return m_type;
}

int Block::getPointVal() const
{
    //sees point val, what to add or reduced to player when updating the score
    return m_pointVal;
}

bool Block::getIsAlive() const
{
    //if shown or not, intractable or to delete, used in draw function
    return m_isAlive;
}

ngl::Vec3 Block::getPosition() const
{
    return m_position;
}

float Block::getSpeed() const
{
    return m_initialSpeed;
}

void Block::update(float _deltaTime)
{
    //update to have different axis?

    //velocity= gravity (-9.81 m/s^2) * time
    // Apply gravity to the block's speed
    float gravity = 9.81f;
    float speedWithGravity = m_initialSpeed + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
}

bool Block::isCaught(const ngl::Vec3 &_conePosition) const
{
    //distance between block position and cone position is within a threshold
    // If so, return true (block is caught), otherwise return false

    // check catching distance
    //helps to have collisions based on spheres instead of their actual geo
    constexpr float catchDistance = 2.0f;
    // distance between block position and cone position calc correct?
    float distance = (_conePosition - m_position).length();
    // Check if block is within catch distance
    return distance <= catchDistance;
}
