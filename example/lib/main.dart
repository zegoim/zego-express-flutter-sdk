import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
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

    ZegoConfig.instance;

    return Scaffold(
        appBar: AppBar(

          title: Text(title),
        ),
        body: SafeArea(
            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: <Widget>[
                  Padding(
                    padding: const EdgeInsets.only(bottom: 10.0),
                  ),
                  CupertinoButton(
                      color: Color(0xff0e88eb),
                      child: Text('Publish Stream'),
                      onPressed: () {
                        Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                          return InitPage(true);
                        }));
                      }
                  ),
                  Padding(
                    padding: const EdgeInsets.only(bottom: 10.0),
                  ),
                  CupertinoButton(
                      color: Color(0xff0e88eb),
                      child: Text('   Play Stream   '),
                      onPressed: () {
                        Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
                          return InitPage(false);
                        }));
                      }
                  ),
                ],
              ),
            )
        )
    );
  }
}


