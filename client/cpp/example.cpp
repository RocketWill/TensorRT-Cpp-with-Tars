#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <opencv2/opencv.hpp>

#include "ConvertImage.h"

static const std::string URL = "http://127.0.0.1:30002";


int main(int argc, char** argv)
{
    ImagemConverter *img_cvtr = new ImagemConverter();
    cv::Mat image = cv::imread("/mnt/f/cy/workspace/Luokung/tars/ws/client/test-image.jpeg");
    std::string base64str = img_cvtr->mat2str(image);
    std::string body = std::string("{\"data\":\"") + base64str + std::string("\"}");
    cpr::Response resp = cpr::Post(cpr::Url{URL},
                         cpr::Body{body});

    std::cout << resp.text << std::endl;
    
    return 0;
}