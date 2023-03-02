#include "Yolov5HttpImp.h"
#include "Yolov5HttpServer.h"
#include "servant/Application.h"
#include "inference.h"
#include "utils.h"

using namespace std;
using namespace inference;

//////////////////////////////////////////////////////
void Yolov5HttpImp::initialize()
{
    //initialize servant here:
    //...
    std::string cfg = "/workspace/Yolov5HttpServer/model/config.json";
    infer = new Inference(parse_json_file(cfg));
    img_cvtr = new ImagemConverter();
    
}

//////////////////////////////////////////////////////
void Yolov5HttpImp::destroy()
{
    //destroy servant here:
    //...
}

int Yolov5HttpImp::doRequest(TarsCurrentPtr current, vector<char> &buffer)
{
    TC_HttpRequest req;
    TC_HttpResponse rsp;
    std::vector<char> v = current->getRequestBuffer();
    string sBuf;
    sBuf.assign(v.data(), v.size());
    req.decode(sBuf);
    int ret = doRequest(req, rsp);
    rsp.encode(buffer);

    return ret;
}

void Yolov5HttpImp::parseContent(std::string& content, Json::Value& root)
{
    JSONCPP_STRING err;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    reader->parse(content.c_str(), content.c_str() + content.size(), &root, &err);
}

int Yolov5HttpImp::doRequest(const TC_HttpRequest &req, TC_HttpResponse &rsp)
{
    string content = req.getContent();
    string r = "[]";

    try {
        Json::Value req_content;
        parseContent(content, req_content);
        string data = req_content["data"].asString();
        vector<cv::Mat> batch_image = {this->img_cvtr->str2mat(data)}; // only one image
        auto dets = infer->do_inference(batch_image);
        Json::Value json_res = infer->to_dict(dets[0]); // cause batch size is one

        r = jsonify(json_res);
    }
    catch(...) {

    }

    // string msg = "success";
    rsp.setContentType("application/json");
    rsp.setResponse(r.c_str(), r.size());

    return 0;   
}