#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint zego_express_engine.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'zego_express_engine'
  s.version          = '2.23.0'
  s.summary          = 'Zego Express Audio/Video Live SDK for Flutter'
  s.description      = <<-DESC
ZegoExpressEngine Flutter SDK is a flutter plugin wrapper based on ZEGO Express RTC SDK (Native & Web), providing live video and real-time audio/video services.
                       DESC
  s.homepage         = 'https://zego.im'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'ZEGO' => 'developer@zego.im' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.dependency 'ZegoExpressEngine', '~> 2.23.0'
  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
  s.script_phases = [
    { :name => 'Precompile',
      :script => '${PODS_TARGET_SRCROOT}/setup.sh',
      :execution_position => :before_compile,
      :input_files => [ '${PODS_TARGET_SRCROOT}/../ios/Classes/internal/**/*' ],
      :output_files => [ '${PODS_TARGET_SRCROOT}/Classes/internal/**/*' ]
    }
  ]
end
