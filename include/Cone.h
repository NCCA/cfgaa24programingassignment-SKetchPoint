#ifndef CONE_H
#define CONE_H

#include <vector>
#include "ngl/Vec3.h"
#include  <ngl/AbstractVAO.h>
#include <memory>
#include <string_view>

#include <ngl/Obj.h>

class Cone
{
public:
    Cone(int _lives=3,int _points=10, float _speed=0.1f, ngl::Vec3 _position={0,0,0}, ngl::Vec3 _colour={1,1,0});

    ngl::Vec3 getPosition() const;
    void setPosition( ngl::Vec3 _pos);

    //player lives, if hits 0 then ends game
    int getLives() const;
    void setLives(int _lives);

    int getPoints() const;
    //allows for points of player to be reduced or added based on type of block caught
    //if check collision
    //getPoints+block.getPointVal
    //else
    void setPoints(int _points);

    // Movement input methods, do in main game loop?
//    void moveForward(float _speed);
//    void moveBackward(float _speed);
//    void moveLeft(float _speed);
//    void moveRight(float _speed);

    // Collision -w- Blocks checked
    void updateScoreAndLives(int _scoreChange, int _livesChange);
    bool checkCollision(const std::vector<ngl::Vec3> &_blockPositions);
private:
    ngl::Vec3 m_position; 
    int m_lives; 
    int m_points; 
    float m_speed;
    ngl::Vec3 m_colour; 
};

#endif

