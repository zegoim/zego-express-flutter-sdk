import 'dart:async';
import 'package:flutter/services.dart';

class ZegoExpressEngine {
  /* Method Channel */
  static const MethodChannel _channel =
      const MethodChannel('plugins.zego.im/zego_express_engine');
  static ZegoExpressEngine _instance = new ZegoExpressEngine._internal();

  ZegoExpressEngine._internal();

  factory ZegoExpressEngine() {
    return _instance;
  }

  static Future<String> get sdkVersion async {
    //final String version = await _channel.invokeMethod('getSDKVersion');
    var version = "hhhhhhhhhhhhhhh";
    return version;
  }

  static Future<void> create() {

  }

  static Future<void> destroy() {

  }

  static ZegoExpressEngine get instance {
    return _instance;
  }

  
}