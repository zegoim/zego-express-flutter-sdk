#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint zego_express_engine.podspec' to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'zego_express_engine'
  s.version          = '3.3.0'
  s.summary          = 'Zego Express Audio/Video Live SDK for Flutter'
  s.description      = <<-DESC
ZegoExpressEngine Flutter SDK is a flutter plugin wrapper based on ZEGO Express RTC SDK (Native & Web), providing live video and real-time audio/video services.
                       DESC
  s.homepage         = 'https://zego.im'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'ZEGO' => 'developer@zego.im' }
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.public_header_files = 'Classes/**/*.h'
  s.static_framework = true
  s.dependency 'Flutter'
  s.vendored_frameworks = 'libs/ZegoExpressEngine.xcframework'
  s.platform = :ios, '9.0'

  # Flutter.framework does not contain a i386 slice.
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386' }

  s.prepare_command = 'sh download.sh'
  s.script_phases = [
    { :name => 'Download native dependency',
      :script => 'sh ${PODS_TARGET_SRCROOT}/download.sh',
      :execution_position => :before_compile,
      :input_files => [ '${PODS_TARGET_SRCROOT}/DEPS' ],
      :output_files => [ '${PODS_TARGET_SRCROOT}/libs/*' ]
    }
  ]
end
