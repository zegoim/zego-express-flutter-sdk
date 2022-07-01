//
//  quick_start_page.dart
//  zego-express-example-topics-flutter
//
//  Created by Patrick Fu on 2020/12/04.
//  Copyright © 2020 Zego. All rights reserved.
//
import 'package:flutter/cupertino.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/utils/zego_config.dart';

import 'package:universal_io/io.dart';

import 'dart:html' as html show window;

class QuickStartPage extends StatefulWidget {
  @override
  _QuickStartPageState createState() => _QuickStartPageState();
}

class _QuickStartPageState extends State<QuickStartPage> {

  int _previewViewID = -1;
  int _playViewID = -1;
  Widget? _previewViewWidget;
  Widget? _playViewWidget;
  GlobalKey _playViewContainerKey = GlobalKey();
  GlobalKey _previewViewContainerKey = GlobalKey();
  static const double viewRatio = 3.0 / 4.0;

  ZegoMediaPlayer? mediaPlayer;

  bool _isEngineActive = false;
  ZegoRoomState _roomState = ZegoRoomState.Disconnected;
  ZegoPublisherState _publisherState = ZegoPublisherState.NoPublish;
  ZegoPlayerState _playerState = ZegoPlayerState.NoPlay;

  TextEditingController _publishingStreamIDController =
      new TextEditingController();
  TextEditingController _playingStreamIDController =
      new TextEditingController();
  TextEditingController _appIDcontroller =
      new TextEditingController();
  TextEditingController _roomIDcontroller =
      new TextEditingController();
  TextEditingController _userIDcontroller =
      new TextEditingController();
  TextEditingController _tokencontroller =
      new TextEditingController();

  String _roomID = "";

  bool muteVideo = false;
  bool muteAudio = false;
  bool enableMic = true;

  @override
  void initState() {
    super.initState();
    ZegoExpressEngine.getVersion()
        .then((value) => print('🌞 SDK Version: $value'));

    setZegoEventCallback();
  }

  @override
  void dispose() {
    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine()
        .then((value) => print('async destroy success'));

    print('🏳️ Destroy ZegoExpressEngine');

    clearZegoEventCallback();

    super.dispose();
  }

  // MARK: - Step 1: CreateEngine

  void createEngine() {
    ZegoEngineProfile profile = ZegoEngineProfile(
      _appIDcontroller.text.trim().isEmpty ? ZegoConfig.instance.appID : int.parse(_appIDcontroller.text.trim()) ,
      // int.parse(_appIDcontroller.text.trim()),
      ZegoConfig.instance.scenario,
      enablePlatformView: ZegoConfig.instance.enablePlatformView,
      appSign: ZegoConfig.instance.appSign);
    ZegoExpressEngine.createEngineWithProfile(profile);

    // Notify View that engine state changed
    setState(() => _isEngineActive = true);

    print('🚀 Create ZegoExpressEngine');

    // ZegoExpressEngine.instance
    //     .getAudioConfig()
    //     .then((value) => print('get audio config: $value'));
  }

  // MARK: - Step 2: LoginRoom

  void loginRoom() {
    // Instantiate a ZegoUser object
    ZegoUser user =
        ZegoUser(ZegoConfig.instance.userID, ZegoConfig.instance.userName.isEmpty? ZegoConfig.instance.userID: ZegoConfig.instance.userName);

        ZegoUser(_userIDcontroller.text.trim().isEmpty ? ZegoConfig.instance.userID : _userIDcontroller.text.trim(),
          ZegoConfig.instance.userName);

    if (kIsWeb) {
      ZegoRoomConfig config = ZegoRoomConfig.defaultConfig();
      config.token = ZegoConfig.instance.token;
      // Login Room WEB only supports token;
      ZegoExpressEngine.instance.loginRoom(_roomID, user, config: config);
    } else {
      // Login Room
      ZegoExpressEngine.instance.loginRoom(_roomID, user);
    }




    print('🚪 Start login room, roomID: $_roomID');
  }

