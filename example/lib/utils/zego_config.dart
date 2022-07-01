import 'package:zego_express_engine/zego_express_engine.dart' show ZegoScenario;

class ZegoConfig {
  static final ZegoConfig instance = ZegoConfig._internal();
  ZegoConfig._internal();

  // ----- Persistence params -----
  int appID = 0;
  ZegoScenario scenario = ZegoScenario.General;

  bool enablePlatformView = false;

  String userID = "";
  String userName = "";
  String appSign = "";
  String token = "";

  String roomID = "";
  String streamID = "";

  // ----- Short-term params -----

  bool isPreviewMirror = true;
  bool isPublishMirror = false;

  bool enableHardwareEncoder = false;
}
