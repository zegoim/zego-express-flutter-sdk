//
//  ZegoExpressEngineEventHandler.m
//  Pods-Runner
//
//  Created by Patrick Fu on 2020/4/9.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoExpressEngineEventHandler.h"
#import "ZegoLog.h"
#import "ZegoCustomVideoCaptureManager.h"
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

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType userList:(NSArray<ZegoUser *> *)userList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomUserUpdate] updateType: %@, usersCount: %d, roomID: %@", updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)userList.count, roomID);

    GUARD_SINK
    if (sink) {
        NSMutableArray *userListArray = [[NSMutableArray alloc] init];
        for (ZegoUser *user in userList) {
            [userListArray addObject:@{
                @"userID": user.userID,
                @"userName": user.userName
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

- (void)onRoomStreamUpdate:(ZegoUpdateType)updateType streamList:(NSArray<ZegoStream *> *)streamList roomID:(NSString *)roomID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onRoomStreamUpdate] updateType: %@, streamsCount: %d, roomID: %@", updateType == ZegoUpdateTypeAdd ? @"Add" : @"Delete", (int)streamList.count, roomID);

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
            @"method": @"onRoomStreamUpdate",
            @"updateType": @(updateType),
            @"streamList": streamListArray,
            @"roomID": roomID
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

- (void)onPublisherQualityUpdate:(ZegoPublishStreamQuality *)quality streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    // High frequency callbacks do not log

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPublisherQualityUpdate",
            @"quality": @{
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
                @"totalSendBytes": @(quality.totalSendBytes),
                @"audioSendBytes": @(quality.audioSendBytes),
                @"videoSendBytes": @(quality.videoSendBytes)
            },
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
                @"videoDecodeFPS": @(quality.videoDecodeFPS),
                @"videoRenderFPS": @(quality.videoRenderFPS),
                @"videoKBPS": @(quality.videoKBPS),
                @"audioRecvFPS": @(quality.audioRecvFPS),
                @"audioDecodeFPS": @(quality.audioDecodeFPS),
                @"audioRenderFPS": @(quality.audioRenderFPS),
                @"audioKBPS": @(quality.audioKBPS),
                @"rtt": @(quality.rtt),
                @"packetLostRate": @(quality.packetLostRate),
                @"peerToPeerDelay": @(quality.peerToPeerDelay),
                @"peerToPeerPacketLostRate": @(quality.peerToPeerPacketLostRate),
                @"level": @(quality.level),
                @"delay": @(quality.delay),
                @"isHardwareDecode": @(quality.isHardwareDecode),
                @"totalRecvBytes": @(quality.totalRecvBytes),
                @"audioRecvBytes": @(quality.audioRecvBytes),
                @"videoRecvBytes": @(quality.videoRecvBytes)
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

- (void)onPlayerRecvSEI:(NSData *)data streamID:(NSString *)streamID {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onPlayerRecvSEI] streamID: %@", streamID);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onPlayerRecvSEI",
            @"data": [FlutterStandardTypedData typedDataWithBytes:data],
            @"streamID": streamID
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

- (void)onDeviceError:(int)errorCode deviceName:(NSString *)deviceName {
    FlutterEventSink sink = _eventSink;
    ZGLog(@"[onDeviceError] errorCode: %d, deviceName: %@", errorCode, deviceName);

    GUARD_SINK
    if (sink) {
        sink(@{
            @"method": @"onDeviceError",
            @"errorCode": @(errorCode),
            @"deviceName": deviceName
        });
    }
}

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


# pragma mark - ZegoAudioEffectPlayerEventHandler
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
                @"currentFileSize": @(progress.currentFileSize)
            },
            @"config": @{
                @"filePath": config.filePath,
                @"recordType": @((int)config.recordType)
            },
            @"channel": @(channel)
        });
    }
}


@end
