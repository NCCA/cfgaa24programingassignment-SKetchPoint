#ifndef BLOCK_H
#define BLOCK_H

#include <ngl/Vec3.h>

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

    //When touches ground plane or cone (checkCollision from Cone.h) ends life
    bool isCaught(const ngl::Vec3 &_conePosition) const;

private:
    bool m_type;
    bool m_isAlive;
    int m_pointVal;
    ngl::Vec3 m_position;
    float m_initialSpeed;
};

#endif // BLOCK_H
