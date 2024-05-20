/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <QSlider>
#include<QBoxLayout>
#include<QLabel>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // create an OpenGL format specifier
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);
#if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
#else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
#endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile(QSurfaceFormat::CoreProfile);
    // now set the depth buffer to 24 bits
    format.setDepthBufferSize(24);
    // now we are going to create our scene window
    NGLScene window;
    // and set the OpenGL format
    window.setFormat(format);

    // we can now query the version to see if it worked
    std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
    // set the window size
    window.resize(1024, 720);

    //2nd window for button(s)
    QWidget window2;
    window2.setWindowTitle("︶ Settings ︶꒦*꒷");
    window2.resize(200, 400);
    window2.move(1025,25);
    //creating vertical layout
    QVBoxLayout* layout = new QVBoxLayout(&window2);
    //button creation and geometry
    QPushButton *pauseBtn = new QPushButton("[ | | ]", &window2);
    QPushButton *resetBtn = new QPushButton("Reset", &window2);
    QPushButton *asciBtn = new QPushButton(" _--````--_\n"
                                           "|              |\n"
                                           " \\.--.---.-./\n"
                                           " (_.--._.-._)\n"
                                           "  \\=-=-=-/\n"
                                           "  \\=-=-/\n"
                                           "   \\=-/\n"
                                           "   \\/", &window2);
    QPushButton *controllsBtn = new QPushButton("Controlls", &window2);
    //buttons moved and sized
    pauseBtn->setGeometry(35, 10, 50, 30);
    controllsBtn->setGeometry(20, 50, 80, 30);
    asciBtn->setGeometry(10,480,100,150);
    resetBtn->setGeometry(20, 650, 80, 30);
    //connecting the buttons to actions
    QObject::connect(pauseBtn, &QPushButton::clicked, &window, &NGLScene::pauseButtonClicked);
    QObject::connect(controllsBtn, &QPushButton::clicked, &window, &NGLScene::controllsButtonClicked);
    QObject::connect(asciBtn, &QPushButton::clicked, &window, &NGLScene::asciButtonClicked);
    QObject::connect(resetBtn, &QPushButton::clicked, &window, &NGLScene::resetButtonClicked);

    QSlider* yOffsetSlider = new QSlider(Qt::Horizontal, &window2);
    yOffsetSlider->setRange(0, 5); // Adjust range as needed for your Y offset
    yOffsetSlider->setValue(0); // Set initial value

    QSlider* sceneSpeedSlider = new QSlider(Qt::Horizontal, &window2);
    yOffsetSlider->setRange(1, 100); // Adjust range as needed for your Y offset
    yOffsetSlider->setValue(1); // Set initial value

    //connect dial and slider to settings
    QObject::connect(yOffsetSlider, &QSlider::valueChanged, &window,&NGLScene::setYOffset);
    QObject::connect(sceneSpeedSlider, &QSlider::valueChanged, &window, &NGLScene::setSceneSpeed);
    //titles of labels
    QLabel* yOffsetLabel = new QLabel("Y Offset:");
    QLabel* sceneSpeedLabel = new QLabel("Scene Speed:");
    //slider layout
    layout->addWidget(pauseBtn, 0, Qt::AlignTop);
    layout->addWidget(controllsBtn, 0, Qt::AlignTop);

    layout->addWidget(yOffsetLabel, 0, Qt::AlignTop);
    layout->addWidget(yOffsetSlider, 0, Qt::AlignTop);

    layout->addWidget(sceneSpeedLabel, 0, Qt::AlignTop);
    layout->addWidget(sceneSpeedSlider, 0, Qt::AlignTop);

    layout->addWidget(asciBtn, 0, Qt::AlignBottom);

    layout->addWidget(resetBtn, 0, Qt::AlignBottom);

    //show the two windows and keep them open
    window2.show();
    window.show();
    //loop till nothing left to execute
    return app.exec() ;
}