  void logoutRoom() {
    // Logout room will automatically stop publishing/playing stream.
    //
    // But directly logout room without destroying the [PlatformView]
    // or [TextureRenderer] may cause a memory leak.
    ZegoExpressEngine.instance.logoutRoom(_roomID);
    print('🚪 logout room, roomID: $_roomID');

    clearPreviewView();
    clearPlayView();
  }

  // MARK: - Step 3: StartPublishingStream
  void startPreview( {double width = 360, double height = 640}) {
    Future<void> _startPreview(int viewID) async {
      ZegoCanvas canvas = ZegoCanvas.view(viewID);
      await ZegoExpressEngine.instance.startPreview(canvas: canvas);
      print('🔌 Start preview, viewID: $viewID');
    }

    ZegoExpressEngine.instance.setVideoConfig(ZegoVideoConfig.preset(ZegoVideoConfigPreset.Preset1080P));
    ZegoExpressEngine.instance.enableAGC(false);
    ZegoExpressEngine.instance.enableANS(false);
    ZegoExpressEngine.instance.enableAEC(false);

    if (Platform.isIOS || Platform.isAndroid || kIsWeb) {
      if (ZegoConfig.instance.enablePlatformView || kIsWeb ) {
        // Render with PlatformView
        setState(() {
          _previewViewWidget =
              ZegoExpressEngine.instance.createPlatformView((viewID) async {
            _previewViewID = viewID;
            await _startPreview(_previewViewID);
          });
        });
      } else {
        // Render with TextureRenderer
        ZegoExpressEngine.instance
            .createTextureRenderer(width.toInt(), height.toInt())
            .then((viewID) {
          _previewViewID = viewID;
          setState(() => _previewViewWidget = Texture(textureId: viewID));
          _startPreview(viewID);
        });
      }
    } else {
      ZegoExpressEngine.instance.startPreview();
    }
  }

  void stopPreview() {
    if (!Platform.isAndroid && !Platform.isIOS && !kIsWeb) return;

    if (_previewViewWidget == null) {
      return;
    }

    ZegoExpressEngine.instance.stopPreview();
    clearPreviewView();
  }

  void startPublishingStream(String streamID) {
    ZegoExpressEngine.instance.startPublishingStream(streamID);
    print('📤 Start publishing stream, streamID: $streamID');
  }

  void stopPublishingStream() {
    ZegoExpressEngine.instance.stopPublishingStream();
  }

  // MARK: - Step 4: StartPlayingStream

  void startPlayingStream(String streamID,
      {double width = 360, double height = 640}) {
    void _startPlayingStream(int viewID, String streamID) {
      ZegoCanvas canvas = ZegoCanvas.view(viewID);
      ZegoExpressEngine.instance.startPlayingStream(streamID, canvas: canvas);
      print('📥 Start playing stream, streamID: $streamID, viewID: $viewID');
    }

    if (Platform.isIOS || Platform.isAndroid || kIsWeb) {
      if (ZegoConfig.instance.enablePlatformView || kIsWeb) {
        // Render with PlatformView
        html.window.console.error("startPlayingStream create view");
        setState(() {
          _playViewWidget =
              ZegoExpressEngine.instance.createPlatformView((viewID) {
            _playViewID = viewID;
            _startPlayingStream(viewID, streamID);
          });
        });
      } else {
        // Render with TextureRenderer
        ZegoExpressEngine.instance
            .createTextureRenderer(width.toInt(), height.toInt())
            .then((viewID) {
          _playViewID = viewID;
          setState(() => _playViewWidget = Texture(textureId: viewID));
          _startPlayingStream(viewID, streamID);
        });
      }
    } else {
      ZegoExpressEngine.instance.startPlayingStream(streamID);
    }
  }

  void stopPlayingStream(String streamID) {
    ZegoExpressEngine.instance.stopPlayingStream(streamID);
    clearPlayView();
  }

  // MARK: - Exit

