#pragma once

#include <memory>
#include <flutter/event_channel.h>
#include <flutter/encodable_value.h>

#include <ZegoExpressSDK.h>
using namespace ZEGO;

#define FTValue(varName) flutter::EncodableValue(varName)
#define FTMap flutter::EncodableMap
#define FTArray flutter::EncodableList

class ZegoExpressEngineEventHandler
    : public EXPRESS::IZegoEventHandler
    , public EXPRESS::IZegoAudioEffectPlayerEventHandler
    , public EXPRESS::IZegoMediaPlayerEventHandler
    , public EXPRESS::IZegoMediaDataPublisherEventHandler
    , public EXPRESS::IZegoAudioDataHandler
    , public EXPRESS::IZegoDataRecordEventHandler
    , public EXPRESS::IZegoApiCalledEventHandler
    , public EXPRESS::IZegoCopyrightedMusicEventHandler
    , public EXPRESS::IZegoRealTimeSequentialDataEventHandler
    , public EXPRESS::IZegoRangeAudioEventHandler
    , public EXPRESS::IZegoCustomAudioProcessHandler
    , public EXPRESS::IZegoScreenCaptureSourceEventHandler
    , public EXPRESS::IZegoAIVoiceChangerEventHandler
{
public:
    ~ZegoExpressEngineEventHandler(){ std::cout << "event handler destroy" << std::endl;  }
    ZegoExpressEngineEventHandler() { std::cout << "event handler create" << std::endl; }

    static std::shared_ptr<ZegoExpressEngineEventHandler>& getInstance()
    {
        if (!m_instance) {
            m_instance = std::shared_ptr<ZegoExpressEngineEventHandler>(new ZegoExpressEngineEventHandler);
        }

        return m_instance;
    }

    void setEventSink(std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> &&eventSink);
    void clearEventSink();

private:
    static std::shared_ptr<ZegoExpressEngineEventHandler> m_instance;

protected:
    void onDebugError(int errorCode, const std::string& funcName, const std::string& info) override;

    void onApiCalledResult(int errorCode, const std::string& funcName, const std::string& info) override;

    void onFatalError(int errorCode) override;

    void onEngineStateUpdate(EXPRESS::ZegoEngineState state) override;

    void onRoomStateUpdate(const std::string& roomID, EXPRESS::ZegoRoomState state, int errorCode, const std::string& extendedData) override;

    void onRoomStateChanged(const std::string& roomID, EXPRESS::ZegoRoomStateChangedReason reason, int errorCode, const std::string& extendedData) override;

    void onRoomUserUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoUser>& userList) override;

    void onRoomOnlineUserCountUpdate(const std::string& roomID, int count) override;

    void onRoomStreamUpdate(const std::string& roomID, EXPRESS::ZegoUpdateType updateType, const std::vector<EXPRESS::ZegoStream>& streamList, const std::string& extendedData) override;

    void onRoomStreamExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoStream>& streamList) override;

    void onRoomExtraInfoUpdate(const std::string& roomID, const std::vector<EXPRESS::ZegoRoomExtraInfo>& roomExtraInfoList) override;

    void onPublisherStateUpdate(const std::string& streamID, EXPRESS::ZegoPublisherState state, int errorCode, const std::string& extendedData) override;

    void onPublisherQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPublishStreamQuality& quality) override;

    void onPublisherCapturedAudioFirstFrame() override;

    void onPublisherSendAudioFirstFrame(EXPRESS::ZegoPublishChannel channel) override;
    
    void onPublisherStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) override;

    void onVideoObjectSegmentationStateChanged(EXPRESS::ZegoObjectSegmentationState state, EXPRESS::ZegoPublishChannel channel, int errorCode) override;

    void onPublisherLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID, EXPRESS::ZegoPublishChannel channel) override;
    
    void onPublisherDummyCaptureImagePathError(int errorCode, const std::string& path, EXPRESS::ZegoPublishChannel channel) override;

    void onPlayerStateUpdate(const std::string& streamID, EXPRESS::ZegoPlayerState state, int errorCode, const std::string& extendedData) override;

    void onPlayerQualityUpdate(const std::string& streamID, const EXPRESS::ZegoPlayStreamQuality& quality) override;

    void onPlayerMediaEvent(const std::string& streamID, EXPRESS::ZegoPlayerMediaEvent event) override;

    void onPlayerRecvAudioFirstFrame(const std::string& streamID) override;

    void onPlayerRecvSEI(const std::string& streamID, const unsigned char* data, unsigned int dataLength) override;

    void onPlayerRecvMediaSideInfo(const EXPRESS::ZegoMediaSideInfo & info) override;

    void onPlayerRecvAudioSideInfo(const std::string& streamID, const unsigned char* data, unsigned int dataLength) override;

    void onPlayerStreamEvent(EXPRESS::ZegoStreamEvent eventID, const std::string& streamID, const std::string& extraInfo) override;

    void onPlayerRenderCameraVideoFirstFrame(const std::string& streamID) override;

    // void onPlayerVideoSuperResolutionUpdate(std::string streamID,EXPRESS::ZegoSuperResolutionState state,int errorCode) override;

    void onMixerRelayCDNStateUpdate(const std::string& taskID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo>& infoList) override;

    void onMixerSoundLevelUpdate(const std::unordered_map<unsigned int, float>& soundLevels) override;

    void onAudioDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, EXPRESS::ZegoAudioDeviceType deviceType, const EXPRESS::ZegoDeviceInfo& deviceInfo) override;

    void onAudioDeviceVolumeChanged(EXPRESS::ZegoAudioDeviceType deviceType, const std::string& deviceID, int volume);

    void onCapturedSoundLevelUpdate(float soundLevel) override;

    void onRemoteSoundLevelUpdate(const std::unordered_map<std::string, float>& soundLevels) override;

    void onRemoteMicStateUpdate(const std::string& streamID, EXPRESS::ZegoRemoteDeviceState state) override;


    void onNetworkTimeSynchronized() override;

    void onRequestDumpData() override;
    void onRequestUploadDumpData(const std::string &dumpDir, bool takePhoto) override;

    void onStartDumpData(int errorCode) override;

    void onStopDumpData(int errorCode, const std::string& dumpDir) override;
    
    void onUploadDumpData(int errorCode) override;

    void onRoomTokenWillExpire(const std::string & roomID, int remainTimeInSecond) override;

    void onPublisherCapturedVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) override;

    void onPublisherSendVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) override;

