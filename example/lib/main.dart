import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:zego_express_engine/zego_express_engine.dart';

void main() => runApp(MyApp());

class MyApp extends StatefulWidget {
    @override
    _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
    String version = 'Unknown';

    // Apply AppID and AppSign from Zego
    final int appID = 1234567890;

    // Apply AppID and AppSign from Zego
    final String appSign = 'abcdefghijklmnopqrstuvwzyv123456789abcdefghijklmnopqrstuvwzyz123';

    // Use Test Environment
    final bool isTestEnv = true;

    // Specify General Scenario
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
