//
//  global_setting_page.dart
//  zego-express-example-topics-flutter
//
//  Created by Patrick Fu on 2020/11/23.
//  Copyright © 2020 Zego. All rights reserved.
//

import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';
import 'package:permission_handler/permission_handler.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/utils/zego_config.dart';
import 'package:zego_express_engine_example/utils/zego_utils.dart';

class GlobalSettingPage extends StatefulWidget {
  @override
  _GlobalSettingPageState createState() => new _GlobalSettingPageState();
}

class _GlobalSettingPageState extends State<GlobalSettingPage> {

  final TextEditingController _appIDEdController = new TextEditingController();
  final TextEditingController _appSignEdController = new TextEditingController();

  String _version;

  bool _isTestEnv = true;
  ZegoScenario _scenario = ZegoScenario.General;
  bool _enablePlatformView = false;

  bool _isCameraPermissionGranted = false;
  bool _isMicrophonePermissionGranted = false;

  @override
  void initState() {
    super.initState();

    ZegoExpressEngine.getVersion().then((value) {
      setState(() => _version = value);
    });

    if (ZegoConfig.instance.appID > 0) {
      _appIDEdController.text = ZegoConfig.instance.appID.toString();
    }

    if (ZegoConfig.instance.appSign.isNotEmpty) {
      _appSignEdController.text = ZegoConfig.instance.appSign;
    }

    _isTestEnv = ZegoConfig.instance.isTestEnv;
    _scenario = ZegoConfig.instance.scenario;
    _enablePlatformView = ZegoConfig.instance.enablePlatformView;

    Permission.camera.status.then((value) => setState(() => _isCameraPermissionGranted = value == PermissionStatus.granted));
    Permission.microphone.status.then((value) => setState(() => _isMicrophonePermissionGranted = value == PermissionStatus.granted));
  }

  Future<void> requestCameraPermission() async {
    PermissionStatus cameraStatus = await Permission.camera.request();
    setState(() => _isCameraPermissionGranted = cameraStatus.isGranted);
  }

  Future<void> requestMicrophonePermission() async {
    PermissionStatus microphoneStatus = await Permission.microphone.request();
    setState(() => _isMicrophonePermissionGranted = microphoneStatus.isGranted);
  }

  Future<bool> _onWillPop() {
    return showDialog(context: context, builder: (BuildContext context) {
      return AlertDialog(
        title: Text('Do you need to save the settings before exiting?'),
        actions: <Widget>[
          FlatButton(
            child: Text('NO, EXIT'),
            onPressed: () {
              Navigator.of(context).pop(true);
            },
          ),
          FlatButton(
            child: Text('OK, SAVE'),
            onPressed: () {
              Navigator.of(context).pop(true);
              _onSaveButtonClicked();
            },
          ),
        ],
      );
    });
  }

  void _onSaveButtonClicked() {

    String strAppID = _appIDEdController.text.trim();
    String appSign = _appSignEdController.text.trim();

    // if (strAppID.isEmpty || appSign.isEmpty) {
    //   ZegoUtils.showAlert(context, 'AppID or AppSign cannot be empty');
    //   return;
    // }
    //
    int appID = int.tryParse(strAppID);
    // if (appID == null) {
    //   ZegoUtils.showAlert(context, 'AppID is invalid, should be int');
    //   return;
    // }

    if (!_isCameraPermissionGranted) {
      ZegoUtils.showAlert(context, 'Camera permission is not granted, please click the camera icon to request permission');
      return;
    }

    if (!_isMicrophonePermissionGranted) {
      ZegoUtils.showAlert(context, 'Microphone permission is not granted, please click the mic icon to request permission');
      return;
    }

    ZegoConfig.instance.appID = appID;
    ZegoConfig.instance.appSign = appSign;
    ZegoConfig.instance.isTestEnv = this._isTestEnv;
    ZegoConfig.instance.scenario = this._scenario;
    ZegoConfig.instance.enablePlatformView = this._enablePlatformView;
    ZegoConfig.instance.saveConfig();
  }

