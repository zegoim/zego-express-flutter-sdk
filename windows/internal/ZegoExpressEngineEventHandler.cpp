#include "ZegoExpressEngineEventHandler.h"
#include "../ZegoLog.h"
#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoTextureRendererController.h"
#include <flutter/encodable_value.h>
#include <memory>

std::shared_ptr<ZegoExpressEngineEventHandler> ZegoExpressEngineEventHandler::m_instance = nullptr;

void ZegoExpressEngineEventHandler::setEventSink(
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&eventSink) {
    eventSink_ = std::move(eventSink);
}

void ZegoExpressEngineEventHandler::clearEventSink() { eventSink_.reset(); }

void ZegoExpressEngineEventHandler::onDebugError(int errorCode, const std::string &funcName,
                                                 const std::string &info) {

    ZF::logInfo("[onDebugError] errorCode: %d, funcName: %s, info: %s", errorCode, funcName.c_str(), info.c_str());
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onDebugError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("funcName")] = FTValue(funcName);
        retMap[FTValue("info")] = FTValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onApiCalledResult(int errorCode, const std::string &funcName,
                                                      const std::string &info) {

    ZF::logInfo("[onApiCalledResult] errorCode: %d, funcName: %s, info: %s", errorCode, funcName.c_str(), info.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onApiCalledResult");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("funcName")] = FTValue(funcName);
        retMap[FTValue("info")] = FTValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onFatalError(int errorCode) {
    
    ZF::logInfo("[onFatalError] errorCode: %d", errorCode);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onFatalError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

    ZF::logInfo("[onEngineStateUpdate] state: %d", state);

    if (eventSink_) {
        flutter::EncodableMap retMap;
        retMap[FTValue("method")] = FTValue("onEngineStateUpdate");
        retMap[FTValue("state")] = FTValue(state);

        eventSink_->Success(retMap);
    }

    if (state == EXPRESS::ZegoEngineState::ZEGO_ENGINE_STATE_STOP) {
        ZegoTextureRendererController::getInstance()->resetAllRenderFirstFrame();
    }
}

void ZegoExpressEngineEventHandler::onRoomStateUpdate(const std::string &roomID,
                                                      EXPRESS::ZegoRoomState state, int errorCode,
                                                      const std::string &extendedData) {

    ZF::logInfo("[onRoomStateUpdate] roomID: %s, state: %d, errorCode: %d, extendedData: %s", roomID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStateUpdate");
        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("roomID")] = FTValue(roomID);
        retMap[FTValue("extendedData")] = FTValue(extendedData);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStateChanged(const std::string &roomID,
                                                       EXPRESS::ZegoRoomStateChangedReason reason,
                                                       int errorCode,
                                                       const std::string &extendedData) {

    ZF::logInfo("[onRoomStateChanged] roomID: %s, reason: %d, errorCode: %d, extendedData: %s", roomID.c_str(), reason, errorCode, extendedData.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStateChanged");
        retMap[FTValue("reason")] = FTValue(reason);
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("roomID")] = FTValue(roomID);

        std::string extendedData_ = extendedData.empty() ? "{}" : extendedData;
        retMap[FTValue("extendedData")] = FTValue(extendedData_);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomUserUpdate(
    const std::string &roomID, EXPRESS::ZegoUpdateType updateType,
    const std::vector<EXPRESS::ZegoUser> &userList) {

    ZF::logInfo("[onRoomUserUpdate] roomID: %s, updateType: %d, userListCount: %d", roomID.c_str(), updateType, userList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomUserUpdate");
        retMap[FTValue("updateType")] = FTValue(updateType);
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray userListArray;
        for (auto &user : userList) {
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(user.userID);
            userMap[FTValue("userName")] = FTValue(user.userName);
            userListArray.emplace_back(FTValue(userMap));
        }
        retMap[FTValue("userList")] = FTValue(userListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomOnlineUserCountUpdate(const std::string &roomID,
                                                                int count) {

    ZF::logInfo("[onRoomOnlineUserCountUpdate] roomID: %s, count: %d", roomID.c_str(), count);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomOnlineUserCountUpdate");
        retMap[FTValue("count")] = FTValue(count);
        retMap[FTValue("roomID")] = FTValue(roomID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStreamUpdate(
    const std::string &roomID, EXPRESS::ZegoUpdateType updateType,
    const std::vector<EXPRESS::ZegoStream> &streamList, const std::string &extendedData) {

    ZF::logInfo("[onRoomStreamUpdate] roomID: %s, updateType: %d, streamListCount: %d, extendedData :%d", roomID.c_str(), updateType, streamList.size(), extendedData.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStreamUpdate");
        retMap[FTValue("updateType")] = FTValue(updateType);
        retMap[FTValue("roomID")] = FTValue(roomID);
        retMap[FTValue("extendedData")] = FTValue(extendedData);

        FTArray streamListArray;
        for (auto &stream : streamList) {
            FTMap streamMap;
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(stream.user.userID);
            userMap[FTValue("userName")] = FTValue(stream.user.userName);

            streamMap[FTValue("streamID")] = FTValue(stream.streamID);
            streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
            streamMap[FTValue("user")] = FTValue(userMap);

            streamListArray.emplace_back(FTValue(streamMap));
        }
        retMap[FTValue("streamList")] = FTValue(streamListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStreamExtraInfoUpdate(
    const std::string &roomID, const std::vector<EXPRESS::ZegoStream> &streamList) {

    ZF::logInfo("[onRoomStreamExtraInfoUpdate] roomID: %s, streamListCount: %d", roomID.c_str(), streamList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStreamExtraInfoUpdate");
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray streamListArray;
        for (auto &stream : streamList) {
            FTMap streamMap;
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(stream.user.userID);
            userMap[FTValue("userName")] = FTValue(stream.user.userName);

            streamMap[FTValue("streamID")] = FTValue(stream.streamID);
            streamMap[FTValue("extraInfo")] = FTValue(stream.extraInfo);
            streamMap[FTValue("user")] = FTValue(userMap);

            streamListArray.emplace_back(FTValue(streamMap));
        }
        retMap[FTValue("streamList")] = FTValue(streamListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomExtraInfoUpdate(
    const std::string &roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo> &roomExtraInfoList) {

    ZF::logInfo("[onRoomExtraInfoUpdate] roomID: %s, streamListCount: %d", roomID.c_str(), roomExtraInfoList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomExtraInfoUpdate");
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray roomExtraInfoListArray;
        for (auto &roomExtraInfo : roomExtraInfoList) {
            FTMap infoMap;
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(roomExtraInfo.updateUser.userID);
            userMap[FTValue("userName")] = FTValue(roomExtraInfo.updateUser.userName);

            infoMap[FTValue("key")] = FTValue(roomExtraInfo.key);
            infoMap[FTValue("value")] = FTValue(roomExtraInfo.value);
            infoMap[FTValue("updateTime")] = FTValue((int64_t)roomExtraInfo.updateTime);
            infoMap[FTValue("updateUser")] = FTValue(userMap);

            roomExtraInfoListArray.emplace_back(FTValue(infoMap));
        }
        retMap[FTValue("roomExtraInfoList")] = flutter::EncodableValue(roomExtraInfoListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherStateUpdate(const std::string &streamID,
                                                           EXPRESS::ZegoPublisherState state,
                                                           int errorCode,
                                                           const std::string &extendedData) {
    ZF::logInfo("[onPublisherStateUpdate] streamID: %s, state: %d, errorCode: %d, extendedData: %s", streamID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherStateUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("state")] = flutter::EncodableValue(state);
        retMap[FTValue("errorCode")] = flutter::EncodableValue(errorCode);
        retMap[FTValue("extendedData")] = flutter::EncodableValue(extendedData);

        eventSink_->Success(retMap);
    }
}

flutter::EncodableMap ZegoExpressEngineEventHandler::convertPublishQuality(const EXPRESS::ZegoPublishStreamQuality &quality) {
    FTMap qualityMap;
    qualityMap[FTValue("videoCaptureFPS")] = FTValue(quality.videoCaptureFPS);
    qualityMap[FTValue("videoEncodeFPS")] = FTValue(quality.videoEncodeFPS);
    qualityMap[FTValue("videoSendFPS")] = FTValue(quality.videoSendFPS);
    qualityMap[FTValue("videoKBPS")] = FTValue(quality.videoKBPS);
    qualityMap[FTValue("audioCaptureFPS")] = FTValue(quality.audioCaptureFPS);
    qualityMap[FTValue("audioSendFPS")] = FTValue(quality.audioSendFPS);
    qualityMap[FTValue("audioKBPS")] = FTValue(quality.audioKBPS);
    qualityMap[FTValue("rtt")] = FTValue(quality.rtt);
    qualityMap[FTValue("packetLostRate")] = FTValue(quality.packetLostRate);
    qualityMap[FTValue("level")] = FTValue(quality.level);
    qualityMap[FTValue("isHardwareEncode")] = FTValue(quality.isHardwareEncode);
    qualityMap[FTValue("videoCodecID")] = FTValue(quality.videoCodecID);
    qualityMap[FTValue("totalSendBytes")] = FTValue(quality.totalSendBytes);
    qualityMap[FTValue("audioSendBytes")] = FTValue(quality.audioSendBytes);
    qualityMap[FTValue("videoSendBytes")] = FTValue(quality.videoSendBytes);

    return qualityMap;
}

void ZegoExpressEngineEventHandler::onPublisherQualityUpdate(
    const std::string &streamID, const EXPRESS::ZegoPublishStreamQuality &quality) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherQualityUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);

        FTMap qualityMap = convertPublishQuality(quality);

        retMap[FTValue("quality")] = FTValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedAudioFirstFrame() {

    ZF::logInfo("[onPublisherCapturedAudioFirstFrame]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherCapturedAudioFirstFrame");

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherSendAudioFirstFrame(EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherSendAudioFirstFrame] channel: %d", channel);
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherSendAudioFirstFrame");
        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherStreamEvent(EXPRESS::ZegoStreamEvent eventID,
                                                           const std::string &streamID,
                                                           const std::string &extraInfo) {

    ZF::logInfo("[onPublisherStreamEvent] eventID: %d, streamID: %s, extraInfo: %s", eventID, streamID.c_str(), extraInfo.c_str());

    if (eventSink_) {
        FTMap retMap;

        retMap[FTValue("method")] = FTValue("onPublisherStreamEvent");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("eventID")] = flutter::EncodableValue((int32_t)eventID);
        retMap[FTValue("streamID")] = flutter::EncodableValue(streamID);
        retMap[FTValue("extraInfo")] = flutter::EncodableValue(extraInfo);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onVideoObjectSegmentationStateChanged(
    EXPRESS::ZegoObjectSegmentationState state, EXPRESS::ZegoPublishChannel channel,
    int errorCode) {

    ZF::logInfo("[onVideoObjectSegmentationStateChanged] state: %d, channel: %d", state, channel);

    if (eventSink_) {
        FTMap retMap;

        retMap[FTValue("method")] = FTValue("onVideoObjectSegmentationStateChanged");
        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("channel")] = FTValue((int32_t)channel);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID, EXPRESS::ZegoPublishChannel channel) {
    ZF::logInfo("[onPublisherLowFpsWarning] codecID: %d, channel: %d", codecID, channel);

    if (eventSink_) {
        FTMap retMap;

        retMap[FTValue("method")] = FTValue("onPublisherLowFpsWarning");
        retMap[FTValue("codecID")] = FTValue((int32_t)codecID);
        retMap[FTValue("channel")] = FTValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherDummyCaptureImagePathError(int errorCode, const std::string& path, EXPRESS::ZegoPublishChannel channel) {
    ZF::logInfo("[onPublisherDummyCaptureImagePathError] errorCode: %d, path: %s, channel: %d", errorCode, path.c_str(), channel);

    if (eventSink_) {
        FTMap retMap;

        retMap[FTValue("method")] = FTValue("onPublisherDummyCaptureImagePathError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("path")] = FTValue(path);
        retMap[FTValue("channel")] = FTValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}


void ZegoExpressEngineEventHandler::onPlayerStateUpdate(const std::string &streamID,
                                                        EXPRESS::ZegoPlayerState state,
                                                        int errorCode,
                                                        const std::string &extendedData) {

    ZF::logInfo("[onPlayerStateUpdate] streamID: %s, state: %d, errorCode: %d, extendedData: %s", streamID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerStateUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("state")] = flutter::EncodableValue(state);
        retMap[FTValue("errorCode")] = flutter::EncodableValue(errorCode);
        retMap[FTValue("extendedData")] = flutter::EncodableValue(extendedData);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerQualityUpdate(
    const std::string &streamID, const EXPRESS::ZegoPlayStreamQuality &quality) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerQualityUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);

        FTMap qualityMap;
        qualityMap[FTValue("videoRecvFPS")] = FTValue(quality.videoRecvFPS);
        qualityMap[FTValue("videoDejitterFPS")] = FTValue(quality.videoDejitterFPS);
        qualityMap[FTValue("videoDecodeFPS")] = FTValue(quality.videoDecodeFPS);
        qualityMap[FTValue("videoRenderFPS")] = FTValue(quality.videoRenderFPS);
        qualityMap[FTValue("videoKBPS")] = FTValue(quality.videoKBPS);
        qualityMap[FTValue("videoBreakRate")] = FTValue(quality.videoBreakRate);
        qualityMap[FTValue("audioRecvFPS")] = FTValue(quality.audioRecvFPS);
        qualityMap[FTValue("audioDejitterFPS")] = FTValue(quality.audioDejitterFPS);
        qualityMap[FTValue("audioDecodeFPS")] = FTValue(quality.audioDecodeFPS);
        qualityMap[FTValue("audioRenderFPS")] = FTValue(quality.audioRenderFPS);
        qualityMap[FTValue("audioKBPS")] = FTValue(quality.audioKBPS);
        qualityMap[FTValue("audioBreakRate")] = FTValue(quality.audioBreakRate);
        qualityMap[FTValue("mos")] = FTValue(quality.mos);
        qualityMap[FTValue("rtt")] = FTValue(quality.rtt);
        qualityMap[FTValue("packetLostRate")] = FTValue(quality.packetLostRate);
        qualityMap[FTValue("peerToPeerDelay")] = FTValue(quality.peerToPeerDelay);
        qualityMap[FTValue("peerToPeerPacketLostRate")] = FTValue(quality.peerToPeerPacketLostRate);
        qualityMap[FTValue("level")] = FTValue(quality.level);
        qualityMap[FTValue("delay")] = FTValue(quality.delay);
        qualityMap[FTValue("avTimestampDiff")] = FTValue(quality.avTimestampDiff);
        qualityMap[FTValue("isHardwareDecode")] = FTValue(quality.isHardwareDecode);
        qualityMap[FTValue("videoCodecID")] = FTValue(quality.videoCodecID);
        qualityMap[FTValue("totalRecvBytes")] = FTValue(quality.totalRecvBytes);
        qualityMap[FTValue("audioRecvBytes")] = FTValue(quality.audioRecvBytes);
        qualityMap[FTValue("videoRecvBytes")] = FTValue(quality.videoRecvBytes);
        qualityMap[FTValue("audioCumulativeBreakCount")] = FTValue((int32_t)quality.audioCumulativeBreakCount);
        qualityMap[FTValue("videoCumulativeBreakCount")] = FTValue((int32_t)quality.videoCumulativeBreakCount);
        qualityMap[FTValue("audioCumulativeBreakTime")] = FTValue((int32_t)quality.audioCumulativeBreakTime);
        qualityMap[FTValue("videoCumulativeBreakTime")] = FTValue((int32_t)quality.videoCumulativeBreakTime);
        qualityMap[FTValue("audioCumulativeBreakRate")] = FTValue(quality.audioCumulativeBreakRate);
        qualityMap[FTValue("videoCumulativeBreakRate")] = FTValue(quality.videoCumulativeBreakRate);
        qualityMap[FTValue("audioCumulativeDecodeTime")] = FTValue((int32_t)quality.audioCumulativeDecodeTime);
        qualityMap[FTValue("videoCumulativeDecodeTime")] = FTValue((int32_t)quality.videoCumulativeDecodeTime);
        qualityMap[FTValue("muteVideo")] = FTValue(quality.muteVideo);
        qualityMap[FTValue("muteAudio")] = FTValue(quality.muteAudio);

        retMap[FTValue("quality")] = FTValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerMediaEvent(const std::string &streamID,
                                                       EXPRESS::ZegoPlayerMediaEvent event) {

    ZF::logInfo("[onPlayerMediaEvent] streamID: %s, event: %d", streamID.c_str(), event);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerMediaEvent");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("event")] = flutter::EncodableValue(event);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRecvAudioFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvAudioFirstFrame");
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvSEI(const std::string &streamID,
                                                    const unsigned char *data,
                                                    unsigned int dataLength) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvSEI");
        retMap[FTValue("streamID")] = FTValue(streamID);

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvMediaSideInfo(const EXPRESS::ZegoMediaSideInfo & info) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvMediaSideInfo");
        retMap[FTValue("streamID")] = FTValue(info.streamID);
        retMap[FTValue("timestampNs")] = FTValue(info.timestampNs);

        auto nonConstData = const_cast<unsigned char *>(info.SEIData);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + info.SEIDataLength);

        retMap[FTValue("SEIData")] = FTValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioSideInfo(const std::string &streamID,
                                                              const unsigned char *data,
                                                              unsigned int dataLength) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvAudioSideInfo");
        retMap[FTValue("streamID")] = FTValue(streamID);

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerStreamEvent(EXPRESS::ZegoStreamEvent eventID,
                                                        const std::string &streamID,
                                                        const std::string &extraInfo) {

    ZF::logInfo("[onPlayerStreamEvent] eventID: %d, streamID: %s, extraInfo: %s", eventID, streamID.c_str(), extraInfo.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerStreamEvent");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("eventID")] = FTValue((int32_t)eventID);
        retMap[FTValue("extraInfo")] = FTValue(extraInfo);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderCameraVideoFirstFrame(
    const std::string &streamID) {

    ZF::logInfo("[onPlayerRenderCameraVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRenderCameraVideoFirstFrame");
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

// void ZegoExpressEngineEventHandler::onPlayerVideoSuperResolutionUpdate(std::string streamID,EXPRESS::ZegoSuperResolutionState state,int errorCode) {
//     if (eventSink_) {
//         FTMap retMap;
//         retMap[FTValue("method")] = FTValue("onPlayerVideoSuperResolutionUpdate");
//         retMap[FTValue("streamID")] = FTValue(streamID);
//         retMap[FTValue("state")] = FTValue((int32_t)state);
//         retMap[FTValue("errorCode")] = FTValue(errorCode);

//         eventSink_->Success(retMap);
//     }
// }

void ZegoExpressEngineEventHandler::onMixerRelayCDNStateUpdate(
    const std::string &taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> &infoList) {

    ZF::logInfo("[onMixerRelayCDNStateUpdate] taskID: %s, infoListCount: %d", taskID.c_str(), infoList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerRelayCDNStateUpdate");
        retMap[FTValue("taskID")] = FTValue(taskID);

        FTArray infoListArray;
        for (auto &info : infoList) {
            FTMap infoMap;
            infoMap[FTValue("url")] = FTValue(info.url);
            infoMap[FTValue("state")] = FTValue((int32_t)info.state);
            infoMap[FTValue("updateReason")] = FTValue((int32_t)info.updateReason);
            infoMap[FTValue("stateTime")] = FTValue((int32_t)info.stateTime);
            infoListArray.emplace_back(FTValue(infoMap));
        }
        retMap[FTValue("infoList")] = FTValue(infoListArray);
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixerSoundLevelUpdate(
    const std::unordered_map<unsigned int, float> &soundLevels) {

    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto &soundlevel : soundLevels) {
            soundLevelsMap[FTValue((int32_t)soundlevel.first)] = FTValue(soundlevel.second);
        }
        retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceStateChanged(
    EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType,
    const EXPRESS::ZegoDeviceInfo &deviceInfo) {

    ZF::logInfo("[onAudioDeviceStateChanged] updateType: %d, deviceType: %d, deviceID: %s, deviceName: %s", updateType, deviceType, deviceInfo.deviceID.c_str(), deviceInfo.deviceName.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAudioDeviceStateChanged");

        retMap[FTValue("updateType")] = FTValue((int32_t)updateType);
        retMap[FTValue("deviceType")] = FTValue((int32_t)deviceType);

        FTMap deviceInfoMap;
        deviceInfoMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceInfoMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);

        retMap[FTValue("deviceInfo")] = FTValue(deviceInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceVolumeChanged(
    EXPRESS::ZegoAudioDeviceType deviceType, const std::string &deviceID, int volume) {

    ZF::logInfo("[onAudioDeviceVolumeChanged] deviceType: %d, deviceID: %s, volume: %d", deviceType, deviceID.c_str(), volume);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAudioDeviceVolumeChanged");

        retMap[FTValue("deviceType")] = FTValue((int32_t)deviceType);

        retMap[FTValue("deviceID")] = FTValue(deviceID);
        retMap[FTValue("volume")] = FTValue(volume);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedSoundLevelUpdate(float soundLevel) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCapturedSoundLevelUpdate");
        retMap[FTValue("soundLevel")] = FTValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelUpdate(
    const std::unordered_map<std::string, float> &soundLevels) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto &soundlevel : soundLevels) {
            soundLevelsMap[FTValue(soundlevel.first)] = FTValue(soundlevel.second);
        }
        retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

/*void ZegoExpressEngineEventHandler::onCapturedAudioSpectrumUpdate(const EXPRESS::ZegoAudioSpectrum& audioSpectrum) {

}

void ZegoExpressEngineEventHandler::onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum>& audioSpectrums) {

}*/

void ZegoExpressEngineEventHandler::onRemoteMicStateUpdate(const std::string &streamID,
                                                           EXPRESS::ZegoRemoteDeviceState state) {
    ZF::logInfo("[onRemoteMicStateUpdate] streamID: %s, state: %d", streamID.c_str(), state);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteMicStateUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("state")] = FTValue(state);

        eventSink_->Success(retMap);
    }
}

// Audio Effect Player
void ZegoExpressEngineEventHandler::onAudioEffectPlayStateUpdate(
    EXPRESS::IZegoAudioEffectPlayer *audioEffectPlayer, unsigned int audioEffectID,
    EXPRESS::ZegoAudioEffectPlayState state, int errorCode) {
    ZF::logInfo("[onAudioEffectPlayStateUpdate] index: %d, audioEffectID: %d, state: %d, errorCode:%d", audioEffectPlayer->getIndex(), audioEffectID, state, errorCode);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAudioEffectPlayStateUpdate");
        retMap[FTValue("audioEffectPlayerIndex")] = FTValue(audioEffectPlayer->getIndex());
        retMap[FTValue("audioEffectID")] = FTValue((int32_t)audioEffectID);
        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerStateUpdate(EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                             EXPRESS::ZegoMediaPlayerState state,
                                                             int errorCode) {
    ZF::logInfo("[onMediaPlayerStateUpdate] index: %d, state: %d, errorCode:%d", mediaPlayer->getIndex(), state, errorCode);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerStateUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }

    if (state == EXPRESS::ZegoMediaPlayerState::ZEGO_MEDIA_PLAYER_STATE_NO_PLAY || state == EXPRESS::ZegoMediaPlayerState::ZEGO_MEDIA_PLAYER_STATE_PLAY_ENDED) {
        ZegoTextureRendererController::getInstance()->resetMediaPlayerRenderFirstFrame(mediaPlayer);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerNetworkEvent(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, EXPRESS::ZegoMediaPlayerNetworkEvent networkEvent) {

    ZF::logInfo("[onMediaPlayerNetworkEvent] index: %d, networkEvent: %d", mediaPlayer->getIndex(), networkEvent);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerNetworkEvent");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        retMap[FTValue("networkEvent")] = FTValue(networkEvent);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerPlayingProgress(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, unsigned long long millisecond) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerPlayingProgress");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        // TODO: convert need test?
        retMap[FTValue("millisecond")] = FTValue((int64_t)millisecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerRecvSEI(EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                         const unsigned char *data,
                                                         unsigned int dataLength) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerRecvSEI");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        // TODO: convert need test?
        std::vector<uint8_t> vec_data(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(vec_data);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerSoundLevelUpdate(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, float soundLevel) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerSoundLevelUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("soundLevel")] = FTValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerFrequencySpectrumUpdate(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, const EXPRESS::ZegoAudioSpectrum &spectrumList) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerFrequencySpectrumUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("spectrumList")] = FTValue(spectrumList);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerFirstFrameEvent(
    EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerFirstFrameEvent event) {

    ZF::logInfo("[onMediaPlayerFirstFrameEvent] index: %d, event: %d", mediaPlayer->getIndex(), event);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerFirstFrameEvent");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("event")] = FTValue(event);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerRenderingProgress(EXPRESS::IZegoMediaPlayer* mediaPlayer, unsigned long long millisecond) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerRenderingProgress");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("millisecond")] = FTValue((int64_t)millisecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerVideoSizeChanged(EXPRESS::IZegoMediaPlayer* mediaPlayer, int width, int height) {
    ZF::logInfo("[onMediaPlayerVideoSizeChanged] index: %d, width: %d, height: %d", mediaPlayer->getIndex(), width, height);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerVideoSizeChanged");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("width")] = FTValue(width);
        retMap[FTValue("height")] = FTValue(height);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerLocalCache(EXPRESS::IZegoMediaPlayer * mediaPlayer, int errorCode, const std::string &resource, const std::string & cachedFile) {
    ZF::logInfo("[onMediaPlayerLocalCache] index: %d, error: %d, resource: %s, cachedFile: %s", mediaPlayer->getIndex(), errorCode, resource, cachedFile);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerLocalCache");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("resource")] = FTValue(resource);
        retMap[FTValue("cachedFile")] = FTValue(cachedFile);

        eventSink_->Success(retMap);
    }
}

// MediaDataPublisher
void ZegoExpressEngineEventHandler::onMediaDataPublisherFileOpen(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileOpen] index: %d, path: %s", mediaDataPublisher->getIndex(), path.c_str());

    if (eventSink_) {
        FTMap return_map;
        return_map[FTValue("method")] = FTValue("onMediaDataPublisherFileOpen");
        return_map[FTValue("publisherIndex")] = FTValue(mediaDataPublisher->getIndex());
        return_map[FTValue("path")] = FTValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onMediaDataPublisherFileClose(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, int errorCode, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileClose] index: %d, errorCode: %d, path: %s", mediaDataPublisher->getIndex(), errorCode, path.c_str());

    if (eventSink_) {
        FTMap return_map;
        return_map[FTValue("method")] = FTValue("onMediaDataPublisherFileClose");
        return_map[FTValue("publisherIndex")] = FTValue(mediaDataPublisher->getIndex());
        return_map[FTValue("errorCode")] = FTValue(errorCode);
        return_map[FTValue("path")] = FTValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onMediaDataPublisherFileDataBegin(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileDataBegin] index: %d, path: %s", mediaDataPublisher->getIndex(), path.c_str());

    if (eventSink_) {
        FTMap return_map;
        return_map[FTValue("method")] = FTValue("onMediaDataPublisherFileDataBegin");
        return_map[FTValue("publisherIndex")] = FTValue(mediaDataPublisher->getIndex());
        return_map[FTValue("path")] = FTValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioData(const unsigned char *data,
                                                        unsigned int dataLength,
                                                        EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCapturedAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataVec);
        retMap[FTValue("dataLength")] = FTValue((int)dataLength);
        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param.sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param.channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlaybackAudioData(const unsigned char *data,
                                                        unsigned int dataLength,
                                                        EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlaybackAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataVec);
        retMap[FTValue("dataLength")] = FTValue((int)dataLength);
        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param.sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param.channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixedAudioData(const unsigned char *data,
                                                     unsigned int dataLength,
                                                     EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixedAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataVec);
        retMap[FTValue("dataLength")] = FTValue((int)dataLength);
        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param.sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param.channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerAudioData(const unsigned char *data,
                                                      unsigned int dataLength,
                                                      EXPRESS::ZegoAudioFrameParam param,
                                                      const std::string &streamID) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataVec);
        retMap[FTValue("dataLength")] = FTValue((int)dataLength);
        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param.sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param.channel);
        retMap[FTValue("param")] = FTValue(paramMap);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordStateUpdate(
    EXPRESS::ZegoDataRecordState state, int errorCode, EXPRESS::ZegoDataRecordConfig config,
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onCapturedDataRecordStateUpdate] state: %d, errorCode: %d, filePath: %s, recordType: %d, channel: %d", state, errorCode, config.filePath, config.recordType, channel);

    if (eventSink_) {
        FTMap retMap;
        FTMap configMap;
        retMap[FTValue("method")] = FTValue("onCapturedDataRecordStateUpdate");

        configMap[FTValue("filePath")] = FTValue(config.filePath);
        configMap[FTValue("recordType")] = FTValue(config.recordType);

        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("config")] = FTValue(configMap);
        retMap[FTValue("channel")] = FTValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordProgressUpdate(
    EXPRESS::ZegoDataRecordProgress progress, EXPRESS::ZegoDataRecordConfig config,
    EXPRESS::ZegoPublishChannel channel) {
        
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        FTMap progressMap;
        FTMap configMap;
        retMap[FTValue("method")] = FTValue("onCapturedDataRecordProgressUpdate");

        progressMap[FTValue("currentFileSize")] = FTValue((int32_t)progress.currentFileSize);
        progressMap[FTValue("duration")] = FTValue((int32_t)progress.duration);
        progressMap[FTValue("quality")] = FTValue(convertPublishQuality(progress.quality));

        configMap[FTValue("filePath")] = FTValue(config.filePath);
        configMap[FTValue("recordType")] = FTValue(config.recordType);

        retMap[FTValue("config")] = FTValue(configMap);
        retMap[FTValue("progress")] = FTValue(progressMap);
        retMap[FTValue("channel")] = FTValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onDownloadProgressUpdate(
    EXPRESS::IZegoCopyrightedMusic *copyrightedMusic, const std::string &resourceID,
    float progressRate) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onDownloadProgressUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("progressRate")] = FTValue(progressRate);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCurrentPitchValueUpdate(
    EXPRESS::IZegoCopyrightedMusic *copyrightedMusic, const std::string &resourceID,
    int currentDuration, int pitchValue) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCurrentPitchValueUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("currentDuration")] = FTValue(currentDuration);
        retMap[FTValue("pitchValue")] = FTValue(pitchValue);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkTimeSynchronized() {

    ZF::logInfo("[onNetworkTimeSynchronized]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkTimeSynchronized");
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRequestDumpData() {
    ZF::logInfo("[onRequestDumpData]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRequestDumpData");
        
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRequestUploadDumpData(const std::string &dumpDir, bool takePhoto) {
    ZF::logInfo("[onRequestUploadDumpData]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRequestUploadDumpData");
        retMap[FTValue("dumpDir")] = FTValue(dumpDir);
        retMap[FTValue("takePhoto")] = FTValue(takePhoto);
        
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onStartDumpData(int errorCode) {
    ZF::logInfo("[onStartDumpData]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onStartDumpData");
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onStopDumpData(int errorCode, const std::string& dumpDir) {
     ZF::logInfo("[onStopDumpData]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onStopDumpData");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("dumpDir")] = FTValue(dumpDir);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUploadDumpData(int errorCode) {
     ZF::logInfo("[onUploadDumpData]");

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onUploadDumpData");
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomTokenWillExpire(const std::string &roomID,
                                                          int remainTimeInSecond) {

    ZF::logInfo("[onRoomTokenWillExpire] roomID: %s, remainTimeInSecond: %d", roomID.c_str(), remainTimeInSecond);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomTokenWillExpire");

        retMap[FTValue("roomID")] = FTValue(roomID);
        retMap[FTValue("remainTimeInSecond")] = FTValue(remainTimeInSecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherCapturedVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherCapturedVideoFirstFrame");

        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherSendVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherSendVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherSendVideoFirstFrame");

        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRenderVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherRenderVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherRenderVideoFirstFrame");

        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherVideoSizeChanged(
    int width, int height, EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherVideoSizeChanged] width: %d, height: %d, channel: %d", width, height, channel);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherVideoSizeChanged");

        retMap[FTValue("width")] = FTValue(width);
        retMap[FTValue("height")] = FTValue(height);
        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRelayCDNStateUpdate(
    const std::string &streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> &infoList) {

    ZF::logInfo("[onPublisherRelayCDNStateUpdate] streamID: %s, infoListCount: %d", streamID.c_str(), infoList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherRelayCDNStateUpdate");

        retMap[FTValue("streamID")] = FTValue(streamID);
        FTArray infoListArray;
        for (auto info : infoList) {
            FTMap infoMap;
            infoMap[FTValue("url")] = FTValue(info.url);
            infoMap[FTValue("state")] = FTValue((int32_t)info.state);
            infoMap[FTValue("updateReason")] = FTValue((int32_t)info.updateReason);
            infoMap[FTValue("stateTime")] = FTValue((int64_t)info.stateTime);
            infoListArray.push_back(FTValue(infoMap));
        }
        retMap[FTValue("streamInfoList")] = FTValue(infoListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherVideoEncoderChanged(
    EXPRESS::ZegoVideoCodecID fromCodecID, EXPRESS::ZegoVideoCodecID toCodecID,
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherVideoEncoderChanged] fromCodecID: %d, toCodecID: %d, channel: %d", fromCodecID, toCodecID, channel);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherVideoEncoderChanged");

        retMap[FTValue("fromCodecID")] = FTValue((int32_t)fromCodecID);
        retMap[FTValue("toCodecID")] = FTValue((int32_t)toCodecID);
        retMap[FTValue("channel")] = FTValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvVideoFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRecvVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvVideoFirstFrame");

        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderVideoFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRenderVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRenderVideoFirstFrame");

        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerVideoSizeChanged(const std::string &streamID, int width,
                                                             int height) {

    ZF::logInfo("[onPlayerVideoSizeChanged] streamID: %s, width: %d, height: %d", streamID.c_str(), width, height);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerVideoSizeChanged");

        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("width")] = FTValue(width);
        retMap[FTValue("height")] = FTValue(height);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID,
                                                          const std::string &streamID) {

    ZF::logInfo("[onPlayerLowFpsWarning] streamID: %s, codecID: %d", streamID.c_str(), codecID);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerLowFpsWarning");

        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("codecID")] = FTValue((int32_t)codecID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAutoMixerSoundLevelUpdate(
    const std::unordered_map<std::string, float> &soundLevels) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAutoMixerSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto soundLevel : soundLevels) {
            soundLevelsMap[FTValue(soundLevel.first)] = FTValue((double)soundLevel.second);
        }
        retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onVideoDeviceStateChanged(
    EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo &deviceInfo) {

    ZF::logInfo("[onVideoDeviceStateChanged] updateType: %d, deviceID: %s, deviceName: %s", updateType, deviceInfo.deviceID.c_str(), deviceInfo.deviceName.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onVideoDeviceStateChanged");

        retMap[FTValue("updateType")] = FTValue((int32_t)updateType);
        FTMap deviceInfoMap;
        deviceInfoMap[FTValue("deviceID")] = FTValue(deviceInfo.deviceID);
        deviceInfoMap[FTValue("deviceName")] = FTValue(deviceInfo.deviceName);
        retMap[FTValue("deviceInfo")] = FTValue(deviceInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedSoundLevelInfoUpdate(
    const EXPRESS::ZegoSoundLevelInfo &soundLevelInfo) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCapturedSoundLevelInfoUpdate");

        FTMap soundLevelInfoMap;
        soundLevelInfoMap[FTValue("soundLevel")] = FTValue((double)soundLevelInfo.soundLevel);
        soundLevelInfoMap[FTValue("vad")] = FTValue(soundLevelInfo.vad);
        retMap[FTValue("soundLevelInfo")] = FTValue(soundLevelInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelInfoUpdate(
    const std::unordered_map<std::string, EXPRESS::ZegoSoundLevelInfo> &soundLevelInfos) {

    // High frequency callbacks do not log
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSoundLevelInfoUpdate");

        FTMap soundLevelInfosMap;
        for (auto soundLevelInfo : soundLevelInfos) {
            FTMap soundLevelInfoMap;
            soundLevelInfoMap[FTValue("soundLevel")] =
                FTValue((double)soundLevelInfo.second.soundLevel);
            soundLevelInfoMap[FTValue("vad")] = FTValue(soundLevelInfo.second.vad);
            soundLevelInfosMap[FTValue(soundLevelInfo.first)] = FTValue(soundLevelInfoMap);
        }

        retMap[FTValue("soundLevelInfos")] = FTValue(soundLevelInfosMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioSpectrumUpdate(
    const EXPRESS::ZegoAudioSpectrum &audioSpectrum) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCapturedAudioSpectrumUpdate");

        FTArray audioSpectrumArray;
        for (auto audioSpectrum_ : audioSpectrum) {
            audioSpectrumArray.push_back(FTValue((double)audioSpectrum_));
        }

        retMap[FTValue("audioSpectrum")] = FTValue(audioSpectrumArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteAudioSpectrumUpdate(
    const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum> &audioSpectrums) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteAudioSpectrumUpdate");

        FTMap audioSpectrumsMap;
        for (auto audioSpectrum : audioSpectrums) {
            FTArray audioSpectrumArray;
            for (auto audioSpectrum_ : audioSpectrum.second) {
                audioSpectrumArray.push_back(FTValue((double)audioSpectrum_));
            }
            audioSpectrumsMap[FTValue(audioSpectrum.first)] = FTValue(audioSpectrumArray);
        }

        retMap[FTValue("audioSpectrums")] = FTValue(audioSpectrumsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onLocalDeviceExceptionOccurred(
    EXPRESS::ZegoDeviceExceptionType exceptionType, EXPRESS::ZegoDeviceType deviceType,
    const std::string &deviceID) {

    ZF::logInfo("[onLocalDeviceExceptionOccurred] exceptionType: %d, deviceID: %s, deviceType: %d", exceptionType, deviceID.c_str(), deviceType);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onLocalDeviceExceptionOccurred");

        retMap[FTValue("exceptionType")] = FTValue((int32_t)exceptionType);
        retMap[FTValue("deviceType")] = FTValue((int32_t)deviceType);
        retMap[FTValue("deviceID")] = FTValue(deviceID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteCameraStateUpdate(
    const std::string &streamID, EXPRESS::ZegoRemoteDeviceState state) {

    ZF::logInfo("[onRemoteCameraStateUpdate] streamID: %s, state: %d", streamID.c_str(), state);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteCameraStateUpdate");

        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSpeakerStateUpdate(
    const std::string &streamID, EXPRESS::ZegoRemoteDeviceState state) {

    ZF::logInfo("[onRemoteSpeakerStateUpdate] streamID: %s, state: %d", streamID.c_str(), state);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSpeakerStateUpdate");

        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioVADStateUpdate(
    EXPRESS::ZegoAudioVADStableStateMonitorType type, EXPRESS::ZegoAudioVADType state) {

    ZF::logInfo("[onAudioVADStateUpdate] type: %d, state: %d", type, state);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAudioVADStateUpdate");

        retMap[FTValue("type")] = FTValue((int32_t)type);
        retMap[FTValue("state")] = FTValue((int32_t)state);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvBroadcastMessage(
    const std::string &roomID, std::vector<EXPRESS::ZegoBroadcastMessageInfo> messageList) {

    ZF::logInfo("[onIMRecvBroadcastMessage] roomID: %s, messageListCount: %d", roomID.c_str(), messageList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onIMRecvBroadcastMessage");

        retMap[FTValue("roomID")] = FTValue(roomID);
        FTArray messageListArray;
        for (auto message : messageList) {
            FTMap messageMap;
            messageMap[FTValue("message")] = FTValue(message.message);
            messageMap[FTValue("messageID")] = FTValue((int64_t)message.messageID);
            messageMap[FTValue("sendTime")] = FTValue((int64_t)message.sendTime);
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(message.fromUser.userID);
            userMap[FTValue("userName")] = FTValue(message.fromUser.userName);

            messageMap[FTValue("fromUser")] = FTValue(userMap);

            messageListArray.push_back(FTValue(messageMap));
        }
        retMap[FTValue("messageList")] = FTValue(messageListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvBarrageMessage(
    const std::string &roomID, std::vector<EXPRESS::ZegoBarrageMessageInfo> messageList) {

    ZF::logInfo("[onIMRecvBarrageMessage] roomID: %s, messageListCount: %d", roomID.c_str(), messageList.size());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onIMRecvBarrageMessage");

        retMap[FTValue("roomID")] = FTValue(roomID);
        FTArray messageListArray;
        for (auto message : messageList) {
            FTMap messageMap;
            messageMap[FTValue("message")] = FTValue(message.message);
            messageMap[FTValue("messageID")] = FTValue(message.messageID);
            messageMap[FTValue("sendTime")] = FTValue((int64_t)message.sendTime);
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(message.fromUser.userID);
            userMap[FTValue("userName")] = FTValue(message.fromUser.userName);

            messageMap[FTValue("fromUser")] = FTValue(userMap);

            messageListArray.push_back(FTValue(messageMap));
        }
        retMap[FTValue("messageList")] = FTValue(messageListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvCustomCommand(const std::string &roomID,
                                                          EXPRESS::ZegoUser fromUser,
                                                          const std::string &command) {

    ZF::logInfo("[onIMRecvCustomCommand] roomID: %s, userID: %s, command: %s", roomID.c_str(), fromUser.userID.c_str(), command.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onIMRecvCustomCommand");

        retMap[FTValue("roomID")] = FTValue(roomID);
        FTMap userMap;
        userMap[FTValue("userID")] = FTValue(fromUser.userID);
        userMap[FTValue("userName")] = FTValue(fromUser.userName);
        retMap[FTValue("fromUser")] = FTValue(userMap);
        retMap[FTValue("command")] = FTValue(command);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRecvRoomTransparentMessage(const std::string & roomID, const EXPRESS::ZegoRoomRecvTransparentMessage& message) 
{
    ZF::logInfo("[onRecvRoomTransparentMessage] roomID: %s, userID: %s", roomID.c_str(), message.sendUser.userID.c_str());
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRecvRoomTransparentMessage");
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTMap userMap, messageMap;

        userMap[FTValue("userID")] = FTValue(message.sendUser.userID);
        userMap[FTValue("userName")] = FTValue(message.sendUser.userName);
        messageMap[FTValue("sendUser")] = FTValue(userMap);

        unsigned char* data =  (unsigned char* )message.content.data();
        unsigned int data_length = (unsigned int)message.content.length();
        std::vector<uint8_t> dataArray(data, data + data_length);
        
        messageMap[FTValue("content")] = FTValue(dataArray);

        retMap[FTValue("message")] = FTValue(messageMap);
        
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPerformanceStatusUpdate(
    const EXPRESS::ZegoPerformanceStatus &status) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPerformanceStatusUpdate");

        FTMap statusMap;
        statusMap[FTValue("cpuUsageApp")] = FTValue(status.cpuUsageApp);
        statusMap[FTValue("cpuUsageSystem")] = FTValue(status.cpuUsageSystem);
        statusMap[FTValue("memoryUsageApp")] = FTValue(status.memoryUsageApp);
        statusMap[FTValue("memoryUsageSystem")] = FTValue(status.memoryUsageSystem);
        statusMap[FTValue("memoryUsedApp")] = FTValue(status.memoryUsedApp);
        retMap[FTValue("status")] = FTValue(statusMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkModeChanged(EXPRESS::ZegoNetworkMode mode) {

    ZF::logInfo("[onNetworkModeChanged] mode: %d", mode);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkModeChanged");
        retMap[FTValue("mode")] = FTValue((int32_t)mode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestError(
    int errorCode, EXPRESS::ZegoNetworkSpeedTestType type) {

    ZF::logInfo("[onNetworkSpeedTestError] errorCode: %d, type: %d", errorCode, type);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkSpeedTestError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("type")] = FTValue((int32_t)type);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestQualityUpdate(
    const EXPRESS::ZegoNetworkSpeedTestQuality &quality, EXPRESS::ZegoNetworkSpeedTestType type) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkSpeedTestQualityUpdate");
        FTMap qualityMap;
        qualityMap[FTValue("connectCost")] = FTValue((int64_t)quality.connectCost);
        qualityMap[FTValue("rtt")] = FTValue((int64_t)quality.rtt);
        qualityMap[FTValue("packetLostRate")] = FTValue(quality.packetLostRate);
        qualityMap[FTValue("quality")] = FTValue((int32_t)quality.quality);

        retMap[FTValue("quality")] = FTValue(qualityMap);
        retMap[FTValue("type")] = FTValue((int32_t)type);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRecvExperimentalAPI(const std::string &content) {

    ZF::logInfo("[onRecvExperimentalAPI] content: %s", content.c_str());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRecvExperimentalAPI");
        retMap[FTValue("content")] = FTValue(content);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkQuality(
    const std::string &userID, EXPRESS::ZegoStreamQualityLevel upstreamQuality,
    EXPRESS::ZegoStreamQualityLevel downstreamQuality) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkQuality");
        retMap[FTValue("upstreamQuality")] = FTValue((int32_t)upstreamQuality);
        retMap[FTValue("downstreamQuality")] = FTValue((int32_t)downstreamQuality);
        retMap[FTValue("userID")] = FTValue(userID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onReceiveRealTimeSequentialData(
    EXPRESS::IZegoRealTimeSequentialDataManager *manager, const unsigned char *data,
    unsigned int dataLength, const std::string &streamID) {
    
    // High frequency callbacks do not log
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onReceiveRealTimeSequentialData");
        retMap[FTValue("realTimeSequentialDataManagerIndex")] = FTValue(manager->getIndex());

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRangeAudioMicrophoneStateUpdate(
    EXPRESS::IZegoRangeAudio *rangeAudio, EXPRESS::ZegoRangeAudioMicrophoneState state,
    int errorCode) {

    ZF::logInfo("[onRangeAudioMicrophoneStateUpdate] index: %d, state: %d, errorCode: %d", 0, state, errorCode);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRangeAudioMicrophoneStateUpdate");
        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessCapturedAudioData(unsigned char *data,
                                                               unsigned int dataLength,
                                                               EXPRESS::ZegoAudioFrameParam *param,
                                                               double timestamp) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onProcessCapturedAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("dataLength")] = FTValue((int64_t)dataLength);
        retMap[FTValue("timestamp")] = FTValue(timestamp);

        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param->sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param->channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(
    unsigned char *data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam *param,
    double timestamp) {

    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onProcessCapturedAudioDataAfterUsedHeadphoneMonitor");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("dataLength")] = FTValue((int64_t)dataLength);
        retMap[FTValue("timestamp")] = FTValue(timestamp);

        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param->sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param->channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessRemoteAudioData(unsigned char *data,
                                                             unsigned int dataLength,
                                                             EXPRESS::ZegoAudioFrameParam *param,
                                                             const std::string &streamID,
                                                             double timestamp) {
    // High frequency callbacks do not log

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onProcessRemoteAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("dataLength")] = FTValue((int64_t)dataLength);
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("timestamp")] = FTValue(timestamp);

        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param->sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param->channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessPlaybackAudioData(unsigned char *data,
                                                               unsigned int dataLength,
                                                               EXPRESS::ZegoAudioFrameParam *param,
                                                               double timestamp) {

    // High frequency callbacks do not log
    
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onProcessPlaybackAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("dataLength")] = FTValue((int64_t)dataLength);
        retMap[FTValue("timestamp")] = FTValue(timestamp);

        FTMap paramMap;
        paramMap[FTValue("sampleRate")] = FTValue((int32_t)param->sampleRate);
        paramMap[FTValue("channel")] = FTValue((int32_t)param->channel);
        retMap[FTValue("param")] = FTValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAvailableFrame(EXPRESS::IZegoScreenCaptureSource *source,
                                                     const void *data, unsigned int dataLength,
                                                     EXPRESS::ZegoVideoFrameParam param) {

    // High frequency callbacks do not log
    unsigned char *rgb_data = (unsigned char *)data;
    ZegoTextureRendererController::getInstance()->sendScreenCapturedVideoFrameRawData(
        &rgb_data, &dataLength, param, ZEGO::EXPRESS::ZEGO_VIDEO_FLIP_MODE_NONE);
}

void ZegoExpressEngineEventHandler::onExceptionOccurred(
    EXPRESS::IZegoScreenCaptureSource *source,
    EXPRESS::ZegoScreenCaptureSourceExceptionType exceptionType) {

    ZF::logInfo("[onExceptionOccurred] index: %d, exceptionType: %d", source->getIndex(), exceptionType);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onExceptionOccurred");
        retMap[FTValue("screenCaptureSourceIndex")] = FTValue(source->getIndex());
        retMap[FTValue("exceptionType")] = FTValue(static_cast<int32_t>(exceptionType));

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onWindowStateChanged(
    EXPRESS::IZegoScreenCaptureSource *source, EXPRESS::ZegoScreenCaptureWindowState windowState,
    EXPRESS::ZegoRect windowRect) {

    ZF::logInfo("[onWindowStateChanged] index: %d, windowState: %d", source->getIndex(), windowState);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onWindowStateChanged");
        retMap[FTValue("screenCaptureSourceIndex")] = FTValue(source->getIndex());
        retMap[FTValue("windowState")] = FTValue(static_cast<int32_t>(windowState));

        FTMap rectMap;
        rectMap[FTValue("x")] = FTValue(windowRect.x);
        rectMap[FTValue("y")] = FTValue(windowRect.y);
        rectMap[FTValue("width")] = FTValue(windowRect.width);
        rectMap[FTValue("height")] = FTValue(windowRect.height);

        retMap[FTValue("windowRect")] = rectMap;

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRectChanged(EXPRESS::IZegoScreenCaptureSource* source, EXPRESS::ZegoRect captureRect) {
    ZF::logInfo("[onRectChanged] index: %d", source->getIndex());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRectChanged");
        retMap[FTValue("screenCaptureSourceIndex")] = FTValue(source->getIndex());

        FTMap rectMap;
        rectMap[FTValue("x")] = FTValue(captureRect.x);
        rectMap[FTValue("y")] = FTValue(captureRect.y);
        rectMap[FTValue("width")] = FTValue(captureRect.width);
        rectMap[FTValue("height")] = FTValue(captureRect.height);

        retMap[FTValue("captureRect")] = rectMap;

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onInit(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger,
                                           int errorCode) {
    ZF::logInfo("[onAIVoiceChangerInit] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAIVoiceChangerInit");
        retMap[FTValue("aiVoiceChangerIndex")] = FTValue(aiVoiceChanger->getIndex());
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUpdate(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger,
                                             int errorCode) {
    ZF::logInfo("[onAIVoiceChangerUpdate] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAIVoiceChangerUpdate");
        retMap[FTValue("aiVoiceChangerIndex")] = FTValue(aiVoiceChanger->getIndex());
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onGetSpeakerList(
    EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, int errorCode,
    const std::vector<EXPRESS::ZegoAIVoiceChangerSpeakerInfo> &speakerList) {
    ZF::logInfo("[onAIVoiceChangerGetSpeakerList] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAIVoiceChangerGetSpeakerList");
        retMap[FTValue("aiVoiceChangerIndex")] = FTValue(aiVoiceChanger->getIndex());
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        FTArray speakerListArray;
        for (auto &speaker : speakerList) {
            FTMap speakerMap;
            speakerMap[FTValue("id")] = FTValue(speaker.id);
            speakerMap[FTValue("name")] = FTValue(speaker.name);
            speakerListArray.emplace_back(FTValue(speakerMap));
        }
        retMap[FTValue("speakerList")] = FTValue(speakerListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUpdateProgress(
    EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, double percent,
    int fileIndex, int fileCount) {
    ZF::logInfo("[onAIVoiceChangerUpdateProgress] index: %d, percent: %lf, fileIndex: %d, fileCount: %d", aiVoiceChanger->getIndex(), percent, fileIndex, fileCount);

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAIVoiceChangerUpdateProgress");
        retMap[FTValue("aiVoiceChangerIndex")] = FTValue(aiVoiceChanger->getIndex());
        retMap[FTValue("percent")] = FTValue(percent);
        retMap[FTValue("fileIndex")] = FTValue(fileIndex);
        retMap[FTValue("fileCount")] = FTValue(fileCount);

        eventSink_->Success(retMap);
    }
}
