#ifndef _Yolov5HttpImp_H_
#define _Yolov5HttpImp_H_

#include "servant/Application.h"
#include "json/json.h"
#include "inference.h"
#include "ConvertImage.h"
/**
 *
 *
 */
using namespace inference;

class Yolov5HttpImp : public Servant
{
public:
    /**
     *
     */
    virtual ~Yolov5HttpImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    int doRequest(TarsCurrentPtr current, vector<char> &buffer);

private:

    Inference* infer = nullptr;
    ImagemConverter *img_cvtr = nullptr;

    int doRequest(const TC_HttpRequest &req, TC_HttpResponse &rsp);
    void parseContent(std::string& content, Json::Value& root);

};
/////////////////////////////////////////////////////
#endif
