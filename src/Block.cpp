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
    //velocity= gravity (-9.81 m/s^2) * time
    // Apply gravity to the block's speed
    float gravity = 9.81f;
    float speedWithGravity = m_initialSpeed + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
}
void Block::draw(const std::string &_shader)
{
    //draws blocks that are alive only
    if(m_isAlive)
    {
        ngl::ShaderLib::use(_shader);
        switch(m_type)
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
bool Block::isCaught(const ngl::Vec3 &_conePosition) const
{
    float margineCatch= 0.2;
    //on block side length is 1, so .5 for each corner
    //block center (0,0,0)
    //block vertices
    //(.5,.5,.5),(-.5,.5,.5),(-.5,-.5,.5),(.5,-.5,.5)
    //(.5,.5,-.5),(-.5,.5,-.5),(-.5,-.5,-.5),(.5,-.5,-.5)
    //calc cone box by a cylinder
    //cone center (0,0,0)
    //cone point(0,-1.5,0)
    //cone rim(0,2,0)
    //
    return false;
}
