//
//  ZegoPlatformViewFactory.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoPlatformViewFactory.h"
#import "ZegoLog.h"

@interface ZegoPlatformViewFactory()

@property (nonatomic, strong) NSMutableDictionary<NSNumber*, ZegoPlatformView*> *platformViewMap;

@end

@implementation ZegoPlatformViewFactory

+ (instancetype)sharedInstance {
    static ZegoPlatformViewFactory *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoPlatformViewFactory alloc] init];
    });
    return instance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _platformViewMap = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (BOOL)destroyPlatformView:(NSNumber *)viewID {

    ZegoPlatformView *platformView = self.platformViewMap[viewID];

    if (!platformView) {
        ZGLog(@"[destroyPlatformView] platform view for viewID:%d not exists", viewID.intValue);
        [self logCurrentPlatformViews];
        return NO;
    }

    ZGLog(@"[destroyPlatformView] viewID:%d, UIView:%p", viewID.intValue, platformView.view);

    [self.platformViewMap removeObjectForKey:viewID];

    [self logCurrentPlatformViews];

    return YES;
}

- (nullable ZegoPlatformView *)getPlatformView:(NSNumber *)viewID {

    ZGLog(@"[getPlatformView] viewID:%d", viewID.intValue);

    [self logCurrentPlatformViews];
    
    return [self.platformViewMap objectForKey:viewID];
}

- (void)addPlatformView:(ZegoPlatformView *)view viewID:(NSNumber *)viewID {

    ZGLog(@"[createPlatformView] viewID:%d, UIView:%p", viewID.intValue, view.view);

    [self.platformViewMap setObject:view forKey:viewID];

    [self logCurrentPlatformViews];
}

- (void)logCurrentPlatformViews {
    NSMutableString *desc = [[NSMutableString alloc] init];
    for (NSNumber *i in self.platformViewMap) {
        ZegoPlatformView *eachPlatformView = self.platformViewMap[i];
        if (eachPlatformView) {
            [desc appendFormat:@"[ID:%d|View:%p] ", i.intValue, eachPlatformView.view];
        }
    }
    ZGLog(@"[ZegoPlatformViewFactory] currentPlatformViews: %@", desc);
}

#pragma mark FlutterPlatformViewFactory Delegate

#if TARGET_OS_IPHONE
/// Called when dart invoke `createPlatformView`, that is, when Widget `UiKitView` is added to the flutter render tree
- (NSObject<FlutterPlatformView>*)createWithFrame:(CGRect)frame
                                   viewIdentifier:(int64_t)viewId
                                        arguments:(id _Nullable)args {
    
    ZegoPlatformView *view = [[ZegoPlatformView alloc] initWithRect:frame viewID:viewId];
    [self addPlatformView:view viewID:@(viewId)];
    
    return view;
}

#elif TARGET_OS_OSX

- (nonnull NSView *)createWithViewIdentifier:(int64_t)viewId arguments:(nullable id)args {
    ZegoPlatformView *view = [[ZegoPlatformView alloc] initWithRect:CGRectZero viewID:viewId];
    [self addPlatformView:view viewID:@(viewId)];
    
    return view.view;
}

#endif

@end
