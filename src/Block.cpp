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
void Block::setIsAlive(bool _state)
{
    m_isAlive=_state;
}
ngl::Vec3 Block::getPosition() const
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
ngl::Vec3 Block::support(const ngl::Vec3& _direction, bool _isBlock, bool _isFarthest) const
{
    // furthest corner of box OR just sphere pos, helps with GJK implementation
    if (_isBlock)
    {
        // Find the corner with the maximum or minimum projection on direction
        float extremumProjection = FLT_MIN;  // For farthest corner (initialize with minimum)
        float otherProjection = FLT_MAX;     // For nearest corner (initialize with maximum)
        ngl::Vec3 extremeCorner;

        // Loop through all 8 corners of the box
        for (int i = -1; i <= 1; i += 2)
        {

            for (int j = -1; j <= 1; j += 2)
            {
                for (int k = -1; k <= 1; k += 2)
                {
                    // Calc corner first from 0,0,0 then to m_position
                    ngl::Vec3 corner = ngl::Vec3(0.5f * (float)i, 0.5f * (float)j, 0.5f * (float)k);
                    corner.m_x += m_position.m_x;
                    corner.m_y += m_position.m_y;
                    corner.m_z += m_position.m_z;

                    // Dot product to get projection on direction
                    float projection = _direction.dot(corner);
                    if (_isFarthest)
                    {
                        if (projection > extremumProjection)
                        {
                            //farthest
                            extremumProjection = projection;
                            extremeCorner = corner;
                        }
                    }
                    else
                    {
                        if (projection < otherProjection)
                        {
                            //nearest
                            otherProjection = projection;
                            extremeCorner = corner;
                        }
                    }
                }
            }
        }
        return extremeCorner;
    }
    else
    {
        // Return center of sphere
        return getPosition();
    }
}
float Block::dot(const ngl::Vec3& _a, const ngl::Vec3& _b)
{
    return _a.m_x * _b.m_x + _a.m_y * _b.m_y + _a.m_z * _b.m_z;
}
float Block::norm(const ngl::Vec3& _a)
{
    return std::sqrt(dot(_a, _a));
}
ngl::Vec3 Block::normalize(const ngl::Vec3& _a)
{
    float mag = norm(_a);
    if (mag > FLT_EPSILON)
    {
        // Avoid division by zero
        return _a / mag;
    } else
    {
        return _a; // Return original vector if magnitude is very small
    }
}
bool Block::circleIntersectRay(float _radius,const ngl::Vec3& _rayDirection)
{
    // Distance between ray origin (cone center) and circle center
    float dx =0.0f;
    float dy =2.0f;
    float dz =0.0f;
    // Solve for the intersection point along the ray parameter `t`
    float a = dot(_rayDirection, _rayDirection);
    float b = 2.0f * (_rayDirection.m_x * dx + _rayDirection.m_z * dz);
    float c = dx * dx + dy * dy + dz * dz - _radius * _radius; // Include dy for y-axis
    float determinant = b * b - 4.0f * a * c;
    // Check for intersection based on the discriminant
    if (determinant < 0.0f)
    {
        // No intersection
        return false;
    } else if (determinant == 0.0f)
    {
        return true;
    } else
    {
        // Two intersections (ray goes through the circle)
        float t1 = (-b + std::sqrt(determinant)) / (2.0f * a);
        float t2 = (-b - std::sqrt(determinant)) / (2.0f * a);
        // Ensure the intersection point is along the positive ray direction (t > 0)
        return (t1 > 0.0f || t2 > 0.0f);
    }
}
bool Block::isCollidingGJK(const ngl::Vec3& _coneCenter, float _coneRadius, const ngl::Vec3& _support, const ngl::Vec3& _negSupport)
{
    // Initial direction towards cone
    ngl::Vec3 a = _support - _coneCenter;
    ngl::Vec3 b;
    std::vector<ngl::Vec3> simplex = {a};
    ngl::Vec3 blockCenter = getPosition();
    ngl::Vec3 rayDirection = blockCenter - _coneCenter;
    // Use ngl library functions (or implement your own) for circle-ray intersection
    bool intersectsTop = circleIntersectRay(_coneRadius,rayDirection);
    bool intersectsBottom = circleIntersectRay(_coneRadius,rayDirection);

    if (!intersectsTop || !intersectsBottom)
    {
        // Ray misses the cylinder, no collision
        return false;
    }

    while (true)
    {
        // b = farthest point on the block in the opposite dir of a (modified with coneRadius)
        b = _negSupport + _coneRadius * normalize(a);
        ngl::Vec3 ao = a - _coneCenter;
        ngl::Vec3 ab = b - a;
        ngl::Vec3 abProjAo = dot(ab, ao) * ab / norm(ab);
        // FLT_EPSILON difference in 1 and least val > 1
        if (norm(abProjAo) < FLT_EPSILON)
        {
            // Simplex contains origin, indicating collision
            return true;
        }
        a = abProjAo;
        simplex.push_back(a);
        // simplex based on last two points
        if (simplex.size() == 2)
        {
            // Line segment
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
    //use sphere to check collisions
    float coneRadius=1.5f;
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
void Block::update(float _deltaTime)
{
    //velocity= gravity (-9.81 m/s^2) * time
    // Apply gravity to the block's speed
    float gravity = 9.81f;
    float speedWithGravity = getSpeed() + gravity * _deltaTime;
    m_position.m_y -= speedWithGravity * _deltaTime;
}