  void destroyEngine() async {
    stopPreview();
    clearPreviewView();
    clearPlayView();

    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine()
        .then((ret) => print('already destroy engine'));

    print('🏳️ Destroy ZegoExpressEngine');

    // Notify View that engine state changed
    setState(() {
      _isEngineActive = false;
      _roomState = ZegoRoomState.Disconnected;
      _publisherState = ZegoPublisherState.NoPublish;
      _playerState = ZegoPlayerState.NoPlay;
    });
  }

  // MARK: - Zego Event

  void setZegoEventCallback() {
    ZegoExpressEngine.onRoomStateUpdate = (String roomID, ZegoRoomState state,
        int errorCode, Map<String, dynamic> extendedData) {
      print(
          '🚩 🚪 Room state update, state: $state, errorCode: $errorCode, roomID: $roomID');
      setState(() => _roomState = state);
    };

    ZegoExpressEngine.onPublisherStateUpdate = (String streamID,
        ZegoPublisherState state,
        int errorCode,
        Map<String, dynamic> extendedData) {
      print(
          '🚩 📤 Publisher state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _publisherState = state);
    };

    ZegoExpressEngine.onPlayerStateUpdate = (String streamID,
        ZegoPlayerState state,
        int errorCode,
        Map<String, dynamic> extendedData) {
      print(
          '🚩 📥 Player state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _playerState = state);

      if (_playerState == ZegoPlayerState.NoPlay) {
        stopPlayingStream(_playingStreamIDController.text.trim());
      }
    };

    ZegoExpressEngine.onRoomUserUpdate = (roomID, updateType, userList) {

      userList.forEach((e) {
        var userID = e.userID;
        var userName = e.userName;
         print(
          '🚩 🚪 Room user update, roomID: $roomID, updateType: $updateType userID: $userID userName: $userName');
      });
    };

    ZegoExpressEngine.onRoomStreamUpdate = ((roomID, updateType, streamList, extendedData) {
        streamList.forEach((stream) {
          var streamID = stream.streamID;
          print('🚩 🚪 Room stream update, roomID: $roomID, updateType: $updateType streamID:$streamID');

          if (updateType == ZegoPlayerState.NoPlay) {
            stopPlayingStream(_playingStreamIDController.text.trim());
          }
        });
    });
  }

  void clearZegoEventCallback() {
    ZegoExpressEngine.onRoomStateUpdate = null;
    ZegoExpressEngine.onPublisherStateUpdate = null;
    ZegoExpressEngine.onPlayerStateUpdate = null;
  }

  void clearPreviewView() {
    if (!Platform.isAndroid && !Platform.isIOS && !kIsWeb) return;

    if (_previewViewWidget == null) {
      return;
    }

    // Developers should destroy the [PlatformView] or [TextureRenderer] after
    // [stopPublishingStream] or [stopPreview] to release resource and avoid memory leaks
    if (ZegoConfig.instance.enablePlatformView || kIsWeb) {
      ZegoExpressEngine.instance.destroyPlatformView(_previewViewID);
    } else {
      ZegoExpressEngine.instance.destroyTextureRenderer(_previewViewID);
    }
    setState(() => _previewViewWidget = null);
  }

  void clearPlayView() {
    if (!Platform.isAndroid && !Platform.isIOS && !kIsWeb) return;

    if (_playViewWidget == null) {
      return;
    }

    // Developers should destroy the [PlatformView] or [TextureRenderer]
    // after [stopPlayingStream] to release resource and avoid memory leaks
    if (ZegoConfig.instance.enablePlatformView || kIsWeb) {
      ZegoExpressEngine.instance.destroyPlatformView(_playViewID);
    } else {
      ZegoExpressEngine.instance.destroyTextureRenderer(_playViewID);
    }
    setState(() => _playViewWidget = null);
  }

  void mutePublishStreamVideo() {
    if (muteVideo == true) {
      muteVideo = false;
    } else {
      muteVideo = true;
    }
    ZegoExpressEngine.instance.mutePublishStreamVideo(muteVideo);
  }

  void mutePublishStreamAudio() {
    if (muteAudio == true) {
      muteAudio = false;
    } else {
      muteAudio = true;
    }
    ZegoExpressEngine.instance.mutePublishStreamAudio(muteAudio);
  }

  void sendBroadcastMessage() {

    ZegoExpressEngine.instance.sendBroadcastMessage(_roomID, "FLUTTER WEB TEST");
  }
  // MARK: Widget

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('QuickStart')),
      body: SafeArea(
          child: GestureDetector(
        child: mainContent(),
        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),
      )),
    );
  }

  Widget mainContent() {
    return SingleChildScrollView(
        child: Column(children: [
      Divider(),
      Padding(
        padding: const EdgeInsets.symmetric(horizontal: 20.0),
        child: Column(children: [
          viewsWidget(),
          stepOneCreateEngineWidget(),
          stepTwoLoginRoomWidget(),
          stepThreeStartPublishingStreamWidget(),
          stepFourStartPlayingStreamWidget(),
          Padding(padding: const EdgeInsets.only(bottom: 20.0)),
          CupertinoButton.filled(
            child: Text(
              'DestroyEngine',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: destroyEngine,
            padding: EdgeInsets.symmetric(vertical: 10, horizontal: 50),
          )
        ]),
      ),
    ]));
  }

  Widget viewsWidget() {
    return Container(
      height: MediaQuery.of(context).size.width * 0.65,
      child: GridView(
        padding: const EdgeInsets.all(10.0),
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 2,
          mainAxisSpacing: 10.0,
          crossAxisSpacing: 10.0,
          childAspectRatio: viewRatio,
        ),
        children: [
          Stack(children: [
            Container(
              color: Colors.grey,
              child: _previewViewWidget,
              key: _previewViewContainerKey,
            ),
            Text('Local Preview View', style: TextStyle(color: Colors.white))
          ], alignment: AlignmentDirectional.topCenter),
          Stack(children: [
            Container(
              color: Colors.grey,
              child: _playViewWidget,
              key: _playViewContainerKey,
            ),
            Text('Remote Play View', style: TextStyle(color: Colors.white))
          ], alignment: AlignmentDirectional.topCenter),
        ],
      ),
    );
  }

  Widget stepOneCreateEngineWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          'Step1:',
          style: TextStyle(fontWeight: FontWeight.bold),
        ),
        Row(children: [
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: TextField(
                controller: _appIDcontroller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.all(10.0),
                  isDense: true,
                  labelText: 'appID:',
                  labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                  hintText: 'Please enter appID',
                  hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                ),
              ),
          ),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _isEngineActive ? '✅ CreateEngine' : 'CreateEngine',
                style: TextStyle(fontSize: 14.0),
              ),
              onPressed: createEngine,
              padding: EdgeInsets.all(10.0),
            ),
          )
        ]),
        Divider(),
      ],
    );
  }

  Widget stepTwoLoginRoomWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step2:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      Row(children: [
        Column(
          children: [
            Container(
              width: MediaQuery.of(context).size.width / 2.5,
              child: TextField(
                controller: _tokencontroller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.all(10.0),
                  isDense: true,
                  labelText: 'token:',
                  labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                  hintText: 'Please enter token',
                  hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                ),
              ),
            ),
            Container(
              width: MediaQuery.of(context).size.width / 2.5,
              child: TextField(
                controller: _roomIDcontroller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.all(10.0),
                  isDense: true,
                  labelText: 'roomID:',
                  labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                  hintText: 'Please enter roomID',
                  hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                ),
              ),
            ),
            Container(
              width: MediaQuery.of(context).size.width / 2.5,
              child: TextField(
                controller: _userIDcontroller,
                decoration: InputDecoration(
                  contentPadding: const EdgeInsets.all(10.0),
                  isDense: true,
                  labelText: 'userID:',
                  labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                  hintText: 'Please enter userID',
                  hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                ),
              ),
            )
          ],
          crossAxisAlignment: CrossAxisAlignment.start,
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: CupertinoButton.filled(
            child: Text(
              _roomState == ZegoRoomState.Connected
                  ? '✅ LoginRoom'
                  : 'LoginRoom',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _roomState == ZegoRoomState.Disconnected
                ? loginRoom
                : logoutRoom,
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
    ]);
  }

  Widget stepThreeStartPublishingStreamWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step3:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      SizedBox(height: 10),
      Row(children: [
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: TextField(
            controller: _publishingStreamIDController,
            decoration: InputDecoration(
                contentPadding: const EdgeInsets.all(10.0),
                isDense: true,
                labelText: 'Publish StreamID:',
                labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                hintText: 'Please enter streamID',
                hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Color(0xff0e88eb)))),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text(
              _previewViewWidget != null
                  ? '✅ StartPreview'
                  : 'StartPreview',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _previewViewWidget == null
                ? () {
                    double pixelRatio = MediaQuery.of(context).devicePixelRatio;
                    Size? widgetSize =
                        _previewViewContainerKey.currentContext?.size;
                    startPreview(
                        width: widgetSize!.width * pixelRatio,
                        height: widgetSize.height * pixelRatio);
                  }
                : () {
                    stopPreview();
                  },
            padding: EdgeInsets.all(10.0),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text(
              _publisherState == ZegoPublisherState.Publishing
                  ? '✅ StartPublishing'
                  : 'StartPublishing',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: _publisherState == ZegoPublisherState.NoPublish
                ? () {
                    double pixelRatio = MediaQuery.of(context).devicePixelRatio;
                    Size? widgetSize =
                        _previewViewContainerKey.currentContext?.size;
                    startPublishingStream(
                        _publishingStreamIDController.text.trim());
                  }
                : () {
                    stopPublishingStream();
                  },
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
      Row(children: [
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text('mutePublishStreamVideo',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: mutePublishStreamVideo,
            padding: EdgeInsets.all(10.0),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text('mutePublishStreamAudio',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: mutePublishStreamAudio,
            padding: EdgeInsets.all(10.0),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 5,
          child: CupertinoButton.filled(
            child: Text("sendBroadcastMessage",
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed: sendBroadcastMessage,
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
    ]);
  }

  Widget stepFourStartPlayingStreamWidget() {
    return Column(crossAxisAlignment: CrossAxisAlignment.start, children: [
      Text(
        'Step4:',
        style: TextStyle(fontWeight: FontWeight.bold),
      ),
      SizedBox(height: 10),
      Row(children: [
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: TextField(
            controller: _playingStreamIDController,
            decoration: InputDecoration(
                contentPadding: const EdgeInsets.all(10.0),
                isDense: true,
                labelText: 'Play StreamID:',
                labelStyle: TextStyle(color: Colors.black54, fontSize: 14.0),
                hintText: 'Please enter streamID',
                hintStyle: TextStyle(color: Colors.black26, fontSize: 10.0),
                enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Color(0xff0e88eb)))),
          ),
        ),
        Spacer(),
        Container(
          width: MediaQuery.of(context).size.width / 2.5,
          child: CupertinoButton.filled(
            child: Text(
              _playerState == ZegoPlayerState.Playing
                  ? '✅ StartPlaying'
                  : 'StartPlaying',
              style: TextStyle(fontSize: 14.0),
            ),
            onPressed:
            _playerState == ZegoPlayerState.NoPlay
                ? () {
                    double pixelRatio = MediaQuery.of(context).devicePixelRatio;
                    Size? widgetSize =
                        _playViewContainerKey.currentContext?.size;
                    startPlayingStream(_playingStreamIDController.text.trim(),
                        width: widgetSize!.width * pixelRatio,
                        height: widgetSize.height * pixelRatio);
                  }
                : () {
                    stopPlayingStream(_playingStreamIDController.text.trim());
                  },
            padding: EdgeInsets.all(10.0),
          ),
        )
      ]),
      Divider(),
    ]);
  }

}
