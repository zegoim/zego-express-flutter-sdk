//
//  ZegoExpressEngineEventHandler.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/4/9.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoExpressEngineEventHandler.h"
#import "ZegoLog.h"
#import "ZegoTextureRendererController.h"
#import "ZegoExpressEngineMethodHandler.h"
#import <objc/message.h>

#define GUARD_SINK if(!sink){ZGError(@"[%s] FlutterEventSink is nil", __FUNCTION__);}

@implementation ZegoExpressEngineEventHandler

+ (instancetype)sharedInstance {
    static ZegoExpressEngineEventHandler *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoExpressEngineEventHandler alloc] init];
    });
    return instance;
}

#pragma mark - ZegoEventHandler

- (void)onDebugError:(int)errorCode funcName:(NSString *)funcName info:(NSString *)info {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onDebugError] errorCode: %d, funcName: %@, info: %@", errorCode, funcName, info);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onDebugError",
            @"errorCode": @(errorCode),
            @"funcName": funcName,
            @"info": info
        });
    }
}

- (void)onApiCalledResult:(int)errorCode funcName:(NSString *)funcName info:(NSString *)info {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onApiCalledResult] errorCode: %d, funcName: %@, info: %@", errorCode, funcName, info);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onApiCalledResult",
            @"errorCode": @(errorCode),
            @"funcName": funcName,
            @"info": info
        });
    }
}

- (void)onFatalError:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onFatalError] errorCode: %d", errorCode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onFatalError",
            @"errorCode": @(errorCode)
        });
    }
}

- (void)onEngineStateUpdate:(ZegoEngineState)state {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onEngineStateUpdate] state: %d", (int)state);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onEngineStateUpdate",
            @"state": @(state)
        });
    }
    
    if (state == ZegoEngineStateStop) {
        [[ZegoTextureRendererController sharedInstance] resetAllRenderFirstFrame];
    }
}

- (void)onNetworkTimeSynchronized {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onNetworkTimeSynchronized]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onNetworkTimeSynchronized"
        });
    }
}

- (void)onRequestDumpData {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRequestDumpData]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRequestDumpData"
        });
    }
}

- (void)onRequestUploadDumpData:(NSString *)dumpDir takePhoto:(BOOL)takePhoto {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRequestUploadDumpData]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRequestUploadDumpData",
            @"dumpDir": dumpDir,
            @"takePhoto": @(takePhoto),
        });
    }
}

- (void)onStartDumpData:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onStartDumpData]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onStartDumpData",
            @"errorCode": @(errorCode)
        });
    }
}

- (void)onStopDumpData:(int)errorCode dumpDir:(NSString *)dumpDir {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onStopDumpData]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onStopDumpData",
            @"errorCode": @(errorCode),
            @"dumpDir": dumpDir
        });
    }
}

- (void)onUploadDumpData:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onUploadDumpData]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onUploadDumpData",
            @"errorCode": @(errorCode)
        });
    }
}

#pragma mark Room Callback

- (void)onRoomStateUpdate:(ZegoRoomState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomStateUpdate] state: %d, errorCode: %d, roomID: %@", (int)state, errorCode, roomID);

    GUARD_SINK
    if (sink) {
        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[onRoomStateUpdate] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        sink(@{
            @"method": @"onRoomStateUpdate",
            @"state": @(state),
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString,
            @"roomID": roomID
        });
    }
}

- (void)onRoomStateChanged:(ZegoRoomStateChangedReason)reason errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomStateChanged] reason: %d, errorCode: %d, roomID: %@", (int)reason, errorCode, roomID);

    GUARD_SINK
    if (sink) {
        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[onRoomStateChanged] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        sink(@{
            @"method": @"onRoomStateChanged",
            @"reason": @(reason),
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString,
            @"roomID": roomID
        });
    }
}

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType userList:(NSArray<ZegoUser *> *)userList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomUserUpdate] updateType: %@, usersCount: %d, roomID: %@", updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)userList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *userListArray = [[NSMutableArray alloc] init];
        for (ZegoUser *user in userList) {
            [userListArray addObject:@{
                @"userID": user.userID == nil ? @"" : user.userID,
                @"userName": user.userName == nil ? @"" : user.userName
            }];
        }

        sink(@{
            @"method": @"onRoomUserUpdate",
            @"updateType": @(updateType),
            @"userList": userListArray,
            @"roomID": roomID
        });
    }
}

- (void)onRoomOnlineUserCountUpdate:(int)count roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomOnlineUserCountUpdate] count: %d, roomID: %@", count, roomID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRoomOnlineUserCountUpdate",
            @"count": @(count),
            @"roomID": roomID
        });
    }
}

- (void)onRoomStreamUpdate:(ZegoUpdateType)updateType streamList:(NSArray<ZegoStream *> *)streamList extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomStreamUpdate] updateType: %@, streamsCount: %d, extendedDataLength: %d, roomID: %@", updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)streamList.count, (int)extendedData.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *streamListArray = [[NSMutableArray alloc] init];
        for (ZegoStream *stream in streamList) {
            NSString *userID = @"";
            NSString *userName = @"";
            if (stream.user != nil && stream.user.userID != nil) {
                userID = stream.user.userID;
            }
            if (stream.user != nil && stream.user.userName != nil) {
                userName = stream.user.userName;
            }
            [streamListArray addObject:@{
                @"user": @{
                    @"userID": userID,
                    @"userName": userName
                },
                @"streamID": stream.streamID,
                @"extraInfo": stream.extraInfo
            }];
        }

        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[onRoomStateUpdate] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        sink(@{
            @"method": @"onRoomStreamUpdate",
            @"updateType": @(updateType),
            @"streamList": streamListArray,
            @"roomID": roomID,
            @"extendedData": extendedDataJsonString
        });
    }
}

