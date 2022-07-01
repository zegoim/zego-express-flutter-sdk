import 'package:zego_express_engine/zego_express_engine.dart' show ZegoScenario;

class ZegoConfig {
  static final ZegoConfig instance = ZegoConfig._internal();
  ZegoConfig._internal();

  int appID = 0;

  String appSign = ""; // Only for native
  String token =
      ""; // Token is required for web, native recommends using token but not required

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
