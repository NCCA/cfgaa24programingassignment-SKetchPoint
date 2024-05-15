#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include "WindowParams.h"
#include "Cone.h"
#include "Block.h"
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
#include<memory>
#include<ngl/Mat4.h>
#include <ngl/Transformation.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void createShaderProgram(const std::string& shaderName, float r,float g,float b,ngl::Vec3 from);
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;
    ///Allowing the title to update as lives and points are added/deducted
    void updateWindowTitle();
    /// generation of scoops on  a 2D plane in random spots at a constant time
    void generateRandomScoop();
    ///getting the value of m_coneIsContinualMove for buttons
    bool isConeIsContinual();
    ///reversing the setting of continual/ direct cone movement
    void reverseConeIsContinualMove();
    ///resetting the game for the reset button and if lives reach 0
    void resetGame();
    ///Pausing game either through the keyboard or through menu bar with pause button
    void pauseGame();

public slots:
    void pauseButtonClicked();
    void controllsButtonClicked();
    void asciButtonClicked();
    void resetButtonClicked();
private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform matrices to the shader
    //----------------------------------------------------------------------------------------------------------------------
    //void loadMatricesToShader();
    void loadMatricesToShader(const std::string &_shader);
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent(QTimerEvent*_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw our scene passing in the shader to use
    /// @param[in] _shader the name of the shader to use when drawing
    //----------------------------------------------------------------------------------------------------------------------
    void drawScene(const std::string &_shader);
    /// @brief windows parameters for mouse control etc.
    WinParams m_win;
    /// position for our model
    ngl::Vec3 m_modelPos;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    ///mouse transformation global
    ngl::Mat4 m_mouseGlobalTX;
    /// @brief transformation stack for the gl transformations etc
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation m_transform;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 m_lightPos;
    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    /// the angle of the light updated when the timer triggers to make it rotate
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Real m_lightAngle{};
    ///updating scene in timer event, the game loop that is calculated each time
    //movement of cone key event
    bool m_isKeyPressed;
    // if continual movement, creates a vec to have linear interpolation
    ngl::Vec3 m_moveVec;
    //boundary of the level determined, wont go past absolute val of level boundary
    float levelBoundary;
    //helper for the event timer for indicating updates
    float m_elapsedTime;
    //switch of either cone is always moving or strictly user controlled
    bool m_coneIsContinualMove;
    //ability to pause scene
    bool m_isPaused;
    //timers
    int m_updateConeTimer;
    int m_drawTimer;
    int m_scoopTimer;
    //player cone to control
    std::unique_ptr<Cone> m_cone;
    //scoop that sits ontop of the player controlls
    std::unique_ptr<Block>m_starterScoop;
    //tester scoop for collision
    //std::unique_ptr<Block>m_testScoop;
    std::list<std::unique_ptr<Block>> m_scoops;


};

#endif