- (void)onRoomStreamExtraInfoUpdate:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomStreamExtraInfoUpdate] streamsCount: %d, roomID: %@", (int)streamList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *streamListArray = [[NSMutableArray alloc] init];
        for (ZegoStream *stream in streamList) {
            [streamListArray addObject:@{
                @"user": @{
                    @"userID": stream.user.userID,
                    @"userName": stream.user.userName
                },
                @"streamID": stream.streamID,
                @"extraInfo": stream.extraInfo
            }];
        }

        sink(@{
            @"method": @"onRoomStreamExtraInfoUpdate",
            @"streamList": streamListArray,
            @"roomID": roomID
        });
    }
}

- (void)onRoomExtraInfoUpdate:(NSArray<ZegoRoomExtraInfo *> *)roomExtraInfoList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomExtraInfoUpdate] infosCount: %d, roomID: %@", (int)roomExtraInfoList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *roomExtraInfoListArray = [[NSMutableArray alloc] init];
        for (ZegoRoomExtraInfo *info in roomExtraInfoList) {
            [roomExtraInfoListArray addObject:@{
                @"key": info.key,
                @"value": info.value,
                @"updateUser": @{
                    @"userID": info.updateUser.userID,
                    @"userName": info.updateUser.userName
                },
                @"updateTime": @(info.updateTime)
            }];
        }

        sink(@{
            @"method": @"onRoomExtraInfoUpdate",
            @"roomExtraInfoList": roomExtraInfoListArray,
            @"roomID": roomID
        });
    }
}

- (void)onRoomTokenWillExpire:(int)remainTimeInSecond roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomTokenWillExpire] remainTimeInSecond: %d, roomID: %@", remainTimeInSecond, roomID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRoomTokenWillExpire",
            @"remainTimeInSecond": @(remainTimeInSecond),
            @"roomID": roomID
        });
    }
}

#pragma mark Publisher Callback

- (void)onPublisherStateUpdate:(ZegoPublisherState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherStateUpdate] state: %d, errorCode: %d, streamID: %@", (int)state, errorCode, streamID);

    GUARD_SINK
    if (sink) {
        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[onPublisherStateUpdate] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        sink(@{
            @"method": @"onPublisherStateUpdate",
            @"state": @(state),
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString,
            @"streamID": streamID
        });
    }
}

- (NSDictionary *)convertPublishQuality:(ZegoPublishStreamQuality *)quality {
    return @{
                @"videoCaptureFPS": @(quality.videoCaptureFPS),
                @"videoEncodeFPS": @(quality.videoEncodeFPS),
                @"videoSendFPS": @(quality.videoSendFPS),
                @"videoKBPS": @(quality.videoKBPS),
                @"audioCaptureFPS": @(quality.audioCaptureFPS),
                @"audioSendFPS": @(quality.audioSendFPS),
                @"audioKBPS": @(quality.audioKBPS),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"level": @(quality.level),
                @"isHardwareEncode": @(quality.isHardwareEncode),
                @"videoCodecID": @(quality.videoCodecID),
                @"totalSendBytes": @(quality.totalSendBytes),
                @"audioSendBytes": @(quality.audioSendBytes),
                @"videoSendBytes": @(quality.videoSendBytes)
            };
}

- (void)onPublisherQualityUpdate:(ZegoPublishStreamQuality *)quality streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherQualityUpdate",
            @"quality": [self convertPublishQuality:quality],
            @"streamID": streamID
        });
    }
}

- (void)onPublisherCapturedAudioFirstFrame {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherCapturedAudioFirstFrame]");

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherCapturedAudioFirstFrame",
        });
    }
}

- (void)onPublisherCapturedVideoFirstFrame:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherCapturedVideoFirstFrame] channel: %d", (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherCapturedVideoFirstFrame",
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherSendAudioFirstFrame:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherSendAudioFirstFrame] channel: %d", (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherSendAudioFirstFrame",
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherSendVideoFirstFrame:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherSendVideoFirstFrame] channel: %d", (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherSendVideoFirstFrame",
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherRenderVideoFirstFrame:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherRenderVideoFirstFrame] channel: %d", (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherRenderVideoFirstFrame",
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherVideoSizeChanged:(CGSize)size channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherVideoSizeChanged] width: %d, height: %d, channel: %d", (int)size.width, (int)size.height, (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherVideoSizeChanged",
            @"width": @((int)size.width),
            @"height": @((int)size.height),
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherRelayCDNStateUpdate:(NSArray<ZegoStreamRelayCDNInfo *> *)streamInfoList streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherRelayCDNStateUpdate] infosCount: %d, streamID: %@", (int)streamInfoList.count, streamID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *streamInfoListArray = [[NSMutableArray alloc] init];
        for (ZegoStreamRelayCDNInfo *info in streamInfoList) {
            [streamInfoListArray addObject:@{
                @"url": info.url,
                @"state": @(info.state),
                @"updateReason": @(info.updateReason),
                @"stateTime": @(info.stateTime)
            }];
        }

        sink(@{
            @"method": @"onPublisherRelayCDNStateUpdate",
            @"streamInfoList": streamInfoListArray,
            @"streamID": streamID
        });
    }
}

- (void)onPublisherVideoEncoderChanged:(ZegoVideoCodecID)fromCodecID toCodecID:(ZegoVideoCodecID)toCodecID channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherVideoEncoderChanged] fromCodecID: %d, toCodecID: %d, channel: %d", (int)fromCodecID, (int)toCodecID, (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherVideoEncoderChanged",
            @"fromCodecID": @(fromCodecID),
            @"toCodecID": @(toCodecID),
            @"channel": @(channel)
        });
    }
}

