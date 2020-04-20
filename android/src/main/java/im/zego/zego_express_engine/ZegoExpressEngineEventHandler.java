//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

import im.zego.zegoexpress.callback.IZegoEventHandler;
import im.zego.zegoexpress.constants.ZegoPlayerMediaEvent;
import im.zego.zegoexpress.constants.ZegoPlayerState;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoPublisherState;
import im.zego.zegoexpress.constants.ZegoRemoteDeviceState;
import im.zego.zegoexpress.constants.ZegoRoomState;
import im.zego.zegoexpress.constants.ZegoUpdateType;
import im.zego.zegoexpress.entity.ZegoBarrageMessageInfo;
import im.zego.zegoexpress.entity.ZegoBroadcastMessageInfo;
import im.zego.zegoexpress.entity.ZegoPlayStreamQuality;
import im.zego.zegoexpress.entity.ZegoPublishStreamQuality;
import im.zego.zegoexpress.entity.ZegoStream;
import im.zego.zegoexpress.entity.ZegoStreamRelayCDNInfo;
import im.zego.zegoexpress.entity.ZegoUser;
import io.flutter.plugin.common.EventChannel;

public class ZegoExpressEngineEventHandler extends IZegoEventHandler {

    private final EventChannel.EventSink sink;

    ZegoExpressEngineEventHandler(EventChannel.EventSink sink) {
        super();
        this.sink = sink;
    }

    @Override
    public void onDebugError(int errorCode, String funcName, String info) {
        super.onDebugError(errorCode, funcName, info);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onDebugError");
        map.put("errorCode", errorCode);
        map.put("funcName", funcName);
        map.put("info", info);

        sink.success(map);
    }


    /* Room */

    @Override
    public void onRoomStateUpdate(String roomID, ZegoRoomState state, int errorCode, JSONObject extendedData) {
        super.onRoomStateUpdate(roomID, state, errorCode, extendedData);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRoomStateUpdate");
        map.put("roomID", roomID);
        map.put("state", state.value());
        map.put("errorCode", errorCode);
        map.put("extendedData", extendedData.toString());

        sink.success(map);
    }

    @Override
    public void onRoomUserUpdate(String roomID, ZegoUpdateType updateType, ArrayList<ZegoUser> userList) {
        super.onRoomUserUpdate(roomID, updateType, userList);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRoomUserUpdate");
        map.put("roomID", roomID);
        map.put("updateType", updateType.value());
        map.put("userList", this.mapListFromUserList(userList));

        sink.success(map);
    }

    @Override
    public void onRoomStreamUpdate(String roomID, ZegoUpdateType updateType, ArrayList<ZegoStream> streamList) {
        super.onRoomStreamUpdate(roomID, updateType, streamList);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRoomStreamUpdate");
        map.put("roomID", roomID);
        map.put("updateType", updateType.value());
        map.put("streamList", this.mapListFromStreamList(streamList));

        sink.success(map);
    }

    @Override
    public void onRoomStreamExtraInfoUpdate(String roomID, ArrayList<ZegoStream> streamList) {
        super.onRoomStreamExtraInfoUpdate(roomID, streamList);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRoomStreamExtraInfoUpdate");
        map.put("roomID", roomID);
        map.put("streamList", this.mapListFromStreamList(streamList));

        sink.success(map);
    }


    /* Publisher */

    @Override
    public void onPublisherStateUpdate(String streamID, ZegoPublisherState state, int errorCode, JSONObject extendedData) {
        super.onPublisherStateUpdate(streamID, state, errorCode, extendedData);

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
        qualityMap.put("totalSendBytes", quality.totalSendBytes);
        qualityMap.put("audioSendBytes", quality.audioSendBytes);
        qualityMap.put("videoSendBytes", quality.videoSendBytes);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPublisherQualityUpdate");
        map.put("streamID", streamID);
        map.put("quality", qualityMap);

        sink.success(map);
    }

    @Override
    public void onPublisherCapturedAudioFirstFrame() {
        super.onPublisherCapturedAudioFirstFrame();

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPublisherCapturedAudioFirstFrame");

        sink.success(map);
    }

    @Override
    public void onPublisherCapturedVideoFirstFrame(ZegoPublishChannel channel) {
        super.onPublisherCapturedVideoFirstFrame(channel);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPublisherCapturedVideoFirstFrame");
        map.put("channel", channel.value());

        sink.success(map);
    }

    @Override
    public void onPublisherVideoSizeChanged(int width, int height, ZegoPublishChannel channel) {
        super.onPublisherVideoSizeChanged(width, height, channel);

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

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPublisherRelayCDNStateUpdate");
        map.put("streamID", streamID);
        map.put("streamInfoList", this.mapListFromStreamRelayCdnInfoList(streamInfoList));

        sink.success(map);
    }


    /* Player */

