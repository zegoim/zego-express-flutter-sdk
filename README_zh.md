# zego_express_engine

[English](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README.md)

[中文](https://github.com/zegoim/zego-express-flutter-sdk/blob/master/README_zh.md)

即构科技 (ZEGO) 极速音视频 Flutter SDK，提供视频直播以及实时音视频服务。仅需4行代码，30分钟即可轻松接入。

了解更多解决方案：[https://www.zego.im](https://www.zego.im)

## 一、下载并安装 Flutter

[Flutter Get Started](https://flutter.dev/docs/get-started/install)

## 二、配置 Flutter 开发环境

- Android Studio: `Preferences` -> `Plugins`，搜索 `Flutter` 插件进行下载，在插件中配置第一步下载好的 Flutter SDK 路径。

- Visual Code: 在应用商店中搜索 `Flutter` 扩展并下载。

以上任一开发环境配置好 Flutter 环境之后，命令行执行 `flutter doctor`，根据提示内容补全相关未下载的依赖项。

## 三、申请 ZEGO AppID

登录 [ZEGO 官网](https://www.zego.im) 注册账号，根据自身实际业务需求选择场景，获取 AppID 与 AppSign，用于初始化 SDK。

## 四、导入 `zego_express_engine`

打开工程中的 `pubspec.yaml`，添加 `zego_express_engine` 依赖：

### 以 pub 形式依赖（推荐）

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine: ^1.5.5
```

### 以 git 形式依赖

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
```

### 以 git 形式依赖并指定分支

```yaml
dependencies:
  flutter:
  sdk: flutter

  zego_express_engine:
    git:
      url: git://github.com/zegoim/zego-express-flutter-sdk.git
      ref: some-branch
```

## 五、初始化 SDK

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

    // Apply appID and appSign from ZEGO
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

更多功能请参考 API 文档。

## 六、API 文档

[ZegoExpressEngine API Reference](https://pub.dev/documentation/zego_express_engine/latest/)

## 七、常见问题

### 1. iOS: 使用 Platform View 时报错：`[VERBOSE-2:platform_view_layer.cc(28)] Trying to embed a platform view but the PaintContext does not support embedding`

打开需要使用 Platform View 的 iOS 工程，在 `Info.plist` 中添加字段 `io.flutter.embedded_views_preview`，其值为 `YES`。

![iOS-Enable-PlatformView-Guide](https://raw.githubusercontent.com/patrick-fu/personal_blog_image/master/image/20200408194006.png)

### 2. iOS: fatal error: lipo: -extract armv7 specified but fat file: [...] does not contain that architecture

通常在切换 iOS 设备时出现，可通过删除 `flutter-project-path/build/` 和 `flutter-project-path/ios/DerivedData/` 目录解决。（找不到 `DerivedData` 文件夹的话，请查找 `/Users/your-user-name/Library/Developer/Xcode/DerivedData/`）

### 3. Android: Flutter 升级至 v1.10 或以上时，Android release 下出现 `NoClassDefFoundError` 导致 Crash

Flutter 在 1.10 或以上版本默认开启了混淆，请在项目中 `app/proguard-rules.pro` 为 SDK 添加 -keep 类的配置防止混淆。

```java
-keep class **.zego.**{*;}
```
