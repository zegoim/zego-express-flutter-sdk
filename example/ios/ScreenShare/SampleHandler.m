//
//  SampleHandler.m
//  ScreenShare
//
//  Created by 27 on 2022/12/23.
//  Copyright © 2022 The Chromium Authors. All rights reserved.
//


#import "SampleHandler.h"
#import <ZegoExpressEngine/ZegoExpressDefines.h>
#import <ZegoExpressEngine/ZegoExpressEventHandler.h>

@interface SampleHandler() <ZegoReplayKitExtHandler>
@end

@implementation SampleHandler

- (void)broadcastStartedWithSetupInfo:(NSDictionary<NSString *,NSObject *> *)setupInfo {
    // User has requested to start the broadcast. Setup info from the UI extension can be supplied but optional.
    [ZegoReplayKitExt.sharedInstance setupWithDelegate:self];
}

- (void)broadcastPaused {
    // User has requested to pause the broadcast. Samples will stop being delivered.
}

- (void)broadcastResumed {
    // User has requested to resume the broadcast. Samples delivery will resume.
}

- (void)broadcastFinished {
    // User has requested to finish the broadcast.
    [ZegoReplayKitExt.sharedInstance finished];
}

- (void)processSampleBuffer:(CMSampleBufferRef)sampleBuffer withType:(RPSampleBufferType)sampleBufferType {
    [ZegoReplayKitExt.sharedInstance sendSampleBuffer:sampleBuffer withType:sampleBufferType];
}

#pragma mark - Zego Replay Kit Ext Handler
- (void)broadcastFinished:(ZegoReplayKitExt *)broadcast reason:(ZegoReplayKitExtReason)reason {
    
    switch (reason) {
        case ZegoReplayKitExtReasonHostStop:
            {
                NSDictionary *userInfo = @{NSLocalizedDescriptionKey : @"Host app stop srceen capture"};
                NSError *error = [NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:userInfo];
                [self finishBroadcastWithError:error];
            }
            break;
        case ZegoReplayKitExtReasonConnectFail:
            {
                NSDictionary *userInfo = @{NSLocalizedDescriptionKey : @"Connect host app fail need startScreenCapture in host app"};
                NSError *error = [NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:userInfo];
                [self finishBroadcastWithError:error];
            }
            break;
        case ZegoReplayKitExtReasonDisconnect:
            {
                NSDictionary *userInfo = @{NSLocalizedDescriptionKey : @"disconnect with host app"};
                NSError *error = [NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:userInfo];
                [self finishBroadcastWithError:error];
            }
            break;
    }
}

@end
