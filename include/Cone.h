#ifndef CONE_H
#define CONE_H

#include <vector>
#include "ngl/Vec3.h"
#include  <ngl/AbstractVAO.h>
#include <memory>
#include <string_view>

#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Cone
{
public:
    Cone(int _lives=3,int _points=10, float _speed=0.1f, ngl::Vec3 _position={0.0f,1.0f,0.0f}, ngl::Vec3 _colour={1,1,0});

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


    // Collision -w- Blocks checked
    void updateScoreAndLives(int _scoreChange, int _livesChange);
    void move(float _x,float _y, float _z);
    bool checkCollision(const std::vector<ngl::Vec3> &_blockPositions);
private:
    ngl::Vec3 m_pos;
    int m_lives; 
    int m_points; 
    float m_speed;
    ngl::Vec3 m_colour;
    ngl::Transformation m_transform;
    void draw(const std::string &_shader, const ngl::Mat4 &_view, const ngl::Mat4 &_project);
};

#endif