public:
    void onPublisherRenderVideoFirstFrame(EXPRESS::ZegoPublishChannel channel) override;

protected:
    void onPublisherVideoSizeChanged(int width, int height, EXPRESS::ZegoPublishChannel channel) override;

    void onPublisherRelayCDNStateUpdate(const std::string & streamID, const std::vector<EXPRESS::ZegoStreamRelayCDNInfo> & infoList) override;

    void onPublisherVideoEncoderChanged(EXPRESS::ZegoVideoCodecID fromCodecID, EXPRESS::ZegoVideoCodecID toCodecID, EXPRESS::ZegoPublishChannel channel) override;

    void onPlayerRecvVideoFirstFrame(const std::string & streamID) override;

    void onPlayerRenderVideoFirstFrame(const std::string & streamID) override;

    void onPlayerVideoSizeChanged(const std::string & streamID, int width, int height) override;

    void onPlayerLowFpsWarning(EXPRESS::ZegoVideoCodecID codecID, const std::string & streamID) override;

    void onAutoMixerSoundLevelUpdate(const std::unordered_map<std::string, float> & soundLevels) override;

    void onVideoDeviceStateChanged(EXPRESS::ZegoUpdateType updateType, const EXPRESS::ZegoDeviceInfo & deviceInfo) override;

    void onCapturedSoundLevelInfoUpdate(const EXPRESS::ZegoSoundLevelInfo & soundLevelInfo) override;

    void onRemoteSoundLevelInfoUpdate( const std::unordered_map<std::string, EXPRESS::ZegoSoundLevelInfo> & soundLevelInfos) override;

    void onCapturedAudioSpectrumUpdate(const EXPRESS::ZegoAudioSpectrum & audioSpectrum) override;

    void onRemoteAudioSpectrumUpdate(const std::unordered_map<std::string, EXPRESS::ZegoAudioSpectrum> & audioSpectrums) override;

    void onLocalDeviceExceptionOccurred(EXPRESS::ZegoDeviceExceptionType exceptionType, EXPRESS::ZegoDeviceType deviceType, const std::string & deviceID) override;

    void onRemoteCameraStateUpdate(const std::string & streamID, EXPRESS::ZegoRemoteDeviceState state) override;

    void onRemoteSpeakerStateUpdate(const std::string & streamID, EXPRESS::ZegoRemoteDeviceState state) override;

    void onAudioVADStateUpdate(EXPRESS::ZegoAudioVADStableStateMonitorType type, EXPRESS::ZegoAudioVADType state) override;

    void onIMRecvBroadcastMessage(const std::string & roomID, std::vector<EXPRESS::ZegoBroadcastMessageInfo> messageList) override;

    void onIMRecvBarrageMessage(const std::string & roomID, std::vector<EXPRESS::ZegoBarrageMessageInfo> messageList) override;

    void onIMRecvCustomCommand(const std::string & roomID, EXPRESS::ZegoUser fromUser, const std::string & command) override;
    
    void onRecvRoomTransparentMessage(const std::string & roomID, const EXPRESS::ZegoRoomRecvTransparentMessage& message) override;

    void onPerformanceStatusUpdate(const EXPRESS::ZegoPerformanceStatus & status) override;

    void onNetworkModeChanged(EXPRESS::ZegoNetworkMode mode) override;

    void onNetworkSpeedTestError(int errorCode, EXPRESS::ZegoNetworkSpeedTestType type) override;

    void onNetworkSpeedTestQualityUpdate(const EXPRESS::ZegoNetworkSpeedTestQuality & quality, EXPRESS::ZegoNetworkSpeedTestType type) override;

    void onRecvExperimentalAPI(const std::string & content) override;

    void onNetworkQuality(const std::string & userID, EXPRESS::ZegoStreamQualityLevel upstreamQuality, EXPRESS::ZegoStreamQualityLevel downstreamQuality) override;

