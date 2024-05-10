//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import org.json.JSONObject;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import im.zego.zegoexpress.ZegoAIVoiceChanger;
import im.zego.zegoexpress.ZegoAudioEffectPlayer;
import im.zego.zegoexpress.ZegoCopyrightedMusic;
import im.zego.zegoexpress.ZegoMediaDataPublisher;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.ZegoRangeAudio;
import im.zego.zegoexpress.ZegoRealTimeSequentialDataManager;
import im.zego.zegoexpress.callback.IZegoAIVoiceChangerEventHandler;
import im.zego.zegoexpress.callback.IZegoApiCalledEventHandler;
import im.zego.zegoexpress.callback.IZegoAudioDataHandler;
import im.zego.zegoexpress.callback.IZegoAudioEffectPlayerEventHandler;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicEventHandler;
import im.zego.zegoexpress.callback.IZegoCustomAudioProcessHandler;
import im.zego.zegoexpress.callback.IZegoDataRecordEventHandler;
import im.zego.zegoexpress.callback.IZegoEventHandler;
import im.zego.zegoexpress.callback.IZegoMediaDataPublisherEventHandler;
import im.zego.zegoexpress.callback.IZegoMediaPlayerEventHandler;
import im.zego.zegoexpress.callback.IZegoRangeAudioEventHandler;
import im.zego.zegoexpress.callback.IZegoRealTimeSequentialDataEventHandler;
import im.zego.zegoexpress.constants.ZegoAudioEffectPlayState;
import im.zego.zegoexpress.constants.ZegoAudioRoute;
import im.zego.zegoexpress.constants.ZegoDataRecordState;
import im.zego.zegoexpress.constants.ZegoEngineState;
import im.zego.zegoexpress.constants.ZegoMediaPlayerFirstFrameEvent;
import im.zego.zegoexpress.constants.ZegoMediaPlayerNetworkEvent;
import im.zego.zegoexpress.constants.ZegoMediaPlayerState;
import im.zego.zegoexpress.constants.ZegoNetworkMode;
import im.zego.zegoexpress.constants.ZegoNetworkSpeedTestType;
import im.zego.zegoexpress.constants.ZegoObjectSegmentationState;
import im.zego.zegoexpress.constants.ZegoPlayerMediaEvent;
import im.zego.zegoexpress.constants.ZegoPlayerState;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoPublisherState;
import im.zego.zegoexpress.constants.ZegoRangeAudioMicrophoneState;
import im.zego.zegoexpress.constants.ZegoRemoteDeviceState;
import im.zego.zegoexpress.constants.ZegoRoomState;
import im.zego.zegoexpress.constants.ZegoRoomStateChangedReason;
import im.zego.zegoexpress.constants.ZegoScreenCaptureExceptionType;
import im.zego.zegoexpress.constants.ZegoStreamEvent;
import im.zego.zegoexpress.constants.ZegoStreamQualityLevel;
import im.zego.zegoexpress.constants.ZegoUpdateType;
import im.zego.zegoexpress.constants.ZegoAudioSampleRate;
import im.zego.zegoexpress.constants.ZegoVideoCodecID;
import im.zego.zegoexpress.constants.ZegoAudioVADType;
import im.zego.zegoexpress.constants.ZegoAudioVADStableStateMonitorType;
import im.zego.zegoexpress.constants.ZegoDeviceExceptionType;
import im.zego.zegoexpress.constants.ZegoDeviceType;
import im.zego.zegoexpress.constants.ZegoSuperResolutionState;
import im.zego.zegoexpress.entity.ZegoAIVoiceChangerSpeakerInfo;
import im.zego.zegoexpress.entity.ZegoAudioFrameParam;
import im.zego.zegoexpress.entity.ZegoBarrageMessageInfo;
import im.zego.zegoexpress.entity.ZegoBroadcastMessageInfo;
import im.zego.zegoexpress.entity.ZegoDataRecordConfig;
import im.zego.zegoexpress.entity.ZegoDataRecordProgress;
import im.zego.zegoexpress.entity.ZegoMediaSideInfo;
import im.zego.zegoexpress.entity.ZegoNetworkSpeedTestQuality;
import im.zego.zegoexpress.entity.ZegoPerformanceStatus;
import im.zego.zegoexpress.entity.ZegoPlayStreamQuality;
import im.zego.zegoexpress.entity.ZegoPublishStreamQuality;
import im.zego.zegoexpress.entity.ZegoRoomExtraInfo;
import im.zego.zegoexpress.entity.ZegoSoundLevelInfo;
import im.zego.zegoexpress.entity.ZegoStream;
import im.zego.zegoexpress.entity.ZegoStreamRelayCDNInfo;
import im.zego.zegoexpress.entity.ZegoUser;
import im.zego.zegoexpress.entity.ZegoRoomRecvTransparentMessage;
import io.flutter.plugin.common.EventChannel;
import android.os.Handler;
import android.os.Looper;

public class ZegoExpressEngineEventHandler {

    private volatile static ZegoExpressEngineEventHandler instance;

    public static ZegoExpressEngineEventHandler getInstance() {
        if (instance == null) {
            synchronized (ZegoExpressEngineEventHandler.class) {
                if (instance == null) {
                    instance = new ZegoExpressEngineEventHandler();
                }
            }
        }
        return instance;
    }

    private Handler mUIHandler = null;

    public ZegoExpressEngineEventHandler() {
        if(mUIHandler == null) {
            mUIHandler = new Handler(Looper.getMainLooper());
        }
    }

    EventChannel.EventSink sink;

    private boolean guardSink() {
        if (sink == null) {
            ZegoLog.error("FlutterEventSink is null");
            return true;
        }
        return false;
    }

    private HashMap<String, Object> convertPublishStreamQuality(ZegoPublishStreamQuality quality) {
        HashMap<String, Object> qualityMap = new HashMap<>();
        qualityMap.put("videoCaptureFPS", quality.videoCaptureFPS);
        qualityMap.put("videoEncodeFPS", quality.videoEncodeFPS);
        qualityMap.put("videoSendFPS", quality.videoSendFPS);
        qualityMap.put("videoKBPS", quality.videoKBPS);
        qualityMap.put("audioCaptureFPS", quality.audioCaptureFPS);
        qualityMap.put("audioSendFPS", quality.audioSendFPS);
        qualityMap.put("audioKBPS", quality.audioKBPS);
        qualityMap.put("rtt", quality.rtt);
        qualityMap.put("packetLostRate", quality.packetLostRate);
        qualityMap.put("level", quality.level.value());
        qualityMap.put("isHardwareEncode", quality.isHardwareEncode);
        qualityMap.put("videoCodecID", quality.videoCodecID.value());
        qualityMap.put("totalSendBytes", quality.totalSendBytes);
        qualityMap.put("audioSendBytes", quality.audioSendBytes);
        qualityMap.put("videoSendBytes", quality.videoSendBytes);

        return qualityMap;
    }

    IZegoApiCalledEventHandler apiCalledEventHandler = new IZegoApiCalledEventHandler() {
        @Override
        public void onApiCalledResult(int errorCode, String funcName, String info) {
            super.onApiCalledResult(errorCode, funcName, info);
            ZegoLog.log("[onApiCalledResult] errorCode: %d, funcName: %s, info: %s", errorCode, funcName, info);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onApiCalledResult");
            map.put("errorCode", errorCode);
            map.put("funcName", funcName);
            map.put("info", info);

            sink.success(map);
        }
    };

