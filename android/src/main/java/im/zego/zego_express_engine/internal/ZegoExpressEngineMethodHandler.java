//
//  android
//  im.zego.zego_express_engine
//
//  Created by Patrick Fu on 2020-03-31.
//  Copyright © 2020 Zego. All rights reserved.
//

package im.zego.zego_express_engine.internal;

import static im.zego.zego_express_engine.internal.ZegoUtils.getStackTrace;

import android.app.Application;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.SurfaceView;

import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.lang.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;

import im.zego.zego_express_engine.ZegoCustomVideoCaptureManager;
import im.zego.zego_express_engine.ZegoCustomVideoProcessManager;
import im.zego.zego_express_engine.ZegoCustomVideoRenderManager;
import im.zego.zego_express_engine.ZegoMediaPlayerAudioManager;
import im.zego.zego_express_engine.ZegoMediaPlayerBlockDataManager;
import im.zego.zego_express_engine.ZegoMediaPlayerVideoManager;
import im.zego.zegoexpress.ZegoAIVoiceChanger;
import im.zego.zegoexpress.ZegoAudioEffectPlayer;
import im.zego.zegoexpress.ZegoCopyrightedMusic;
import im.zego.zegoexpress.ZegoExpressEngine;
import im.zego.zegoexpress.ZegoMediaDataPublisher;
import im.zego.zegoexpress.ZegoMediaPlayer;
import im.zego.zegoexpress.ZegoRangeAudio;
import im.zego.zegoexpress.ZegoRealTimeSequentialDataManager;
import im.zego.zegoexpress.callback.IZegoAudioEffectPlayerLoadResourceCallback;
import im.zego.zegoexpress.callback.IZegoAudioEffectPlayerSeekToCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicDownloadCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicGetKrcLyricByTokenCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicGetLrcLyricCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicGetMusicByTokenCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicGetSharedResourceCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicGetStandardPitchCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicInitCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicRequestAccompanimentCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicRequestAccompanimentClipCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicRequestResourceCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicRequestSongCallback;
import im.zego.zegoexpress.callback.IZegoCopyrightedMusicSendExtendedRequestCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBarrageMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendBroadcastMessageCallback;
import im.zego.zegoexpress.callback.IZegoIMSendCustomCommandCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerLoadResourceCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerSeekToCallback;
import im.zego.zegoexpress.callback.IZegoMediaPlayerTakeSnapshotCallback;
import im.zego.zegoexpress.callback.IZegoMixerStartCallback;
import im.zego.zegoexpress.callback.IZegoMixerStopCallback;
import im.zego.zegoexpress.callback.IZegoNetworkProbeResultCallback;
import im.zego.zegoexpress.callback.IZegoPlayerTakeSnapshotCallback;
import im.zego.zegoexpress.callback.IZegoPublisherSetStreamExtraInfoCallback;
import im.zego.zegoexpress.callback.IZegoPublisherTakeSnapshotCallback;
import im.zego.zegoexpress.callback.IZegoPublisherUpdateCdnUrlCallback;
import im.zego.zegoexpress.callback.IZegoRoomLoginCallback;
import im.zego.zegoexpress.callback.IZegoRoomLogoutCallback;
import im.zego.zegoexpress.callback.IZegoRoomSetRoomExtraInfoCallback;
import im.zego.zegoexpress.callback.IZegoRealTimeSequentialDataSentCallback;
import im.zego.zegoexpress.callback.IZegoRoomSendTransparentMessageCallback;
import im.zego.zegoexpress.constants.ZegoAECMode;
import im.zego.zegoexpress.constants.ZegoANSMode;
import im.zego.zegoexpress.constants.ZegoAudioCaptureStereoMode;
import im.zego.zegoexpress.constants.ZegoAudioChannel;
import im.zego.zegoexpress.constants.ZegoAudioCodecID;
import im.zego.zegoexpress.constants.ZegoAudioMixMode;
import im.zego.zegoexpress.constants.ZegoAudioRoute;
import im.zego.zegoexpress.constants.ZegoAudioSampleRate;
import im.zego.zegoexpress.constants.ZegoAudioSourceType;
import im.zego.zegoexpress.constants.ZegoBackgroundBlurLevel;
import im.zego.zegoexpress.constants.ZegoBackgroundProcessType;
import im.zego.zegoexpress.constants.ZegoCapturePipelineScaleMode;
import im.zego.zegoexpress.constants.ZegoCopyrightedMusicBillingMode;
import im.zego.zegoexpress.constants.ZegoCopyrightedMusicResourceQualityType;
import im.zego.zegoexpress.constants.ZegoCopyrightedMusicResourceType;
import im.zego.zegoexpress.constants.ZegoCopyrightedMusicType;
import im.zego.zegoexpress.constants.ZegoDataRecordType;
import im.zego.zegoexpress.constants.ZegoDumpDataType;
import im.zego.zegoexpress.constants.ZegoElectronicEffectsMode;
import im.zego.zegoexpress.constants.ZegoFontType;
import im.zego.zegoexpress.constants.ZegoGeoFenceType;
import im.zego.zegoexpress.constants.ZegoHttpDNSType;
import im.zego.zegoexpress.constants.ZegoFeatureType;
import im.zego.zegoexpress.constants.ZegoLiveAudioEffectMode;
import im.zego.zegoexpress.constants.ZegoMediaDataPublisherMode;
import im.zego.zegoexpress.constants.ZegoMediaPlayerAudioChannel;
import im.zego.zegoexpress.constants.ZegoMediaPlayerAudioTrackMode;
import im.zego.zegoexpress.constants.ZegoMediaPlayerState;
import im.zego.zegoexpress.constants.ZegoMediaStreamType;
import im.zego.zegoexpress.constants.ZegoMixRenderMode;
import im.zego.zegoexpress.constants.ZegoMixerInputContentType;
import im.zego.zegoexpress.constants.ZegoObjectSegmentationType;
import im.zego.zegoexpress.constants.ZegoOrientation;
import im.zego.zegoexpress.constants.ZegoPublishChannel;
import im.zego.zegoexpress.constants.ZegoRangeAudioMode;
import im.zego.zegoexpress.constants.ZegoResourceType;
import im.zego.zegoexpress.constants.ZegoReverbPreset;
import im.zego.zegoexpress.constants.ZegoRoomMode;
import im.zego.zegoexpress.constants.ZegoSEIType;
import im.zego.zegoexpress.constants.ZegoScenario;
import im.zego.zegoexpress.constants.ZegoStreamResourceMode;
import im.zego.zegoexpress.constants.ZegoStreamResourceSwitchMode;
import im.zego.zegoexpress.constants.ZegoStreamResourceType;
import im.zego.zegoexpress.constants.ZegoTrafficControlFocusOnMode;
import im.zego.zegoexpress.constants.ZegoTrafficControlMinVideoBitrateMode;
import im.zego.zegoexpress.constants.ZegoVideoBufferType;
import im.zego.zegoexpress.constants.ZegoVideoCodecID;
import im.zego.zegoexpress.constants.ZegoVideoCodecBackend;
import im.zego.zegoexpress.constants.ZegoOrientationMode;
import im.zego.zegoexpress.constants.ZegoVideoFrameFormat;
import im.zego.zegoexpress.constants.ZegoVideoFrameFormatSeries;
import im.zego.zegoexpress.constants.ZegoVideoMirrorMode;
import im.zego.zegoexpress.constants.ZegoVideoRateControlMode;
import im.zego.zegoexpress.constants.ZegoViewMode;
import im.zego.zegoexpress.constants.ZegoVideoStreamType;
import im.zego.zegoexpress.constants.ZegoVoiceChangerPreset;
import im.zego.zegoexpress.constants.ZegoStreamAlignmentMode;
import im.zego.zegoexpress.constants.ZegoCameraFocusMode;
import im.zego.zegoexpress.constants.ZegoCameraExposureMode;
import im.zego.zegoexpress.constants.ZegoAudioVADStableStateMonitorType;
import im.zego.zegoexpress.constants.ZegoEncodeProfile;
import im.zego.zegoexpress.constants.ZegoStreamCensorshipMode;
import im.zego.zegoexpress.constants.ZegoLowlightEnhancementMode;
import im.zego.zegoexpress.constants.ZegoVideoSourceType;
import im.zego.zegoexpress.constants.ZegoAudioDeviceMode;
import im.zego.zegoexpress.constants.ZegoCopyrightedMusicVendorID;
import im.zego.zegoexpress.constants.ZegoMultimediaLoadType;
import im.zego.zegoexpress.constants.ZegoAlphaLayoutType;
import im.zego.zegoexpress.constants.ZegoRangeAudioSpeakMode;
import im.zego.zegoexpress.constants.ZegoRangeAudioListenMode;
import im.zego.zegoexpress.constants.ZegoRoomTransparentMessageMode;
import im.zego.zegoexpress.constants.ZegoRoomTransparentMessageType;
import im.zego.zegoexpress.constants.ZegoRoomStreamListType;
import im.zego.zegoexpress.entity.ZegoAccurateSeekConfig;
import im.zego.zegoexpress.entity.ZegoAudioConfig;
import im.zego.zegoexpress.entity.ZegoAudioEffectPlayConfig;
import im.zego.zegoexpress.entity.ZegoAudioFrameParam;
import im.zego.zegoexpress.entity.ZegoAudioSourceMixConfig;
import im.zego.zegoexpress.entity.ZegoAutoMixerTask;
import im.zego.zegoexpress.entity.ZegoBackgroundConfig;
import im.zego.zegoexpress.entity.ZegoBeautifyOption;
import im.zego.zegoexpress.entity.ZegoCDNConfig;
import im.zego.zegoexpress.entity.ZegoCanvas;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicConfig;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicGetLyricConfig;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicQueryCacheConfig;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicQueryCacheConfigV2;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicRequestConfig;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicRequestConfigV2;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicGetSharedConfig;
import im.zego.zegoexpress.entity.ZegoCopyrightedMusicGetSharedConfigV2;
import im.zego.zegoexpress.entity.ZegoCrossAppInfo;
import im.zego.zegoexpress.entity.ZegoCustomAudioConfig;
import im.zego.zegoexpress.entity.ZegoCustomAudioProcessConfig;
import im.zego.zegoexpress.entity.ZegoCustomVideoCaptureConfig;
import im.zego.zegoexpress.entity.ZegoCustomVideoProcessConfig;
import im.zego.zegoexpress.entity.ZegoCustomVideoRenderConfig;
import im.zego.zegoexpress.entity.ZegoDataRecordConfig;
import im.zego.zegoexpress.entity.ZegoDumpDataConfig;
import im.zego.zegoexpress.entity.ZegoEngineConfig;
import im.zego.zegoexpress.entity.ZegoEngineProfile;
import im.zego.zegoexpress.entity.ZegoFontStyle;
import im.zego.zegoexpress.entity.ZegoLabelInfo;
import im.zego.zegoexpress.entity.ZegoLogConfig;
import im.zego.zegoexpress.entity.ZegoMediaDataPublisherConfig;
import im.zego.zegoexpress.entity.ZegoMediaPlayerMediaInfo;
import im.zego.zegoexpress.entity.ZegoMediaPlayerStatisticsInfo;
import im.zego.zegoexpress.entity.ZegoMixerAudioConfig;
import im.zego.zegoexpress.entity.ZegoMixerInput;
import im.zego.zegoexpress.entity.ZegoMixerOutput;
import im.zego.zegoexpress.entity.ZegoMixerOutputVideoConfig;
import im.zego.zegoexpress.entity.ZegoMixerTask;
import im.zego.zegoexpress.entity.ZegoMixerVideoConfig;
import im.zego.zegoexpress.entity.ZegoNetWorkResourceCache;
import im.zego.zegoexpress.entity.ZegoNetworkProbeConfig;
import im.zego.zegoexpress.entity.ZegoNetworkProbeResult;
import im.zego.zegoexpress.entity.ZegoNetworkSpeedTestConfig;
import im.zego.zegoexpress.entity.ZegoNetworkTimeInfo;
import im.zego.zegoexpress.entity.ZegoObjectSegmentationConfig;
import im.zego.zegoexpress.entity.ZegoPlayerConfig;
import im.zego.zegoexpress.entity.ZegoProxyInfo;
import im.zego.zegoexpress.entity.ZegoPublishDualStreamConfig;
import im.zego.zegoexpress.entity.ZegoPublisherConfig;
import im.zego.zegoexpress.entity.ZegoReceiveRangeParam;
import im.zego.zegoexpress.entity.ZegoReverbAdvancedParam;
import im.zego.zegoexpress.entity.ZegoReverbEchoParam;
import im.zego.zegoexpress.entity.ZegoRoomConfig;
import im.zego.zegoexpress.entity.ZegoSEIConfig;
import im.zego.zegoexpress.entity.ZegoScreenCaptureConfig;
import im.zego.zegoexpress.entity.ZegoSoundLevelConfig;
import im.zego.zegoexpress.entity.ZegoUser;
import im.zego.zegoexpress.entity.ZegoVideoConfig;
import im.zego.zegoexpress.entity.ZegoVocalRangeParam;
import im.zego.zegoexpress.entity.ZegoVoiceChangerParam;
import im.zego.zegoexpress.entity.ZegoWatermark;
import im.zego.zegoexpress.entity.ZegoMixerWhiteboard;
import im.zego.zegoexpress.entity.ZegoMediaPlayerResource;
import im.zego.zegoexpress.entity.ZegoEffectsBeautyParam;
import im.zego.zegoexpress.entity.ZegoMixerImageInfo;
import im.zego.zegoexpress.entity.ZegoColorEnhancementParams;
import im.zego.zegoexpress.entity.ZegoRoomSendTransparentMessage;
import im.zego.zegoexpress.entity.ZegoRoomStreamList;
import im.zego.zegoexpress.entity.ZegoStream;

import io.flutter.embedding.engine.plugins.FlutterPlugin.FlutterPluginBinding;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel.Result;
import io.flutter.plugin.common.PluginRegistry.Registrar;
import io.flutter.view.TextureRegistry;


public class ZegoExpressEngineMethodHandler {

    private static Registrar registrar = null;

    private static FlutterPluginBinding pluginBinding = null;

    private static Application application = null;

    private static TextureRegistry textureRegistry = null;

    private static boolean enablePlatformView = false;

    private static boolean pluginReported = false;

    private static final HashMap<Integer, ZegoMediaPlayer> mediaPlayerHashMap = new HashMap<>();

    private static final HashMap<Integer, ZegoAudioEffectPlayer> audioEffectPlayerHashMap = new HashMap<>();

    private static final HashMap<Integer, ZegoMediaDataPublisher> mediaDataPublisherHashMap = new HashMap<>();

    private static final HashMap<Integer, ZegoRealTimeSequentialDataManager> realTimeSequentialDataManagerHashMap = new HashMap<>();


    private static final HashMap<Integer, ZegoAIVoiceChanger> aiVoiceChangerHashMap = new HashMap<>();

    private static ZegoRangeAudio rangeAudioInstance = null;

    private static ZegoCopyrightedMusic copyrightedMusicInstance = null;

    @SuppressWarnings("unused")
    public static void initApiCalledCallback() {
        ZegoExpressEngine.setApiCalledCallback(ZegoExpressEngineEventHandler.getInstance().apiCalledEventHandler);
    }

    @SuppressWarnings("unused")
    public static void createEngineWithProfile(MethodCall call, Result result, Registrar reg, FlutterPluginBinding binding, EventChannel.EventSink sink) {

        // Report framework info
        reportPluginInfo();
        
        HashMap<String, Object> profileMap = call.argument("profile");
        long appID = ZegoUtils.longValue((Number)profileMap.get("appID"));
        String appSign = (String) profileMap.get("appSign");
        ZegoScenario scenario = ZegoScenario.getZegoScenario(ZegoUtils.intValue((Number)profileMap.get("scenario")));

        enablePlatformView = ZegoUtils.boolValue((Boolean) profileMap.get("enablePlatformView"));

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

        ZegoEngineProfile profile = new ZegoEngineProfile();
        profile.appID = appID;
        if (appSign != null) {
            profile.appSign = appSign;
        }
        profile.scenario = scenario;
        profile.application = application;
        ZegoExpressEngine.createEngine(profile, ZegoExpressEngineEventHandler.getInstance().eventHandler);
        setPlatformLanguage();

        ZegoExpressEngine.getEngine().setDataRecordEventHandler(ZegoExpressEngineEventHandler.getInstance().dataRecordEventHandler);
        ZegoExpressEngine.getEngine().setAudioDataHandler(ZegoExpressEngineEventHandler.getInstance().audioDataHandler);
        ZegoExpressEngine.getEngine().setCustomAudioProcessHandler(ZegoExpressEngineEventHandler.getInstance().customAudioProcessHandler);

        ZegoLog.log("[createEngine] platform:Android, enablePlatformView:%s, sink: %d, appID:%d, scenario:%s", enablePlatformView ? "true" : "false", sink!=null ? sink.hashCode() : -1, appID, scenario.name());

        result.success(null);
    }

