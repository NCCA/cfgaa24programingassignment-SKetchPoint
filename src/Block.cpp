#include "Block.h"
#include <cmath>
#include <iostream>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include<cfloat>

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
    float speedWithGravity = getSpeed() + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
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
ngl::Vec3 Block::support(const ngl::Vec3& direction, bool isBlock, bool isFarthest) const
{
    // furthest corner of box OR just sphere pos, helps with GJK implementation
    if (isBlock) {
        // Find the corner with the maximum or minimum projection on direction
        float extremumProjection = FLT_MIN;  // For farthest corner (initialize with minimum)
        float otherProjection = FLT_MAX;     // For nearest corner (initialize with maximum)
        ngl::Vec3 extremumCorner;

        // Loop through all 8 corners of the box
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                for (int k = -1; k <= 1; k += 2) {
                    // Calc corner first from 0,0,0 then to m_position
                    ngl::Vec3 corner = ngl::Vec3(0.5f * i, 0.5f * j, 0.5f * k);
                    corner.m_x += m_position.m_x;
                    corner.m_y += m_position.m_y;
                    corner.m_z += m_position.m_z;

                    // Dot product to get projection on direction
                    float projection = direction.dot(corner);
                    if (isFarthest)
                    {
                        if (projection > extremumProjection)
                        {
                            //farthest
                            extremumProjection = projection;
                            extremumCorner = corner;
                        }
                    } else
                    {
                        if (projection < otherProjection)
                        {
                            //nearest
                            otherProjection = projection;
                            extremumCorner = corner;
                        }
                    }
                }
            }
        }

        return extremumCorner;
    } else {
        // Return center of sphere
        return getPosition();
    }
}
float Block::dot(const ngl::Vec3& a, const ngl::Vec3& b)
{
    return a.m_x * b.m_x + a.m_y * b.m_y + a.m_z * b.m_z;
}
float Block::norm(const ngl::Vec3& a)
{
    return std::sqrt(dot(a, a));
}
ngl::Vec3 Block::normalize(const ngl::Vec3& a)
{
    float mag = norm(a);
    if (mag > FLT_EPSILON)
    {
        // Avoid division by zero
        return a / mag;
    } else
    {
        return a; // Return original vector if magnitude is very small
    }
}
bool Block::isCollidingGJK(const ngl::Vec3& coneCenter, float coneRadius, const ngl::Vec3& support, const ngl::Vec3& negSupport)
{
    // Initial direction towards cone
    ngl::Vec3 a = support - coneCenter;
    ngl::Vec3 b;
    std::vector<ngl::Vec3> simplex = {a};

    while (true)
    {
        //b=farthest point on the block in the opposite dir of a
        b = negSupport;
        ngl::Vec3 ao = a - coneCenter;
        ngl::Vec3 ab = b - a;
        // ao onto ab projection
        ngl::Vec3 abProjAo = dot(ab, ao) * ab / norm(ab);
        //FLT_EPSILON difference in 1 and least val > 1
        if (norm(abProjAo) < FLT_EPSILON)
        {
            // Simplex contains origin, indicating collision
            return true;
        }
        a = abProjAo;
        simplex.push_back(a);
        // Update simplex based on last two points (a, b)
        if (simplex.size() == 2)
        {
            // Line segment case
            ngl::Vec3 abNormalized = normalize(ab);
            if (dot(ao, abNormalized) > 0.0f)
            {
                simplex[0] = a;
            } else
            {
                // No collision
                return false;
            }
        }
        //triangle case not calc
    }
    // Shouldn't reach here
    return false;
}
bool Block::isCaught(const ngl::Vec3 &_conePosition)
{
    bool returnCollision= false;
    if(!getIsAlive())
    {
        //no need to hit again, is already caught
        return returnCollision;
    }
    //calc cone box by a cylinder
    //cone + initial scoop height~2
    float coneHeight=1.0f;
    float coneRadius=0.6f;
    float blockRadius=0.5f;
    ngl::Vec3 direction=_conePosition - getPosition();
    //determining hit box of the Block
    if(m_type==0)
    {
        //if garbage (it is a cube)
        //on block side length is 1, so .5 for each corner
        //GJK collision detection algorithm
        returnCollision = isCollidingGJK(_conePosition,coneRadius,support(direction,true,true),support(direction,true,false));
    }
    else
    {
        //if Block is any other type(it is a sphere), radius is .5
        //GJK collision detection algorithm
        returnCollision = isCollidingGJK(_conePosition,coneRadius,support(direction,false,true),support(direction,false,false));
    }
    return returnCollision;
}