    IZegoEventHandler eventHandler = new IZegoEventHandler() {

        @Override
        public void onDebugError(int errorCode, String funcName, String info) {
            super.onDebugError(errorCode, funcName, info);
            ZegoLog.log("[onDebugError] errorCode: %d, funcName: %s, info: %s", errorCode, funcName, info);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onDebugError");
            map.put("errorCode", errorCode);
            map.put("funcName", funcName);
            map.put("info", info);

            sink.success(map);
        }

        @Override
        public void onFatalError(int errorCode) {
            super.onFatalError(errorCode);
            ZegoLog.log("[onFatalError] errorCode: %d", errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onFatalError");
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onEngineStateUpdate(ZegoEngineState state) {
            super.onEngineStateUpdate(state);
            ZegoLog.log("[onEngineStateUpdate] state: %s", state.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onEngineStateUpdate");
            map.put("state", state.value());

            sink.success(map);
        }

        @Override
        public void onNetworkTimeSynchronized() {
            super.onNetworkTimeSynchronized();
            ZegoLog.log("[onNetworkTimeSynchronized]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onNetworkTimeSynchronized");

            sink.success(map);
        }

        /* Room */

        @Override
        public void onRoomStateUpdate(String roomID, ZegoRoomState state, int errorCode, JSONObject extendedData) {
            super.onRoomStateUpdate(roomID, state, errorCode, extendedData);
            ZegoLog.log("[onRoomStateUpdate] roomID: %s, state: %s, errorCode: %d", roomID, state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomStateUpdate");
            map.put("roomID", roomID);
            map.put("state", state.value());
            map.put("errorCode", errorCode);
            map.put("extendedData", extendedData.toString());

            sink.success(map);
        }

        @Override
        public void onRoomStateChanged(String roomID, ZegoRoomStateChangedReason reason, int errorCode, JSONObject extendedData) {
            super.onRoomStateChanged(roomID, reason, errorCode, extendedData);
            ZegoLog.log("[onRoomStateChanged] roomID: %s, reason: %s, errorCode: %d", roomID, reason.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomStateChanged");
            map.put("roomID", roomID);
            map.put("reason", reason.value());
            map.put("errorCode", errorCode);
            map.put("extendedData", extendedData.toString());

            sink.success(map);
        }

        @Override
        public void onRoomUserUpdate(String roomID, ZegoUpdateType updateType, ArrayList<ZegoUser> userList) {
            super.onRoomUserUpdate(roomID, updateType, userList);
            ZegoLog.log("[onRoomUserUpdate] roomID: %s, updateType: %s, userListCount: %d", roomID, updateType.name(), userList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomUserUpdate");
            map.put("roomID", roomID);
            map.put("updateType", updateType.value());
            map.put("userList", this.mapListFromUserList(userList));

            sink.success(map);
        }

        @Override
        public void onRoomOnlineUserCountUpdate(String roomID, int count) {
            super.onRoomOnlineUserCountUpdate(roomID, count);
            ZegoLog.log("[onRoomOnlineUserCountUpdate] roomID: %s, count: %d", roomID, count);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomOnlineUserCountUpdate");
            map.put("roomID", roomID);
            map.put("count", count);

            sink.success(map);
        }

        @Override
        public void onRoomStreamUpdate(String roomID, ZegoUpdateType updateType, ArrayList<ZegoStream> streamList, JSONObject extendedData) {
            super.onRoomStreamUpdate(roomID, updateType, streamList, extendedData);
            ZegoLog.log("[onRoomStreamUpdate] roomID: %s, updateType: %s, streamListCount: %d, extendedDataLength: %d", roomID, updateType.name(), streamList.size(), extendedData.length());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomStreamUpdate");
            map.put("roomID", roomID);
            map.put("updateType", updateType.value());
            map.put("streamList", this.mapListFromStreamList(streamList));
            map.put("extendedData", extendedData.toString());

            sink.success(map);
        }

        @Override
        public void onRoomStreamExtraInfoUpdate(String roomID, ArrayList<ZegoStream> streamList) {
            super.onRoomStreamExtraInfoUpdate(roomID, streamList);
            ZegoLog.log("[onRoomStreamExtraInfoUpdate] roomID: %s, streamListCount: %d", roomID, streamList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomStreamExtraInfoUpdate");
            map.put("roomID", roomID);
            map.put("streamList", this.mapListFromStreamList(streamList));

            sink.success(map);
        }

        @Override
        public void onRoomExtraInfoUpdate(String roomID, ArrayList<ZegoRoomExtraInfo> roomExtraInfoList) {
            super.onRoomExtraInfoUpdate(roomID, roomExtraInfoList);
            ZegoLog.log("[onRoomExtraInfoUpdate] roomID: %s, roomExtraInfoListCount: %d", roomID, roomExtraInfoList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomExtraInfoUpdate");
            map.put("roomID", roomID);
            map.put("roomExtraInfoList", this.mapListFromRoomExtraInfoList(roomExtraInfoList));

            sink.success(map);
        }

        @Override
        public void onRoomTokenWillExpire(String roomID, int remainTimeInSecond) {
            super.onRoomTokenWillExpire(roomID, remainTimeInSecond);
            ZegoLog.log("[onRoomTokenWillExpire] roomID: %s, remainTimeInSecond: %d", roomID, remainTimeInSecond);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRoomTokenWillExpire");
            map.put("roomID", roomID);
            map.put("remainTimeInSecond", remainTimeInSecond);

            sink.success(map);
        }

        /* Publisher */

        @Override
        public void onPublisherStateUpdate(String streamID, ZegoPublisherState state, int errorCode, JSONObject extendedData) {
            super.onPublisherStateUpdate(streamID, state, errorCode, extendedData);
            ZegoLog.log("[onPublisherStateUpdate] streamID: %s, state: %s, errorCode: %d", streamID, state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherStateUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());
            map.put("errorCode", errorCode);
            map.put("extendedData", extendedData.toString());

            sink.success(map);
        }

        @Override
        public void onPublisherQualityUpdate(String streamID, ZegoPublishStreamQuality quality) {
            super.onPublisherQualityUpdate(streamID, quality);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> qualityMap = convertPublishStreamQuality(quality);

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherQualityUpdate");
            map.put("streamID", streamID);
            map.put("quality", qualityMap);

            sink.success(map);
        }

        @Override
        public void onPublisherCapturedAudioFirstFrame() {
            super.onPublisherCapturedAudioFirstFrame();
            ZegoLog.log("[onPublisherCapturedAudioFirstFrame]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherCapturedAudioFirstFrame");

            sink.success(map);
        }

        @Override
        public void onPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel) {
            super.onPublisherCapturedVideoFirstFrame(channel);
            ZegoLog.log("[onPublisherCapturedVideoFirstFrame] channel: %s", channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherCapturedVideoFirstFrame");
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherSendAudioFirstFrame(ZegoPublishChannel channel) {
            super.onPublisherSendAudioFirstFrame(channel);
            ZegoLog.log("[onPublisherSendAudioFirstFrame] channel: %s", channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherSendAudioFirstFrame");
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherSendVideoFirstFrame(ZegoPublishChannel channel) {
            super.onPublisherSendVideoFirstFrame(channel);
            ZegoLog.log("[onPublisherSendVideoFirstFrame] channel: %s", channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherSendVideoFirstFrame");
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherRenderVideoFirstFrame(ZegoPublishChannel channel) {
            super.onPublisherRenderVideoFirstFrame(channel);
            ZegoLog.log("[onPublisherRenderVideoFirstFrame] channel: %s", channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherRenderVideoFirstFrame");
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel) {
            super.onPublisherVideoSizeChanged(width, height, channel);
            ZegoLog.log("[onPublisherVideoSizeChanged] width: %d, height: %d, channel: %s", width, height, channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherVideoSizeChanged");
            map.put("width", width);
            map.put("height", height);
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherRelayCDNStateUpdate(String streamID, ArrayList<ZegoStreamRelayCDNInfo> streamInfoList) {
            super.onPublisherRelayCDNStateUpdate(streamID, streamInfoList);
            ZegoLog.log("[onPublisherRelayCDNStateUpdate] streamID: %s, streamInfoListCount: %d", streamID, streamInfoList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherRelayCDNStateUpdate");
            map.put("streamID", streamID);
            map.put("streamInfoList", this.mapListFromStreamRelayCdnInfoList(streamInfoList));

            sink.success(map);
        }

        @Override
        public void onPublisherVideoEncoderChanged(ZegoVideoCodecID fromCodecID, ZegoVideoCodecID toCodecID, ZegoPublishChannel channel) {
            super.onPublisherVideoEncoderChanged(fromCodecID, toCodecID, channel);
            ZegoLog.log("[onPublisherVideoEncoderChanged] fromCodecID: %s, toCodecID: %s, channel: %s", fromCodecID.name(), toCodecID.name(), channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPublisherVideoEncoderChanged");
            map.put("fromCodecID", fromCodecID.value());
            map.put("toCodecID", toCodecID.value());
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherStreamEvent(ZegoStreamEvent eventID, String streamID, String extraInfo) {
            super.onPublisherStreamEvent(eventID, streamID, extraInfo);
            ZegoLog.log("[onPublisherStreamEvent] eventID: %s, streamID: %s, extraInfo: %s", eventID.name(), streamID, extraInfo);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPublisherStreamEvent");
            map.put("eventID", eventID.value());
            map.put("streamID", streamID);
            map.put("extraInfo", extraInfo);

            sink.success(map);
        }

        @Override
        public void onVideoObjectSegmentationStateChanged(ZegoObjectSegmentationState state, ZegoPublishChannel channel, int errorCode) {
            super.onVideoObjectSegmentationStateChanged(state, channel, errorCode);
            ZegoLog.log("[onVideoObjectSegmentationStateChanged] state: %s, channel: %s, errorCode: %d", state.name(), channel.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onVideoObjectSegmentationStateChanged");
            map.put("state", state.value());
            map.put("channel", channel.value());
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onPublisherLowFpsWarning(ZegoVideoCodecID codecID, ZegoPublishChannel channel) {
            super.onPublisherLowFpsWarning(codecID, channel);
            ZegoLog.log("[onPublisherLowFpsWarning] codecID: %s, channel: %s", codecID.name(), channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPublisherLowFpsWarning");
            map.put("codecID", codecID.value());
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onPublisherDummyCaptureImagePathError(int errorCode, String path, ZegoPublishChannel channel) {
            super.onPublisherDummyCaptureImagePathError(errorCode, path, channel);
            ZegoLog.log("[onPublisherDummyCaptureImagePathError] errorCode: %d, path: %s, channel: %s", errorCode, path, channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPublisherDummyCaptureImagePathError");
            map.put("errorCode", errorCode);
            map.put("path", path);
            map.put("channel", channel.value());

            sink.success(map);
        }

        /* Player */

        @Override
        public void onPlayerStateUpdate(String streamID, ZegoPlayerState state, int errorCode, JSONObject extendedData) {
            super.onPlayerStateUpdate(streamID, state, errorCode, extendedData);
            ZegoLog.log("[onPlayerStateUpdate] streamID: %s, state: %s, errorCode: %d", streamID, state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerStateUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());
            map.put("errorCode", errorCode);
            map.put("extendedData", extendedData.toString());

            sink.success(map);
        }

        @Override
        public void onPlayerQualityUpdate(String streamID, ZegoPlayStreamQuality quality) {
            super.onPlayerQualityUpdate(streamID, quality);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> qualityMap = new HashMap<>();
            qualityMap.put("videoRecvFPS", quality.videoRecvFPS);
            qualityMap.put("videoDejitterFPS", quality.videoDejitterFPS);
            qualityMap.put("videoDecodeFPS", quality.videoDecodeFPS);
            qualityMap.put("videoRenderFPS", quality.videoRenderFPS);
            qualityMap.put("videoKBPS", quality.videoKBPS);
            qualityMap.put("videoBreakRate", quality.videoBreakRate);
            qualityMap.put("audioRecvFPS", quality.audioRecvFPS);
            qualityMap.put("audioDejitterFPS", quality.audioDejitterFPS);
            qualityMap.put("audioDecodeFPS", quality.audioDecodeFPS);
            qualityMap.put("audioRenderFPS", quality.audioRenderFPS);
            qualityMap.put("audioKBPS", quality.audioKBPS);
            qualityMap.put("audioBreakRate", quality.audioBreakRate);
            qualityMap.put("mos", quality.mos);
            qualityMap.put("rtt", quality.rtt);
            qualityMap.put("packetLostRate", quality.packetLostRate);
            qualityMap.put("peerToPeerDelay", quality.peerToPeerDelay);
            qualityMap.put("peerToPeerPacketLostRate", quality.peerToPeerPacketLostRate);
            qualityMap.put("level", quality.level.value());
            qualityMap.put("delay", quality.delay);
            qualityMap.put("avTimestampDiff", quality.avTimestampDiff);
            qualityMap.put("isHardwareDecode", quality.isHardwareDecode);
            qualityMap.put("videoCodecID", quality.videoCodecID.value());
            qualityMap.put("totalRecvBytes", quality.totalRecvBytes);
            qualityMap.put("audioRecvBytes", quality.audioRecvBytes);
            qualityMap.put("videoRecvBytes", quality.videoRecvBytes);
            qualityMap.put("audioCumulativeBreakCount", quality.audioCumulativeBreakCount);
            qualityMap.put("videoCumulativeBreakCount", quality.videoCumulativeBreakCount);
            qualityMap.put("audioCumulativeBreakTime", quality.audioCumulativeBreakTime);
            qualityMap.put("videoCumulativeBreakTime", quality.videoCumulativeBreakTime);
            qualityMap.put("audioCumulativeBreakRate", quality.audioCumulativeBreakRate);
            qualityMap.put("videoCumulativeBreakRate", quality.videoCumulativeBreakRate);
            qualityMap.put("audioCumulativeDecodeTime", quality.audioCumulativeDecodeTime);
            qualityMap.put("videoCumulativeDecodeTime", quality.videoCumulativeDecodeTime);
            qualityMap.put("muteVideo", quality.muteVideo);
            qualityMap.put("muteAudio", quality.muteAudio);

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerQualityUpdate");
            map.put("streamID", streamID);
            map.put("quality", qualityMap);

            sink.success(map);
        }

        @Override
        public void onPlayerMediaEvent(String streamID, ZegoPlayerMediaEvent event) {
            super.onPlayerMediaEvent(streamID, event);
            ZegoLog.log("[onPlayerMediaEvent] streamID: %s, event: %s", streamID, event.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerMediaEvent");
            map.put("streamID", streamID);
            map.put("event", event.value());

            sink.success(map);
        }

        @Override
        public void onPlayerRecvAudioFirstFrame(String streamID) {
            super.onPlayerRecvAudioFirstFrame(streamID);
            ZegoLog.log("[onPlayerRecvAudioFirstFrame] streamID: %s", streamID);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerRecvAudioFirstFrame");
            map.put("streamID", streamID);

            sink.success(map);
        }

        @Override
        public void onPlayerRecvVideoFirstFrame(String streamID) {
            super.onPlayerRecvVideoFirstFrame(streamID);
            ZegoLog.log("[onPlayerRecvVideoFirstFrame] streamID: %s", streamID);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerRecvVideoFirstFrame");
            map.put("streamID", streamID);

            sink.success(map);
        }

        @Override
        public void onPlayerRenderVideoFirstFrame(String streamID) {
            super.onPlayerRenderVideoFirstFrame(streamID);
            ZegoLog.log("[onPlayerRenderVideoFirstFrame] streamID: %s", streamID);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerRenderVideoFirstFrame");
            map.put("streamID", streamID);

            sink.success(map);
        }

        @Override
        public void onPlayerVideoSizeChanged(String streamID, int width, int height) {
            super.onPlayerVideoSizeChanged(streamID, width, height);
            ZegoLog.log("[onPlayerVideoSizeChanged] streamID: %s, width: %d, height: %d", streamID, width, height);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerVideoSizeChanged");
            map.put("streamID", streamID);
            map.put("width", width);
            map.put("height", height);

            sink.success(map);
        }

        @SuppressWarnings("deprecation")
        @Override
        public void onPlayerRecvSEI(String streamID, byte[] data) {
            super.onPlayerRecvSEI(streamID, data);
            // Do not log high frequency callback
            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerRecvSEI");
            map.put("streamID", streamID);
            map.put("data", data);

            sink.success(map);
        }

        @Override
        public void onPlayerRecvMediaSideInfo(ZegoMediaSideInfo info) {
            super.onPlayerRecvMediaSideInfo(info);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            byte[] data = new byte[info.SEIData.remaining()];
            info.SEIData.get(data, 0, info.SEIDataLength);

            map.put("method", "onPlayerRecvMediaSideInfo");
            map.put("streamID", info.streamID);
            map.put("SEIData", data);
            map.put("timestampNs", info.timestampNs);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });

        }

        @Override
        public void onPlayerRecvAudioSideInfo(String streamID, byte[] data) {
            super.onPlayerRecvAudioSideInfo(streamID, data);
            // Do not log high frequency callback
            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerRecvAudioSideInfo");
            map.put("streamID", streamID);
            map.put("data", data);

            sink.success(map);
        }

        @Override
        public void onPlayerLowFpsWarning(ZegoVideoCodecID codecID, String streamID) {
            super.onPlayerLowFpsWarning(codecID, streamID);
            ZegoLog.log("[onPlayerLowFpsWarning] ZegoVideoCodecID: %s, streamID: %s", codecID.name(), streamID);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerLowFpsWarning");
            map.put("codecID", codecID.value());
            map.put("streamID", streamID);

            sink.success(map);
        }

        @Override
        public void onPlayerStreamEvent(ZegoStreamEvent eventID, String streamID, String extraInfo) {
            super.onPlayerStreamEvent(eventID, streamID, extraInfo);
            ZegoLog.log("[onPlayerStreamEvent] eventID: %s, streamID: %s, extraInfo: %s", eventID.name(), streamID, extraInfo);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPlayerStreamEvent");
            map.put("eventID", eventID.value());
            map.put("streamID", streamID);
            map.put("extraInfo", extraInfo);

            sink.success(map);
        }

        @Override
        public void onPlayerRenderCameraVideoFirstFrame(String streamID) {
            super.onPlayerRenderCameraVideoFirstFrame(streamID);
            ZegoLog.log("[onPlayerRenderCameraVideoFirstFrame] streamID: %s", streamID);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPlayerRenderCameraVideoFirstFrame");
            map.put("streamID", streamID);

            sink.success(map);
        }

        @Override
        public void onPlayerVideoSuperResolutionUpdate(String streamID,ZegoSuperResolutionState state,int errorCode) {
            super.onPlayerVideoSuperResolutionUpdate(streamID, state, errorCode);
            ZegoLog.log("[onPlayerVideoSuperResolutionUpdate] streamID: %s, state: %s, errorCode: %d", streamID, state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPlayerVideoSuperResolutionUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        /* Mixer */

        @Override
        public void onMixerRelayCDNStateUpdate(String taskID, ArrayList<ZegoStreamRelayCDNInfo> infoList) {
            super.onMixerRelayCDNStateUpdate(taskID, infoList);
            ZegoLog.log("[onMixerRelayCDNStateUpdate] taskID: %s, infosCount: %d", taskID, infoList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMixerRelayCDNStateUpdate");
            map.put("infoList", mapListFromStreamRelayCdnInfoList(infoList));
            map.put("taskID", taskID);

            sink.success(map);
        }

        @Override
        public void onMixerSoundLevelUpdate(HashMap<Integer, Float> soundLevels) {
            super.onMixerSoundLevelUpdate(soundLevels);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMixerSoundLevelUpdate");
            map.put("soundLevels", soundLevels);

            sink.success(map);
        }

        @Override
        public void onAutoMixerSoundLevelUpdate(HashMap<String, Float> soundLevels) {
            super.onAutoMixerSoundLevelUpdate(soundLevels);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAutoMixerSoundLevelUpdate");
            map.put("soundLevels", soundLevels);

            sink.success(map);
        }


        /* Device */

        @Override
        public void onCapturedSoundLevelUpdate(float soundLevel) {
            super.onCapturedSoundLevelUpdate(soundLevel);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onCapturedSoundLevelUpdate");
            map.put("soundLevel", soundLevel);

            sink.success(map);
        }

        @Override
        public void onCapturedSoundLevelInfoUpdate(ZegoSoundLevelInfo soundLevelInfo) {
            super.onCapturedSoundLevelInfoUpdate(soundLevelInfo);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();


            HashMap<String, Object> soundLevelMap = new HashMap<>();
            soundLevelMap.put("soundLevel", soundLevelInfo.soundLevel);
            soundLevelMap.put("vad", soundLevelInfo.vad);

            map.put("method", "onCapturedSoundLevelInfoUpdate");
            map.put("soundLevelInfo", soundLevelMap);

            sink.success(map);
        }

        @Override
        public void onRemoteSoundLevelUpdate(HashMap<String, Float> soundLevels) {
            super.onRemoteSoundLevelUpdate(soundLevels);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRemoteSoundLevelUpdate");
            map.put("soundLevels", soundLevels);

            sink.success(map);
        }

        @Override
        public void onRemoteSoundLevelInfoUpdate(HashMap<String, ZegoSoundLevelInfo> soundLevelInfos) {
            super.onRemoteSoundLevelInfoUpdate(soundLevelInfos);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onRemoteSoundLevelInfoUpdate");

            HashMap<String, Object> soundLevelMaps = new HashMap<>();
            for (Map.Entry<String, ZegoSoundLevelInfo> entry : soundLevelInfos.entrySet()) {
                ZegoSoundLevelInfo soundLevelInfo = entry.getValue();

                HashMap<String, Object> soundLevelMap = new HashMap<>();
                soundLevelMap.put("soundLevel", soundLevelInfo.soundLevel);
                soundLevelMap.put("vad", soundLevelInfo.vad);
                soundLevelMaps.put(entry.getKey(), soundLevelMap);
            }

            map.put("soundLevelInfos", soundLevelMaps);
            sink.success(map);
        }

        @Override
        public void onCapturedAudioSpectrumUpdate(float[] audioSpectrum) {
            super.onCapturedAudioSpectrumUpdate(audioSpectrum);
            // Super high frequency callbacks do not log, do not guard sink

            ArrayList<Float> audioSpectrumList = new ArrayList<>();

            for (float spectrumValue: audioSpectrum) {
                audioSpectrumList.add(spectrumValue);
            }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onCapturedAudioSpectrumUpdate");
            map.put("audioSpectrum", audioSpectrumList);

            sink.success(map);
        }

        @Override
        public void onRemoteAudioSpectrumUpdate(HashMap<String, float[]> audioSpectrums) {
            super.onRemoteAudioSpectrumUpdate(audioSpectrums);
            // Super high frequency callbacks do not log, do not guard sink

            HashMap<String, ArrayList<Float>> audioSpectrumsMap = new HashMap<>();

            for (String streamID: audioSpectrums.keySet()) {
                ArrayList<Float> audioSpectrumList = new ArrayList<>();

                float[] audioSpectrum = audioSpectrums.get(streamID);
                if (audioSpectrum != null) {
                    for (float spectrumValue: audioSpectrum) {
                        audioSpectrumList.add(spectrumValue);
                    }
                    audioSpectrumsMap.put(streamID, audioSpectrumList);
                }
            }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRemoteAudioSpectrumUpdate");
            map.put("audioSpectrums", audioSpectrumsMap);

            sink.success(map);
        }

        @Override
        public void onLocalDeviceExceptionOccurred (ZegoDeviceExceptionType exceptionType, ZegoDeviceType deviceType, String deviceID) {
            super.onLocalDeviceExceptionOccurred(exceptionType, deviceType, deviceID);
            ZegoLog.log("[onLocalDeviceExceptionOccurred] deviceID: %s, exceptionType: %s, deviceType: %s", deviceID, exceptionType.name(), deviceType.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onLocalDeviceExceptionOccurred");
            map.put("deviceID", deviceID);
            map.put("exceptionType", exceptionType.value());
            map.put("deviceType", deviceType.value());

            sink.success(map);
        }

        @Override
        public void onRemoteCameraStateUpdate(String streamID, ZegoRemoteDeviceState state) {
            super.onRemoteCameraStateUpdate(streamID, state);
            ZegoLog.log("[onRemoteCameraStateUpdate] streamID: %s, state: %s", streamID, state.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRemoteCameraStateUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());

            sink.success(map);
        }

        @Override
        public void onRemoteSpeakerStateUpdate(String streamID, ZegoRemoteDeviceState state) {
            super.onRemoteSpeakerStateUpdate(streamID, state);
            ZegoLog.log("[onRemoteSpeakerStateUpdate] streamID: %s, state: %s", streamID, state.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRemoteSpeakerStateUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());

            sink.success(map);
        }

        @Override
        public void onRemoteMicStateUpdate(String streamID, ZegoRemoteDeviceState state) {
            super.onRemoteMicStateUpdate(streamID, state);
            ZegoLog.log("[onRemoteMicStateUpdate] streamID: %s, state: %s", streamID, state.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRemoteMicStateUpdate");
            map.put("streamID", streamID);
            map.put("state", state.value());

            sink.success(map);
        }

        @Override
        public void onAudioRouteChange(ZegoAudioRoute audioRoute) {
            super.onAudioRouteChange(audioRoute);
            ZegoLog.log("[onAudioRouteChange] audioRoute: %s", audioRoute.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAudioRouteChange");
            map.put("audioRoute", audioRoute.value());

            sink.success(map);
        }

        @Override
        public void onAudioVADStateUpdate(ZegoAudioVADStableStateMonitorType type, ZegoAudioVADType state) {
            super.onAudioVADStateUpdate(type, state);
            ZegoLog.log("[onAudioVADStateUpdate] type: %s, state: %s", type.name(), state.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAudioVADStateUpdate");
            map.put("type", type.value());
            map.put("state", state.value());

            sink.success(map);
        }


        /* IM */

        @Override
        public void onIMRecvBroadcastMessage(String roomID, ArrayList<ZegoBroadcastMessageInfo> messageList) {
            super.onIMRecvBroadcastMessage(roomID, messageList);
            ZegoLog.log("[onIMRecvBroadcastMessage] roomID: %s, messageListCount: %d", roomID, messageList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            ArrayList<HashMap<String, Object>> messageArrayList = new ArrayList<>();

            for (ZegoBroadcastMessageInfo message: messageList) {

                HashMap<String, Object> userMap = new HashMap<>();
                userMap.put("userID", message.fromUser.userID);
                userMap.put("userName", message.fromUser.userName);

                HashMap<String, Object> messageMap = new HashMap<>();
                messageMap.put("message", message.message);
                messageMap.put("messageID", message.messageID);
                messageMap.put("sendTime", message.sendTime);
                messageMap.put("fromUser", userMap);

                messageArrayList.add(messageMap);
            }

            map.put("method", "onIMRecvBroadcastMessage");
            map.put("roomID", roomID);
            map.put("messageList", messageArrayList);

            sink.success(map);
        }

        @Override
        public void onIMRecvBarrageMessage(String roomID, ArrayList<ZegoBarrageMessageInfo> messageList) {
            super.onIMRecvBarrageMessage(roomID, messageList);
            ZegoLog.log("[onIMRecvBarrageMessage] roomID: %s, messageListCount: %d", roomID, messageList.size());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            ArrayList<HashMap<String, Object>> messageArrayList = new ArrayList<>();

            for (ZegoBarrageMessageInfo message: messageList) {

                HashMap<String, Object> userMap = new HashMap<>();
                userMap.put("userID", message.fromUser.userID);
                userMap.put("userName", message.fromUser.userName);

                HashMap<String, Object> messageMap = new HashMap<>();
                messageMap.put("message", message.message);
                messageMap.put("messageID", message.messageID);
                messageMap.put("sendTime", message.sendTime);
                messageMap.put("fromUser", userMap);

                messageArrayList.add(messageMap);
            }

            map.put("method", "onIMRecvBarrageMessage");
            map.put("roomID", roomID);
            map.put("messageList", messageArrayList);

            sink.success(map);
        }

        @Override
        public void onIMRecvCustomCommand(String roomID, ZegoUser fromUser, String command) {
            super.onIMRecvCustomCommand(roomID, fromUser, command);
            ZegoLog.log("[onIMRecvCustomCommand] roomID: %s, fromUserID: %s, fromUserName: %s, command: %s", roomID, fromUser.userID, fromUser.userName, command);

            if (guardSink()) { return; }

            HashMap<String, Object> userMap = new HashMap<>();
            userMap.put("userID", fromUser.userID);
            userMap.put("userName", fromUser.userName);

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onIMRecvCustomCommand");
            map.put("roomID", roomID);
            map.put("fromUser", userMap);
            map.put("command", command);

            sink.success(map);
        }
        
        @Override
        public void onRecvRoomTransparentMessage(String roomID, ZegoRoomRecvTransparentMessage message)
        {
            super.onRecvRoomTransparentMessage(roomID, message);
            ZegoLog.log("[onRecvRoomTransparentMessage] roomID: %s, sendUserID: %s, sendUserName: %s", roomID, message.sendUser.userID, message.sendUser.userName);

            if (guardSink()) { return; }



            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onRecvRoomTransparentMessage");
            map.put("roomID", roomID);

            HashMap<String, Object> messageMap = new HashMap<>();

            HashMap<String, Object> userMap = new HashMap<>();
            userMap.put("userID", message.sendUser.userID);
            userMap.put("userName", message.sendUser.userName);

            messageMap.put("sendUser", userMap);
            messageMap.put("content", message.content);

            map.put("message", messageMap);


            sink.success(map);
        }


        /* Utilities */

        @Override
        public void onPerformanceStatusUpdate(ZegoPerformanceStatus status) {
            super.onPerformanceStatusUpdate(status);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> statusMap = new HashMap<>();
            statusMap.put("cpuUsageApp", status.cpuUsageApp);
            statusMap.put("cpuUsageSystem", status.cpuUsageSystem);
            statusMap.put("memoryUsageApp", status.memoryUsageApp);
            statusMap.put("memoryUsageSystem", status.memoryUsageSystem);
            statusMap.put("memoryUsedApp", status.memoryUsedApp);

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onPerformanceStatusUpdate");
            map.put("status", statusMap);

            sink.success(map);
        }

        @Override
        public void onNetworkModeChanged(ZegoNetworkMode mode) {
            super.onNetworkModeChanged(mode);
            ZegoLog.log("[onNetworkModeChanged] mode: %s", mode.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onNetworkModeChanged");
            map.put("mode", mode.value());

            sink.success(map);
        }

        @Override
        public void onNetworkSpeedTestError(int errorCode, ZegoNetworkSpeedTestType type) {
            super.onNetworkSpeedTestError(errorCode, type);
            ZegoLog.log("[onNetworkSpeedTestError] errorCode: %d, type: %s", errorCode, type.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onNetworkSpeedTestError");
            map.put("errorCode", errorCode);
            map.put("type", type.value());

            sink.success(map);
        }

        @Override
        public void onNetworkSpeedTestQualityUpdate(ZegoNetworkSpeedTestQuality quality, ZegoNetworkSpeedTestType type) {
            super.onNetworkSpeedTestQualityUpdate(quality, type);
            
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> qualityMap = new HashMap<>();
            qualityMap.put("connectCost", quality.connectCost);
            qualityMap.put("rtt", quality.rtt);
            qualityMap.put("packetLostRate", quality.packetLostRate);
            qualityMap.put("quality", quality.quality.value());

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onNetworkSpeedTestQualityUpdate");
            map.put("quality", qualityMap);
            map.put("type", type.value());

            sink.success(map);
        }

        @Override
        public void onRequestDumpData() {
            super.onRequestDumpData();
            ZegoLog.log("[onRequestDumpData]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onRequestDumpData");

            sink.success(map);
        }

        @Override
        public void onRequestUploadDumpData(String dumpDir, boolean takePhoto) {
            super.onRequestUploadDumpData(dumpDir, takePhoto);
            ZegoLog.log("[onRequestUploadDumpData]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onRequestUploadDumpData");
            map.put("dumpDir", dumpDir);
            map.put("takePhoto", takePhoto);

            sink.success(map);
        }

        @Override
        public void onStartDumpData(int errorCode) {
            super.onStartDumpData(errorCode);
            ZegoLog.log("[onStartDumpData]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onStartDumpData");
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onStopDumpData(int errorCode, String dumpDir) {
            super.onStopDumpData(errorCode, dumpDir);
            ZegoLog.log("[onStopDumpData]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onStopDumpData");
            map.put("errorCode", errorCode);
            map.put("dumpDir", dumpDir);

            sink.success(map);
        }

        @Override
        public void onUploadDumpData(int errorCode) {
            super.onUploadDumpData(errorCode);
            ZegoLog.log("[onUploadDumpData]");

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onUploadDumpData");
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onRecvExperimentalAPI(String content) {
            super.onRecvExperimentalAPI(content);
            ZegoLog.log("[onRecvExperimentalAPI] content: %s", content);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onRecvExperimentalAPI");
            map.put("content", content);

            sink.success(map);
        }

        @Override
        public void onNetworkQuality(String userID, ZegoStreamQualityLevel upstreamQuality, ZegoStreamQualityLevel downstreamQuality) {
            super.onNetworkQuality(userID, upstreamQuality, downstreamQuality);
            
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onNetworkQuality");
            map.put("userID", userID);
            map.put("upstreamQuality", upstreamQuality.value());
            map.put("downstreamQuality", downstreamQuality.value());

            sink.success(map);
        }

        /* Screen Capture */
        @Override
        public void onScreenCaptureExceptionOccurred(ZegoScreenCaptureExceptionType exceptionType) {
            super.onScreenCaptureExceptionOccurred(exceptionType);
            ZegoLog.log("[onScreenCaptureExceptionOccurred] exceptionType: %s", exceptionType);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onMobileScreenCaptureExceptionOccurred");
            map.put("exceptionType", exceptionType.value());

            sink.success(map);
        }

        /* Private Utils */

        private ArrayList<HashMap<String, Object>> mapListFromUserList(ArrayList<ZegoUser> users) {

            ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();

            for (ZegoUser user: users) {

                HashMap<String, Object> map = new HashMap<>();
                map.put("userID", user.userID);
                map.put("userName", user.userName);

                arrayList.add(map);
            }
            return arrayList;
        }

        private ArrayList<HashMap<String, Object>> mapListFromStreamList(ArrayList<ZegoStream> streams) {

            ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();

            for (ZegoStream stream: streams) {

                HashMap<String, Object> userMap = new HashMap<>();
                userMap.put("userID", stream.user.userID);
                userMap.put("userName", stream.user.userName);

                HashMap<String, Object> map = new HashMap<>();
                map.put("user", userMap);
                map.put("streamID", stream.streamID);
                map.put("extraInfo", stream.extraInfo);

                arrayList.add(map);
            }
            return arrayList;
        }

        private ArrayList<HashMap<String, Object>> mapListFromRoomExtraInfoList(ArrayList<ZegoRoomExtraInfo> extraInfos) {

            ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();

            for (ZegoRoomExtraInfo extraInfo: extraInfos) {

                HashMap<String, Object> userMap = new HashMap<>();
                userMap.put("userID", extraInfo.updateUser.userID);
                userMap.put("userName", extraInfo.updateUser.userName);

                HashMap<String, Object> map = new HashMap<>();
                map.put("key", extraInfo.key);
                map.put("value", extraInfo.value);
                map.put("updateUser", userMap);
                map.put("updateTime", extraInfo.updateTime);

                arrayList.add(map);
            }
            return arrayList;
        }

        private ArrayList<HashMap<String, Object>> mapListFromStreamRelayCdnInfoList(ArrayList<ZegoStreamRelayCDNInfo> infos) {

            ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();

            for (ZegoStreamRelayCDNInfo info: infos) {

                HashMap<String, Object> map = new HashMap<>();
                map.put("url", info.url);
                map.put("state", info.state.value());
                map.put("updateReason", info.updateReason.value());
                map.put("stateTime", info.stateTime);

                arrayList.add(map);
            }
            return arrayList;
        }
    };


    IZegoMediaPlayerEventHandler mediaPlayerEventHandler = new IZegoMediaPlayerEventHandler() {
        @Override
        public void onMediaPlayerStateUpdate(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerState state, int errorCode) {
            super.onMediaPlayerStateUpdate(mediaPlayer, state, errorCode);
            ZegoLog.log("[onMediaPlayerStateUpdate] idx: %d, state: %s, errorCode: %d", mediaPlayer.getIndex(), state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerStateUpdate");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("state", state.value());
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerNetworkEvent(ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerNetworkEvent networkEvent) {
            super.onMediaPlayerNetworkEvent(mediaPlayer, networkEvent);
            ZegoLog.log("[onMediaPlayerNetworkEvent] idx: %d, networkEvent: %s", mediaPlayer.getIndex(), networkEvent.name());

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerNetworkEvent");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("networkEvent", networkEvent.value());

            sink.success(map);
        }

        @Override
        public void onMediaPlayerPlayingProgress(ZegoMediaPlayer mediaPlayer, long millisecond) {
            super.onMediaPlayerPlayingProgress(mediaPlayer, millisecond);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerPlayingProgress");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("millisecond", millisecond);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerRecvSEI(ZegoMediaPlayer mediaPlayer, byte[] data) {
            super.onMediaPlayerRecvSEI(mediaPlayer, data);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerRecvSEI");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("data", data);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerSoundLevelUpdate(ZegoMediaPlayer mediaPlayer, float soundLevel){
            super.onMediaPlayerSoundLevelUpdate(mediaPlayer, soundLevel);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerSoundLevelUpdate");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("soundLevel", soundLevel);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerFrequencySpectrumUpdate (ZegoMediaPlayer mediaPlayer, float[] spectrumList){
            super.onMediaPlayerFrequencySpectrumUpdate(mediaPlayer, spectrumList);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            ArrayList<Float> spectrumArray = new ArrayList<>();

            for (float spectrum: spectrumList) {
                spectrumArray.add(spectrum);
            }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerFrequencySpectrumUpdate");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("spectrumList", spectrumArray);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerFirstFrameEvent (ZegoMediaPlayer mediaPlayer, ZegoMediaPlayerFirstFrameEvent event){
            super.onMediaPlayerFirstFrameEvent(mediaPlayer, event);
            ZegoLog.log("[onMediaPlayerFirstFrameEvent] idx: %d, event: %s", mediaPlayer.getIndex(), event);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerFirstFrameEvent");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("event", event.value());

            sink.success(map);
        }

        @Override
        public void onMediaPlayerRenderingProgress(ZegoMediaPlayer mediaPlayer, long millisecond) {
            super.onMediaPlayerRenderingProgress(mediaPlayer, millisecond);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerRenderingProgress");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("millisecond", millisecond);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerVideoSizeChanged(ZegoMediaPlayer mediaPlayer, int width, int height) {
            super.onMediaPlayerVideoSizeChanged(mediaPlayer, width, height);
            ZegoLog.log("[onMediaPlayerVideoSizeChanged] idx: %d, width: %d, height: %d", mediaPlayer.getIndex(), width, height);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerVideoSizeChanged");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("width", width);
            map.put("height", height);

            sink.success(map);
        }

        @Override
        public void onMediaPlayerLocalCache(ZegoMediaPlayer mediaPlayer, int errorCode, String resource, String cachedFile) {
            super.onMediaPlayerLocalCache(mediaPlayer, errorCode, resource, cachedFile);
            ZegoLog.log("[onMediaPlayerLocalCache] idx: %d, error: %d, resource: %s, cachedFile: %s", mediaPlayer.getIndex(), errorCode, resource, cachedFile);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaPlayerLocalCache");
            map.put("mediaPlayerIndex", mediaPlayer.getIndex());
            map.put("errorCode", errorCode);
            map.put("resource", resource);
            map.put("cachedFile", cachedFile);

            sink.success(map);
        }
    };


    IZegoAudioEffectPlayerEventHandler audioEffectPlayerEventHandler = new IZegoAudioEffectPlayerEventHandler() {
        @Override
        public void onAudioEffectPlayStateUpdate(ZegoAudioEffectPlayer audioEffectPlayer, int audioEffectID, ZegoAudioEffectPlayState state, int errorCode) {
            super.onAudioEffectPlayStateUpdate(audioEffectPlayer, audioEffectID, state, errorCode);
            ZegoLog.log("[onAudioEffectPlayStateUpdate] idx: %d, effectID: %d, state: %s, errorCode: %d", audioEffectPlayer.getIndex(), audioEffectID, state.name(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAudioEffectPlayStateUpdate");
            map.put("audioEffectPlayerIndex", audioEffectPlayer.getIndex());
            map.put("audioEffectID", audioEffectID);
            map.put("state", state.value());
            map.put("errorCode", errorCode);

            sink.success(map);
        }
    };

    IZegoMediaDataPublisherEventHandler mediaDataPublisherEventHandler = new IZegoMediaDataPublisherEventHandler() {
        @Override
        public void onMediaDataPublisherFileOpen(ZegoMediaDataPublisher mediaDataPublisher, String path) {
            super.onMediaDataPublisherFileOpen(mediaDataPublisher, path);
            ZegoLog.log("[onMediaDataPublisherFileOpen] idx: %d, path: %s", mediaDataPublisher.getIndex(), path);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaDataPublisherFileOpen");
            map.put("publisherIndex", mediaDataPublisher.getIndex());
            map.put("path", path);

            sink.success(map);
        }

        @Override
        public void onMediaDataPublisherFileClose(ZegoMediaDataPublisher mediaDataPublisher, int errorCode, String path) {
            super.onMediaDataPublisherFileClose(mediaDataPublisher, errorCode, path);
            ZegoLog.log("[onMediaDataPublisherFileClose] idx: %d, errorCode: %d, path: %s", mediaDataPublisher.getIndex(), errorCode, path);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaDataPublisherFileClose");
            map.put("publisherIndex", mediaDataPublisher.getIndex());
            map.put("errorCode", errorCode);
            map.put("path", path);

            sink.success(map);
        }

        @Override
        public void onMediaDataPublisherFileDataBegin(ZegoMediaDataPublisher mediaDataPublisher, String path) {
            super.onMediaDataPublisherFileDataBegin(mediaDataPublisher, path);
            ZegoLog.log("[onMediaDataPublisherFileDataBegin] idx: %d, path: %s", mediaDataPublisher.getIndex(), path);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMediaDataPublisherFileDataBegin");
            map.put("publisherIndex", mediaDataPublisher.getIndex());
            map.put("path", path);

            sink.success(map);
        }
    };

    IZegoRangeAudioEventHandler rangeAudioEventHandler = new IZegoRangeAudioEventHandler() {
        @Override
        public void onRangeAudioMicrophoneStateUpdate(ZegoRangeAudio rangeAudio, ZegoRangeAudioMicrophoneState state, int errorCode) {
            super.onRangeAudioMicrophoneStateUpdate(rangeAudio, state, errorCode);

            int rangeAudioIndex = 0;
            ZegoLog.log("[onRangeAudioMicrophoneStateUpdate] rangeAudioIndex: %d, state: %s, errorCode: %s", rangeAudioIndex, state.name(), errorCode);

            if (guardSink()) { return; }
            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onRangeAudioMicrophoneStateUpdate");
            map.put("rangeAudioIndex", rangeAudioIndex);
            map.put("state", state.value());
            map.put("errorCode", errorCode);

            sink.success(map);
        }
    };

    IZegoRealTimeSequentialDataEventHandler realTimeSequentialDataEventHandler = new IZegoRealTimeSequentialDataEventHandler() {
        @Override
        public void onReceiveRealTimeSequentialData(ZegoRealTimeSequentialDataManager manager, byte[] data, String streamID) {
            super.onReceiveRealTimeSequentialData(manager, data, streamID);

            // High frequency callbacks do not log

            int realTimeSequentialDataManagerIndex = manager.getIndex();

            if (guardSink()) { return; }
            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onReceiveRealTimeSequentialData");
            map.put("realTimeSequentialDataManagerIndex", realTimeSequentialDataManagerIndex);
            map.put("data", data);
            map.put("streamID", streamID);

            sink.success(map);
        }
    };


    IZegoDataRecordEventHandler dataRecordEventHandler = new IZegoDataRecordEventHandler() {
        @Override
        public void onCapturedDataRecordStateUpdate(ZegoDataRecordState state, int errorCode, ZegoDataRecordConfig config, ZegoPublishChannel channel) {
            super.onCapturedDataRecordStateUpdate(state, errorCode, config, channel);
            ZegoLog.log("[onCapturedDataRecordStateUpdate] state: %s, errorCode: %d, filePath: %s, recordType: %s, channel: %s", state.name(), errorCode, config.filePath, config.recordType.name(), channel.name());

            if (guardSink()) { return; }

            HashMap<String, Object> configMap = new HashMap<>();
            configMap.put("filePath", config.filePath);
            configMap.put("recordType", config.recordType.value());

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onCapturedDataRecordStateUpdate");
            map.put("state", state.value());
            map.put("errorCode", errorCode);
            map.put("config", configMap);
            map.put("channel", channel.value());

            sink.success(map);
        }

        @Override
        public void onCapturedDataRecordProgressUpdate(ZegoDataRecordProgress progress, ZegoDataRecordConfig config, ZegoPublishChannel channel) {
            super.onCapturedDataRecordProgressUpdate(progress, config, channel);
            // High frequency callbacks do not log

            if (guardSink()) { return; }

            HashMap<String, Object> qualityMap = convertPublishStreamQuality(progress.quality);

            HashMap<String, Object> progressMap = new HashMap<>();
            progressMap.put("duration", progress.duration);
            progressMap.put("currentFileSize", progress.currentFileSize);
            progressMap.put("quality", qualityMap);

            HashMap<String, Object> configMap = new HashMap<>();
            configMap.put("filePath", config.filePath);
            configMap.put("recordType", config.recordType.value());

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onCapturedDataRecordProgressUpdate");
            map.put("progress", progressMap);
            map.put("config", configMap);
            map.put("channel", channel.value());

            sink.success(map);
        }
    };

    IZegoAudioDataHandler audioDataHandler = new IZegoAudioDataHandler() {
        @Override
        public void onCapturedAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param){
            super.onCapturedAudioData(data, dataLength, param);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onCapturedAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onPlaybackAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param){
            super.onPlaybackAudioData(data, dataLength, param);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlaybackAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onMixedAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param){
            super.onMixedAudioData(data, dataLength, param);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onMixedAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onPlayerAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param, String streamID){
            super.onPlayerAudioData(data, dataLength, param, streamID);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onPlayerAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);
            map.put("streamID", streamID);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }
    };

    /* Audio Process Handler */
    IZegoCustomAudioProcessHandler customAudioProcessHandler = new IZegoCustomAudioProcessHandler() {
        @Override
        public void onProcessCapturedAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param, double timestamp) {
            super.onProcessCapturedAudioData(data, dataLength, param, timestamp);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onProcessCapturedAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);
            map.put("timestamp", timestamp);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(ByteBuffer data, int dataLength, ZegoAudioFrameParam param, double timestamp) {
            super.onProcessCapturedAudioDataAfterUsedHeadphoneMonitor(data, dataLength, param, timestamp);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onProcessCapturedAudioDataAfterUsedHeadphoneMonitor");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);
            map.put("timestamp", timestamp);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onAlignedAudioAuxData(ByteBuffer data,int dataLength,ZegoAudioFrameParam param) {
            super.onAlignedAudioAuxData(data, dataLength, param);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAlignedAudioAuxData");
            map.put("data", bytes);
            map.put("param", paramMap);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onProcessRemoteAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param, String streamID, double timestamp) {
            super.onProcessRemoteAudioData(data, dataLength, param, streamID, timestamp);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onProcessRemoteAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);
            map.put("streamID", streamID);
            map.put("timestamp", timestamp);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }

        @Override
        public void onProcessPlaybackAudioData(ByteBuffer data, int dataLength, ZegoAudioFrameParam param, double timestamp) {
            super.onProcessPlaybackAudioData(data, dataLength, param, timestamp);

            if (guardSink()) { return; }

            int len = data.limit() - data.position();
            byte[] bytes = new byte[len];
            data.get(bytes);

            HashMap<String, Object> paramMap = new HashMap<>();
            paramMap.put("sampleRate", param.sampleRate.value());
            paramMap.put("channel", param.channel.value());

            final HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onProcessPlaybackAudioData");
            map.put("data", bytes);
            map.put("dataLength", dataLength);
            map.put("param", paramMap);
            map.put("timestamp", timestamp);

            mUIHandler.post(new Runnable() {
                @Override
                public void run() {
                    sink.success(map);
                }
            });
        }
    };

    IZegoCopyrightedMusicEventHandler copyrightedMusicEventHandler = new IZegoCopyrightedMusicEventHandler() {
        @Override
        public void onDownloadProgressUpdate(ZegoCopyrightedMusic copyrightedMusic, String resourceID, float progressRate) {
            super.onDownloadProgressUpdate(copyrightedMusic, resourceID, progressRate);
            // High frequency callbacks do not log

            if (guardSink()) { return; }
            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onDownloadProgressUpdate");
            map.put("resourceID", resourceID);
            map.put("progressRate", progressRate);

            sink.success(map);
        }

        @Override
        public void onCurrentPitchValueUpdate(ZegoCopyrightedMusic copyrightedMusic, String resourceID, int currentDuration, int pitchValue) {
            super.onCurrentPitchValueUpdate(copyrightedMusic, resourceID, currentDuration, pitchValue);
            // High frequency callbacks do not log

            if (guardSink()) { return; }
            HashMap<String, Object> map = new HashMap<>();
            map.put("method", "onCurrentPitchValueUpdate");
            map.put("resourceID", resourceID);
            map.put("currentDuration", currentDuration);
            map.put("pitchValue", pitchValue);

            sink.success(map);
        }
    };

    IZegoAIVoiceChangerEventHandler aiVoiceChangerEventHandler = new IZegoAIVoiceChangerEventHandler() {
        @Override
        public void onInit(ZegoAIVoiceChanger aiVoiceChanger, int errorCode) {
            super.onInit(aiVoiceChanger, errorCode);
            ZegoLog.log("[onAIVoiceChangerInit] idx: %d, errorCode: %d", aiVoiceChanger.getIndex(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAIVoiceChangerInit");
            map.put("aiVoiceChangerIndex", aiVoiceChanger.getIndex());
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onUpdate(ZegoAIVoiceChanger aiVoiceChanger, int errorCode) {
            super.onUpdate(aiVoiceChanger, errorCode);
            ZegoLog.log("[onAIVoiceChangerUpdate] idx: %d, errorCode: %d", aiVoiceChanger.getIndex(), errorCode);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAIVoiceChangerUpdate");
            map.put("aiVoiceChangerIndex", aiVoiceChanger.getIndex());
            map.put("errorCode", errorCode);

            sink.success(map);
        }

        @Override
        public void onGetSpeakerList(ZegoAIVoiceChanger aiVoiceChanger, int errorCode, ArrayList<ZegoAIVoiceChangerSpeakerInfo> speakerList) {
            super.onGetSpeakerList(aiVoiceChanger, errorCode, speakerList);
            ZegoLog.log("[onAIVoiceChangerGetSpeakerList] idx: %d, errorCode: %d", aiVoiceChanger.getIndex(), errorCode);

            if (guardSink()) { return; }

            ArrayList<HashMap<String, Object>> speakerMapList = new ArrayList<>();
            for (ZegoAIVoiceChangerSpeakerInfo info : speakerList) {
                HashMap<String, Object> speakerMap = new HashMap<>();
                speakerMap.put("name", info.name);
                speakerMap.put("id", info.id);
                speakerMapList.add(speakerMap);
            }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAIVoiceChangerGetSpeakerList");
            map.put("aiVoiceChangerIndex", aiVoiceChanger.getIndex());
            map.put("errorCode", errorCode);
            map.put("speakerList", speakerMapList);

            sink.success(map);
        }

        @Override
        public void onUpdateProgress(ZegoAIVoiceChanger aiVoiceChanger, double percent, int fileIndex, int fileCount) {
            super.onUpdateProgress(aiVoiceChanger, percent, fileIndex, fileCount);

            ZegoLog.log("[onAIVoiceChangerUpdateProgress] idx: %d, percent: %lf, fileIndex: %d, fileCount: %d", aiVoiceChanger.getIndex(), percent, fileIndex, fileCount);

            if (guardSink()) { return; }

            HashMap<String, Object> map = new HashMap<>();

            map.put("method", "onAIVoiceChangerUpdateProgress");
            map.put("aiVoiceChangerIndex", aiVoiceChanger.getIndex());
            map.put("percent", percent);
            map.put("fileIndex", fileIndex);
            map.put("fileCount", fileCount);

            sink.success(map);
        }
    };
}
