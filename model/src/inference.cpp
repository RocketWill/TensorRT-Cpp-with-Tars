#include "inference.h"
#include "cuda_utils.h"
#include "preprocess.h"
#include "postprocess.h"
#include "model.h"
#include "utils.h"
#include "cuda_utils.h"

#include <iostream>
#include <fstream>

namespace inference
{
    using std::string;
    using std::cout;
    using std::endl;
    using namespace nvinfer1;

    Inference::Inference(Json::Value cfg)
    {
        cudaSetDevice(kGpuId);
        cudaStreamCreate(&stream);
        cuda_preprocess_init(kMaxInputImageSize);
        cfg = cfg;
        max_batchsize = cfg["max_batchsize"].asUInt();
        wts_file = cfg["wts_file"].asString();
        engine_file = cfg["engine_file"].asString();
        kInputTensorName = cfg["kInputTensorName"].asString();
        kOutputTensorName = cfg["kOutputTensorName"].asString();
        kNumClass = cfg["kNumClass"].asInt();
        kBatchSize = cfg["kBatchSize"].asInt64();
        kInputH = cfg["kInputH"].asInt();
        kInputW = cfg["kInputW"].asInt();
        kMaxNumOutputBbox = cfg["kMaxNumOutputBbox"].asInt();
        kNumAnchor = cfg["kNumAnchor"].asInt();
        kIgnoreThresh = cfg["kIgnoreThresh"].asFloat();
        kNmsThresh = cfg["kNmsThresh"].asFloat();
        kConfThresh = cfg["kConfThresh"].asFloat();
        kGpuId = cfg["kGpuId"].asInt();
        kMaxInputImageSize = cfg["kMaxInputImageSize"].asInt();
        gd = cfg["gd"].asFloat();
        gw = cfg["gw"].asFloat();
        is_p6 = cfg["is_p6"].asBool();
        kOutputSize = kMaxNumOutputBbox * sizeof(Detection) / sizeof(float) + 1;
        
        if (!file_exist(engine_file)) {
            assert(file_exist(wts_file));
            serialize_engine(max_batchsize, is_p6, gd, gw, wts_file, engine_file);
        }
        deserialize_engine(engine_file, &runtime, &engine, &context);
        prepare_buffers(engine, &(gpu_buffers[0]), &(gpu_buffers[1]), &cpu_output_buffer);
    }

    Inference::~Inference()
    {
        cudaStreamDestroy(stream);
        context->destroy();
        engine->destroy();
        runtime->destroy();
    }

    void Inference::serialize_engine(unsigned int max_batchsize, bool& is_p6, float& gd, float& gw, std::string& wts_file, std::string& engine_file)
    {
        IBuilder *builder = createInferBuilder(gLogger);
        IBuilderConfig *config = builder->createBuilderConfig();
        ICudaEngine *engine = nullptr;
        if (is_p6)
        {
            engine = build_det_p6_engine(cfg, max_batchsize, builder, config, DataType::kFLOAT, gd, gw, wts_file);
        }
        else
        {
            engine = build_det_engine(cfg, max_batchsize, builder, config, DataType::kFLOAT, gd, gw, wts_file);
        }
        assert(engine != nullptr);

        // Serialize the engine
        IHostMemory *serialized_engine = engine->serialize();
        assert(serialized_engine != nullptr);

        // Save engine to file
        std::ofstream p(engine_file, std::ios::binary);
        if (!p)
        {
            std::cerr << "Could not open plan output file" << std::endl;
            assert(false);
        }
        p.write(reinterpret_cast<const char *>(serialized_engine->data()), serialized_engine->size());

        // Close everything down
        engine->destroy();
        builder->destroy();
        config->destroy();
        serialized_engine->destroy();
    }

    void Inference::deserialize_engine(std::string& engine_file, IRuntime** runtime, ICudaEngine** engine, IExecutionContext** context)
    {
        std::ifstream file(engine_file, std::ios::binary);
        if (!file.good()) {
            std::cerr << "read " << engine_file << " error!" << std::endl;
            assert(false);
        }
        size_t size = 0;
        file.seekg(0, file.end);
        size = file.tellg();
        file.seekg(0, file.beg);
        char* serialized_engine = new char[size];
        assert(serialized_engine);
        file.read(serialized_engine, size);
        file.close();

        *runtime = createInferRuntime(gLogger);
        assert(*runtime);
        *engine = (*runtime)->deserializeCudaEngine(serialized_engine, size);
        assert(*engine);
        *context = (*engine)->createExecutionContext();
        assert(*context);
        delete[] serialized_engine;
        cout << "Deserialize engine completed." << endl;
    }

    Json::Value Inference::to_dict(std::vector<Detection> detections)
    {
        Json::Value root;
        int idx = 0;
        for (Detection& det: detections) {
            Json::Value data;
            Json::Value bbox;
            bbox[0] = det.bbox[0];
            bbox[1] = det.bbox[1];
            bbox[2] = det.bbox[2];
            bbox[3] = det.bbox[3];
            data["bbox"] = bbox;
            data["conf"] = det.conf;
            data["cls_id"] = det.class_id;
            root[idx++] = data;
        }
        return root;
    }

    void Inference::prepare_buffers(ICudaEngine* engine, float** gpu_input_buffer, float** gpu_output_buffer, float** cpu_output_buffer)
    {
        assert(engine->getNbBindings() == 2);
        // In order to bind the buffers, we need to know the names of the input and output tensors.
        // Note that indices are guaranteed to be less than IEngine::getNbBindings()
        const int inputIndex = engine->getBindingIndex(kInputTensorName.c_str());
        const int outputIndex = engine->getBindingIndex(kOutputTensorName.c_str());
        assert(inputIndex == 0);
        assert(outputIndex == 1);
        // Create GPU buffers on device
        CUDA_CHECK(cudaMalloc((void**)gpu_input_buffer, kBatchSize * 3 * kInputH * kInputW * sizeof(float)));
        CUDA_CHECK(cudaMalloc((void**)gpu_output_buffer, kBatchSize * kOutputSize * sizeof(float)));
        *cpu_output_buffer = new float[kBatchSize * kOutputSize];
        cout << "prepare buffers done" << endl;
    }

    std::vector<std::vector<Detection>> Inference::do_inference(std::vector<cv::Mat> img_batch)
    {
        cuda_batch_preprocess(img_batch, gpu_buffers[0], kInputW, kInputH, stream);
        context->enqueue(kBatchSize, (void**)gpu_buffers, stream, nullptr);
        CUDA_CHECK(cudaMemcpyAsync(cpu_output_buffer, ((void**)gpu_buffers)[1], kBatchSize * kOutputSize * sizeof(float), cudaMemcpyDeviceToHost, stream));
        cudaStreamSynchronize(stream);
        std::vector<std::vector<Detection>> res_batch;
        batch_nms(res_batch, cpu_output_buffer, img_batch.size(), kOutputSize, kConfThresh, kNmsThresh, kMaxNumOutputBbox);
        return res_batch;
    }
}