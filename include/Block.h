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
    ngl::Vec3 getPosition() const;
    //might delete getSpeed
    float getSpeed() const;

    // Update block position based on time, position also bound by playable area x and z
    //time in
    void update(float _deltaTime);
    //drawing the block
    void draw(const std::string &_shader );
    //further corner for block calc for GJK
    ngl::Vec3 support(const ngl::Vec3 &direction, bool isBlock, bool isFarthest) const;
    //functions for calcs in GJK
    static float dot(const ngl::Vec3 &a, const ngl::Vec3 &b);
    static float norm(const ngl::Vec3 &a);
    static ngl::Vec3 normalize(const ngl::Vec3 &a);
    static bool isCollidingGJK(const ngl::Vec3 &coneCenter, float coneRadius, const ngl::Vec3 &support,const ngl::Vec3& negSupport);
    bool isCaught(const ngl::Vec3 &_conePosition) const ;

private:
    int m_type;
    bool m_isAlive;
    int m_pointVal;
    ngl::Vec3 m_position;
    float m_initialSpeed;
};

#endif // BLOCK_H
