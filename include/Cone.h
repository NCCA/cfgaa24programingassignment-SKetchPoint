#ifndef CONE_H
#define CONE_H

#include <vector>
#include "ngl/Vec3.h"
#include "Block.h"
#include  <ngl/AbstractVAO.h>
#include <memory>
#include <string_view>

#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Cone
{
public:
    Cone(int _lives=3,int _points=10, float _speed=0.1f, ngl::Vec3 _position={0.0f,1.0f,0.0f});

    ngl::Vec3 getPosition();
    void setPosition( ngl::Vec3 _pos);

    //player lives, if hits 0 then ends game
    int getLives() const;
    void setLives(int _lives);

    int getPoints() const;
    //allows for points of player to be reduced or added based on type of block caught
    //if check collision
    //getPoints+block.getPointVal
    void setPoints(int _points);
    //movement
    float getSpeed() const;
    void setSpeed(int _speed);
    void move(float _x,float _y, float _z, float _boundary);

    // Collision -w- Blocks checked
    void updateScoreAndLives(int _scoreChange, int _livesChange);
    //making a distance formula where sqrt((x2-x1)^2+(y2-y1)^2) but in 3D
    float distanceBetweenVec(ngl::Vec3 &_point1, ngl::Vec3 _point2);
    //checks if the block collides with the cone, if so then it determins based on block type what the geometry of the block
    bool checkCollision(ngl::Vec3 _blockPosition, int _blockType, ngl::Vec3 _playerPosition, float _playerYOffset);
    //draws out player cone
    void draw( const std::string &_shader,float _r,float _g,float _b);

private:
    ngl::Vec3 m_pos;
    int m_lives;
    int m_points;
    float m_speed;
    ngl::Vec3 m_colour;
    ngl::Transformation m_transform;

};

#endif