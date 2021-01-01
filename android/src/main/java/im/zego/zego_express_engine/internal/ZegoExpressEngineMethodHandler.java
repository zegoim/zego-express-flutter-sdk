//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import android.app.Application;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.lang.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;

import im.zego.zego_express_engine.ZegoCustomVideoCaptureManager;
import im.zego.zegoexpress.ZegoAudioEffectPlayer;
import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.callback.IZegoAudioEffectPlayerLoadResourceCallback;
import im.zego.zegoexpress.callback.IZegoAudioEffectPlayerSeekToCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBarrageMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBroadcastMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendCustomCommandCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerLoadResourceCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerSeekToCallback;
import im.zego.zegoexpress.callback.IZegoMixerStartCallback;
import im.zego.zegoexpress.callback.IZegoMixerStopCallback;
import im.zego.zegoexpress.callback.IZegoPlayerTakeSnapshotCallback;
import im.zego.zegoexpress.callback.IZegoPublisherSetStreamExtraInfoCallback;
import im.zego.zegoexpress.callback.IZegoPublisherTakeSnapshotCallback;
import im.zego.zegoexpress.callback.IZegoPublisherUpdateCdnUrlCallback;
import im.zego.zegoexpress.callback.IZegoRoomSetRoomExtraInfoCallback;
import im.zego.zegoexpress.constants.ZegoAECMode;
import im.zego.zegoexpress.constants.ZegoANSMode;
import im.zego.zegoexpress.constants.ZegoAudioCaptureStereoMode;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioCodecID;
import im.zego.zegoexpress.constants.ZegoCapturePipelineScaleMode;
import im.zego.zegoexpress.constants.ZegoDataRecordType;
import im.zego.zegoexpress.constants.ZegoLanguage;
import im.zego.zegoexpress.constants.ZegoMediaPlayerAudioChannel;
import im.zego.zegoexpress.constants.ZegoMediaPlayerState;
import im.zego.zegoexpress.constants.ZegoMixerInputContentType;
import im.zego.zegoexpress.constants.ZegoOrientation;
import im.zego.zegoexpress.constants.ZegoPlayerVideoLayer;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoReverbPreset;
import im.zego.zegoexpress.constants.ZegoSEIType;
import im.zego.zegoexpress.constants.ZegoScenario;
import im.zego.zegoexpress.constants.ZegoStreamResourceMode;
import im.zego.zegoexpress.constants.ZegoTrafficControlMinVideoBitrateMode;
import im.zego.zegoexpress.constants.ZegoVideoBufferType;
import im.zego.zegoexpress.constants.ZegoVideoCodecID;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.constants.ZegoViewMode;
import im.zego.zegoexpress.constants.ZegoVoiceChangerPreset;
import im.zego.zegoexpress.entity.ZegoAudioConfig;
import im.zego.zegoexpress.entity.ZegoAudioEffectPlayConfig;
import im.zego.zegoexpress.entity.ZegoBeautifyOption;
import im.zego.zegoexpress.entity.ZegoCDNConfig;
import im.zego.zegoexpress.entity.ZegoCanvas;
import im.zego.zegoexpress.entity.ZegoCustomVideoCaptureConfig;
import im.zego.zegoexpress.entity.ZegoDataRecordConfig;
import im.zego.zegoexpress.entity.ZegoEngineConfig;
import im.zego.zegoexpress.entity.ZegoLogConfig;
import im.zego.zegoexpress.entity.ZegoMixerAudioConfig;
import im.zego.zegoexpress.entity.ZegoMixerInput;
import im.zego.zegoexpress.entity.ZegoMixerOutput;
import im.zego.zegoexpress.entity.ZegoMixerTask;
import im.zego.zegoexpress.entity.ZegoMixerVideoConfig;
import im.zego.zegoexpress.entity.ZegoNetworkSpeedTestConfig;
import im.zego.zegoexpress.entity.ZegoPlayerConfig;
import im.zego.zegoexpress.entity.ZegoReverbAdvancedParam;
import im.zego.zegoexpress.entity.ZegoReverbEchoParam;
import im.zego.zegoexpress.entity.ZegoReverbParam;
import im.zego.zegoexpress.entity.ZegoRoomConfig;
import im.zego.zegoexpress.entity.ZegoSEIConfig;
import im.zego.zegoexpress.entity.ZegoUser;
import im.zego.zegoexpress.entity.ZegoVideoConfig;
import im.zego.zegoexpress.entity.ZegoVoiceChangerParam;
import im.zego.zegoexpress.entity.ZegoWatermark;
import io.flutter.embedding.engine.plugins.FlutterPlugin.FlutterPluginBinding;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;
import io.flutter.view.TextureRegistry;

