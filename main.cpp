#include "Yolov5HttpServer.h"

Yolov5HttpServer g_app;

int main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        std::cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown exception." << std::endl;
    }
    return -1;
}
