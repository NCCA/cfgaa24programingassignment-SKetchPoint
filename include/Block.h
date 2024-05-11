#ifndef BLOCK_H
#define BLOCK_H

#include <ngl/Vec3.h>
#include <ngl/VAOPrimitives.h>
class Block
{
public:
    Block(int _type, bool _isAlive, const ngl::Vec3 &_position, float _initialSpeed);

    //type 0=trash ,1=scoop, 2=bonus scoop
    //type determines point val too
    int getType() const;
    int getPointVal()const;
    bool getIsAlive()const;
    void setIsAlive(bool _state);
    ngl::Vec3 getPosition() const;
    //might delete getSpeed
    float getSpeed() const;
    //drawing the block
    void draw(const std::string &_shader );
    //further corner for block calc for GJK
    ngl::Vec3 support(const ngl::Vec3 &_direction, bool _isBlock, bool _isFarthest) const;
    //functions for calcs in GJK
    float dot(const ngl::Vec3 &_a, const ngl::Vec3 &_b);
    float norm(const ngl::Vec3 &_a);
    ngl::Vec3 normalize(const ngl::Vec3 &_a);
    //helps collision w cone
    bool circleIntersectRay( float _radius, const ngl::Vec3 &_rayDirection);
    //GJK collision algorithm based for cone vs scoops
    bool isCollidingGJK(const ngl::Vec3 &_coneCenter, float _coneRadius, const ngl::Vec3 &_support,const ngl::Vec3& _negSupport);
    //indicates of the collision on cone is true or not
    bool isCaught(const ngl::Vec3 &_conePosition) ;
    // Update block position based on time, position also bound by playable area x and z
    //time in
    void update(float _deltaTime);

private:
    int m_type;
    bool m_isAlive;
    int m_pointVal;
    ngl::Vec3 m_position;
    float m_initialSpeed;
};

#endif // BLOCK_H
