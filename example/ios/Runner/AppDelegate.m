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
    
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}

@end