    /* Main */
    @SuppressWarnings("unused")
    public static void createEngine(MethodCall call, Result result, Registrar reg, FlutterPluginBinding binding, EventChannel.EventSink sink) {

        // Report framework info
        reportPluginInfo();
        
        long appID = ZegoUtils.longValue((Number)call.argument("appID"));
        String appSign = call.argument("appSign");
        boolean isTestEnv = ZegoUtils.boolValue((Boolean) call.argument("isTestEnv"));
        ZegoScenario scenario = ZegoScenario.getZegoScenario(ZegoUtils.intValue((Number)call.argument("scenario")));

        enablePlatformView = ZegoUtils.boolValue((Boolean) call.argument("enablePlatformView"));

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
        ZegoExpressEngine.getEngine().setAudioDataHandler(ZegoExpressEngineEventHandler.getInstance().audioDataHandler);
        ZegoExpressEngine.getEngine().setCustomAudioProcessHandler(ZegoExpressEngineEventHandler.getInstance().customAudioProcessHandler);
        
        ZegoLog.log("[createEngine] platform:Android, enablePlatformView:%s, sink: %d, appID:%d, isTestEnv:%s, scenario:%s", enablePlatformView ? "true" : "false", sink!=null ? sink.hashCode() : -1, appID, isTestEnv ? "true" : "false", scenario.name());

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void destroyEngine(MethodCall call, Result result) {

        ZegoExpressEngine.destroyEngine(null);

        result.success(null);
    }

    @SuppressWarnings({"unused", "unchecked"})
    public static void setEngineConfig(MethodCall call, Result result) {

        // Report framework info
        reportPluginInfo();

        HashMap<String, Object> configMap = call.argument("config");
        ZegoEngineConfig configObject;
        if (configMap != null && !configMap.isEmpty()) {
            configObject = new ZegoEngineConfig();
            configObject.advancedConfig = (HashMap<String, String>) configMap.get("advancedConfig");

            HashMap<String, Object> logConfigMap = (HashMap<String, Object>) configMap.get("logConfig");
            ZegoLogConfig logConfigObject;
            if (logConfigMap != null && !logConfigMap.isEmpty()) {
                logConfigObject = new ZegoLogConfig();
                logConfigObject.logPath = (String) logConfigMap.get("logPath");
                logConfigObject.logSize  = ZegoUtils.intValue((Number) logConfigMap.get("logSize"));
                logConfigObject.logCount = ZegoUtils.intValue((Number) logConfigMap.get("logCount"));

                configObject.logConfig = logConfigObject;
            }

            ZegoExpressEngine.setEngineConfig(configObject);

            result.success(null);
        } else {
            result.error("setEngineConfig_null_config".toUpperCase(), "Invoke `setEngineConfig` with null config", null);
        }
    }

    @SuppressWarnings("unused")
    public static void setLogConfig(MethodCall call, Result result) { 
        HashMap<String, Object> configMap = call.argument("config");
        ZegoLogConfig logConfigObject;
        if (configMap != null && !configMap.isEmpty()) {
            logConfigObject = new ZegoLogConfig();
            logConfigObject.logPath = (String) configMap.get("logPath");
            logConfigObject.logSize  = ZegoUtils.intValue((Number) configMap.get("logSize"));
            logConfigObject.logCount = ZegoUtils.intValue((Number) configMap.get("logCount"));

            ZegoExpressEngine.setLogConfig(logConfigObject);
            result.success(null);
        } else {
            result.error("setLogConfig_null_config".toUpperCase(), "Invoke `setLogConfig` with null config", null);
        }
    }

    @SuppressWarnings("unused")
    public static void setLocalProxyConfig(MethodCall call, Result result) { 
        ArrayList<HashMap<String, Object>> proxyArray = call.argument("proxyList");

        ArrayList<ZegoProxyInfo> proxyList = new ArrayList<>();

        for (HashMap<String, Object> proxyMap : proxyArray) {
            ZegoProxyInfo proxy = new ZegoProxyInfo();
            proxy.ip = (String) proxyMap.get("ip");
            proxy.port = (int) proxyMap.get("port");
            proxy.hostName = (String) proxyMap.get("hostName");
            proxy.userName = (String) proxyMap.get("userName");
            proxy.password = (String) proxyMap.get("password");

            proxyList.add(proxy);
        }
        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoExpressEngine.setLocalProxyConfig(proxyList, enable);
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCloudProxyConfig(MethodCall call, Result result) {
        ArrayList<HashMap<String, Object>> proxyArray = call.argument("proxyList");

        ArrayList<ZegoProxyInfo> proxyList = new ArrayList<>();

        for (HashMap<String, Object> proxyMap : proxyArray) {
            ZegoProxyInfo proxy = new ZegoProxyInfo();
            proxy.ip = (String) proxyMap.get("ip");
            proxy.port = (int) proxyMap.get("port");
            proxy.hostName = (String) proxyMap.get("hostName");
            proxy.userName = (String) proxyMap.get("userName");
            proxy.password = (String) proxyMap.get("password");

            proxyList.add(proxy);
        }
        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        String token = call.argument("token");

        ZegoExpressEngine.setCloudProxyConfig(proxyList, token, enable);
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setRoomMode(MethodCall call, Result result) {

        ZegoRoomMode mode = ZegoRoomMode.getZegoRoomMode(ZegoUtils.intValue((Number)call.argument("mode")));

        ZegoExpressEngine.setRoomMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setLicense(MethodCall call, Result result) {

        String license = call.argument("license");

        ZegoExpressEngine.setLicense(license);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setGeoFence(MethodCall call, Result result) {

        ZegoGeoFenceType type = ZegoGeoFenceType.getZegoGeoFenceType(ZegoUtils.intValue((Number)call.argument("type")));
        ArrayList<Integer> areaList = (ArrayList<Integer>)call.argument("areaList");
        ZegoExpressEngine.setGeoFence(type, areaList);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getVersion(MethodCall call, Result result) {

        result.success(ZegoExpressEngine.getVersion());
    }

    @SuppressWarnings("unused")
    public static void isFeatureSupported(MethodCall call, Result result) {

        ZegoFeatureType featureType = ZegoFeatureType.getZegoFeatureType(ZegoUtils.intValue((Number)call.argument("featureType")));
        result.success(ZegoExpressEngine.isFeatureSupported(featureType));
    }

    @SuppressWarnings("unused")
    public static void setRoomScenario(MethodCall call, Result result) {

        ZegoScenario scenario = ZegoScenario.getZegoScenario(ZegoUtils.intValue((Number)call.argument("scenario")));
        ZegoExpressEngine.getEngine().setRoomScenario(scenario);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void uploadLog(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().uploadLog();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void submitLog(MethodCall call, Result result) {

        ZegoExpressEngine.submitLog();

        result.success(null);
    }

    public static void enableDebugAssistant(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableDebugAssistant(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void callExperimentalAPI(MethodCall call, Result result) {

        String params = call.argument("params");

        String callResult = ZegoExpressEngine.getEngine().callExperimentalAPI(params);

        result.success(callResult);
    }

    @SuppressWarnings("unused")
    public static void setDummyCaptureImagePath(MethodCall call, Result result) {

        String filePath = call.argument("filePath");
        if (filePath != null && filePath.startsWith("flutter-asset://")) {
            String processedURL = filePath.replace("flutter-asset://", "asset:flutter_assets/");
            ZegoLog.log("[setDummyCaptureImagePath] Flutter asset prefix detected, origin URL: '%s', processed URL: '%s'", filePath, processedURL);
            filePath = processedURL;
        }
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setDummyCaptureImagePath(filePath, channel);

        result.success(null);
    }

    /* Room */

    @SuppressWarnings("unused")
    public static void loginRoom(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");

        HashMap<String, Object> userMap = call.argument("user");
        ZegoUser user = new ZegoUser((String) (userMap != null ? userMap.get("userID") : null), (String) (userMap != null ? userMap.get("userName") : null));

        HashMap<String, Object> configMap = call.argument("config");
        ZegoRoomConfig roomConfig = new ZegoRoomConfig();
        if (configMap != null && !configMap.isEmpty()) {
            
            roomConfig.isUserStatusNotify = ZegoUtils.boolValue((Boolean)configMap.get("isUserStatusNotify"));
            roomConfig.maxMemberCount = ZegoUtils.intValue((Number)configMap.get("maxMemberCount"));
            roomConfig.token = (String)configMap.get("token");
            
        }

        ZegoExpressEngine.getEngine().loginRoom(roomID, user, roomConfig, new IZegoRoomLoginCallback() {
            @Override
            public void onRoomLoginResult(int errorCode, JSONObject extendedData) {
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                resultMap.put("extendedData", extendedData.toString());
                result.success(resultMap);
            }
        });
    }

    @SuppressWarnings("unused")
    public static void logoutRoom(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");
        if (roomID != null) {
            ZegoExpressEngine.getEngine().logoutRoom(roomID, new IZegoRoomLogoutCallback() {
                @Override
                public void onRoomLogoutResult(int errorCode, JSONObject extendedData) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("extendedData", extendedData.toString());
                    result.success(resultMap);
                }
            });
        } else {
            ZegoExpressEngine.getEngine().logoutRoom(new IZegoRoomLogoutCallback() {
                @Override
                public void onRoomLogoutResult(int errorCode, JSONObject extendedData) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("extendedData", extendedData.toString());
                    result.success(resultMap);
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void switchRoom(MethodCall call, Result result) {

        String fromRoomID = call.argument("fromRoomID");
        String toRoomID = call.argument("toRoomID");

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap != null && !configMap.isEmpty()) {
            ZegoRoomConfig roomConfig = new ZegoRoomConfig();
            roomConfig.isUserStatusNotify = ZegoUtils.boolValue((Boolean)configMap.get("isUserStatusNotify"));
            roomConfig.maxMemberCount = ZegoUtils.intValue((Number)configMap.get("maxMemberCount"));
            roomConfig.token = (String)configMap.get("token");
            ZegoExpressEngine.getEngine().switchRoom(fromRoomID, toRoomID, roomConfig);
        } else {
            ZegoExpressEngine.getEngine().switchRoom(fromRoomID, toRoomID);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void renewToken(MethodCall call, Result result) {

        String roomID = call.argument("roomID");
        String token = call.argument("token");

        ZegoExpressEngine.getEngine().renewToken(roomID, token);

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

    @SuppressWarnings("unused")
    public static void getRoomStreamList(MethodCall call, final Result result) {

        String roomID = call.argument("roomID");
        int type = ZegoUtils.intValue((Number) call.argument("streamListType"));

        ZegoRoomStreamList roomList = ZegoExpressEngine.getEngine().getRoomStreamList(roomID,ZegoRoomStreamListType.getZegoRoomStreamListType(type));
        HashMap<String, Object> resultMap = new HashMap<>();
        ArrayList<HashMap<String, Object>> publishStreamArrayList = new ArrayList<HashMap<String, Object>>();
        ArrayList<HashMap<String, Object>> playStreamListArrayList = new ArrayList<HashMap<String, Object>>();
        if(null != roomList)
        {
            if(null != roomList.publishStreamList)
            {
                for(ZegoStream stream : roomList.publishStreamList)
                {
                    HashMap<String, Object> streamHashMap = new HashMap<>();

                    HashMap<String, Object> userHashMap = new HashMap<>();
                    userHashMap.put("userID", stream.user.userID);
                    userHashMap.put("userName", stream.user.userName);
                    streamHashMap.put("user", userHashMap);

                    streamHashMap.put("streamID", stream.streamID);
                    streamHashMap.put("extraInfo", stream.extraInfo);

                    publishStreamArrayList.add(streamHashMap);
                }
            }

            if(null != roomList.playStreamList)
            {
                for(ZegoStream stream : roomList.playStreamList)
                {
                    HashMap<String, Object> streamHashMap = new HashMap<>();

                    HashMap<String, Object> userHashMap = new HashMap<>();
                    userHashMap.put("userID", stream.user.userID);
                    userHashMap.put("userName", stream.user.userName);
                    streamHashMap.put("user", userHashMap);

                    streamHashMap.put("streamID", stream.streamID);
                    streamHashMap.put("extraInfo", stream.extraInfo);

                    playStreamListArrayList.add(streamHashMap);
                }
            }

        }

        resultMap.put("publishStreamList", publishStreamArrayList);
        resultMap.put("playStreamList", playStreamListArrayList);
        
        result.success(resultMap);
    }


    /* Publisher */

    @SuppressWarnings("unused")
    public static void startPublishingStream(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoPublisherConfig config = null;

        HashMap<String, Object> configMap = call.argument("config");

        if (configMap != null && !configMap.isEmpty()) {
            config = new ZegoPublisherConfig();
            config.roomID = (String) configMap.get("roomID");
            config.forceSynchronousNetworkTime = ZegoUtils.intValue((Number)configMap.get("forceSynchronousNetworkTime"));
            config.streamCensorshipMode = ZegoStreamCensorshipMode.getZegoStreamCensorshipMode(ZegoUtils.intValue((Number)configMap.get("streamCensorshipMode")));
        }

        if (config != null) {
            ZegoExpressEngine.getEngine().startPublishingStream(streamID, config, channel);
        } else {
            ZegoExpressEngine.getEngine().startPublishingStream(streamID, channel);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopPublishingStream(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopPublishingStream(channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setStreamExtraInfo(MethodCall call, final Result result) {

        String extraInfo = call.argument("extraInfo");

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

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

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        HashMap<String, Object> canvasMap = call.argument("canvas");

        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Preview video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = ZegoUtils.intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(ZegoUtils.intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = ZegoUtils.intValue((Number) canvasMap.get("backgroundColor"));
            boolean alphaBlend = ZegoUtils.boolValue((Boolean) canvasMap.get("alphaBlend"));

            Object view;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getSurfaceView();
                    if (alphaBlend) {
                        ((SurfaceView)view).getHolder().setFormat(PixelFormat.TRANSLUCENT);
                        ((SurfaceView)view).setZOrderOnTop(true);
                    }
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
                    // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createCanvasView` first and get the textureID", viewID);
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
                canvas.alphaBlend = alphaBlend;

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

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

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

        int captureWidth = ZegoUtils.intValue((Number) configMap.get("captureWidth"));
        int captureHeight = ZegoUtils.intValue((Number) configMap.get("captureHeight"));
        int encodeWidth = ZegoUtils.intValue((Number) configMap.get("encodeWidth"));
        int encodeHeight = ZegoUtils.intValue((Number) configMap.get("encodeHeight"));
        int fps = ZegoUtils.intValue((Number) configMap.get("fps"));
        int bitrate = ZegoUtils.intValue((Number) configMap.get("bitrate"));
        int codecID = ZegoUtils.intValue((Number) configMap.get("codecID"));

        ZegoVideoConfig config = new ZegoVideoConfig();
        config.setCaptureResolution(captureWidth, captureHeight);
        config.setEncodeResolution(encodeWidth, encodeHeight);
        config.setVideoFPS(fps);
        config.setVideoBitrate(bitrate);
        
        if (codecID == ZegoVideoCodecID.values().length - 1) {
            config.setCodecID(ZegoVideoCodecID.UNKNOWN);
        } else {
            config.setCodecID(ZegoVideoCodecID.getZegoVideoCodecID(codecID));
        }

        if (configMap.containsKey("keyFrameInterval") && configMap.get("keyFrameInterval") != null) {
            config.keyFrameInterval = ZegoUtils.intValue((Number) configMap.get("keyFrameInterval"));
        }

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoConfig(config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getVideoConfig(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

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
    public static void setPublishDualStreamConfig(MethodCall call, Result result) {

        ArrayList<HashMap<String, Object>> configListMap = call.argument("configList");
        ArrayList<ZegoPublishDualStreamConfig> configList = new ArrayList<>();
        for (HashMap<String, Object> configMap : configListMap) {
            ZegoVideoStreamType streamType = ZegoVideoStreamType.getZegoVideoStreamType(ZegoUtils.intValue((Number) configMap.get("streamType")));
            int width = ZegoUtils.intValue((Number) configMap.get("encodeWidth"));
            int height = ZegoUtils.intValue((Number) configMap.get("encodeHeight"));
            int fps = ZegoUtils.intValue((Number) configMap.get("fps"));
            int bitrate = ZegoUtils.intValue((Number) configMap.get("bitrate"));

            ZegoPublishDualStreamConfig config = new ZegoPublishDualStreamConfig();
            config.streamType = streamType;
            config.encodeWidth = width;
            config.encodeHeight = height;
            config.fps = fps;
            config.bitrate = bitrate;

            configList.add(config);
        }

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setPublishDualStreamConfig(configList, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVideoMirrorMode(MethodCall call, Result result) {

        ZegoVideoMirrorMode mirrorMode = ZegoVideoMirrorMode.getZegoVideoMirrorMode(ZegoUtils.intValue((Number) call.argument("mirrorMode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setVideoMirrorMode(mirrorMode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAppOrientation(MethodCall call, Result result) {

        ZegoOrientation orientation = ZegoOrientation.values()[ZegoUtils.intValue((Number) call.argument("orientation"))];
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setAppOrientation(orientation, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAudioConfig(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));
        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) {
            result.error("setAudioConfig_Null_Config".toUpperCase(), "[setAudioConfig] Null config", null);
            return;
        }

        int bitrate = ZegoUtils.intValue((Number) configMap.get("bitrate"));
        int audioChannel = ZegoUtils.intValue((Number) configMap.get("channel"));
        int codecID = ZegoUtils.intValue((Number) configMap.get("codecID"));

        ZegoAudioConfig config = new ZegoAudioConfig();
        config.bitrate = bitrate;
        config.channel = ZegoAudioChannel.getZegoAudioChannel(audioChannel);
        config.codecID = ZegoAudioCodecID.getZegoAudioCodecID(codecID);

        ZegoExpressEngine.getEngine().setAudioConfig(config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getAudioConfig(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoAudioConfig config = ZegoExpressEngine.getEngine().getAudioConfig(channel);

        HashMap<String, Object> resultMap = new HashMap<>();
        resultMap.put("bitrate", config.bitrate);
        resultMap.put("channel", config.channel.value());
        resultMap.put("codecID", config.codecID.value());
        result.success(resultMap);
    }

    @SuppressWarnings("unused")
    public static void setPublishStreamEncryptionKey(MethodCall call, Result result) {

        String key = call.argument("key");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setPublishStreamEncryptionKey(key, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void takePublishStreamSnapshot(MethodCall call, final Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

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

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamAudio(mute, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePublishStreamVideo(MethodCall call, Result result) {

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().mutePublishStreamVideo(mute, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setStreamAlignmentProperty(MethodCall call, Result result) {

        int alignment = ZegoUtils.intValue((Number) call.argument("alignment"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

         ZegoExpressEngine.getEngine().setStreamAlignmentProperty(alignment, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableTrafficControl(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int property = ZegoUtils.intValue((Number) call.argument("property"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableTrafficControl(enable, property, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setMinVideoBitrateForTrafficControl(MethodCall call, Result result) {

        int bitrate = ZegoUtils.intValue((Number) call.argument("bitrate"));
        ZegoTrafficControlMinVideoBitrateMode mode = ZegoTrafficControlMinVideoBitrateMode.getZegoTrafficControlMinVideoBitrateMode(ZegoUtils.intValue((Number) call.argument("mode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setMinVideoBitrateForTrafficControl(bitrate, mode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setMinVideoFpsForTrafficControl(MethodCall call, Result result) {

        int fps = ZegoUtils.intValue((Number) call.argument("fps"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setMinVideoFpsForTrafficControl(fps, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setMinVideoResolutionForTrafficControl(MethodCall call, Result result) {

        int width = ZegoUtils.intValue((Number) call.argument("width"));
        int height = ZegoUtils.intValue((Number) call.argument("height"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setMinVideoResolutionForTrafficControl(width, height, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setTrafficControlFocusOn(MethodCall call, Result result) {

        ZegoTrafficControlFocusOnMode mode = ZegoTrafficControlFocusOnMode.getZegoTrafficControlFocusOnMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setTrafficControlFocusOn(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCaptureVolume(MethodCall call, Result result) {

        int volume = ZegoUtils.intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setCaptureVolume(volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAudioCaptureStereoMode(MethodCall call, Result result) {

        ZegoAudioCaptureStereoMode mode = ZegoAudioCaptureStereoMode.getZegoAudioCaptureStereoMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setAudioCaptureStereoMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void addPublishCdnUrl(MethodCall call, final Result result) {

        String streamID = call.argument("streamID");
        String targetURL = call.argument("targetURL");
        int timeout = ZegoUtils.intValue((Number) call.argument("timeout"));

        ZegoExpressEngine.getEngine().addPublishCdnUrl(streamID, targetURL, timeout, new IZegoPublisherUpdateCdnUrlCallback() {
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

        HashMap<String, Object> configMap = call.argument("config");

        ZegoCDNConfig config = null;
        if (configMap != null && !configMap.isEmpty()) {
            config = new ZegoCDNConfig();
            config.url = (String) configMap.get("url");
            config.authParam = (String) configMap.get("authParam");
            config.protocol = (String) configMap.get("protocol");
            config.quicVersion = (String) configMap.get("quicVersion");
            config.quicConnectMode = ZegoUtils.intValue((Number) configMap.get("quicConnectMode"));
            config.httpdns = ZegoHttpDNSType.getZegoHttpDNSType(ZegoUtils.intValue((Number) configMap.get("httpdns")));
        }

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enablePublishDirectToCDN(enable, config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPublishWatermark(MethodCall call, Result result) {

        HashMap<String, Object> watermarkMap = call.argument("watermark");

        ZegoWatermark watermark = null;
        if (watermarkMap != null && !watermarkMap.isEmpty()) {
            int left = ZegoUtils.intValue((Number) watermarkMap.get("left"));
            int top = ZegoUtils.intValue((Number) watermarkMap.get("top"));
            int right = ZegoUtils.intValue((Number) watermarkMap.get("right"));
            int bottom = ZegoUtils.intValue((Number) watermarkMap.get("bottom"));
            Rect rect = new Rect(left, top, right, bottom);
            String imageURL = (String) watermarkMap.get("imageURL");

            if (imageURL != null && imageURL.startsWith("flutter-asset://")) {
                String processedURL = imageURL.replace("flutter-asset://", "asset://flutter_assets/");
                ZegoLog.log("[setPublishWatermark] Flutter asset prefix detected, origin URL: '%s', processed URL: '%s'", imageURL, processedURL);
                imageURL = processedURL;
            }

            watermark = new ZegoWatermark(imageURL, rect);
        }

        boolean isPreviewVisible = ZegoUtils.boolValue((Boolean) call.argument("isPreviewVisible"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setPublishWatermark(watermark, isPreviewVisible, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setSEIConfig(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");

        ZegoSEIConfig config = new ZegoSEIConfig(); // Use default config
        if (configMap != null && !configMap.isEmpty()) {
            config.type = ZegoSEIType.getZegoSEIType(ZegoUtils.intValue((Number) configMap.get("type")));
        }

        ZegoExpressEngine.getEngine().setSEIConfig(config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void sendSEI(MethodCall call, Result result) {

        byte[] data = call.argument("data");
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendSEI(data, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void sendAudioSideInfo(MethodCall call, Result result) {

        byte[] data = call.argument("data");
        double timeStampMs = ZegoUtils.doubleValue((Number) call.argument("timeStampMs"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendAudioSideInfo(data, timeStampMs, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHardwareEncoder(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareEncoder(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCapturePipelineScaleMode(MethodCall call, Result result) {

        ZegoCapturePipelineScaleMode mode = ZegoCapturePipelineScaleMode.getZegoCapturePipelineScaleMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setCapturePipelineScaleMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableH265EncodeFallback(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableH265EncodeFallback(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isVideoEncoderSupported(MethodCall call, Result result) {

        int codecIDIndex = ZegoUtils.intValue((Number) call.argument("codecID"));
        ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(codecIDIndex);
        if (codecIDIndex == ZegoVideoCodecID.values().length - 1) {
            codecID = ZegoVideoCodecID.UNKNOWN;
        }

        int supportRet = 0;
        if (call.argument("codecBackend") == null) {
            supportRet = ZegoExpressEngine.getEngine().isVideoEncoderSupported(codecID) ? 1: 0;
        } else {
            int codecBackendIndex = ZegoUtils.intValue((Number) call.argument("codecBackend"));
            ZegoVideoCodecBackend codecBackend = ZegoVideoCodecBackend.getZegoVideoCodecBackend(codecBackendIndex);
            supportRet = ZegoExpressEngine.getEngine().isVideoEncoderSupported(codecID, codecBackend);
        }

        result.success(supportRet);
    }

    @SuppressWarnings("unused")
    public static void setAppOrientationMode(MethodCall call, Result result) {

        ZegoOrientationMode mode = ZegoOrientationMode.getZegoOrientationMode( ZegoUtils.intValue((Number) call.argument("mode")) );

        ZegoExpressEngine.getEngine().setAppOrientationMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setLowlightEnhancement(MethodCall call, Result result) {

        ZegoLowlightEnhancementMode mode = ZegoLowlightEnhancementMode.getZegoLowlightEnhancementMode(ZegoUtils.intValue((Number) call.argument("mode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setLowlightEnhancement(mode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVideoSource(MethodCall call, Result result) {

        ZegoVideoSourceType source = ZegoVideoSourceType.getZegoVideoSourceType(ZegoUtils.intValue((Number) call.argument("source")));
        boolean hasChannel = false;
        ZegoPublishChannel channel = null;
        if (call.argument("channel") != null) {
            hasChannel = true;
            channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));
        }
        boolean hasInstanceID = false;
        int instanceID = -1;
        if (call.argument("instanceID") != null) {
            hasInstanceID = true;
            instanceID = ZegoUtils.intValue((Number) call.argument("instanceID"));
        }

        int ret = 0;
        if (!hasChannel && !hasInstanceID) {
            ret = ZegoExpressEngine.getEngine().setVideoSource(source);
        } else if (hasChannel && !hasInstanceID) {
            ret = ZegoExpressEngine.getEngine().setVideoSource(source, channel);
        } else if (!hasChannel && hasInstanceID) {
            ret = ZegoExpressEngine.getEngine().setVideoSource(source, instanceID);
        } else {
            ret = ZegoExpressEngine.getEngine().setVideoSource(source, instanceID, channel);
        }

        result.success(ret);
    }

    @SuppressWarnings("unused")
    public static void setAudioSource(MethodCall call, Result result) {

        ZegoAudioSourceType source = ZegoAudioSourceType.getZegoAudioSourceType(ZegoUtils.intValue((Number) call.argument("source")));
        boolean hasChannel = false;
        ZegoPublishChannel channel = null;
        if (call.argument("channel") != null) {
            hasChannel = true;
            channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));
        }
        boolean hasConfig = false;
        ZegoAudioSourceMixConfig config = new ZegoAudioSourceMixConfig();
        if (call.argument("config") != null) {
            hasConfig = true;
            HashMap<String, Object> configMap = call.argument("config");

            ArrayList<Integer> audioEffectPlayerIndexList = (ArrayList<Integer>)configMap.get("audioEffectPlayerIndexList");
            ArrayList<Integer> mediaPlayerIndexList = (ArrayList<Integer>)configMap.get("mediaPlayerIndexList");

            int[] audioEffectPlayerIndexList_ = new int[audioEffectPlayerIndexList.size()];
            for (int i = 0; i < audioEffectPlayerIndexList.size(); i++) {
                audioEffectPlayerIndexList_[i] = audioEffectPlayerIndexList.get(i);
            }
            int[] mediaPlayerIndexList_ = new int[mediaPlayerIndexList.size()];
            for (int i = 0; i < mediaPlayerIndexList.size(); i++) {
                mediaPlayerIndexList_[i] = mediaPlayerIndexList.get(i);
            }
            config.audioEffectPlayerIndexList = audioEffectPlayerIndexList_;
            config.mediaPlayerIndexList = mediaPlayerIndexList_;
        }

        int ret = -1;
        if (!hasChannel && !hasConfig) {
            ret = ZegoExpressEngine.getEngine().setAudioSource(source);
        } else if (hasChannel && !hasConfig) {
            ret = ZegoExpressEngine.getEngine().setAudioSource(source, channel);
        } else if (!hasChannel && hasConfig) {
            ret = ZegoExpressEngine.getEngine().setAudioSource(source, config);
        } else {
            if (channel == ZegoPublishChannel.MAIN) {
                ret = ZegoExpressEngine.getEngine().setAudioSource(source, config);
            }
        }

        result.success(ret);
    }

    @SuppressWarnings("unused")
    public static void enableVideoObjectSegmentation(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoObjectSegmentationConfig config = new ZegoObjectSegmentationConfig();
        HashMap<String, Object> configMap = call.argument("config");
        ZegoObjectSegmentationType type = ZegoObjectSegmentationType.getZegoObjectSegmentationType(ZegoUtils.intValue((Number) configMap.get("objectSegmentationType")));

        ZegoBackgroundConfig backgroundConfig = new ZegoBackgroundConfig();
        HashMap<String, Object> backgroundMap = (HashMap<String, Object>) configMap.get("backgroundConfig");
        backgroundConfig.processType = ZegoBackgroundProcessType.getZegoBackgroundProcessType(ZegoUtils.intValue((Number) backgroundMap.get("processType")));
        backgroundConfig.color = (ZegoUtils.intValue((Number) backgroundMap.get("color")));
        backgroundConfig.imageURL = (String) backgroundMap.get("imageURL");
        backgroundConfig.videoURL = (String) backgroundMap.get("videoURL");
        backgroundConfig.blurLevel = ZegoBackgroundBlurLevel.getZegoBackgroundBlurLevel(ZegoUtils.intValue((Number) backgroundMap.get("blurLevel")));

        config.backgroundConfig = backgroundConfig;
        config.objectSegmentationType = type;

        ZegoExpressEngine.getEngine().enableVideoObjectSegmentation(enable, config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableAlphaChannelVideoEncoder(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoAlphaLayoutType alphaLayout = ZegoAlphaLayoutType.getZegoAlphaLayoutType(ZegoUtils.intValue((Number) call.argument("alphaLayout")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableAlphaChannelVideoEncoder(enable, alphaLayout, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraStabilizationMode(MethodCall call, Result result) {

        int mode = ZegoUtils.intValue((Number) call.argument("mode"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraStabilizationMode(mode, channel);

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
            playerConfig.resourceMode = ZegoStreamResourceMode.getZegoStreamResourceMode(ZegoUtils.intValue((Number) playerConfigMap.get("resourceMode")));
            
            int videoCodecIDIndex = ZegoUtils.intValue((Number) playerConfigMap.get("videoCodecID"));
            if (videoCodecIDIndex == ZegoVideoCodecID.values().length - 1) {
                playerConfig.videoCodecID = ZegoVideoCodecID.UNKNOWN;
            } else {
                playerConfig.videoCodecID = ZegoVideoCodecID.getZegoVideoCodecID(videoCodecIDIndex);
            }
            playerConfig.roomID = (String) playerConfigMap.get("roomID");
            playerConfig.sourceResourceType = ZegoResourceType.getZegoResourceType(ZegoUtils.intValue((Number) playerConfigMap.get("sourceResourceType")));
            playerConfig.codecTemplateID = ZegoUtils.intValue((Number) playerConfigMap.get("codecTemplateID"));
            playerConfig.resourceSwitchMode = ZegoStreamResourceSwitchMode.getZegoStreamResourceSwitchMode(ZegoUtils.intValue((Number) playerConfigMap.get("resourceSwitchMode")));
            playerConfig.resourceWhenStopPublish = ZegoStreamResourceType.getZegoStreamResourceType(ZegoUtils.intValue((Number) playerConfigMap.get("resourceWhenStopPublish")));

            HashMap<String, Object> cdnConfigMap = (HashMap<String, Object>) playerConfigMap.get("cdnConfig");
            if (cdnConfigMap != null && !cdnConfigMap.isEmpty()) {

                ZegoCDNConfig cdnConfig = new ZegoCDNConfig();
                cdnConfig.url = (String) cdnConfigMap.get("url");
                cdnConfig.authParam = (String) cdnConfigMap.get("authParam");
                cdnConfig.protocol = (String) cdnConfigMap.get("protocol");
                cdnConfig.quicVersion = (String) cdnConfigMap.get("quicVersion");
                cdnConfig.quicConnectMode = ZegoUtils.intValue((Number) cdnConfigMap.get("quicConnectMode"));
                cdnConfig.httpdns =
                        ZegoHttpDNSType.getZegoHttpDNSType(ZegoUtils.intValue((Number) cdnConfigMap.get("httpdns")));
                playerConfig.cdnConfig = cdnConfig;
            }
        }

        // Handle ZegoCanvas

        HashMap<String, Object> canvasMap = call.argument("canvas");

        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Play video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = ZegoUtils.intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(ZegoUtils.intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = ZegoUtils.intValue((Number) canvasMap.get("backgroundColor"));
            boolean alphaBlend = ZegoUtils.boolValue((Boolean) canvasMap.get("alphaBlend"));

            Object view;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getSurfaceView();
                    if (alphaBlend) {
                        ((SurfaceView)view).getHolder().setFormat(PixelFormat.TRANSLUCENT);
                        ((SurfaceView)view).setZOrderOnTop(true);
                    }
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
                    // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createCanvasView` first and get the textureID", viewID);
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
                canvas.alphaBlend = alphaBlend;
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
    public static void setPlayStreamCrossAppInfo(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        HashMap<String, Object> infoMap = call.argument("info");
        ZegoCrossAppInfo info = new ZegoCrossAppInfo();
        info.appID = ZegoUtils.intValue((Number) infoMap.get("appID"));
        info.token = (String) infoMap.get("token");
        ZegoExpressEngine.getEngine().setPlayStreamCrossAppInfo(streamID, info);

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
        int volume = ZegoUtils.intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setPlayVolume(streamID, volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAllPlayStreamVolume(MethodCall call, Result result) {

        int volume = ZegoUtils.intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setAllPlayStreamVolume(volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayStreamVideoType(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        ZegoVideoStreamType streamType = ZegoVideoStreamType.getZegoVideoStreamType(ZegoUtils.intValue((Number) call.argument("streamType")));

        ZegoExpressEngine.getEngine().setPlayStreamVideoType(streamID, streamType);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayStreamBufferIntervalRange(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        int minBufferInterval = ZegoUtils.intValue((Number) call.argument("minBufferInterval"));
        int maxBufferInterval = ZegoUtils.intValue((Number) call.argument("maxBufferInterval"));

        ZegoExpressEngine.getEngine().setPlayStreamBufferIntervalRange(streamID, minBufferInterval, maxBufferInterval);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setPlayStreamFocusOn(MethodCall call, Result result) {

        String streamID = call.argument("streamID");

        ZegoExpressEngine.getEngine().setPlayStreamFocusOn(streamID);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePlayStreamAudio(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamAudio(streamID, mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void muteAllPlayStreamAudio(MethodCall call, Result result) {

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteAllPlayStreamAudio(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void muteAllPlayAudioStreams(MethodCall call, Result result) {

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteAllPlayAudioStreams(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mutePlayStreamVideo(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().mutePlayStreamVideo(streamID, mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void muteAllPlayVideoStreams(MethodCall call, Result result) {

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteAllPlayVideoStreams(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void muteAllPlayStreamVideo(MethodCall call, Result result) {

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteAllPlayStreamVideo(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHardwareDecoder(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHardwareDecoder(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCheckPoc(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableCheckPoc(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isVideoDecoderSupported(MethodCall call, Result result) {

        int videoCodecIDIndex = ZegoUtils.intValue((Number) call.argument("codecID"));
        ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(videoCodecIDIndex);
        if (videoCodecIDIndex == ZegoVideoCodecID.values().length - 1) {
            codecID = ZegoVideoCodecID.UNKNOWN;
        }

        int supportRet = 0;
        if (call.argument("codecBackend") == null) {
            supportRet = ZegoExpressEngine.getEngine().isVideoDecoderSupported(codecID) ? 1: 0;
        } else {
            int codecBackendIndex = ZegoUtils.intValue((Number) call.argument("codecBackend"));
            ZegoVideoCodecBackend codecBackend = ZegoVideoCodecBackend.getZegoVideoCodecBackend(codecBackendIndex);
            supportRet = ZegoExpressEngine.getEngine().isVideoDecoderSupported(codecID, codecBackend);
        }

        result.success(supportRet);
    }
    
    @SuppressWarnings("unused")
    public static void setPlayStreamsAlignmentProperty(MethodCall call, Result result) {

        ZegoStreamAlignmentMode mode = ZegoStreamAlignmentMode.getZegoStreamAlignmentMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setPlayStreamsAlignmentProperty(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableVideoSuperResolution(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        String streamID = call.argument("streamID");
        ZegoExpressEngine.getEngine().enableVideoSuperResolution(streamID, enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void initVideoSuperResolution(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().initVideoSuperResolution();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void uninitVideoSuperResolution(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().uninitVideoSuperResolution();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void updatePlayingCanvas(MethodCall call, Result result) {

        String streamID = call.argument("streamID");
        // Handle ZegoCanvas

        HashMap<String, Object> canvasMap = call.argument("canvas");
        ZegoCanvas canvas = null;
        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Play video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = ZegoUtils.intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(ZegoUtils.intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = ZegoUtils.intValue((Number) canvasMap.get("backgroundColor"));
            boolean alphaBlend = ZegoUtils.boolValue((Boolean) canvasMap.get("alphaBlend"));

            Object view;

            if (enablePlatformView) {
                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view = platformView.getSurfaceView();
                    if (alphaBlend) {
                        ((SurfaceView)view).getHolder().setFormat(PixelFormat.TRANSLUCENT);
                        ((SurfaceView)view).setZOrderOnTop(true);
                    }
                } else {
                    // Play video without creating the PlatformView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                    String errorMessage = String.format(Locale.ENGLISH, "The PlatformView for viewID:%d cannot be found, developer should call `createPlatformView` first and get the viewID", viewID);
                    ZegoLog.error("[updatePlayingCanvas] %s", errorMessage);
                    result.error("updatePlayingCanvas_No_PlatformView".toUpperCase(), errorMessage, null);
                    return;
                }

            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long) viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Play video without creating TextureRenderer in advance
                    // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createCanvasView` first and get the textureID", viewID);
                    ZegoLog.error("[updatePlayingCanvas] %s", errorMessage);
                    result.error("updatePlayingCanvas_No_TextureRenderer".toUpperCase(), errorMessage, null);
                    return;
                }
            }

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;
            }

            // Mark the canvas is in use
            if (!enablePlatformView) {
                ZegoTextureRendererController.getInstance().playerCanvasInUse.put(streamID, canvas);
            }
        }
        int errorCode = ZegoExpressEngine.getEngine().updatePlayingCanvas(streamID, canvas);

        result.success(errorCode);
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
                int contentType = ZegoUtils.intValue((Number) inputMap.get("contentType"));
                int left = ZegoUtils.intValue((Number) inputMap.get("left"));
                int top = ZegoUtils.intValue((Number) inputMap.get("top"));
                int right = ZegoUtils.intValue((Number) inputMap.get("right"));
                int bottom = ZegoUtils.intValue((Number) inputMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                int soundLevelID = ZegoUtils.intValue((Number) inputMap.get("soundLevelID"));
                
                ZegoMixerInput inputObject = new ZegoMixerInput(streamID, ZegoMixerInputContentType.getZegoMixerInputContentType(contentType), rect, soundLevelID);

                inputObject.volume = ZegoUtils.intValue((Number) inputMap.get("volume"));
                inputObject.isAudioFocus = ZegoUtils.boolValue((Boolean) inputMap.get("isAudioFocus"));
                inputObject.audioDirection = ZegoUtils.intValue((Number) inputMap.get("audioDirection"));

                if (inputMap.containsKey("label") && inputMap.get("label") != null) {
                    HashMap<String, Object> labelMap = (HashMap<String, Object>) inputMap.get("label");
                    String text = (String) labelMap.get("text");

                    ZegoLabelInfo labelInfo = new ZegoLabelInfo(text);
                    labelInfo.left = ZegoUtils.intValue((Number) labelMap.get("left"));
                    labelInfo.top = ZegoUtils.intValue((Number) labelMap.get("top"));

                    HashMap<String, Object> fontMap = (HashMap<String, Object>) labelMap.get("font");
                    ZegoFontStyle fontStyle = new ZegoFontStyle();
                    fontStyle.type = ZegoFontType.getZegoFontType(ZegoUtils.intValue((Number) fontMap.get("type")));
                    fontStyle.size = ZegoUtils.intValue((Number) fontMap.get("size"));
                    fontStyle.color = ZegoUtils.intValue((Number) fontMap.get("color"));
                    fontStyle.transparency = ZegoUtils.intValue((Number) fontMap.get("transparency"));
                    fontStyle.border = ZegoUtils.boolValue((Boolean) fontMap.get("border"));
                    fontStyle.borderColor = ZegoUtils.intValue((Number) fontMap.get("borderColor"));

                    labelInfo.font = fontStyle;
                    inputObject.label = labelInfo;
                }

                if (inputMap.containsKey("renderMode") && inputMap.get("renderMode") != null) {
                    inputObject.renderMode = ZegoMixRenderMode.getZegoMixRenderMode(ZegoUtils.intValue((Number) inputMap.get("renderMode")));
                }

                if (inputMap.containsKey("imageInfo") && inputMap.get("imageInfo") != null) {
                    HashMap<String, Object> imageInfoMap = (HashMap<String, Object>) inputMap.get("imageInfo");
                    String url = (String) imageInfoMap.get("url");
                    int displayMode = 0;
                    if (imageInfoMap.containsKey("displayMode") && imageInfoMap.get("displayMode") != null) {
                        displayMode = (int) imageInfoMap.get("displayMode");
                    }
                    inputObject.imageInfo = new ZegoMixerImageInfo(url, displayMode);
                }

                if (inputMap.containsKey("cornerRadius") && inputMap.get("cornerRadius") != null) {
                    inputObject.cornerRadius = ZegoUtils.intValue((Number) inputMap.get("cornerRadius"));
                }
                
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

                if (outputMap.containsKey("videoConfig") && outputMap.get("videoConfig") != null) {
                    HashMap<String, Object> videoConfigMap = (HashMap<String, Object>) outputMap.get("videoConfig");
                    int codecIDIndex = ZegoUtils.intValue((Number) videoConfigMap.get("videoCodecID"));
                    ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(codecIDIndex);
                    if (codecIDIndex == ZegoVideoCodecID.values().length - 1) {
                        codecID = ZegoVideoCodecID.UNKNOWN;
                    }
                    int bitrate = ZegoUtils.intValue((Number) videoConfigMap.get("bitrate"));
                    ZegoEncodeProfile encodeProfile = ZegoEncodeProfile.getZegoEncodeProfile(ZegoUtils.intValue((Number) videoConfigMap.get("encodeProfile")));
                    int encodeLatency = ZegoUtils.intValue((Number) videoConfigMap.get("encodeLatency"));
                    boolean enableLowBitrateHD = ZegoUtils.boolValue((Boolean) videoConfigMap.get("enableLowBitrateHD"));
                    outputObject.setVideoConfig(new ZegoMixerOutputVideoConfig(codecID, bitrate, encodeProfile, encodeLatency, enableLowBitrateHD));
                }
                outputListObject.add(outputObject);
            }
            taskObject.setOutputList(outputListObject);
        }

        // AudioConfig
        HashMap<String, Object> audioConfigMap = call.argument("audioConfig");
        if (audioConfigMap != null && !audioConfigMap.isEmpty()) {
            int bitrate = ZegoUtils.intValue((Number) audioConfigMap.get("bitrate"));
            int channel = ZegoUtils.intValue((Number) audioConfigMap.get("channel"));
            int codecID = ZegoUtils.intValue((Number) audioConfigMap.get("codecID"));
            int mixMode = ZegoUtils.intValue((Number) audioConfigMap.get("mixMode"));
            ZegoMixerAudioConfig audioConfigObject = new ZegoMixerAudioConfig();
            audioConfigObject.bitrate = bitrate;
            audioConfigObject.channel = ZegoAudioChannel.getZegoAudioChannel(channel);
            audioConfigObject.codecID = ZegoAudioCodecID.getZegoAudioCodecID(codecID);
            audioConfigObject.mixMode = ZegoAudioMixMode.getZegoAudioMixMode(mixMode);
            taskObject.setAudioConfig(audioConfigObject);
        }

        // VideoConfig
        HashMap<String, Object> videoConfigMap = call.argument("videoConfig");
        if (videoConfigMap != null && !videoConfigMap.isEmpty()) {
            int width = ZegoUtils.intValue((Number) videoConfigMap.get("width"));
            int height = ZegoUtils.intValue((Number) videoConfigMap.get("height"));
            int fps = ZegoUtils.intValue((Number) videoConfigMap.get("fps"));
            int bitrate = ZegoUtils.intValue((Number) videoConfigMap.get("bitrate"));
            ZegoMixerVideoConfig videoConfigObject = new ZegoMixerVideoConfig(width, height, fps, bitrate);
            videoConfigObject.quality = ZegoUtils.intValue((Number) videoConfigMap.get("quality"));
            videoConfigObject.rateControlMode = ZegoVideoRateControlMode.getZegoVideoRateControlMode(ZegoUtils.intValue((Number) videoConfigMap.get("rateControlMode")));

            taskObject.setVideoConfig(videoConfigObject);
        }

        // Watermark
        HashMap<String, Object> watermarkMap = call.argument("watermark");
        if (watermarkMap != null && !watermarkMap.isEmpty()) {
            String imageURL = (String) watermarkMap.get("imageURL");
            if (imageURL != null && imageURL.length() > 0) {
                int left = ZegoUtils.intValue((Number) watermarkMap.get("left"));
                int top = ZegoUtils.intValue((Number) watermarkMap.get("top"));
                int right = ZegoUtils.intValue((Number) watermarkMap.get("right"));
                int bottom = ZegoUtils.intValue((Number) watermarkMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                ZegoWatermark watermarkObject = new ZegoWatermark(imageURL, rect);

                taskObject.setWatermark(watermarkObject);
            }
        }

        // whiteboard
        HashMap<String, Object> whiteboardMap = call.argument("whiteboard");
        if (whiteboardMap != null && !whiteboardMap.isEmpty()) {
            long whiteboardID = ZegoUtils.longValue((Number) whiteboardMap.get("whiteboardID"));
            if (whiteboardID != 0) {
                ZegoMixerWhiteboard whiteboard = new ZegoMixerWhiteboard();
                whiteboard.whiteboardID = whiteboardID;
                whiteboard.horizontalRatio = ZegoUtils.intValue((Number) whiteboardMap.get("horizontalRatio"));
                whiteboard.verticalRatio = ZegoUtils.intValue((Number) whiteboardMap.get("verticalRatio"));
                whiteboard.isPPTAnimation = ZegoUtils.boolValue((Boolean) whiteboardMap.get("isPPTAnimation"));
                whiteboard.zOrder = ZegoUtils.intValue((Number) whiteboardMap.get("zOrder"));
                HashMap<String, Object> layoutMap = (HashMap<String, Object>)whiteboardMap.get("layout");
                if (layoutMap != null && !layoutMap.isEmpty()) {
                    whiteboard.layout = new Rect();
                    whiteboard.layout.top = ZegoUtils.intValue((Number) layoutMap.get("top"));
                    whiteboard.layout.left = ZegoUtils.intValue((Number) layoutMap.get("left"));
                    whiteboard.layout.right = ZegoUtils.intValue((Number) layoutMap.get("right"));
                    whiteboard.layout.bottom = ZegoUtils.intValue((Number) layoutMap.get("bottom"));
                }
                taskObject.setWhiteboard(whiteboard);
            }
        }

        taskObject.backgroundColor = ZegoUtils.intValue((Number) call.argument("backgroundColor"));

        // Background Image
        String backgroundImageURL = call.argument("backgroundImageURL");
        if (backgroundImageURL != null && backgroundImageURL.length() > 0) {
            taskObject.setBackgroundImageURL(backgroundImageURL);
        }

        // Enable SoundLevel
        boolean enableSoundLevel = ZegoUtils.boolValue((Boolean) call.argument("enableSoundLevel"));
        taskObject.enableSoundLevel(enableSoundLevel);

        ZegoStreamAlignmentMode streamAlignmentMode = ZegoStreamAlignmentMode.getZegoStreamAlignmentMode(ZegoUtils.intValue((Number) call.argument("streamAlignmentMode")));
        taskObject.setStreamAlignmentMode(streamAlignmentMode);

        byte[] userData = call.argument("userData");
        ByteBuffer userDataByteBuffer =  ByteBuffer.allocateDirect(userData.length);
        userDataByteBuffer = userDataByteBuffer.put(userData);
        userDataByteBuffer.flip();
        taskObject.setUserData(userDataByteBuffer, userData.length);

        // minPlayStreamBufferLength
        int minPlayStreamBufferLength = ZegoUtils.intValue((Number) call.argument("minPlayStreamBufferLength"));
        taskObject.setMinPlayStreamBufferLength(minPlayStreamBufferLength);

        // Set AdvancedConfig
        HashMap<String, String> advancedConfig = call.argument("advancedConfig");
        taskObject.setAdvancedConfig(advancedConfig);

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
    public static void stopMixerTask(final MethodCall call, final Result result) {

        String taskID = call.argument("taskID");
        ZegoMixerTask taskObject = new ZegoMixerTask(taskID);

        // MixerInput
        ArrayList<HashMap<String, Object>> inputListMap = call.argument("inputList");
        if (inputListMap != null && !inputListMap.isEmpty()) {
            ArrayList<ZegoMixerInput> inputListObject= new ArrayList<>();
            for (HashMap<String, Object> inputMap: inputListMap) {
                String streamID = (String) inputMap.get("streamID");
                int contentType = ZegoUtils.intValue((Number) inputMap.get("contentType"));
                int left = ZegoUtils.intValue((Number) inputMap.get("left"));
                int top = ZegoUtils.intValue((Number) inputMap.get("top"));
                int right = ZegoUtils.intValue((Number) inputMap.get("right"));
                int bottom = ZegoUtils.intValue((Number) inputMap.get("bottom"));
                Rect rect = new Rect(left, top, right, bottom);
                int soundLevelID = ZegoUtils.intValue((Number) inputMap.get("soundLevelID"));
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

                if (outputMap.containsKey("videoConfig") && outputMap.get("videoConfig") != null) {
                    HashMap<String, Object> videoConfigMap = (HashMap<String, Object>) outputMap.get("videoConfig");
                    int codecIDIndex = ZegoUtils.intValue((Number) videoConfigMap.get("videoCodecID"));
                    ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(codecIDIndex);
                    if (codecIDIndex == ZegoVideoCodecID.values().length -1) {
                        codecID = ZegoVideoCodecID.UNKNOWN;
                    }
                    int bitrate = ZegoUtils.intValue((Number) videoConfigMap.get("bitrate"));
                    ZegoEncodeProfile encodeProfile = ZegoEncodeProfile.getZegoEncodeProfile(ZegoUtils.intValue((Number) videoConfigMap.get("encodeProfile")));
                    int encodeLatency = ZegoUtils.intValue((Number) videoConfigMap.get("encodeLatency"));
                    boolean enableLowBitrateHD = ZegoUtils.boolValue((Boolean) videoConfigMap.get("enableLowBitrateHD"));
                    outputObject.setVideoConfig(new ZegoMixerOutputVideoConfig(codecID, bitrate, encodeProfile, encodeLatency, enableLowBitrateHD));
                }
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
                try {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }  catch (IllegalStateException e) {
                    // exception: Reply already submitted
                    ZegoLog.log("[DartCall] [IllegalStateException] [%s] %s | %s | %s", call.method, e.getCause(), e.getMessage(), getStackTrace(e));
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void startAutoMixerTask(MethodCall call, final Result result) {

        String taskID = call.argument("taskID");
        String roomID = call.argument("roomID");
        ZegoAutoMixerTask taskObject = new ZegoAutoMixerTask();
        taskObject.taskID = taskID;
        taskObject.roomID = roomID;

        // MixerOutput
        ArrayList<HashMap<String, Object>> outputListMap = call.argument("outputList");
        if (outputListMap != null && !outputListMap.isEmpty()) {
            ArrayList<ZegoMixerOutput> outputListObject = new ArrayList<>();
            for (HashMap<String, Object> outputMap : outputListMap) {
                String target = (String) outputMap.get("target");
                ZegoMixerOutput outputObject = new ZegoMixerOutput(target);

                if (outputMap.containsKey("videoConfig") && outputMap.get("videoConfig") != null) {
                    HashMap<String, Object> videoConfigMap = (HashMap<String, Object>) outputMap.get("videoConfig");
                    int codecIDIndex = ZegoUtils.intValue((Number) videoConfigMap.get("videoCodecID"));
                    ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(codecIDIndex);
                    if (codecIDIndex == ZegoVideoCodecID.values().length - 1) {
                        codecID = ZegoVideoCodecID.UNKNOWN;
                    }
                    int bitrate = ZegoUtils.intValue((Number) videoConfigMap.get("bitrate"));
                    ZegoEncodeProfile encodeProfile = ZegoEncodeProfile.getZegoEncodeProfile(ZegoUtils.intValue((Number) videoConfigMap.get("encodeProfile")));
                    int encodeLatency = ZegoUtils.intValue((Number) videoConfigMap.get("encodeLatency"));
                    boolean enableLowBitrateHD = ZegoUtils.boolValue((Boolean) videoConfigMap.get("enableLowBitrateHD"));
                    outputObject.setVideoConfig(new ZegoMixerOutputVideoConfig(codecID, bitrate, encodeProfile, encodeLatency, enableLowBitrateHD));
                }
                outputListObject.add(outputObject);
            }
            taskObject.outputList = outputListObject;
        }

        // AudioConfig
        HashMap<String, Object> audioConfigMap = call.argument("audioConfig");
        if (audioConfigMap != null && !audioConfigMap.isEmpty()) {
            int bitrate = ZegoUtils.intValue((Number) audioConfigMap.get("bitrate"));
            int channel = ZegoUtils.intValue((Number) audioConfigMap.get("channel"));
            int codecID = ZegoUtils.intValue((Number) audioConfigMap.get("codecID"));
            ZegoMixerAudioConfig audioConfigObject = new ZegoMixerAudioConfig();
            audioConfigObject.bitrate = bitrate;
            audioConfigObject.channel = ZegoAudioChannel.getZegoAudioChannel(channel);
            audioConfigObject.codecID = ZegoAudioCodecID.getZegoAudioCodecID(codecID);

            taskObject.audioConfig = audioConfigObject;
        }

        // Enable SoundLevel
        boolean enableSoundLevel = ZegoUtils.boolValue((Boolean) call.argument("enableSoundLevel"));
        taskObject.enableSoundLevel = enableSoundLevel;
        
        ZegoStreamAlignmentMode streamAlignmentMode = ZegoStreamAlignmentMode.getZegoStreamAlignmentMode(ZegoUtils.intValue((Number) call.argument("streamAlignmentMode")));
        taskObject.streamAlignmentMode = streamAlignmentMode;
        
        // minPlayStreamBufferLength
        taskObject.minPlayStreamBufferLength = ZegoUtils.intValue((Number)call.argument("minPlayStreamBufferLength"));

        ZegoExpressEngine.getEngine().startAutoMixerTask(taskObject, new IZegoMixerStartCallback() {
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
    public static void stopAutoMixerTask(MethodCall call, final Result result) {

        String taskID = call.argument("taskID");
        String roomID = call.argument("roomID");
        ZegoAutoMixerTask taskObject = new ZegoAutoMixerTask();
        taskObject.taskID = taskID;
        taskObject.roomID = roomID;

        // MixerOutput
        ArrayList<HashMap<String, Object>> outputListMap = call.argument("inputList");
        if (outputListMap != null && !outputListMap.isEmpty()) {
            ArrayList<ZegoMixerOutput> outputListObject = new ArrayList<>();
            for (HashMap<String, Object> outputMap : outputListMap) {
                String target = (String) outputMap.get("target");
                ZegoMixerOutput outputObject = new ZegoMixerOutput(target);

                if (outputMap.containsKey("videoConfig") && outputMap.get("videoConfig") != null) {
                    HashMap<String, Object> videoConfigMap = (HashMap<String, Object>) outputMap.get("videoConfig");
                    int codecIDIndex = ZegoUtils.intValue((Number) videoConfigMap.get("videoCodecID"));
                    ZegoVideoCodecID codecID = ZegoVideoCodecID.getZegoVideoCodecID(codecIDIndex);
                    if (codecIDIndex == ZegoVideoCodecID.values().length - 1) {
                        codecID = ZegoVideoCodecID.UNKNOWN;
                    }
                    int bitrate = ZegoUtils.intValue((Number) videoConfigMap.get("bitrate"));
                    ZegoEncodeProfile encodeProfile = ZegoEncodeProfile.getZegoEncodeProfile(ZegoUtils.intValue((Number) videoConfigMap.get("encodeProfile")));
                    int encodeLatency = ZegoUtils.intValue((Number) videoConfigMap.get("encodeLatency"));
                    boolean enableLowBitrateHD = ZegoUtils.boolValue((Boolean) videoConfigMap.get("enableLowBitrateHD"));
                    outputObject.setVideoConfig(new ZegoMixerOutputVideoConfig(codecID, bitrate, encodeProfile, encodeLatency, enableLowBitrateHD));
                }
                outputListObject.add(outputObject);
            }
            taskObject.outputList = outputListObject;
        }

        // no need to set audio config

        // no need to set video config

        // no need to set watermark

        // no need to set background image

        // no need to set enable sound level

        ZegoExpressEngine.getEngine().stopAutoMixerTask(taskObject, new IZegoMixerStopCallback() {
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

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

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

        boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));

        ZegoExpressEngine.getEngine().muteSpeaker(mute);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isSpeakerMuted(MethodCall call, Result result) {

        boolean muted = ZegoExpressEngine.getEngine().isSpeakerMuted();

        result.success(muted);
    }

    @SuppressWarnings("unused")
    public static void setAudioDeviceMode(MethodCall call, Result result) {

        ZegoAudioDeviceMode deviceMode = ZegoAudioDeviceMode.getZegoAudioDeviceMode( ZegoUtils.intValue((Number) call.argument("deviceMode")));
        ZegoExpressEngine.getEngine().setAudioDeviceMode(deviceMode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableAudioCaptureDevice(MethodCall call, final Result result) {

        final boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        new Thread() {
            @Override
            public void run() {
                super.run();
                ZegoExpressEngine.getEngine().enableAudioCaptureDevice(enable);

                result.success(null);
            }
        }.start();
    }

    @SuppressWarnings("unused")
    public static void setAudioRouteToSpeaker(MethodCall call, Result result) {

        boolean defaultToSpeaker = ZegoUtils.boolValue((Boolean) call.argument("defaultToSpeaker"));

        ZegoExpressEngine.getEngine().setAudioRouteToSpeaker(defaultToSpeaker);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getAudioRouteType(MethodCall call, Result result) {

        ZegoAudioRoute route = ZegoExpressEngine.getEngine().getAudioRouteType();

        // TODO: Fix the issue of native returning to null
        if (route != null) {
            result.success(route.value());
        } else {
            result.success(0);
        }
    }

    @SuppressWarnings("unused")
    public static void enableCamera(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableCamera(enable, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void useFrontCamera(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().useFrontCamera(enable, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void isCameraFocusSupported(MethodCall call, Result result) {
        
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        boolean supported = ZegoExpressEngine.getEngine().isCameraFocusSupported(channel);

        result.success(supported);
    }

    @SuppressWarnings("unused")
    public static void setCameraFocusMode(MethodCall call, Result result) {

        ZegoCameraFocusMode mode = ZegoCameraFocusMode.getZegoCameraFocusMode(ZegoUtils.intValue((Number) call.argument("mode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraFocusMode(mode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraFocusPointInPreview(MethodCall call, Result result) {

        float x = ZegoUtils.floatValue((Number) call.argument("x"));
        float y = ZegoUtils.floatValue((Number) call.argument("y"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraFocusPointInPreview(x, y, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraExposureMode(MethodCall call, Result result) {

        ZegoCameraExposureMode mode = ZegoCameraExposureMode.getZegoCameraExposureMode(ZegoUtils.intValue((Number) call.argument("mode")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraExposureMode(mode, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraExposurePointInPreview(MethodCall call, Result result) {

        float x = ZegoUtils.floatValue((Number) call.argument("x"));
        float y = ZegoUtils.floatValue((Number) call.argument("y"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraExposurePointInPreview(x, y, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraExposureCompensation(MethodCall call, Result result) {

        float value = ZegoUtils.floatValue((Number) call.argument("value"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraExposureCompensation(value, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setCameraZoomFactor(MethodCall call, Result result) {

        float factor = ZegoUtils.floatValue((Number) call.argument("factor"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setCameraZoomFactor(factor, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getCameraMaxZoomFactor(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        float factor = ZegoExpressEngine.getEngine().getCameraMaxZoomFactor(channel);

        result.success(factor);
    }

    @SuppressWarnings("unused")
    public static void enableCameraAdaptiveFPS(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int minFPS = ZegoUtils.intValue((Number) call.argument("minFPS"));
        int maxFPS = ZegoUtils.intValue((Number) call.argument("maxFPS"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));
        ZegoLog.error("[enableCameraAdaptiveFPS] enable: %s, minFPS: %d, maxFPS: %d, channel: %s", enable?"true":"false", minFPS, maxFPS, channel.name());
        ZegoExpressEngine.getEngine().enableCameraAdaptiveFPS(enable, minFPS, maxFPS, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startSoundLevelMonitor(MethodCall call, Result result) {

        ZegoSoundLevelConfig config = new ZegoSoundLevelConfig();

        HashMap<String, Object> configMap = call.argument("config");
        config.millisecond = ZegoUtils.intValue((Number) configMap.get("millisecond"));
        config.enableVAD = ZegoUtils.boolValue((Boolean) configMap.get("enableVAD"));

        ZegoExpressEngine.getEngine().startSoundLevelMonitor(config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopSoundLevelMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopSoundLevelMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startAudioSpectrumMonitor(MethodCall call, Result result) {

        int millisecond = ZegoUtils.intValue((Number) call.argument("millisecond"));

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

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHeadphoneMonitor(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setHeadphoneMonitorVolume(MethodCall call, Result result) {

        int volume = ZegoUtils.intValue((Number) call.argument("volume"));

        ZegoExpressEngine.getEngine().setHeadphoneMonitorVolume(volume);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startAudioVADStableStateMonitor(MethodCall call, Result result) {

        ZegoAudioVADStableStateMonitorType type = ZegoAudioVADStableStateMonitorType.getZegoAudioVADStableStateMonitorType(ZegoUtils.intValue((Number) call.argument("type")));
            
        if (call.argument("millisecond") == null) {
            ZegoExpressEngine.getEngine().startAudioVADStableStateMonitor(type);
        } else {
            int millisecond = ZegoUtils.intValue((Number) call.argument("millisecond"));
            ZegoExpressEngine.getEngine().startAudioVADStableStateMonitor(type, millisecond);
        }  

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopAudioVADStableStateMonitor(MethodCall call, Result result) {

        ZegoAudioVADStableStateMonitorType type = ZegoAudioVADStableStateMonitorType.getZegoAudioVADStableStateMonitorType(ZegoUtils.intValue((Number) call.argument("type")));
        
        ZegoExpressEngine.getEngine().stopAudioVADStableStateMonitor(type);

        result.success(null);
    }

    /* PreProcess */

    @SuppressWarnings("unused")
    public static void enableAEC(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAEC(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableHeadphoneAEC(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableHeadphoneAEC(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAECMode(MethodCall call, Result result) {

        ZegoAECMode mode = ZegoAECMode.getZegoAECMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setAECMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableAGC(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableAGC(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableANS(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableANS(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableTransientANS(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableTransientANS(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setANSMode(MethodCall call, Result result) {

        ZegoANSMode mode = ZegoANSMode.getZegoANSMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setANSMode(mode);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableSpeechEnhance(MethodCall call, Result result) {

        Boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int level = ZegoUtils.intValue((Number) call.argument("level"));

        ZegoExpressEngine.getEngine().enableSpeechEnhance(enable, level);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startEffectsEnv(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().startEffectsEnv();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopEffectsEnv(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopEffectsEnv();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableEffectsBeauty(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));

        ZegoExpressEngine.getEngine().enableEffectsBeauty(enable);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setEffectsBeautyParam(MethodCall call, Result result) {

        HashMap<String, Integer> paramMap = call.argument("param");
        if (paramMap == null || paramMap.isEmpty()) {
            result.error("setEffectsBeautyParam_Null_Param".toUpperCase(), "[setEffectsBeautyParam] Null param", null);
            return;
        }

        Integer rosyIntensity = ZegoUtils.intValue(paramMap.get("rosyIntensity"));
        Integer sharpenIntensity = ZegoUtils.intValue(paramMap.get("sharpenIntensity"));
        Integer smoothIntensity = ZegoUtils.intValue(paramMap.get("smoothIntensity"));
        Integer whitenIntensity = ZegoUtils.intValue(paramMap.get("whitenIntensity"));

        ZegoEffectsBeautyParam param = new ZegoEffectsBeautyParam();
        param.rosyIntensity = rosyIntensity;
        param.sharpenIntensity = sharpenIntensity;
        param.smoothIntensity = smoothIntensity;
        param.whitenIntensity = whitenIntensity;

        ZegoExpressEngine.getEngine().setEffectsBeautyParam(param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableBeautify(MethodCall call, Result result) {

        int featureBitmask = ZegoUtils.intValue((Number) call.argument("featureBitmask"));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

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

        double polishStep = ZegoUtils.doubleValue(optionMap.get("polishStep"));
        double whitenFactor = ZegoUtils.doubleValue(optionMap.get("whitenFactor"));
        double sharpenFactor = ZegoUtils.doubleValue(optionMap.get("sharpenFactor"));

        ZegoBeautifyOption option = new ZegoBeautifyOption();
        option.polishStep = polishStep;
        option.whitenFactor = whitenFactor;
        option.sharpenFactor = sharpenFactor;

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().setBeautifyOption(option, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setAudioEqualizerGain(MethodCall call, Result result) {

        int bandIndex = ZegoUtils.intValue((Number) call.argument("bandIndex"));
        float bandGain = ZegoUtils.floatValue((Number) call.argument("bandGain"));

        ZegoExpressEngine.getEngine().setAudioEqualizerGain(bandIndex, bandGain);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setVoiceChangerPreset(MethodCall call, Result result) {

        ZegoVoiceChangerPreset preset = ZegoVoiceChangerPreset.getZegoVoiceChangerPreset(ZegoUtils.intValue((Number) call.argument("preset")));

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
        param.pitch = ZegoUtils.floatValue(paramMap.get("pitch"));

        ZegoExpressEngine.getEngine().setVoiceChangerParam(param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setReverbPreset(MethodCall call, Result result) {

        ZegoReverbPreset preset = ZegoReverbPreset.getZegoReverbPreset(ZegoUtils.intValue((Number) call.argument("preset")));

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
        param.roomSize = ZegoUtils.floatValue((Number) paramMap.get("roomSize"));
        param.reverberance = ZegoUtils.floatValue((Number) paramMap.get("reverberance"));
        param.damping = ZegoUtils.floatValue((Number) paramMap.get("damping"));
        param.wetOnly = ZegoUtils.boolValue((Boolean) paramMap.get("wetOnly"));
        param.wetGain = ZegoUtils.floatValue((Number) paramMap.get("wetGain"));
        param.dryGain = ZegoUtils.floatValue((Number) paramMap.get("dryGain"));
        param.toneLow = ZegoUtils.floatValue((Number) paramMap.get("toneLow"));
        param.toneHigh = ZegoUtils.floatValue((Number) paramMap.get("toneHigh"));
        param.preDelay = ZegoUtils.floatValue((Number) paramMap.get("preDelay"));
        param.stereoWidth = ZegoUtils.floatValue((Number) paramMap.get("stereoWidth"));

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

        param.inGain = ZegoUtils.floatValue((Number) paramMap.get("inGain"));
        param.outGain = ZegoUtils.floatValue((Number) paramMap.get("outGain"));
        param.numDelays = ZegoUtils.intValue((Number) paramMap.get("numDelays"));
        param.delay = ZegoUtils.intArrayValue((ArrayList<Integer>) paramMap.get("delay"));
        param.decay = ZegoUtils.floatArrayValueFromDoubleArray((ArrayList<Double>) paramMap.get("decay"));

        ZegoExpressEngine.getEngine().setReverbEchoParam(param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableVirtualStereo(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int angle = ZegoUtils.intValue((Number) call.argument("angle"));

        ZegoExpressEngine.getEngine().enableVirtualStereo(enable, angle);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enablePlayStreamVirtualStereo(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int angle = ZegoUtils.intValue((Number) call.argument("angle"));
        String streamID = call.argument("streamID");

        ZegoExpressEngine.getEngine().enablePlayStreamVirtualStereo(enable, angle, streamID);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void setElectronicEffects(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        int tonal = ZegoUtils.intValue((Number) call.argument("tonal"));
        ZegoElectronicEffectsMode mode = ZegoElectronicEffectsMode.getZegoElectronicEffectsMode(ZegoUtils.intValue((Number) call.argument("mode")));

        ZegoExpressEngine.getEngine().setElectronicEffects(enable, mode, tonal);

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

    @SuppressWarnings("unused")
    public static void sendTransparentMessage(MethodCall call, final Result result) {

        ArrayList<ZegoUser> toUserList = new ArrayList<>();

        ArrayList<HashMap<String, Object>> toUserMapList = call.argument("recvUserList");
        if (toUserMapList != null)
        {
            for (HashMap<String, Object> userMap: toUserMapList) {
                String userID = (String) userMap.get("userID");

                if(userID.isEmpty())
                    continue;

                String userName = (String) userMap.get("userName");
                ZegoUser user = new ZegoUser(userID, userName);
                toUserList.add(user);
            }
        }

        String roomID = call.argument("roomID");
        int sendMode = ZegoUtils.intValue((Number) call.argument("sendMode"));
        int sendType = ZegoUtils.intValue((Number) call.argument("sendType"));
        int timeOut = ZegoUtils.intValue((Number) call.argument("timeOut"));
        
        byte[] content = call.argument("content");

        ZegoRoomSendTransparentMessage message = new ZegoRoomSendTransparentMessage();
        message.sendMode = ZegoRoomTransparentMessageMode.getZegoRoomTransparentMessageMode(sendMode);
        message.sendType = ZegoRoomTransparentMessageType.getZegoRoomTransparentMessageType(sendType);
        message.timeOut = timeOut;
        message.recvUserList = toUserList;
        message.content = content;

        ZegoExpressEngine.getEngine().sendTransparentMessage(roomID, message,new IZegoRoomSendTransparentMessageCallback() {
            @Override
            public void onRoomSendTransparentMessageResult(int errorCode){
                HashMap<String, Object> resultMap = new HashMap<>();
                resultMap.put("errorCode", errorCode);
                result.success(resultMap);
            }
        });
    }

    /* CustomVideoCapture */
    @SuppressWarnings("unused")
    public static void enableCustomVideoRender(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomVideoRenderConfig config = new ZegoCustomVideoRenderConfig();
        int bufferType = ZegoUtils.intValue((Number) configMap.get("bufferType"));
        config.bufferType = ZegoVideoBufferType.getZegoVideoBufferType(bufferType);
        int frameFormatSeries = ZegoUtils.intValue((Number) configMap.get("frameFormatSeries"));
        config.frameFormatSeries = ZegoVideoFrameFormatSeries.getZegoVideoFrameFormatSeries(frameFormatSeries);
        config.enableEngineRender = ZegoUtils.boolValue((Boolean) configMap.get("enableEngineRender"));

        if (enable) {
            ZegoExpressEngine.getEngine().setCustomVideoRenderHandler(ZegoCustomVideoRenderManager.getInstance());
        } else {
            ZegoExpressEngine.getEngine().setCustomVideoRenderHandler(null);
        }
        
        ZegoExpressEngine.getEngine().enableCustomVideoRender(enable, config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomVideoCapture(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Double> configMap = call.argument("config");
        int channel = ZegoUtils.intValue((Number) call.argument("channel"));

        ZegoCustomVideoCaptureConfig config = new ZegoCustomVideoCaptureConfig();

        if (configMap != null && !configMap.isEmpty()) {
            int bufferType = ZegoUtils.intValue(configMap.get("bufferType"));
            config.bufferType = ZegoVideoBufferType.getZegoVideoBufferType(bufferType);
        } else {
            // If `config` is empty, set the default configuration (raw data for Android)
            config.bufferType = ZegoVideoBufferType.RAW_DATA;
        }

        if (enable) {
            ZegoExpressEngine.getEngine().setCustomVideoCaptureHandler(ZegoCustomVideoCaptureManager.getInstance());
        } else {
            ZegoExpressEngine.getEngine().setCustomVideoCaptureHandler(null);
        }
        
        ZegoExpressEngine.getEngine().enableCustomVideoCapture(enable, config, ZegoPublishChannel.getZegoPublishChannel(channel));

        // When using custom video capture, turn off preview mirroring
        if (enable) {
            ZegoExpressEngine.getEngine().setVideoMirrorMode(ZegoVideoMirrorMode.NO_MIRROR, ZegoPublishChannel.getZegoPublishChannel(channel));
        } else {
            ZegoExpressEngine.getEngine().setVideoMirrorMode(ZegoVideoMirrorMode.ONLY_PREVIEW_MIRROR, ZegoPublishChannel.getZegoPublishChannel(channel));
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomVideoProcessing(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Double> configMap = call.argument("config");
        int channel = ZegoUtils.intValue((Number) call.argument("channel"));

        ZegoCustomVideoProcessConfig config = new ZegoCustomVideoProcessConfig();

        int bufferType = ZegoUtils.intValue(configMap.get("bufferType"));
        config.bufferType = ZegoVideoBufferType.getZegoVideoBufferType(bufferType);

        if (enable) {
            ZegoExpressEngine.getEngine().setCustomVideoProcessHandler(ZegoCustomVideoProcessManager.getInstance());
        } else {
            ZegoExpressEngine.getEngine().setCustomVideoProcessHandler(null);
        }
        
        ZegoExpressEngine.getEngine().enableCustomVideoProcessing(enable, config, ZegoPublishChannel.getZegoPublishChannel(channel));

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomAudioCaptureProcessing(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomAudioProcessConfig config = new ZegoCustomAudioProcessConfig();
        config.samples = ZegoUtils.intValue((Number) configMap.get("samples"));
        config.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) configMap.get("sampleRate")));
        config.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) configMap.get("channel")));

        ZegoExpressEngine.getEngine().enableCustomAudioCaptureProcessing(enable, config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomAudioProcessConfig config = new ZegoCustomAudioProcessConfig();
        config.samples = ZegoUtils.intValue((Number) configMap.get("samples"));
        config.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) configMap.get("sampleRate")));
        config.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) configMap.get("channel")));

        ZegoExpressEngine.getEngine().enableCustomAudioCaptureProcessingAfterHeadphoneMonitor(enable, config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableAlignedAudioAuxData(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> paramMap = call.argument("param");

        ZegoAudioFrameParam param = new ZegoAudioFrameParam();
        param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) paramMap.get("sampleRate")));
        param.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) paramMap.get("channel")));

        ZegoExpressEngine.getEngine().enableAlignedAudioAuxData(enable, param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomAudioRemoteProcessing(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomAudioProcessConfig config = new ZegoCustomAudioProcessConfig();
        config.samples = ZegoUtils.intValue((Number) configMap.get("samples"));
        config.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) configMap.get("sampleRate")));
        config.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) configMap.get("channel")));

        ZegoExpressEngine.getEngine().enableCustomAudioRemoteProcessing(enable, config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomAudioPlaybackProcessing(MethodCall call, Result result) {

        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomAudioProcessConfig config = new ZegoCustomAudioProcessConfig();
        config.samples = ZegoUtils.intValue((Number) configMap.get("samples"));
        config.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) configMap.get("sampleRate")));
        config.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) configMap.get("channel")));

        ZegoExpressEngine.getEngine().enableCustomAudioPlaybackProcessing(enable, config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startAudioDataObserver(MethodCall call, Result result) {

        int bitmask = ZegoUtils.intValue((Number) call.argument("observerBitMask"));
        HashMap<String, Object> paramMap = call.argument("param");

        ZegoAudioFrameParam param = new ZegoAudioFrameParam();
        param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) paramMap.get("sampleRate")));
        param.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) paramMap.get("channel")));

        ZegoExpressEngine.getEngine().startAudioDataObserver(bitmask, param);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopAudioDataObserver(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopAudioDataObserver();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void enableCustomAudioIO(MethodCall call, Result result) {
        
        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> configMap = call.argument("config");

        ZegoCustomAudioConfig config = new ZegoCustomAudioConfig();
        config.sourceType = ZegoAudioSourceType.getZegoAudioSourceType(ZegoUtils.intValue((Number) configMap.get("sourceType")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableCustomAudioIO(enable, config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void sendCustomAudioCaptureAACData(MethodCall call, Result result) {
        
        byte[] data = call.argument("data");
        ByteBuffer dataByteBuffer =  ByteBuffer.allocateDirect(data.length);
        dataByteBuffer = dataByteBuffer.put(data);
        dataByteBuffer.flip();
        int dataLength = ZegoUtils.intValue((Number) call.argument("dataLength"));
        int configLength = ZegoUtils.intValue((Number) call.argument("configLength"));
        int referenceTimeMillisecond = ZegoUtils.intValue((Number) call.argument("referenceTimeMillisecond"));
        int samples = ZegoUtils.intValue((Number) call.argument("samples"));
        HashMap<String, Object> paramMap = call.argument("param");

        ZegoAudioFrameParam param = new ZegoAudioFrameParam();
        param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) paramMap.get("sampleRate")));
        param.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) paramMap.get("channel")));

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendCustomAudioCaptureAACData(dataByteBuffer, dataLength, configLength, referenceTimeMillisecond, samples, param, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void sendCustomAudioCapturePCMData(MethodCall call, Result result) {
        
        byte[] data = call.argument("data");
        ByteBuffer dataByteBuffer =  ByteBuffer.allocateDirect(data.length);
        dataByteBuffer = dataByteBuffer.put(data);
        dataByteBuffer.flip();
        int dataLength = ZegoUtils.intValue((Number) call.argument("dataLength"));
        HashMap<String, Object> paramMap = call.argument("param");

        ZegoAudioFrameParam param = new ZegoAudioFrameParam();
        param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) paramMap.get("sampleRate")));
        param.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) paramMap.get("channel")));
        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().sendCustomAudioCapturePCMData(dataByteBuffer, dataLength, param, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void fetchCustomAudioRenderPCMData(MethodCall call, Result result) {
        
        byte[] data = call.argument("data");
        ByteBuffer dataByteBuffer =  ByteBuffer.allocateDirect(data.length);
        dataByteBuffer = dataByteBuffer.put(data);
        dataByteBuffer.flip();
        int dataLength = ZegoUtils.intValue((Number) call.argument("dataLength"));
        HashMap<String, Object> paramMap = call.argument("param");

        ZegoAudioFrameParam param = new ZegoAudioFrameParam();
        param.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) paramMap.get("sampleRate")));
        param.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) paramMap.get("channel")));

        ZegoExpressEngine.getEngine().fetchCustomAudioRenderPCMData(dataByteBuffer, dataLength, param);

        result.success(null);
    }

    /* MediaPlayer */

    public static ZegoMediaPlayer getMediaPlayer(Integer index) {
        return mediaPlayerHashMap.get(index);
    }

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

        // Mark the canvas is no longer used
        if (!enablePlatformView) {
            ZegoTextureRendererController.getInstance().mediaPlayerCanvasInUse.remove(index);
        }

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
                    try {
                        result.success(resultMap);
                    } catch (Exception e) {
                        ZegoLog.error("[onLoadResourceCallback] Receive multiple callbacks");
                    }
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerLoadResourceFromMediaData(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int startPosition = ZegoUtils.intValue((Number) call.argument("startPosition"));
            byte[] mediaData = call.argument("mediaData");

            mediaPlayer.loadResourceFromMediaData(mediaData, startPosition, new IZegoMediaPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    try {
                        result.success(resultMap);
                    } catch (Exception e) {
                        ZegoLog.error("[onLoadResourceCallback] Receive multiple callbacks");
                    }
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerLoadResourceWithPosition(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int startPosition = ZegoUtils.intValue((Number) call.argument("startPosition"));
            String path = call.argument("path");

            mediaPlayer.loadResourceWithPosition(path, startPosition, new IZegoMediaPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    try {
                        result.success(resultMap);
                    } catch (Exception e) {
                        ZegoLog.error("[onLoadResourceCallback] Receive multiple callbacks");
                    }
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
            long millisecond = ZegoUtils.longValue((Number) call.argument("millisecond"));
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
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            mediaPlayer.enableRepeat(enable);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableAux(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            mediaPlayer.enableAux(enable);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerMuteLocal(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));
            mediaPlayer.muteLocal(mute);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPlayerCanvas(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer == null) {
            result.success(null);
            return;
        }

        // Handle ZegoCanvas
        HashMap<String, Object> canvasMap = call.argument("canvas");

        if (canvasMap != null && !canvasMap.isEmpty()) {
            // Media video

            // This parameter is actually viewID when using PlatformView, and is actually textureID when using Texture render
            int viewID = ZegoUtils.intValue((Number) canvasMap.get("view"));
            ZegoViewMode viewMode = ZegoViewMode.getZegoViewMode(ZegoUtils.intValue((Number) canvasMap.get("viewMode")));
            int backgroundColor = ZegoUtils.intValue((Number) canvasMap.get("backgroundColor"));
            boolean alphaBlend = ZegoUtils.boolValue((Boolean) canvasMap.get("alphaBlend"));

            Object view;

            if (enablePlatformView) {

                // Render with PlatformView
                ZegoPlatformView platformView = ZegoPlatformViewFactory.getInstance().getPlatformView(viewID);

                if (platformView != null) {
                    view  = platformView.getSurfaceView();
                    if (alphaBlend) {
                        ((SurfaceView)view).getHolder().setFormat(PixelFormat.TRANSLUCENT);
                        ((SurfaceView)view).setZOrderOnTop(true);
                    }
                } else {
                    // Media video without creating the PlatformView in advance
                    // Need to invoke dart `createPlatformView` method in advance to create PlatformView and get viewID (PlatformViewID)
                    String errorMessage = String.format(Locale.ENGLISH, "The PlatformView for viewID:%d cannot be found, developer should call `createPlatformView` first and get the viewID", viewID);
                    ZegoLog.error("[mediaPlayerSetPlayerCanvas] %s", errorMessage);
                    result.error("mediaPlayerSetPlayerCanvas_No_PlatformView".toUpperCase(), errorMessage, null);
                    return;
                }
            } else {
                // Render with Texture
                ZegoTextureRenderer textureRenderer = ZegoTextureRendererController.getInstance().getTextureRenderer((long)viewID);

                if (textureRenderer != null) {
                    view = textureRenderer.getSurface();
                } else {
                    // Media video without creating TextureRenderer in advance
                    // Need to invoke dart `createCanvasView` method in advance to create TextureRenderer and get viewID (TextureID)
                    String errorMessage = String.format(Locale.ENGLISH, "The TextureRenderer for textureID:%d cannot be found, developer should call `createCanvasView` first and get the textureID", viewID);
                    ZegoLog.error("[mediaPlayerSetPlayerCanvas] %s", errorMessage);
                    result.error("mediaPlayerSetPlayerCanvas_No_TextureRenderer".toUpperCase(), errorMessage, null);
                    return;
                }
            }

            ZegoCanvas canvas = null;

            if (view != null) {
                canvas = new ZegoCanvas(view);
                canvas.viewMode = viewMode;
                canvas.backgroundColor = backgroundColor;
                canvas.alphaBlend = alphaBlend;

                // Mark the canvas is in use
                if (!enablePlatformView) {
                    ZegoTextureRendererController.getInstance().mediaPlayerCanvasInUse.put(index, canvas);
                }

                mediaPlayer.setPlayerCanvas(canvas);
            }
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            mediaPlayer.setVolume(volume);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPlayVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            mediaPlayer.setPlayVolume(volume);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPublishVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            mediaPlayer.setPublishVolume(volume);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetProgressInterval(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            long millisecond = ZegoUtils.longValue((Number) call.argument("millisecond"));
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
            int trackIndex = ZegoUtils.intValue((Number) call.argument("trackIndex"));
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

            ZegoMediaPlayerAudioChannel audioChannel = ZegoMediaPlayerAudioChannel.getZegoMediaPlayerAudioChannel(ZegoUtils.intValue((Number) call.argument("audioChannel")));

            ZegoVoiceChangerParam param = new ZegoVoiceChangerParam();
            param.pitch = ZegoUtils.floatValue(paramMap.get("pitch"));

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

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableAccurateSeek(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            HashMap<String, Object> configMap = call.argument("config");
            ZegoAccurateSeekConfig config = new ZegoAccurateSeekConfig();
            config.timeout = ZegoUtils.intValue((Number) configMap.get("timeout"));

            mediaPlayer.enableAccurateSeek(enable, config);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetNetWorkBufferThreshold(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int threshold = ZegoUtils.intValue((Number) call.argument("threshold"));

            mediaPlayer.setNetWorkBufferThreshold(threshold);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetNetWorkResourceMaxCache(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int time = ZegoUtils.intValue((Number) call.argument("time"));
            int size = ZegoUtils.intValue((Number) call.argument("size"));

            mediaPlayer.setNetWorkResourceMaxCache(time, size);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPlaySpeed(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            float speed = ZegoUtils.floatValue((Number) call.argument("speed"));

            mediaPlayer.setPlaySpeed(speed);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerTakeSnapshot(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {

            mediaPlayer.takeSnapshot(new IZegoMediaPlayerTakeSnapshotCallback() {
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
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetNetWorkResourceCache(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoNetWorkResourceCache netWorkResourceCache = mediaPlayer.getNetWorkResourceCache();
            HashMap<String, Object> resultMap = new HashMap<>();
            resultMap.put("time", netWorkResourceCache.time);
            resultMap.put("size", netWorkResourceCache.size);
            result.success(resultMap);
        } else {
            result.success(null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableFrequencySpectrumMonitor(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            int millisecond = ZegoUtils.intValue((Number) call.argument("millisecond"));

            mediaPlayer.enableFrequencySpectrumMonitor(enable, millisecond);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableSoundLevelMonitor(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            int millisecond = ZegoUtils.intValue((Number) call.argument("millisecond"));

            mediaPlayer.enableSoundLevelMonitor(enable, millisecond);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerLoadCopyrightedMusicResourceWithPosition(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int startPosition = ZegoUtils.intValue((Number) call.argument("startPosition"));
            String resourceID = call.argument("resourceID");

            mediaPlayer.loadCopyrightedMusicResourceWithPosition(resourceID, startPosition, new IZegoMediaPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    try {
                        result.success(resultMap);
                    } catch (Exception e) {
                        ZegoLog.error("[onLoadResourceCallback] Receive multiple callbacks");
                    }
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerClearView(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            mediaPlayer.clearView();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetActiveAudioChannel(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaPlayerAudioChannel audioChannel = ZegoMediaPlayerAudioChannel.getZegoMediaPlayerAudioChannel(ZegoUtils.intValue((Number) call.argument("audioChannel")));
            mediaPlayer.setActiveAudioChannel(audioChannel);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetAudioTrackMode(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaPlayerAudioTrackMode mode = ZegoMediaPlayerAudioTrackMode.getZegoMediaPlayerAudioTrackMode(ZegoUtils.intValue((Number) call.argument("mode")));
            mediaPlayer.setAudioTrackMode(mode);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetAudioTrackPublishIndex(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            int index_ = ZegoUtils.intValue((Number) call.argument("index_"));
            mediaPlayer.setAudioTrackPublishIndex(index_);
        }

        result.success(null);
    }


    @SuppressWarnings("unused")
    public static void mediaPlayerEnableAudioData(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            if (enable) {
                mediaPlayer.setAudioHandler(ZegoMediaPlayerAudioManager.getInstance());
            } else {
                mediaPlayer.setAudioHandler(null);
            }
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableVideoData(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            ZegoVideoFrameFormat format = ZegoVideoFrameFormat.getZegoVideoFrameFormat(ZegoUtils.intValue((Number) call.argument("format")));
            if (enable) {
                mediaPlayer.setVideoHandler(ZegoMediaPlayerVideoManager.getInstance(), format);
            } else {
                mediaPlayer.setVideoHandler(null, format);
            }
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableBlockData(MethodCall call, final Result result) {

         Integer index = call.argument("index");
         ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

         if (mediaPlayer != null) {
             boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
             int blockSize = ZegoUtils.intValue((Number) call.argument("blockSize"));
             if (enable) {
                 mediaPlayer.setBlockDataHandler(ZegoMediaPlayerBlockDataManager.getInstance(), blockSize);
             } else {
                 mediaPlayer.setBlockDataHandler(null, blockSize);
             }
         }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerLoadResourceWithConfig(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            HashMap<String, Object> resourceMap = call.argument("resource");

            ZegoMediaPlayerResource resource = new ZegoMediaPlayerResource();
            resource.resourceID = (String)resourceMap.get("resourceID");
            resource.startPosition = ZegoUtils.longValue((Number) resourceMap.get("startPosition"));
            resource.loadType = ZegoMultimediaLoadType.getZegoMultimediaLoadType(ZegoUtils.intValue((Number) resourceMap.get("loadType")));
            resource.filePath = (String)resourceMap.get("filePath");
            resource.alphaLayout = ZegoAlphaLayoutType.getZegoAlphaLayoutType(ZegoUtils.intValue((Number) resourceMap.get("alphaLayout")));
            byte[] memory = (byte[]) resourceMap.get("memory");
            ByteBuffer memoryByteBuffer =  ByteBuffer.allocateDirect(memory.length);
            resource.memory = memoryByteBuffer.put(memory);
            resource.memory.flip();
            resource.memoryLength = memory.length;

            mediaPlayer.loadResourceWithConfig(resource, new IZegoMediaPlayerLoadResourceCallback() {
                @Override
                public void onLoadResourceCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    try {
                        result.success(resultMap);
                    } catch (Exception e) {
                        ZegoLog.error("[onLoadResourceCallback] Receive multiple callbacks");
                    }
                }
            });
        } else {
            result.error("loadResourceWithConfig_Can_not_find_player".toUpperCase(), "Invoke `loadResourceWithConfig` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerUpdatePosition(MethodCall call, final Result result) {

         Integer index = call.argument("index");
         ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

         if (mediaPlayer != null) {
             float[] positionList = call.argument("position");
             mediaPlayer.updatePosition(positionList);
         }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetMediaInfo(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaPlayerMediaInfo info = mediaPlayer.getMediaInfo();
            HashMap<String, Object> resultMap = new HashMap<>();
            resultMap.put("width", info.width);
            resultMap.put("height", info.height);
            resultMap.put("frameRate", info.frameRate);
            result.success(resultMap);
        } else {
            result.success(null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetHttpHeader(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            HashMap<String, String> headers = call.argument("headers");
            mediaPlayer.setHttpHeader(headers);
        }
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableLiveAudioEffect(MethodCall call, final Result result) {
        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            ZegoLiveAudioEffectMode mode = ZegoLiveAudioEffectMode.getZegoLiveAudioEffectMode(ZegoUtils.intValue((Number) call.argument("mode")));

            mediaPlayer.enableLiveAudioEffect(enable, mode);
        }
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerSetPlayMediaStreamType(MethodCall call, final Result result) {
        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaStreamType streamType = ZegoMediaStreamType.getZegoMediaStreamType(ZegoUtils.intValue((Number) call.argument("streamType")));

            mediaPlayer.setPlayMediaStreamType(streamType);
        }
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerEnableLocalCache(MethodCall call, final Result result) {
        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            String cacheDir = call.argument("cacheDir");

            mediaPlayer.enableLocalCache(enable, cacheDir);
        }
        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void mediaPlayerGetPlaybackStatistics(MethodCall call, final Result result) {
        Integer index = call.argument("index");
        ZegoMediaPlayer mediaPlayer = mediaPlayerHashMap.get(index);

        if (mediaPlayer != null) {
            ZegoMediaPlayerStatisticsInfo info = mediaPlayer.getPlaybackStatistics();
            HashMap resultMap = new HashMap();
            resultMap.put("videoSourceFps", info.videoSourceFps);
            resultMap.put("videoDecodeFps", info.videoSourceFps);
            resultMap.put("videoRenderFps", info.videoSourceFps);
            resultMap.put("audioSourceFps", info.videoSourceFps);
            resultMap.put("audioDecodeFps", info.videoSourceFps);
            resultMap.put("audioRenderFps", info.videoSourceFps);
            result.success(resultMap);
        } else {
            result.success(null);
        }
    }

    /* MediaPlayer */

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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            String path = call.argument("path");

            HashMap<String, Object> configMap = call.argument("config");

            ZegoAudioEffectPlayConfig config = null;
            if (configMap != null && !configMap.isEmpty()) {
                config = new ZegoAudioEffectPlayConfig();
                config.playCount = ZegoUtils.intValue((Number) configMap.get("playCount"));
                config.isPublishOut = ZegoUtils.boolValue((Boolean) configMap.get("isPublishOut"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            long millisecond = ZegoUtils.longValue((Number) call.argument("millisecond"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setVolume(audioEffectID, volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetVolume_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetVolume` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetPlayVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setPlayVolume(audioEffectID, volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetPlayVolume_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetPlayVolume` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetPublishVolume(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setPublishVolume(audioEffectID, volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetPublishVolume_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetPublishVolume` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetVolumeAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setVolumeAll(volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetVolumeAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetVolumeAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetPlayVolumeAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setPlayVolumeAll(volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetPlayVolumeAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetPlayVolumeAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetPublishVolumeAll(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            audioEffectPlayer.setPublishVolumeAll(volume);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetPublishVolumeAll_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetPublishVolumeAll` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerGetTotalDuration(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
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
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            audioEffectPlayer.unloadResource(audioEffectID);

            result.success(null);

        } else {
            result.error("audioEffectPlayerUnloadResource_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerUnloadResource` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerSetPlaySpeed(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            float speed = ZegoUtils.floatValue((Number) call.argument("speed"));
            audioEffectPlayer.setPlaySpeed(audioEffectID, speed);

            result.success(null);

        } else {
            result.error("audioEffectPlayerSetPlaySpeed_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerSetPlaySpeed` but can't find specific player", null);
        }
    }

    @SuppressWarnings("unused")
    public static void audioEffectPlayerUpdatePosition(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoAudioEffectPlayer audioEffectPlayer = audioEffectPlayerHashMap.get(index);

        if (audioEffectPlayer != null) {
            int audioEffectID = ZegoUtils.intValue((Number) call.argument("audioEffectID"));
            float[] positionList = call.argument("position");
            audioEffectPlayer.updatePosition(audioEffectID, positionList);

            result.success(null);

        } else {
            result.error("audioEffectPlayerUpdatePosition_Can_not_find_player".toUpperCase(), "Invoke `audioEffectPlayerUpdatePosition` but can't find specific player", null);
        }
    }

    /* MediaDataPublisher */

    @SuppressWarnings("unused")
    public static void createMediaDataPublisher(MethodCall call, Result result) {
        HashMap<String, Object> configMap = call.argument("config");
        ZegoMediaDataPublisherConfig config = null;
        if (configMap != null && !configMap.isEmpty()) {
            config = new ZegoMediaDataPublisherConfig();
            config.channel = ZegoUtils.intValue((Number) configMap.get("channel"));
            config.mode = ZegoMediaDataPublisherMode.getZegoMediaDataPublisherMode(ZegoUtils.intValue((Number) configMap.get("mode")));
        }

        ZegoMediaDataPublisher publisher = ZegoExpressEngine.getEngine().createMediaDataPublisher(config);
        if (publisher != null) {
            int index = publisher.getIndex();
            publisher.setEventHandler(ZegoExpressEngineEventHandler.getInstance().mediaDataPublisherEventHandler);
            mediaDataPublisherHashMap.put(index, publisher);
            result.success(index);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyMediaDataPublisher(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            publisher.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyMediaDataPublisher(publisher);
            mediaDataPublisherHashMap.remove(index);
            result.success(null);
        } else {
            result.error("destroyMediaDataPublisher_Can_not_find_publisher".toUpperCase(), "Invoke `destroyMediaDataPublisher` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherAddMediaFilePath(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            String path = call.argument("path");
            boolean isClear = ZegoUtils.boolValue((Boolean) call.argument("isClear"));
            publisher.addMediaFilePath(path, isClear);
            result.success(null);
        } else {
            result.error("mediaDataPublisherAddMediaFilePath_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherAddMediaFilePath` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherGetCurrentDuration(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            long duration = publisher.getCurrentDuration();
            result.success(duration);
        } else {
            result.error("mediaDataPublisherGetCurrentDuration_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherGetCurrentDuration` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherGetTotalDuration(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            long duration = publisher.getTotalDuration();
            result.success(duration);
        } else {
            result.error("mediaDataPublisherGetTotalDuration_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherGetTotalDuration` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherReset(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            publisher.reset();
            result.success(null);
        } else {
            result.error("mediaDataPublisherReset_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherReset` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherSeekTo(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            long millisecond = ZegoUtils.longValue((Number) call.argument("millisecond"));
            publisher.seekTo(millisecond);
            result.success(null);
        } else {
            result.error("mediaDataPublisherSeekTo_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherSeekTo` but can't find specific publisher", null);
        }
    }

    @SuppressWarnings("unused")
    public static void mediaDataPublisherSetVideoSendDelayTime(MethodCall call, Result result) {
        Integer index = call.argument("index");
        ZegoMediaDataPublisher publisher = mediaDataPublisherHashMap.get(index);
        if (publisher != null) {
            String path = call.argument("path");
            int delayTime = ZegoUtils.intValue((Number) call.argument("delayTime"));
            publisher.setVideoSendDelayTime(delayTime);
            result.success(null);
        } else {
            result.error("mediaDataPublisherSetVideoSendDelayTime_Can_not_find_publisher".toUpperCase(), "Invoke `mediaDataPublisherSetVideoSendDelayTime` but can't find specific publisher", null);
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
        config.recordType = ZegoDataRecordType.getZegoDataRecordType(ZegoUtils.intValue((Number) configMap.get("recordType")));

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().startRecordingCapturedData(config, channel);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopRecordingCapturedData(MethodCall call, Result result) {

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().stopRecordingCapturedData(channel);

        result.success(null);
    }

    /* Range Audio */
    @SuppressWarnings("unused")
    public static void createRangeAudio(MethodCall call, Result result) {

        ZegoRangeAudio rangeAudio = ZegoExpressEngine.getEngine().createRangeAudio();

        if (rangeAudio != null) {
            rangeAudioInstance = rangeAudio;
            rangeAudio.setEventHandler(ZegoExpressEngineEventHandler.getInstance().rangeAudioEventHandler);
            result.success(0);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyRangeAudio(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            rangeAudioInstance.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyRangeAudio(rangeAudioInstance);
            rangeAudioInstance = null;
            result.success(null);
        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `destroyRangeAudio` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioEnableMicrophone(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            rangeAudioInstance.enableMicrophone(enable);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioEnableMicrophone` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioEnableSpatializer(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            rangeAudioInstance.enableSpatializer(enable);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioEnableSpatializer` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioEnableSpeaker(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
            rangeAudioInstance.enableSpeaker(enable);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioEnableSpeaker` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetAudioReceiveRange(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            ZegoReceiveRangeParam rangeParam = new ZegoReceiveRangeParam();
            HashMap<String, Object> paramMap = call.argument("param");
            if (paramMap != null) {
                rangeParam.min = ZegoUtils.floatValue((Double) paramMap.get("min"));
                rangeParam.max = ZegoUtils.floatValue((Double) paramMap.get("max"));
            }
            int ret = rangeAudioInstance.setAudioReceiveRange(rangeParam);

            result.success(ret);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetAudioReceiveRange` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetMode(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            ZegoRangeAudioMode mode = ZegoRangeAudioMode.getZegoRangeAudioMode(ZegoUtils.intValue((Number) call.argument("mode")));
            rangeAudioInstance.setRangeAudioMode(mode);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetMode` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetTeamID(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            String teamID = call.argument("teamID");
            rangeAudioInstance.setTeamID(teamID);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetTeamID` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioUpdateAudioSource(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            String userID = call.argument("userID");
            float[] positionList = call.argument("position");
            rangeAudioInstance.updateAudioSource(userID, positionList);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioUpdateAudioSource` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioUpdateSelfPosition(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            float[] position = call.argument("position");
            float[] axisForward = call.argument("axisForward");
            float[] axisRight = call.argument("axisRight");
            float[] axisUp = call.argument("axisUp");
            rangeAudioInstance.updateSelfPosition(position, axisForward, axisRight, axisUp);
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioUpdateSelfPosition` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioMuteUser(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            String userID = call.argument("userID");
            boolean mute = ZegoUtils.boolValue((Boolean) call.argument("mute"));
            rangeAudioInstance.muteUser(userID, mute);
            
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioMuteUser` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetPositionUpdateFrequency(MethodCall call, Result result) {
        
        if (rangeAudioInstance != null) {
            int frequency = ZegoUtils.intValue((Number) call.argument("frequency"));
            rangeAudioInstance.setPositionUpdateFrequency(frequency);
            
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetPositionUpdateFrequency` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetRangeAudioVolume(MethodCall call, Result result) {
        
        if (rangeAudioInstance != null) {
            int volume = ZegoUtils.intValue((Number) call.argument("volume"));
            rangeAudioInstance.setRangeAudioVolume(volume);
            
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetRangeAudioVolume` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetStreamVocalRange(MethodCall call, Result result) {
        
        if (rangeAudioInstance != null) {
            String streamID = call.argument("streamID");
            ZegoVocalRangeParam rangeParam = new ZegoVocalRangeParam();
            HashMap<String, Object> paramMap = call.argument("param");
            if (paramMap != null) {
                rangeParam.min = ZegoUtils.floatValue((Double) paramMap.get("min"));
                rangeParam.max = ZegoUtils.floatValue((Double) paramMap.get("max"));
            }
            int ret = rangeAudioInstance.setStreamVocalRange(streamID, rangeParam);
            
            result.success(ret);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioUpdateStreamPosition(MethodCall call, Result result) {
        
        if (rangeAudioInstance != null) {
            String streamID = call.argument("streamID");
            float[] position = call.argument("position");
            rangeAudioInstance.updateStreamPosition(streamID, position);
            
            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioUpdateStreamPosition` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void rangeAudioSetRangeAudioCustomMode(MethodCall call, Result result) {

        if (rangeAudioInstance != null) {
            ZegoRangeAudioSpeakMode speakMode = ZegoRangeAudioSpeakMode.getZegoRangeAudioSpeakMode(ZegoUtils.intValue((Number) call.argument("speakMode")));
            ZegoRangeAudioListenMode listenMode = ZegoRangeAudioListenMode.getZegoRangeAudioListenMode(ZegoUtils.intValue((Number) call.argument("listenMode")));
            rangeAudioInstance.setRangeAudioCustomMode(speakMode, listenMode);

            result.success(null);

        } else {
            result.error("rangeAudio_Can_not_find_instance".toUpperCase(), "Invoke `rangeAudioSetStreamVocalRange` but can't find specific instance", null);
        }
    }

    /*  real time sequential data manager */

    @SuppressWarnings("unused")
    public static void createRealTimeSequentialDataManager(MethodCall call, Result result) {

        String roomID = call.argument("roomID");

        ZegoRealTimeSequentialDataManager manager = ZegoExpressEngine.getEngine().createRealTimeSequentialDataManager(roomID);

        if (manager != null)
        {
            int index = manager.getIndex();

            manager.setEventHandler(ZegoExpressEngineEventHandler.getInstance().realTimeSequentialDataEventHandler);
            realTimeSequentialDataManagerHashMap.put(index, manager);

            result.success(index);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyRealTimeSequentialDataManager(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            manager.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyRealTimeSequentialDataManager(manager);
        } 

        realTimeSequentialDataManagerHashMap.remove(index);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void dataManagerSendRealTimeSequentialData(MethodCall call, final Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            byte[] data = call.argument("data");
            String streamID = call.argument("streamID");

            manager.sendRealTimeSequentialData(data, streamID, new IZegoRealTimeSequentialDataSentCallback() {
                @Override
                public void onRealTimeSequentialDataSent(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        }
    }

    @SuppressWarnings("unused")
    public static void dataManagerStartBroadcasting(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            String streamID = call.argument("streamID");
            manager.startBroadcasting(streamID);
            result.success(null);

        } else {
            result.error("realTimeSequentialDataManager_Can_not_find_instance".toUpperCase(), "Invoke `dataManagerStartBroadcasting` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void dataManagerStartSubscribing(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            String streamID = call.argument("streamID");
            manager.startSubscribing(streamID);
            result.success(null);

        } else {
            result.error("realTimeSequentialDataManager_Can_not_find_instance".toUpperCase(), "Invoke `dataManagerStartSubscribing` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void dataManagerStopBroadcasting(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            String streamID = call.argument("streamID");
            manager.stopBroadcasting(streamID);
            result.success(null);

        } else {
            result.error("realTimeSequentialDataManager_Can_not_find_instance".toUpperCase(), "Invoke `dataManagerStopBroadcasting` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void dataManagerStopSubscribing(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoRealTimeSequentialDataManager manager = realTimeSequentialDataManagerHashMap.get(index);

        if (manager != null) {
            String streamID = call.argument("streamID");
            manager.stopSubscribing(streamID);
            result.success(null);

        } else {
            result.error("realTimeSequentialDataManager_Can_not_find_instance".toUpperCase(), "Invoke `dataManagerStopSubscribing` but can't find specific instance", null);
        }
    }

    /*  Utilities */

    @SuppressWarnings("unused")
    public static void startPerformanceMonitor(MethodCall call, Result result) {

        int millisecond = ZegoUtils.intValue((Number) call.argument("millisecond"));

        ZegoExpressEngine.getEngine().startPerformanceMonitor(millisecond);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopPerformanceMonitor(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopPerformanceMonitor();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void startNetworkProbe(MethodCall call, final Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        if (configMap == null || configMap.isEmpty()) {
            result.error("startNetworkProbe_Null_Config".toUpperCase(), "[startNetworkProbe] Null config", null);
            return;
        }

        ZegoNetworkProbeConfig config = new ZegoNetworkProbeConfig();
        config.enableTraceroute = ZegoUtils.boolValue((Boolean) configMap.get("enableTraceroute"));

        ZegoExpressEngine.getEngine().startNetworkProbe(config, new IZegoNetworkProbeResultCallback() {
            @Override
            public void onNetworkProbeResult(int i, ZegoNetworkProbeResult zegoNetworkProbeResult) {
                HashMap<String, Object> resultMap = new HashMap<>();

                HashMap<String, Object> httpProbeResultMap = new HashMap<>();
                HashMap<String, Object> tcpProbeResultMap = new HashMap<>();
                HashMap<String, Object> udpProbeResultMap = new HashMap<>();
                HashMap<String, Object> tracerouteResultMap = new HashMap<>();

                if (zegoNetworkProbeResult.httpProbeResult != null) {
                    httpProbeResultMap.put("errorCode", zegoNetworkProbeResult.httpProbeResult.errorCode);
                    httpProbeResultMap.put("requestCostTime", zegoNetworkProbeResult.httpProbeResult.requestCostTime);
                    resultMap.put("httpProbeResult", httpProbeResultMap);
                }

                if (zegoNetworkProbeResult.tcpProbeResult != null) {
                    tcpProbeResultMap.put("errorCode", zegoNetworkProbeResult.tcpProbeResult.errorCode);
                    tcpProbeResultMap.put("connectCostTime", zegoNetworkProbeResult.tcpProbeResult.connectCostTime);
                    tcpProbeResultMap.put("rtt", zegoNetworkProbeResult.tcpProbeResult.rtt);
                    resultMap.put("tcpProbeResult", tcpProbeResultMap);
                }

                if (zegoNetworkProbeResult.udpProbeResult != null) {
                    udpProbeResultMap.put("errorCode", zegoNetworkProbeResult.udpProbeResult.errorCode);
                    udpProbeResultMap.put("rtt", zegoNetworkProbeResult.udpProbeResult.rtt);
                    resultMap.put("udpProbeResult", udpProbeResultMap);
                }

                if (zegoNetworkProbeResult.tracerouteResult != null) {
                    tracerouteResultMap.put("errorCode", zegoNetworkProbeResult.tracerouteResult.errorCode);
                    tracerouteResultMap.put("tracerouteCostTime", zegoNetworkProbeResult.tracerouteResult.tracerouteCostTime);
                    resultMap.put("tracerouteResult", tracerouteResultMap);
                }

                result.success(resultMap);
            }
        });
    }

    @SuppressWarnings("unused")
    public static void stopNetworkProbe(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopNetworkProbe();

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
        config.testUplink = ZegoUtils.boolValue((Boolean) configMap.get("testUplink"));
        config.expectedUplinkBitrate = ZegoUtils.intValue((Number) call.argument("expectedUplinkBitrate"));
        config.testDownlink = ZegoUtils.boolValue((Boolean) configMap.get("testDownlink"));
        config.expectedDownlinkBitrate = ZegoUtils.intValue((Number) call.argument("expectedDownlinkBitrate"));

        int interval = ZegoUtils.intValue((Number)call.argument("interval"));
        ZegoExpressEngine.getEngine().startNetworkSpeedTest(config, interval);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopNetworkSpeedTest(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopNetworkSpeedTest();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void getNetworkTimeInfo(MethodCall call, Result result) {

        ZegoNetworkTimeInfo info = ZegoExpressEngine.getEngine().getNetworkTimeInfo();
        
        HashMap<String, Object> resultMap = new HashMap<>();
        resultMap.put("timestamp", info.timestamp);
        resultMap.put("maxDeviation", info.maxDeviation);
        result.success(resultMap);
    }

    @SuppressWarnings("unused")
    public static void startDumpData(MethodCall call, Result result) {

        ZegoDumpDataConfig config = new ZegoDumpDataConfig();
        config.dataType = ZegoDumpDataType.getZegoDumpDataType(ZegoUtils.intValue((Number) call.argument("dataType")));
        ZegoExpressEngine.getEngine().startDumpData(config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopDumpData(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopDumpData();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void uploadDumpData(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().uploadDumpData();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void removeDumpData(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().removeDumpData();

        result.success(null);
    }

    /* Copyrighted Music */
    @SuppressWarnings("unused")
    public static void createCopyrightedMusic(MethodCall call, Result result) {

        ZegoCopyrightedMusic copyrightedMusic =  ZegoExpressEngine.getEngine().createCopyrightedMusic();
        if (copyrightedMusic != null) {
            copyrightedMusicInstance = copyrightedMusic;
            copyrightedMusic.setEventHandler(ZegoExpressEngineEventHandler.getInstance().copyrightedMusicEventHandler);
            result.success(0);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyCopyrightedMusic(MethodCall call, Result result) {
        
        if (copyrightedMusicInstance != null) {
            copyrightedMusicInstance.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyCopyrightedMusic(copyrightedMusicInstance);
            copyrightedMusicInstance = null;
            result.success(null);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `destroyCopyrightedMusic` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicClearCache(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            copyrightedMusicInstance.clearCache();
            result.success(null);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicClearCache` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicDownload(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            copyrightedMusicInstance.download(resourceID, new IZegoCopyrightedMusicDownloadCallback() {
                @Override
                public void onDownloadCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicDownload` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicCancelDownload(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            copyrightedMusicInstance.cancelDownload(resourceID);
            result.success(null);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicCancelDownload` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetAverageScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int averageScore = copyrightedMusicInstance.getAverageScore(resourceID);
            result.success(averageScore);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetAverageScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetCacheSize(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            long cacheSize = copyrightedMusicInstance.getCacheSize();
            result.success(cacheSize);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetCacheSize` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetCurrentPitch(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int currentPitch = copyrightedMusicInstance.getCurrentPitch(resourceID);
            result.success(currentPitch);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetCurrentPitch` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetDuration(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            long duration = copyrightedMusicInstance.getDuration(resourceID);
            result.success(duration);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetDuration` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetKrcLyricByToken(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String krcToken = call.argument("krcToken");
            copyrightedMusicInstance.getKrcLyricByToken(krcToken, new IZegoCopyrightedMusicGetKrcLyricByTokenCallback() {

                @Override
                public void onGetKrcLyricByTokenCallback(int errorCode, String lyrics) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("lyrics", lyrics);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetKrcLyricByToken` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetLrcLyric(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String songID = call.argument("songID");

            if (call.argument("vendorID") == null) {
                copyrightedMusicInstance.getLrcLyric(songID, new IZegoCopyrightedMusicGetLrcLyricCallback() {

                    @Override
                    public void onGetLrcLyricCallback(int errorCode, String lyrics) {
                        HashMap<String, Object> resultMap = new HashMap<>();
                        resultMap.put("errorCode", errorCode);
                        resultMap.put("lyrics", lyrics);
                        result.success(resultMap);
                    }
                });
            } else {
                int vendorID = ZegoUtils.intValue((Number) call.argument("vendorID"));
                copyrightedMusicInstance.getLrcLyric(songID, ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(vendorID),  new IZegoCopyrightedMusicGetLrcLyricCallback() {

                    @Override
                    public void onGetLrcLyricCallback(int errorCode, String lyrics) {
                        HashMap<String, Object> resultMap = new HashMap<>();
                        resultMap.put("errorCode", errorCode);
                        resultMap.put("lyrics", lyrics);
                        result.success(resultMap);
                    }
                });
            }
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetLrcLyric` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetLrcLyricWithConfig(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicGetLyricConfig config = new ZegoCopyrightedMusicGetLyricConfig();
            config.songID = configMap.get("songID").toString();
            config.vendorID =ZegoUtils.intValue((Number) configMap.get("vendorID"));

            copyrightedMusicInstance.getLrcLyric(config, new IZegoCopyrightedMusicGetLrcLyricCallback() {

                    @Override
                    public void onGetLrcLyricCallback(int errorCode, String lyrics) {
                        HashMap<String, Object> resultMap = new HashMap<>();
                        resultMap.put("errorCode", errorCode);
                        resultMap.put("lyrics", lyrics);
                        result.success(resultMap);
                    }
                });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetLrcLyricWithConfig` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetMusicByToken(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String shareToken = call.argument("shareToken");
            copyrightedMusicInstance.getMusicByToken(shareToken, new IZegoCopyrightedMusicGetMusicByTokenCallback() {

                @Override
                public void onGetMusicByTokenCallback(int errorCode, String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetMusicByToken` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetPreviousScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int previousScore = copyrightedMusicInstance.getPreviousScore(resourceID);
            result.success(previousScore);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetPreviousScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetStandardPitch(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            copyrightedMusicInstance.getStandardPitch(resourceID, new IZegoCopyrightedMusicGetStandardPitchCallback() {

                @Override
                public void onGetStandardPitchCallback(int errorCode, String pitch) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("pitch", pitch);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetStandardPitch` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetTotalScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int totalScore = copyrightedMusicInstance.getTotalScore(resourceID);
            result.success(totalScore);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetTotalScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicInitCopyrightedMusic(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicConfig config = new ZegoCopyrightedMusicConfig();

            HashMap<String, Object> userMap = (HashMap<String, Object>)configMap.get("user");
            String userID = userMap.get("userID").toString();
            String userName = userMap.get("userName").toString();
            ZegoUser user = new ZegoUser(userID, userName);
            config.user = user;
            copyrightedMusicInstance.initCopyrightedMusic(config, new IZegoCopyrightedMusicInitCallback() {

                @Override
                public void onInitCallback(int errorCode) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicInitCopyrightedMusic` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicPauseScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int score = copyrightedMusicInstance.pauseScore(resourceID);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicPauseScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicQueryCache(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String songID = call.argument("songID");
            ZegoCopyrightedMusicType type = ZegoCopyrightedMusicType.getZegoCopyrightedMusicType(ZegoUtils.intValue((Number) call.argument("type")));

            boolean isCache = false;
            if (call.argument("vendorID") == null) {
                isCache = copyrightedMusicInstance.queryCache(songID, type);
            } else {
                int vendorID = ZegoUtils.intValue((Number) call.argument("vendorID"));
                isCache = copyrightedMusicInstance.queryCache(songID, type, ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(vendorID));
            }

            result.success(isCache);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicQueryCache` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicQueryCacheWithConfig(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicQueryCacheConfig config = new ZegoCopyrightedMusicQueryCacheConfig();

            if (configMap != null) {
                config.songID = configMap.get("songID").toString();
                config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
                config.resourceType = ZegoCopyrightedMusicResourceType.getZegoCopyrightedMusicResourceType(ZegoUtils.intValue((Number) configMap.get("resourceType")));
                config.resourceQualityType = ZegoCopyrightedMusicResourceQualityType.getZegoCopyrightedMusicResourceQualityType(ZegoUtils.intValue((Number) configMap.get("resourceQualityType")));
            }

            boolean isCache = copyrightedMusicInstance.queryCache(config);
            result.success(isCache);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicQueryCacheWithConfig` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicQueryCacheWithConfigV2(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicQueryCacheConfigV2 config = new ZegoCopyrightedMusicQueryCacheConfigV2();

            if (configMap != null) {
                config.songID = configMap.get("songID").toString();
                config.vendorID = ZegoUtils.intValue((Number) configMap.get("vendorID"));
                config.resourceType = ZegoUtils.intValue((Number) configMap.get("resourceType"));
                config.resourceQualityType = ZegoUtils.intValue((Number) configMap.get("resourceQualityType"));
            }

            boolean isCache = copyrightedMusicInstance.queryCache(config);
            result.success(isCache);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicQueryCacheWithConfigV2` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicRequestAccompaniment(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicRequestConfig config = new ZegoCopyrightedMusicRequestConfig();
            config.songID = configMap.get("songID").toString();
            config.mode = ZegoCopyrightedMusicBillingMode.getZegoCopyrightedMusicBillingMode(ZegoUtils.intValue((Number) configMap.get("mode")));
            config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
            config.roomID = configMap.get("roomID").toString();
            config.masterID = configMap.get("masterID").toString();
            config.sceneID = ZegoUtils.intValue((Number) configMap.get("sceneID"));
            copyrightedMusicInstance.requestAccompaniment(config, new IZegoCopyrightedMusicRequestAccompanimentCallback() {

                @Override
                public void onRequestAccompanimentCallback(int errorCode, String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicRequestAccompaniment` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicRequestAccompanimentClip(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicRequestConfig config = new ZegoCopyrightedMusicRequestConfig();
            config.songID = configMap.get("songID").toString();
            config.mode = ZegoCopyrightedMusicBillingMode.getZegoCopyrightedMusicBillingMode(ZegoUtils.intValue((Number) configMap.get("mode")));
            config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
            config.roomID = configMap.get("roomID").toString();
            config.masterID = configMap.get("masterID").toString();
            config.sceneID = ZegoUtils.intValue((Number) configMap.get("sceneID"));
            copyrightedMusicInstance.requestAccompanimentClip(config, new IZegoCopyrightedMusicRequestAccompanimentClipCallback() {

                @Override
                public void onRequestAccompanimentClipCallback(int errorCode, String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicRequestAccompanimentClip` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicRequestSong(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicRequestConfig config = new ZegoCopyrightedMusicRequestConfig();
            config.songID = configMap.get("songID").toString();
            config.mode = ZegoCopyrightedMusicBillingMode.getZegoCopyrightedMusicBillingMode(ZegoUtils.intValue((Number) configMap.get("mode")));
            config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
            config.roomID = configMap.get("roomID").toString();
            config.masterID = configMap.get("masterID").toString();
            config.sceneID = ZegoUtils.intValue((Number) configMap.get("sceneID"));
            copyrightedMusicInstance.requestSong(config, new IZegoCopyrightedMusicRequestSongCallback() {

                @Override
                public void onRequestSongCallback(int errorCode, String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicRequestSong` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicResetScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int score = copyrightedMusicInstance.resetScore(resourceID);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicResetScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicResumeScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int score = copyrightedMusicInstance.resumeScore(resourceID);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicResumeScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicSendExtendedRequest(MethodCall call, final Result result) {

        if (copyrightedMusicInstance != null) {
            String command = call.argument("command");
            String params = call.argument("params");
            copyrightedMusicInstance.sendExtendedRequest(command, params, new IZegoCopyrightedMusicSendExtendedRequestCallback() {

                @Override
                public void onSendExtendedRequestCallback(int errorCode, String command, String result_) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("command", command);
                    resultMap.put("result", result_);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicSendExtendedRequest` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicSetScoringLevel(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            int level = ZegoUtils.intValue((Number) call.argument("level"));
            copyrightedMusicInstance.setScoringLevel(level);
            result.success(null);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicSetScoringLevel` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicStartScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int pitchValueInterval = ZegoUtils.intValue((Number) call.argument("pitchValueInterval"));
            int score = copyrightedMusicInstance.startScore(resourceID, pitchValueInterval);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicStartScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicStopScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int score = copyrightedMusicInstance.stopScore(resourceID);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicStopScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetFullScore(MethodCall call, Result result) {

        if (copyrightedMusicInstance != null) {
            String resourceID = call.argument("resourceID");
            int score = copyrightedMusicInstance.getFullScore(resourceID);
            result.success(score);
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetFullScore` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetSharedResource(MethodCall call,final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicGetSharedConfig config = new ZegoCopyrightedMusicGetSharedConfig();
            config.songID = configMap.get("songID").toString();
            config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
            config.roomID = configMap.get("roomID").toString();
            ZegoCopyrightedMusicResourceType type = ZegoCopyrightedMusicResourceType.getZegoCopyrightedMusicResourceType(ZegoUtils.intValue((Number) call.argument("type")));
            copyrightedMusicInstance.getSharedResource(config, type, new IZegoCopyrightedMusicGetSharedResourceCallback() {

                @Override
                public void onGetSharedResourceCallback(int errorCode,String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetSharedResource` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicGetSharedResourceV2(MethodCall call,final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicGetSharedConfigV2 config = new ZegoCopyrightedMusicGetSharedConfigV2();
            config.songID = configMap.get("songID").toString();
            config.vendorID = ZegoUtils.intValue((Number) configMap.get("vendorID"));
            config.roomID = configMap.get("roomID").toString();
            config.resourceType = ZegoUtils.intValue((Number) configMap.get("resourceType"));
            copyrightedMusicInstance.getSharedResource(config, new IZegoCopyrightedMusicGetSharedResourceCallback() {

                @Override
                public void onGetSharedResourceCallback(int errorCode,String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicGetSharedResourceV2` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicRequestResource(MethodCall call,final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicRequestConfig config = new ZegoCopyrightedMusicRequestConfig();
            config.songID = configMap.get("songID").toString();
            config.mode = ZegoCopyrightedMusicBillingMode.getZegoCopyrightedMusicBillingMode(ZegoUtils.intValue((Number) configMap.get("mode")));
            config.vendorID = ZegoCopyrightedMusicVendorID.getZegoCopyrightedMusicVendorID(ZegoUtils.intValue((Number) configMap.get("vendorID")));
            config.roomID = configMap.get("roomID").toString();
            config.masterID = configMap.get("masterID").toString();
            config.sceneID = ZegoUtils.intValue((Number) configMap.get("sceneID"));

            ZegoCopyrightedMusicResourceType type = ZegoCopyrightedMusicResourceType.getZegoCopyrightedMusicResourceType(ZegoUtils.intValue((Number) call.argument("type")));
            copyrightedMusicInstance.requestResource(config, type, new IZegoCopyrightedMusicRequestResourceCallback() {

                @Override
                public void onRequestResourceCallback(int errorCode,String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicRequestResource` but can't find specific instance", null);
        }
    }

    @SuppressWarnings("unused")
    public static void copyrightedMusicRequestResourceV2(MethodCall call,final Result result) {

        if (copyrightedMusicInstance != null) {
            HashMap<String, Object> configMap = call.argument("config");
            ZegoCopyrightedMusicRequestConfigV2 config = new ZegoCopyrightedMusicRequestConfigV2();
            config.songID = configMap.get("songID").toString();
            config.mode = ZegoUtils.intValue((Number) configMap.get("mode"));
            config.vendorID = ZegoUtils.intValue((Number) configMap.get("vendorID"));
            config.roomID = configMap.get("roomID").toString();
            config.masterID = configMap.get("masterID").toString();
            config.sceneID = ZegoUtils.intValue((Number) configMap.get("sceneID"));
            config.resourceType = ZegoUtils.intValue((Number) configMap.get("resourceType"));

            ZegoCopyrightedMusicResourceType type = ZegoCopyrightedMusicResourceType.getZegoCopyrightedMusicResourceType(ZegoUtils.intValue((Number) call.argument("type")));
            copyrightedMusicInstance.requestResource(config, new IZegoCopyrightedMusicRequestResourceCallback() {

                @Override
                public void onRequestResourceCallback(int errorCode,String resource) {
                    HashMap<String, Object> resultMap = new HashMap<>();
                    resultMap.put("errorCode", errorCode);
                    resultMap.put("resource", resource);
                    result.success(resultMap);
                }
            });
        } else {
            result.error("copyrightedMusic_Can_not_find_instance".toUpperCase(), "Invoke `copyrightedMusicRequestResource` but can't find specific instance", null);
        }
    }

    /* Screen Capture */
    @SuppressWarnings("unused")
    public static void startCaptureScreenCaptureSource(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        ZegoScreenCaptureConfig config = new ZegoScreenCaptureConfig();

        if (configMap != null) {
            config.captureAudio = ZegoUtils.boolValue((Boolean) configMap.get("captureAudio"));
            config.captureVideo = ZegoUtils.boolValue((Boolean) configMap.get("captureVideo"));

            HashMap<String, Object> audioParamMap = (HashMap<String, Object>) configMap.get("audioParam");
            if (audioParamMap != null) {
                ZegoAudioFrameParam audioFrameParam = new ZegoAudioFrameParam();
                audioFrameParam.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) audioParamMap.get("sampleRate")));
                audioFrameParam.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) audioParamMap.get("channel")));
                config.audioParam = audioFrameParam;
            }
        }
        ZegoExpressEngine.getEngine().startScreenCapture(config);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void stopCaptureScreenCaptureSource(MethodCall call, Result result) {

        ZegoExpressEngine.getEngine().stopScreenCapture();

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void updateScreenCaptureConfigScreenCaptureSource(MethodCall call, Result result) {

        HashMap<String, Object> configMap = call.argument("config");
        ZegoScreenCaptureConfig config = new ZegoScreenCaptureConfig();

        if (configMap != null) {
            config.captureAudio = ZegoUtils.boolValue((Boolean) configMap.get("captureAudio"));
            config.captureVideo = ZegoUtils.boolValue((Boolean) configMap.get("captureVideo"));

            HashMap<String, Object> audioParamMap = (HashMap<String, Object>) configMap.get("audioParam");
            if (audioParamMap != null) {
                ZegoAudioFrameParam audioFrameParam = new ZegoAudioFrameParam();
                audioFrameParam.sampleRate = ZegoAudioSampleRate.getZegoAudioSampleRate(ZegoUtils.intValue((Number) audioParamMap.get("sampleRate")));
                audioFrameParam.channel = ZegoAudioChannel.getZegoAudioChannel(ZegoUtils.intValue((Number) audioParamMap.get("channel")));
                config.audioParam = audioFrameParam;
            }
        }
        ZegoExpressEngine.getEngine().updateScreenCaptureConfig(config);

        result.success(null);
    }

    /* AI Voice Changer */

    public static ZegoAIVoiceChanger getAIVoiceChanger(Integer index) {
        return aiVoiceChangerHashMap.get(index);
    }

    @SuppressWarnings("unused")
    public static void createAIVoiceChanger(MethodCall call, Result result) {

        ZegoAIVoiceChanger aiVoiceChanger = ZegoExpressEngine.getEngine().createAIVoiceChanger();

        if (aiVoiceChanger != null) {
            int index = aiVoiceChanger.getIndex();

            aiVoiceChanger.setEventHandler(ZegoExpressEngineEventHandler.getInstance().aiVoiceChangerEventHandler);
            aiVoiceChangerHashMap.put(index, aiVoiceChanger);

            result.success(index);
        } else {
            result.success(-1);
        }
    }

    @SuppressWarnings("unused")
    public static void destroyAIVoiceChanger(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAIVoiceChanger aiVoiceChanger = aiVoiceChangerHashMap.get(index);

        if (aiVoiceChanger != null) {
            aiVoiceChanger.setEventHandler(null);
            ZegoExpressEngine.getEngine().destroyAIVoiceChanger(aiVoiceChanger);
        }

        aiVoiceChangerHashMap.remove(index);

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void aiVoiceChangerGetSpeakerList(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAIVoiceChanger aiVoiceChanger = aiVoiceChangerHashMap.get(index);

        if (aiVoiceChanger != null) {
            aiVoiceChanger.getSpeakerList();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void aiVoiceChangerInitEngine(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAIVoiceChanger aiVoiceChanger = aiVoiceChangerHashMap.get(index);

        if (aiVoiceChanger != null) {
            aiVoiceChanger.initEngine();
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void aiVoiceChangerSetSpeaker(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAIVoiceChanger aiVoiceChanger = aiVoiceChangerHashMap.get(index);

        if (aiVoiceChanger != null) {
            int speakerID = ZegoUtils.intValue((Number) call.argument("speakerID"));
            aiVoiceChanger.setSpeaker(speakerID);
        }

        result.success(null);
    }

    @SuppressWarnings("unused")
    public static void aiVoiceChangerUpdate(MethodCall call, Result result) {

        Integer index = call.argument("index");
        ZegoAIVoiceChanger aiVoiceChanger = aiVoiceChangerHashMap.get(index);

        if (aiVoiceChanger != null) {
            aiVoiceChanger.update();
        }

        result.success(null);
    }
    @SuppressWarnings("unused")
    public static void enableColorEnhancement(MethodCall call, Result result) {
        boolean enable = ZegoUtils.boolValue((Boolean) call.argument("enable"));
        HashMap<String, Object> paramsMap = call.argument("params");

        ZegoColorEnhancementParams p = new ZegoColorEnhancementParams();
        p.intensity = ZegoUtils.floatValue((Double)paramsMap.get("intensity"));
        p.skinToneProtectionLevel = ZegoUtils.floatValue((Double)paramsMap.get("skinToneProtectionLevel"));
        p.lipColorProtectionLevel = ZegoUtils.floatValue((Double)paramsMap.get("lipColorProtectionLevel"));

        ZegoPublishChannel channel = ZegoPublishChannel.getZegoPublishChannel(ZegoUtils.intValue((Number) call.argument("channel")));

        ZegoExpressEngine.getEngine().enableColorEnhancement(enable, p, channel);

        result.success(null);
    }

    /* PlatformView Utils */

    @SuppressWarnings("unused")
    public static void destroyPlatformView(MethodCall call, Result result) {

        int viewID = ZegoUtils.intValue((Number) call.argument("viewID"));

        Boolean state = ZegoPlatformViewFactory.getInstance().destroyPlatformView(viewID);

        ZegoLog.log("[destroyPlatformView][Result] viewID: %d, success: %s", viewID, state ? "true" : "false");

        result.success(state);
    }


    /* TextureRenderer Utils */

    @SuppressWarnings("unused")
    public static void createTextureRenderer(MethodCall call, Result result) {

        int width = ZegoUtils.intValue((Number) call.argument("width"));
        int height = ZegoUtils.intValue((Number) call.argument("height"));

        Long textureID = ZegoTextureRendererController.getInstance().createTextureRenderer(textureRegistry.createSurfaceTexture(), width, height);

        ZegoLog.log("[createTextureRenderer][Result] w: %d, h: %d, textureID: %d", width, height, textureID);

        result.success(textureID);
    }

    @SuppressWarnings("unused")
    public static void updateTextureRendererSize(MethodCall call, Result result) {

        Long textureID = ZegoUtils.longValue((Number) call.argument("textureID"));
        int width = ZegoUtils.intValue((Number) call.argument("width"));
        int height = ZegoUtils.intValue((Number) call.argument("height"));

        Boolean state = ZegoTextureRendererController.getInstance().updateTextureRendererSize(textureID, width, height);

        ZegoLog.log("[updateTextureRendererSize][Result] w: %d, h: %d, textureID: %d, success: %s", width, height, textureID, state ? "true" : "false");

        result.success(state);
    }

    @SuppressWarnings("unused")
    public static void destroyTextureRenderer(MethodCall call, Result result) {

        Long textureID = ZegoUtils.longValue((Number) call.argument("textureID"));

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

    private static float[] converFloatArray(ArrayList<Double> list) {
        float[] position = new float[list.size()];
        int i = 0;
        for (Double f : list) {
            position[i++] = (float) (f != null ? f : Double.NaN); // Or whatever default you want.
        }
        return position;
    }

    @SuppressWarnings("unused")
    public static void setPluginVersion(MethodCall call, Result result) {
        String version = call.argument("version");
        ZegoLog.log("*** Plugin Version: %s", version);
        
        result.success(null);
    }

    private static void reportPluginInfo() {

        if (pluginReported) { return; }

        pluginReported = true;

        HashMap<String, String> advancedConfigMap = new HashMap<>();
        advancedConfigMap.put("thirdparty_framework_info", "flutter");
        
        ZegoEngineConfig configObject = new ZegoEngineConfig();
        configObject.advancedConfig = advancedConfigMap;

        ZegoExpressEngine.setEngineConfig(configObject);
    }
}