- (void)onPublisherStreamEvent:(ZegoStreamEvent)eventID streamID:(NSString *)streamID extraInfo:(NSString *)extraInfo {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherStreamEvent] eventID: %d, streamID: %@, extraInfo: %@", (int)eventID, streamID, extraInfo);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherStreamEvent",
            @"eventID": @(eventID),
            @"streamID": streamID,
            @"extraInfo": extraInfo
        });
    }
}

- (void)onVideoObjectSegmentationStateChanged:(ZegoObjectSegmentationState)state
                                      channel:(ZegoPublishChannel)channel
                                    errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onVideoObjectSegmentationStateChanged] state: %d, channel: %d, errorCode: %d", (int)state, (int)channel, errorCode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onVideoObjectSegmentationStateChanged",
            @"state": @((int)state),
            @"channel": @((int)channel),
            @"errorCode": @(errorCode)
        });
    }
}

- (void)onPublisherLowFpsWarning:(ZegoVideoCodecID)codecID channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherLowFpsWarning] codecID: %d, channel: %d", (int)codecID, (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherLowFpsWarning",
            @"codecID": @((int)codecID),
            @"channel": @((int)channel),
        });
    }
}

- (void)onPublisherDummyCaptureImagePathError:(int)errorCode path:(NSString *)path channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPublisherDummyCaptureImagePathError] errorCode: %d, path: %@, channel: %d", errorCode, path, (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherDummyCaptureImagePathError",
            @"errorCode": @(errorCode),
            @"path": path,
            @"channel": @((int)channel),
        });
    }
}

#pragma mark Player Callback

- (void)onPlayerStateUpdate:(ZegoPlayerState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerStateUpdate] state: %d, errorCode: %d, streamID: %@", (int)state, errorCode, streamID);

    GUARD_SINK
    if (sink) {
        NSString *extendedDataJsonString = @"{}";
        if (extendedData) {
            NSError *error;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:extendedData options:0 error:&error];
            if (!jsonData) {
                ZGLog(@"[onPlayerStateUpdate] extendedData error: %@", error);
            }else{
                extendedDataJsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
            }
        }

        sink(@{
            @"method": @"onPlayerStateUpdate",
            @"state": @(state),
            @"errorCode": @(errorCode),
            @"extendedData": extendedDataJsonString,
            @"streamID": streamID
        });
    }
}

- (void)onPlayerQualityUpdate:(ZegoPlayStreamQuality *)quality streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerQualityUpdate",
            @"quality": @{
                @"videoRecvFPS": @(quality.videoRecvFPS),
                @"videoDejitterFPS": @(quality.videoDejitterFPS),
                @"videoDecodeFPS": @(quality.videoDecodeFPS),
                @"videoRenderFPS": @(quality.videoRenderFPS),
                @"videoKBPS": @(quality.videoKBPS),
                @"videoBreakRate": @(quality.videoBreakRate),
                @"audioRecvFPS": @(quality.audioRecvFPS),
                @"audioDejitterFPS": @(quality.audioDejitterFPS),
                @"audioDecodeFPS": @(quality.audioDecodeFPS),
                @"audioRenderFPS": @(quality.audioRenderFPS),
                @"audioKBPS": @(quality.audioKBPS),
                @"audioBreakRate": @(quality.audioBreakRate),
                @"mos": @(quality.mos),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"peerToPeerDelay": @(quality.peerToPeerDelay),
                @"peerToPeerPacketLostRate": @(quality.peerToPeerPacketLostRate),
                @"level": @(quality.level),
                @"delay": @(quality.delay),
                @"avTimestampDiff": @(quality.avTimestampDiff),
                @"isHardwareDecode": @(quality.isHardwareDecode),
                @"videoCodecID": @(quality.videoCodecID),
                @"totalRecvBytes": @(quality.totalRecvBytes),
                @"audioRecvBytes": @(quality.audioRecvBytes),
                @"videoRecvBytes": @(quality.videoRecvBytes),
                @"audioCumulativeBreakCount": @(quality.audioCumulativeBreakCount),
                @"videoCumulativeBreakCount": @(quality.videoCumulativeBreakCount),
                @"audioCumulativeBreakTime": @(quality.audioCumulativeBreakTime),
                @"videoCumulativeBreakTime": @(quality.videoCumulativeBreakTime),
                @"audioCumulativeBreakRate": @(quality.audioCumulativeBreakRate),
                @"videoCumulativeBreakRate": @(quality.videoCumulativeBreakRate),
                @"audioCumulativeDecodeTime": @(quality.audioCumulativeDecodeTime),
                @"videoCumulativeDecodeTime": @(quality.videoCumulativeDecodeTime),
                @"muteVideo": @(quality.muteVideo),
                @"muteAudio": @(quality.muteAudio)
            },
            @"streamID": streamID
        });
    }
}

- (void)onPlayerMediaEvent:(ZegoPlayerMediaEvent)event streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerMediaEvent] event: %d, streamID: %@", (int)event, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerMediaEvent",
            @"event": @(event),
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvAudioFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerRecvAudioFirstFrame] streamID: %@", streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvAudioFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvVideoFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerRecvVideoFirstFrame] streamID: %@", streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvVideoFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRenderVideoFirstFrame:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerRenderVideoFirstFrame] streamID: %@", streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRenderVideoFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerVideoSizeChanged:(CGSize)size streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerVideoSizeChanged] width: %d, height: %d, streamID: %@", (int)size.width, (int)size.height, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerVideoSizeChanged",
            @"width": @((int)size.width),
            @"height": @((int)size.height),
            @"streamID": streamID
        });
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-implementations"
- (void)onPlayerRecvSEI:(NSData *)data streamID:(NSString *)streamID {
#pragma clang diagnostic pop
    FlutterEventSink sink = _eventSink;
    // Do not log high frequency callback

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvSEI",
            @"data": [FlutterStandardTypedData typedDataWithBytes:data],
            @"streamID": streamID
        });
    }
}

