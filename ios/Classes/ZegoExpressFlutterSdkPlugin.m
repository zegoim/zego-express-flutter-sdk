#import "ZegoExpressFlutterSdkPlugin.h"
#if __has_include(<zego_express_flutter_sdk/zego_express_flutter_sdk-Swift.h>)
#import <zego_express_flutter_sdk/zego_express_flutter_sdk-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "zego_express_flutter_sdk-Swift.h"
#endif

@implementation ZegoExpressFlutterSdkPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftZegoExpressFlutterSdkPlugin registerWithRegistrar:registrar];
}
@end
