import 'package:zego_express_engine/zego_express_engine.dart' show ZegoScenario;

class ZegoConfig {
  static final ZegoConfig instance = ZegoConfig._internal();
  ZegoConfig._internal();

  int appID = 245697902;

  // It is for native only, do not use it for web!
  String appSign = "39011cbb0e6b9afd6b3aed4449a467e514b2975fba484daad3e3f217589751df";

  // It is required for web and is recommended for native but not required.
  String token = "";

  ZegoScenario scenario = ZegoScenario.Default;
  bool enablePlatformView = false;

  String userID = "linux_test";
  String userName = "linux_test";

  bool isPreviewMirror = true;
  bool isPublishMirror = false;

  bool enableHardwareEncoder = false;
}
