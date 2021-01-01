#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint zego_express_engine.podspec' to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'zego_express_engine'
  s.version          = '2.0.0'
  s.summary          = 'Zego Express Audio/Video Live SDK for Flutter'
  s.description      = <<-DESC
ZegoExpressEngine Flutter SDK is a flutter plugin wrapper based on ZegoExpressEngine native Android / iOS SDK, providing live video and real-time audio/video services.
                       DESC
  s.homepage         = 'https://zego.im'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'ZEGO' => 'developer@zego.im' }
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.public_header_files = 'Classes/**/*.h'
  s.static_framework = true
  s.dependency 'Flutter'
  s.dependency 'ZegoExpressEngine', '2.0.0'
  s.platform = :ios, '8.0'
  s.ios.deployment_target = '8.0'

  # Flutter.framework does not contain a i386 slice. Only x86_64 simulators are supported.
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'VALID_ARCHS[sdk=iphonesimulator*]' => 'x86_64' }
end