import static im.zego.zego_express_engine.internal.ZegoUtils.boolValue;
import static im.zego.zego_express_engine.internal.ZegoUtils.floatArrayValueFromDoubleArray;
import static im.zego.zego_express_engine.internal.ZegoUtils.intArrayValue;
import static im.zego.zego_express_engine.internal.ZegoUtils.intValue;
import static im.zego.zego_express_engine.internal.ZegoUtils.longValue;
import static im.zego.zego_express_engine.internal.ZegoUtils.floatValue;
import static im.zego.zego_express_engine.internal.ZegoUtils.doubleValue;

public class ZegoExpressEngineMethodHandler {

    private static Registrar registrar = null;

    private static FlutterPluginBinding pluginBinding = null;

    private static Application application = null;

    private static TextureRegistry textureRegistry = null;

    private static boolean enablePlatformView = false;

    private static final HashMap<Integer, ZegoMediaPlayer> mediaPlayerHashMap = new HashMap<>();

    private static final HashMap<Integer, ZegoAudioEffectPlayer> audioEffectPlayerHashMap = new HashMap<>();

    /* Main */
    @SuppressWarnings("unused")
    public static void createEngine(MethodCall call, Result result, Registrar reg, FlutterPluginBinding binding, EventChannel.EventSink sink) {

        long appID = longValue((Number)call.argument("appID"));
        String appSign = call.argument("appSign");
        boolean isTestEnv = boolValue((Boolean) call.argument("isTestEnv"));
        ZegoScenario scenario = ZegoScenario.getZegoScenario(intValue((Number)call.argument("scenario")));

        enablePlatformView = boolValue((Boolean) call.argument("enablePlatformView"));

        if (binding != null) {
            application = (Application) binding.getApplicationContext();
            textureRegistry = binding.getTextureRegistry();
        } else {
            application = (Application) reg.context();
            textureRegistry = reg.textures();
        }

        registrar = reg;
        pluginBinding = binding;

        // Set eventSink for ZegoExpressEngineEventHandler
        if (sink == null) {
            ZegoLog.error("[createEngine] FlutterEventSink is null");
        }
        ZegoExpressEngineEventHandler.getInstance().sink = sink;

        ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario, application, ZegoExpressEngineEventHandler.getInstance().eventHandler);
        setPlatformLanguage();

        ZegoExpressEngine.getEngine().setDataRecordEventHandler(ZegoExpressEngineEventHandler.getInstance().dataRecordEventHandler);

        ZegoLog.log("[createEngine] platform:Android, enablePlatformView:%s, sink: %d, appID:%d, appSign:%s, isTestEnv:%s, scenario:%s", enablePlatformView ? "true" : "false", sink!=null ? sink.hashCode() : -1, appID, appSign, isTestEnv ? "true" : "false", scenario.name());

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void destroyEngine(MethodCall call, Result result) {

        ZegoExpressEngine.destroyEngine(null);

        result.success(null);
    }

    @SuppressWarnings({"unused", "unchecked"})
    public static void setEngineConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        ZegoEngineConfig configObject;
        if (configMap != null && !configMap.isEmpty()) {
            configObject = new ZegoEngineConfig();
            configObject.advancedConfig = (HashMap<String, String>) configMap.get("advancedConfig");

            HashMap<String, Object> logConfigMap = call.argument("logConfig");
            ZegoLogConfig logConfigObject;
            if (logConfigMap != null && !logConfigMap.isEmpty()) {
                logConfigObject = new ZegoLogConfig();
                logConfigObject.logPath = (String) logConfigMap.get("logPath");
                logConfigObject.logSize = intValue((Number) configMap.get("logSize"));

                configObject.logConfig = logConfigObject;
            }

            ZegoExpressEngine.setEngineConfig(configObject);

            result.success(null);
        } else {
            result.error("setEngineConfig_null_config".toUpperCase(), "Invoke `setEngineConfig` with null config", null);
        }
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
    public static void loginMultiRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        HashMap<String, Object> configMap = call.argument("config");
        ZegoRoomConfig roomConfig = null;
        if (configMap != null && !configMap.isEmpty()) {
            roomConfig = new ZegoRoomConfig();
            roomConfig.isUserStatusNotify = boolValue((Boolean)configMap.get("isUserStatusNotify"));
            roomConfig.maxMemberCount = intValue((Number)configMap.get("maxMemberCount"));
            roomConfig.token = (String)configMap.get("token");
        }