// RealTimeSequentialData
protected:
    void onReceiveRealTimeSequentialData(EXPRESS::IZegoRealTimeSequentialDataManager * manager, const unsigned char * data, unsigned int dataLength, const std::string & streamID) override;

// rangeAudio
protected:
    void onRangeAudioMicrophoneStateUpdate(EXPRESS::IZegoRangeAudio * rangeAudio, EXPRESS::ZegoRangeAudioMicrophoneState state, int errorCode) override;

protected:
    void onAudioEffectPlayStateUpdate(EXPRESS::IZegoAudioEffectPlayer* audioEffectPlayer, unsigned int audioEffectID, EXPRESS::ZegoAudioEffectPlayState state, int errorCode) override;

protected:
    void onMediaPlayerStateUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerState state, int errorCode) override;

    void onMediaPlayerNetworkEvent(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerNetworkEvent networkEvent) override;

    void onMediaPlayerPlayingProgress(EXPRESS::IZegoMediaPlayer* mediaPlayer, unsigned long long millisecond) override;

    void onMediaPlayerRecvSEI(EXPRESS::IZegoMediaPlayer* mediaPlayer, const unsigned char* data, unsigned int dataLength) override;

    void onMediaPlayerSoundLevelUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, float soundLevel) override;

    void onMediaPlayerFrequencySpectrumUpdate(EXPRESS::IZegoMediaPlayer* mediaPlayer, const EXPRESS::ZegoAudioSpectrum& spectrumList) override;

