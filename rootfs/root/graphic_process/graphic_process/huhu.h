#ifndef HUHU_H
#define HUHU_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "usbserial.h"
#include <QDebug>


bool open_serial(std::string serial,int speed);
bool display_model(cv::VideoCapture& v_capture, usbserial &u_serial);


#endif // HUHU_H