- (void)onPlayerRecvMediaSideInfo:(ZegoMediaSideInfo *)info {
    FlutterEventSink sink = _eventSink;
    // Do not log high frequency callback

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvMediaSideInfo",
            @"SEIData": [FlutterStandardTypedData typedDataWithBytes:info.SEIData],
            @"streamID": info.streamID,
            @"timestampNs": @(info.timestampNs)
        });
    }
}

- (void)onPlayerRecvAudioSideInfo:(NSData *)data streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // Do not log high frequency callback

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvAudioSideInfo",
            @"data": [FlutterStandardTypedData typedDataWithBytes:data],
            @"streamID": streamID
        });
    }
}

- (void)onPlayerLowFpsWarning:(ZegoVideoCodecID) codecID streamID:(NSString *) streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerLowFpsWarning] codecID: %d,streamID: %@", (int)codecID, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerLowFpsWarning",
            @"codecID": @(codecID),
            @"streamID": streamID
        });
    }
}

- (void)onPlayerStreamEvent:(ZegoStreamEvent)eventID streamID:(NSString *)streamID extraInfo:(NSString *)extraInfo {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerStreamEvent] eventID: %d,streamID: %@, extraInfo: %@", (int)eventID, streamID, extraInfo);

    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerStreamEvent",
            @"eventID": @(eventID),
            @"streamID": streamID,
            @"extraInfo": extraInfo
        });
    }
}

- (void)onPlayerRenderCameraVideoFirstFrame:(NSString *) streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerRenderCameraVideoFirstFrame] streamID: %@", streamID);

    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerRenderCameraVideoFirstFrame",
            @"streamID": streamID
        });
    }
}

- (void)onPlayerVideoSuperResolutionUpdate:(NSString *) streamID state:(ZegoSuperResolutionState) state errorCode:(int) errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerVideoSuperResolutionUpdate] streamID: %@, state: %d, errorCode: %d", streamID, (int)state, errorCode);

    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onPlayerVideoSuperResolutionUpdate",
            @"streamID": streamID,
            @"state": @(state),
            @"errorCode": @(errorCode)
        });
    }
}

#pragma mark Mixer Callback

- (void)onMixerRelayCDNStateUpdate:(NSArray<ZegoStreamRelayCDNInfo *> *)infoList taskID:(NSString *)taskID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMixerRelayCDNStateUpdate] infosCount: %d, taskID: %@", (int)infoList.count, taskID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *infoListArray = [[NSMutableArray alloc] init];
        for (ZegoStreamRelayCDNInfo *info in infoList) {
            [infoListArray addObject:@{
                @"url": info.url,
                @"state": @(info.state),
                @"updateReason": @(info.updateReason),
                @"stateTime": @(info.stateTime)
            }];
        }

        sink(@{
            @"method": @"onMixerRelayCDNStateUpdate",
            @"infoList": infoListArray,
            @"taskID": taskID
        });
    }
}

- (void)onMixerSoundLevelUpdate:(NSDictionary<NSNumber *,NSNumber *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onMixerSoundLevelUpdate",
            @"soundLevels": soundLevels
        });
    }
}

- (void)onAutoMixerSoundLevelUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onAutoMixerSoundLevelUpdate",
            @"soundLevels": soundLevels
        });
    }
}

#pragma mark Device Callback

- (void)onCapturedSoundLevelUpdate:(NSNumber *)soundLevel {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onCapturedSoundLevelUpdate",
            @"soundLevel": soundLevel
        });
    }
}

- (void)onCapturedSoundLevelInfoUpdate:(ZegoSoundLevelInfo *)soundLevelInfo {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onCapturedSoundLevelInfoUpdate",
            @"soundLevelInfo": @{
                @"soundLevel": @(soundLevelInfo.soundLevel),
                @"vad": @(soundLevelInfo.vad)
            }
        });
    }
}

- (void)onRemoteSoundLevelUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onRemoteSoundLevelUpdate",
            @"soundLevels": soundLevels
        });
    }
}

- (void)onRemoteSoundLevelInfoUpdate:(NSDictionary<NSString *, ZegoSoundLevelInfo *> *)soundLevelInfos {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    NSMutableDictionary *resultDic = @{}.mutableCopy;

    [soundLevelInfos enumerateKeysAndObjectsUsingBlock:^(NSString *key, ZegoSoundLevelInfo *obj, BOOL * _Nonnull stop) {
        resultDic[key] = @{
            @"soundLevel": @(obj.soundLevel),
            @"vad": @(obj.vad)
        };
    }]; 

    if (sink) {
        sink(@{
            @"method": @"onRemoteSoundLevelInfoUpdate",
            @"soundLevelInfos": resultDic
        });
    }
}

- (void)onCapturedAudioSpectrumUpdate:(NSArray<NSNumber *> *)audioSpectrum {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onCapturedAudioSpectrumUpdate",
            @"audioSpectrum": audioSpectrum
        });
    }
}

- (void)onRemoteAudioSpectrumUpdate:(NSDictionary<NSString *,NSArray<NSNumber *> *> *)audioSpectrums {
    FlutterEventSink sink = _eventSink;
    // Super high frequency callbacks do not log, do not guard sink

    if (sink) {
        sink(@{
            @"method": @"onRemoteAudioSpectrumUpdate",
            @"audioSpectrums": audioSpectrums
        });
    }
}

- (void)onLocalDeviceExceptionOccurred:(ZegoDeviceExceptionType) exceptionType deviceType:(ZegoDeviceType) deviceType deviceID:(NSString *) deviceID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onLocalDeviceExceptionOccurred] exceptionType: %d, deviceType: %d, deviceID: %@", (int)exceptionType, (int)deviceType, deviceID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onLocalDeviceExceptionOccurred",
            @"exceptionType": @(exceptionType),
            @"deviceType": @(deviceType),
            @"deviceID": deviceID
        });
    }
}

