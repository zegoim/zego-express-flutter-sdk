//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import android.app.Application;
import android.graphics.Canvas;
import android.graphics.Rect;

import java.lang.*;
import java.util.ArrayList;
import java.util.HashMap;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.callback.IZegoIMSendBarrageMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBroadcastMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendCustomCommandCallback;
import im.zego.zegoexpress.callback.IZegoPublisherSetStreamExtraInfoCallback;
import im.zego.zegoexpress.callback.IZegoPublisherUpdateCdnUrlCallback;
import im.zego.zegoexpress.constants.ZegoAECMode;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioCodecID;
import im.zego.zegoexpress.constants.ZegoCapturePipelineScaleMode;
import im.zego.zegoexpress.constants.ZegoLanguage;
import im.zego.zegoexpress.constants.ZegoOrientation;
import im.zego.zegoexpress.constants.ZegoPlayerVideoLayer;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoScenario;
import im.zego.zegoexpress.constants.ZegoTrafficControlMinVideoBitrateMode;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.constants.ZegoViewMode;
import im.zego.zegoexpress.entity.ZegoAudioConfig;
import im.zego.zegoexpress.entity.ZegoBeautifyOption;
import im.zego.zegoexpress.entity.ZegoCDNConfig;
import im.zego.zegoexpress.entity.ZegoCanvas;
import im.zego.zegoexpress.entity.ZegoPlayerConfig;
import im.zego.zegoexpress.entity.ZegoRoomConfig;
import im.zego.zegoexpress.entity.ZegoUser;
import im.zego.zegoexpress.entity.ZegoVideoConfig;
import im.zego.zegoexpress.entity.ZegoWatermark;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.view.TextureRegistry;

public class ZegoExpressEngineMethodHandler {

    private static boolean enablePlatformView = false;

    private static TextureRegistry textureRegistry = null;

    /* Main */

    public static void createEngine(MethodCall call, Result result, Application application, EventChannel.EventSink sink, TextureRegistry registry) {

        long appID = longValue((Number)call.argument("appID"));
        String appSign = call.argument("appSign");
        boolean isTestEnv = boolValue((Boolean) call.argument("isTestEnv"));
        ZegoScenario scenario = ZegoScenario.getZegoScenario(intValue((Number)call.argument("scenario")));
        enablePlatformView = boolValue((Boolean) call.argument("enablePlatformView"));
        textureRegistry = registry;

        ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario, application, new ZegoExpressEngineEventHandler(sink));

