#include <QMouseEvent>
#include <QGuiApplication>
#include <QPushButton>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Util.h>
#include<ngl/ShaderLib.h>
#include<ngl/Transformation.h>

#include <iostream>
#include <random>

using namespace std;

NGLScene::NGLScene()
{
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    setTitle("NGL Scoops");
    //for updating the cone
    m_updateConeTimer= startTimer(02);
    //drawing the scene
    m_drawTimer=startTimer(16);
    //generating new falling scoops
    m_scoopTimer=startTimer(16);
    //trying to improve movement
    m_isKeyPressed = false;
    m_moveVec = ngl::Vec3::zero();
    coneIsContinualMove = true;
    levelBoundary = 4.7f;
}

NGLScene::~NGLScene()
{
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w , int _h)
{
    m_win.width  = static_cast<int>( _w * devicePixelRatio() );
    m_win.height = static_cast<int>( _h * devicePixelRatio() );
    m_project = ngl::perspective(45.0f, float(_w)/float(_h),0.1f,200);
}
void NGLScene::createShaderProgram(const std::string& shaderName, float r,float g,float b,ngl::Vec3 from)
{
    //Creating a different shader program to assign when drawing objects
    //ID is the string you assign, rgb values as floats,and the from needed for the cam position
    constexpr auto vertexShader = "PBRVertex";
    constexpr auto fragShader = "PBRFragment";

    ngl::ShaderLib::createShaderProgram(shaderName);
    ngl::ShaderLib::attachShader(vertexShader, ngl::ShaderType::VERTEX);
    ngl::ShaderLib::attachShader(fragShader, ngl::ShaderType::FRAGMENT);
    ngl::ShaderLib::loadShaderSource(vertexShader, "shaders/PBRVertex.glsl");
    ngl::ShaderLib::loadShaderSource(fragShader, "shaders/PBRFragment.glsl");
    ngl::ShaderLib::compileShader(vertexShader);
    ngl::ShaderLib::compileShader(fragShader);
    ngl::ShaderLib::attachShaderToProgram(shaderName, vertexShader);
    ngl::ShaderLib::attachShaderToProgram(shaderName, fragShader);
    ngl::ShaderLib::linkProgramObject(shaderName);
    ngl::ShaderLib::use(shaderName);
    ngl::ShaderLib::setUniform("camPos", from);
    m_lightPos.set(0.0f, 0.0f, -5.0f, 1.0f);
    ngl::ShaderLib::setUniform("lightPosition", m_lightPos.toVec3());
    ngl::ShaderLib::setUniform("lightColor", 400.0f, 240.0f, 160.0f);
    ngl::ShaderLib::setUniform("exposure", 3.0f);
    ngl::ShaderLib::setUniform("albedo", r, g,b); // Red color
    ngl::ShaderLib::setUniform("metallic", 0.0f);
    ngl::ShaderLib::setUniform("roughness", 1.0f);
    ngl::ShaderLib::setUniform("ao", 0.4f);
}
void NGLScene::initializeGL()
{
    // we must call that first before any other GL commands to load and link the
    // gl commands from the lib, if that is not done program will crash
    ngl::NGLInit::initialize();
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    m_lightAngle = 20.0;
    m_lightPos.set(sinf(m_lightAngle), 2, cosf(m_lightAngle));

    // We now create our view matrix for a static camera
    ngl::Vec3 from(0.0f, 4.0f, 10.0f);
    ngl::Vec3 to(0.0f, 0.0f, 0.0f);
    ngl::Vec3 up(0.0f, 1.0f, 0.0f);
    // now load to our new camera
    m_view = ngl::lookAt(from, to, up);
    m_project = ngl::perspective(45.0f, 1024.0f/720.0f, 0.05f, 350.0f);
    //creating colors to use later
    createShaderProgram("PBR",0.5f,0.5f,0.5f,from);
    //create a sphere, cone and plane to create officially later
    ngl::VAOPrimitives::createSphere("sphere", -0.5f, 50);//lands ontop of cone
    ngl::VAOPrimitives::createTrianglePlane("plane", 10, 10, 1,1, ngl::Vec3(0.353, 0.42, 0.612));//baseplate for gameplay
    ngl::VAOPrimitives::createCone("cone", 0.5, 1.4f, 20, 20);//cone
    //creating the player controlled cone
    m_cone=std::make_unique<Cone>(3,10,0.2,ngl::Vec3(0.0f,1.5f,0.0f));
    m_starterScoop=std::make_unique<Block>(3,true,ngl::Vec3(0.0f,2.0f,0.0f),0.0);
    //y=5-auto screen space, 10 more optimal if want to see board too, max tried 12
    ////m_testScoop=std::make_unique<Block>(0,true,ngl::Vec3(3.0f,1.0f,0.0f),0.0);
}
void NGLScene::loadMatricesToShader(const std::string &_shader)
{
    ngl::ShaderLib::use(_shader);
    struct transform
    {
        ngl::Mat4 MVP;
        ngl::Mat4 normalMatrix;
        ngl::Mat4 M;
    };

    transform t;
    //updating via mouse rotations to get our view of the scene
    t.M = m_view * m_mouseGlobalTX * m_transform.getMatrix();
    t.MVP = m_project * t.M;
    t.normalMatrix = t.M;
    t.normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniformBuffer("TransformUBO", sizeof(transform), &t.MVP.m_00);
    //ngl::ShaderLib::setUniform("lightPosition", (m_mouseGlobalTX * m_lightPos).toVec3());
}

