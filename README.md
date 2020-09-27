# zego_express_engine [![pub package](https://img.shields.io/pub/v/zego_express_engine.svg)](https://pub.dartlang.org/packages/zego_express_engine)

[English](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README.md) | [中文](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README_zh.md)

ZegoExpressEngine Audio/Video Flutter SDK is a flutter plugin wrapper based on [ZegoExpressEngine](https://doc-en.zego.im/en/693.html) native Android / iOS SDK, providing live video and real-time audio/video services. It only needs 4 lines of code and can be easily accessed in 30 minutes.

Learn more about the solution: [https://www.zego.im](https://www.zego.im)

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

  zego_express_engine: ^1.16.0
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

![Add Android Privacy](https://storage.zego.im/sdk-doc/Pics/Android/ZegoLiveRoom/ZegoLiveRoom-IntegrationGuide/3.2-insert_sourceSets_node-4.png)

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
    <!-- Permissions required by the App -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.WAKE_LOCK" />

    <uses-feature
        android:glEsVersion="0x00020000"
        android:required="true" />

    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
```

> Note: Because Android 6.0 requires dynamic permissions for some of the more important permissions, you cannot apply for static permissions only through the `AndroidMainfest.xml` file. Therefore, you need to refer to the following code (requestPermissions is the Activity method)

```java
String[] permissionNeeded = {
        "android.permission.CAMERA",
        "android.permission.RECORD_AUDIO"};

if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
    if (ContextCompat.checkSelfPermission(this, "android.permission.CAMERA") != PackageManager.PERMISSION_GRANTED ||
        ContextCompat.checkSelfPermission(this, "android.permission.RECORD_AUDIO") != PackageManager.PERMISSION_GRANTED) {
        requestPermissions(permissionNeeded, 101);
    }
}
```

### iOS

Choose the option TARGETS -> Info -> Custom iOS Target Properties

![Add iOS Privacy](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description.png)

Click the + Add button to add camera and microphone permissions.

1. `Privacy - Camera Usage Description`

2. `Privacy - Microphone Usage Description`

After adding permissions, it will be as shown:

![Add iOS Privacy Done](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description-done.png)

> If you use Platform View, you need to add an additional description for iOS, refer to [FAQ-1](#1-ios-error-when-using-platform-view-verbose-2platform_view_layercc28-trying-to-embed-a-platform-view-but-the-paintcontext-does-not-support-embedding)

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

## 7️⃣ API documentation

[ZegoExpressEngine API Reference](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine-class.html)

## 8️⃣ FAQ

### 1. iOS: error when using Platform View: `[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

Open the iOS project that requires Platform View and add the field `io.flutter.embedded_views_preview` to `Info.plist` with a value of `YES`.

![iOS-Enable-PlatformView-Guide](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/flutter_embeded_views_plist.png)

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

## 9️⃣ How to contribute

We welcome contributions, information on how to get started can be found at our [contributor guide](CONTRIBUTING.md).
