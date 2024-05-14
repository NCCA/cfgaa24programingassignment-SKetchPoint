/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include <QtGui/QGuiApplication>
#include <iostream>
#include "NGLScene.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
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
  QApplication app2 (argc, argv);
  NGLScene scene;
  QWidget window2;
  window2.resize(120, 700);
  window2.move(1100,100);
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
  QObject::connect(pauseBtn, &QPushButton::clicked, &scene, &NGLScene::pauseButtonClicked);
  QObject::connect(controllsBtn, &QPushButton::clicked, &scene, &NGLScene::controllsButtonClicked);
  QObject::connect(asciBtn, &QPushButton::clicked, &scene, &NGLScene::asciButtonClicked);
  QObject::connect(resetBtn, &QPushButton::clicked, &scene, &NGLScene::resetButtonClicked);
    //show the two windows and keep them open
  window.show();
  window2.show();
  //loop till nothing left to execute
  return app.exec() && app2.exec();
}



