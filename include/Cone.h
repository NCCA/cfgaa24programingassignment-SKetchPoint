#ifndef CONE_H
#define CONE_H

#include <vector>
#include "ngl/Vec3.h"
#include  <ngl/AbstractVAO.h>
#include <memory>
#include <string_view>

class Cone
{
public:
    Cone();
    Cone(int _lives=3,int _points=100, float _speed=0.1f, ngl::Vec3 _position={0,1,0}, ngl::Vec3 _colour={1,1,0});

    ngl::Vec3 getPosition() const;
    void setPosition(const ngl::Vec3 &_pos);

    int getLives() const;
    void setLives(int _lives);
    int getPoints() const;
    void setPoints(int _points);
    // Movement input methods
    void moveForward(float _speed);
    void moveBackward(float _speed); 
    void moveLeft(float _speed);
    void moveRight(float _speed); 
    // Collision -w- Blocks checked
    bool checkCollision(const std::vector<ngl::Vec3> &_blockPositions);
    //either score will update or lives will update
    void updateScoreAndLives(int _scoreChange, int _livesChange);
    void setColor(float _r, float _g, float _b);
private:
    ngl::Vec3 m_position; 
    int m_lives; 
    int m_points; 
    float m_speed;
    ngl::Vec3 m_colour; 
};

#endif

