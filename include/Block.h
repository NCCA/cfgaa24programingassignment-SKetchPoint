#ifndef BLOCK_H
#define BLOCK_H

#include <ngl/Vec3.h>
#include <ngl/VAOPrimitives.h>
class Block
{
public:
    Block(int _type, bool _isAlive, ngl::Vec3 _position, float _initialSpeed);

    //type 0=trash ,1=scoop, 2=bonus scoop
    //type determines point val too
    int getType() const;
    int getPointVal()const;
    bool getIsAlive()const;
    void setIsAlive(bool _state);
    ngl::Vec3 getPosition()const;
    //might delete getSpeed
    float getSpeed() const;
    //drawing the block
    void draw(const std::string &_shader);
    //making a distance formula where sqrt((x2-x1)^2+(y2-y1)^2) but in 3D
    static float distanceBetweenVec(ngl::Vec3 &_point1,ngl::Vec3 &_point2);
    //indicates of the collision on cone is true or not
    bool isCaught(ngl::Vec3 _conePosition);
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
