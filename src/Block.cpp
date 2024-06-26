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
//getter and setter methods
int Block::getType() const
{
    //Grabs to see if its trash, scoop, or bonus scoop
    //helpful to determine if life of cone needs to be reduced
    return m_type;
}
void Block::setType(int _newType)
{
    m_type=_newType;
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
    //indicator if it draws or not
    m_isAlive=_state;
}
ngl::Vec3 Block::getPosition()const
{
    return m_position;
}
void Block::setPosition(ngl::Vec3 _newPosition)
{
    m_position=_newPosition;
}
float Block::getSpeed() const
{
    return m_initialSpeed;
}
void Block::setSpeed(float _newSpeed)
{
    m_initialSpeed=_newSpeed;
}
//render the scoops/blocks only if they are alive, type determines color
void Block::draw(const std::string&_shader)
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
                //initial scoop on top of cone at the start = brown
                ngl::ShaderLib::setUniform("albedo", 0.008f,0.004f,0.0f);
                ngl::VAOPrimitives::draw("sphere");
                break;
            default:
                std::cerr << "ERROR - Invalid block type: " << m_type << std::endl;
                break;
        }

    }
}
void Block::update(float _deltaTime)
{
    //velocity= gravity (-9.81 m/s^2) * time
    // Apply gravity to the block's speed
    float gravity = 0.0981f;
    float speedWithGravity = getSpeed() + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
}
