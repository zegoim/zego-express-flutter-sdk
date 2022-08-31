#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoExpressEngineEventHandler.h"
#include "ZegoTextureRendererController.h"

#include <variant>
#include <functional>
#include <flutter/encodable_value.h>
#include <flutter/plugin_registrar_windows.h>

#include <Windows.h>

std::pair<long, BYTE*> CreateFromHBITMAP(HBITMAP hBitmap)
{
    HDC hdc=GetDC(NULL);
    HDC memdc=CreateCompatibleDC(hdc);
    SelectObject(memdc,hBitmap);
    BITMAP bm;
    GetObject(hBitmap,sizeof(BITMAP),&bm);
    BITMAPINFO bi;
    bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth=bm.bmWidth;
    bi.bmiHeader.biHeight=bm.bmHeight;
    bi.bmiHeader.biPlanes=1;
    bi.bmiHeader.biBitCount=32;
    bi.bmiHeader.biCompression=BI_RGB;
    bi.bmiHeader.biSizeImage=0;
    bi.bmiHeader.biXPelsPerMeter=bm.bmWidth;
    bi.bmiHeader.biYPelsPerMeter=bm.bmHeight;
    bi.bmiHeader.biClrUsed=0;
    bi.bmiHeader.biClrImportant=0;
    int bitsize=bm.bmWidth*bm.bmHeight*4;
    BYTE* buff=new BYTE[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bitsize];
    BYTE* tmpbuff = buff;
    RtlZeroMemory(buff,sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bitsize);
    BITMAPFILEHEADER bf;
    bf.bfType=0x4d42;
    bf.bfSize=bitsize+14+sizeof(BITMAPINFOHEADER);
    bf.bfOffBits=sizeof(bi.bmiHeader)+sizeof(bf);
    bf.bfReserved1=0;
    bf.bfReserved2=0;
    RtlMoveMemory(tmpbuff,&bf,sizeof(BITMAPFILEHEADER));
    tmpbuff += sizeof(BITMAPFILEHEADER);
    RtlMoveMemory(tmpbuff,&bi,sizeof(BITMAPINFO));
    tmpbuff += sizeof(BITMAPINFO);
    GetDIBits(memdc,hBitmap,0,bm.bmHeight,tmpbuff,&bi,DIB_PAL_COLORS);  
    return std::pair(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bitsize, buff) ;
}

std::pair<long, BYTE*> makeBtimap(const std::vector<uint8_t> *frame, std::pair<int32_t, int32_t> size)
{
    auto bufferLen = frame->size();
    auto width = size.first;
    auto height = size.second;
    // rgba -> bgra  视频帧数据格式转换，windows只支持bgra
    std::vector<uint8_t> destBuffer_;
    {
        std::vector<uint8_t> srcBuffer_(*frame);
        destBuffer_.resize(bufferLen);
        FlutterDesktopPixel* src =
            reinterpret_cast<FlutterDesktopPixel*>(srcBuffer_.data());
        VideoFormatBGRAPixel* dst =
            reinterpret_cast<VideoFormatBGRAPixel*>(destBuffer_.data());
        
        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                uint32_t sp = (y * width) + x;
                {
                    dst[sp].r = src[sp].r;
                    dst[sp].g = src[sp].g;
                    dst[sp].b = src[sp].b;
                    dst[sp].a = 255;
                }
            }
          }
    }
    BITMAPINFO bi;
    bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth=width;
    bi.bmiHeader.biHeight=height;
    bi.bmiHeader.biPlanes=1;
    bi.bmiHeader.biBitCount=32;
    bi.bmiHeader.biCompression=BI_RGB;
    bi.bmiHeader.biSizeImage=0;
  
    bi.bmiHeader.biXPelsPerMeter=width;
    bi.bmiHeader.biYPelsPerMeter=height;
    bi.bmiHeader.biClrUsed=0;
    bi.bmiHeader.biClrImportant=0;
    // int bitsize=height*height*3;
    BYTE* buff=new BYTE[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bufferLen];
    BYTE* tmpbuff = buff;
    RtlZeroMemory(buff,sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bufferLen);
    BITMAPFILEHEADER bf;
    bf.bfType=0x4d42;
    bf.bfSize=bufferLen+14+sizeof(BITMAPINFOHEADER);
    bf.bfOffBits=sizeof(bi.bmiHeader)+sizeof(bf);
    bf.bfReserved1=0;
    bf.bfReserved2=0;
    RtlMoveMemory(tmpbuff,&bf,sizeof(BITMAPFILEHEADER));
    tmpbuff += sizeof(BITMAPFILEHEADER);
    RtlMoveMemory(tmpbuff,&bi,sizeof(BITMAPINFO));
    tmpbuff += sizeof(BITMAPINFO);
    RtlMoveMemory(tmpbuff,destBuffer_.data(),bufferLen);
    return std::pair(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) + bufferLen, buff) ;
}


void ZegoExpressEngineMethodHandler::getVersion(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->Success(EXPRESS::ZegoExpressSDK::getVersion());
}

void ZegoExpressEngineMethodHandler::isFeatureSupported(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto featureType = std::get<int32_t>(argument[FTValue("featureType")]);
    result->Success(EXPRESS::ZegoExpressSDK::isFeatureSupported((EXPRESS::ZegoFeatureType)featureType));
}

void ZegoExpressEngineMethodHandler::setPluginVersion(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    std::string version = std::get<std::string>(argument[FTValue("version")]);
    
    version = "*** Plugin Version: " + version;

    zego_express_custom_log(version.c_str(), "Flutter");
    
    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoEngineConfig config;
    config.advancedConfig = {{"video_data_image_colorspace", "rgba"}, {"thirdparty_framework_info", "flutter"}};
    EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    // TODO: need to write getValue utils
    unsigned int appID = argument[FTValue("appID")].LongValue();
    std::string appSign = std::get<std::string>(argument[FTValue("appSign")]);
    bool isTestEnv = std::get<bool>(argument[FTValue("isTestEnv")]);
    int scenario = std::get<int32_t>(argument[FTValue("scenario")]);

    auto engine = EXPRESS::ZegoExpressSDK::createEngine(appID, appSign, isTestEnv, (EXPRESS::ZegoScenario)scenario, ZegoExpressEngineEventHandler::getInstance());

    engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
    engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());
    EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());

    ZegoTextureRendererController::getInstance()->init(registrar_->messenger());

    result->Success();
}