void NGLScene::drawScene(const std::string &_shader) {
    //mouse rotation of scene
    ngl::ShaderLib::use(_shader);
    // Rotation based on the mouse position for our global transform
    ngl::Mat4 rotX = ngl::Mat4::rotateX(float(m_win.spinXFace));
    ngl::Mat4 rotY = ngl::Mat4::rotateY(float(m_win.spinYFace));
    // multiply the rotations
    m_mouseGlobalTX = rotY * rotX;
    // add the translations
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
    m_transform.reset();
    m_transform.setPosition(0.0f, 0.0f, 0.0f);
    ngl::ShaderLib::setUniform("albedo", 0.0f, 0.2f, 0.6f);//blue bg color
    loadMatricesToShader(_shader);
    //building the base game box
    // Draw plane
    ngl::VAOPrimitives::draw("plane");
    // Loop to draw cubes around the perimeter
    float m_cubeX = 0.0f;
    float m_cubeY = 0.0f;
    float m_cubeZ = 0.0f;
    for (int i = 0; i < 4; ++i) {
        // Adjust rotation and position for each side
        switch (i) {
            case 0://right side
                m_cubeX = 5.5f;
                m_cubeZ = 6.5f;
                m_transform.reset();
                for (int j = 0; j < 11; j++) {
                    m_cubeZ -= 1.0f;
                    m_transform.setPosition(m_cubeX, m_cubeY, m_cubeZ);
                    loadMatricesToShader(_shader);
                    ngl::VAOPrimitives::draw("cube");
                }
                break;
            case 1://bottom
                m_cubeX = 5.5f;
                m_cubeZ = 5.5f;
                m_transform.reset();
                for (int j = 0; j < 11; j++) {
                    m_cubeX -= 1.0f;
                    m_transform.setPosition(m_cubeX, m_cubeY, m_cubeZ);
                    loadMatricesToShader(_shader);
                    ngl::VAOPrimitives::draw("cube");
                }
                break;
            case 2: //left
                m_cubeX = -5.5f;
                m_cubeZ = 5.5f;
                m_transform.reset();
                for (int j = 0; j < 11; j++)
                {
                    m_cubeZ -= 1.0f;
                    m_transform.setPosition(m_cubeX, m_cubeY, m_cubeZ);
                    loadMatricesToShader(_shader);
                    ngl::VAOPrimitives::draw("cube");
                }
                break;
            case 3://top
                m_cubeX = 6.5f;
                m_cubeZ - 5.5f;
                m_transform.reset();
                for (int j = 0; j < 11; j++) {
                    m_cubeX -= 1.0f;
                    m_transform.setPosition(m_cubeX, m_cubeY, m_cubeZ);
                    loadMatricesToShader(_shader);
                    ngl::VAOPrimitives::draw("cube");
                }
                break;
        }
    }
    // Draw the cone with a scoop on top
    m_transform.reset();
    m_transform.setPosition(m_cone->getPosition());
    m_transform.setRotation(ngl::Vec3(90.0f, 0.0f, 0.0f));
    loadMatricesToShader(_shader);
    m_cone->draw(_shader, 0.83f, 0.43f, 0.07f);//sets cone to brown color
    m_starterScoop->draw(_shader);
    // Generate new scoops periodically (adjust interval as needed)
    //    //falling scoops

    // Loop through the list of scoops and draw each one
    for (const auto& scoop : m_scoops)
    {
        bool checkCollide = m_cone->checkCollision(scoop->getPosition(), scoop->getType(), scoop->getPointVal(), m_cone->getPosition());
        //if it collides, has to update the score and set alive as false
        if(checkCollide&& scoop->getIsAlive())
        {
            //if it collides AND is still alive, update the score based on the type of scoop
            switch (scoop->getType())
            {
                case 0:
                    // Trash
                    m_cone->updateScoreAndLives(scoop->getPointVal(), -1);
                    break;
                case 1:
                    // Scoop
                    m_cone->updateScoreAndLives(scoop->getPointVal(), 0);
                    break;
                case 2:
                    // Bonus scoop
                    m_cone->updateScoreAndLives(scoop->getPointVal(), 2);
                    break;
                default:
                    //not a playable block type, something has gone wrong
                    std::cerr << "ERROR - Invalid block type: " <<scoop->getPointVal() << std::endl;
                    break;
            }
            scoop->setIsAlive(false);
        }
        //draw the scoop only if it's alive
        if(scoop->getIsAlive())
        {
            m_transform.reset();
            m_transform.setPosition(scoop->getPosition());
            loadMatricesToShader(_shader);
            scoop->draw(_shader);
        }
    }
}
//shows the lives and points in the title
void NGLScene::updateWindowTitle()
{
    // Get the current cone lives and points
    int lives = m_cone->getLives();
    int points = m_cone->getPoints();
    // Format string
    std::string title = "︶꒦*꒷︶    NGL SCOOPS        Lives: " + std::to_string(lives) + "        Points: " + std::to_string(points)+"          ︶꒷*꒦꒷︶";
    // Set the window title
    setTitle(QString::fromStdString(title));
}

