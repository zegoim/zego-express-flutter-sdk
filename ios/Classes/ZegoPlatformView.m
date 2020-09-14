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

@property (nonatomic, strong) UIView *uiView;
@property (nonatomic, assign) int64_t viewID;

@end

@implementation ZegoPlatformView

- (instancetype)initWithRect:(CGRect)rect viewID:(int64_t) viewID {
    self = [super init];
    if (self) {
        _uiView = [[UIView alloc] initWithFrame:rect];
        _uiView.backgroundColor = [UIColor blackColor];
        _viewID = viewID;
    }

    ZGLog(@"[ZegoPlatformView] [init] UIView:%p", self.uiView);
    
    return self;
}

- (UIView *)getUIView {
    return self.uiView;
}

- (void)dealloc {
    ZGLog(@"[ZegoPlatformView] [dispose] UIView:%p", self.uiView);
}

# pragma mark - Flutter Platform View Delegate

- (UIView *)view {
    return self.uiView;
}


@end
