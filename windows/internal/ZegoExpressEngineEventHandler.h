#pragma once

#include <memory>
#include <flutter/event_channel.h>

#include <ZegoExpressSDK.h>
using namespace ZEGO;

class ZegoExpressEngineEventHandler : EXPRESS::IZegoEventHandler
{
public:
    ~ZegoExpressEngineEventHandler(){}
    ZegoExpressEngineEventHandler() {}

    /*static std::shared_ptr<ZegoExpressEngineEventHandler> getInstance()
    {
        static std::shared_ptr<ZegoExpressEngineEventHandler> m_instance = std::make_shared<ZegoExpressEngineEventHandler>();
        return m_instance;
    }*/

    void setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&eventSink);
    void clearEventSink();

protected:
    virtual void onDebugError(int errorCode, const std::string& funcName, const std::string& info) {

    }

    virtual void onEngineStateUpdate(EXPRESS::ZegoEngineState state) {

    }

    virtual void onRoomStateUpdate(const std::string& roomID, EXPRESS::ZegoRoomState state, int errorCode, const std::string& extendedData) {

    }

    virtual void onRoomUserUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoUser>& userList) {

    }

    virtual void onRoomOnlineUserCountUpdate(const std::string& roomID, int count) {

    }

    virtual void onRoomStreamUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoStream>& streamList, const std::string& extendedData) {

    }

    virtual void onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoStream>& streamList) {

    }

    virtual void onRoomExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo>& roomExtraInfoList) {

    }

    virtual void onPublisherStateUpdate(const std::string& streamID, EXPRESS::ZegoPublisherState state, int errorCode, const std::string& extendedData) {

    }

    virtual void onPublisherQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPublishStreamQuality& quality) {

    }

    virtual void onPublisherCapturedAudioFirstFrame() {

    }

    virtual void onPublisherCapturedVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) {

    }

    virtual void onPublisherRenderVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) {

    }

    virtual void onPublisherVideoSizeChanged(int /*width*/, int /*height*/, EXPRESS::ZegoPublishChannel channel) {

    }

    virtual void onPublisherRelayCDNStateUpdate(const std::string& streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {

    }

    virtual void onPlayerStateUpdate(const std::string& streamID, EXPRESS::ZegoPlayerState state, int errorCode, const std::string& extendedData) {

    }

    virtual void onPlayerQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPlayStreamQuality& quality) {

    }

    virtual void onPlayerMediaEvent(const std::string& streamID, EXPRESS::ZegoPlayerMediaEvent event) {

    }

    virtual void onPlayerRecvAudioFirstFrame(const std::string& streamID) {

    }

    virtual void onPlayerRecvVideoFirstFrame(const std::string& streamID) {

    }

    virtual void onPlayerRenderVideoFirstFrame(const std::string& streamID) {

    }

    virtual void onPlayerVideoSizeChanged(const std::string& streamID, int width, int height) {

    }

    virtual void onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength) {

    }

    virtual void onMixerRelayCDNStateUpdate(const std::string& /*taskID*/, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) {

    }

    virtual void onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) {

    }

    virtual void onAudioDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {

    }

    virtual void onAudioDeviceVolumeChanged(EXPRESS::ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume) {

    }

    virtual void onVideoDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo& deviceInfo) {

    }

    virtual void onCapturedSoundLevelUpdate(float soundLevel) {

    }

    virtual void onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels) {

    }

    virtual void onCapturedAudioSpectrumUpdate(const EXPRESS::ZegoAudioSpectrum& audioSpectrum) {

    }

    virtual void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum>& audioSpectrums) {

    }

    virtual void onDeviceError(int errorCode, const std::string& deviceName) {

    }

    virtual void onRemoteCameraStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

    }

    virtual void onRemoteMicStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) {

    }

private:
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> eventSink_;

private:
    //ZegoExpressEngineEventHandler() = default;
};