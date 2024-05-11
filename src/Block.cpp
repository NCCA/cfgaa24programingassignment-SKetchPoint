#include "Block.h"
#include <cmath>
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include<cfloat>
#include <algorithm>

Block::Block(int _type, bool _isAlive, ngl::Vec3 _position, float _initialSpeed)
{
    m_type= _type;
    m_isAlive= _isAlive;
    // Set point value based on block type
    switch (_type)
    {
        case 0: // Trash
            m_pointVal =-2;//deducts points
            break;
        case 1: // Scoop
            m_pointVal =1;//adds point
            break;
        case 2: // Bonus scoop
            m_pointVal =3;//adds points
            break;
        case 3: //Initial Scoop
            m_pointVal=0;//nothing
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
void Block::setIsAlive(bool _state)
{
    m_isAlive=_state;
}
ngl::Vec3 Block::getPosition()const
{
    return m_position;
}
float Block::getSpeed() const
{
    return m_initialSpeed;
}
void Block::draw(const std::string &_shader)
{
    //draws blocks that are alive only
    if(m_isAlive)
    {
        ngl::ShaderLib::use(_shader);
        switch(getType())
        {
            case 0:
                //trash=green
                ngl::ShaderLib::setUniform("albedo", 0.0156f,0.08235f,0.05098f);
                ngl::VAOPrimitives::draw("cube");
                break;
            case 1:
                //normal=off-white
                ngl::ShaderLib::setUniform("albedo", 0.933f,0.898f,0.898f);
                ngl::VAOPrimitives::draw("sphere");
                break;
            case 2:
                //bonus=purple
                ngl::ShaderLib::setUniform("albedo", 0.07f,0.0f,0.07f);
                ngl::VAOPrimitives::draw("sphere");
                break;
            case 3:
                //scoop on top of cone at the start
                ngl::ShaderLib::setUniform("albedo", 0.933f,0.898f,0.898f);
                ngl::VAOPrimitives::draw("sphere");
                break;
            default:
                std::cerr << "ERROR - Invalid block type: " << m_type << std::endl;
                break;
        }

    }
}
float Block::distanceBetweenVec(ngl::Vec3 &_point1,ngl::Vec3 &_point2)
{
    float distance = sqrtf(_point2.m_x+_point1.m_x)+sqrtf(_point2.m_y+_point1.m_y)+sqrtf(_point2.m_z+_point1.m_z);
    distance= sqrtf(distance);
    return distance;
}

bool Block::isCaught(ngl::Vec3 _conePosition)
{
    //implement AABB
    //cone has a sphere on top making cone position have a +0.5 in the y direction in cone position
    ngl::Vec3 userScoop={_conePosition.m_x,_conePosition.m_y+0.5f,_conePosition.m_z};
    // Define AABB based on block type
    if (getType() == 0)
    {
        // AABB for a cube with side length 1, centered at origin
        ngl::Vec3 aabbMin(_conePosition.m_x-0.5f, _conePosition.m_y, _conePosition.m_z-0.5f);  // Minimum corner
        ngl::Vec3 aabbMax(_conePosition.m_x+0.5f, _conePosition.m_y+1.0f, _conePosition.m_z+0.5f);   // Maximum corner

        // Check for cone position inside AABB
        return(_conePosition.m_x >= aabbMin.m_x &&
                           _conePosition.m_x <= aabbMax.m_x &&
                           _conePosition.m_y >= aabbMin.m_y &&
                           _conePosition.m_y <= aabbMax.m_y &&
                           _conePosition.m_z >= aabbMin.m_z &&
                           _conePosition.m_z <= aabbMax.m_z);
    }
    else
    {
        float sphereRadius=0.5;
        float distance = distanceBetweenVec(_conePosition, userScoop);
        // Check if squared distance is less than or equal to squared radius
        return (distance >= sphereRadius*sphereRadius);
    }
    return false;
}
void Block::update(float _deltaTime)
{
    //velocity= gravity (-9.81 m/s^2) * time
    // Apply gravity to the block's speed
    float gravity = 9.81f;
    float speedWithGravity = getSpeed() + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
}
