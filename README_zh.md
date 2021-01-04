# zego_express_engine [![pub package](https://img.shields.io/pub/v/zego_express_engine.svg)](https://pub.dartlang.org/packages/zego_express_engine)

[English](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README.md) | [中文](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README_zh.md)

即构科技 (ZEGO) 极速音视频 Flutter SDK 是一个基于 [ZegoExpressEngine](https://doc-zh.zego.im/zh/693.html) 原生 Android/iOS SDK 的 Flutter Plugin Wrapper，提供视频直播以及实时音视频服务。🚀 仅需 4 行代码，30 分钟即可轻松接入。

了解更多解决方案：[https://www.zego.im](https://www.zego.im)

> 🔥 示例 Demo

1. **[https://github.com/zegoim/zego-express-example-topics-flutter](https://github.com/zegoim/zego-express-example-topics-flutter)**

    🧬 一个展示 SDK 推拉流以及各种丰富功能的示例专题 Demo（包括 `快速开始` Demo 在内），持续更新中。

2. **[https://github.com/zegoim/zego-express-flutter-sdk/tree/master/example](https://github.com/zegoim/zego-express-flutter-sdk/tree/master/example)**

    🧪 一个简易的推拉流示例 Demo，内置于此仓库的 `example` 目录下，展示了 SDK 最基础的推拉流功能。

3. **[https://github.com/zegoim/zego-express-example-screen-capture-flutter](https://github.com/zegoim/zego-express-example-screen-capture-flutter)**

    📲 一个实现了 Android/iOS 屏幕采集并推流直播的示例 Demo。

## 1️⃣ 下载并安装 Flutter

### **[Flutter Get Started](https://flutter.dev/docs/get-started/install)**

## 2️⃣ 配置 Flutter 开发环境

- Android Studio: `Preferences` -> `Plugins`，搜索 `Flutter` 插件进行下载，在插件中配置第一步下载好的 Flutter SDK 路径。

- VS Code: 在应用商店中搜索 `Flutter` 扩展并下载。

以上任一开发环境配置好 Flutter 环境之后，命令行执行 `flutter doctor`，根据提示内容补全相关未下载的依赖项。

## 3️⃣ 申请 ZEGO AppID

登录 [ZEGO 官网](https://www.zego.im) 注册账号，根据自身实际业务需求选择场景，获取 AppID 与 AppSign，用于初始化 SDK。

## 4️⃣ 导入 `zego_express_engine`

打开工程中的 `pubspec.yaml`，添加 `zego_express_engine` 依赖：

- ### 以 `pub` 形式依赖（推荐）

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine: ^2.0.0
```

- ### 以 git 形式依赖

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
      ref: master
```

保存文件后，执行 `flutter pub get`

## 5️⃣ 添加设备权限

### Android

打开 `app/src/main/AndroidManifest.xml` 文件，添加如下内容：

```xml
    <!-- SDK 必须使用的权限 -->
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.RECORD_AUDIO" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.CAMERA" />
    <uses-permission android:name="android.permission.BLUETOOTH" />
    <uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />

    <!-- Demo App 需要使用的部分权限 -->
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.WAKE_LOCK" />

    <uses-feature android:glEsVersion="0x00020000" android:required="true" />
    <uses-feature android:name="android.hardware.camera" />
    <uses-feature android:name="android.hardware.camera.autofocus" />
```

> 如果打包 release apk 时 (`flutter build apk`) 开启了混淆功能 (Flutter 新版本默认开启)，需要为 ZEGO 相关的类配置防止混淆，参考 [常见问题-4](#4-android-flutter-升级至-v110-或以上时android-release-下出现-noclassdeffounderror-导致-crash)

### iOS

选择项目 `TARGETS` -> `Info` -> `Custom iOS Target Properties`

![iOS Privacy Description](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description.png)

点击 `+` 添加按钮，添加摄像头和麦克风权限。

1. `Privacy - Camera Usage Description`

2. `Privacy - Microphone Usage Description`

添加权限完成后如图所示：

![iOS Privacy Description Done](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/privacy-description-done.png)

> 如果需要使用 Platform View，并且 Flutter 版本小于 1.22，iOS 端还需额外添加一行描述，参考 [常见问题-1](#1-ios-使用-platform-view-时报错verbose-2platform_view_layercc28-trying-to-embed-a-platform-view-but-the-paintcontext-does-not-support-embedding)

## 6️⃣ 初始化 SDK

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

🔮 更多功能和使用方法请参考 **[示例专题 Demo](https://github.com/zegoim/zego-express-example-topics-flutter)** 和 API 文档。

## 7️⃣ API 文档

[ZegoExpressEngine API Reference](https://pub.dev/documentation/zego_express_engine/latest/zego_express_engine/ZegoExpressEngine-class.html)

## 8️⃣ 常见问题

### 1. iOS: 使用 Platform View 时报错：`[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

> Flutter 1.22 及以后的版本不再需要此设置。

打开需要使用 Platform View 的 iOS 原生工程 (Runner.xcworkspace) ，在 `Info.plist` 中添加字段 `io.flutter.embedded_views_preview`，其值为 `YES`。

![flutter enable platform view](https://storage.zego.im/sdk-doc/Pics/iOS/ZegoExpressEngine/Common/flutter_embeded_views_plist.png)

### 2. iOS: `fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture`

通常在切换 iOS 设备时出现，可通过删除 `flutter-project-path/build/` 和 `flutter-project-path/ios/DerivedData/` 目录解决。（找不到 `DerivedData` 文件夹的话，请查找 `/Users/your-user-name/Library/Developer/Xcode/DerivedData/`）

### 3. iOS: 编译时报错 `CDN: trunk URL couldn't be downloaded` 或者 `CDN: trunk Repo update failed`

打开 Terminal 终端 `cd` 进项目根目录下的 `ios` 文件夹（`Podfile` 文件所在的目录），执行 `pod repo update`。

通常是国内网络不畅导致，建议开启代理。请参考 [iOS CocoaPods 常见问题](https://doc-zh.zego.im/zh/1253.html)

### 4. Android: Flutter 升级至 v1.10 或以上时，Android release 下出现 `NoClassDefFoundError` 导致 Crash

Flutter 在 1.10 或以上版本默认开启了混淆，请在项目中 `app/proguard-rules.pro` 为 SDK 添加 -keep 类的配置防止混淆。

```java
-keep class **.zego.**{*;}
```

### 5. Android: 频繁创建销毁 `TextureRenderer` 时，可能出现以下 Crash

```text
OpenGLRenderer  E  [SurfaceTexture-0-4944-46] updateTexImage: SurfaceTexture is abandoned!
       flutter  E  [ERROR:flutter/shell/platform/android/platform_view_android_jni.cc(39)] java.lang.RuntimeException: Error during updateTexImage (see logcat for details)
```

此问题是 Flutter Engine 内部在调用 SurfaceTexture 的 updateTexImage() 与 release() 时线程不安全引起的，并已经在 Flutter `1.24-candidate.2` 版本修复，具体详见：[https://github.com/flutter/engine/pull/21777](https://github.com/flutter/engine/pull/21777)

## 9️⃣ How to contribute

🎉 We welcome contributions, information on how to get started can be found at our [contributor guide](CONTRIBUTING.md).