        result.success(null);
    }

    public static void destroyEngine(MethodCall call, Result result) {

        ZegoExpressEngine.destroyEngine(null);

        result.success(null);
    }

    public static void getVersion(MethodCall call, Result result) {

        result.success(ZegoExpressEngine.getVersion());
    }

    public static void uploadLog(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().uploadLog();

        result.success(null);
    }

    public static void setDebugVerbose(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoLanguage language = ZegoLanguage.getZegoLanguage(intValue((Number)call.argument("language")));

        ZegoExpressEngine.getEngine().setDebugVerbose(enable, language);

        result.success(null);
    }


    /* Room */

    public static void loginRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        HashMap<String, Object> userMap = call.argument("user");
        ZegoUser user = new ZegoUser((String) userMap.get("userID"), (String) userMap.get("userName"));

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap != null && !configMap.isEmpty()) {
            ZegoRoomConfig roomConfig = new ZegoRoomConfig();
            roomConfig.isUserStatusNotify = boolValue((Boolean)configMap.get("isUserStatusNotify"));
            roomConfig.maxMemberCount = intValue((Number)configMap.get("maxMemberCount"));
            roomConfig.token = (String)configMap.get("token");
            ZegoExpressEngine.getEngine().loginRoom(roomID, user, roomConfig);
        } else {
            ZegoExpressEngine.getEngine().loginRoom(roomID, user);
        }

        result.success(null);
    }

    public static void logoutRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        ZegoExpressEngine.getEngine().logoutRoom(roomID);

        result.success(null);
    }


    /* Publisher */

    public static void startPublishingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().startPublishingStream(streamID, channel);

        result.success(null);
    }

    public static void stopPublishingStream(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopPublishingStream(channel);

        result.success(null);
    }

    public static void setStreamExtraInfo(MethodCall call, final Result result) {

        String extraInfo = call.argument("extraInfo");

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setStreamExtraInfo(extraInfo, channel, new IZegoPublisherSetStreamExtraInfoCallback() {
            @Override
            public void onPublisherSetStreamExtraInfoResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });

    }

    public static void startPreview(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        HashMap<String, Object> canvasMap = call.argument("canvas");

        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Preview video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = intValue((Number) canvasMap.get("backgroundColor"));

            Object view = null;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getView();
                } else {
                    // Preview video without creating the PlatfromView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatfromView and get viewID (PlatformViewID)
                    // TODO: Throw Flutter Exception
                }

            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long)viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Preview video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    // TODO: Throw Flutter Exception
                }
            }

            ZegoCanvas canvas = null;

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;
            }

            ZegoExpressEngine.getEngine().startPreview(canvas, channel);

        } else { /* if (canvasMap != null && !canvasMap.isEmpty()) */

            // Preview audio only
            ZegoExpressEngine.getEngine().startPreview(null, channel);
        }

        result.success(null);
    }

    public static void stopPreview(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopPreview(channel);

        result.success(null);
    }

    public static void setVideoConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) { return; }

        int captureWidth = intValue((Number) configMap.get("captureWidth"));
        int captureHeight = intValue((Number) configMap.get("captureHeight"));
        int encodeWidth = intValue((Number) configMap.get("encodeWidth"));
        int encodeHeight = intValue((Number) configMap.get("encodeHeight"));
        int fps = intValue((Number) configMap.get("fps"));
        int bitrate = intValue((Number) configMap.get("bitrate"));

        ZegoVideoConfig config = new ZegoVideoConfig();
        config.setCaptureResolution(captureWidth, captureHeight);
        config.setEncodeResolution(encodeWidth, encodeHeight);
        config.setVideoFPS(fps);
        config.setVideoBitrate(bitrate);

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoConfig(config, channel);

        result.success(null);
    }

    public static void setVideoMirrorMode(MethodCall call, Result result) {

        ZegoVideoMirrorMode mirrorMode = ZegoVideoMirrorMode.getZegoVideoMirrorMode(intValue((Number) call.argument("mirrorMode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoMirrorMode(mirrorMode, channel);

        result.success(null);
    }

    public static void setAppOrientation(MethodCall call, Result result) {

        ZegoOrientation orientation = ZegoOrientation.values()[intValue((Number) call.argument("orientation"))];
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setAppOrientation(orientation, channel);

        result.success(null);
    }

    public static void setAudioConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) { return; }

        int bitrate = intValue((Number) configMap.get("bitrate"));
        int channel = intValue((Number) configMap.get("channel"));
        int codecID = intValue((Number) configMap.get("codecID"));

        ZegoAudioConfig config = new ZegoAudioConfig();
        config.bitrate = bitrate;
        config.channel = ZegoAudioChannel.getZegoAudioChannel(channel);
        config.codecID = ZegoAudioCodecID.getZegoAudioCodecID(codecID);

        ZegoExpressEngine.getEngine().setAudioConfig(config);

        result.success(null);
    }

    public static void mutePublishStreamAudio(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamAudio(mute, channel);

        result.success(null);
    }

    public static void mutePublishStreamVideo(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamVideo(mute, channel);

        result.success(null);
    }

    public static void enableTrafficControl(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        int property = intValue((Number) call.argument("property"));

        ZegoExpressEngine.getEngine().enableTrafficControl(enable, property);

        result.success(null);
    }

    public static void setMinVideoBitrateForTrafficControl(MethodCall call, Result result) {

        int bitrate = intValue((Number) call.argument("bitrate"));
        ZegoTrafficControlMinVideoBitrateMode mode = ZegoTrafficControlMinVideoBitrateMode.getZegoTrafficControlMinVideoBitrateMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setMinVideoBitrateForTrafficControl(bitrate, mode);

        result.success(null);
    }

    public static void setCaptureVolume(MethodCall call, Result result) {

        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setCaptureVolume(volume);

        result.success(null);
    }

    public static void addPublishCdnUrl(MethodCall call, final Result result) {

        String streamID = call.argument("streamID");
        String targetURL = call.argument("targetURL");

        ZegoExpressEngine.getEngine().addPublishCdnUrl(streamID, targetURL, new IZegoPublisherUpdateCdnUrlCallback() {
            @Override
            public void onPublisherUpdateCdnUrlResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
    }

    public static void removePublishCdnUrl(MethodCall call, final Result result) {

        String streamID = call.argument("streamID");
        String targetURL = call.argument("targetURL");

        ZegoExpressEngine.getEngine().removePublishCdnUrl(streamID, targetURL, new IZegoPublisherUpdateCdnUrlCallback() {
            @Override
            public void onPublisherUpdateCdnUrlResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
    }

    public static void enablePublishDirectToCDN(MethodCall call, Result result) {

        HashMap<String, String> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) { return; }

        ZegoCDNConfig config = new ZegoCDNConfig();
        config.url = configMap.get("url");
        config.authParam = configMap.get("authParam");

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enablePublishDirectToCDN(enable, config, channel);

        result.success(null);
    }

    public static void setPublishWatermark(MethodCall call, Result result) {

        HashMap<String, Object> watermarkMap = call.argument("watermark");
        if (watermarkMap == null) { return; }

        ZegoWatermark watermark = null;
        if (!watermarkMap.isEmpty()) {
            int left = intValue((Number) watermarkMap.get("left"));
            int top = intValue((Number) watermarkMap.get("top"));
            int right = intValue((Number) watermarkMap.get("right"));
            int bottom = intValue((Number) watermarkMap.get("bottom"));
            Rect rect = new Rect(left, top, right, bottom);
            watermark = new ZegoWatermark((String) watermarkMap.get("imageURL"), rect);
        }

        boolean isPreviewVisible = boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setPublishWatermark(watermark, isPreviewVisible, channel);

        result.success(null);
    }

    public static void sendSEI(MethodCall call, Result result) {

        byte[] data = call.argument("data");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendSEI(data, channel);

        result.success(null);
    }

    public static void enableHardwareEncoder(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareEncoder(enable);

        result.success(null);
    }

    public static void setCapturePipelineScaleMode(MethodCall call, Result result) {

        ZegoCapturePipelineScaleMode mode = ZegoCapturePipelineScaleMode.getZegoCapturePipelineScaleMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setCapturePipelineScaleMode(mode);

        result.success(null);
    }


    /* Player */

    public static void startPlayingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");

        // Handle ZegoPlayerConfig

        ZegoPlayerConfig playerConfig = null;

        HashMap<String, Object> playerConfigMap = call.argument("config");

        if (playerConfigMap != null && !playerConfigMap.isEmpty()) {

            playerConfig = new ZegoPlayerConfig();
            playerConfig.videoLayer = ZegoPlayerVideoLayer.getZegoPlayerVideoLayer(intValue((Number) playerConfigMap.get("videoLayer")));

            HashMap<String, Object> cdnConfigMap = (HashMap<String, Object>) playerConfigMap.get("cdnConfig");
            if (cdnConfigMap != null && !cdnConfigMap.isEmpty()) {

                ZegoCDNConfig cdnConfig = new ZegoCDNConfig();
                cdnConfig.url = (String) cdnConfigMap.get("url");
                cdnConfig.authParam = (String) cdnConfigMap.get("authParam");
                playerConfig.cdnConfig = cdnConfig;
            }
        }

        // Handle ZegoCanvas

        HashMap<String, Object> canvasMap = call.argument("canvas");

        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Play video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = intValue((Number) canvasMap.get("backgroundColor"));

            Object view = null;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getView();
                } else {
                    // Play video without creating the PlatfromView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatfromView and get viewID (PlatformViewID)
                    // TODO: Throw Flutter Exception
                }

            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long)viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Play video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    // TODO: Throw Flutter Exception
                }
            }

            ZegoCanvas canvas = null;

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;
            }

            ZegoExpressEngine.getEngine().startPlayingStream(streamID, canvas, playerConfig);

        } else { /* if (canvasMap != null && !canvasMap.isEmpty()) */

            // Play audio only
            ZegoExpressEngine.getEngine().startPlayingStream(streamID, null, playerConfig);
        }

        result.success(null);
    }

    public static void stopPlayingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");

        ZegoExpressEngine.getEngine().stopPlayingStream(streamID);

        result.success(null);
    }

    public static void setPlayVolume(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setPlayVolume(streamID, volume);

        result.success(null);
    }

    public static void mutePlayStreamAudio(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamAudio(streamID, mute);

        result.success(null);
    }

    public static void mutePlayStreamVideo(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamVideo(streamID, mute);

        result.success(null);
    }

    public static void enableHardwareDecoder(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareDecoder(enable);

        result.success(null);
    }

    public static void enableCheckPoc(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableCheckPoc(enable);

        result.success(null);
    }


    /* Mixer */

    public static void startMixerTask(MethodCall call, Result result) {

        // TODO: startMixerTask

        result.success(null);
    }

    public static void stopMixerTask(MethodCall call, Result result) {

        // TODO: stopMixerTask

        result.success(null);
    }


    /* Device */

    public static void muteMicrophone(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteMicrophone(mute);

        result.success(null);
    }

    public static void muteSpeaker(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteSpeaker(mute);

        result.success(null);
    }

    public static void enableAudioCaptureDevice(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAudioCaptureDevice(enable);

        result.success(null);
    }

    public static void setBuiltInSpeakerOn(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().setBuiltInSpeakerOn(enable);

        result.success(null);
    }

    public static void enableCamera(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableCamera(enable);

        result.success(null);
    }

    public static void useFrontCamera(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().useFrontCamera(enable, channel);

        result.success(null);
    }

    public static void startSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().startSoundLevelMonitor();

        result.success(null);
    }

    public static void stopSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopSoundLevelMonitor();

        result.success(null);
    }

    public static void startAudioSpectrumMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().startAudioSpectrumMonitor();

        result.success(null);
    }

    public static void stopAudioSpectrumMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopAudioSpectrumMonitor();

        result.success(null);
    }


    /* PreProcess */

    public static void enableAEC(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAEC(enable);

        result.success(null);
    }

    public static void setAECMode(MethodCall call, Result result) {

        ZegoAECMode mode = ZegoAECMode.getZegoAECMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setAECMode(mode);

        result.success(null);
    }

    public static void enableAGC(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAGC(enable);

        result.success(null);
    }

    public static void enableANS(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableANS(enable);

        result.success(null);
    }

    public static void enableBeautify(MethodCall call, Result result) {

        int featureBitmask = intValue((Number) call.argument("featureBitmask"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableBeautify(featureBitmask, channel);

        result.success(null);
    }

    public static void setBeautifyOption(MethodCall call, Result result) {

        HashMap<String, Double> optionMap = call.argument("option");
        if (optionMap == null || optionMap.isEmpty()) { return; }

        double polishStep = doubleValue(optionMap.get("polishStep"));
        double whitenFactor = doubleValue(optionMap.get("whitenFactor"));
        double sharpenFactor = doubleValue(optionMap.get("sharpenFactor"));

        ZegoBeautifyOption option = new ZegoBeautifyOption();
        option.polishStep = polishStep;
        option.whitenFactor = whitenFactor;
        option.sharpenFactor = sharpenFactor;

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setBeautifyOption(option, channel);

        result.success(null);
    }

    public static void sendBroadcastMessage(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");
        String message = call.argument("message");

        ZegoExpressEngine.getEngine().sendBroadcastMessage(roomID, message, new IZegoIMSendBroadcastMessageCallback() {
            @Override
            public void onIMSendBroadcastMessageResult(int errorCode, long messageID) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                resultMap.put("messageID", messageID);
                result.success(resultMap);
            }
        });
    }

    public static void sendBarrageMessage(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");
        String message = call.argument("message");

        ZegoExpressEngine.getEngine().sendBarrageMessage(roomID, message, new IZegoIMSendBarrageMessageCallback() {
            @Override
            public void onIMSendBarrageMessageResult(int errorCode, String messageID) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                resultMap.put("messageID", messageID);
                result.success(resultMap);
            }
        });

        result.success(null);
    }

    public static void sendCustomCommand(MethodCall call, final Result result) {

        ArrayList<ZegoUser> toUserList = new ArrayList<>();

        ArrayList<HashMap<String, Object>> toUserMapList = call.argument("toUserList");
        if (toUserMapList == null) { return; }

        for (HashMap<String, Object> userMap: toUserMapList) {
            String userID = (String) userMap.get("userID");
            String userName = (String) userMap.get("userName");
            ZegoUser user = new ZegoUser(userID, userName);
            toUserList.add(user);
        }

        String roomID = call.argument("roomID");
        String command = call.argument("command");

        ZegoExpressEngine.getEngine().sendCustomCommand(roomID, command, toUserList, new IZegoIMSendCustomCommandCallback() {
            @Override
            public void onIMSendCustomCommandResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
    }


    /* PlatformView Utils */

    public static void destroyPlatformView(MethodCall call, Result result) {

        int viewID = intValue((Number) call.argument("viewID"));

        ZegoPlatformViewFactory.getInstance().destroyPlatformView(viewID);

        result.success(null);
    }


    /* TextureRenderer Utils */

    public static void createTextureRenderer(MethodCall call, Result result) {

        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        Long textureID = ZegoTextureRendererController.getInstance().createTextureRenderer(textureRegistry.createSurfaceTexture(), width, height);

        result.success(textureID);
    }

    public static void updateTextureRenderer(MethodCall call, Result result) {

        Long textureID = longValue((Number) call.argument("textureID"));
        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        ZegoTextureRendererController.getInstance().updateTextureRenderer(textureID, width, height);

        result.success(null);
    }

    public static void destroyTextureRenderer(MethodCall call, Result result) {

        Long textureID = longValue((Number) call.argument("textureID"));

        ZegoTextureRendererController.getInstance().destroyTextureRenderer(textureID);

        result.success(null);
    }


    /* Utils */

    private static boolean boolValue(Boolean number) {
        return number != null && number;
    }

    private static int intValue(Number number) {
        return number != null ? number.intValue() : 0;
    }

    private static long longValue(Number number) {
        return number != null ? number.longValue() : 0;
    }

    private static float floatValue(Number number) {
        return number != null ? number.floatValue() : .0f;
    }

    private static double doubleValue(Number number) {
        return number != null ? number.doubleValue() : .0f;
    }
}
