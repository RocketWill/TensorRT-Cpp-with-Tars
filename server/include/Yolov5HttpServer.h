#ifndef _Yolov5HttpServer_H_
#define _Yolov5HttpServer_H_

#include <iostream>
#include "servant/Application.h"
#include "inference.h"
#include "ConvertImage.h"

using namespace tars;

/**
 *
 **/
class Yolov5HttpServer : public Application
{
public:
    /**
     *
     **/
    virtual ~Yolov5HttpServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();

};

extern Yolov5HttpServer g_app;

////////////////////////////////////////////
#endif
