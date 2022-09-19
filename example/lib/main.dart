import 'package:flutter/material.dart';
import 'package:zego_express_engine_example/home/home_page.dart';

// This is a simple publish & play stream demo,
// shows the basic function of the SDK.

// You can refer to this example topic demo for more use cases.
// https://github.com/zegoim/zego-express-example-topics-flutter

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ZegoExpressExample',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomePage(), // Main page of this demo
    );
  }
}