#pragma clang diagnostic pop

- (void)onRemoteCameraStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRemoteCameraStateUpdate] state: %d, streamID: %@", (int)state, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRemoteCameraStateUpdate",
            @"state": @(state),
            @"streamID": streamID
        });
    }
}

- (void)onRemoteSpeakerStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRemoteSpeakerStateUpdate] state: %d, streamID: %@", (int)state, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRemoteSpeakerStateUpdate",
            @"state": @(state),
            @"streamID": streamID
        });
    }
}

- (void)onRemoteMicStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRemoteMicStateUpdate] state: %d, streamID: %@", (int)state, streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRemoteMicStateUpdate",
            @"state": @(state),
            @"streamID": streamID
        });
    }
}

- (void)onAudioRouteChange:(ZegoAudioRoute)audioRoute {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAudioRouteChange] audioRoute: %d", (int)audioRoute);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onAudioRouteChange",
            @"audioRoute": @(audioRoute),
        });
    }
}

- (void)onAudioVADStateUpdate:(ZegoAudioVADType) state monitorType:(ZegoAudioVADStableStateMonitorType) type {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAudioVADStateUpdate] state: %d, type: %d", (int)state, (int)type);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onAudioVADStateUpdate",
            @"state": @(state),
            @"type": @(type),
        });
    }
}

- (void)onAudioDeviceVolumeChanged:(int)volume
                        deviceType:(ZegoAudioDeviceType)deviceType
                          deviceID:(NSString *)deviceID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAudioDeviceVolumeChanged] volume: %d, deviceType: %d, deviceID: %@", volume, (int)deviceType, deviceID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onAudioDeviceVolumeChanged",
            @"volume": @(volume),
            @"deviceType": @(deviceType),
            @"deviceID": deviceID,
        });
    }
}

- (void)onAudioDeviceStateChanged:(ZegoDeviceInfo *)deviceInfo
                       updateType:(ZegoUpdateType)updateType
                       deviceType:(ZegoAudioDeviceType)deviceType {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAudioDeviceStateChanged] deviceID: %@, deviceName: %@, deviceType: %td, updateType: %td", deviceInfo.deviceID, deviceInfo.deviceName, deviceType, updateType);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onAudioDeviceStateChanged",
            @"deviceInfo": @{
                @"deviceID": deviceInfo.deviceID,
                @"deviceName": deviceInfo.deviceName
            },
            @"deviceType": @(deviceType),
            @"updateType": @(updateType),
        });
    }
}

- (void)onVideoDeviceStateChanged:(ZegoDeviceInfo *)deviceInfo
                       updateType:(ZegoUpdateType)updateType {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onVideoDeviceStateChanged] deviceID: %@, deviceName: %@, updateType: %td", deviceInfo.deviceID, deviceInfo.deviceName, updateType);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onVideoDeviceStateChanged",
            @"deviceInfo": @{
                @"deviceID": deviceInfo.deviceID,
                @"deviceName": deviceInfo.deviceName
            },
            @"updateType": @(updateType),
        });
    }
}

#pragma mark IM Callback

- (void)onIMRecvBroadcastMessage:(NSArray<ZegoBroadcastMessageInfo *> *)messageList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onIMRecvBroadcastMessage] messageListCount: %d, roomID: %@", (int)messageList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *messageListArray = [[NSMutableArray alloc] init];
        for (ZegoBroadcastMessageInfo *info in messageList) {
            [messageListArray addObject:@{
                @"message": info.message,
                @"messageID": @(info.messageID),
                @"sendTime": @(info.sendTime),
                @"fromUser": @{
                    @"userID": info.fromUser.userID,
                    @"userName": info.fromUser.userName
                }
            }];
        }

        sink(@{
            @"method": @"onIMRecvBroadcastMessage",
            @"messageList": messageListArray,
            @"roomID": roomID
        });
    }
}

- (void)onIMRecvBarrageMessage:(NSArray<ZegoBarrageMessageInfo *> *)messageList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onIMRecvBarrageMessage] messageListCount: %d, roomID: %@", (int)messageList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *messageListArray = [[NSMutableArray alloc] init];
        for (ZegoBarrageMessageInfo *info in messageList) {
            [messageListArray addObject:@{
                @"message": info.message,
                @"messageID": info.messageID,
                @"sendTime": @(info.sendTime),
                @"fromUser": @{
                    @"userID": info.fromUser.userID,
                    @"userName": info.fromUser.userName
                }
            }];
        }

        sink(@{
            @"method": @"onIMRecvBarrageMessage",
            @"messageList": messageListArray,
            @"roomID": roomID
        });
    }
}

- (void)onIMRecvCustomCommand:(NSString *)command fromUser:(ZegoUser *)fromUser roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onIMRecvCustomCommand] command: %@, fromUserID: %@, fromUserName: %@, roomID: %@", command, fromUser.userID, fromUser.userName, roomID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onIMRecvCustomCommand",
            @"command": command,
            @"fromUser": @{
                @"userID": fromUser.userID,
                @"userName": fromUser.userName
            },
            @"roomID": roomID
        });
    }
}

- (void)onRecvRoomTransparentMessage:(ZegoRoomRecvTransparentMessage *)message roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRecvRoomTransparentMessage] sendUserID: %@, sendUserName: %@, roomID: %@", message.sendUser.userID, message.sendUser.userName, roomID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRecvRoomTransparentMessage",
            @"message": @{
                @"sendUser": @{
                    @"userID": message.sendUser.userID,
                    @"userName": message.sendUser.userName
                },
                @"content": message.content
            },
            @"roomID": roomID
        });
    }
}

#pragma mark Utilities Callback

