# zego_express_engine [![pub package](https://img.shields.io/pub/v/zego_express_engine.svg)](https://pub.dartlang.org/packages/zego_express_engine)

[English](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README.md) | [中文](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README_zh.md)

ZegoExpressEngine Audio/Video Flutter SDK is a flutter plugin wrapper based on [ZegoExpressEngine](https://doc-en.zego.im/en/693.html) native Android/iOS SDK, providing live video and real-time audio/video services. 🚀 It only needs 4 lines of code and can be easily accessed in 30 minutes.

Learn more about the solution: [https://www.zego.im](https://www.zego.im)

> 🔥 Sample demo

1. **[https://github.com/zegoim/zego-express-example-topics-flutter](https://github.com/zegoim/zego-express-example-topics-flutter)**

    🧬 An example topic demo (including the `Quick Start` demo) showing the SDK publish & play stream functions and other various features. This demo is continuously updated.

2. **[https://github.com/zegoim/zego-express-flutter-sdk/tree/master/example](https://github.com/zegoim/zego-express-flutter-sdk/tree/master/example)**

    🧪 A simple publish & play stream example demo, in the `example` directory of this repository, shows the basic function of the SDK.

3. **[https://github.com/zegoim/zego-express-example-screen-capture-flutter](https://github.com/zegoim/zego-express-example-screen-capture-flutter)**

    📲 An example demo that implements screen capture and live broadcast on Android/iOS.

## 1️⃣ Download and Install Flutter

### **[Flutter Get Started](https://flutter.dev/docs/get-started/install)**

## 2️⃣ Configuration development environment

- Android Studio: `Preferences` -> `Plugins`, search for the `Flutter` plugin to download, configure the first step to download the Flutter SDK path in the plugin.

- VS Code: Search for the `Flutter` extension in the app store and download

After configuring the flutter environment in any of the above development environments, execute the `flutter doctor` on the command line and complete the related undownloaded dependencies according to the prompts.

## 3️⃣ Apply for ZEGO AppID

Log in to [ZEGO Official Website](https://www.zego.im) to register an account, select a scenario according to your actual business needs, and obtain AppID and App Sign for initializing the SDK.

## 4️⃣ Import `zego_express_engine`

Open the `pubspec.yaml` in the project and add the `zego_express_engine` dependency:

- ### Depends on `pub` (recommended)

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine: ^2.0.0
```

- ### Depends on git

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
      ref: master
```

After saving the file, execute `flutter pub get`

## 5️⃣ Add device permissions

### Android

Open the file `app/src/main/AndroidManifest.xml`, and add the following contents:

```xml
    <!-- Permissions required by the SDK -->
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.BLUETOOTH" />
    <uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />

    <!-- Permissions required by the Demo App -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.WAKE_LOCK" />

    <uses-feature android:glEsVersion="0x00020000" android:required="true" />
    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
```

> If the obfuscation is enabled during building release apk (`flutter build apk`) (default is enabled), you need to configure ZEGO-related classes to prevent obfuscation, refer to [FAQ-4](#4-android-building-release-crashes-with-noclassdeffounderror-when-flutter-is-upgraded-to-v110-or-above)

### iOS

Choose the option `TARGETS` -> `Info` -> `Custom iOS Target Properties`

![iOS Privacy Description](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description.png)

Click the `+` Add button to add camera and microphone permissions.

1. `Privacy - Camera Usage Description`

2. `Privacy - Microphone Usage Description`

After adding permissions, it will be as shown:

![iOS Privacy Description Done](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description-done.png)

> If you use Platform View, and Flutter version is lower than 1.22, you need to add an additional description for iOS, refer to [FAQ-1](#1-ios-error-when-using-platform-view-verbose-2platform_view_layercc28-trying-to-embed-a-platform-view-but-the-paintcontext-does-not-support-embedding)

## 6️⃣ Init SDK

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

    // Apply appID and appSign from ZEGO
    final int appID = 1234567890;
    final String appSign = 'abcdefghijklmnopqrstuvwzyv123456789abcdefghijklmnopqrstuvwzyz123';

    // Specify test environment and app scenario
    final bool isTestEnv = true;
    final ZegoScenario scenario = ZegoScenario.General;

    @override
    void initState() {
        super.initState();

        // Get SDK Version
        ZegoExpressEngine.getVersion().then((value) {
            setState(() => version = value);
        });

        // Create ZegoExpressEngine (Init SDK)
        ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario);
    }

    @override
    Widget build(BuildContext context) {
        return MaterialApp(home: Scaffold(
            appBar: AppBar(title: const Text('ZegoExpressEngine')),
            body: Center(child: Text('Version: $version')),
        ));
    }
}
```

🔮 Please refer to the **[Example Topics Demo](https://github.com/zegoim/zego-express-example-topics-flutter)** and API documentation for more features and usage.

## 7️⃣ API documentation

[ZegoExpressEngine API Reference](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine-class.html)

## 8️⃣ FAQ

### 1. iOS: error when using Platform View: `[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

> This setting is no longer required for Flutter 1.22 and later versions.

Open the iOS native project (Runner.xcworkspace) that requires Platform View and add the field `io.flutter.embedded_views_preview` to `Info.plist` with a value of `YES`.

![flutter enable platform view](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/flutter_embeded_views_plist.png)

### 2. iOS: `fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture`

Usually occurs when switching iOS devices, which can be resolved by deleting the `flutter-project-path/build/` and `flutter-project-path/ios/DerivedData/` directories. (If you cannot find the `DerivedData` folder, please look for `/Users/your-user-name/Library/Developer/Xcode/DerivedData/`)

### 3. iOS: Error when compiling: `CDN: trunk URL couldn't be downloaded` or `CDN: trunk Repo update failed`

Open the Terminal terminal `cd` into the `ios` folder in the root directory of your project (the directory where the `Podfile` file is located) and execute `pod repo update`.

It is usually caused by a poor Internet connection in mainland China. It is recommended to turn on the proxy. Please refer to [iOS CocoaPods FAQ](https://doc-zh.zego.im/zh/1253.html)

### 4. Android: building release crashes with `NoClassDefFoundError` when Flutter is upgraded to v1.10 or above

Flutter is enabled obfuscation by default in version 1.10 or above. Please add the following line in `app/proguard-rules.pro` to prevent the SDK obfuscation.

```java
-keep class **.zego.**{*;}
```

### 5. Android: When `TextureRenderer` is frequently created and destroyed, the following crash may occur

```text
OpenGLRenderer  E  [SurfaceTexture-0-4944-46] updateTexImage: SurfaceTexture is abandoned!
       flutter  E  [ERROR:flutter/shell/platform/android/platform_view_android_jni.cc(39)] java.lang.RuntimeException: Error during updateTexImage (see logcat for details)
```

This issue is caused by thread unsafe when calling SurfaceTexture's updateTexImage() and release() internally in Flutter Engine. It has been fixed in Flutter `1.24-candidate.2` version. For details, see: [https://github.com/flutter/engine/pull/21777](https://github.com/flutter/engine/pull/21777)

## 9️⃣ How to contribute

🎉 We welcome contributions, information on how to get started can be found at our [contributor guide](CONTRIBUTING.md).
