//
//  CustomCamera.m
//  Runner
//
//  Created by lizhanpeng@ZEGO on 2020/9/14.
//  Copyright © 2020 The Chromium Authors. All rights reserved.
//

#import "CustomCamera.h"
@interface CustomCamera() <AVCaptureVideoDataOutputSampleBufferDelegate> {
    dispatch_queue_t _sampleBufferCallbackQueue;
}

@property (nonatomic, strong) ZegoCustomVideoCaptureClient *client;
@property (nonatomic, strong) AVCaptureDeviceInput *input;
@property (nonatomic, strong) AVCaptureVideoDataOutput *output;
@property (nonatomic, strong) AVCaptureSession *session;
@property (nonatomic, assign) BOOL isFrontCamera;

@end


@implementation CustomCamera

- (instancetype)init {
    if(self = [super init]) {
        _sampleBufferCallbackQueue = dispatch_queue_create("im.zego.customCamera.outputCallbackQueue", DISPATCH_QUEUE_SERIAL);
        _isFrontCamera = YES;
    }
    
    return self;
}

#pragma mark ZegoCustomVideoCaptureDelegate

- (void)onStart:(ZegoCustomVideoCaptureClient*)client {
    self.client = client;
    
    [self.session beginConfiguration];
    
    if ([self.session canSetSessionPreset:AVCaptureSessionPresetHigh]) {
        [self.session setSessionPreset:AVCaptureSessionPresetHigh];
    }
    
    AVCaptureDeviceInput *input = self.input;
    
    if ([self.session canAddInput:input]) {
        [self.session addInput:input];
    }
    
    
    AVCaptureVideoDataOutput *output = self.output;
    
    if ([self.session canAddOutput:output]) {
        [self.session addOutput:output];
    }
    
    AVCaptureConnection *captureConnection = [output connectionWithMediaType:AVMediaTypeVideo];
    
    if (input.device.position == AVCaptureDevicePositionFront) {
        // 使用前置摄像头时，需调用 SDK 设置镜像
        [self.client setFlipMode:ZegoVideoFlipModeX];
        self.isFrontCamera = YES;
    }
    
    if (captureConnection.isVideoOrientationSupported) {
        captureConnection.videoOrientation = AVCaptureVideoOrientationPortrait;
    }
    
    [self.session commitConfiguration];
    
    if (!self.session.isRunning) {
        [self.session startRunning];
    }
}

- (void)onStop {
    
    if (self.session.isRunning) {
        [self.session stopRunning];
    }
    
    self.client = nil;
}

- (AVCaptureSession *)session {
    if (!_session) {
        _session = [[AVCaptureSession alloc] init];
    }
    return _session;
}

- (AVCaptureDeviceInput *)input {
    if (!_input) {
            NSArray *cameras= [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
            
    #if TARGET_OS_OSX
            // Note: This demonstration selects the last camera. Developers should choose the appropriate camera device by themselves.
            AVCaptureDevice *camera = cameras.lastObject;
            if (!camera) {
                NSLog(@"Failed to get camera");
                return nil;
            }
    #elif TARGET_OS_IOS
            // Note: This demonstration selects the front camera. Developers should choose the appropriate camera device by themselves.
            NSArray *captureDeviceArray = [cameras filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:@"position == %d", AVCaptureDevicePositionFront]];
            if (captureDeviceArray.count == 0) {
                NSLog(@"Failed to get camera");
                return nil;
            }
            AVCaptureDevice *camera = captureDeviceArray.firstObject;
    #endif
            
            NSError *error = nil;
            AVCaptureDeviceInput *captureDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:camera error:&error];
            if (error) {
                NSLog(@"Conversion of AVCaptureDevice to AVCaptureDeviceInput failed");
                return nil;
            }
            _input = captureDeviceInput;
        }
        return _input;
}

- (AVCaptureVideoDataOutput *)output {
    if (!_output) {
        AVCaptureVideoDataOutput *videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
        videoDataOutput.videoSettings = @{(id)kCVPixelBufferPixelFormatTypeKey:@(kCVPixelFormatType_32BGRA)};
        [videoDataOutput setSampleBufferDelegate:self queue:_sampleBufferCallbackQueue];
        _output = videoDataOutput;
    }
    return _output;
}

#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    CVPixelBufferRef buffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CMTime timeStamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
    
    // TODO: 美颜
    
    
    if(self.client) {
        [self.client sendCVPixelBuffer:buffer timestamp:timeStamp];
    }
}

@end
