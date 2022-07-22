//
//  ZegoPlatformView.m
//  Pods-Runner
//
//  Created by lizhanpeng@ZEGO on 2020/3/26.
//  Copyright © 2020 Zego. All rights reserved.
//

#import "ZegoPlatformView.h"
#import "ZegoLog.h"

@interface ZegoPlatformView()

@property (nonatomic, strong) ZGView *uiView;
@property (nonatomic, assign) int64_t viewID;

@end

@implementation ZegoPlatformView

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID {
    self = [super init];
    if (self) {
#if TARGET_OS_IPHONE
        _uiView = [[UIView alloc] initWithFrame:rect];
        _uiView.backgroundColor = [UIColor blackColor];
#elif TARGET_OS_OSX
//        _uiView
#endif
        _viewID = viewID;
    }

    ZGLog(@"[ZegoPlatformView] [init] UIView:%p", self.uiView);
    
    return self;
}

- (ZGView *)getUIView {
    return self.uiView;
}

- (void)dealloc {
    ZGLog(@"[ZegoPlatformView] [dispose] UIView:%p", self.uiView);
}

# pragma mark - Flutter Platform View Delegate

#if TARGET_OS_IPHONE
- (UIView *)view {
    return self.uiView;
}
#endif


@end
