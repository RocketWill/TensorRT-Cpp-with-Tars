# C++ 客户端请求示例
### 1. 安装依赖 & 编译
- 测试时使用的 conan 版本为 1.59
- 需要的依赖为 `opencv` 和 `cpr`，参考 [conanfile.txt](conanfile.txt)
```bash
conan install . --install-folder cmake-build-debug --build=missing # 会创建cmake-build-debug文件夹

mkdir build && cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-build-debug/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug

make
```

### 2. 运行
```bash
# in build folder
./request-example
```
- 说明：
    - 发送的图像需转为 `base64` 字符串
    - 请求参数为 `{"data": IMAGE_STRING}`

### 3. 运行结果
结果包含 `bbox` (x, y, w, h)，`cls_id` (COCO数据集类别id)，`conf` (置信度)  
```json
[
    {
        bbox: [
            169.55561828613281,
            305.57791137695312,
            104.35741424560547,
            196.04292297363281
        ],
        cls_id: 0.0,
        conf: 0.78032165765762329
    },
    {
        bbox: [
            170.7706298828125,
            383.7095947265625,
            49.480766296386719,
            87.9088134765625
        ],
        cls_id: 1.0,
        conf: 0.78273206949234009
    },
    ...
]
```