        ZegoExpressEngine.getEngine().loginMultiRoom(roomID, roomConfig);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void logoutRoom(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        ZegoExpressEngine.getEngine().logoutRoom(roomID);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void switchRoom(MethodCall call, Result result) {

        String fromRoomID = call.argument("fromRoomID");
        String toRoomID = call.argument("toRoomID");

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap != null && !configMap.isEmpty()) {
            ZegoRoomConfig roomConfig = new ZegoRoomConfig();
            roomConfig.isUserStatusNotify = boolValue((Boolean)configMap.get("isUserStatusNotify"));
            roomConfig.maxMemberCount = intValue((Number)configMap.get("maxMemberCount"));
            roomConfig.token = (String)configMap.get("token");
            ZegoExpressEngine.getEngine().switchRoom(fromRoomID, toRoomID, roomConfig);
        } else {
            ZegoExpressEngine.getEngine().switchRoom(fromRoomID, toRoomID);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setRoomExtraInfo(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");
        String key = call.argument("key");
        String value = call.argument("value");

        ZegoExpressEngine.getEngine().setRoomExtraInfo(roomID, key, value, new IZegoRoomSetRoomExtraInfoCallback() {
            @Override
            public void onRoomSetRoomExtraInfoResult(int errorCode) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
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

            Object view;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getSurfaceView();
                } else {
                    // Preview video without creating the PlatformView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                    String errorMessage = String.format(Locale.ENGLISH, "The PlatformView for viewID:%d cannot be found, developer should call `createPlatformView` first and get the viewID", viewID);
                    ZegoLog.error("[startPreview] %s", errorMessage);
                    result.error("startPreview_No_PlatformView".toUpperCase(), errorMessage, null);
                    return;
                }

            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long)viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Preview video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createTextureRenderer` first and get the textureID", viewID);
                    ZegoLog.error("[startPreview] %s", errorMessage);
                    result.error("startPreview_No_TextureRenderer".toUpperCase(), errorMessage, null);
                    return;
                }
            }

            ZegoCanvas canvas = null;

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;

                // Mark the canvas is in use
                if (!enablePlatformView) {
                    ZegoTextureRendererController.getInstance().previewCanvasInUse.put(channel, canvas);
                }
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

        // Mark the canvas is no longer used
        if (!enablePlatformView) {
            ZegoTextureRendererController.getInstance().previewCanvasInUse.remove(channel);
        }

        ZegoExpressEngine.getEngine().stopPreview(channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVideoConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) {
            result.error("setVideoConfig_Null_Config".toUpperCase(), "[setVideoConfig] Null config", null);
            return;
        }

        int captureWidth = intValue((Number) configMap.get("captureWidth"));
        int captureHeight = intValue((Number) configMap.get("captureHeight"));
        int encodeWidth = intValue((Number) configMap.get("encodeWidth"));
        int encodeHeight = intValue((Number) configMap.get("encodeHeight"));
        int fps = intValue((Number) configMap.get("fps"));
        int bitrate = intValue((Number) configMap.get("bitrate"));
        int codecID = intValue((Number) configMap.get("codecID"));

        ZegoVideoConfig config = new ZegoVideoConfig();
        config.setCaptureResolution(captureWidth, captureHeight);
        config.setEncodeResolution(encodeWidth, encodeHeight);
        config.setVideoFPS(fps);
        config.setVideoBitrate(bitrate);
        config.setCodecID(ZegoVideoCodecID.getZegoVideoCodecID(codecID));

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoConfig(config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getVideoConfig(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoVideoConfig config = ZegoExpressEngine.getEngine().getVideoConfig(channel);

        HashMap<String, Object> resultMap = new HashMap<>();
        resultMap.put("captureWidth", config.captureWidth);
        resultMap.put("captureHeight", config.captureHeight);
        resultMap.put("encodeWidth", config.encodeWidth);
        resultMap.put("encodeHeight", config.encodeHeight);
        resultMap.put("fps", config.fps);
        resultMap.put("bitrate", config.bitrate);
        resultMap.put("codecID", config.codecID.value());
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
        if (configMap == null || configMap.isEmpty()) {
            result.error("setAudioConfig_Null_Config".toUpperCase(), "[setAudioConfig] Null config", null);
            return;
        }

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
    public static void setPublishStreamEncryptionKey(MethodCall call, Result result) {

        String key = call.argument("key");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setPublishStreamEncryptionKey(key, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void takePublishStreamSnapshot(MethodCall call, final Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().takePublishStreamSnapshot(new IZegoPublisherTakeSnapshotCallback() {
            @Override
            public void onPublisherTakeSnapshotResult(final int errorCode, final Bitmap image) {

                if (image != null) {
                    new Thread(new Runnable(){
                        public void run() {
                            ByteArrayOutputStream stream = new ByteArrayOutputStream();
                            image.compress(Bitmap.CompressFormat.JPEG, 100, stream);
                            final byte[] imageData = stream.toByteArray();
                            image.recycle();

                            new Handler(Looper.getMainLooper()).post(new Runnable() {
                                @Override
                                public void run() {
                                    HashMap<String, Object> resultMap = new HashMap<>();
                                    resultMap.put("errorCode", errorCode);
                                    resultMap.put("image", imageData);
                                    result.success(resultMap);
                                }
                            });
                        }
                    }).start();

                } else {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("image", null);
                    result.success(resultMap);
                }
            }
        }, channel);
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
    public static void setAudioCaptureStereoMode(MethodCall call, Result result) {

        ZegoAudioCaptureStereoMode mode = ZegoAudioCaptureStereoMode.getZegoAudioCaptureStereoMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setAudioCaptureStereoMode(mode);

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

        ZegoCDNConfig config = null;
        if (configMap != null && !configMap.isEmpty()) {
            config = new ZegoCDNConfig();
            config.url = configMap.get("url");
            config.authParam = configMap.get("authParam");
        }

        boolean enable = boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enablePublishDirectToCDN(enable, config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPublishWatermark(MethodCall call, Result result) {

        HashMap<String, Object> watermarkMap = call.argument("watermark");

        ZegoWatermark watermark = null;
        if (watermarkMap != null && !watermarkMap.isEmpty()) {
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
    public static void setSEIConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");

        ZegoSEIConfig config = new ZegoSEIConfig(); // Use default config
        if (configMap != null && !configMap.isEmpty()) {
            config.type = ZegoSEIType.getZegoSEIType(intValue((Number) configMap.get("type")));
        }

        ZegoExpressEngine.getEngine().setSEIConfig(config);

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

    @SuppressWarnings({"unused", "unchecked", "deprecation"})
    public static void startPlayingStream(MethodCall call, Result result) {
        // TODO: Deprecated since 1.19.0

        String streamID = call.argument("streamID");

        // Handle ZegoPlayerConfig

        ZegoPlayerConfig playerConfig = null;

        HashMap<String, Object> playerConfigMap = call.argument("config");

        if (playerConfigMap != null && !playerConfigMap.isEmpty()) {

            playerConfig = new ZegoPlayerConfig();
            playerConfig.resourceMode = ZegoStreamResourceMode.getZegoStreamResourceMode(intValue((Number) playerConfigMap.get("resourceMode")));
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

            Object view;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getSurfaceView();
                } else {
                    // Play video without creating the PlatformView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                    String errorMessage = String.format(Locale.ENGLISH, "The PlatformView for viewID:%d cannot be found, developer should call `createPlatformView` first and get the viewID", viewID);
                    ZegoLog.error("[startPlayingStream] %s", errorMessage);
                    result.error("startPlayingStream_No_PlatformView".toUpperCase(), errorMessage, null);
                    return;
                }

            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long)viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Play video without creating TextureRenderer in advance
                    // Need to invoke dart `createTextureRenderer` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createTextureRenderer` first and get the textureID", viewID);
                    ZegoLog.error("[startPlayingStream] %s", errorMessage);
                    result.error("startPlayingStream_No_TextureRenderer".toUpperCase(), errorMessage, null);
                    return;
                }
            }

            ZegoCanvas canvas = null;

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;
            }

