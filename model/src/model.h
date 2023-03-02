#pragma once

#include <json/json.h>

#include <NvInfer.h>
#include <string>

nvinfer1::ICudaEngine* build_det_engine(Json::Value &cfg, unsigned int maxBatchSize, nvinfer1::IBuilder* builder,
                                        nvinfer1::IBuilderConfig* config, nvinfer1::DataType dt,
                                        float& gd, float& gw, std::string& wts_name);

nvinfer1::ICudaEngine* build_det_p6_engine(Json::Value &cfg, unsigned int maxBatchSize, nvinfer1::IBuilder* builder,
                                           nvinfer1::IBuilderConfig* config, nvinfer1::DataType dt,
                                           float& gd, float& gw, std::string& wts_name);

nvinfer1::ICudaEngine* build_cls_engine(Json::Value &cfg, unsigned int maxBatchSize, nvinfer1::IBuilder* builder, nvinfer1::IBuilderConfig* config, nvinfer1::DataType dt, float& gd, float& gw, std::string& wts_name);

nvinfer1::ICudaEngine* build_seg_engine(Json::Value &cfg, unsigned int maxBatchSize, nvinfer1::IBuilder* builder, nvinfer1::IBuilderConfig* config, nvinfer1::DataType dt, float& gd, float& gw, std::string& wts_name);
