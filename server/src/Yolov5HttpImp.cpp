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
    Json::Value rsp_msg;
    rsp_msg["code"] = 200;
    rsp_msg["msg"] = "ok";
    rsp_msg["show_msg"] = "完成";

    string content = req.getContent();
    Json::Value det_res = {};

    try {
        Json::Value req_content;
        parseContent(content, req_content);
        string data = req_content["data"].asString();
        vector<cv::Mat> batch_image = {this->img_cvtr->str2mat(data)}; // only one image
        auto dets = infer->do_inference(batch_image);
        det_res = infer->to_dict(dets[0]); // cause batch size is one
        rsp.setStatus(200);
    }
    catch(const std::exception& ex) {
        TLOGERROR(__FILE__ << "|" << __LINE__ << "|" << ex.what() << endl);
        rsp.setStatus(500);
        rsp_msg["code"] = 500;
        rsp_msg["msg"] = "error";
        rsp_msg["show_msg"] = "错误";
    }
    catch(...) {
        TLOGERROR(__FILE__ << "|" << __LINE__ << "|" << "Unknown failure occurred. Possible memory corruption" << endl);
        rsp.setStatus(500);
        rsp_msg["code"] = 500;
        rsp_msg["msg"] = "error";
        rsp_msg["show_msg"] = "错误";
    }

    rsp_msg["result"] = det_res;
    string rsp_msg_str = jsonify(rsp_msg);
    rsp.setContentType("application/json");
    rsp.setResponse(rsp_msg_str.c_str(), rsp_msg_str.size());

    return 0;   
}