            // Mark the canvas is in use
            if (!enablePlatformView) {
                ZegoTextureRendererController.getInstance().playerCanvasInUse.put(streamID, canvas);
                if (playerConfig != null) {
                    ZegoTextureRendererController.getInstance().playerConfigInUse.put(streamID, playerConfig);
                }
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

        // Mark the canvas is no longer used
        if (!enablePlatformView) {
            ZegoTextureRendererController.getInstance().playerCanvasInUse.remove(streamID);
            ZegoTextureRendererController.getInstance().playerConfigInUse.remove(streamID);
        }

        ZegoExpressEngine.getEngine().stopPlayingStream(streamID);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayStreamDecryptionKey(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        String key = call.argument("key");

        ZegoExpressEngine.getEngine().setPlayStreamDecryptionKey(streamID, key);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void takePlayStreamSnapshot(MethodCall call, final Result result) {

        String streamID = call.argument("streamID");

        ZegoExpressEngine.getEngine().takePlayStreamSnapshot(streamID, new IZegoPlayerTakeSnapshotCallback() {
            @Override
            public void onPlayerTakeSnapshotResult(final int errorCode, final Bitmap image) {

                if (image != null) {
                    new Thread(new Runnable(){
                        public void run() {
                            ByteArrayOutputStream stream = new ByteArrayOutputStream();
                            image.compress(Bitmap.CompressFormat.JPEG, 100, stream);
                            final byte[] imageData = stream.toByteArray();
                            image.recycle();

                            new Handler(Looper.getMainLooper()).post(new Runnable() {
                                @Override
                                public void run() {
                                    HashMap<String, Object> resultMap = new HashMap<>();
                                    resultMap.put("errorCode", errorCode);
                                    resultMap.put("image", imageData);
                                    result.success(resultMap);
                                }
                            });
                        }
                    }).start();

                } else {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("image", null);
                    result.success(resultMap);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setPlayVolume(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setPlayVolume(streamID, volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayStreamVideoLayer(MethodCall call, Result result) {

        String streamID = call.argument("streamID");

        ZegoPlayerVideoLayer videoLayer = ZegoPlayerVideoLayer.getZegoPlayerVideoLayer(intValue((Number) call.argument("videoLayer")));

        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setPlayStreamVideoLayer(streamID, videoLayer);

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
        ArrayList<HashMap<String, Object>> outputListMap = call.argument("outputList");
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
            if (imageURL != null && imageURL.length() > 0) {
                int left = intValue((Number) watermarkMap.get("left"));
                int top = intValue((Number) watermarkMap.get("top"));
                int right = intValue((Number) watermarkMap.get("right"));
                int bottom = intValue((Number) watermarkMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                ZegoWatermark watermarkObject = new ZegoWatermark(imageURL, rect);

                taskObject.setWatermark(watermarkObject);
            }
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
    public static void setCameraZoomFactor(MethodCall call, Result result) {

        float factor = floatValue((Number) call.argument("factor"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraZoomFactor(factor, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getCameraMaxZoomFactor(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        float factor = ZegoExpressEngine.getEngine().getCameraMaxZoomFactor(channel);

        result.success(factor);
    }

    @SuppressWarnings("unused")
    public static void startSoundLevelMonitor(MethodCall call, Result result) {

        int millisecond = intValue((Number) call.argument("millisecond"));

        ZegoExpressEngine.getEngine().startSoundLevelMonitor(millisecond);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopSoundLevelMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startAudioSpectrumMonitor(MethodCall call, Result result) {

        int millisecond = intValue((Number) call.argument("millisecond"));

        ZegoExpressEngine.getEngine().startAudioSpectrumMonitor(millisecond);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopAudioSpectrumMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopAudioSpectrumMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHeadphoneMonitor(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHeadphoneMonitor(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setHeadphoneMonitorVolume(MethodCall call, Result result) {

        int volume = intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setHeadphoneMonitorVolume(volume);

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
    public static void enableTransientANS(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableTransientANS(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setANSMode(MethodCall call, Result result) {

        ZegoANSMode mode = ZegoANSMode.getZegoANSMode(intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setANSMode(mode);

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
        if (optionMap == null || optionMap.isEmpty()) {
            result.error("setBeautifyOption_Null_Option".toUpperCase(), "[setBeautifyOption] Null option", null);
            return;
        }

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
    public static void setAudioEqualizerGain(MethodCall call, Result result) {

        int bandIndex = intValue((Number) call.argument("bandIndex"));
        float bandGain = floatValue((Number) call.argument("bandGain"));

        ZegoExpressEngine.getEngine().setAudioEqualizerGain(bandIndex, bandGain);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVoiceChangerPreset(MethodCall call, Result result) {

        ZegoVoiceChangerPreset preset = ZegoVoiceChangerPreset.getZegoVoiceChangerPreset(intValue((Number) call.argument("preset")));

        ZegoExpressEngine.getEngine().setVoiceChangerPreset(preset);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVoiceChangerParam(MethodCall call, Result result) {

        HashMap<String, Double> paramMap = call.argument("param");
        if (paramMap == null || paramMap.isEmpty()) {
            result.error("setVoiceChangerParam_Null_Param".toUpperCase(), "[setVoiceChangerParam] Null param", null);
            return;
        }

        ZegoVoiceChangerParam param = new ZegoVoiceChangerParam();
        param.pitch = floatValue(paramMap.get("pitch"));

        ZegoExpressEngine.getEngine().setVoiceChangerParam(param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setReverbPreset(MethodCall call, Result result) {

        ZegoReverbPreset preset = ZegoReverbPreset.getZegoReverbPreset(intValue((Number) call.argument("preset")));

        ZegoExpressEngine.getEngine().setReverbPreset(preset);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setReverbAdvancedParam(MethodCall call, Result result) {

        HashMap<String, Object> paramMap = call.argument("param");
        if (paramMap == null || paramMap.isEmpty()) {
            result.error("setReverbAdvancedParam_Null_Param".toUpperCase(), "[setReverbAdvancedParam] Null param", null);
            return;
        }

        ZegoReverbAdvancedParam param = new ZegoReverbAdvancedParam();
        param.roomSize = floatValue((Number) paramMap.get("roomSize"));
        param.reverberance = floatValue((Number) paramMap.get("reverberance"));
        param.damping = floatValue((Number) paramMap.get("damping"));
        param.wetOnly = boolValue((Boolean) paramMap.get("wetOnly"));
        param.wetGain = floatValue((Number) paramMap.get("wetGain"));
        param.dryGain = floatValue((Number) paramMap.get("dryGain"));
        param.toneLow = floatValue((Number) paramMap.get("toneLow"));
        param.toneHigh = floatValue((Number) paramMap.get("toneHigh"));
        param.preDelay = floatValue((Number) paramMap.get("preDelay"));
        param.stereoWidth = floatValue((Number) paramMap.get("stereoWidth"));

        ZegoExpressEngine.getEngine().setReverbAdvancedParam(param);

        result.success(null);
    }

    @SuppressWarnings({"unused", "unchecked"})
    public static void setReverbEchoParam(MethodCall call, Result result) {

        HashMap<String, Object> paramMap = call.argument("param");
        if (paramMap == null || paramMap.isEmpty()) {
            result.error("setReverbEchoParam_Null_Param".toUpperCase(), "[setReverbEchoParam] Null param", null);
            return;
        }

        ZegoReverbEchoParam param = new ZegoReverbEchoParam();

        param.inGain = floatValue((Number) paramMap.get("inGain"));
        param.outGain = floatValue((Number) paramMap.get("outGain"));
        param.numDelays = intValue((Number) paramMap.get("numDelays"));
        param.delay = intArrayValue((ArrayList<Integer>) paramMap.get("delay"));
        param.decay = floatArrayValueFromDoubleArray((ArrayList<Double>) paramMap.get("decay"));

        ZegoExpressEngine.getEngine().setReverbEchoParam(param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableVirtualStereo(MethodCall call, Result result) {

        boolean enable = boolValue((Boolean) call.argument("enable"));
        int angle = intValue((Number) call.argument("angle"));

        ZegoExpressEngine.getEngine().enableVirtualStereo(enable, angle);

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
        if (toUserMapList == null) {
            result.error("sendCustomCommand_Null_toUserList".toUpperCase(), "[sendCustomCommand] Null toUserList", null);
            return;
        }

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

    /* CustomVideoCapture */

    @SuppressWarnings("unused")
    public static void enableCustomVideoCapture(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Double> configMap = call.argument("config");
        int channel = intValue((Number) call.argument("channel"));

        ZegoCustomVideoCaptureConfig config = new ZegoCustomVideoCaptureConfig();

        if (configMap != null && !configMap.isEmpty()) {
            int bufferType = intValue(configMap.get("bufferType"));
            config.bufferType = ZegoVideoBufferType.getZegoVideoBufferType(bufferType);
        } else {
            // If `config` is empty, set the default configuration (raw data for Android)
            config.bufferType = ZegoVideoBufferType.RAW_DATA;
        }

        ZegoExpressEngine.getEngine().setCustomVideoCaptureHandler(ZegoCustomVideoCaptureManager.getInstance());
        ZegoExpressEngine.getEngine().enableCustomVideoCapture(enable, config, ZegoPublishChannel.getZegoPublishChannel(channel));

        // When using custom video capture, turn off preview mirroring
        if (enable) {
            ZegoExpressEngine.getEngine().setVideoMirrorMode(ZegoVideoMirrorMode.NO_MIRROR, ZegoPublishChannel.getZegoPublishChannel(channel));
        } else {
            ZegoExpressEngine.getEngine().setVideoMirrorMode(ZegoVideoMirrorMode.ONLY_PREVIEW_MIRROR, ZegoPublishChannel.getZegoPublishChannel(channel));
        }

        result.success(null);
    }

    /* MediaPlayer */

    @SuppressWarnings("unused")
    public static void createMediaPlayer(MethodCall call, Result result) {

        ZegoMediaPlayer mediaPlayer = ZegoExpressEngine.getEngine().createMediaPlayer();

        if (mediaPlayer != null) {
            int index = mediaPlayer.getIndex();

            mediaPlayer.setEventHandler(ZegoExpressEngineEventHandler.getInstance().mediaPlayerEventHandler);
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
    public static void mediaPlayerSetPlayVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = intValue((Number) call.argument("volume"));
            mediaPlayer.setPlayVolume(volume);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPublishVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = intValue((Number) call.argument("volume"));
            mediaPlayer.setPublishVolume(volume);
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
    public static void mediaPlayerGetPlayVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = mediaPlayer.getPlayVolume();
            result.success(volume);
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetPublishVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = mediaPlayer.getPublishVolume();
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
    public static void mediaPlayerGetAudioTrackCount(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int audioTrackCount = mediaPlayer.getAudioTrackCount();
            result.success(audioTrackCount);
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetAudioTrackIndex(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int trackIndex = intValue((Number) call.argument("trackIndex"));
            mediaPlayer.setAudioTrackIndex(trackIndex);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetVoiceChangerParam(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            HashMap<String, Double> paramMap = call.argument("param");
            if (paramMap == null || paramMap.isEmpty()) {
                result.error("mediaPlayer_SetVoiceChangerParam_Null_Param".toUpperCase(), "[mediaPlayerSetVoiceChangerParam] Null param", null);
                return;
            }

            ZegoMediaPlayerAudioChannel audioChannel = ZegoMediaPlayerAudioChannel.getZegoMediaPlayerAudioChannel(intValue((Number) call.argument("audioChannel")));

            ZegoVoiceChangerParam param = new ZegoVoiceChangerParam();
            param.pitch = floatValue(paramMap.get("pitch"));

            mediaPlayer.setVoiceChangerParam(audioChannel, param);
        }

        result.success(null);
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


    /* AudioEffectPlayer */

    @SuppressWarnings("unused")
    public static void createAudioEffectPlayer(MethodCall call, Result result) {

        ZegoAudioEffectPlayer audioEffectPlayer = ZegoExpressEngine.getEngine().createAudioEffectPlayer();

        if (audioEffectPlayer != null) {
            int index = audioEffectPlayer.getIndex();

            audioEffectPlayer.setEventHandler(ZegoExpressEngineEventHandler.getInstance().audioEffectPlayerEventHandler);
            audioEffectPlayerHashMap.put(index, audioEffectPlayer);

            result.success(index);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyAudioEffectPlayer(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            audioEffectPlayer.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyAudioEffectPlayer(audioEffectPlayer);
            audioEffectPlayerHashMap.remove(index);

            result.success(null);

        } else {
            result.error("destroyAudioEffectPlayer_Can_not_find_player".toUpperCase(), "Invoke `destroyAudioEffectPlayer` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerStart(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            String path = call.argument("path");

            HashMap<String, Object> configMap = call.argument("config");

            ZegoAudioEffectPlayConfig config = null;
            if (configMap != null && !configMap.isEmpty()) {
                config = new ZegoAudioEffectPlayConfig();
                config.playCount = intValue((Number) configMap.get("playCount"));
                config.isPublishOut = boolValue((Boolean) configMap.get("isPublishOut"));
            }

            audioEffectPlayer.start(audioEffectID, path, config);

            result.success(null);

        } else {
            result.error("audioEffectPlayerStart_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerStart` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerStop(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            audioEffectPlayer.stop(audioEffectID);

            result.success(null);

        } else {
            result.error("audioEffectPlayerStop_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerStop` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerPause(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            audioEffectPlayer.pause(audioEffectID);

            result.success(null);

        } else {
            result.error("audioEffectPlayerPause_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerPause` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerResume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            audioEffectPlayer.resume(audioEffectID);

            result.success(null);

        } else {
            result.error("audioEffectPlayerResume_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerResume` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerStopAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            audioEffectPlayer.stopAll();

            result.success(null);

        } else {
            result.error("audioEffectPlayerStopAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerStopAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerPauseAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            audioEffectPlayer.pauseAll();

            result.success(null);

        } else {
            result.error("audioEffectPlayerPauseAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerPauseAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerResumeAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            audioEffectPlayer.resumeAll();

            result.success(null);

        } else {
            result.error("audioEffectPlayerResumeAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerResumeAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSeekTo(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            long millisecond = longValue((Number) call.argument("millisecond"));
            audioEffectPlayer.seekTo(audioEffectID, millisecond, new IZegoAudioEffectPlayerSeekToCallback() {
                @Override
                public void onSeekToCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("audioEffectPlayerSeekTo_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSeekTo` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            int volume = intValue((Number) call.argument("volume"));
            audioEffectPlayer.setVolume(audioEffectID, volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetVolume_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetVolume` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetVolumeAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int volume = intValue((Number) call.argument("volume"));
            audioEffectPlayer.setVolumeAll(volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetVolumeAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerGetTotalDuration(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            long totalDuration = audioEffectPlayer.getTotalDuration(audioEffectID);
            result.success(totalDuration);

        } else {
            result.error("audioEffectPlayerGetTotalDuration_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerGetTotalDuration` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerGetCurrentProgress(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            long currentProgress = audioEffectPlayer.getCurrentProgress(audioEffectID);
            result.success(currentProgress);

        } else {
            result.error("audioEffectPlayerGetCurrentProgress_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerGetCurrentProgress` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerLoadResource(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            String path = call.argument("path");
            audioEffectPlayer.loadResource(audioEffectID, path, new IZegoAudioEffectPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("audioEffectPlayerLoadResource_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerLoadResource` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerUnloadResource(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = intValue((Number) call.argument("audioEffectID"));
            audioEffectPlayer.unloadResource(audioEffectID);

            result.success(null);

        } else {
            result.error("audioEffectPlayerUnloadResource_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerUnloadResource` but can't find specific player", null);
        }
    }


    /* Record */

    @SuppressWarnings("unused")
    public static void startRecordingCapturedData(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) {
            result.error("startRecordingCapturedData_Null_Config".toUpperCase(), "[startRecordingCapturedData] Null config", null);
            return;
        }

        ZegoDataRecordConfig config = new ZegoDataRecordConfig();
        config.filePath = (String) configMap.get("filePath");
        config.recordType = ZegoDataRecordType.getZegoDataRecordType(intValue((Number) configMap.get("recordType")));

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().startRecordingCapturedData(config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopRecordingCapturedData(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopRecordingCapturedData(channel);

        result.success(null);
    }


    /*  Utilities */

    @SuppressWarnings("unused")
    public static void startPerformanceMonitor(MethodCall call, Result result) {

        int millisecond = intValue((Number) call.argument("millisecond"));

        ZegoExpressEngine.getEngine().startPerformanceMonitor(millisecond);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopPerformanceMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopPerformanceMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startNetworkSpeedTest(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) {
            result.error("startNetworkSpeedTest_Null_Config".toUpperCase(), "[startNetworkSpeedTest] Null config", null);
            return;
        }

        ZegoNetworkSpeedTestConfig config = new ZegoNetworkSpeedTestConfig();
        config.testUplink = boolValue((Boolean) configMap.get("testUplink"));
        config.expectedUplinkBitrate = intValue((Number) call.argument("expectedUplinkBitrate"));
        config.testDownlink = boolValue((Boolean) configMap.get("testDownlink"));
        config.expectedDownlinkBitrate = intValue((Number) call.argument("expectedDownlinkBitrate"));

        ZegoExpressEngine.getEngine().startNetworkSpeedTest(config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopNetworkSpeedTest(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopNetworkSpeedTest();

        result.success(null);
    }


    /* PlatformView Utils */

    @SuppressWarnings("unused")
    public static void destroyPlatformView(MethodCall call, Result result) {

        int viewID = intValue((Number) call.argument("viewID"));

        Boolean state = ZegoPlatformViewFactory.getInstance().destroyPlatformView(viewID);

        ZegoLog.log("[destroyPlatformView][Result] viewID: %d, success: %s", viewID, state ? "true" : "false");

        result.success(state);
    }


    /* TextureRenderer Utils */

    @SuppressWarnings("unused")
    public static void createTextureRenderer(MethodCall call, Result result) {

        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        Long textureID = ZegoTextureRendererController.getInstance().createTextureRenderer(textureRegistry.createSurfaceTexture(), width, height);

        ZegoLog.log("[createTextureRenderer][Result] w: %d, h: %d, textureID: %d", width, height, textureID);

        result.success(textureID);
    }

    @SuppressWarnings("unused")
    public static void updateTextureRendererSize(MethodCall call, Result result) {

        Long textureID = longValue((Number) call.argument("textureID"));
        int width = intValue((Number) call.argument("width"));
        int height = intValue((Number) call.argument("height"));

        Boolean state = ZegoTextureRendererController.getInstance().updateTextureRendererSize(textureID, width, height);

        ZegoLog.log("[updateTextureRendererSize][Result] w: %d, h: %d, textureID: %d, success: %s", width, height, textureID, state ? "true" : "false");

        result.success(state);
    }

    @SuppressWarnings("unused")
    public static void destroyTextureRenderer(MethodCall call, Result result) {

        Long textureID = longValue((Number) call.argument("textureID"));

        Boolean state = ZegoTextureRendererController.getInstance().destroyTextureRenderer(textureID);

        ZegoLog.log("[destroyTextureRenderer][Result] textureID: %d, success: %s", textureID, state ? "true" : "false");

        result.success(state);
    }


    /* Assets Utils */

    @SuppressWarnings("unused")
    public static void getAssetAbsolutePath(MethodCall call, Result result) {

        String assetPath = call.argument("assetPath");
        if (assetPath == null) {
            result.success("");
            return;
        }

        String assetKey;
        if (pluginBinding != null) {
            assetKey = pluginBinding.getFlutterAssets().getAssetFilePathByName(assetPath);
        } else {
            assetKey = registrar.lookupKeyForAsset(assetPath);
        }
        String realPath = application.getFilesDir().getAbsolutePath() + File.separator + assetKey;

        ZegoLog.log("[getAssetAbsolutePath] assetPath: %s, realPath: %s", assetPath, realPath);

        result.success(realPath);
    }


    /* Private functions */

    private static void setPlatformLanguage() {
        try {
            Class<?> jniClass = Class.forName("im.zego.zegoexpress.internal.ZegoExpressEngineInternalImpl");
            Method jniMethod = jniClass.getDeclaredMethod("setPlatformLanguage", int.class);
            jniMethod.setAccessible(true);
            jniMethod.invoke(null, 4);
        } catch (ClassNotFoundException e) {
            Log.e("ZEGO", "[Flutter] Set platform language failed, class ZegoExpressEngineInternalImpl not found.");
        } catch (NoSuchMethodException e) {
            Log.e("ZEGO", "[Flutter] Set platform language failed, method setPlatformLanguage not found.");
        } catch (IllegalAccessException e) {
            Log.e("ZEGO", "[Flutter] Set platform language failed, illegal access.");
        } catch (InvocationTargetException e) {
            Log.e("ZEGO", "[Flutter] Set platform language failed, invocation failed.");
        }
    }
}
