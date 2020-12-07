import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:zego_express_engine_example/pages/init_sdk_page.dart';
import 'package:zego_express_engine_example/config/zego_config.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'ZegoExpressExample',
      theme: ThemeData(

        primarySwatch: Colors.blue,
      ),
      home: HomePage(title: 'ZegoExpressExample'),
    );
  }
}

class HomePage extends StatelessWidget {
  HomePage({Key key, this.title}) : super(key: key);

  final String title;

  @override
  Widget build(BuildContext context) {

    ZegoConfig.instance.init(); // Load config instance

    return Scaffold(
      appBar: AppBar(title: Text(title)),
      body: SafeArea(child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Container(
              padding: const EdgeInsets.symmetric(horizontal: 50.0, vertical: 20.0),
              child: Text('This is a simple demo showing how to use the SDK to implement basic publish and play stream functions.', style: TextStyle(fontSize: 10.0),),
            ),
            Container(child: CupertinoButton(
              padding: EdgeInsets.symmetric(horizontal: 10.0),
              color: Color(0xff0e88eb),
              child: Text('Publish Stream'),
              onPressed: () {
                Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                  return InitPage(true);
                }));
              }
            ), width: 150.0),
            SizedBox(height: 10),
            Container(child: CupertinoButton(
              padding: EdgeInsets.symmetric(horizontal: 10.0),
              color: Color(0xff0e88eb),
              child: Text('Play Stream'),
              onPressed: () {
                Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                  return InitPage(false);
                }));
              }
            ), width: 150.0),
            SizedBox(height: 30),
            Container(
              padding: const EdgeInsets.symmetric(horizontal: 10.0, vertical: 10.0),
              child: Text('For more features, please refer to the example topic demo.', style: TextStyle(fontSize: 10.0),),
            ),
            Container(child: CupertinoButton(
              padding: EdgeInsets.symmetric(horizontal: 10.0),
              color: Color(0xff0e88eb),
              child: Text('Open Github'),
              onPressed: () => launch('https://github.com/zegoim/zego-express-example-topics-flutter')
            ), width: 150.0),
          ],
        ),
      ))
    );
  }
}


