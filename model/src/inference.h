#ifndef _INFERENCE_H
#define _INFERENCE_H

#include <string>
#include <vector>

#include <NvInfer.h>
#include <opencv2/opencv.hpp>
#include "json/json.h"

#include "logging.h"
#include "types.h"

namespace inference
{

using std::string;
using std::vector;
using namespace nvinfer1;

class Inference
{
public:
    Logger gLogger;
    Json::Value cfg;
    unsigned int max_batchsize;
    string wts_file;
    string engine_file;
    string kInputTensorName = "data";
    string kOutputTensorName = "prob";
    int kNumClass = 80, kBatchSize = 1;
    int kInputH = 640, kInputW = 640;
    int kMaxNumOutputBbox = 1000;
    int kNumAnchor = 3;
    float kIgnoreThresh = 0.1f;
    float kNmsThresh = 0.45f;
    float kConfThresh = 0.5f;
    int kGpuId = 0;
    int kMaxInputImageSize = 4096 * 3112;
    float gd = 0.33f, gw = 0.50f;
    bool is_p6 = false;
    int kOutputSize;
    cudaStream_t stream;
    
    Inference(Json::Value cfg);
    ~Inference();
    std::vector<std::vector<Detection>> do_inference(std::vector<cv::Mat> img_batch);
    void serialize_engine(unsigned int max_batchsize, bool& is_p6, float& gd, float& gw, std::string& wts_name, std::string& engine_name);
    void deserialize_engine(std::string& engine_name, IRuntime** runtime, ICudaEngine** engine, IExecutionContext** context);
    Json::Value to_dict(std::vector<Detection> detections);
    void prepare_buffers(ICudaEngine* engine, float** gpu_input_buffer, float** gpu_output_buffer, float** cpu_output_buffer);

// private:
    IRuntime* runtime = nullptr;
    ICudaEngine* engine = nullptr;
    IExecutionContext* context = nullptr;
    float* gpu_buffers[2];
    float* cpu_output_buffer = nullptr;
    
};

}


#endif // _INFERENCE_H