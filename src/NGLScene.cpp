#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>

#include <ngl/Util.h>
#include<ngl/ShaderLib.h>
#include<ngl/Transformation.h>

#include <iostream>


NGLScene::NGLScene()
{
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    setTitle("Blank NGL");
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
constexpr auto shaderProgram = "PBR";
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
    m_lightAngle = 0.0;
    m_lightPos.set(sinf(m_lightAngle), 2, cosf(m_lightAngle));

    // now to load the shader and set the values

    // we are creating a shader called PBR to save typos
    // in the code create some constexpr
    constexpr auto vertexShader = "PBRVertex";
    constexpr auto fragShader = "PBRFragment";
    // create the shader program
    ngl::ShaderLib::createShaderProgram(shaderProgram);
    // now we are going to create empty shaders for Frag and Vert
    ngl::ShaderLib::attachShader(vertexShader, ngl::ShaderType::VERTEX);
    ngl::ShaderLib::attachShader(fragShader, ngl::ShaderType::FRAGMENT);
    // attach the source
    ngl::ShaderLib::loadShaderSource(vertexShader, "shaders/PBRVertex.glsl");
    ngl::ShaderLib::loadShaderSource(fragShader, "shaders/PBRFragment.glsl");
    // compile the shaders
    ngl::ShaderLib::compileShader(vertexShader);
    ngl::ShaderLib::compileShader(fragShader);
    // add them to the program
    ngl::ShaderLib::attachShaderToProgram(shaderProgram, vertexShader);
    ngl::ShaderLib::attachShaderToProgram(shaderProgram, fragShader);
    // now we have associated that data we can link the shader
    ngl::ShaderLib::linkProgramObject(shaderProgram);
    // and make it active ready to load values
    ngl::ShaderLib::use(shaderProgram);


    // We now create our view matrix for a static camera
    ngl::Vec3 from(0.0f, 4.0f, 10.0f);
    ngl::Vec3 to(0.0f, 0.0f, 0.0f);
    ngl::Vec3 up(0.0f, 1.0f, 0.0f);
    // now load to our new camera
    m_view = ngl::lookAt(from, to, up);
    m_project = ngl::perspective(45.0f, 1024.0f/720.0f, 0.05f, 350.0f);

    ngl::ShaderLib::setUniform("camPos", from);
    // now a light
    m_lightPos.set(0.0, 2.0f, 2.0f, 1.0f);
    // set up the default shader material and light properties
    // these are "uniform" so will retain their values
    ngl::ShaderLib::setUniform("lightPosition", m_lightPos.toVec3());
    ngl::ShaderLib::setUniform("lightColor", 400.0f, 400.0f, 400.0f);
    ngl::ShaderLib::setUniform("exposure", 2.2f);
    //base color=albedo
    ngl::ShaderLib::setUniform("albedo", 01.0f, 0.5f, 1.0f);
    ngl::ShaderLib::setUniform("metallic", 0.1f);
    ngl::ShaderLib::setUniform("roughness", 0.8f);
    ngl::ShaderLib::setUniform("ao", 0.4f);
    //create a sphere, cone and plane
    ngl::VAOPrimitives::createSphere("sphere", -0.5f, 50);
    ngl::VAOPrimitives::createTrianglePlane("plane", 10, 10, 1,1, ngl::Vec3(0.353, 0.42, 0.612));
    ngl::VAOPrimitives::createCone("cone", 0.5, 1.4f, 20, 20);
    startTimer(10);
}
void NGLScene::loadMatricesToShader()
{
    ngl::ShaderLib::use("PBR");
    struct transform
    {
        ngl::Mat4 MVP;
        ngl::Mat4 normalMatrix;
        ngl::Mat4 M;
    };

    transform t;
    t.M = m_view * m_mouseGlobalTX * m_transform.getMatrix();

    t.MVP = m_project * t.M;
    t.normalMatrix = t.M;
    t.normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniformBuffer("TransformUBO", sizeof(transform), &t.MVP.m_00);

    ngl::ShaderLib::setUniform("lightPosition", (m_mouseGlobalTX * m_lightPos).toVec3());
}

void NGLScene::drawScene(const std::string &_shader)
{
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

    //initial placement of primitives
    m_transform.reset();
    {
        m_transform.setPosition(0.0f, 1.0f, 0.0f);
        loadMatricesToShader();
        ngl::VAOPrimitives::draw("sphere");
    } // and before a pop

    m_transform.reset();
    {
        m_transform.setPosition(0.0f, 0.0f, 5.0f);
        loadMatricesToShader();
        ngl::VAOPrimitives::draw("cube");
    } // and before a pop

    m_transform.reset();
    {
        m_transform.setPosition(0.0f, 1.0f, 0.0f);
        m_transform.setRotation(90.0f,0.0f,0.0f);
        loadMatricesToShader();
        ngl::VAOPrimitives::draw("cone");
    } // and before a pop

    m_transform.reset();
    {
        m_transform.setPosition(0.0f, 0.0f, 0.0f);
        loadMatricesToShader();
        ngl::VAOPrimitives::draw("plane");
    } // and before a pop
}

void NGLScene::paintGL()
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_win.width, m_win.height);
    drawScene("PBR");
}


//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
    // this method is called every time the main window receives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    ngl::Vec3 currentPosition = m_transform.getPosition();

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
        default : break;
    }
    //update and redraw
    update();
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
    update();
}