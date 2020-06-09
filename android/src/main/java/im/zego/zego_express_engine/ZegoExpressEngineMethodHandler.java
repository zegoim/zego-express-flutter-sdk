//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine;

import android.app.Application;
import android.graphics.Rect;

import org.json.JSONObject;

import java.lang.*;
import java.util.ArrayList;
import java.util.HashMap;

import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoIMSendBarrageMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBroadcastMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendCustomCommandCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerLoadResourceCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerSeekToCallback;
import im.zego.zegoexpress.callback.IZegoMixerStartCallback;
import im.zego.zegoexpress.callback.IZegoMixerStopCallback;
import im.zego.zegoexpress.callback.IZegoPublisherSetStreamExtraInfoCallback;
import im.zego.zegoexpress.callback.IZegoPublisherUpdateCdnUrlCallback;
import im.zego.zegoexpress.constants.ZegoAECMode;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioCodecID;
import im.zego.zegoexpress.constants.ZegoCapturePipelineScaleMode;
import im.zego.zegoexpress.constants.ZegoLanguage;
import im.zego.zegoexpress.constants.ZegoMediaPlayerState;
import im.zego.zegoexpress.constants.ZegoMixerInputContentType;
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
import im.zego.zegoexpress.entity.ZegoMixerAudioConfig;
import im.zego.zegoexpress.entity.ZegoMixerInput;
import im.zego.zegoexpress.entity.ZegoMixerOutput;
import im.zego.zegoexpress.entity.ZegoMixerTask;
import im.zego.zegoexpress.entity.ZegoMixerVideoConfig;
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

    private static ZegoExpressEngineEventHandler eventHandler = null;

    private static HashMap<Integer, ZegoMediaPlayer> mediaPlayerHashMap = new HashMap<>();

    /* Main */

    @SuppressWarnings("unused")
    public static void createEngine(MethodCall call, Result result, Application application, EventChannel.EventSink sink, TextureRegistry registry) {

        long appID = longValue((Number)call.argument("appID"));
        String appSign = call.argument("appSign");
        boolean isTestEnv = boolValue((Boolean) call.argument("isTestEnv"));
        ZegoScenario scenario = ZegoScenario.getZegoScenario(intValue((Number)call.argument("scenario")));

        enablePlatformView = boolValue((Boolean) call.argument("enablePlatformView"));
        textureRegistry = registry;

        eventHandler = new ZegoExpressEngineEventHandler(sink);

        ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario, application, eventHandler.eventHandler);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void destroyEngine(MethodCall call, Result result) {

        ZegoExpressEngine.destroyEngine(null);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getVersion(MethodCall call, Result result) {

        result.success(ZegoExpressEngine.getVersion());
    }

    @SuppressWarnings("unused")
    public static void uploadLog(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().uploadLog();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setDebugVerbose(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoLanguage language = ZegoLanguage.getZegoLanguage(intValue((Number)call.argument("language")));

        ZegoExpressEngine.getEngine().setDebugVerbose(enable, language);

        result.success(null);
    }


    /* Room */

    @SuppressWarnings("unused")
    public static void loginRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        HashMap<String, Object> userMap = call.argument("user");
        ZegoUser user = new ZegoUser((String) (userMap != null ? userMap.get("userID") : null), (String) (userMap != null ? userMap.get("userName") : null));

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

    @SuppressWarnings("unused")
    public static void logoutRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        ZegoExpressEngine.getEngine().logoutRoom(roomID);

        result.success(null);
    }


    /* Publisher */

    @SuppressWarnings("unused")
    public static void startPublishingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().startPublishingStream(streamID, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopPublishingStream(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopPublishingStream(channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
    public static void stopPreview(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopPreview(channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
    public static void getVideoConfig(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoVideoConfig config = ZegoExpressEngine.getEngine().getVideoConfig(channel);

        HashMap<String, Object> resultMap = new HashMap<>();
        // TODO: GetVideoConfig
        result.success(resultMap);
    }

    @SuppressWarnings("unused")
    public static void setVideoMirrorMode(MethodCall call, Result result) {

        ZegoVideoMirrorMode mirrorMode = ZegoVideoMirrorMode.getZegoVideoMirrorMode(intValue((Number) call.argument("mirrorMode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoMirrorMode(mirrorMode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAppOrientation(MethodCall call, Result result) {

        ZegoOrientation orientation = ZegoOrientation.values()[intValue((Number) call.argument("orientation"))];
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setAppOrientation(orientation, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
    public static void getAudioConfig(MethodCall call, Result result) {

        ZegoAudioConfig config = ZegoExpressEngine.getEngine().getAudioConfig();

        HashMap<String, Object> resultMap = new HashMap<>();
        resultMap.put("bitrate", config.bitrate);
        resultMap.put("channel", config.channel.value());
        resultMap.put("codecID", config.codecID.value());
        result.success(resultMap);
    }

    @SuppressWarnings("unused")
    public static void mutePublishStreamAudio(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamAudio(mute, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePublishStreamVideo(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamVideo(mute, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableTrafficControl(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        int property = intValue((Number) call.argument("property"));

        ZegoExpressEngine.getEngine().enableTrafficControl(enable, property);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setMinVideoBitrateForTrafficControl(MethodCall call, Result result) {

        int bitrate = intValue((Number) call.argument("bitrate"));
        ZegoTrafficControlMinVideoBitrateMode mode = ZegoTrafficControlMinVideoBitrateMode.getZegoTrafficControlMinVideoBitrateMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setMinVideoBitrateForTrafficControl(bitrate, mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCaptureVolume(MethodCall call, Result result) {

        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setCaptureVolume(volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
    public static void sendSEI(MethodCall call, Result result) {

        byte[] data = call.argument("data");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendSEI(data, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHardwareEncoder(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareEncoder(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCapturePipelineScaleMode(MethodCall call, Result result) {

        ZegoCapturePipelineScaleMode mode = ZegoCapturePipelineScaleMode.getZegoCapturePipelineScaleMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setCapturePipelineScaleMode(mode);

        result.success(null);
    }


    /* Player */

    @SuppressWarnings({"unused", "unchecked"})
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

    @SuppressWarnings("unused")
    public static void stopPlayingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");

        ZegoExpressEngine.getEngine().stopPlayingStream(streamID);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayVolume(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setPlayVolume(streamID, volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePlayStreamAudio(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamAudio(streamID, mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePlayStreamVideo(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamVideo(streamID, mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHardwareDecoder(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareDecoder(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCheckPoc(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableCheckPoc(enable);

        result.success(null);
    }


    /* Mixer */

    @SuppressWarnings("unused")
    public static void startMixerTask(MethodCall call, final Result result) {

        String taskID = call.argument("taskID");
        ZegoMixerTask taskObject = new ZegoMixerTask(taskID);

        // MixerInput
        ArrayList<HashMap<String, Object>> inputListMap = call.argument("inputList");
        if (inputListMap != null && !inputListMap.isEmpty()) {
            ArrayList<ZegoMixerInput> inputListObject= new ArrayList<>();
            for (HashMap<String, Object> inputMap: inputListMap) {
                String streamID = (String) inputMap.get("streamID");
                int contentType = intValue((Number) inputMap.get("contentType"));
                int left = intValue((Number) inputMap.get("left"));
                int top = intValue((Number) inputMap.get("top"));
                int right = intValue((Number) inputMap.get("right"));
                int bottom = intValue((Number) inputMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                int soundLevelID = intValue((Number) inputMap.get("soundLevelID"));
                ZegoMixerInput inputObject = new ZegoMixerInput(streamID, ZegoMixerInputContentType.getZegoMixerInputContentType(contentType), rect, soundLevelID);
                inputListObject.add(inputObject);
            }
            taskObject.setInputList(inputListObject);
        }

        // MixerOutput
        ArrayList<HashMap<String, Object>> outputListMap = call.argument("inputList");
        if (outputListMap != null && !outputListMap.isEmpty()) {
            ArrayList<ZegoMixerOutput> outputListObject = new ArrayList<>();
            for (HashMap<String, Object> outputMap : outputListMap) {
                String target = (String) outputMap.get("target");
                ZegoMixerOutput outputObject = new ZegoMixerOutput(target);
                outputListObject.add(outputObject);
            }
            taskObject.setOutputList(outputListObject);
        }

        // AudioConfig
        HashMap<String, Object> audioConfigMap = call.argument("audioConfig");
        if (audioConfigMap != null && !audioConfigMap.isEmpty()) {
            int bitrate = intValue((Number) audioConfigMap.get("bitrate"));
            int channel = intValue((Number) audioConfigMap.get("channel"));
            int codecID = intValue((Number) audioConfigMap.get("codecID"));
            ZegoMixerAudioConfig audioConfigObject = new ZegoMixerAudioConfig();
            audioConfigObject.bitrate = bitrate;
            audioConfigObject.channel = ZegoAudioChannel.getZegoAudioChannel(channel);
            audioConfigObject.codecID = ZegoAudioCodecID.getZegoAudioCodecID(codecID);

            taskObject.setAudioConfig(audioConfigObject);
        }

        // VideoConfig
        HashMap<String, Object> videoConfigMap = call.argument("videoConfig");
        if (videoConfigMap != null && !videoConfigMap.isEmpty()) {
            int width = intValue((Number) videoConfigMap.get("width"));
            int height = intValue((Number) videoConfigMap.get("height"));
            int fps = intValue((Number) videoConfigMap.get("fps"));
            int bitrate = intValue((Number) videoConfigMap.get("bitrate"));
            ZegoMixerVideoConfig videoConfigObject = new ZegoMixerVideoConfig(width, height, fps, bitrate);

            taskObject.setVideoConfig(videoConfigObject);
        }

        // Watermark
        HashMap<String, Object> watermarkMap = call.argument("watermark");
        if (watermarkMap != null && !watermarkMap.isEmpty()) {
            String imageURL = (String) watermarkMap.get("imageURL");
            int left = intValue((Number) watermarkMap.get("left"));
            int top = intValue((Number) watermarkMap.get("top"));
            int right = intValue((Number) watermarkMap.get("right"));
            int bottom = intValue((Number) watermarkMap.get("bottom"));
            Rect rect = new Rect(left, top, right, bottom);
            ZegoWatermark watermarkObject = new ZegoWatermark(imageURL, rect);

            taskObject.setWatermark(watermarkObject);
        }

        // Background Image
        String backgroundImageURL = call.argument("backgroundImageURL");

        if (backgroundImageURL != null && backgroundImageURL.length() > 0) {
            taskObject.setBackgroundImageURL(backgroundImageURL);
        }

        // Enable SoundLevel
        boolean enableSoundLevel = boolValue((Boolean) call.argument("enableSoundLevel"));

        taskObject.enableSoundLevel(enableSoundLevel);


        ZegoExpressEngine.getEngine().startMixerTask(taskObject, new IZegoMixerStartCallback() {
            @Override
            public void onMixerStartResult(int errorCode, JSONObject extendedData) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                resultMap.put("extendedData", extendedData.toString());
                result.success(resultMap);
            }
        });
    }

    @SuppressWarnings("unused")
    public static void stopMixerTask(MethodCall call, final Result result) {

        String taskID = call.argument("taskID");
        ZegoMixerTask taskObject = new ZegoMixerTask(taskID);

        // MixerInput
        ArrayList<HashMap<String, Object>> inputListMap = call.argument("inputList");
        if (inputListMap != null && !inputListMap.isEmpty()) {
            ArrayList<ZegoMixerInput> inputListObject= new ArrayList<>();
            for (HashMap<String, Object> inputMap: inputListMap) {
                String streamID = (String) inputMap.get("streamID");
                int contentType = intValue((Number) inputMap.get("contentType"));
                int left = intValue((Number) inputMap.get("left"));
                int top = intValue((Number) inputMap.get("top"));
                int right = intValue((Number) inputMap.get("right"));
                int bottom = intValue((Number) inputMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                int soundLevelID = intValue((Number) inputMap.get("soundLevelID"));
                ZegoMixerInput inputObject = new ZegoMixerInput(streamID, ZegoMixerInputContentType.getZegoMixerInputContentType(contentType), rect, soundLevelID);
                inputListObject.add(inputObject);
            }
            taskObject.setInputList(inputListObject);
        }

        // MixerOutput
        ArrayList<HashMap<String, Object>> outputListMap = call.argument("inputList");
        if (outputListMap != null && !outputListMap.isEmpty()) {
            ArrayList<ZegoMixerOutput> outputListObject = new ArrayList<>();
            for (HashMap<String, Object> outputMap : outputListMap) {
                String target = (String) outputMap.get("target");
                ZegoMixerOutput outputObject = new ZegoMixerOutput(target);
                outputListObject.add(outputObject);
            }
            taskObject.setOutputList(outputListObject);
        }

        // no need to set audio config

        // no need to set video config

        // no need to set watermark

        // no need to set background image

        // no need to set enable sound level

        ZegoExpressEngine.getEngine().stopMixerTask(taskObject, new IZegoMixerStopCallback() {
            @Override
            public void onMixerStopResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
    }


    /* Device */

    @SuppressWarnings("unused")
    public static void muteMicrophone(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteMicrophone(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isMicrophoneMuted(MethodCall call, Result result) {

        boolean muted = ZegoExpressEngine.getEngine().isMicrophoneMuted();

        result.success(muted);
    }

    @SuppressWarnings("unused")
    public static void muteSpeaker(MethodCall call, Result result) {

        boolean mute = boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteSpeaker(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isSpeakerMuted(MethodCall call, Result result) {

        boolean muted = ZegoExpressEngine.getEngine().isSpeakerMuted();

        result.success(muted);
    }

    @SuppressWarnings("unused")
    public static void enableAudioCaptureDevice(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAudioCaptureDevice(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setBuiltInSpeakerOn(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().setBuiltInSpeakerOn(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCamera(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableCamera(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void useFrontCamera(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().useFrontCamera(enable, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().startSoundLevelMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopSoundLevelMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startAudioSpectrumMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().startAudioSpectrumMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopAudioSpectrumMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopAudioSpectrumMonitor();

        result.success(null);
    }


    /* PreProcess */

    @SuppressWarnings("unused")
    public static void enableAEC(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAEC(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAECMode(MethodCall call, Result result) {

        ZegoAECMode mode = ZegoAECMode.getZegoAECMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setAECMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableAGC(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAGC(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableANS(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableANS(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableBeautify(MethodCall call, Result result) {

        int featureBitmask = intValue((Number) call.argument("featureBitmask"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableBeautify(featureBitmask, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
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
    }

    @SuppressWarnings("unused")
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


    /* MediaPlayer */

    @SuppressWarnings("unused")
    public static void createMediaPlayer(MethodCall call, Result result) {

        ZegoMediaPlayer mediaPlayer = ZegoExpressEngine.getEngine().createMediaPlayer();

        if (mediaPlayer != null) {
            int index = mediaPlayer.getIndex();

            mediaPlayer.setEventHandler(eventHandler.mediaPlayerEventHandler);
            mediaPlayerHashMap.put(index, mediaPlayer);

            result.success(index);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyMediaPlayer(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyMediaPlayer(mediaPlayer);
        }

        mediaPlayerHashMap.remove(index);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerLoadResource(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            String path = call.argument("path");
            mediaPlayer.loadResource(path, new IZegoMediaPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerStart(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.start();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerStop(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.stop();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerPause(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.pause();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerResume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.resume();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSeekTo(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            long millisecond = longValue((Number) call.argument("millisecond"));
            mediaPlayer.seekTo(millisecond, new IZegoMediaPlayerSeekToCallback() {
                @Override
                public void onSeekToTimeCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableRepeat(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = boolValue((Boolean) call.argument("enable"));
            mediaPlayer.enableRepeat(enable);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableAux(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = boolValue((Boolean) call.argument("enable"));
            mediaPlayer.enableAux(enable);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerMuteLocal(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean mute = boolValue((Boolean) call.argument("mute"));
            mediaPlayer.muteLocal(mute);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = intValue((Number) call.argument("volume"));
            mediaPlayer.setVolume(volume);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetProgressInterval(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            long millisecond = longValue((Number) call.argument("millisecond"));
            mediaPlayer.setProgressInterval(millisecond);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = mediaPlayer.getVolume();
            result.success(volume);
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetTotalDuration(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            long totalDuration = mediaPlayer.getTotalDuration();
            result.success(totalDuration);
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetCurrentProgress(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            long currentProgress = mediaPlayer.getCurrentProgress();
            result.success(currentProgress);
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetCurrentState(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaPlayerState state = mediaPlayer.getCurrentState();
            result.success(state.value());
        } else {
            result.success(0);
        }
    }


    /* PlatformView Utils */

    @SuppressWarnings("unused")
    public static void destroyPlatformView(MethodCall call, Result result) {

        int viewID = intValue((Number) call.argument("viewID"));

        ZegoPlatformViewFactory.getInstance().destroyPlatformView(viewID);

        result.success(null);
    }


    /* TextureRenderer Utils */

    @SuppressWarnings("unused")
    public static void createTextureRenderer(MethodCall call, Result result) {

        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        Long textureID = ZegoTextureRendererController.getInstance().createTextureRenderer(textureRegistry.createSurfaceTexture(), width, height);

        result.success(textureID);
    }

    @SuppressWarnings("unused")
    public static void updateTextureRenderer(MethodCall call, Result result) {

        Long textureID = longValue((Number) call.argument("textureID"));
        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        ZegoTextureRendererController.getInstance().updateTextureRenderer(textureID, width, height);

        result.success(null);
    }

    @SuppressWarnings("unused")
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

    private static double doubleValue(Number number) {
        return number != null ? number.doubleValue() : .0f;
    }
}