public:
    void onMediaPlayerFirstFrameEvent(EXPRESS::IZegoMediaPlayer* mediaPlayer, EXPRESS::ZegoMediaPlayerFirstFrameEvent event) override;

protected:
    void onMediaPlayerRenderingProgress(EXPRESS::IZegoMediaPlayer* mediaPlayer, unsigned long long millisecond) override;

    void onMediaPlayerVideoSizeChanged(EXPRESS::IZegoMediaPlayer* mediaPlayer, int width, int height) override;

    void onMediaPlayerLocalCache(EXPRESS::IZegoMediaPlayer * mediaPlayer, int errorCode, const std::string &resource, const std::string & cachedFile) override;

// MediaDataPublisher
protected:
    void onMediaDataPublisherFileOpen(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) override;

    void onMediaDataPublisherFileClose(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, int errorCode, const std::string &path) override;

    void onMediaDataPublisherFileDataBegin(EXPRESS::IZegoMediaDataPublisher *mediaDataPublisher, const std::string &path) override;


protected:
    void onCapturedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param) override;

    void onPlaybackAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param) override;

    void onMixedAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param) override;

    void onPlayerAudioData(const unsigned char* data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam param, const std::string& streamID) override;

    void onCapturedDataRecordStateUpdate(EXPRESS::ZegoDataRecordState state, int errorCode, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel) override;

    void onCapturedDataRecordProgressUpdate(EXPRESS::ZegoDataRecordProgress progress, EXPRESS::ZegoDataRecordConfig config, EXPRESS::ZegoPublishChannel channel) override;

// CustomAudioProcessHandler
protected:
    void onProcessCapturedAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp) override;

    void onProcessCapturedAudioDataAfterUsedHeadphoneMonitor( unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp) override;

    void onProcessRemoteAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, const std::string & streamID, double timestamp) override;

    void onProcessPlaybackAudioData(unsigned char * data, unsigned int dataLength, EXPRESS::ZegoAudioFrameParam * param, double timestamp) override;
    
protected:

    void onDownloadProgressUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, float progressRate) override;

    void onCurrentPitchValueUpdate(EXPRESS::IZegoCopyrightedMusic* copyrightedMusic, const std::string& resourceID, int currentDuration, int pitchValue) override;

// ScreenCapture
protected:
    void onAvailableFrame(EXPRESS::IZegoScreenCaptureSource* source, const void * data, unsigned int dataLength, EXPRESS::ZegoVideoFrameParam param) override;

    void onExceptionOccurred(EXPRESS::IZegoScreenCaptureSource* source, EXPRESS::ZegoScreenCaptureSourceExceptionType exceptionType) override;

    void onWindowStateChanged(EXPRESS::IZegoScreenCaptureSource* source, EXPRESS::ZegoScreenCaptureWindowState windowState, EXPRESS::ZegoRect windowRect) override;

    void onRectChanged(EXPRESS::IZegoScreenCaptureSource* source, EXPRESS::ZegoRect captureRect) override;

// AIVoiceChanger
 protected:
    void onInit(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, int errorCode) override;

    void onUpdate(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, int errorCode) override;

    void onGetSpeakerList(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, int errorCode,
                          const std::vector<EXPRESS::ZegoAIVoiceChangerSpeakerInfo> &speakerList) override;

    void onUpdateProgress(EXPRESS::IZegoAIVoiceChanger *aiVoiceChanger, double percent,
                          int fileIndex, int fileCount) override;

private:
    flutter::EncodableMap convertPublishQuality(const EXPRESS::ZegoPublishStreamQuality &quality);

private:
    std::unique_ptr<flutter::EventSink<flutter::EncodableValue>> eventSink_;

};