  // ----- Widgets -----

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      // resizeToAvoidBottomPadding: false,
      appBar: AppBar(
        title: Text('Setting'),
        actions: [
          IconButton(icon: Icon(Icons.save), onPressed: _onSaveButtonClicked)
        ],
      ),
      body: SafeArea(
        child: WillPopScope(
          onWillPop: _onWillPop,
          child: GestureDetector(
            behavior: HitTestBehavior.translucent,
            onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),

            child: SingleChildScrollView(
              padding: const EdgeInsets.symmetric(horizontal: 30.0),
              child: Column(
                children: <Widget>[
                  userInfoWidget(),
                  checkPermissionWidget(),

                  appIDWidget(),
                  appSignWidget(),

                  selectEnvironmentWidget(),
                  selectScenarioWidget(),
                  selectRendererWidget(),
                ],
              ),
            ),
          )
        )
      ),
    );
  }

  Widget checkPermissionWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(padding: const EdgeInsets.only(top: 20.0)),
        Text("Permission status: (Press icon to request)"),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(
              children: [
                IconButton(
                  icon: _isCameraPermissionGranted ? Icon(Icons.camera_alt) : Icon(Icons.camera_alt_outlined),
                  iconSize: 50.0,
                  onPressed: requestCameraPermission
                ),
                Text(_isCameraPermissionGranted ? '✅' : '❗️')
              ],
            ),
            SizedBox(width: 50),
            Row(
              children: [
                IconButton(
                  icon: _isMicrophonePermissionGranted ? Icon(Icons.mic) : Icon(Icons.mic_none),
                  iconSize: 50.0,
                  onPressed: requestMicrophonePermission
                ),
                Text(_isMicrophonePermissionGranted ? '✅' : '❗️')
              ],
            )
          ],
        ),
      ],
    );
  }

  Widget userInfoWidget() {
    return Column(
      children: [
        Padding(padding: const EdgeInsets.only(top: 20.0)),
        Row(
          children: <Widget>[
            Text('Native SDK Version: '),
            Expanded(child: Text('$_version')),
          ],
        ),
        Padding(padding: const EdgeInsets.only(top: 10.0)),
        Row(
          children: <Widget>[
            Text('User ID: '),
            Padding(padding: const EdgeInsets.only(left: 10.0)),
            Text(ZegoConfig.instance.userID??'unknown'),
          ],
        ),
        Padding(
          padding: const EdgeInsets.only(top: 10.0),
        ),
        Row(
          children: <Widget>[
            Text('User Name: '),
            Padding(padding: const EdgeInsets.only(top: 10.0)),
            Text(ZegoConfig.instance.userName??'unknown'),
          ],
        ),
      ]
    );
  }

  Widget appIDWidget() {
    return Column(
      children: [
        Row(
          children: <Widget>[
            Text('AppID:'),
            GestureDetector(
              child: Icon(Icons.help_outline),
              onTap: () {
                ZegoUtils.showAlert(context, 'AppID and AppSign are the unique identifiers of each customer, please apply on https://zego.im');
              },
            ),
          ],
        ),
        TextField(
          controller: _appIDEdController,
          decoration: InputDecoration(
            contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
            hintText: 'Please enter AppID',
            enabledBorder: OutlineInputBorder(borderSide: BorderSide(color: Colors.grey)),
            focusedBorder: OutlineInputBorder(borderSide: BorderSide(color: Color(0xff0e88eb))),
          ),
        ),
      ],
    );
  }

  Widget appSignWidget() {
    return Column(
      children: [
        Padding(padding: const EdgeInsets.only(top: 10.0)),
        Row(
          children: <Widget>[
            Text('AppSign:'),
            GestureDetector(
              child: Icon(Icons.help_outline),
              onTap: () {
                ZegoUtils.showAlert(context, 'AppID and AppSign are the unique identifiers of each customer, please apply on https://zego.im');
              },
            ),
          ],
        ),
        TextField(
          controller: _appSignEdController,
          decoration: InputDecoration(
            contentPadding: const EdgeInsets.only(left: 10.0, top: 12.0, bottom: 12.0),
            hintText: 'Please enter AppSign',
            enabledBorder: OutlineInputBorder(borderSide: BorderSide(color: Colors.grey)),
            focusedBorder: OutlineInputBorder(borderSide: BorderSide(color: Color(0xff0e88eb))),
          ),
        ),
      ],
    );
  }

  Widget selectEnvironmentWidget() {
    void onEnvironmentChanged(int value) {
      setState(() => this._isTestEnv = value > 0 ? true : false);
    };

    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(padding: const EdgeInsets.only(top: 20.0)),
        Text('SDK Environment'),
        Text('(Please select the environment corresponding to AppID)', style: TextStyle(fontSize: 10.0)),
        Row(
          children: <Widget>[
            Radio(
              value: 1,
              groupValue: this._isTestEnv ? 1 : 0,
              onChanged: onEnvironmentChanged,
            ),
            Text('Test'),
            SizedBox(width: 50),
            Radio(
              value: 0,
              groupValue: this._isTestEnv ? 1 : 0,
              onChanged: onEnvironmentChanged,
            ),
            Text('Formal'),
          ],
        ),
      ],
    );
  }

  Widget selectScenarioWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(padding: const EdgeInsets.only(top: 20.0)),
        Text('Scenario'),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            ChoiceChip(
              label: Text('General'),
              selected: this._scenario.index == 0,
              onSelected: (value) => setState(() => this._scenario = ZegoScenario.General),
            ),
            ChoiceChip(
              label: Text('Communication'),
              selected: this._scenario.index == 1,
              onSelected: (value) => setState(() => this._scenario = value ? ZegoScenario.Communication : ZegoScenario.General),
            ),
            ChoiceChip(
              label: Text('Live'),
              selected: this._scenario.index == 2,
              onSelected: (value) => setState(() => this._scenario = value ? ZegoScenario.Live : ZegoScenario.General),
            ),
          ],
        ),
      ],
    );
  }

  Widget selectRendererWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Padding(padding: const EdgeInsets.only(top: 20.0)),
        Text('Rendering options'),
        Text('(Ways to render video frames)', style: TextStyle(fontSize: 10.0)),
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
          children: <Widget>[
            ChoiceChip(
              label: Text('TextureRenderer'),
              selected: !this._enablePlatformView,
              onSelected: (value) => setState(() => this._enablePlatformView = !value),
            ),
            ChoiceChip(
              label: Text('PlatformView'),
              selected: this._enablePlatformView,
              onSelected: (value) => setState(() => this._enablePlatformView = value),
            ),
          ],
        ),
      ]
    );
  }

}