void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_win.width, m_win.height);
    //update the entire scene (base platform, cone)
    drawScene("PBR");
    updateWindowTitle();
}


//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
    // this method is called every time the main window receives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    ngl::Vec3 currentPosition = m_transform.getPosition();
    //float levelBoundary = 4.7f;
    // Define the movement speed
    switch (_event->key())
    {
        case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
            //space reset changed to 1
        case Qt::Key_1:
            m_win.spinXFace=0;
            m_win.spinYFace=0;
            m_modelPos.set(ngl::Vec3::zero());
            break;
        case Qt::Key_Space:
            //checking position of cone for debug purpose
            std::cout << "Cone position: "
                      << m_cone->getPosition().m_x << ", "
                      << m_cone->getPosition().m_y << ", "
                      << m_cone->getPosition().m_z << std::endl;
        default : break;
    }
    //determining movement of cone type is either continually moving on the screen or completely user controlled
    if (coneIsContinualMove)
    {
        float movementSpeed = m_cone->getSpeed() *5 ;
        switch (_event->key())
        {
            case Qt::Key_W:
            case Qt::Key_S:
            case Qt::Key_A:
            case Qt::Key_D:
                //more distance covered in qezx, but harder for user
            case Qt::Key_Q:
            case Qt::Key_E:
            case Qt::Key_Z:
            case Qt::Key_X:
                // Set movement vector based on pressed key
                m_isKeyPressed = true;
                m_moveVec = ngl::Vec3::zero();
                if (_event->key() == Qt::Key_W || _event->key() == Qt::Key_Q || _event->key() == Qt::Key_E) {
                    m_moveVec.m_z = -movementSpeed;
                } else if (_event->key() == Qt::Key_S || _event->key() == Qt::Key_Z || _event->key() == Qt::Key_X) {
                    m_moveVec.m_z = movementSpeed;
                }
                if (_event->key() == Qt::Key_A || _event->key() == Qt::Key_Q || _event->key() == Qt::Key_Z) {
                    m_moveVec.m_x = -movementSpeed;
                } else if (_event->key() == Qt::Key_D || _event->key() == Qt::Key_E || _event->key() == Qt::Key_X) {
                    m_moveVec.m_x = movementSpeed;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        //original controls
        switch(_event->key())
        {
            //WSAD control of cone, one dir at a time
            case Qt::Key_W:
                //up
                m_cone->move(0.0f,0.0f,-(m_cone->getSpeed()),levelBoundary);
                break;
            case Qt::Key_S:
                //down
                m_cone->move(0.0f,0.0f,m_cone->getSpeed(),levelBoundary);
                break;
            case Qt::Key_A:
                //left
                m_cone->move(-(m_cone->getSpeed()),0.0f,0.0f,levelBoundary);
                break;
            case Qt::Key_D:
                //right
                m_cone->move(m_cone->getSpeed(),0.0f,0.0f,levelBoundary);
                break;
                //Diagonal movement QEZX(first overcome less of a pause)
            case Qt::Key_Q:
                //up + left
                m_cone->move(-(m_cone->getSpeed()),0.0f,-(m_cone->getSpeed()),levelBoundary);
                break;
            case Qt::Key_E:
                //up + right
                m_cone->move(m_cone->getSpeed(),0.0f,-(m_cone->getSpeed()),levelBoundary);
                break;
            case Qt::Key_Z:
                //down + left
                m_cone->move(-(m_cone->getSpeed()),0.0f,m_cone->getSpeed(),levelBoundary);
                break;
            case Qt::Key_X:
                //down + right
                m_cone->move(m_cone->getSpeed(),0.0f,m_cone->getSpeed(),levelBoundary);
                break;
            default:
                break;
        }
    }
    //update and redraw
    update();
}

void NGLScene::generateRandomScoop()
{
    //minimum and maximum positions for the scoop(boundaries of gameplay box approximately
    float minX = -5.0f;
    float maxX = 5.0f;
    float minZ = -5.0f;
    float maxZ = 5.0f;
    random_device rd;
    mt19937 gen(rd());
    // random x and z positions within the defined boundaries
    uniform_real_distribution<float> disX(minX, maxX);
    uniform_real_distribution<float> disZ(minZ, maxZ);
    float randomX = disX(gen);
    float randomZ = disZ(gen);
    // Choose a random scoop type reminder-(0: Trash, 1: Scoop, 2: Bonus)
    int scoopType = rand() % 3;
    // Create a new scoop obj -w- generated position + type at height 14
    std::unique_ptr<Block> newScoop = std::make_unique<Block>(scoopType, true, ngl::Vec3(randomX, 14.0f, randomZ), 0.0);
    m_scoops.push_back(std::move(newScoop));
}

float m_elapsedTime =0.0f;
void NGLScene::timerEvent(QTimerEvent *_event)
{
    //timers trigger events such as movement, scoop generation,and updating draw
    m_elapsedTime +=0.16f;

    if (_event->timerId() == m_scoopTimer && m_elapsedTime >= 200.0f)
    {
        generateRandomScoop();
        // Reset elapsed time after generating a scoop
        m_elapsedTime = 0.0f;
    }
    if (_event->timerId() == m_updateConeTimer)
    {
        //user choice if they want the scoop to continually move or to be more direct control
        if (coneIsContinualMove)
        {
            if (m_isKeyPressed)
            {
                // Update cone position based on movement vector and elapsed time
                float elapsedTime = 0.016f; // Assuming timer fires every 16 milliseconds
                m_cone->move(m_moveVec.m_x * elapsedTime, 0.0f, m_moveVec.m_z * elapsedTime, levelBoundary);
            }
        }
        //blocks move during update
        for (auto& block : m_scoops)
        {
            //blocks update based on change in time, small amount of speed up
            float deltaY=0.2f;
            block->update(deltaY);
            //checks if it was caught before hitting ground
            if(block->getPosition().m_y<=0.0f&&block->getIsAlive())
            {
                //block is lower or has hit 0 and is still alive, delete
                int blockType = block->getType();
                int points = block->getPointVal();
                switch (blockType)
                {
                    case 0:
                        // Trash
                        m_cone->updateScoreAndLives(-points,0);
                        break;
                    case 1:
                        // Scoop
                        m_cone->updateScoreAndLives(-points, -1);
                        break;
                    case 2:
                        // Bonus scoop
                        m_cone->updateScoreAndLives(-points, -1);
                        break;
                    default:
                        std::cerr << "ERROR - Invalid block type: " << points << std::endl;
                        break;
                }
                block->setIsAlive(false);
            }
        }

    }
    if (_event->timerId() == m_drawTimer)
    {
        update();
    }
}