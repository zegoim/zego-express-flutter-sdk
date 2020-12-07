import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/config/zego_config.dart';
import 'package:zego_express_engine_example/pages/login_room_page.dart';
import 'package:zego_express_engine_example/ui/zego_ui_tool.dart';

class InitPage extends StatefulWidget {

  final bool isPublish;

  InitPage(this.isPublish);

  @override
  _InitPageState createState() => new _InitPageState();
}

class _InitPageState extends State<InitPage> {

  final TextEditingController _appIDEdController = new TextEditingController();
  final TextEditingController _appSignEdController = new TextEditingController();

  String _version;

  @override
  void initState() {
    super.initState();

    if (ZegoConfig.instance.appID > 0) {
      _appIDEdController.text = ZegoConfig.instance.appID.toString();
    }

    if (ZegoConfig.instance.appSign.isNotEmpty) {
      _appSignEdController.text = ZegoConfig.instance.appSign;
    }

    ZegoExpressEngine.getVersion().then((version) {
      print('[SDK Version] $version');
      setState(() {
        _version = version;
      });
    });
  }


  void onButtonPressed() {

    String strAppID = _appIDEdController.text.trim();
    String appSign = _appSignEdController.text.trim();

    if (strAppID.isEmpty || appSign.isEmpty) {
      ZegoUITool.showAlert(context, 'AppID or AppSign cannot be empty');
      return;
    }

    int appID = int.tryParse(strAppID);
    if (appID == null) {
      ZegoUITool.showAlert(context, 'AppID is invalid, should be int');
      return;
    }

    bool isTestEnv = ZegoConfig.instance.isTestEnv;
    ZegoScenario scenario = ZegoConfig.instance.scenario;

    bool enablePlatformView = ZegoConfig.instance.enablePlatformView;

    // Step1: Create ZegoExpressEngine
    ZegoExpressEngine.createEngine(appID, appSign, isTestEnv, scenario, enablePlatformView: enablePlatformView);

    ZegoConfig.instance.appID = appID;
    ZegoConfig.instance.appSign = appSign;
    ZegoConfig.instance.saveConfig();

    Navigator.of(context).push(MaterialPageRoute(builder: (BuildContext context) {
      return LoginRoomPage(widget.isPublish);
    }));

  }

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
      resizeToAvoidBottomPadding: false,
      appBar: AppBar(
        title: Text('Step1 CreateEngine'),
      ),
      body: SafeArea(
          child: GestureDetector(

            behavior: HitTestBehavior.translucent,
            onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

            child: Container(
              padding: const EdgeInsets.symmetric(horizontal: 40.0),
              child: Column(
                children: <Widget>[
                  Padding(
                    padding: const EdgeInsets.only(top: 20.0),
                  ),
                  Row(
                    children: <Widget>[
                      Text('Native SDK Version: '),
                      Padding(
                        padding: const EdgeInsets.only(left: 10.0),
                      ),
                      Expanded(
                        child: Text('$_version'),
                      ),
                    ],
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 20.0),
                  ),
                  Row(
                    children: <Widget>[
                      Text('User ID: '),
                      Padding(
                        padding: const EdgeInsets.only(left: 10.0),
                      ),
                      Text(ZegoConfig.instance.userID??'unknown'),
                    ],
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 10.0),
                  ),
                  Row(
                    children: <Widget>[
                      Text('User Name: '),
                      Padding(
                        padding: const EdgeInsets.only(left: 10.0),
                      ),
                      Text(ZegoConfig.instance.userName??'unknown'),
                    ],
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 10.0),
                  ),
                  Row(
                    children: <Widget>[
                      Text('AppID:'),
                      GestureDetector(
                        child: Container(
                          width: 40.0,
                          height: 40.0,
                          child: Image(
                              image: AssetImage('resources/images/settings_tips.png')
                          ),
                        ),
                        onTap: () {
                          ZegoUITool.showAlert(context, 'AppID and AppSign are the unique identifiers of each customer, please apply on https://zego.im');
                        },
                      ),
                    ],
                  ),
                  TextField(
                    controller: _appIDEdController,
                    decoration: InputDecoration(
                      contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                      hintText: 'Please enter AppID',
                      enabledBorder: OutlineInputBorder(
                        borderSide: BorderSide(
                          color: Colors.grey,
                        )
                      ),
                      focusedBorder: OutlineInputBorder(
                          borderSide: BorderSide(
                            color: Color(0xff0e88eb),
                          )
                      ),
                    ),

                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 10.0),
                  ),
                  Row(
                    children: <Widget>[

                      Text('AppSign:'),
                    ],
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 10.0),
                  ),
                  TextField(
                    controller: _appSignEdController,
                    decoration: InputDecoration(
                      contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
                      hintText: 'Please enter AppSign',
                      enabledBorder: OutlineInputBorder(
                          borderSide: BorderSide(
                            color: Colors.grey,
                          )
                      ),
                      focusedBorder: OutlineInputBorder(
                          borderSide: BorderSide(
                            color: Color(0xff0e88eb),
                          )
                      ),
                    ),
                  ),
                  Padding(
                    padding: const EdgeInsets.only(top: 10.0),
                  ),
                  Row(
                    children: <Widget>[
                      Text('SDK Environment  '),
                      Expanded(
                        child: Text('(Please select the environment corresponding to AppID)',
                          style: TextStyle(
                            fontSize: 10.0
                          ),
                        ),
                      ),
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Checkbox(
                        value: ZegoConfig.instance.isTestEnv,
                        onChanged: (value) {
                          setState(() {
                            ZegoConfig.instance.isTestEnv = value;
                            ZegoConfig.instance.saveConfig();
                          });
                        },
                      ),
                      Text('Test'),

                      Checkbox(
                        value: !ZegoConfig.instance.isTestEnv,
                        onChanged: (value) {
                          setState(() {
                            ZegoConfig.instance.isTestEnv = !value;
                            ZegoConfig.instance.saveConfig();
                          });
                        },
                      ),
                      Text('Formal'),
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Text('Rendering options  '),
                      Expanded(
                        child: Text('(Ways to render video frames)',
                          style: TextStyle(
                            fontSize: 10.0
                          ),
                        ),
                      ),
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Checkbox(
                        value: !ZegoConfig.instance.enablePlatformView,
                        onChanged: (value) {
                          setState(() {
                            ZegoConfig.instance.enablePlatformView = !value;
                            ZegoConfig.instance.saveConfig();
                          });
                        },
                      ),
                      Text('TextureRenderer'),
                      Checkbox(
                        value: ZegoConfig.instance.enablePlatformView,
                        onChanged: (value) {
                          setState(() {
                            ZegoConfig.instance.enablePlatformView = value;
                            ZegoConfig.instance.saveConfig();
                          });
                        },
                      ),
                      Text('PlatformView'),
                    ],
                  ),
                  Container(
                    padding: const EdgeInsets.all(0.0),
                    decoration: BoxDecoration(
                      borderRadius: BorderRadius.circular(12.0),
                      color: Color(0xff0e88eb),
                    ),
                    width: 240.0,
                    height: 60.0,
                    child: CupertinoButton(
                      child: Text('Create Engine',
                        style: TextStyle(
                          color: Colors.white
                        ),
                      ),
                      onPressed: onButtonPressed,
                    ),
                  )
                ],
              ),
            ),
          )
      ),
    );
  }

}