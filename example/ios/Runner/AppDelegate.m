#import "AppDelegate.h"
#import "GeneratedPluginRegistrant.h"
#import "CustomCamera.h"

@interface AppDelegate()

@property(nonatomic, strong)CustomCamera *camera;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [GeneratedPluginRegistrant registerWithRegistry:self];
    // Override point for customization after application launch.
    
    // 指定主通道将要使用自定义采集
    self.camera = [CustomCamera new];
    [[ZegoCustomVideoCaptureManager sharedInstance] setCustomVideoCaptureDelegate:self.camera channel:ZegoPublishChannelMain];
        
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

@end
