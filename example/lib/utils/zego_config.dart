import 'package:zego_express_engine/zego_express_engine.dart' show ZegoScenario;

class ZegoConfig {
  static final ZegoConfig instance = ZegoConfig._internal();
  ZegoConfig._internal();

  int appID = 0;

  // It is for native only, do not use it for web!
  String appSign = "";

  // It is required for web and is recommended for native but not required.
  String token = "";

  ZegoScenario scenario = ZegoScenario.General;
  bool enablePlatformView = false;

  String userID = "";
  String userName = "";

  String roomID = "";
  String streamID = "";

  bool isPreviewMirror = true;
  bool isPublishMirror = false;

  bool enableHardwareEncoder = false;
}
