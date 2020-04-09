//
//  ZegoPlatformViewFactory.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoPlatformViewFactory.h"

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

- (BOOL)addPlatformView:(ZegoPlatformView *)view viewID:(NSNumber *)viewID {
    if (!viewID || !view)
        return NO;

    [self.platformViewMap setObject:view forKey:viewID];

    return YES;
}

- (BOOL)destroyPlatformView:(NSNumber *)viewID {
    if (!viewID)
        return NO;
    
    if (![[self.platformViewMap allKeys] containsObject:viewID])
        return NO;
    
    [self.platformViewMap removeObjectForKey:viewID];
    return YES;
}

- (nullable ZegoPlatformView *)getPlatformView:(NSNumber *)viewID {
    if (!viewID) {
        return nil;
    }
    
    return [self.platformViewMap objectForKey:viewID];
}

#pragma mark FlutterPlatformViewFactory Delegate

/// Called when dart invoke `createPlatformView`, that is, when Widget `UiKitView` is added to the flutter render tree
- (NSObject<FlutterPlatformView>*)createWithFrame:(CGRect)frame
                                   viewIdentifier:(int64_t)viewId
                                        arguments:(id _Nullable)args {
    
    ZegoPlatformView *view = [[ZegoPlatformView alloc] initWithRect:frame viewID:viewId];
    [self addPlatformView:view viewID:@(viewId)];
    
    return view;
}

@end