- (void)onPerformanceStatusUpdate:(ZegoPerformanceStatus *)status {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPerformanceStatusUpdate",
            @"status": @{
                @"cpuUsageApp": @(status.cpuUsageApp),
                @"cpuUsageSystem": @(status.cpuUsageSystem),
                @"memoryUsageApp": @(status.memoryUsageApp),
                @"memoryUsageSystem": @(status.memoryUsageSystem),
                @"memoryUsedApp": @(status.memoryUsedApp),
            }
        });
    }
}

- (void)onNetworkModeChanged:(ZegoNetworkMode)mode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onNetworkModeChanged] mode: %d", (int)mode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onNetworkModeChanged",
            @"mode": @(mode)
        });
    }
}

- (void)onNetworkSpeedTestError:(int)errorCode type:(ZegoNetworkSpeedTestType)type {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onNetworkSpeedTestError] errorCode: %d, type: %d", errorCode, (int)type);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onNetworkSpeedTestError",
            @"errorCode": @(errorCode),
            @"type": @(type)
        });
    }
}

- (void)onNetworkSpeedTestQualityUpdate:(ZegoNetworkSpeedTestQuality *)quality type:(ZegoNetworkSpeedTestType)type {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onNetworkSpeedTestQualityUpdate",
            @"quality": @{
                @"connectCost": @(quality.connectCost),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"quality": @(quality.quality),
            },
            @"type": @(type)
        });
    }
}

- (void)onRecvExperimentalAPI:(NSString *)content {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRecvExperimentalAPI] content: %@", content);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRecvExperimentalAPI",
            @"content": content
        });
    }
}

- (void)onNetworkQuality:(NSString *)userID upstreamQuality:(ZegoStreamQualityLevel)upstreamQuality downstreamQuality:(ZegoStreamQualityLevel)downstreamQuality {
    FlutterEventSink sink = _eventSink;
    // ZGLog(@"[onNetworkQuality] userID: %@, upstreamQuality: %d, downstreamQuality: %d", userID, (int)upstreamQuality, (int)downstreamQuality);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onNetworkQuality",
            @"userID": userID,
            @"upstreamQuality": @(upstreamQuality),
            @"downstreamQuality": @(downstreamQuality)
        });
    }
}


#pragma mark - ZegoMediaPlayerEventHandler

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer stateUpdate:(ZegoMediaPlayerState)state errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaPlayerStateUpdate] idx: %d, state: %d, errorCode: %d", mediaPlayer.index.intValue, (int)state, errorCode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerStateUpdate",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"state": @(state),
            @"errorCode": @(errorCode)
        });
    }
    
    if (state == ZegoMediaPlayerStateNoPlay || state == ZegoMediaPlayerStatePlayEnded) {
        [[ZegoTextureRendererController sharedInstance] resetMediaRenderFirstFrame:mediaPlayer.index];
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer networkEvent:(ZegoMediaPlayerNetworkEvent)networkEvent {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaPlayerNetworkEvent] idx: %d, networkEvent: %d", mediaPlayer.index.intValue, (int)networkEvent);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerNetworkEvent",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"networkEvent": @(networkEvent)
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer playingProgress:(unsigned long long)millisecond {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerPlayingProgress",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"millisecond": @(millisecond)
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer recvSEI:(NSData *)data {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerRecvSEI",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"data": [FlutterStandardTypedData typedDataWithBytes:data],
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *) mediaPlayer soundLevelUpdate:(float) soundLevel {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerSoundLevelUpdate",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"soundLevel": @(soundLevel),
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *) mediaPlayer frequencySpectrumUpdate:(NSArray<NSNumber *> *) spectrumList {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerFrequencySpectrumUpdate",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"spectrumList": spectrumList,
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *) mediaPlayer firstFrameEvent:(ZegoMediaPlayerFirstFrameEvent)event {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaPlayerFirstFrameEvent] idx: %d, event: %td", mediaPlayer.index.intValue, event);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerFirstFrameEvent",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"event": @(event),
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer renderingProgress:(unsigned long long)millisecond {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerRenderingProgress",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"millisecond": @(millisecond),
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer videoSizeChanged:(CGSize)size {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaPlayerVideoSizeChanged] idx: %d, width: %d, height: %d", mediaPlayer.index.intValue, (int)size.width, (int)size.height);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerVideoSizeChanged",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"width": @((int)size.width),
            @"height": @((int)size.height),
        });
    }
}

- (void)mediaPlayer:(ZegoMediaPlayer *)mediaPlayer localCacheError:(int)errorCode resource:(NSString *)resource cachedFile:(NSString *)cachedFile {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaPlayerLocalCache] idx: %d, error: %d, resource: %@, cached: %@", mediaPlayer.index.intValue, errorCode, resource, cachedFile);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaPlayerLocalCache",
            @"mediaPlayerIndex": mediaPlayer.index,
            @"errorCode": @((int)errorCode),
            @"resource": resource,
            @"cachedFile": cachedFile,
        });
    }
}

#pragma mark - ZegoAudioEffectPlayerEventHandler

- (void)audioEffectPlayer:(ZegoAudioEffectPlayer *)audioEffectPlayer audioEffectID:(unsigned int)audioEffectID playStateUpdate:(ZegoAudioEffectPlayState)state errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAudioEffectPlayStateUpdate] idx: %d, state: %d, errorCode: %d", [audioEffectPlayer getIndex].intValue, (int)state, errorCode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onAudioEffectPlayStateUpdate",
            @"audioEffectPlayerIndex": [audioEffectPlayer getIndex],
            @"audioEffectID": @(audioEffectID),
            @"state": @(state),
            @"errorCode": @(errorCode)
        });
    }
}

#pragma mark - MediaDataPublisher

- (void)mediaDataPublisher:(ZegoMediaDataPublisher *)publisher fileOpen:(NSString *)path {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaDataPublisherFileOpen] idx: %d, path: %@", [publisher getIndex].intValue, path);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaDataPublisherFileOpen",
            @"publisherIndex": [publisher getIndex],
            @"path": path,
        });
    }
}

