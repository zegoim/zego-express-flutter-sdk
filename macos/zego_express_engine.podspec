#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint zego_express_engine.podspec` to validate before publishing.
#

require 'yaml'
require 'ostruct'
pubspec = OpenStruct.new YAML.load_file('../pubspec.yaml')

Pod::Spec.new do |s|
  s.name             = pubspec.name
  s.version          = pubspec.version
  s.summary          = pubspec.summary
  s.description      = pubspec.description
  s.homepage         = pubspec.homepage
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'ZEGO' => 'developer@zego.im' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.vendored_frameworks = 'libs/ZegoExpressEngine.xcframework'
  s.platform = :osx, '10.11'

  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'

  s.prepare_command = 'sh setup.sh && sh download.sh'
  s.script_phases = [
    { :name => 'Download native dependency',
      :script => 'sh ${PODS_TARGET_SRCROOT}/download.sh',
      :execution_position => :before_compile,
      :input_files => [ '${PODS_TARGET_SRCROOT}/DEPS' ],
      :output_files => [ '${PODS_TARGET_SRCROOT}/libs/*' ]
    },
    { :name => 'Copy internal shared code from iOS',
      :script => 'sh ${PODS_TARGET_SRCROOT}/setup.sh',
      :execution_position => :before_compile,
      :input_files => [ '${PODS_TARGET_SRCROOT}/../ios/Classes/internal/**/*' ],
      :output_files => [ '${PODS_TARGET_SRCROOT}/Classes/internal/**/*' ]
    }
  ]
end
