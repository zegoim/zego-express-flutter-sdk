#include <memory>

#include "ZegoExpressEngineEventHandler.h"
#include "ZegoExpressEngineMethodHandler.h"
#include "ZegoTextureRendererController.h"
#include "ZegoLog.h"

std::shared_ptr<ZegoExpressEngineEventHandler> ZegoExpressEngineEventHandler::m_instance = nullptr;

void ZegoExpressEngineEventHandler::setEventSink(ZFEventSink &&eventSink) {
    eventSink_ = std::move(eventSink);
}

void ZegoExpressEngineEventHandler::clearEventSink() { eventSink_.reset(); }

void ZegoExpressEngineEventHandler::onDebugError(int errorCode, const std::string &funcName,
                                                 const std::string &info) {

    ZF::logInfo("[onDebugError] errorCode: %d, funcName: %s, info: %s", errorCode, funcName.c_str(),
                info.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onDebugError");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("funcName")] = ZFValue(funcName);
        retMap[ZFValue("info")] = ZFValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onApiCalledResult(int errorCode, const std::string &funcName,
                                                      const std::string &info) {

    ZF::logInfo("[onApiCalledResult] errorCode: %d, funcName: %s, info: %s", errorCode,
                funcName.c_str(), info.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onApiCalledResult");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("funcName")] = ZFValue(funcName);
        retMap[ZFValue("info")] = ZFValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onFatalError(int errorCode) {

    ZF::logInfo("[onFatalError] errorCode: %d", errorCode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onFatalError");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

    ZF::logInfo("[onEngineStateUpdate] state: %d", state);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onEngineStateUpdate");
        retMap[ZFValue("state")] = ZFValue(state);

        eventSink_->Success(retMap);
    }

    if (state == EXPRESS::ZegoEngineState::ZEGO_ENGINE_STATE_STOP) {
        ZegoTextureRendererController::getInstance()->resetAllRenderFirstFrame();
    }
}

void ZegoExpressEngineEventHandler::onRoomStateUpdate(const std::string &roomID,
                                                      EXPRESS::ZegoRoomState state, int errorCode,
                                                      const std::string &extendedData) {

    ZF::logInfo("[onRoomStateUpdate] roomID: %s, state: %d, errorCode: %d, extendedData: %s",
                roomID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomStateUpdate");
        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("roomID")] = ZFValue(roomID);
        retMap[ZFValue("extendedData")] = ZFValue(extendedData);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStateChanged(const std::string &roomID,
                                                       EXPRESS::ZegoRoomStateChangedReason reason,
                                                       int errorCode,
                                                       const std::string &extendedData) {

    ZF::logInfo("[onRoomStateChanged] roomID: %s, reason: %d, errorCode: %d, extendedData: %s",
                roomID.c_str(), reason, errorCode, extendedData.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomStateChanged");
        retMap[ZFValue("reason")] = ZFValue(reason);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        std::string extendedData_ = extendedData.empty() ? "{}" : extendedData;
        retMap[ZFValue("extendedData")] = ZFValue(extendedData_);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomUserUpdate(
    const std::string &roomID, EXPRESS::ZegoUpdateType updateType,
    const std::vector<EXPRESS::ZegoUser> &userList) {

    ZF::logInfo("[onRoomUserUpdate] roomID: %s, updateType: %d, userListCount: %d", roomID.c_str(),
                updateType, userList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomUserUpdate");
        retMap[ZFValue("updateType")] = ZFValue(updateType);
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        ZFArray userListArray;
        for (auto &user : userList) {
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(user.userID);
            userMap[ZFValue("userName")] = ZFValue(user.userName);
            userListArray.emplace_back(ZFValue(userMap));
        }
        retMap[ZFValue("userList")] = ZFValue(userListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomOnlineUserCountUpdate(const std::string &roomID,
                                                                int count) {

    ZF::logInfo("[onRoomOnlineUserCountUpdate] roomID: %s, count: %d", roomID.c_str(), count);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomOnlineUserCountUpdate");
        retMap[ZFValue("count")] = ZFValue(count);
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStreamUpdate(
    const std::string &roomID, EXPRESS::ZegoUpdateType updateType,
    const std::vector<EXPRESS::ZegoStream> &streamList, const std::string &extendedData) {

    ZF::logInfo(
        "[onRoomStreamUpdate] roomID: %s, updateType: %d, streamListCount: %d, extendedData :%d",
        roomID.c_str(), updateType, streamList.size(), extendedData.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomStreamUpdate");
        retMap[ZFValue("updateType")] = ZFValue(updateType);
        retMap[ZFValue("roomID")] = ZFValue(roomID);
        retMap[ZFValue("extendedData")] = ZFValue(extendedData);

        ZFArray streamListArray;
        for (auto &stream : streamList) {
            ZFMap streamMap;
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(stream.user.userID);
            userMap[ZFValue("userName")] = ZFValue(stream.user.userName);

            streamMap[ZFValue("streamID")] = ZFValue(stream.streamID);
            streamMap[ZFValue("extraInfo")] = ZFValue(stream.extraInfo);
            streamMap[ZFValue("user")] = ZFValue(userMap);

            streamListArray.emplace_back(ZFValue(streamMap));
        }
        retMap[ZFValue("streamList")] = ZFValue(streamListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStreamExtraInfoUpdate(
    const std::string &roomID, const std::vector<EXPRESS::ZegoStream> &streamList) {

    ZF::logInfo("[onRoomStreamExtraInfoUpdate] roomID: %s, streamListCount: %d", roomID.c_str(),
                streamList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomStreamExtraInfoUpdate");
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        ZFArray streamListArray;
        for (auto &stream : streamList) {
            ZFMap streamMap;
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(stream.user.userID);
            userMap[ZFValue("userName")] = ZFValue(stream.user.userName);

            streamMap[ZFValue("streamID")] = ZFValue(stream.streamID);
            streamMap[ZFValue("extraInfo")] = ZFValue(stream.extraInfo);
            streamMap[ZFValue("user")] = ZFValue(userMap);

            streamListArray.emplace_back(ZFValue(streamMap));
        }
        retMap[ZFValue("streamList")] = ZFValue(streamListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomExtraInfoUpdate(
    const std::string &roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo> &roomExtraInfoList) {

    ZF::logInfo("[onRoomExtraInfoUpdate] roomID: %s, streamListCount: %d", roomID.c_str(),
                roomExtraInfoList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomExtraInfoUpdate");
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        ZFArray roomExtraInfoListArray;
        for (auto &roomExtraInfo : roomExtraInfoList) {
            ZFMap infoMap;
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(roomExtraInfo.updateUser.userID);
            userMap[ZFValue("userName")] = ZFValue(roomExtraInfo.updateUser.userName);

            infoMap[ZFValue("key")] = ZFValue(roomExtraInfo.key);
            infoMap[ZFValue("value")] = ZFValue(roomExtraInfo.value);
            infoMap[ZFValue("updateTime")] = ZFValue((int64_t)roomExtraInfo.updateTime);
            infoMap[ZFValue("updateUser")] = ZFValue(userMap);

            roomExtraInfoListArray.emplace_back(ZFValue(infoMap));
        }
        retMap[ZFValue("roomExtraInfoList")] = ZFValue(roomExtraInfoListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherStateUpdate(const std::string &streamID,
                                                           EXPRESS::ZegoPublisherState state,
                                                           int errorCode,
                                                           const std::string &extendedData) {
    ZF::logInfo("[onPublisherStateUpdate] streamID: %s, state: %d, errorCode: %d, extendedData: %s",
                streamID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherStateUpdate");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("extendedData")] = ZFValue(extendedData);

        eventSink_->Success(retMap);
    }
}

ZFMap ZegoExpressEngineEventHandler::convertPublishQuality(
    const EXPRESS::ZegoPublishStreamQuality &quality) {
    ZFMap qualityMap;
    qualityMap[ZFValue("videoCaptureFPS")] = ZFValue(quality.videoCaptureFPS);
    qualityMap[ZFValue("videoEncodeFPS")] = ZFValue(quality.videoEncodeFPS);
    qualityMap[ZFValue("videoSendFPS")] = ZFValue(quality.videoSendFPS);
    qualityMap[ZFValue("videoKBPS")] = ZFValue(quality.videoKBPS);
    qualityMap[ZFValue("audioCaptureFPS")] = ZFValue(quality.audioCaptureFPS);
    qualityMap[ZFValue("audioSendFPS")] = ZFValue(quality.audioSendFPS);
    qualityMap[ZFValue("audioKBPS")] = ZFValue(quality.audioKBPS);
    qualityMap[ZFValue("rtt")] = ZFValue(quality.rtt);
    qualityMap[ZFValue("packetLostRate")] = ZFValue(quality.packetLostRate);
    qualityMap[ZFValue("level")] = ZFValue(quality.level);
    qualityMap[ZFValue("isHardwareEncode")] = ZFValue(quality.isHardwareEncode);
    qualityMap[ZFValue("videoCodecID")] = ZFValue(quality.videoCodecID);
    qualityMap[ZFValue("totalSendBytes")] = ZFValue(quality.totalSendBytes);
    qualityMap[ZFValue("audioSendBytes")] = ZFValue(quality.audioSendBytes);
    qualityMap[ZFValue("videoSendBytes")] = ZFValue(quality.videoSendBytes);

    return qualityMap;
}

void ZegoExpressEngineEventHandler::onPublisherQualityUpdate(
    const std::string &streamID, const EXPRESS::ZegoPublishStreamQuality &quality) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherQualityUpdate");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        ZFMap qualityMap = convertPublishQuality(quality);

        retMap[ZFValue("quality")] = ZFValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedAudioFirstFrame() {

    ZF::logInfo("[onPublisherCapturedAudioFirstFrame]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherCapturedAudioFirstFrame");

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherSendAudioFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherSendAudioFirstFrame] channel: %d", channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherSendAudioFirstFrame");
        retMap[ZFValue("channel")] = ZFValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherStreamEvent(EXPRESS::ZegoStreamEvent eventID,
                                                           const std::string &streamID,
                                                           const std::string &extraInfo) {

    ZF::logInfo("[onPublisherStreamEvent] eventID: %d, streamID: %s, extraInfo: %s", eventID,
                streamID.c_str(), extraInfo.c_str());

    if (eventSink_) {
        ZFMap retMap;

        retMap[ZFValue("method")] = ZFValue("onPublisherStreamEvent");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("eventID")] = ZFValue((int32_t)eventID);
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("extraInfo")] = ZFValue(extraInfo);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onVideoObjectSegmentationStateChanged(
    EXPRESS::ZegoObjectSegmentationState state, EXPRESS::ZegoPublishChannel channel,
    int errorCode) {

    ZF::logInfo("[onVideoObjectSegmentationStateChanged] state: %d, channel: %d", state, channel);

    if (eventSink_) {
        ZFMap retMap;

        retMap[ZFValue("method")] = ZFValue("onVideoObjectSegmentationStateChanged");
        retMap[ZFValue("state")] = ZFValue((int32_t)state);
        retMap[ZFValue("channel")] = ZFValue((int32_t)channel);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID,
                                                             EXPRESS::ZegoPublishChannel channel) {
    ZF::logInfo("[onPublisherLowFpsWarning] codecID: %d, channel: %d", codecID, channel);

    if (eventSink_) {
        ZFMap retMap;

        retMap[ZFValue("method")] = ZFValue("onPublisherLowFpsWarning");
        retMap[ZFValue("codecID")] = ZFValue((int32_t)codecID);
        retMap[ZFValue("channel")] = ZFValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherDummyCaptureImagePathError(
    int errorCode, const std::string &path, EXPRESS::ZegoPublishChannel channel) {
    ZF::logInfo("[onPublisherDummyCaptureImagePathError] errorCode: %d, path: %s, channel: %d",
                errorCode, path.c_str(), channel);

    if (eventSink_) {
        ZFMap retMap;

        retMap[ZFValue("method")] = ZFValue("onPublisherDummyCaptureImagePathError");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("path")] = ZFValue(path);
        retMap[ZFValue("channel")] = ZFValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerStateUpdate(const std::string &streamID,
                                                        EXPRESS::ZegoPlayerState state,
                                                        int errorCode,
                                                        const std::string &extendedData) {

    ZF::logInfo("[onPlayerStateUpdate] streamID: %s, state: %d, errorCode: %d, extendedData: %s",
                streamID.c_str(), state, errorCode, extendedData.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerStateUpdate");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("extendedData")] = ZFValue(extendedData);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerSwitched(const std::string& streamID, int errorCode) {

    ZF::logInfo("[onPlayerSwitched] streamID: %s, errorCode: %d", streamID.c_str(), errorCode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerSwitched");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerQualityUpdate(
    const std::string &streamID, const EXPRESS::ZegoPlayStreamQuality &quality) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerQualityUpdate");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        ZFMap qualityMap;
        qualityMap[ZFValue("videoRecvFPS")] = ZFValue(quality.videoRecvFPS);
        qualityMap[ZFValue("videoDejitterFPS")] = ZFValue(quality.videoDejitterFPS);
        qualityMap[ZFValue("videoDecodeFPS")] = ZFValue(quality.videoDecodeFPS);
        qualityMap[ZFValue("videoRenderFPS")] = ZFValue(quality.videoRenderFPS);
        qualityMap[ZFValue("videoKBPS")] = ZFValue(quality.videoKBPS);
        qualityMap[ZFValue("videoBreakRate")] = ZFValue(quality.videoBreakRate);
        qualityMap[ZFValue("audioRecvFPS")] = ZFValue(quality.audioRecvFPS);
        qualityMap[ZFValue("audioDejitterFPS")] = ZFValue(quality.audioDejitterFPS);
        qualityMap[ZFValue("audioDecodeFPS")] = ZFValue(quality.audioDecodeFPS);
        qualityMap[ZFValue("audioRenderFPS")] = ZFValue(quality.audioRenderFPS);
        qualityMap[ZFValue("audioKBPS")] = ZFValue(quality.audioKBPS);
        qualityMap[ZFValue("audioBreakRate")] = ZFValue(quality.audioBreakRate);
        qualityMap[ZFValue("mos")] = ZFValue(quality.mos);
        qualityMap[ZFValue("rtt")] = ZFValue(quality.rtt);
        qualityMap[ZFValue("packetLostRate")] = ZFValue(quality.packetLostRate);
        qualityMap[ZFValue("peerToPeerDelay")] = ZFValue(quality.peerToPeerDelay);
        qualityMap[ZFValue("peerToPeerPacketLostRate")] = ZFValue(quality.peerToPeerPacketLostRate);
        qualityMap[ZFValue("level")] = ZFValue(quality.level);
        qualityMap[ZFValue("delay")] = ZFValue(quality.delay);
        qualityMap[ZFValue("avTimestampDiff")] = ZFValue(quality.avTimestampDiff);
        qualityMap[ZFValue("isHardwareDecode")] = ZFValue(quality.isHardwareDecode);
        qualityMap[ZFValue("videoCodecID")] = ZFValue(quality.videoCodecID);
        qualityMap[ZFValue("totalRecvBytes")] = ZFValue(quality.totalRecvBytes);
        qualityMap[ZFValue("audioRecvBytes")] = ZFValue(quality.audioRecvBytes);
        qualityMap[ZFValue("videoRecvBytes")] = ZFValue(quality.videoRecvBytes);
        qualityMap[ZFValue("audioCumulativeBreakCount")] =
            ZFValue((int32_t)quality.audioCumulativeBreakCount);
        qualityMap[ZFValue("videoCumulativeBreakCount")] =
            ZFValue((int32_t)quality.videoCumulativeBreakCount);
        qualityMap[ZFValue("audioCumulativeBreakTime")] =
            ZFValue((int32_t)quality.audioCumulativeBreakTime);
        qualityMap[ZFValue("videoCumulativeBreakTime")] =
            ZFValue((int32_t)quality.videoCumulativeBreakTime);
        qualityMap[ZFValue("audioCumulativeBreakRate")] = ZFValue(quality.audioCumulativeBreakRate);
        qualityMap[ZFValue("videoCumulativeBreakRate")] = ZFValue(quality.videoCumulativeBreakRate);
        qualityMap[ZFValue("audioCumulativeDecodeTime")] =
            ZFValue((int32_t)quality.audioCumulativeDecodeTime);
        qualityMap[ZFValue("videoCumulativeDecodeTime")] =
            ZFValue((int32_t)quality.videoCumulativeDecodeTime);
        qualityMap[ZFValue("muteVideo")] = ZFValue(quality.muteVideo);
        qualityMap[ZFValue("muteAudio")] = ZFValue(quality.muteAudio);

        retMap[ZFValue("quality")] = ZFValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerMediaEvent(const std::string &streamID,
                                                       EXPRESS::ZegoPlayerMediaEvent event) {

    ZF::logInfo("[onPlayerMediaEvent] streamID: %s, event: %d", streamID.c_str(), event);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerMediaEvent");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("event")] = ZFValue(event);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRecvAudioFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRecvAudioFirstFrame");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvSEI(const std::string &streamID,
                                                    const unsigned char *data,
                                                    unsigned int dataLength) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRecvSEI");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[ZFValue("data")] = ZFValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvMediaSideInfo(
    const EXPRESS::ZegoMediaSideInfo &info) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRecvMediaSideInfo");
        retMap[ZFValue("streamID")] = ZFValue(info.streamID);
        retMap[ZFValue("timestampNs")] = ZFValue(info.timestampNs);

        auto nonConstData = const_cast<unsigned char *>(info.SEIData);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + info.SEIDataLength);

        retMap[ZFValue("SEIData")] = ZFValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioSideInfo(const std::string &streamID,
                                                              const unsigned char *data,
                                                              unsigned int dataLength) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRecvAudioSideInfo");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[ZFValue("data")] = ZFValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerStreamEvent(EXPRESS::ZegoStreamEvent eventID,
                                                        const std::string &streamID,
                                                        const std::string &extraInfo) {

    ZF::logInfo("[onPlayerStreamEvent] eventID: %d, streamID: %s, extraInfo: %s", eventID,
                streamID.c_str(), extraInfo.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerStreamEvent");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("eventID")] = ZFValue((int32_t)eventID);
        retMap[ZFValue("extraInfo")] = ZFValue(extraInfo);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderCameraVideoFirstFrame(
    const std::string &streamID) {

    ZF::logInfo("[onPlayerRenderCameraVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRenderCameraVideoFirstFrame");
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

// void ZegoExpressEngineEventHandler::onPlayerVideoSuperResolutionUpdate(std::string streamID,EXPRESS::ZegoSuperResolutionState state,int errorCode) {
//     if (eventSink_) {
//         ZFMap retMap;
//         retMap[ZFValue("method")] = ZFValue("onPlayerVideoSuperResolutionUpdate");
//         retMap[ZFValue("streamID")] = ZFValue(streamID);
//         retMap[ZFValue("state")] = ZFValue((int32_t)state);
//         retMap[ZFValue("errorCode")] = ZFValue(errorCode);

//         eventSink_->Success(retMap);
//     }
// }

void ZegoExpressEngineEventHandler::onMixerRelayCDNStateUpdate(
    const std::string &taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> &infoList) {

    ZF::logInfo("[onMixerRelayCDNStateUpdate] taskID: %s, infoListCount: %d", taskID.c_str(),
                infoList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMixerRelayCDNStateUpdate");
        retMap[ZFValue("taskID")] = ZFValue(taskID);

        ZFArray infoListArray;
        for (auto &info : infoList) {
            ZFMap infoMap;
            infoMap[ZFValue("url")] = ZFValue(info.url);
            infoMap[ZFValue("state")] = ZFValue((int32_t)info.state);
            infoMap[ZFValue("updateReason")] = ZFValue((int32_t)info.updateReason);
            infoMap[ZFValue("stateTime")] = ZFValue((int32_t)info.stateTime);
            infoListArray.emplace_back(ZFValue(infoMap));
        }
        retMap[ZFValue("infoList")] = ZFValue(infoListArray);
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixerSoundLevelUpdate(
    const std::unordered_map<unsigned int, float> &soundLevels) {

    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMixerSoundLevelUpdate");

        ZFMap soundLevelsMap;
        for (auto &soundlevel : soundLevels) {
            soundLevelsMap[ZFValue((int32_t)soundlevel.first)] = ZFValue(soundlevel.second);
        }
        retMap[ZFValue("soundLevels")] = ZFValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceStateChanged(
    EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType,
    const EXPRESS::ZegoDeviceInfo &deviceInfo) {

    ZF::logInfo(
        "[onAudioDeviceStateChanged] updateType: %d, deviceType: %d, deviceID: %s, deviceName: %s",
        updateType, deviceType, deviceInfo.deviceID.c_str(), deviceInfo.deviceName.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAudioDeviceStateChanged");

        retMap[ZFValue("updateType")] = ZFValue((int32_t)updateType);
        retMap[ZFValue("deviceType")] = ZFValue((int32_t)deviceType);

        ZFMap deviceInfoMap;
        deviceInfoMap[ZFValue("deviceID")] = ZFValue(deviceInfo.deviceID);
        deviceInfoMap[ZFValue("deviceName")] = ZFValue(deviceInfo.deviceName);

        retMap[ZFValue("deviceInfo")] = ZFValue(deviceInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceVolumeChanged(
    EXPRESS::ZegoAudioDeviceType deviceType, const std::string &deviceID, int volume) {

    ZF::logInfo("[onAudioDeviceVolumeChanged] deviceType: %d, deviceID: %s, volume: %d", deviceType,
                deviceID.c_str(), volume);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAudioDeviceVolumeChanged");

        retMap[ZFValue("deviceType")] = ZFValue((int32_t)deviceType);

        retMap[ZFValue("deviceID")] = ZFValue(deviceID);
        retMap[ZFValue("volume")] = ZFValue(volume);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedSoundLevelUpdate(float soundLevel) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedSoundLevelUpdate");
        retMap[ZFValue("soundLevel")] = ZFValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelUpdate(
    const std::unordered_map<std::string, float> &soundLevels) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteSoundLevelUpdate");

        ZFMap soundLevelsMap;
        for (auto &soundlevel : soundLevels) {
            soundLevelsMap[ZFValue(soundlevel.first)] = ZFValue(soundlevel.second);
        }
        retMap[ZFValue("soundLevels")] = ZFValue(soundLevelsMap);

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
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteMicStateUpdate");
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("state")] = ZFValue(state);

        eventSink_->Success(retMap);
    }
}

// Audio Effect Player
void ZegoExpressEngineEventHandler::onAudioEffectPlayStateUpdate(
    EXPRESS::IZegoAudioEffectPlayer *audioEffectPlayer, unsigned int audioEffectID,
    EXPRESS::ZegoAudioEffectPlayState state, int errorCode) {
    ZF::logInfo(
        "[onAudioEffectPlayStateUpdate] index: %d, audioEffectID: %d, state: %d, errorCode:%d",
        audioEffectPlayer->getIndex(), audioEffectID, state, errorCode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAudioEffectPlayStateUpdate");
        retMap[ZFValue("audioEffectPlayerIndex")] = ZFValue(audioEffectPlayer->getIndex());
        retMap[ZFValue("audioEffectID")] = ZFValue((int32_t)audioEffectID);
        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerStateUpdate(EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                             EXPRESS::ZegoMediaPlayerState state,
                                                             int errorCode) {
    ZF::logInfo("[onMediaPlayerStateUpdate] index: %d, state: %d, errorCode:%d",
                mediaPlayer->getIndex(), state, errorCode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerStateUpdate");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());
        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }

    if (state == EXPRESS::ZegoMediaPlayerState::ZEGO_MEDIA_PLAYER_STATE_NO_PLAY ||
        state == EXPRESS::ZegoMediaPlayerState::ZEGO_MEDIA_PLAYER_STATE_PLAY_ENDED) {
        ZegoTextureRendererController::getInstance()->resetMediaPlayerRenderFirstFrame(mediaPlayer);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerNetworkEvent(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, EXPRESS::ZegoMediaPlayerNetworkEvent networkEvent) {

    ZF::logInfo("[onMediaPlayerNetworkEvent] index: %d, networkEvent: %d", mediaPlayer->getIndex(),
                networkEvent);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerNetworkEvent");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());
        retMap[ZFValue("networkEvent")] = ZFValue(networkEvent);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerPlayingProgress(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, unsigned long long millisecond) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerPlayingProgress");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());
        // TODO: convert need test?
        retMap[ZFValue("millisecond")] = ZFValue((int64_t)millisecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerRecvSEI(EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                         const unsigned char *data,
                                                         unsigned int dataLength) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerRecvSEI");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());
        // TODO: convert need test?
        std::vector<uint8_t> vec_data(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(vec_data);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerSoundLevelUpdate(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, float soundLevel) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerSoundLevelUpdate");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("soundLevel")] = ZFValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerFrequencySpectrumUpdate(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, const EXPRESS::ZegoAudioSpectrum &spectrumList) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerFrequencySpectrumUpdate");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("spectrumList")] = ZFValue(spectrumList);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerFirstFrameEvent(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, EXPRESS::ZegoMediaPlayerFirstFrameEvent event) {

    ZF::logInfo("[onMediaPlayerFirstFrameEvent] index: %d, event: %d", mediaPlayer->getIndex(),
                event);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerFirstFrameEvent");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("event")] = ZFValue(event);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerRenderingProgress(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, unsigned long long millisecond) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerRenderingProgress");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("millisecond")] = ZFValue((int64_t)millisecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerVideoSizeChanged(
    EXPRESS::IZegoMediaPlayer *mediaPlayer, int width, int height) {
    ZF::logInfo("[onMediaPlayerVideoSizeChanged] index: %d, width: %d, height: %d",
                mediaPlayer->getIndex(), width, height);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerVideoSizeChanged");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("width")] = ZFValue(width);
        retMap[ZFValue("height")] = ZFValue(height);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerLocalCache(EXPRESS::IZegoMediaPlayer *mediaPlayer,
                                                            int errorCode,
                                                            const std::string &resource,
                                                            const std::string &cachedFile) {
    ZF::logInfo("[onMediaPlayerLocalCache] index: %d, error: %d, resource: %s, cachedFile: %s",
                mediaPlayer->getIndex(), errorCode, resource.c_str(), cachedFile.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMediaPlayerLocalCache");
        retMap[ZFValue("mediaPlayerIndex")] = ZFValue(mediaPlayer->getIndex());

        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("resource")] = ZFValue(resource);
        retMap[ZFValue("cachedFile")] = ZFValue(cachedFile);

        eventSink_->Success(retMap);
    }
}

// MediaDataPublisher
void ZegoExpressEngineEventHandler::onMediaDataPublisherFileOpen(
    EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileOpen] index: %d, path: %s",
                mediaDataPublisher->getIndex(), path.c_str());

    if (eventSink_) {
        ZFMap return_map;
        return_map[ZFValue("method")] = ZFValue("onMediaDataPublisherFileOpen");
        return_map[ZFValue("publisherIndex")] = ZFValue(mediaDataPublisher->getIndex());
        return_map[ZFValue("path")] = ZFValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onMediaDataPublisherFileClose(
    EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, int errorCode, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileClose] index: %d, errorCode: %d, path: %s",
                mediaDataPublisher->getIndex(), errorCode, path.c_str());

    if (eventSink_) {
        ZFMap return_map;
        return_map[ZFValue("method")] = ZFValue("onMediaDataPublisherFileClose");
        return_map[ZFValue("publisherIndex")] = ZFValue(mediaDataPublisher->getIndex());
        return_map[ZFValue("errorCode")] = ZFValue(errorCode);
        return_map[ZFValue("path")] = ZFValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onMediaDataPublisherFileDataBegin(
    EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) {
    ZF::logInfo("[onMediaDataPublisherFileDataBegin] index: %d, path: %s",
                mediaDataPublisher->getIndex(), path.c_str());

    if (eventSink_) {
        ZFMap return_map;
        return_map[ZFValue("method")] = ZFValue("onMediaDataPublisherFileDataBegin");
        return_map[ZFValue("publisherIndex")] = ZFValue(mediaDataPublisher->getIndex());
        return_map[ZFValue("path")] = ZFValue(path);

        eventSink_->Success(return_map);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioData(const unsigned char *data,
                                                        unsigned int dataLength,
                                                        EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataVec);
        retMap[ZFValue("dataLength")] = ZFValue((int)dataLength);
        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param.sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param.channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlaybackAudioData(const unsigned char *data,
                                                        unsigned int dataLength,
                                                        EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlaybackAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataVec);
        retMap[ZFValue("dataLength")] = ZFValue((int)dataLength);
        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param.sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param.channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixedAudioData(const unsigned char *data,
                                                     unsigned int dataLength,
                                                     EXPRESS::ZegoAudioFrameParam param) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onMixedAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataVec);
        retMap[ZFValue("dataLength")] = ZFValue((int)dataLength);
        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param.sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param.channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerAudioData(const unsigned char *data,
                                                      unsigned int dataLength,
                                                      EXPRESS::ZegoAudioFrameParam param,
                                                      const std::string &streamID) {
    // Super high frequency callbacks do not log, do not guard sink

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerAudioData");
        std::vector<uint8_t> dataVec(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataVec);
        retMap[ZFValue("dataLength")] = ZFValue((int)dataLength);
        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param.sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param.channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordStateUpdate(
    EXPRESS::ZegoDataRecordState state, int errorCode, EXPRESS::ZegoDataRecordConfig config,
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onCapturedDataRecordStateUpdate] state: %d, errorCode: %d, filePath: %s, "
                "recordType: %d, channel: %d",
                state, errorCode, config.filePath.c_str(), config.recordType, channel);

    if (eventSink_) {
        ZFMap retMap;
        ZFMap configMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedDataRecordStateUpdate");

        configMap[ZFValue("filePath")] = ZFValue(config.filePath);
        configMap[ZFValue("recordType")] = ZFValue(config.recordType);

        retMap[ZFValue("state")] = ZFValue(state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("config")] = ZFValue(configMap);
        retMap[ZFValue("channel")] = ZFValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedDataRecordProgressUpdate(
    EXPRESS::ZegoDataRecordProgress progress, EXPRESS::ZegoDataRecordConfig config,
    EXPRESS::ZegoPublishChannel channel) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        ZFMap progressMap;
        ZFMap configMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedDataRecordProgressUpdate");

        progressMap[ZFValue("currentFileSize")] = ZFValue((int32_t)progress.currentFileSize);
        progressMap[ZFValue("duration")] = ZFValue((int32_t)progress.duration);
        progressMap[ZFValue("quality")] = ZFValue(convertPublishQuality(progress.quality));

        configMap[ZFValue("filePath")] = ZFValue(config.filePath);
        configMap[ZFValue("recordType")] = ZFValue(config.recordType);

        retMap[ZFValue("config")] = ZFValue(configMap);
        retMap[ZFValue("progress")] = ZFValue(progressMap);
        retMap[ZFValue("channel")] = ZFValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onDownloadProgressUpdate(
    EXPRESS::IZegoCopyrightedMusic *copyrightedMusic, const std::string &resourceID,
    float progressRate) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onDownloadProgressUpdate");

        retMap[ZFValue("resourceID")] = ZFValue(resourceID);
        retMap[ZFValue("progressRate")] = ZFValue(progressRate);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCurrentPitchValueUpdate(
    EXPRESS::IZegoCopyrightedMusic *copyrightedMusic, const std::string &resourceID,
    int currentDuration, int pitchValue) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onCurrentPitchValueUpdate");

        retMap[ZFValue("resourceID")] = ZFValue(resourceID);
        retMap[ZFValue("currentDuration")] = ZFValue(currentDuration);
        retMap[ZFValue("pitchValue")] = ZFValue(pitchValue);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkTimeSynchronized() {

    ZF::logInfo("[onNetworkTimeSynchronized]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onNetworkTimeSynchronized");
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRequestDumpData() {
    ZF::logInfo("[onRequestDumpData]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRequestDumpData");

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRequestUploadDumpData(const std::string &dumpDir,
                                                            bool takePhoto) {
    ZF::logInfo("[onRequestUploadDumpData]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRequestUploadDumpData");
        retMap[ZFValue("dumpDir")] = ZFValue(dumpDir);
        retMap[ZFValue("takePhoto")] = ZFValue(takePhoto);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onStartDumpData(int errorCode) {
    ZF::logInfo("[onStartDumpData]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onStartDumpData");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onStopDumpData(int errorCode, const std::string &dumpDir) {
    ZF::logInfo("[onStopDumpData]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onStopDumpData");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("dumpDir")] = ZFValue(dumpDir);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUploadDumpData(int errorCode) {
    ZF::logInfo("[onUploadDumpData]");

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onUploadDumpData");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomTokenWillExpire(const std::string &roomID,
                                                          int remainTimeInSecond) {

    ZF::logInfo("[onRoomTokenWillExpire] roomID: %s, remainTimeInSecond: %d", roomID.c_str(),
                remainTimeInSecond);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRoomTokenWillExpire");

        retMap[ZFValue("roomID")] = ZFValue(roomID);
        retMap[ZFValue("remainTimeInSecond")] = ZFValue(remainTimeInSecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherCapturedVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherCapturedVideoFirstFrame");

        retMap[ZFValue("channel")] = ZFValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherSendVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherSendVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherSendVideoFirstFrame");

        retMap[ZFValue("channel")] = ZFValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRenderVideoFirstFrame(
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherRenderVideoFirstFrame] channel: %d", channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherRenderVideoFirstFrame");

        retMap[ZFValue("channel")] = ZFValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherVideoSizeChanged(
    int width, int height, EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherVideoSizeChanged] width: %d, height: %d, channel: %d", width, height,
                channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherVideoSizeChanged");

        retMap[ZFValue("width")] = ZFValue(width);
        retMap[ZFValue("height")] = ZFValue(height);
        retMap[ZFValue("channel")] = ZFValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRelayCDNStateUpdate(
    const std::string &streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> &infoList) {

    ZF::logInfo("[onPublisherRelayCDNStateUpdate] streamID: %s, infoListCount: %d",
                streamID.c_str(), infoList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherRelayCDNStateUpdate");

        retMap[ZFValue("streamID")] = ZFValue(streamID);
        ZFArray infoListArray;
        for (auto info : infoList) {
            ZFMap infoMap;
            infoMap[ZFValue("url")] = ZFValue(info.url);
            infoMap[ZFValue("state")] = ZFValue((int32_t)info.state);
            infoMap[ZFValue("updateReason")] = ZFValue((int32_t)info.updateReason);
            infoMap[ZFValue("stateTime")] = ZFValue((int64_t)info.stateTime);
            infoListArray.push_back(ZFValue(infoMap));
        }
        retMap[ZFValue("streamInfoList")] = ZFValue(infoListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherVideoEncoderChanged(
    EXPRESS::ZegoVideoCodecID fromCodecID, EXPRESS::ZegoVideoCodecID toCodecID,
    EXPRESS::ZegoPublishChannel channel) {

    ZF::logInfo("[onPublisherVideoEncoderChanged] fromCodecID: %d, toCodecID: %d, channel: %d",
                fromCodecID, toCodecID, channel);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPublisherVideoEncoderChanged");

        retMap[ZFValue("fromCodecID")] = ZFValue((int32_t)fromCodecID);
        retMap[ZFValue("toCodecID")] = ZFValue((int32_t)toCodecID);
        retMap[ZFValue("channel")] = ZFValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvVideoFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRecvVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRecvVideoFirstFrame");

        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderVideoFirstFrame(const std::string &streamID) {

    ZF::logInfo("[onPlayerRenderVideoFirstFrame] streamID: %s", streamID.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerRenderVideoFirstFrame");

        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerVideoSizeChanged(const std::string &streamID, int width,
                                                             int height) {

    ZF::logInfo("[onPlayerVideoSizeChanged] streamID: %s, width: %d, height: %d", streamID.c_str(),
                width, height);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerVideoSizeChanged");

        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("width")] = ZFValue(width);
        retMap[ZFValue("height")] = ZFValue(height);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID,
                                                          const std::string &streamID) {

    ZF::logInfo("[onPlayerLowFpsWarning] streamID: %s, codecID: %d", streamID.c_str(), codecID);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPlayerLowFpsWarning");

        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("codecID")] = ZFValue((int32_t)codecID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAutoMixerSoundLevelUpdate(
    const std::unordered_map<std::string, float> &soundLevels) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAutoMixerSoundLevelUpdate");

        ZFMap soundLevelsMap;
        for (auto soundLevel : soundLevels) {
            soundLevelsMap[ZFValue(soundLevel.first)] = ZFValue((double)soundLevel.second);
        }
        retMap[ZFValue("soundLevels")] = ZFValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onVideoDeviceStateChanged(
    EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo &deviceInfo) {

    ZF::logInfo("[onVideoDeviceStateChanged] updateType: %d, deviceID: %s, deviceName: %s",
                updateType, deviceInfo.deviceID.c_str(), deviceInfo.deviceName.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onVideoDeviceStateChanged");

        retMap[ZFValue("updateType")] = ZFValue((int32_t)updateType);
        ZFMap deviceInfoMap;
        deviceInfoMap[ZFValue("deviceID")] = ZFValue(deviceInfo.deviceID);
        deviceInfoMap[ZFValue("deviceName")] = ZFValue(deviceInfo.deviceName);
        retMap[ZFValue("deviceInfo")] = ZFValue(deviceInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedSoundLevelInfoUpdate(
    const EXPRESS::ZegoSoundLevelInfo &soundLevelInfo) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedSoundLevelInfoUpdate");

        ZFMap soundLevelInfoMap;
        soundLevelInfoMap[ZFValue("soundLevel")] = ZFValue((double)soundLevelInfo.soundLevel);
        soundLevelInfoMap[ZFValue("vad")] = ZFValue(soundLevelInfo.vad);
        retMap[ZFValue("soundLevelInfo")] = ZFValue(soundLevelInfoMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelInfoUpdate(
    const std::unordered_map<std::string, EXPRESS::ZegoSoundLevelInfo> &soundLevelInfos) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteSoundLevelInfoUpdate");

        ZFMap soundLevelInfosMap;
        for (auto soundLevelInfo : soundLevelInfos) {
            ZFMap soundLevelInfoMap;
            soundLevelInfoMap[ZFValue("soundLevel")] =
                ZFValue((double)soundLevelInfo.second.soundLevel);
            soundLevelInfoMap[ZFValue("vad")] = ZFValue(soundLevelInfo.second.vad);
            soundLevelInfosMap[ZFValue(soundLevelInfo.first)] = ZFValue(soundLevelInfoMap);
        }

        retMap[ZFValue("soundLevelInfos")] = ZFValue(soundLevelInfosMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioSpectrumUpdate(
    const EXPRESS::ZegoAudioSpectrum &audioSpectrum) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onCapturedAudioSpectrumUpdate");

        ZFArray audioSpectrumArray;
        for (auto audioSpectrum_ : audioSpectrum) {
            audioSpectrumArray.push_back(ZFValue((double)audioSpectrum_));
        }

        retMap[ZFValue("audioSpectrum")] = ZFValue(audioSpectrumArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteAudioSpectrumUpdate(
    const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum> &audioSpectrums) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteAudioSpectrumUpdate");

        ZFMap audioSpectrumsMap;
        for (auto audioSpectrum : audioSpectrums) {
            ZFArray audioSpectrumArray;
            for (auto audioSpectrum_ : audioSpectrum.second) {
                audioSpectrumArray.push_back(ZFValue((double)audioSpectrum_));
            }
            audioSpectrumsMap[ZFValue(audioSpectrum.first)] = ZFValue(audioSpectrumArray);
        }

        retMap[ZFValue("audioSpectrums")] = ZFValue(audioSpectrumsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onLocalDeviceExceptionOccurred(
    EXPRESS::ZegoDeviceExceptionType exceptionType, EXPRESS::ZegoDeviceType deviceType,
    const std::string &deviceID) {

    ZF::logInfo("[onLocalDeviceExceptionOccurred] exceptionType: %d, deviceID: %s, deviceType: %d",
                exceptionType, deviceID.c_str(), deviceType);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onLocalDeviceExceptionOccurred");

        retMap[ZFValue("exceptionType")] = ZFValue((int32_t)exceptionType);
        retMap[ZFValue("deviceType")] = ZFValue((int32_t)deviceType);
        retMap[ZFValue("deviceID")] = ZFValue(deviceID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteCameraStateUpdate(
    const std::string &streamID, EXPRESS::ZegoRemoteDeviceState state) {

    ZF::logInfo("[onRemoteCameraStateUpdate] streamID: %s, state: %d", streamID.c_str(), state);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteCameraStateUpdate");

        retMap[ZFValue("state")] = ZFValue((int32_t)state);
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSpeakerStateUpdate(
    const std::string &streamID, EXPRESS::ZegoRemoteDeviceState state) {

    ZF::logInfo("[onRemoteSpeakerStateUpdate] streamID: %s, state: %d", streamID.c_str(), state);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRemoteSpeakerStateUpdate");

        retMap[ZFValue("state")] = ZFValue((int32_t)state);
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioVADStateUpdate(
    EXPRESS::ZegoAudioVADStableStateMonitorType type, EXPRESS::ZegoAudioVADType state) {

    ZF::logInfo("[onAudioVADStateUpdate] type: %d, state: %d", type, state);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAudioVADStateUpdate");

        retMap[ZFValue("type")] = ZFValue((int32_t)type);
        retMap[ZFValue("state")] = ZFValue((int32_t)state);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvBroadcastMessage(
    const std::string &roomID, std::vector<EXPRESS::ZegoBroadcastMessageInfo> messageList) {

    ZF::logInfo("[onIMRecvBroadcastMessage] roomID: %s, messageListCount: %d", roomID.c_str(),
                messageList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onIMRecvBroadcastMessage");

        retMap[ZFValue("roomID")] = ZFValue(roomID);
        ZFArray messageListArray;
        for (auto message : messageList) {
            ZFMap messageMap;
            messageMap[ZFValue("message")] = ZFValue(message.message);
            messageMap[ZFValue("messageID")] = ZFValue((int64_t)message.messageID);
            messageMap[ZFValue("sendTime")] = ZFValue((int64_t)message.sendTime);
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(message.fromUser.userID);
            userMap[ZFValue("userName")] = ZFValue(message.fromUser.userName);

            messageMap[ZFValue("fromUser")] = ZFValue(userMap);

            messageListArray.push_back(ZFValue(messageMap));
        }
        retMap[ZFValue("messageList")] = ZFValue(messageListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvBarrageMessage(
    const std::string &roomID, std::vector<EXPRESS::ZegoBarrageMessageInfo> messageList) {

    ZF::logInfo("[onIMRecvBarrageMessage] roomID: %s, messageListCount: %d", roomID.c_str(),
                messageList.size());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onIMRecvBarrageMessage");

        retMap[ZFValue("roomID")] = ZFValue(roomID);
        ZFArray messageListArray;
        for (auto message : messageList) {
            ZFMap messageMap;
            messageMap[ZFValue("message")] = ZFValue(message.message);
            messageMap[ZFValue("messageID")] = ZFValue(message.messageID);
            messageMap[ZFValue("sendTime")] = ZFValue((int64_t)message.sendTime);
            ZFMap userMap;
            userMap[ZFValue("userID")] = ZFValue(message.fromUser.userID);
            userMap[ZFValue("userName")] = ZFValue(message.fromUser.userName);

            messageMap[ZFValue("fromUser")] = ZFValue(userMap);

            messageListArray.push_back(ZFValue(messageMap));
        }
        retMap[ZFValue("messageList")] = ZFValue(messageListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvCustomCommand(const std::string &roomID,
                                                          EXPRESS::ZegoUser fromUser,
                                                          const std::string &command) {

    ZF::logInfo("[onIMRecvCustomCommand] roomID: %s, userID: %s, command: %s", roomID.c_str(),
                fromUser.userID.c_str(), command.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onIMRecvCustomCommand");

        retMap[ZFValue("roomID")] = ZFValue(roomID);
        ZFMap userMap;
        userMap[ZFValue("userID")] = ZFValue(fromUser.userID);
        userMap[ZFValue("userName")] = ZFValue(fromUser.userName);
        retMap[ZFValue("fromUser")] = ZFValue(userMap);
        retMap[ZFValue("command")] = ZFValue(command);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRecvRoomTransparentMessage(
    const std::string &roomID, const EXPRESS::ZegoRoomRecvTransparentMessage &message) {
    ZF::logInfo("[onRecvRoomTransparentMessage] roomID: %s, userID: %s", roomID.c_str(),
                message.sendUser.userID.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRecvRoomTransparentMessage");
        retMap[ZFValue("roomID")] = ZFValue(roomID);

        ZFMap userMap, messageMap;

        userMap[ZFValue("userID")] = ZFValue(message.sendUser.userID);
        userMap[ZFValue("userName")] = ZFValue(message.sendUser.userName);
        messageMap[ZFValue("sendUser")] = ZFValue(userMap);

        unsigned char *data = (unsigned char *)message.content.data();
        unsigned int data_length = (unsigned int)message.content.length();
        std::vector<uint8_t> dataArray(data, data + data_length);

        messageMap[ZFValue("content")] = ZFValue(dataArray);

        retMap[ZFValue("message")] = ZFValue(messageMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPerformanceStatusUpdate(
    const EXPRESS::ZegoPerformanceStatus &status) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onPerformanceStatusUpdate");

        ZFMap statusMap;
        statusMap[ZFValue("cpuUsageApp")] = ZFValue(status.cpuUsageApp);
        statusMap[ZFValue("cpuUsageSystem")] = ZFValue(status.cpuUsageSystem);
        statusMap[ZFValue("memoryUsageApp")] = ZFValue(status.memoryUsageApp);
        statusMap[ZFValue("memoryUsageSystem")] = ZFValue(status.memoryUsageSystem);
        statusMap[ZFValue("memoryUsedApp")] = ZFValue(status.memoryUsedApp);
        retMap[ZFValue("status")] = ZFValue(statusMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkModeChanged(EXPRESS::ZegoNetworkMode mode) {

    ZF::logInfo("[onNetworkModeChanged] mode: %d", mode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onNetworkModeChanged");
        retMap[ZFValue("mode")] = ZFValue((int32_t)mode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestError(
    int errorCode, EXPRESS::ZegoNetworkSpeedTestType type) {

    ZF::logInfo("[onNetworkSpeedTestError] errorCode: %d, type: %d", errorCode, type);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onNetworkSpeedTestError");
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);
        retMap[ZFValue("type")] = ZFValue((int32_t)type);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestQualityUpdate(
    const EXPRESS::ZegoNetworkSpeedTestQuality &quality, EXPRESS::ZegoNetworkSpeedTestType type) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onNetworkSpeedTestQualityUpdate");
        ZFMap qualityMap;
        qualityMap[ZFValue("connectCost")] = ZFValue((int64_t)quality.connectCost);
        qualityMap[ZFValue("rtt")] = ZFValue((int64_t)quality.rtt);
        qualityMap[ZFValue("packetLostRate")] = ZFValue(quality.packetLostRate);
        qualityMap[ZFValue("quality")] = ZFValue((int32_t)quality.quality);

        retMap[ZFValue("quality")] = ZFValue(qualityMap);
        retMap[ZFValue("type")] = ZFValue((int32_t)type);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRecvExperimentalAPI(const std::string &content) {

    ZF::logInfo("[onRecvExperimentalAPI] content: %s", content.c_str());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRecvExperimentalAPI");
        retMap[ZFValue("content")] = ZFValue(content);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkQuality(
    const std::string &userID, EXPRESS::ZegoStreamQualityLevel upstreamQuality,
    EXPRESS::ZegoStreamQualityLevel downstreamQuality) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onNetworkQuality");
        retMap[ZFValue("upstreamQuality")] = ZFValue((int32_t)upstreamQuality);
        retMap[ZFValue("downstreamQuality")] = ZFValue((int32_t)downstreamQuality);
        retMap[ZFValue("userID")] = ZFValue(userID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onReceiveRealTimeSequentialData(
    EXPRESS::IZegoRealTimeSequentialDataManager *manager, const unsigned char *data,
    unsigned int dataLength, const std::string &streamID) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onReceiveRealTimeSequentialData");
        retMap[ZFValue("realTimeSequentialDataManagerIndex")] = ZFValue(manager->getIndex());

        auto nonConstData = const_cast<unsigned char *>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[ZFValue("data")] = ZFValue(dataArray);
        retMap[ZFValue("streamID")] = ZFValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRangeAudioMicrophoneStateUpdate(
    EXPRESS::IZegoRangeAudio *rangeAudio, EXPRESS::ZegoRangeAudioMicrophoneState state,
    int errorCode) {

    ZF::logInfo("[onRangeAudioMicrophoneStateUpdate] index: %d, state: %d, errorCode: %d", 0, state,
                errorCode);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRangeAudioMicrophoneStateUpdate");
        retMap[ZFValue("state")] = ZFValue((int32_t)state);
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessCapturedAudioData(unsigned char *data,
                                                               unsigned int dataLength,
                                                               EXPRESS::ZegoAudioFrameParam *param,
                                                               double timestamp) {
    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onProcessCapturedAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataArray);
        retMap[ZFValue("dataLength")] = ZFValue((int64_t)dataLength);
        retMap[ZFValue("timestamp")] = ZFValue(timestamp);

        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param->sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param->channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(
    unsigned char *data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam *param,
    double timestamp) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onProcessCapturedAudioDataAfterUsedHeadphoneMonitor");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataArray);
        retMap[ZFValue("dataLength")] = ZFValue((int64_t)dataLength);
        retMap[ZFValue("timestamp")] = ZFValue(timestamp);

        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param->sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param->channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

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
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onProcessRemoteAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataArray);
        retMap[ZFValue("dataLength")] = ZFValue((int64_t)dataLength);
        retMap[ZFValue("streamID")] = ZFValue(streamID);
        retMap[ZFValue("timestamp")] = ZFValue(timestamp);

        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param->sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param->channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessPlaybackAudioData(unsigned char *data,
                                                               unsigned int dataLength,
                                                               EXPRESS::ZegoAudioFrameParam *param,
                                                               double timestamp) {

    // High frequency callbacks do not log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onProcessPlaybackAudioData");
        std::vector<uint8_t> dataArray(data, data + dataLength);
        retMap[ZFValue("data")] = ZFValue(dataArray);
        retMap[ZFValue("dataLength")] = ZFValue((int64_t)dataLength);
        retMap[ZFValue("timestamp")] = ZFValue(timestamp);

        ZFMap paramMap;
        paramMap[ZFValue("sampleRate")] = ZFValue((int32_t)param->sampleRate);
        paramMap[ZFValue("channel")] = ZFValue((int32_t)param->channel);
        retMap[ZFValue("param")] = ZFValue(paramMap);

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

    ZF::logInfo("[onExceptionOccurred] index: %d, exceptionType: %d", source->getIndex(),
                exceptionType);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onExceptionOccurred");
        retMap[ZFValue("screenCaptureSourceIndex")] = ZFValue(source->getIndex());
        retMap[ZFValue("exceptionType")] = ZFValue(static_cast<int32_t>(exceptionType));

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onWindowStateChanged(
    EXPRESS::IZegoScreenCaptureSource *source, EXPRESS::ZegoScreenCaptureWindowState windowState,
    EXPRESS::ZegoRect windowRect) {

    ZF::logInfo("[onWindowStateChanged] index: %d, windowState: %d", source->getIndex(),
                windowState);

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onWindowStateChanged");
        retMap[ZFValue("screenCaptureSourceIndex")] = ZFValue(source->getIndex());
        retMap[ZFValue("windowState")] = ZFValue(static_cast<int32_t>(windowState));

        ZFMap rectMap;
        rectMap[ZFValue("x")] = ZFValue(windowRect.x);
        rectMap[ZFValue("y")] = ZFValue(windowRect.y);
        rectMap[ZFValue("width")] = ZFValue(windowRect.width);
        rectMap[ZFValue("height")] = ZFValue(windowRect.height);

        retMap[ZFValue("windowRect")] = rectMap;

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRectChanged(EXPRESS::IZegoScreenCaptureSource *source,
                                                  EXPRESS::ZegoRect captureRect) {
    ZF::logInfo("[onRectChanged] index: %d", source->getIndex());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onRectChanged");
        retMap[ZFValue("screenCaptureSourceIndex")] = ZFValue(source->getIndex());

        ZFMap rectMap;
        rectMap[ZFValue("x")] = ZFValue(captureRect.x);
        rectMap[ZFValue("y")] = ZFValue(captureRect.y);
        rectMap[ZFValue("width")] = ZFValue(captureRect.width);
        rectMap[ZFValue("height")] = ZFValue(captureRect.height);

        retMap[ZFValue("captureRect")] = rectMap;

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onInit(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger,
                                           int errorCode) {
    ZF::logInfo("[onAIVoiceChangerInit] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAIVoiceChangerInit");
        retMap[ZFValue("aiVoiceChangerIndex")] = ZFValue(aiVoiceChanger->getIndex());
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUpdate(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger,
                                             int errorCode) {
    ZF::logInfo("[onAIVoiceChangerUpdate] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAIVoiceChangerUpdate");
        retMap[ZFValue("aiVoiceChangerIndex")] = ZFValue(aiVoiceChanger->getIndex());
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onGetSpeakerList(
    EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, int errorCode,
    const std::vector<EXPRESS::ZegoAIVoiceChangerSpeakerInfo> &speakerList) {
    ZF::logInfo("[onAIVoiceChangerGetSpeakerList] index: %d", aiVoiceChanger->getIndex());

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAIVoiceChangerGetSpeakerList");
        retMap[ZFValue("aiVoiceChangerIndex")] = ZFValue(aiVoiceChanger->getIndex());
        retMap[ZFValue("errorCode")] = ZFValue(errorCode);

        ZFArray speakerListArray;
        for (auto &speaker : speakerList) {
            ZFMap speakerMap;
            speakerMap[ZFValue("id")] = ZFValue(speaker.id);
            speakerMap[ZFValue("name")] = ZFValue(speaker.name);
            speakerListArray.emplace_back(ZFValue(speakerMap));
        }
        retMap[ZFValue("speakerList")] = ZFValue(speakerListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onUpdateProgress(
    EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, double percent,
    int fileIndex, int fileCount) {
    // Do not print log

    if (eventSink_) {
        ZFMap retMap;
        retMap[ZFValue("method")] = ZFValue("onAIVoiceChangerUpdateProgress");
        retMap[ZFValue("aiVoiceChangerIndex")] = ZFValue(aiVoiceChanger->getIndex());
        retMap[ZFValue("percent")] = ZFValue(percent);
        retMap[ZFValue("fileIndex")] = ZFValue(fileIndex);
        retMap[ZFValue("fileCount")] = ZFValue(fileCount);

        eventSink_->Success(retMap);
    }
}