- (void)mediaDataPublisher:(ZegoMediaDataPublisher *)publisher fileClose:(NSString *)path errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaDataPublisherFileClose] idx: %d, path: %@, errorCode: %d", [publisher getIndex].intValue, path, errorCode);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaDataPublisherFileClose",
            @"publisherIndex": [publisher getIndex],
            @"path": path,
            @"errorCode": @(errorCode),
        });
    }
}

- (void)mediaDataPublisher:(ZegoMediaDataPublisher *)publisher fileDataBegin:(NSString *)path {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onMediaDataPublisherFileDataBegin] idx: %d, path: %@", [publisher getIndex].intValue, path);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onMediaDataPublisherFileDataBegin",
            @"publisherIndex": [publisher getIndex],
            @"path": path,
        });
    }
}

#pragma mark - Range Audio
- (void)rangeAudio:(ZegoRangeAudio *)rangeAudio microphoneStateUpdate:(ZegoRangeAudioMicrophoneState)state errorCode:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRangeAudioMicrophoneStateUpdate] idx: %d, state: %d, errorCode: %d", 0, (int)state, errorCode);
    
    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onRangeAudioMicrophoneStateUpdate",
            @"rangeAudioIndex": @(0),
            @"state": @(state),
            @"errorCode": @(errorCode)
        });
    }

}

#pragma mark - Real Time Sequential Data callback
- (void)manager:(ZegoRealTimeSequentialDataManager *) manager receiveRealTimeSequentialData:(NSData *) data streamID:(NSString *) streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onReceiveRealTimeSequentialData",
            @"realTimeSequentialDataManagerIndex": [manager getIndex],
            @"data": [FlutterStandardTypedData typedDataWithBytes:data],
            @"streamID": streamID
        });
    }

}


#pragma mark - Record Callback

- (void)onCapturedDataRecordStateUpdate:(ZegoDataRecordState)state errorCode:(int)errorCode config:(ZegoDataRecordConfig *)config channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onCapturedDataRecordStateUpdate] state: %d, errorCode: %d, filePath: %@, recordType: %d, channel: %d", (int)state, errorCode, config.filePath, (int)config.recordType, (int)channel);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onCapturedDataRecordStateUpdate",
            @"state": @((int)state),
            @"errorCode": @(errorCode),
            @"config": @{
                @"filePath": config.filePath,
                @"recordType": @((int)config.recordType)
            },
            @"channel": @(channel)
        });
    }
}

- (void)onCapturedDataRecordProgressUpdate:(ZegoDataRecordProgress *)progress config:(ZegoDataRecordConfig *)config channel:(ZegoPublishChannel)channel {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onCapturedDataRecordProgressUpdate",
            @"progress": @{
                @"duration": @(progress.duration),
                @"currentFileSize": @(progress.currentFileSize),
                @"quality": [self convertPublishQuality:progress.quality],
            },
            @"config": @{
                @"filePath": config.filePath,
                @"recordType": @((int)config.recordType)
            },
            @"channel": @(channel)
        });
    }
}

#pragma mark - Audio Data Callback

- (void)onCapturedAudioData:(const unsigned char * _Nonnull)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onCapturedAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                    @"sampleRate": @(param.sampleRate),
                    @"channel": @(param.channel)
            }
        });
    }
}


- (void)onPlaybackAudioData:(const unsigned char * _Nonnull)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onPlaybackAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                    @"sampleRate": @(param.sampleRate),
                    @"channel": @(param.channel)
            }
        });
    }
}


- (void)onMixedAudioData:(const unsigned char * _Nonnull)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onMixedAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                    @"sampleRate": @(param.sampleRate),
                    @"channel": @(param.channel)
            }
        });
    }
}


- (void)onPlayerAudioData:(const unsigned char * _Nonnull)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onPlayerAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                    @"sampleRate": @(param.sampleRate),
                    @"channel": @(param.channel)
            },
            @"streamID": streamID
        });
    }
}

#pragma mark - Audio Process Handler
- (void)onProcessCapturedAudioData:(unsigned char *)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param timestamp:(double)timestamp {
    FlutterEventSink sink = _eventSink;
    
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onProcessCapturedAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                @"sampleRate": @(param.sampleRate),
                @"channel": @(param.channel)
            },
            @"timestamp": @(timestamp)
        });
    }
}

- (void)onProcessCapturedAudioDataAfterUsedHeadphoneMonitor:(unsigned char *)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param timestamp:(double)timestamp {
    FlutterEventSink sink = _eventSink;
    
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onProcessCapturedAudioDataAfterUsedHeadphoneMonitor",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                @"sampleRate": @(param.sampleRate),
                @"channel": @(param.channel)
            },
            @"timestamp": @(timestamp)
        });
    }
}

- (void)onAlignedAudioAuxData:(const unsigned char * _Nonnull) data dataLength:(unsigned int) dataLength param:(ZegoAudioFrameParam *) param {
    FlutterEventSink sink = _eventSink;
    
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onAlignedAudioAuxData",
            @"data": objData,
            @"param": @{
                @"sampleRate": @(param.sampleRate),
                @"channel": @(param.channel)
            }
        });
    }
}

- (void)onProcessRemoteAudioData:(unsigned char *)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param streamID:(NSString *)streamID timestamp:(double)timestamp {
    FlutterEventSink sink = _eventSink;
    
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onProcessRemoteAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                @"sampleRate": @(param.sampleRate),
                @"channel": @(param.channel)
            },
            @"streamID": streamID,
            @"timestamp": @(timestamp)
        });
    }
}