    @Override
    public void onPlayerStateUpdate(String streamID, ZegoPlayerState state, int errorCode, JSONObject extendedData) {
        super.onPlayerStateUpdate(streamID, state, errorCode, extendedData);

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

        HashMap<String, Object> qualityMap = new HashMap<>();
        qualityMap.put("videoRecvFPS", quality.videoRecvFPS);
        qualityMap.put("videoDecodeFPS", quality.videoDecodeFPS);
        qualityMap.put("videoRenderFPS", quality.videoRenderFPS);
        qualityMap.put("videoKBPS", quality.videoKBPS);
        qualityMap.put("audioRecvFPS", quality.audioRecvFPS);
        qualityMap.put("audioDecodeFPS", quality.audioDecodeFPS);
        qualityMap.put("audioRenderFPS", quality.audioRenderFPS);
        qualityMap.put("audioKBPS", quality.audioKBPS);
        qualityMap.put("rtt", quality.rtt);
        qualityMap.put("packetLostRate", quality.packetLostRate);
        qualityMap.put("peerToPeerDelay", quality.peerToPeerDelay);
        qualityMap.put("peerToPeerPacketLostRate", quality.peerToPeerPacketLostRate);
        qualityMap.put("level", quality.level.value());
        qualityMap.put("delay", quality.delay);
        qualityMap.put("isHardwareDecode", quality.isHardwareDecode);
        qualityMap.put("totalRecvBytes", quality.totalRecvBytes);
        qualityMap.put("audioRecvBytes", quality.audioRecvBytes);
        qualityMap.put("videoRecvBytes", quality.videoRecvBytes);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerQualityUpdate");
        map.put("streamID", streamID);
        map.put("quality", qualityMap);

        sink.success(map);
    }

    @Override
    public void onPlayerMediaEvent(String streamID, ZegoPlayerMediaEvent event) {
        super.onPlayerMediaEvent(streamID, event);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerMediaEvent");
        map.put("streamID", streamID);
        map.put("event", event.value());

        sink.success(map);
    }

    @Override
    public void onPlayerRecvAudioFirstFrame(String streamID) {
        super.onPlayerRecvAudioFirstFrame(streamID);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerRecvAudioFirstFrame");
        map.put("streamID", streamID);

        sink.success(map);
    }

    @Override
    public void onPlayerRecvVideoFirstFrame(String streamID) {
        super.onPlayerRecvVideoFirstFrame(streamID);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerRecvVideoFirstFrame");
        map.put("streamID", streamID);

        sink.success(map);
    }

    @Override
    public void onPlayerRenderVideoFirstFrame(String streamID) {
        super.onPlayerRenderVideoFirstFrame(streamID);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerRenderVideoFirstFrame");
        map.put("streamID", streamID);

        sink.success(map);
    }

    @Override
    public void onPlayerVideoSizeChanged(String streamID, int width, int height) {
        super.onPlayerVideoSizeChanged(streamID, width, height);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerVideoSizeChanged");
        map.put("streamID", streamID);
        map.put("width", width);
        map.put("height", height);

        sink.success(map);
    }

    @Override
    public void onPlayerRecvSEI(String streamID, byte[] data) {
        super.onPlayerRecvSEI(streamID, data);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onPlayerRecvSEI");
        map.put("streamID", streamID);
        map.put("data", data);

        sink.success(map);
    }


    /* Mixer */

    @Override
    public void onMixerRelayCDNStateUpdate(String taskID, ArrayList<ZegoStreamRelayCDNInfo> infoList) {
        super.onMixerRelayCDNStateUpdate(taskID, infoList);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onMixerRelayCDNStateUpdate");
        map.put("infoList", mapListFromStreamRelayCdnInfoList(infoList));
        map.put("taskID", taskID);

        sink.success(map);
    }

    @Override
    public void onMixerSoundLevelUpdate(HashMap<Integer, Float> soundLevels) {
        super.onMixerSoundLevelUpdate(soundLevels);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onMixerSoundLevelUpdate");
        map.put("soundLevels", soundLevels);

        sink.success(map);
    }


    /* Device */

    @Override
    public void onCapturedSoundLevelUpdate(float soundLevel) {
        super.onCapturedSoundLevelUpdate(soundLevel);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onCapturedSoundLevelUpdate");
        map.put("soundLevels", soundLevel);

        sink.success(map);
    }

    @Override
    public void onRemoteSoundLevelUpdate(HashMap<String, Float> soundLevels) {
        super.onRemoteSoundLevelUpdate(soundLevels);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRemoteSoundLevelUpdate");
        map.put("soundLevels", soundLevels);

        sink.success(map);
    }

    @Override
    public void onCapturedAudioSpectrumUpdate(float[] audioSpectrum) {
        super.onCapturedAudioSpectrumUpdate(audioSpectrum);

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
    public void onDeviceError(int errorCode, String deviceName) {
        super.onDeviceError(errorCode, deviceName);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onDeviceError");
        map.put("errorCode", errorCode);
        map.put("deviceName", deviceName);

        sink.success(map);
    }

    @Override
    public void onRemoteCameraStateUpdate(String streamID, ZegoRemoteDeviceState state) {
        super.onRemoteCameraStateUpdate(streamID, state);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRemoteCameraStateUpdate");
        map.put("streamID", streamID);
        map.put("state", state.value());

        sink.success(map);
    }

    @Override
    public void onRemoteMicStateUpdate(String streamID, ZegoRemoteDeviceState state) {
        super.onRemoteMicStateUpdate(streamID, state);

        HashMap<String, Object> map = new HashMap<>();

        map.put("method", "onRemoteMicStateUpdate");
        map.put("streamID", streamID);
        map.put("state", state.value());

        sink.success(map);
    }


    /* IM */

    @Override
    public void onIMRecvBroadcastMessage(String roomID, ArrayList<ZegoBroadcastMessageInfo> messageList) {
        super.onIMRecvBroadcastMessage(roomID, messageList);

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


    /* Utils */

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
}
