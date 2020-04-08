import 'dart:async';
import 'package:shared_preferences/shared_preferences.dart';
import 'dart:io';
import 'dart:math' show Random;

import 'package:zego_express_engine/zego_express_engine.dart' show ZegoScenario;

class ZegoConfig {

  static final ZegoConfig instance = ZegoConfig._internal();

  int appID;
  String appSign;
  bool isTestEnv;
  int scenario;

  bool enablePlatformView;

  String userID;
  String userName;

  String roomID;
  String streamID;

  bool isPreviewMirror;
  bool isPublishMirror;

  bool enableHardwareEncoder;

  ZegoConfig._internal() {

    SharedPreferences.getInstance().then((config) {

      this.appID = config.getInt('appID') ?? 0;
      this.appSign = config.getString('appSign') ?? '';
      this.isTestEnv = config.getBool('isTestEnv') ?? true;
      this.scenario = config.getInt('scenario') ?? ZegoScenario.General.index;

      this.enablePlatformView = config.getBool('enablePlatformView') ?? false;

      this.userID = config.getString('userID') ?? '${Platform.operatingSystem}-${new Random().nextInt(9999999).toString()}';
      this.userName = config.getString('userName') ?? 'user-$userID';

      this.roomID = config.getString('roomID') ?? '';
      this.streamID = config.getString('streamID') ?? '';

      this.isPreviewMirror = true;
      this.isPublishMirror = false;

      this.enableHardwareEncoder = false;

    });
  }

  Future<void> saveConfig() async {

    SharedPreferences config = await SharedPreferences.getInstance();

    config.setInt('appID', this.appID);
    config.setString('appSign', this.appSign);
    config.setBool('isTestEnv', this.isTestEnv);
    config.setInt('scenario', this.scenario);

    config.setBool('enablePlatformView', this.enablePlatformView);

    config.setString('userID', this.userID);
    config.setString('userName', this.userName);

    config.setString('roomID', this.roomID);
    config.setString('streamID', this.streamID);
  }

}