void ZegoExpressEngineMethodHandler::createEngineWithProfile(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap profileMap = std::get<FTMap>(argument[FTValue("profile")]);
    if(profileMap.size() > 0) {
        EXPRESS::ZegoEngineConfig config;
        config.advancedConfig = {{"video_data_image_colorspace", "rgba"}, {"thirdparty_framework_info", "flutter"}};
        EXPRESS::ZegoExpressSDK::setEngineConfig(config);

        EXPRESS::ZegoEngineProfile profilePtr;
        profilePtr.appID = profileMap[FTValue("appID")].LongValue();
        if (!profileMap[FTValue("appSign")].IsNull()) {
            profilePtr.appSign = std::get<std::string>(profileMap[FTValue("appSign")]);
        }  
        profilePtr.scenario = (EXPRESS::ZegoScenario)std::get<int32_t>(profileMap[FTValue("scenario")]);
        auto engine = EXPRESS::ZegoExpressSDK::createEngine(profilePtr, ZegoExpressEngineEventHandler::getInstance());

        engine->setAudioDataHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setDataRecordEventHandler(ZegoExpressEngineEventHandler::getInstance());
        engine->setCustomAudioProcessHandler(ZegoExpressEngineEventHandler::getInstance());
        EXPRESS::ZegoExpressSDK::setApiCalledCallback(ZegoExpressEngineEventHandler::getInstance());

        ZegoTextureRendererController::getInstance()->init(registrar_->messenger());
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::destroyEngine(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto engine = EXPRESS::ZegoExpressSDK::getEngine();

    if (engine) {
        ZegoTextureRendererController::getInstance()->uninit();
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        EXPRESS::ZegoExpressSDK::destroyEngine(engine, [=]() {
            sharedPtrResult->Success();
        });
    }
    else {
        result->Success();
    }

}

void ZegoExpressEngineMethodHandler::setEngineConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoEngineConfig config;

    if(configMap.size() > 0) {
        FTMap logConfigMap = std::get<FTMap>(configMap[FTValue("logConfig")]);
        std::unique_ptr<EXPRESS::ZegoLogConfig> logConfigPtr = nullptr;
        if(logConfigMap.size() > 0) {
            logConfigPtr = std::make_unique<EXPRESS::ZegoLogConfig>();
            logConfigPtr->logPath = std::get<std::string>(logConfigMap[FTValue("logPath")]);
            logConfigPtr->logSize = std::get<int32_t>(logConfigMap[FTValue("logSize")]);
        }

        config.logConfig = logConfigPtr.get();

        FTMap advancedConfigMap = std::get<FTMap>(configMap[FTValue("advancedConfig")]);
        for (auto& cfg : advancedConfigMap) {
            std::string key = std::get<std::string>(cfg.first);
            std::string value = std::get<std::string>(cfg.second);
			config.advancedConfig[key] = value;
		}

        EXPRESS::ZegoExpressSDK::setEngineConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLogConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoLogConfig config;
    if(configMap.size() > 0) {
        config.logPath = std::get<std::string>(configMap[FTValue("logPath")]);
        config.logSize = std::get<int32_t>(configMap[FTValue("logSize")]);
        EXPRESS::ZegoExpressSDK::setLogConfig(config);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = (EXPRESS::ZegoRoomMode)std::get<int32_t>(argument[FTValue("mode")]);
    EXPRESS::ZegoExpressSDK::setRoomMode(mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::uploadLog(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->uploadLog();
    result->Success();
}

void ZegoExpressEngineMethodHandler::setDebugVerbose(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto language = (EXPRESS::ZegoLanguage)std::get<int32_t>(argument[FTValue("language")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setDebugVerbose(enable, language);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableDebugAssistant(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableDebugAssistant(enable);
    result->Success();
}

void ZegoExpressEngineMethodHandler::callExperimentalAPI(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto params = std::get<std::string>(argument[FTValue("params")]);
    EXPRESS::ZegoExpressSDK::getEngine()->callExperimentalAPI(params);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setDummyCaptureImagePath(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto filePath = std::get<std::string>(argument[FTValue("filePath")]);
    auto channel = (EXPRESS::ZegoPublishChannel)std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setDummyCaptureImagePath(filePath, channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::loginRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto userMap = std::get<flutter::EncodableMap>(argument[FTValue("user")]);

    EXPRESS::ZegoUser user{ std::get<std::string>(userMap[FTValue("userID")]), std::get<std::string>(userMap[FTValue("userName")]) };

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    EXPRESS::ZegoRoomConfig config;
    if (configMap.size() > 0) {
        config.maxMemberCount = (unsigned int)std::get<int32_t>(configMap[FTValue("maxMemberCount")]);
        config.isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
        config.token = std::get<std::string>(configMap[FTValue("token")]);  
    }
    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->loginRoom(roomID, user, config, [=](int errorCode, std::string extendedData) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        if (extendedData.empty()) {
            retMap[FTValue("extendedData")] = FTValue("{}");
        }
        else {
            retMap[FTValue("extendedData")] = FTValue(extendedData);
        }
        sharedPtrResult->Success(retMap);
    });

}

void ZegoExpressEngineMethodHandler::loginMultiRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    EXPRESS::ZegoRoomConfig config;
    if (configMap.size() > 0) {
        config.maxMemberCount = (unsigned int)std::get<int32_t>(configMap[FTValue("maxMemberCount")]);
        config.isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
        config.token = std::get<std::string>(configMap[FTValue("token")]);  
    }
    EXPRESS::ZegoExpressSDK::getEngine()->loginMultiRoom(roomID, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::logoutRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    if (!argument[FTValue("roomID")].IsNull()) {
        auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom(roomID, [=](int errorCode, std::string extendedData) {
            FTMap retMap;

            retMap[FTValue("errorCode")] = FTValue(errorCode);
            if (extendedData.empty()) {
                retMap[FTValue("extendedData")] = FTValue("{}");
            }
            else {
                retMap[FTValue("extendedData")] = FTValue(extendedData);
            }

            sharedPtrResult->Success(retMap);
        });
    } else {
        EXPRESS::ZegoExpressSDK::getEngine()->logoutRoom([=](int errorCode, std::string extendedData) {
            FTMap retMap;

            retMap[FTValue("errorCode")] = FTValue(errorCode);
            if (extendedData.empty()) {
                retMap[FTValue("extendedData")] = FTValue("{}");
            }
            else {
                retMap[FTValue("extendedData")] = FTValue(extendedData);
            }

            sharedPtrResult->Success(retMap);
        });
    }
}

void ZegoExpressEngineMethodHandler::switchRoom(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto fromRoomID = std::get<std::string>(argument[FTValue("fromRoomID")]);
    auto toRoomID = std::get<std::string>(argument[FTValue("toRoomID")]);

    std::unique_ptr<EXPRESS::ZegoRoomConfig> configPtr = nullptr;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoRoomConfig>();

            configPtr->maxMemberCount = configMap[FTValue("maxMemberCount")].LongValue();
            configPtr->isUserStatusNotify = std::get<bool>(configMap[FTValue("isUserStatusNotify")]);
            configPtr->token = std::get<std::string>(configMap[FTValue("token")]);
        }
    }

    EXPRESS::ZegoExpressSDK::getEngine()->switchRoom(fromRoomID, toRoomID, configPtr.get());

    result->Success();
}

void ZegoExpressEngineMethodHandler::renewToken(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto token = std::get<std::string>(argument[FTValue("token")]);

    EXPRESS::ZegoExpressSDK::getEngine()->renewToken(roomID, token);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setRoomExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto value = std::get<std::string>(argument[FTValue("value")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setRoomExtraInfo(roomID, key, value, [=](int errorCode) {
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::startPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);

        if (configMap.size() > 0) {
            EXPRESS::ZegoPublisherConfig config;

            config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
            config.forceSynchronousNetworkTime = std::get<int32_t>(configMap[FTValue("forceSynchronousNetworkTime")]);
            config.streamCensorshipMode = (EXPRESS::ZegoStreamCensorshipMode)std::get<int32_t>(configMap[FTValue("streamCensorshipMode")]);

            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(streamID, config, (EXPRESS::ZegoPublishChannel)channel);
        } else {
            EXPRESS::ZegoExpressSDK::getEngine()->startPublishingStream(streamID, (EXPRESS::ZegoPublishChannel)channel);
        }
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPublishingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPublishingStream((EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setStreamExtraInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto extraInfo = std::get<std::string>(argument[FTValue("extraInfo")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->setStreamExtraInfo(extraInfo, [=](int errorCode) {

        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);

    }, (EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::startPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    flutter::EncodableMap canvasMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("canvas")])) {
        canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        ZegoTextureRendererController::getInstance()->addCapturedRenderer(viewID, (EXPRESS::ZegoPublishChannel)channel, viewMode);
    }
    EXPRESS::ZegoExpressSDK::getEngine()->startPreview(nullptr, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->stopPreview((EXPRESS::ZegoPublishChannel)channel);
    ZegoTextureRendererController::getInstance()->removeCapturedRenderer(
        (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setVideoConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoVideoConfig config;
    config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
    config.captureHeight = std::get<int32_t>(configMap[FTValue("captureHeight")]);
    config.captureWidth = std::get<int32_t>(configMap[FTValue("captureWidth")]);
    config.codecID = (EXPRESS::ZegoVideoCodecID)std::get<int32_t>(configMap[FTValue("codecID")]);
    config.encodeHeight = std::get<int32_t>(configMap[FTValue("encodeHeight")]);
    config.encodeWidth = std::get<int32_t>(configMap[FTValue("encodeWidth")]);
    config.fps = std::get<int32_t>(configMap[FTValue("fps")]);
    config.keyFrameInterval = std::get<int32_t>(configMap[FTValue("keyFrameInterval")]);
    
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setVideoConfig(config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::getVideoConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);
    auto config = EXPRESS::ZegoExpressSDK::getEngine()->getVideoConfig((EXPRESS::ZegoPublishChannel)channel);

    FTMap configMap;
    configMap[FTValue("bitrate")] = FTValue(config.bitrate);
    configMap[FTValue("captureHeight")] = FTValue(config.captureHeight);
    configMap[FTValue("captureWidth")] = FTValue(config.captureWidth);
    configMap[FTValue("codecID")] = FTValue((int32_t)config.codecID);
    configMap[FTValue("encodeHeight")] = FTValue(config.encodeHeight);
    configMap[FTValue("encodeWidth")] = FTValue(config.encodeWidth);
    configMap[FTValue("fps")] = FTValue(config.fps);
    configMap[FTValue("keyFrameInterval")] = FTValue(config.keyFrameInterval);

    result->Success(configMap);
}

void ZegoExpressEngineMethodHandler::setVideoMirrorMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto mirrorMode = (EXPRESS::ZegoVideoMirrorMode)std::get<int32_t>(argument[FTValue("mirrorMode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVideoMirrorMode(mirrorMode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    EXPRESS::ZegoAudioConfig config;
    config.bitrate = std::get<int32_t>(configMap[FTValue("bitrate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);
    config.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(configMap[FTValue("codecID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioConfig(config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::getAudioConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto config = EXPRESS::ZegoExpressSDK::getEngine()->getAudioConfig((EXPRESS::ZegoPublishChannel)channel);
    flutter::EncodableMap retMap;
    retMap[FTValue("bitrate")] = FTValue(config.bitrate);
    retMap[FTValue("channel")] = FTValue(config.channel);
    retMap[FTValue("codecID")] = FTValue(config.codecID);

    result->Success(retMap);
}

void ZegoExpressEngineMethodHandler::setPublishStreamEncryptionKey(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishStreamEncryptionKey(key, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePublishStreamSnapshot(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->takePublishStreamSnapshot([=](int errorCode, void *image){

        auto tmpData = CreateFromHBITMAP((HBITMAP)image);
        std::vector<uint8_t> raw_image(tmpData.second, tmpData.second + tmpData.first);
        delete []tmpData.second;

        FTMap resultMap;
        resultMap[FTValue("errorCode")] = FTValue(errorCode);
        resultMap[FTValue("image")] = FTValue(raw_image);
        sharedPtrResult->Success(resultMap);
        
    },(EXPRESS::ZegoPublishChannel)channel);
}

void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePublishStreamVideo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamVideo(mute, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setCaptureVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setCaptureVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioCaptureStereoMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioCaptureStereoMode((EXPRESS::ZegoAudioCaptureStereoMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendSEI(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendSEI(byteData.data(), (unsigned int)byteData.size(), (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendAudioSideInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto timeStampMs = std::get<double>(argument[FTValue("timeStampMs")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendAudioSideInfo(byteData.data(), (unsigned int)byteData.size(), timeStampMs, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareEncoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareEncoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setLowlightEnhancement(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setLowlightEnhancement((EXPRESS::ZegoLowlightEnhancementMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPlayingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

       flutter::EncodableMap canvasMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("canvas")])) {
        canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);
    }

    EXPRESS::ZegoCanvas canvas;
    if (EXPRESS::ZegoExpressSDK::getEngine() && canvasMap.size() > 0) {
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        ZegoTextureRendererController::getInstance()->addRemoteRenderer(viewID, streamID, viewMode);
    }

    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {

        EXPRESS::ZegoPlayerConfig config;
        config.resourceMode = (EXPRESS::ZegoStreamResourceMode)std::get<int32_t>(configMap[FTValue("resourceMode")]);
        auto videoCodecIDIndex = std::get<int32_t>(configMap[FTValue("videoCodecID")]);
        if (videoCodecIDIndex > 3) {
            config.videoCodecID = EXPRESS::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
        }
        else {
            config.videoCodecID = (EXPRESS::ZegoVideoCodecID)videoCodecIDIndex;
        }
        
        config.roomID = std::get<std::string>(configMap[FTValue("roomID")]);
        config.sourceResourceType = (EXPRESS::ZegoResourceType)std::get<int32_t>(configMap[FTValue("sourceResourceType")]);

        std::unique_ptr<EXPRESS::ZegoCDNConfig> cdnConfigPtr = nullptr;
        if (std::holds_alternative<flutter::EncodableMap>(configMap[FTValue("cdnConfig")])) {
            auto cdnConfigMap = std::get<flutter::EncodableMap>(configMap[FTValue("cdnConfig")]);
            if (cdnConfigMap.size() > 0) {
                cdnConfigPtr = std::make_unique<EXPRESS::ZegoCDNConfig>();
                cdnConfigPtr->url = std::get<std::string>(cdnConfigMap[FTValue("url")]);
                cdnConfigPtr->authParam = std::get<std::string>(cdnConfigMap[FTValue("authParam")]);
            }

        }

        config.cdnConfig = cdnConfigPtr.get();

        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr, config);
    }
    else {
        EXPRESS::ZegoExpressSDK::getEngine()->startPlayingStream(streamID, nullptr);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPlayingStream(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopPlayingStream(streamID);
    ZegoTextureRendererController::getInstance()->removeRemoteRenderer(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamCrossAppInfo(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) 
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    flutter::EncodableMap infoMap = std::get<flutter::EncodableMap>(argument[FTValue("info")]);
    EXPRESS::ZegoCrossAppInfo info;
    info.appID = infoMap[FTValue("appID")].LongValue();
    info.token = std::get<std::string>(infoMap[FTValue("token")]);
    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamCrossAppInfo(streamID, info);

    result->Success();
}

void ZegoExpressEngineMethodHandler::takePlayStreamSnapshot(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) 
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->takePlayStreamSnapshot(streamID, [=](int errorCode, void *image){

        auto tmpData = CreateFromHBITMAP((HBITMAP)image);
        std::vector<uint8_t> raw_image(tmpData.second, tmpData.second + tmpData.first);
        delete []tmpData.second;   

        FTMap resultMap;
        resultMap[FTValue("errorCode")] = FTValue(errorCode);
        resultMap[FTValue("image")] = FTValue(raw_image);
        sharedPtrResult->Success(resultMap);
    });
}

void ZegoExpressEngineMethodHandler::setPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayVolume(streamID, volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAllPlayStreamVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAllPlayStreamVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamAudio(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHardwareDecoder(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHardwareDecoder(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteMicrophone(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteMicrophone(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isMicrophoneMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isMicrophoneMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::muteSpeaker(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteSpeaker(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isSpeakerMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto isMuted = EXPRESS::ZegoExpressSDK::getEngine()->isSpeakerMuted();

    result->Success(FTValue(isMuted));
}

void ZegoExpressEngineMethodHandler::getAudioDeviceList(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);

    FTArray deviceListArray;
    auto deviceList = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceList((EXPRESS::ZegoAudioDeviceType)type);
    for(auto &deviceInfo : deviceList) {
        FTMap deviceMap;
        deviceMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

        deviceListArray.emplace_back(FTValue(deviceMap));
    }

    result->Success(deviceListArray);
}

void ZegoExpressEngineMethodHandler::getDefaultAudioDeviceID(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = EXPRESS::ZegoExpressSDK::getEngine()->getDefaultAudioDeviceID((EXPRESS::ZegoAudioDeviceType)type);

    result->Success(FTValue(deviceID));
}

void ZegoExpressEngineMethodHandler::useAudioDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto type = std::get<int32_t>(argument[FTValue("type")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    EXPRESS::ZegoExpressSDK::getEngine()->useAudioDevice((EXPRESS::ZegoAudioDeviceType)type, deviceID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startSoundLevelMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    flutter::EncodableMap configMap;
    if (std::holds_alternative<flutter::EncodableMap>(argument[FTValue("config")])) {
        configMap = std::get<flutter::EncodableMap>(argument[FTValue("config")]);
    }

    if (configMap.size() > 0) {
         EXPRESS::ZegoSoundLevelConfig config;
        config.millisecond = std::get<int32_t>(configMap[FTValue("millisecond")]);
        config.enableVAD = std::get<bool>(configMap[FTValue("enableVAD")]);
        EXPRESS::ZegoExpressSDK::getEngine()->startSoundLevelMonitor(config);
    }
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopSoundLevelMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopSoundLevelMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableHeadphoneMonitor(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setHeadphoneMonitorVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setHeadphoneMonitorVolume(volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAEC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAEC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAECMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAECMode((EXPRESS::ZegoAECMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAGC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableAGC(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableTransientANS(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTransientANS(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setANSMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    int mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setANSMode((EXPRESS::ZegoANSMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioEqualizerGain(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bandIndex = std::get<int32_t>(argument[FTValue("bandIndex")]);
    auto bandGain = std::get<double>(argument[FTValue("bandGain")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioEqualizerGain(bandIndex, (float)bandGain);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerPreset((EXPRESS::ZegoVoiceChangerPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setVoiceChangerParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoVoiceChangerParam param;
    param.pitch = (float)std::get<double>(paramMap[FTValue("pitch")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setVoiceChangerParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbPreset(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto preset = std::get<int32_t>(argument[FTValue("preset")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbPreset((EXPRESS::ZegoReverbPreset)preset);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbAdvancedParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoReverbAdvancedParam param;
    param.damping = (float)std::get<double>(paramMap[FTValue("damping")]);
    param.roomSize = (float)std::get<double>(paramMap[FTValue("roomSize")]);
    param.reverberance = (float)std::get<double>(paramMap[FTValue("reverberance")]);
    param.wetOnly = std::get<bool>(paramMap[FTValue("wetOnly")]);
    param.wetGain = (float)std::get<double>(paramMap[FTValue("wetGain")]);
    param.dryGain = (float)std::get<double>(paramMap[FTValue("dryGain")]);
    param.toneLow = (float)std::get<double>(paramMap[FTValue("toneLow")]);
    param.toneHigh = (float)std::get<double>(paramMap[FTValue("toneHigh")]);
    param.preDelay = (float)std::get<double>(paramMap[FTValue("preDelay")]);
    param.stereoWidth = (float)std::get<double>(paramMap[FTValue("stereoWidth")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbAdvancedParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setReverbEchoParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<flutter::EncodableMap>(argument[FTValue("param")]);
    EXPRESS::ZegoReverbEchoParam param;
    param.inGain = (float)std::get<double>(paramMap[FTValue("inGain")]);
    param.outGain = (float)std::get<double>(paramMap[FTValue("outGain")]);
    param.numDelays = std::get<int>(paramMap[FTValue("numDelays")]);
    memset(param.delay, 0, sizeof(int));
    memset(param.decay, 0, sizeof(float));
    FTArray delayVec = std::get<FTArray>(paramMap[FTValue("delay")]);
    FTArray decayVec = std::get<FTArray>(paramMap[FTValue("decay")]);

    for (int i = 0; i < param.numDelays; i++) {
        param.delay[i] = std::get<int32_t>(delayVec.at(i));
        param.decay[i] = (float)std::get<double>(decayVec.at(i));
    }

    EXPRESS::ZegoExpressSDK::getEngine()->setReverbEchoParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableVirtualStereo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableVirtualStereo(enable, angle);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enablePlayStreamVirtualStereo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto angle = std::get<int32_t>(argument[FTValue("angle")]);
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePlayStreamVirtualStereo(enable, angle, streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setElectronicEffects(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto tonal = std::get<int32_t>(argument[FTValue("tonal")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setElectronicEffects(enable, (ZEGO::EXPRESS::ZegoElectronicEffectsMode)mode, tonal);

    result->Success();
}


void ZegoExpressEngineMethodHandler::startAudioDataObserver(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bitmask = std::get<int32_t>(argument[FTValue("observerBitMask")]);
    auto param = std::get<FTMap>(argument[FTValue("param")]);

    EXPRESS::ZegoAudioFrameParam nativeParam;
    nativeParam.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(param[FTValue("sampleRate")]);
    nativeParam.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(param[FTValue("channel")]);
    EXPRESS::ZegoExpressSDK::getEngine()->startAudioDataObserver(bitmask, nativeParam);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioDataObserver(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioDataObserver();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createAudioEffectPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto player = EXPRESS::ZegoExpressSDK::getEngine()->createAudioEffectPlayer();
    if(player) {

        int index = player->getIndex();
        player->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        audioEffectPlayerMap_[index] = player;

        result->Success(FTValue(index));

    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyAudioEffectPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if(player) {

        EXPRESS::ZegoExpressSDK::getEngine()->destroyAudioEffectPlayer(player);
        audioEffectPlayerMap_.erase(index);

        result->Success();
    } else {
        result->Error("destroyAudioEffectPlayer_Can_not_find_player", "Invoke `destroyAudioEffectPlayer` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStart(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
        std::unique_ptr<EXPRESS::ZegoAudioEffectPlayConfig> configPtr = nullptr;

        if (configMap.size() > 0) {
            configPtr = std::make_unique<EXPRESS::ZegoAudioEffectPlayConfig>();
            configPtr->playCount = std::get<int32_t>(configMap[FTValue("playCount")]);
            configPtr->isPublishOut = std::get<bool>(configMap[FTValue("isPublishOut")]);
        }

        player->start(audioEffectID, path, configPtr.get());

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStart_Can_not_find_player", "Invoke `audioEffectPlayerStart` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStop(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->stop(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStop_Can_not_find_player", "Invoke `audioEffectPlayerStop` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPause(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->pause(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerPause_Can_not_find_player", "Invoke `audioEffectPlayerPause` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->resume(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerResume_Can_not_find_player", "Invoke `audioEffectPlayerResume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerStopAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->stopAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerStopAll_Can_not_find_player", "Invoke `audioEffectPlayerStopAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerPauseAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->pauseAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerPauseAll_Can_not_find_player", "Invoke `audioEffectPlayerPauseAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerResumeAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        player->resumeAll();

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerResumeAll_Can_not_find_player", "Invoke `audioEffectPlayerResumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSeekTo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->seekTo(audioEffectID, millisecond, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }
    else {
        result->Error("audioEffectPlayerSeekTo_Can_not_find_player", "Invoke `audioEffectPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolume(audioEffectID, volume);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerSetVolume_Can_not_find_player", "Invoke `audioEffectPlayerSetVolume` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetVolumeAll(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        int volume = std::get<int32_t>(argument[FTValue("volume")]);
        player->setVolumeAll(volume);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerSetVolumeAll_Can_not_find_player", "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetTotalDuration(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto totalDuration = player->getTotalDuration(audioEffectID);

        result->Success(FTValue((int64_t)totalDuration));
    }
    else {
        result->Error("audioEffectPlayerGetTotalDuration_Can_not_find_player", "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerGetCurrentProgress(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto currentProgress = player->getCurrentProgress(audioEffectID);

        result->Success(FTValue((int64_t)currentProgress));
    }
    else {
        result->Error("audioEffectPlayerGetCurrentProgress_Can_not_find_player", "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerLoadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        player->loadResource(audioEffectID, path, [=](int errorCode) {

            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }
    else {
        result->Error("audioEffectPlayerLoadResource_Can_not_find_player", "Invoke `audioEffectPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerUnloadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        player->unloadResource(audioEffectID);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerUnLoadResource_Can_not_find_player", "Invoke `audioEffectPlayerUnloadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::audioEffectPlayerSetPlaySpeed(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto player = audioEffectPlayerMap_[index];
    if (player) {

        unsigned int audioEffectID = (unsigned int)std::get<int32_t>(argument[FTValue("audioEffectID")]);
        auto speed = std::get<double>(argument[FTValue("speed")]);
        player->setPlaySpeed(audioEffectID, speed);

        result->Success();
    }
    else {
        result->Error("audioEffectPlayerSetPlaySpeed_Can_not_find_player", "Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::createMediaPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mediaPlayer = EXPRESS::ZegoExpressSDK::getEngine()->createMediaPlayer();
    if(mediaPlayer) {
        auto index = mediaPlayer->getIndex();

        mediaPlayer->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        mediaPlayer->setVideoHandler(ZegoTextureRendererController::getInstance(), ZEGO::EXPRESS::ZEGO_VIDEO_FRAME_FORMAT_RGBA32);
        mediaPlayerMap_[index] = mediaPlayer;

        result->Success(FTValue(index));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyMediaPlayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        ZegoTextureRendererController::getInstance()->removeMediaPlayerRenderer(mediaPlayer);
        EXPRESS::ZegoExpressSDK::getEngine()->destroyMediaPlayer(mediaPlayer);
    }

    mediaPlayerMap_.erase(index);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResource(path, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
    else 
    {
        result->Error("mediaPlayerLoadResource_Can_not_find_player", "Invoke `mediaPlayerLoadResource` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceFromMediaData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto mediaData = std::get<std::vector<uint8_t>>(argument[FTValue("mediaData")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceFromMediaData(mediaData.data(), (int)mediaData.size(),startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
    else 
    {
        result->Error("mediaPlayerLoadResourceFromMediaData_Can_not_find_player", "Invoke `mediaPlayerLoadResourceFromMediaData` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadResourceWithPosition(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        std::string path = std::get<std::string>(argument[FTValue("path")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadResourceWithPosition(path, startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
    else 
    {
        result->Error("mediaPlayerLoadResourceWithPosition_Can_not_find_player", "Invoke `mediaPlayerLoadResourceWithPosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerStart(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->start();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerStop(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->stop();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerPause(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->pause();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerResume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        mediaPlayer->resume();
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSeekTo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        unsigned long long millisecond = argument[FTValue("millisecond")].LongValue();

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        mediaPlayer->seekTo(millisecond, [=](int errorCode){
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    }    
    else 
    {
        result->Error("mediaPlayerSeekTo_Can_not_find_player", "Invoke `mediaPlayerSeekTo` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableRepeat(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableRepeat(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAux(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool enable = std::get<bool>(argument[FTValue("enable")]);

        mediaPlayer->enableAux(enable);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerMuteLocal(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        bool mute = std::get<bool>(argument[FTValue("mute")]);

        mediaPlayer->muteLocal(mute);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPlayVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPublishVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = std::get<int32_t>(argument[FTValue("volume")]);

        mediaPlayer->setPublishVolume(volume);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetProgressInterval(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int millsecond = std::get<int32_t>(argument[FTValue("millisecond")]);

        mediaPlayer->setProgressInterval((unsigned long long)millsecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPlayVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = mediaPlayer->getPlayVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetPublishVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        int volume = mediaPlayer->getPublishVolume();
        result->Success(FTValue(volume));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetTotalDuration(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto totalDuration = mediaPlayer->getTotalDuration();
        // TODO: need test
        result->Success(FTValue((int64_t)totalDuration));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentProgress(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto currentProgress = mediaPlayer->getCurrentProgress();
        // TODO: need test
        result->Success(FTValue((int64_t)currentProgress));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetAudioTrackCount(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto audioTrackCount = mediaPlayer->getAudioTrackCount();
        result->Success(FTValue((int32_t)audioTrackCount));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetAudioTrackIndex(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto trackIndex = std::get<int32_t>(argument[FTValue("trackIndex")]);
        mediaPlayer->setAudioTrackIndex(trackIndex);

    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetVoiceChangerParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        FTMap paramMap = std::get<FTMap>(argument[FTValue("param")]);
        auto pitch = std::get<double>(paramMap[FTValue("pitch")]);

        auto audioChannel = std::get<int32_t>(argument[FTValue("audioChannel")]);

        EXPRESS::ZegoVoiceChangerParam param;
        param.pitch = (float)pitch;
        mediaPlayer->setVoiceChangerParam((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel, param);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetCurrentState(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto state = mediaPlayer->getCurrentState();
        result->Success(FTValue((int32_t)state));
    } else {
        result->Success(FTValue(0));
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlaySpeed(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        auto speed = std::get<double>(argument[FTValue("speed")]);
        mediaPlayer->setPlaySpeed(speed);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableSoundLevelMonitor(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableSoundLevelMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableFrequencySpectrumMonitor(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        auto millisecond = argument[FTValue("millisecond")].LongValue();
        mediaPlayer->enableFrequencySpectrumMonitor(enable, millisecond);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkResourceMaxCache(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto time = argument[FTValue("time")].LongValue();
        auto size = argument[FTValue("size")].LongValue();
        mediaPlayer->setNetWorkResourceMaxCache(time, size);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetNetWorkBufferThreshold(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto threshold = argument[FTValue("threshold")].LongValue();
        mediaPlayer->setNetWorkBufferThreshold(threshold);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::mediaPlayerGetNetWorkResourceCache(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        FTMap resultMap;
        auto netWorkResourceCache = mediaPlayer->getNetWorkResourceCache();

        resultMap[FTValue("time")] = FTValue((int64_t)netWorkResourceCache->time);
        resultMap[FTValue("size")] = FTValue((int64_t)netWorkResourceCache->size);
        result->Success(resultMap);
    }
    else 
    {
        result->Error("mediaPlayerGetNetWorkResourceCache_Can_not_find_player", "Invoke `mediaPlayerGetNetWorkResourceCache` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerEnableAccurateSeek(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoAccurateSeekConfig config;
        config.timeout = argument[FTValue("config")].LongValue();
        mediaPlayer->enableAccurateSeek(enable, &config);

        result->Success();
    }
    else
    {
        result->Error("mediaPlayerEnableAccurateSeek_Can_not_find_player", "Invoke `mediaPlayerEnableAccurateSeek` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerLoadCopyrightedMusicResourceWithPosition(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if(mediaPlayer) {
        std::string resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        uint64_t startPosition = argument[FTValue("startPosition")].LongValue();

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));

        mediaPlayer->loadCopyrightedMusicResourceWithPosition(resourceID, startPosition, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);

            sharedPtrResult->Success(retMap);
        });
    }
    else 
    {
        result->Error("mediaPlayerLoadCopyrightedMusicResourceWithPosition_Can_not_find_player", "Invoke `mediaPlayerLoadCopyrightedMusicResourceWithPosition` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerClearView(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        mediaPlayer->clearView();

        result->Success();
    }
    else
    {
        result->Error("mediaPlayerClearView_Can_not_find_player", "Invoke `mediaPlayerClearView` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetActiveAudioChannel(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto audioChannel = std::get<int32_t>(argument[FTValue("audioChannel")]);
        mediaPlayer->setActiveAudioChannel((EXPRESS::ZegoMediaPlayerAudioChannel)audioChannel);

        result->Success();
    }
    else
    {
        result->Error("mediaPlayerSetActiveAudioChannel_Can_not_find_player", "Invoke `mediaPlayerSetActiveAudioChannel` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerSetPlayerCanvas(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {

        flutter::EncodableMap canvasMap = std::get<flutter::EncodableMap>(argument[FTValue("canvas")]);

        EXPRESS::ZegoCanvas canvas;
        auto viewMode = (EXPRESS::ZegoViewMode)std::get<int32_t>(canvasMap[FTValue("viewMode")]);
        auto viewID = canvasMap[FTValue("view")].LongValue();
        if (ZegoTextureRendererController::getInstance()->addMediaPlayerRenderer(viewID, mediaPlayer, viewMode)) {
            result->Success();
        } else {
            result->Error("mediaPlayerSetPlayerCanvas_Canvas_error", "Invoke `mediaPlayerSetPlayerCanvas` but canvas is abnormal, please check canvas");
        }
    }
    else
    {
        result->Error("mediaPlayerSetPlayerCanvas_Can_not_find_player", "Invoke `mediaPlayerSetPlayerCanvas` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::mediaPlayerTakeSnapshot(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    auto mediaPlayer = mediaPlayerMap_[index];

    if (mediaPlayer) {
        auto pFrame = ZegoTextureRendererController::getInstance()->getMediaPlayerFrame(mediaPlayer);
        auto size = ZegoTextureRendererController::getInstance()->getMediaPlayerSize(mediaPlayer);
        FTMap resultMap;
        if (pFrame && size != std::pair(0,0)) {
            auto tmpData = makeBtimap(pFrame, size);
            std::vector<uint8_t> raw_image(tmpData.second, tmpData.second + tmpData.first);
            delete []tmpData.second;

            resultMap[FTValue("image")] = FTValue(raw_image);
            resultMap[FTValue("errorCode")] = FTValue(0);
        } else {
            resultMap[FTValue("errorCode")] = FTValue(-1);
        }
        result->Success(resultMap);
    }
    else
    {
        result->Error("mediaPlayerTakeSnapshot_Can_not_find_player", "Invoke `mediaPlayerTakeSnapshot` but can't find specific player");
    }
}

void ZegoExpressEngineMethodHandler::startMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{

    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);

    for (auto& cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    auto inputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("inputList")]);
    auto outputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("outputList")]);

    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    // minPlayStreamBufferLength
    task.minPlayStreamBufferLength = std::get<int32_t>(argument[FTValue("minPlayStreamBufferLength")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);

    task.videoConfig.bitrate = std::get<int32_t>(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = std::get<int32_t>(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = std::get<int32_t>(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = std::get<int32_t>(taskVideoConfig[FTValue("width")]);

    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = std::get<FTMap>(inputIter);

        input.streamID = std::get<std::string>(inputMap[FTValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) - std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) - std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        input.volume = std::get<int32_t>(inputMap[FTValue("volume")]);
        input.isAudioFocus = std::get<bool>(inputMap[FTValue("isAudioFocus")]);
        input.audioDirection = std::get<int32_t>(inputMap[FTValue("audioDirection")]);

        if (!inputMap[FTValue("label")].IsNull()) {
            auto labelMap = std::get<flutter::EncodableMap>(inputMap[FTValue("label")]) ;
            auto text = std::get<std::string>(labelMap[FTValue("text")]);
            EXPRESS::ZegoLabelInfo labelInfo(text);
            labelInfo.left = std::get<int32_t>(labelMap[FTValue("left")]);
            labelInfo.top =  std::get<int32_t>(labelMap[FTValue("top")]);
            auto fontMap = std::get<flutter::EncodableMap>(labelMap[FTValue("font")]);
            EXPRESS::ZegoFontStyle fontStyle;
            fontStyle.type = (EXPRESS::ZegoFontType)std::get<int32_t>(fontMap[FTValue("type")]);
            fontStyle.size = std::get<int32_t>(fontMap[FTValue("size")]);
            fontStyle.color = std::get<int32_t>(fontMap[FTValue("color")]);
            fontStyle.transparency = std::get<int32_t>(fontMap[FTValue("transparency")]);
            fontStyle.border = std::get<bool>(fontMap[FTValue("border")]);
            fontStyle.borderColor = std::get<int32_t>(fontMap[FTValue("borderColor")]);
            labelInfo.font = fontStyle;
            input.label = labelInfo;
        }

        if (!inputMap[FTValue("renderMode")].IsNull()) {
            input.renderMode = (EXPRESS::ZegoMixRenderMode)std::get<int32_t>(inputMap[FTValue("renderMode")]);
        }

        if (!inputMap[FTValue("imageInfo")].IsNull()) {
            auto imageInfoMap = std::get<flutter::EncodableMap>(inputMap[FTValue("imageInfo")]);
            auto url = std::get<std::string>(imageInfoMap[FTValue("url")]);
            input.imageInfo = EXPRESS::ZegoMixerImageInfo(url);
        }

        if (!inputMap[FTValue("cornerRadius")].IsNull()) {
            input.cornerRadius = std::get<int32_t>(inputMap[FTValue("cornerRadius")]);
        }

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startMixerTask(task, [=](int errorCode, std::string extendedData) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("extendedData")] = FTValue(extendedData);

        sharedPtrResult->Success(retMap);
    });

}

void ZegoExpressEngineMethodHandler::stopMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoMixerTask task;
    auto taskAudioConfig = std::get<flutter::EncodableMap>(argument[FTValue("audioConfig")]);
    auto taskVideoConfig = std::get<flutter::EncodableMap>(argument[FTValue("videoConfig")]);
    auto taskWatermark = std::get<flutter::EncodableMap>(argument[FTValue("watermark")]);

    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);

    task.audioConfig.bitrate = std::get<int32_t>(taskAudioConfig[FTValue("bitrate")]);
    task.audioConfig.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(taskAudioConfig[FTValue("channel")]);
    task.audioConfig.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(taskAudioConfig[FTValue("codecID")]);

    task.videoConfig.bitrate = std::get<int32_t>(taskVideoConfig[FTValue("bitrate")]);
    task.videoConfig.fps = std::get<int32_t>(taskVideoConfig[FTValue("fps")]);
    task.videoConfig.height = std::get<int32_t>(taskVideoConfig[FTValue("height")]);
    task.videoConfig.width = std::get<int32_t>(taskVideoConfig[FTValue("width")]);

    auto inputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("inputList")]);
    auto outputFlutterList = std::get<flutter::EncodableList>(argument[FTValue("outputList")]);


    for (auto &inputIter : inputFlutterList) {
        EXPRESS::ZegoMixerInput input;

        auto inputMap = std::get<FTMap>(inputIter);

        input.streamID = std::get<std::string>(inputMap[FTValue("streamID")]);
        input.contentType = (EXPRESS::ZegoMixerInputContentType)std::get<int32_t>(inputMap[FTValue("contentType")]);
        input.layout.height = std::get<int32_t>(inputMap[FTValue("bottom")]) - std::get<int32_t>(inputMap[FTValue("top")]);
        input.layout.width = std::get<int32_t>(inputMap[FTValue("right")]) - std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.x = std::get<int32_t>(inputMap[FTValue("left")]);
        input.layout.y = std::get<int32_t>(inputMap[FTValue("top")]);
        input.soundLevelID = std::get<int32_t>(inputMap[FTValue("soundLevelID")]);

        task.inputList.push_back(input);
    }
    for (auto &outputIter : outputFlutterList) {
        EXPRESS::ZegoMixerOutput output;
        FTMap outputMap = std::get<FTMap>(outputIter);
        output.target = std::get<std::string>(outputMap[FTValue("target")]);

        task.outputList.push_back(output);
    }

    task.watermark = nullptr;
    task.backgroundImageURL = std::get<std::string>(argument[FTValue("backgroundImageURL")]);
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    FTMap advancedConfigMap = std::get<FTMap>(argument[FTValue("advancedConfig")]);
    for (auto& cfg : advancedConfigMap) {
        std::string key = std::get<std::string>(cfg.first);
        std::string value = std::get<std::string>(cfg.second);
        task.advancedConfig[key] = value;
    }

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->stopMixerTask(task, [=](int errorCode) {
        FTMap retMap;

        retMap[FTValue("errorCode")] = FTValue(errorCode);

        sharedPtrResult->Success(retMap);
    });

}

void ZegoExpressEngineMethodHandler::setSEIConfig(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoSEIConfig config;
    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.type = (EXPRESS::ZegoSEIType)std::get<int32_t>(configMap[FTValue("type")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setSEIConfig(config);
    result->Success();
}

void ZegoExpressEngineMethodHandler::setAudioDeviceVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto volume = std::get<int32_t>(argument[FTValue("volume")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setAudioDeviceVolume((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), volume);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAudioDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    
    EXPRESS::ZegoExpressSDK::getEngine()->muteAudioDevice((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str(), mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isAudioDeviceMuted(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    auto ret = EXPRESS::ZegoExpressSDK::getEngine()->isAudioDeviceMuted((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID);
    
    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setAudioDeviceMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioDeviceVolume(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto deviceType = std::get<int32_t>(argument[FTValue("deviceType")]);
    auto deviceID = std::get<std::string>(argument[FTValue("deviceID")]);

    auto volume = EXPRESS::ZegoExpressSDK::getEngine()->getAudioDeviceVolume((EXPRESS::ZegoAudioDeviceType)deviceType, deviceID.c_str());

    result->Success(FTValue(volume));
}

void ZegoExpressEngineMethodHandler::enableAudioCaptureDevice(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    EXPRESS::ZegoExpressSDK::getEngine()->enableAudioCaptureDevice(enable);
    result->Success();
}

/*
void ZegoExpressEngineMethodHandler::mutePublishStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePublishStreamAudio(mute, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}
void ZegoExpressEngineMethodHandler::mutePlayStreamAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamAudio(streamID, mute);
    result->Success();
}
*/

void ZegoExpressEngineMethodHandler::enableTrafficControl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto property = std::get<int32_t>(argument[FTValue("property")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableTrafficControl(enable, property);
    result->Success();
}

void ZegoExpressEngineMethodHandler::startRecordingCapturedData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoDataRecordConfig config;

    FTMap configMap = std::get<FTMap>(argument[FTValue("config")]);

    config.filePath = std::get<std::string>(configMap[FTValue("filePath")]);
    config.recordType = (EXPRESS::ZegoDataRecordType)std::get<int32_t>(configMap[FTValue("recordType")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->startRecordingCapturedData(config, (EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::stopRecordingCapturedData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->stopRecordingCapturedData((EXPRESS::ZegoPublishChannel)channel);
    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCamera(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCamera(enable, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCameraAdaptiveFPS(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto minFPS = std::get<int32_t>(argument[FTValue("minFPS")]);
    auto maxFPS = std::get<int32_t>(argument[FTValue("maxFPS")]); 
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCameraAdaptiveFPS(enable, minFPS, maxFPS, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::createCopyrightedMusic(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto tmpCopyrightedMusic = EXPRESS::ZegoExpressSDK::getEngine()->createCopyrightedMusic();
    if (tmpCopyrightedMusic != nullptr) {
        copyrightedMusic_ = tmpCopyrightedMusic;
        copyrightedMusic_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(FTValue(0));
    }
    else
    {
        result->Success(FTValue(-1));
    }
    
}

void ZegoExpressEngineMethodHandler::destroyCopyrightedMusic(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyCopyrightedMusic(copyrightedMusic_);
        copyrightedMusic_ = nullptr;
        result->Success();
    }
    else
    {
        result->Error("destroyCopyrightedMusic_Can_not_find_instance", "Invoke `destroyCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicClearCache(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        copyrightedMusic_->clearCache();
        result->Success();
    }
    else
    {
        result->Error("copyrightedMusicClearCache_Can_not_find_instance", "Invoke `copyrightedMusicClearCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicDownload(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->download(resourceID, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
         });  
    }
    else
    {
        result->Error("copyrightedMusicDownload_Can_not_find_instance", "Invoke `copyrightedMusicDownload` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetAverageScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getAverageScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicGetAverageScore_Can_not_find_instance", "Invoke `copyrightedMusicGetAverageScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCacheSize(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto ret = copyrightedMusic_->getCacheSize();
        result->Success(FTValue((int64_t)ret));
    }
    else
    {
        result->Error("copyrightedMusicGetCacheSize_Can_not_find_instance", "Invoke `copyrightedMusicGetCacheSize` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetCurrentPitch(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getCurrentPitch(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicGetCurrentPitch_Can_not_find_instance", "Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetDuration(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getDuration(resourceID);
        result->Success(FTValue((int64_t)ret));
    }
    else
    {
        result->Error("copyrightedMusicGetDuration_Can_not_find_instance", "Invoke `copyrightedMusicGetDuration` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetKrcLyricByToken(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto krcToken = std::get<std::string>(argument[FTValue("krcToken")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getKrcLyricByToken(krcToken, [=](int errorCode, std::string lyrics) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("lyrics")] = FTValue(lyrics);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicGetKrcLyricByToken_Can_not_find_instance", "Invoke `copyrightedMusicGetKrcLyricByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetLrcLyric(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto songID = std::get<std::string>(argument[FTValue("songID")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getLrcLyric(songID, [=](int errorCode, std::string lyrics) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("lyrics")] = FTValue(lyrics);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicGetLrcLyric_Can_not_find_instance", "Invoke `copyrightedMusicGetLrcLyric` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetMusicByToken(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto shareToken = std::get<std::string>(argument[FTValue("shareToken")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getMusicByToken(shareToken, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicGetMusicByToken_Can_not_find_instance", "Invoke `copyrightedMusicGetMusicByToken` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetPreviousScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getPreviousScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicGetPreviousScore_Can_not_find_instance", "Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetStandardPitch(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->getStandardPitch(resourceID, [=](int errorCode, std::string pitch) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("pitch")] = FTValue(pitch);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicGetStandardPitch_Can_not_find_instance", "Invoke `copyrightedMusicGetStandardPitch` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicGetTotalScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->getTotalScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicGetTotalScore_Can_not_find_instance", "Invoke `copyrightedMusicGetTotalScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicInitCopyrightedMusic(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicConfig config;
        auto userMap = std::get<FTMap>(configMap[FTValue("user")]);
        config.user.userID = std::get<std::string>(userMap[FTValue("userID")]);
        config.user.userName = std::get<std::string>(userMap[FTValue("userName")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->initCopyrightedMusic(config, [=](int errorCode) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicInitCopyrightedMusic_Can_not_find_instance", "Invoke `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicPauseScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->pauseScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicPauseScore_Can_not_find_instance", "Invoke `copyrightedMusicPauseScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicQueryCache(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto songID = std::get<std::string>(argument[FTValue("songID")]);
        auto type = std::get<int32_t>(argument[FTValue("type")]);
        auto ret = copyrightedMusic_->queryCache(songID, (EXPRESS::ZegoCopyrightedMusicType)type);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicQueryCache_Can_not_find_instance", "Invoke `copyrightedMusicQueryCache` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompaniment(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestAccompaniment(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
         });
    }
    else
    {
        result->Error("copyrightedMusicRequestAccompaniment_Can_not_find_instance", "copyrightedMusicRequestAccompaniment `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestAccompanimentClip(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestAccompanimentClip(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicRequestAccompanimentClip_Can_not_find_instance", "copyrightedMusicRequestAccompanimentClip `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicRequestSong(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto configMap = std::get<FTMap>(argument[FTValue("config")]);
        EXPRESS::ZegoCopyrightedMusicRequestConfig config;
        config.songID = std::get<std::string>(configMap[FTValue("songID")]);
        config.mode = (EXPRESS::ZegoCopyrightedMusicBillingMode)std::get<int32_t>(configMap[FTValue("mode")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->requestSong(config, [=](int errorCode, std::string resource) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("resource")] = FTValue(resource);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicRequestSong_Can_not_find_instance", "copyrightedMusicRequestSong `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResetScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resetScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicResetScore_Can_not_find_instance", "Invoke `copyrightedMusicResetScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicResumeScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->resumeScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicResumeScore_Can_not_find_instance", "Invoke `copyrightedMusicResumeScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicSendExtendedRequest(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto command = std::get<std::string>(argument[FTValue("command")]);
        auto params = std::get<std::string>(argument[FTValue("params")]);
        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        copyrightedMusic_->sendExtendedRequest(command, params, [=](int errorCode, std::string command, std::string result) {
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            retMap[FTValue("command")] = FTValue(command);
            retMap[FTValue("result")] = FTValue(result);
            sharedPtrResult->Success(retMap);
            });
    }
    else
    {
        result->Error("copyrightedMusicSendExtendedRequest_Can_not_find_instance", "copyrightedMusicSendExtendedRequest `copyrightedMusicInitCopyrightedMusic` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStartScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto pitchValueInterval = std::get<int32_t>(argument[FTValue("pitchValueInterval")]);
        auto ret = copyrightedMusic_->startScore(resourceID, pitchValueInterval);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicStartScore_Can_not_find_instance", "Invoke `copyrightedMusicStartScore` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::copyrightedMusicStopScore(flutter::EncodableMap& argument, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (copyrightedMusic_) {
        auto resourceID = std::get<std::string>(argument[FTValue("resourceID")]);
        auto ret = copyrightedMusic_->stopScore(resourceID);
        result->Success(FTValue(ret));
    }
    else
    {
        result->Error("copyrightedMusicStopScore_Can_not_find_instance", "Invoke `copyrightedMusicStopScore` but can't find specific instance");
    }
}


void ZegoExpressEngineMethodHandler::createTextureRenderer(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto width = std::get<int32_t>(argument[FTValue("width")]);
    auto height = std::get<int32_t>(argument[FTValue("height")]);

    auto textureID = ZegoTextureRendererController::getInstance()->createTextureRenderer(registrar_->texture_registrar(), width, height);

    result->Success(FTValue(textureID));
}

void ZegoExpressEngineMethodHandler::destroyTextureRenderer(flutter::EncodableMap& argument,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto textureID = argument[FTValue("textureID")].LongValue();
    bool state = ZegoTextureRendererController::getInstance()->destroyTextureRenderer(textureID);

    result->Success(FTValue(state));
}


void ZegoExpressEngineMethodHandler::setMinVideoBitrateForTrafficControl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto bitrate = std::get<int32_t>(argument[FTValue("bitrate")]);
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoBitrateForTrafficControl(bitrate, (EXPRESS::ZegoTrafficControlMinVideoBitrateMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoFpsForTrafficControl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto fps = std::get<int32_t>(argument[FTValue("fps")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoFpsForTrafficControl(fps, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setMinVideoResolutionForTrafficControl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto width = std::get<int32_t>(argument[FTValue("width")]);
    auto height = std::get<int32_t>(argument[FTValue("height")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setMinVideoResolutionForTrafficControl(width, height, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setTrafficControlFocusOn(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setTrafficControlFocusOn((EXPRESS::ZegoTrafficControlFocusOnMode)mode, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::addPublishCdnUrl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto targetURL = std::get<std::string>(argument[FTValue("targetURL")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->addPublishCdnUrl(streamID, targetURL, [=](int errorCode){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::removePublishCdnUrl(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto targetURL = std::get<std::string>(argument[FTValue("targetURL")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->removePublishCdnUrl(streamID, targetURL, [=](int errorCode){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::enablePublishDirectToCDN(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCDNConfig config;
    if (configMap.size() > 0) {
        config.url = std::get<std::string>(configMap[FTValue("url")]);
        config.authParam = std::get<std::string>(configMap[FTValue("authParam")]);
    }

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enablePublishDirectToCDN(enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPublishWatermark(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto watermarkMap = std::get<FTMap>(argument[FTValue("watermark")]);
    EXPRESS::ZegoWatermark watermark;
    if (watermarkMap.size() > 0) {
        watermark.imageURL = std::get<std::string>(watermarkMap[FTValue("imageURL")]);
        watermark.layout.x = std::get<int32_t>(watermarkMap[FTValue("left")]);
        watermark.layout.y = std::get<int32_t>(watermarkMap[FTValue("top")]);
        watermark.layout.width = std::get<int32_t>(watermarkMap[FTValue("right")]) - watermark.layout.x;
        watermark.layout.height = std::get<int32_t>(watermarkMap[FTValue("bottom")]) - watermark.layout.y;
    }
    auto isPreviewVisible = std::get<bool>(argument[FTValue("isPreviewVisible")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPublishWatermark(&watermark, isPreviewVisible, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamDecryptionKey(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto key = std::get<std::string>(argument[FTValue("key")]);
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamDecryptionKey(streamID, key);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamVideoLayer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto videoLayer = std::get<int32_t>(argument[FTValue("videoLayer")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamVideoLayer(streamID, (EXPRESS::ZegoPlayerVideoLayer)videoLayer);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamVideoType(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto streamType = std::get<int32_t>(argument[FTValue("streamType")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamVideoType(streamID, (EXPRESS::ZegoVideoStreamType)streamType);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamBufferIntervalRange(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto minBufferInterval = std::get<int32_t>(argument[FTValue("minBufferInterval")]);
    auto maxBufferInterval = std::get<int32_t>(argument[FTValue("maxBufferInterval")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamBufferIntervalRange(streamID, minBufferInterval, maxBufferInterval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setPlayStreamFocusOn(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamFocusOn(streamID);

    result->Success();
}

void ZegoExpressEngineMethodHandler::mutePlayStreamVideo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto streamID = std::get<std::string>(argument[FTValue("streamID")]);
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->mutePlayStreamVideo(streamID, mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::muteAllPlayStreamVideo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mute = std::get<bool>(argument[FTValue("mute")]);

    EXPRESS::ZegoExpressSDK::getEngine()->muteAllPlayStreamVideo(mute);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCheckPoc(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCheckPoc(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::isVideoDecoderSupported(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto codecID = std::get<int32_t>(argument[FTValue("codecID")]);

    bool ret = EXPRESS::ZegoExpressSDK::getEngine()->isVideoDecoderSupported((EXPRESS::ZegoVideoCodecID)codecID);

    result->Success(FTValue(ret));
}

void ZegoExpressEngineMethodHandler::setPlayStreamsAlignmentProperty(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto mode = std::get<int32_t>(argument[FTValue("mode")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setPlayStreamsAlignmentProperty((EXPRESS::ZegoStreamAlignmentMode)mode);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startAutoMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);
    task.roomID = std::get<std::string>(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = std::get<FTArray>(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = std::get<FTMap>(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = std::get<std::string>(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = std::get<FTMap>(outputMap[FTValue("videoConfig")]);
                
                auto codecIDIndex = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 3) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency = std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile = (ZEGO::EXPRESS::ZegoEncodeProfile)std::get<int32_t>(videoConfigMap[FTValue("encodeProfile")]);
            }
            task.outputList.push_back(output);
        }
    }
    // AudioConfig
    auto audioConfigMap = std::get<FTMap>(argument[FTValue("audioConfig")]);
    if (audioConfigMap.size() > 0) {
        task.audioConfig.bitrate = std::get<int32_t>(audioConfigMap[FTValue("bitrate")]);
        task.audioConfig.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(audioConfigMap[FTValue("channel")]);
        task.audioConfig.codecID = (EXPRESS::ZegoAudioCodecID)std::get<int32_t>(audioConfigMap[FTValue("codecID")]);
    }
    // Enable SoundLevel
    task.enableSoundLevel = std::get<bool>(argument[FTValue("enableSoundLevel")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startAutoMixerTask(task, [=](int errorCode, std::string extendedData){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("extendedData")] = FTValue(extendedData);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::stopAutoMixerTask(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoAutoMixerTask task;
    task.taskID = std::get<std::string>(argument[FTValue("taskID")]);
    task.roomID = std::get<std::string>(argument[FTValue("roomID")]);
    // MixerOutput
    auto outputList = std::get<FTArray>(argument[FTValue("outputList")]);
    if (outputList.size() > 0) {
        for (auto output_ : outputList) {
            FTMap outputMap = std::get<FTMap>(output_);
            EXPRESS::ZegoMixerOutput output;

            output.target = std::get<std::string>(outputMap[FTValue("target")]);
            if (outputMap.find(FTValue("videoConfig")) != outputMap.end()) {
                auto videoConfigMap = std::get<FTMap>(outputMap[FTValue("videoConfig")]);
                
                auto codecIDIndex = std::get<int32_t>(videoConfigMap[FTValue("videoCodecID")]);
                if (codecIDIndex > 3) {
                    codecIDIndex = EXPRESS::ZegoVideoCodecID::ZEGO_VIDEO_CODEC_ID_UNKNOWN;
                }
                output.videoConfig.videoCodecID = (EXPRESS::ZegoVideoCodecID)codecIDIndex;

                output.videoConfig.bitrate = std::get<int32_t>(videoConfigMap[FTValue("bitrate")]);
                output.videoConfig.encodeLatency = std::get<int32_t>(videoConfigMap[FTValue("encodeLatency")]);
                output.videoConfig.encodeProfile = (ZEGO::EXPRESS::ZegoEncodeProfile)std::get<int32_t>(videoConfigMap[FTValue("encodeProfile")]);
            }
            task.outputList.push_back(output);
        }
    }

    // no need to set audio config
    // no need to set video config
    // no need to set enable sound level

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->stopAutoMixerTask(task, [=](int errorCode){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::setAudioRouteToSpeaker(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getAudioRouteType(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::useFrontCamera(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::isCameraFocusSupported(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraFocusPointInPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposurePointInPreview(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraExposureCompensation(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::setCameraZoomFactor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::getCameraMaxZoomFactor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startAudioSpectrumMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startAudioSpectrumMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopAudioSpectrumMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopAudioSpectrumMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableHeadphoneAEC(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::startEffectsEnv(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->startEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopEffectsEnv(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopEffectsEnv();

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableEffectsBeauty(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableEffectsBeauty(enable);

    result->Success();
}

void ZegoExpressEngineMethodHandler::setEffectsBeautyParam(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoEffectsBeautyParam param;
    param.rosyIntensity = std::get<int32_t>(paramMap[FTValue("rosyIntensity")]);
    param.sharpenIntensity = std::get<int32_t>(paramMap[FTValue("sharpenIntensity")]);
    param.smoothIntensity = std::get<int32_t>(paramMap[FTValue("smoothIntensity")]);
    param.whitenIntensity = std::get<int32_t>(paramMap[FTValue("whitenIntensity")]);

    EXPRESS::ZegoExpressSDK::getEngine()->setEffectsBeautyParam(param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableBeautify(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->Error("Not recommended", "Not recommended, please use startEffectsEnv instead");
}

void ZegoExpressEngineMethodHandler::setBeautifyOption(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->Error("Not recommended", "Not recommended, please use startEffectsEnv and setEffectsBeautyParam instead");
}


void ZegoExpressEngineMethodHandler::sendBroadcastMessage(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto message = std::get<std::string>(argument[FTValue("message")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendBroadcastMessage(roomID, message, [=](int errorCode, unsigned long long messageID){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("messageID")] = FTValue((int64_t)messageID);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::sendBarrageMessage(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto message = std::get<std::string>(argument[FTValue("message")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendBarrageMessage(roomID, message, [=](int errorCode, std::string messageID){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("messageID")] = FTValue(messageID);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::sendCustomCommand(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);
    auto command = std::get<std::string>(argument[FTValue("command")]);
    auto toUserListArray = std::get<FTArray>(argument[FTValue("toUserList")]);
    std::vector<ZEGO::EXPRESS::ZegoUser> toUserList;
    for (auto user_ : toUserListArray) {
        ZEGO::EXPRESS::ZegoUser user;
        auto userMap = std::get<FTMap>(user_);
        user.userID = std::get<std::string>(userMap[FTValue("userID")]);
        user.userName = std::get<std::string>(userMap[FTValue("userName")]);
        toUserList.push_back(user);
    }

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomCommand(roomID, command, toUserList, [=](int errorCode){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::enableCustomVideoCapture(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomVideoCaptureConfig config;
    if (configMap.size() > 0) {
        config.bufferType = (EXPRESS::ZegoVideoBufferType)std::get<int32_t>(configMap[FTValue("bufferType")]);
    }

    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomVideoCapture(enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessing(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableAlignedAudioAuxData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    result->NotImplemented();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioRemoteProcessing(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioRemoteProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioPlaybackProcessing(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioProcessConfig config;
    config.samples = std::get<int32_t>(configMap[FTValue("samples")]);
    config.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(configMap[FTValue("sampleRate")]);
    config.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(configMap[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioPlaybackProcessing(enable, &config);

    result->Success();
}

void ZegoExpressEngineMethodHandler::enableCustomAudioIO(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto enable = std::get<bool>(argument[FTValue("enable")]);
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoCustomAudioConfig config;
    config.sourceType = (EXPRESS::ZegoAudioSourceType)std::get<int32_t>(argument[FTValue("sourceType")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->enableCustomAudioIO(enable, &config, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCaptureAACData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto configLength = argument[FTValue("configLength")].LongValue();
    auto referenceTimeMillisecond = argument[FTValue("configLength")].LongValue();
    auto samples = argument[FTValue("samples")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCaptureAACData(byteData.data(), dataLength, configLength, referenceTimeMillisecond, samples, param, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::sendCustomAudioCapturePCMData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);
    auto channel = std::get<int32_t>(argument[FTValue("channel")]);

    EXPRESS::ZegoExpressSDK::getEngine()->sendCustomAudioCapturePCMData(byteData.data(), dataLength, param, (EXPRESS::ZegoPublishChannel)channel);

    result->Success();
}

void ZegoExpressEngineMethodHandler::fetchCustomAudioRenderPCMData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
    auto dataLength = argument[FTValue("dataLength")].LongValue();
    auto paramMap = std::get<FTMap>(argument[FTValue("param")]);
    EXPRESS::ZegoAudioFrameParam param;
    param.channel = (EXPRESS::ZegoAudioChannel)std::get<int32_t>(paramMap[FTValue("channel")]);
    param.sampleRate = (EXPRESS::ZegoAudioSampleRate)std::get<int32_t>(paramMap[FTValue("sampleRate")]);

    EXPRESS::ZegoExpressSDK::getEngine()->fetchCustomAudioRenderPCMData(byteData.data(), dataLength, param);

    result->Success();
}

void ZegoExpressEngineMethodHandler::startPerformanceMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto millisecond = argument[FTValue("millisecond")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startPerformanceMonitor(millisecond);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopPerformanceMonitor(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopPerformanceMonitor();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkProbe(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkProbeConfig config;
    config.enableTraceroute = std::get<bool>(configMap[FTValue("enableTraceroute")]);

    auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkProbe(config, [=](int errorCode, const EXPRESS::ZegoNetworkProbeResult &result){
        FTMap retMap;
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        if (result.httpProbeResult) {
            FTMap httpProbeResultMap;
            httpProbeResultMap[FTValue("errorCode")] = FTValue(result.httpProbeResult->errorCode);
            httpProbeResultMap[FTValue("requestCostTime")] =
                FTValue((int64_t)result.httpProbeResult->requestCostTime);
            retMap[FTValue("httpProbeResult")] = FTValue(httpProbeResultMap);
        }
        if (result.tcpProbeResult) {
            FTMap tcpProbeResultMap;
            tcpProbeResultMap[FTValue("errorCode")] = FTValue(result.tcpProbeResult->errorCode);
            tcpProbeResultMap[FTValue("rtt")] = FTValue((int64_t)result.tcpProbeResult->rtt);
            tcpProbeResultMap[FTValue("connectCostTime")] =
                FTValue((int64_t)result.tcpProbeResult->connectCostTime);
            retMap[FTValue("tcpProbeResult")] = FTValue(tcpProbeResultMap);
        }
        if (result.udpProbeResult) {
            FTMap udpProbeResultMap;
            udpProbeResultMap[FTValue("errorCode")] = FTValue(result.udpProbeResult->errorCode);
            udpProbeResultMap[FTValue("rtt")] = FTValue((int64_t)result.udpProbeResult->rtt);
            retMap[FTValue("udpProbeResult")] = FTValue(udpProbeResultMap);
        }
        if (result.tracerouteResult) {
            FTMap tracerouteResultMap;
            tracerouteResultMap[FTValue("errorCode")] = FTValue(result.tracerouteResult->errorCode);
            tracerouteResultMap[FTValue("tracerouteCostTime")] = FTValue((int64_t)result.tracerouteResult->tracerouteCostTime);
            retMap[FTValue("tracerouteResult")] = FTValue(tracerouteResultMap);
        }
        sharedPtrResult->Success(retMap);
    });
}

void ZegoExpressEngineMethodHandler::stopNetworkProbe(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkProbe();

    result->Success();
}

void ZegoExpressEngineMethodHandler::startNetworkSpeedTest(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto configMap = std::get<FTMap>(argument[FTValue("config")]);
    EXPRESS::ZegoNetworkSpeedTestConfig config;
    config.testUplink = std::get<int32_t>(configMap[FTValue("testUplink")]);
    config.expectedUplinkBitrate = std::get<int32_t>(configMap[FTValue("expectedUplinkBitrate")]);
    config.testDownlink = std::get<int32_t>(configMap[FTValue("testDownlink")]);
    config.expectedDownlinkBitrate = std::get<int32_t>(configMap[FTValue("expectedDownlinkBitrate")]);
    auto interval = argument[FTValue("interval")].LongValue();

    EXPRESS::ZegoExpressSDK::getEngine()->startNetworkSpeedTest(config, interval);

    result->Success();
}

void ZegoExpressEngineMethodHandler::stopNetworkSpeedTest(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->stopNetworkSpeedTest();

    result->Success();
}

void ZegoExpressEngineMethodHandler::getNetworkTimeInfo(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->getNetworkTimeInfo();

    result->Success();
}

void ZegoExpressEngineMethodHandler::createRangeAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    rangeAudio_ = EXPRESS::ZegoExpressSDK::getEngine()->createRangeAudio();
    if (rangeAudio_) {
        rangeAudio_->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        result->Success(FTValue(0));
    } else {
        result->Success(FTValue(-1));
    }
}

void ZegoExpressEngineMethodHandler::destroyRangeAudio(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    EXPRESS::ZegoExpressSDK::getEngine()->destroyRangeAudio(rangeAudio_);
    rangeAudio_ = nullptr;
    result->Success();
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableMicrophone(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);

        rangeAudio_->enableMicrophone(enable);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioEnableMicrophone_Can_not_find_instance", "Invoke `rangeAudioEnableMicrophone` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpatializer(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);
        rangeAudio_->enableSpatializer(enable);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioEnableSpatializer_Can_not_find_instance", "Invoke `rangeAudioEnableSpatializer` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioEnableSpeaker(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto enable = std::get<bool>(argument[FTValue("enable")]);

        rangeAudio_->enableSpeaker(enable);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioEnableSpeaker_Can_not_find_instance", "Invoke `rangeAudioEnableSpeaker` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetAudioReceiveRange(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto range = std::get<double>(argument[FTValue("range")]);

        rangeAudio_->setAudioReceiveRange(range);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioSetAudioReceiveRange_Can_not_find_instance", "Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetMode(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto mode = std::get<int32_t>(argument[FTValue("mode")]);

        rangeAudio_->setRangeAudioMode((EXPRESS::ZegoRangeAudioMode)mode);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioSetMode_Can_not_find_instance", "Invoke `rangeAudioSetMode` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetTeamID(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto teamID = std::get<std::string>(argument[FTValue("teamID")]);

        rangeAudio_->setTeamID(teamID);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioSetTeamID_Can_not_find_instance", "Invoke `rangeAudioSetTeamID` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateAudioSource(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto userID = std::get<std::string>(argument[FTValue("userID")]);
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);

        rangeAudio_->updateAudioSource(userID, position.data());

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance", "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioUpdateSelfPosition(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto position = std::get<std::vector<float>>(argument[FTValue("position")]);
        auto axisForward = std::get<std::vector<float>>(argument[FTValue("axisForward")]);
        auto axisRight = std::get<std::vector<float>>(argument[FTValue("axisRight")]);
        auto axisUp = std::get<std::vector<float>>(argument[FTValue("axisUp")]);

        rangeAudio_->updateSelfPosition(position.data(), axisForward.data(), axisRight.data(), axisUp.data());

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioUpdateAudioSource_Can_not_find_instance", "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioMuteUser(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto userID = std::get<std::string>(argument[FTValue("userID")]);
        auto mute = std::get<bool>(argument[FTValue("mute")]);

        rangeAudio_->muteUser(userID, mute);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioMuteUser_Can_not_find_instance", "Invoke `rangeAudioMuteUser` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::rangeAudioSetPositionUpdateFrequency(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    if (rangeAudio_) {
        auto frequency = std::get<int32_t>(argument[FTValue("frequency")]);

        rangeAudio_->setPositionUpdateFrequency(frequency);

        result->Success();
    } 
    else 
    {
        result->Error("rangeAudioSetPositionUpdateFrequency_Can_not_find_instance", "Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::createRealTimeSequentialDataManager(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto roomID = std::get<std::string>(argument[FTValue("roomID")]);

    auto dataManager = EXPRESS::ZegoExpressSDK::getEngine()->createRealTimeSequentialDataManager(roomID);
    if (dataManager) {
        dataManager->setEventHandler(ZegoExpressEngineEventHandler::getInstance());
        dataManagerMap_[dataManager->getIndex()] = dataManager;
        result->Success(FTValue(dataManager->getIndex()) );
    } else {
        result->Success(-1);
    }
}

void ZegoExpressEngineMethodHandler::destroyRealTimeSequentialDataManager(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        EXPRESS::ZegoExpressSDK::getEngine()->destroyRealTimeSequentialDataManager(dataManagerMap_[index]);
        dataManagerMap_.erase(index);
    }

    result->Success();
}

void ZegoExpressEngineMethodHandler::dataManagerSendRealTimeSequentialData(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto byteData = std::get<std::vector<uint8_t>>(argument[FTValue("data")]);
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        auto sharedPtrResult = std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>>(std::move(result));
        dataManagerMap_[index]->sendRealTimeSequentialData(byteData.data(), byteData.size(), streamID, [=](int errorCode){
            FTMap retMap;
            retMap[FTValue("errorCode")] = FTValue(errorCode);
            sharedPtrResult->Success(retMap);
        });
    } else {
        result->Error("dataManagerSendRealTimeSequentialData_Can_not_find_instance", "Invoke `dataManagerSendRealTimeSequentialData` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartBroadcasting(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartBroadcasting_Can_not_find_instance", "Invoke `dataManagerStartBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStartSubscribing(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->startSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStartSubscribing_Can_not_find_instance", "Invoke `dataManagerStartSubscribing` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopBroadcasting(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopBroadcasting(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopBroadcasting_Can_not_find_instance", "Invoke `dataManagerStopBroadcasting` but can't find specific instance");
    }
}

void ZegoExpressEngineMethodHandler::dataManagerStopSubscribing(flutter::EncodableMap& argument,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
    auto index = std::get<int32_t>(argument[FTValue("index")]);
    if (dataManagerMap_.find(index) != dataManagerMap_.end()) {
        auto streamID = std::get<std::string>(argument[FTValue("streamID")]);

        dataManagerMap_[index]->stopSubscribing(streamID);

        result->Success();
    } else {
        result->Error("dataManagerStopSubscribing_Can_not_find_instance", "Invoke `dataManagerStopSubscribing` but can't find specific instance");
    }
}
