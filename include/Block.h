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
    //setting type for initial scoop to change to what you most recently hit
    void setType(int _newType);
    //getting point value of the scoop to update to cone
    int getPointVal()const;
    //returns if the block is alive
    bool getIsAlive()const;
    //sets the alive state of the block
    void setIsAlive(bool _state);
    //returns the position of the block
    ngl::Vec3 getPosition()const;
    //sets the position of the block
    void setPosition(ngl::Vec3 _newPosition);
    //returns the value of the speed
    float getSpeed() const;
    //sets speed to the new desired speed
    void setSpeed(float _newSpeed);
    //drawing the block
    void draw(const std::string &_shader);
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
