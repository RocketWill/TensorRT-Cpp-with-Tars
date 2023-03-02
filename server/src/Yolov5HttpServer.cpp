#include "Yolov5HttpServer.h"
#include "Yolov5HttpImp.h"
#include "utils.h"

using namespace inference;

/////////////////////////////////////////////////////////////////
void
Yolov5HttpServer::initialize()
{
    //initialize application here:
    addServant<Yolov5HttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".Yolov5HttpObj");
    addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".Yolov5HttpObj",&TC_NetWorkBuffer::parseHttp);
}
/////////////////////////////////////////////////////////////////
void
Yolov5HttpServer::destroyApp()
{
    //destroy application here:
    //...
}

