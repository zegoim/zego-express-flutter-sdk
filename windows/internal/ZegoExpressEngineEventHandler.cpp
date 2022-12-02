#include "ZegoExpressEngineEventHandler.h"
#include <flutter/encodable_value.h>
#include <memory>

std::shared_ptr<ZegoExpressEngineEventHandler> ZegoExpressEngineEventHandler::m_instance = nullptr;

void ZegoExpressEngineEventHandler::setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>>&& eventSink)
{
    eventSink_ = std::move(eventSink);
}

void ZegoExpressEngineEventHandler::clearEventSink()
{
    eventSink_.reset();
}

void ZegoExpressEngineEventHandler::onDebugError(int errorCode, const std::string& funcName, const std::string& info) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onDebugError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("funcName")] = FTValue(funcName);
        retMap[FTValue("info")] = FTValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onApiCalledResult(int errorCode, const std::string& funcName, const std::string& info) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onApiCalledResult");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("funcName")] = FTValue(funcName);
        retMap[FTValue("info")] = FTValue(info);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

    if (eventSink_) {
        flutter::EncodableMap retMap;
        retMap[FTValue("method")] = FTValue("onEngineStateUpdate");
        retMap[FTValue("state")] = FTValue(state);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStateUpdate(const std::string& roomID, EXPRESS::ZegoRoomState state, int errorCode, const std::string& extendedData) {

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

void ZegoExpressEngineEventHandler::onRoomStateChanged(const std::string& roomID, EXPRESS::ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStateChanged");
        retMap[FTValue("reason")] = FTValue(reason);
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("roomID")] = FTValue(roomID);

        std::string extendedData_ = extendedData.empty()? "{}": extendedData;
        retMap[FTValue("extendedData")] = FTValue(extendedData_);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomUserUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoUser>& userList) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomUserUpdate");
        retMap[FTValue("updateType")] = FTValue(updateType);
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray userListArray;
        for (auto& user : userList) {
            FTMap userMap;
            userMap[FTValue("userID")] = FTValue(user.userID);
            userMap[FTValue("userName")] = FTValue(user.userName);
            userListArray.emplace_back(FTValue(userMap));
        }
        retMap[FTValue("userList")] = FTValue(userListArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomOnlineUserCountUpdate(const std::string& roomID, int count) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomOnlineUserCountUpdate");
        retMap[FTValue("count")] = FTValue(count);
        retMap[FTValue("roomID")] = FTValue(roomID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomStreamUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoStream>& streamList, const std::string& extendedData) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStreamUpdate");
        retMap[FTValue("updateType")] = FTValue(updateType);
        retMap[FTValue("roomID")] = FTValue(roomID);
        retMap[FTValue("extendedData")] = FTValue(extendedData);

        FTArray streamListArray;
        for (auto& stream : streamList) {
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

void ZegoExpressEngineEventHandler::onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoStream>& streamList) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomStreamExtraInfoUpdate");
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray streamListArray;
        for (auto& stream : streamList) {
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

void ZegoExpressEngineEventHandler::onRoomExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo>& roomExtraInfoList) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomExtraInfoUpdate");
        retMap[FTValue("roomID")] = FTValue(roomID);

        FTArray roomExtraInfoListArray;
        for (auto& roomExtraInfo : roomExtraInfoList) {
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

void ZegoExpressEngineEventHandler::onPublisherStateUpdate(const std::string& streamID, EXPRESS::ZegoPublisherState state, int errorCode, const std::string& extendedData) {

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

void ZegoExpressEngineEventHandler::onPublisherQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPublishStreamQuality& quality) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherQualityUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);

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

        retMap[FTValue("quality")] = FTValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedAudioFirstFrame() {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherCapturedAudioFirstFrame");

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) {

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

void ZegoExpressEngineEventHandler::onPlayerStateUpdate(const std::string& streamID, EXPRESS::ZegoPlayerState state, int errorCode, const std::string& extendedData) {

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

void ZegoExpressEngineEventHandler::onPlayerQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPlayStreamQuality& quality) {

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

        retMap[FTValue("quality")] = FTValue(qualityMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerMediaEvent(const std::string& streamID, EXPRESS::ZegoPlayerMediaEvent event) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerMediaEvent");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("event")] = flutter::EncodableValue(event);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioFirstFrame(const std::string& streamID) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvAudioFirstFrame");
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvSEI");
        retMap[FTValue("streamID")] = FTValue(streamID);

        auto nonConstData = const_cast<unsigned char*>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvAudioSideInfo(const std::string& streamID, const unsigned char* data, unsigned int dataLength) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvAudioSideInfo");
        retMap[FTValue("streamID")] = FTValue(streamID);

        auto nonConstData = const_cast<unsigned char*>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerStreamEvent");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("eventID")] = FTValue((int32_t)eventID);
        retMap[FTValue("extraInfo")] = FTValue(extraInfo);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderCameraVideoFirstFrame(std::string streamID) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRenderCameraVideoFirstFrame");
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerVideoSuperResolutionUpdate(std::string streamID,EXPRESS::ZegoSuperResolutionState state,int errorCode) {
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerVideoSuperResolutionUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerRelayCDNStateUpdate");
        retMap[FTValue("taskID")] = FTValue(taskID);

        FTArray infoListArray;
        for (auto& info : infoList) {
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

void ZegoExpressEngineEventHandler::onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) {
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMixerSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto& soundlevel : soundLevels) {
            soundLevelsMap[FTValue((int32_t)soundlevel.first)] = FTValue(soundlevel.second);
        }
        retMap[FTValue("soundLevels")] = FTValue(soundLevelsMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {

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

void ZegoExpressEngineEventHandler::onAudioDeviceVolumeChanged(EXPRESS::ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume) {

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

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCapturedSoundLevelUpdate");
        retMap[FTValue("soundLevel")] = FTValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSoundLevelUpdate");

        FTMap soundLevelsMap;
        for (auto& soundlevel : soundLevels) {
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

void ZegoExpressEngineEventHandler::onRemoteMicStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteMicStateUpdate");
        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("state")] = FTValue(state);

        eventSink_->Success(retMap);
    }
}

// Audio Effect Player
void ZegoExpressEngineEventHandler::onAudioEffectPlayStateUpdate(EXPRESS::IZegoAudioEffectPlayer* audioEffectPlayer, unsigned int audioEffectID, EXPRESS::ZegoAudioEffectPlayState state, int errorCode)
{
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

void ZegoExpressEngineEventHandler::onMediaPlayerStateUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerState state, int errorCode)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerStateUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        retMap[FTValue("state")] = FTValue(state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerNetworkEvent(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerNetworkEvent networkEvent)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerNetworkEvent");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        retMap[FTValue("networkEvent")] = FTValue(networkEvent);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerPlayingProgress(EXPRESS::IZegoMediaPlayer* mediaPlayer, unsigned long long millisecond)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerPlayingProgress");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());
        // TODO: convert need test?
        retMap[FTValue("millisecond")] = FTValue((int64_t)millisecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerRecvSEI(EXPRESS::IZegoMediaPlayer* mediaPlayer, const unsigned char* data, unsigned int dataLength)
{
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

void ZegoExpressEngineEventHandler::onMediaPlayerSoundLevelUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, float soundLevel)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerSoundLevelUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("soundLevel")] = FTValue(soundLevel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onMediaPlayerFrequencySpectrumUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, const EXPRESS::ZegoAudioSpectrum& spectrumList)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onMediaPlayerFrequencySpectrumUpdate");
        retMap[FTValue("mediaPlayerIndex")] = FTValue(mediaPlayer->getIndex());

        retMap[FTValue("soundLevel")] = FTValue(spectrumList);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
    if(eventSink_) {
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

void ZegoExpressEngineEventHandler::onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
    if(eventSink_) {
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

void ZegoExpressEngineEventHandler::onMixedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param)
{
    if(eventSink_) {
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

void ZegoExpressEngineEventHandler::onPlayerAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param, const std::string& streamID)
{
    if(eventSink_) {
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

void ZegoExpressEngineEventHandler::onCapturedDataRecordStateUpdate(EXPRESS::ZegoDataRecordState state, int errorCode, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel)
{
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

void ZegoExpressEngineEventHandler::onCapturedDataRecordProgressUpdate(EXPRESS::ZegoDataRecordProgress progress, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        FTMap progressMap;
        FTMap configMap;
        retMap[FTValue("method")] = FTValue("onCapturedDataRecordProgressUpdate");

        progressMap[FTValue("currentFileSize")] = FTValue((int32_t)progress.currentFileSize);
        progressMap[FTValue("duration")] = FTValue((int32_t)progress.duration);

        configMap[FTValue("filePath")] = FTValue(config.filePath);
        configMap[FTValue("recordType")] = FTValue(config.recordType);

        retMap[FTValue("config")] = FTValue(configMap);
        retMap[FTValue("progress")] = FTValue(progressMap);
        retMap[FTValue("channel")] = FTValue(channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onDownloadProgressUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, float progressRate)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onDownloadProgressUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("progressRate")] = FTValue(progressRate);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCurrentPitchValueUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, int currentDuration, int pitchValue) 
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onCurrentPitchValueUpdate");

        retMap[FTValue("resourceID")] = FTValue(resourceID);
        retMap[FTValue("currentDuration")] = FTValue(currentDuration);
        retMap[FTValue("pitchValue")] = FTValue(pitchValue);

        eventSink_->Success(retMap);
    }
}


void ZegoExpressEngineEventHandler::onNetworkTimeSynchronized()
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkTimeSynchronized");
        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRoomTokenWillExpire(const std::string & roomID, int remainTimeInSecond)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRoomTokenWillExpire");

        retMap[FTValue("roomID")] = FTValue(roomID);
        retMap[FTValue("remainTimeInSecond")] = FTValue(remainTimeInSecond);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherCapturedVideoFirstFrame(EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherCapturedVideoFirstFrame");

        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRenderVideoFirstFrame(EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherRenderVideoFirstFrame");

        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherVideoSizeChanged(int width, int height, EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherVideoSizeChanged");

        retMap[FTValue("width")] = FTValue(width);
        retMap[FTValue("height")] = FTValue(height);
        retMap[FTValue("channel")] = FTValue((int)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPublisherRelayCDNStateUpdate(const std::string & streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> & infoList)
{
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

void ZegoExpressEngineEventHandler::onPublisherVideoEncoderChanged(EXPRESS::ZegoVideoCodecID fromCodecID, EXPRESS::ZegoVideoCodecID toCodecID, EXPRESS::ZegoPublishChannel channel)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPublisherVideoEncoderChanged");

        retMap[FTValue("fromCodecID")] = FTValue((int32_t)fromCodecID);
        retMap[FTValue("toCodecID")] = FTValue((int32_t)toCodecID);
        retMap[FTValue("channel")] = FTValue((int32_t)channel);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRecvVideoFirstFrame(const std::string & streamID)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRecvVideoFirstFrame");

        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerRenderVideoFirstFrame(const std::string & streamID)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerRenderVideoFirstFrame");

        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerVideoSizeChanged(const std::string & streamID, int width, int height)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerVideoSizeChanged");

        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("width")] = FTValue(width);
        retMap[FTValue("height")] = FTValue(height);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onPlayerLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID, const std::string & streamID)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onPlayerLowFpsWarning");

        retMap[FTValue("streamID")] = FTValue(streamID);
        retMap[FTValue("codecID")] = FTValue((int32_t)codecID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAutoMixerSoundLevelUpdate(const std::unordered_map<std::string, float> & soundLevels)
{
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

void ZegoExpressEngineEventHandler::onVideoDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo & deviceInfo)
{
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

void ZegoExpressEngineEventHandler::onCapturedSoundLevelInfoUpdate(const EXPRESS::ZegoSoundLevelInfo & soundLevelInfo)
{
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

void ZegoExpressEngineEventHandler::onRemoteSoundLevelInfoUpdate( const std::unordered_map<std::string, EXPRESS::ZegoSoundLevelInfo> & soundLevelInfos)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSoundLevelInfoUpdate");

        FTMap soundLevelInfosMap;
        for (auto soundLevelInfo : soundLevelInfos) {
            FTMap soundLevelInfoMap;
            soundLevelInfoMap[FTValue("soundLevel")] = FTValue((double)soundLevelInfo.second.soundLevel);
            soundLevelInfoMap[FTValue("vad")] = FTValue(soundLevelInfo.second.vad);
            soundLevelInfosMap[FTValue(soundLevelInfo.first)] = FTValue(soundLevelInfoMap);
        }

        retMap[FTValue("soundLevelInfos")] = FTValue(soundLevelInfosMap);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onCapturedAudioSpectrumUpdate(const EXPRESS::ZegoAudioSpectrum & audioSpectrum)
{
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

void ZegoExpressEngineEventHandler::onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum> & audioSpectrums)
{
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

void ZegoExpressEngineEventHandler::onLocalDeviceExceptionOccurred(EXPRESS::ZegoDeviceExceptionType exceptionType, EXPRESS::ZegoDeviceType deviceType, const std::string & deviceID)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onLocalDeviceExceptionOccurred");

        retMap[FTValue("exceptionType")] = FTValue((int32_t)exceptionType);
        retMap[FTValue("deviceType")] = FTValue((int32_t)deviceType);
        retMap[FTValue("deviceID")] = FTValue(deviceID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteCameraStateUpdate(const std::string & streamID, EXPRESS::ZegoRemoteDeviceState state)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteCameraStateUpdate");

        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRemoteSpeakerStateUpdate(const std::string & streamID, EXPRESS::ZegoRemoteDeviceState state)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRemoteSpeakerStateUpdate");

        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onAudioVADStateUpdate(EXPRESS::ZegoAudioVADStableStateMonitorType type, EXPRESS::ZegoAudioVADType state)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onAudioVADStateUpdate");

        retMap[FTValue("type")] = FTValue((int32_t)type);
        retMap[FTValue("state")] = FTValue((int32_t)state);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onIMRecvBroadcastMessage(const std::string & roomID, std::vector<EXPRESS::ZegoBroadcastMessageInfo> messageList)
{
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

void ZegoExpressEngineEventHandler::onIMRecvBarrageMessage(const std::string & roomID, std::vector<EXPRESS::ZegoBarrageMessageInfo> messageList)
{
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

void ZegoExpressEngineEventHandler::onIMRecvCustomCommand(const std::string & roomID, EXPRESS::ZegoUser fromUser, const std::string & command)
{
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

void ZegoExpressEngineEventHandler::onPerformanceStatusUpdate(const EXPRESS::ZegoPerformanceStatus & status)
{
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

void ZegoExpressEngineEventHandler::onNetworkModeChanged(EXPRESS::ZegoNetworkMode mode)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkModeChanged");
        retMap[FTValue("mode")] = FTValue((int32_t)mode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestError(int errorCode, EXPRESS::ZegoNetworkSpeedTestType type)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkSpeedTestError");
        retMap[FTValue("errorCode")] = FTValue(errorCode);
        retMap[FTValue("type")] = FTValue((int32_t)type);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkSpeedTestQualityUpdate(const EXPRESS::ZegoNetworkSpeedTestQuality & quality, EXPRESS::ZegoNetworkSpeedTestType type)
{
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

void ZegoExpressEngineEventHandler::onRecvExperimentalAPI(const std::string & content)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRecvExperimentalAPI");
        retMap[FTValue("content")] = FTValue(content);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onNetworkQuality(const std::string & userID, EXPRESS::ZegoStreamQualityLevel upstreamQuality, EXPRESS::ZegoStreamQualityLevel downstreamQuality)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onNetworkQuality");
        retMap[FTValue("upstreamQuality")] = FTValue((int32_t)upstreamQuality);
        retMap[FTValue("downstreamQuality")] = FTValue((int32_t)downstreamQuality);
        retMap[FTValue("userID")] = FTValue(userID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onReceiveRealTimeSequentialData(EXPRESS::IZegoRealTimeSequentialDataManager * manager, const unsigned char * data, unsigned int dataLength, const std::string & streamID)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onReceiveRealTimeSequentialData");
        retMap[FTValue("realTimeSequentialDataManagerIndex")] = FTValue(manager->getIndex());

        auto nonConstData = const_cast<unsigned char*>(data);
        std::vector<uint8_t> dataArray(nonConstData, nonConstData + dataLength);

        retMap[FTValue("data")] = FTValue(dataArray);
        retMap[FTValue("streamID")] = FTValue(streamID);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onRangeAudioMicrophoneStateUpdate(EXPRESS::IZegoRangeAudio * rangeAudio, EXPRESS::ZegoRangeAudioMicrophoneState state, int errorCode)
{
    if (eventSink_) {
        FTMap retMap;
        retMap[FTValue("method")] = FTValue("onRangeAudioMicrophoneStateUpdate");
        retMap[FTValue("state")] = FTValue((int32_t)state);
        retMap[FTValue("errorCode")] = FTValue(errorCode);

        eventSink_->Success(retMap);
    }
}

void ZegoExpressEngineEventHandler::onProcessCapturedAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp)
{
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

void ZegoExpressEngineEventHandler::onProcessCapturedAudioDataAfterUsedHeadphoneMonitor( unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp)
{
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

void ZegoExpressEngineEventHandler::onProcessRemoteAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, const std::string & streamID, double timestamp)
{
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

void ZegoExpressEngineEventHandler::onProcessPlaybackAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp)
{
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