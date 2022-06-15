import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:zego_express_flutter_sdk/zego_express_flutter_sdk.dart';

void main() {
  const MethodChannel channel = MethodChannel('zego_express_flutter_sdk');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await ZegoExpressFlutterSdk.platformVersion, '42');
  });
}
