# zego_express_engine

[English](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README.md)

[中文](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README_zh.md)

ZegoExpressEngine Audio/Video Flutter SDK provides live video and real-time audio/video services. It only needs 4 lines of code and can be easily accessed in 30 minutes.

Learn more about the solution: [https://www.zego.im](https://www.zego.im)

## I. Download and Install Flutter

[Flutter Get Started](https://flutter.dev/docs/get-started/install)

## II. Configuration development environment

- Android Studio: `Preferences` -> `Plugins`, search for the `Flutter` plugin to download, configure the first step to download the Flutter SDK path in the plugin.

- Visual Code: Search for the `Flutter` extension in the app store and download

After configuring the flutter environment in any of the above development environments, execute the `flutter doctor` on the command line and complete the related undownloaded dependencies according to the prompts.

## III. Apply for ZEGO AppID

Log in to [ZEGO Official Website](https://www.zego.im) to register an account, select a scenario according to your actual business needs, and obtain AppID and App Sign for initializing the SDK.

## IV. Import `zego_express_engine`

Open the `pubspec.yaml` in the project and add the `zego_express_engine` dependency:

### Depends on pub (recommended)

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine: ^1.5.5
```

### Depends on git

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
```

### Depend on git and specify the branch

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
      ref: some-branch
```

## V. Init SDK

```Dart
import 'package:flutter/material.dart';
import 'package:zego_express_engine/zego_express_engine.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
    @override
    _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
    String version = 'Unknown';

    // Apply AppID and AppSign from ZEGO
    final int appID = 1234567890;

    // Apply AppID and AppSign from ZEGO
    final String appSign = 'abcdefghijklmnopqrstuvwzyv123456789abcdefghijklmnopqrstuvwzyz123';

    // Specify test environment
    final bool isTestEnv = true;

    // Specify a general scenario
    final ZegoScenario scenario = ZegoScenario.General;

    @override
    void initState() {
        super.initState();

        // Get SDK Version
        ZegoExpressEngine.getVersion().then((ver) {
            setState(() {
              version = ver;
            });
        });

        // Create ZegoExpressEngine
        ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario);
    }

    @override
    Widget build(BuildContext context) {
        return MaterialApp(
            home: Scaffold(
                appBar: AppBar(
                    title: const Text('ZegoExpressEngine'),
                ),
                body: Center(
                    child: Text('Version: $version\n'),
                ),
            ),
        );
    }
}
```

Please refer to the API documentation for more features.

## VI. API documentation

[ZegoExpressEngine API Reference](https://pub.dev/documentation/zego_express_engine/latest/)

## VII. FAQ

### 1. iOS: error when using Platform View: `[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

Open the iOS project that requires Platform View and add the field `io.flutter.embedded_views_preview` to `Info.plist` with a value of `YES`.

### 2. iOS: fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture

Usually occurs when switching iOS devices, which can be resolved by deleting the `flutter-project-path/build/` and `flutter-project-path/ios/DerivedData/` directories. (If you cannot find the `DerivedData` folder, please look for `/Users/your-user-name/Library/Developer/Xcode/DerivedData/`)

### 3. Android: building release crashes with `NoClassDefFoundError` when Flutter is upgraded to v1.10 or above

Flutter is enabled obfuscation by default in version 1.10 or above. Please add the following line in `app/proguard-rules.pro` to prevent the SDK obfuscation.

```java
-keep class **.zego.**{*;}
```