- (void)onProcessPlaybackAudioData:(unsigned char *)data dataLength:(unsigned int)dataLength param:(ZegoAudioFrameParam *)param timestamp:(double)timestamp {
    FlutterEventSink sink = _eventSink;
    
    // High frequency callbacks do not log

    GUARD_SINK

    NSData *objData = [[NSData alloc] initWithBytes:data length:dataLength];

    if (sink) {
        sink(@{
            @"method": @"onProcessPlaybackAudioData",
            @"data": objData,
            @"dataLength": @(dataLength),
            @"param": @{
                @"sampleRate": @(param.sampleRate),
                @"channel": @(param.channel)
            },
            @"timestamp": @(timestamp)
        });
    }
}

#pragma mark - Copyrighted Music Handler
- (void)onDownloadProgressUpdate:(ZegoCopyrightedMusic *)copyrightedMusic resourceID:(NSString *)resourceID progressRate:(float)progressRate {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onDownloadProgressUpdate",
            @"resourceID": resourceID,
            @"progressRate": @(progressRate)
        });
    }
}

- (void)onCurrentPitchValueUpdate:(ZegoCopyrightedMusic *)copyrightedMusic resourceID:(NSString *)resourceID currentDuration:(int)currentDuration pitchValue:(int)pitchValue {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onCurrentPitchValueUpdate",
            @"resourceID": resourceID,
            @"currentDuration": @(currentDuration),
            @"pitchValue": @(pitchValue)
        });
    }
}

#if TARGET_OS_OSX
- (void)screenCapture:(ZegoScreenCaptureSource *)source availableFrame:(const void *)data dataLength:(unsigned int)dataLength param:(ZegoVideoFrameParam *)param {
    if(![ZegoExpressEngineMethodHandler sharedInstance].enablePlatformView) {
        [ZegoTextureRendererController.sharedInstance sendScreenCapturedVideoFrameRawData:data dataLength:dataLength param:param];
    }
}

- (void)screenCapture:(ZegoScreenCaptureSource *)source exceptionOccurred:(ZegoScreenCaptureSourceExceptionType)type {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[screenCapture:exceptionOccurred:] type: %td", type);

    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onExceptionOccurred",
            @"screenCaptureSourceIndex": [source getIndex],
            @"exceptionType": @(type)
        });
    }
}

- (void)screenCapture:(ZegoScreenCaptureSource *)source windowState:(ZegoScreenCaptureWindowState)state windowRect:(CGRect)rect {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[screenCapture:windowState:windowRect:] state: %td, rect: %@", state, NSStringFromRect(rect));
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onWindowStateChanged",
            @"screenCaptureSourceIndex": [source getIndex],
            @"windowState": @(state),
            @"windowRect": @{
                @"x": @(rect.origin.x),
                @"y": @(rect.origin.y),
                @"width": @(rect.size.width),
                @"height": @(rect.size.height)
            }
        });
    }
}

- (void)screenCapture:(ZegoScreenCaptureSource *)source rectChanged:(CGRect)rect {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[screenCapture:rectChanged:], rect: %@", NSStringFromRect(rect));
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onRectChanged",
            @"screenCaptureSourceIndex": [source getIndex],
            @"captureRect": @{
                @"x": @(rect.origin.x),
                @"y": @(rect.origin.y),
                @"width": @(rect.size.width),
                @"height": @(rect.size.height)
            }
        });
    }
}
#endif

#pragma mark - AI Voice Changer Handler
- (void)aiVoiceChanger:(ZegoAIVoiceChanger *)aiVoiceChanger onInit:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAIVoiceChangerInit], index: %d, errorCode: %d", aiVoiceChanger.getIndex, errorCode);
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onAIVoiceChangerInit",
            @"aiVoiceChangerIndex": @(aiVoiceChanger.getIndex),
            @"errorCode": @(errorCode)
        });
    }
}

- (void)aiVoiceChanger:(ZegoAIVoiceChanger *)aiVoiceChanger onUpdate:(int)errorCode {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAIVoiceChangerUpdate], index: %d, errorCode: %d", aiVoiceChanger.getIndex, errorCode);
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onAIVoiceChangerUpdate",
            @"aiVoiceChangerIndex": @(aiVoiceChanger.getIndex),
            @"errorCode": @(errorCode)
        });
    }
}

- (void)aiVoiceChanger:(ZegoAIVoiceChanger *)aiVoiceChanger onGetSpeakerList:(int)errorCode speakers:(NSArray<ZegoAIVoiceChangerSpeakerInfo *> *)speakers {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAIVoiceChangerGetSpeakerList], index: %d, errorCode: %d, speakers.count:%td", aiVoiceChanger.getIndex, errorCode, speakers.count);
    
    GUARD_SINK
    
    NSMutableArray *speakerArray = [NSMutableArray array];
    for (ZegoAIVoiceChangerSpeakerInfo *info in speakers) {
        NSDictionary *infoMap = @{
            @"name": info.name,
            @"id": @(info.id)
        };
        [speakerArray addObject:infoMap];
    }
    
    if (sink) {
        sink(@{
            @"method": @"onAIVoiceChangerGetSpeakerList",
            @"aiVoiceChangerIndex": @(aiVoiceChanger.getIndex),
            @"errorCode": @(errorCode),
            @"speakerList": speakerArray
        });
    }
}

- (void)aiVoiceChanger:(ZegoAIVoiceChanger *)aiVoiceChanger onUpdateProgress:(double)percent fileIndex:(int)fileIndex fileCount:(int)fileCount {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onAIVoiceChangerUpdateProgress], index: %d, percent: %lf, fileIndex: %d, fileCount: %d", aiVoiceChanger.getIndex, percent, fileIndex, fileCount);
    
    GUARD_SINK
    
    if (sink) {
        sink(@{
            @"method": @"onAIVoiceChangerUpdateProgress",
            @"aiVoiceChangerIndex": @(aiVoiceChanger.getIndex),
            @"percent": @(percent),
            @"fileIndex": @(fileIndex),
            @"fileCount": @(fileCount)
        });
    }
}

@end
