//
//  ZegoPlatformViewRenderController.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//

#import "ZegoPlatformViewRenderController.h"

@interface ZegoPlatformViewRenderController()

@property (nonatomic, strong) NSMutableDictionary<NSNumber*, ZegoPlatformViewRenderer*> *views;

@end

@implementation ZegoPlatformViewRenderController

+ (instancetype)sharedInstance
{
    static ZegoPlatformViewRenderController *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[ZegoPlatformViewRenderController alloc] init];
    });
    return instance;
}

- (instancetype) init {
    
    if(self = [super init]) {
        _views = [[NSMutableDictionary alloc] init];
        
    }
    return self;
}

- (BOOL)addView:(ZegoPlatformViewRenderer *)view viewID:(NSNumber *)viewID {
    if(!viewID || !view)
        return NO;

    [self.views setObject:view forKey:viewID];

    return YES;
}

- (BOOL)removeView:(NSNumber *)viewID {
    if(!viewID)
        return NO;
    
    if(![[self.views allKeys] containsObject:viewID])
        return NO;
    
    [self.views removeObjectForKey:viewID];
    return YES;
}

- (ZegoPlatformViewRenderer *)getRenderer:(NSNumber *) viewID {
    if(!viewID)
        return nil;
    
    return [self.views objectForKey:viewID];
}

#pragma mark FlutterPlatformViewFactory Delegate

- (NSObject<FlutterPlatformView>*)createWithFrame:(CGRect)frame
                                   viewIdentifier:(int64_t)viewId
                                        arguments:(id _Nullable)args {
    
    ZegoPlatformViewRenderer *view = [[ZegoPlatformViewRenderer alloc] initWithRect:frame viewID:viewId];
    [self addView:view viewID:@(viewId)];
    
    return view;
}

@end
