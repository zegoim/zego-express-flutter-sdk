//
//  quick_start_page.dart
//  zego-express-example-topics-flutter
//
//  Created by Patrick Fu on 2020/12/04.
//  Copyright © 2020 Zego. All rights reserved.
//

import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

import 'package:zego_express_engine/zego_express_engine.dart';

import 'package:zego_express_engine_example/utils/zego_config.dart';

class QuickStartPage extends StatefulWidget {
  @override
  _QuickStartPageState createState() => _QuickStartPageState();
}

class _QuickStartPageState extends State<QuickStartPage> {

  final String _roomID = 'QuickStartRoom-1';

  Widget _previewViewWidget;
  Widget _playViewWidget;
  GlobalKey _playViewContainerKey = GlobalKey();
  GlobalKey _previewViewContainerKey = GlobalKey();
  static const double viewRatio = 3.0/4.0;

  bool _isEngineActive = false;
  ZegoRoomState _roomState = ZegoRoomState.Disconnected;
  ZegoPublisherState _publisherState = ZegoPublisherState.NoPublish;
  ZegoPlayerState _playerState = ZegoPlayerState.NoPlay;

  TextEditingController _publishingStreamIDController = new TextEditingController();
  TextEditingController _playingStreamIDController = new TextEditingController();

  @override
  void initState() {
    super.initState();
    ZegoExpressEngine.getVersion().then((value) => print('🌞 SDK Version: $value}'));

    setZegoEventCallback();
  }

  @override
  void dispose() {
    // Logout room will automatically stop publishing/playing stream.
    // ZegoExpressEngine.instance.logoutRoom(_roomID);

    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    ZegoExpressEngine.destroyEngine();

    print('🏳️ Destroy ZegoExpressEngine');

    clearZegoEventCallback();

    super.dispose();
  }


  // MARK: - Step 1: CreateEngine

  void createEngine() {
    ZegoExpressEngine.createEngine(ZegoConfig.instance.appID, ZegoConfig.instance.appSign, ZegoConfig.instance.isTestEnv, ZegoConfig.instance.scenario, enablePlatformView: ZegoConfig.instance.enablePlatformView);

    // Notify View that engine state changed
    setState(() => _isEngineActive = true);

    print('🚀 Create ZegoExpressEngine');
  }

  // MARK: - Step 2: LoginRoom

  void loginRoom() {
    // Instantiate a ZegoUser object
    ZegoUser user = ZegoUser(ZegoConfig.instance.userID, ZegoConfig.instance.userName);

    // Login Room
    ZegoExpressEngine.instance.loginRoom(_roomID, user);

    print('🚪 Start login room, roomID: $_roomID');
  }

  // MARK: - Step 3: StartPublishingStream

  void startPublishingStream(String streamID, {double width = 360, double height = 640}) {

    void _startPreview(int viewID) {
      ZegoCanvas canvas = ZegoCanvas.view(viewID);
      ZegoExpressEngine.instance.startPreview(canvas: canvas);
      print('🔌 Start preview, viewID: $viewID');
    }

    void _startPublishingStream(String streamID) {
      ZegoExpressEngine.instance.startPublishingStream(streamID);
      print('📤 Start publishing stream, streamID: $streamID');
    }

    if (ZegoConfig.instance.enablePlatformView) {
      // Render with PlatformView
      setState(() {
        _previewViewWidget = ZegoExpressEngine.instance.createPlatformView((viewID) {
          _startPreview(viewID);
          _startPublishingStream(streamID);
        });
      });
    } else {
      // Render with TextureRenderer
      ZegoExpressEngine.instance.createTextureRenderer(width.toInt(), height.toInt()).then((viewID) {
        setState(() => _previewViewWidget = Texture(textureId: viewID));
        _startPreview(viewID);
        _startPublishingStream(streamID);
      });
    }
  }

  // MARK: - Step 4: StartPlayingStream

  void startPlayingStream(String streamID, {double width = 360, double height = 640}) {

    void _startPlayingStream(int viewID, String streamID) {
      ZegoCanvas canvas = ZegoCanvas.view(viewID);
      ZegoExpressEngine.instance.startPlayingStream(streamID, canvas: canvas);
      print('📥 Start playing stream, streamID: $streamID, viewID: $viewID');
    }

    if (ZegoConfig.instance.enablePlatformView) {
      // Render with PlatformView
      setState(() {
        _playViewWidget = ZegoExpressEngine.instance.createPlatformView((viewID) {
          _startPlayingStream(viewID, streamID);
        });
      });
    } else {
      // Render with TextureRenderer
      ZegoExpressEngine.instance.createTextureRenderer(width.toInt(), height.toInt()).then((viewID) {
        setState(() => _playViewWidget = Texture(textureId: viewID));
        _startPlayingStream(viewID, streamID);
      });
    }
  }

  // MARK: - Exit

  void destroyEngine() async {
    // Logout room will automatically stop publishing/playing stream.
    // ZegoExpressEngine.instance.logoutRoom(_roomID);

    // Can destroy the engine when you don't need audio and video calls
    //
    // Destroy engine will automatically logout room and stop publishing/playing stream.
    await ZegoExpressEngine.destroyEngine();

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
    ZegoExpressEngine.onRoomStateUpdate = (String roomID, ZegoRoomState state, int errorCode, Map<String, dynamic> extendedData) {
      print('🚩 🚪 Room state update, state: $state, errorCode: $errorCode, roomID: $roomID');
      setState(() => _roomState = state);
    };

    ZegoExpressEngine.onPublisherStateUpdate = (String streamID, ZegoPublisherState state, int errorCode, Map<String, dynamic> extendedData) {
      print('🚩 📤 Publisher state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _publisherState = state);
    };

    ZegoExpressEngine.onPlayerStateUpdate = (String streamID, ZegoPlayerState state, int errorCode, Map<String, dynamic> extendedData) {
      print('🚩 📥 Player state update, state: $state, errorCode: $errorCode, streamID: $streamID');
      setState(() => _playerState = state);
    };
  }

  void clearZegoEventCallback() {
    ZegoExpressEngine.onRoomStateUpdate = null;
    ZegoExpressEngine.onPublisherStateUpdate = null;
    ZegoExpressEngine.onPlayerStateUpdate = null;
  }

  // MARK: Widget

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('QuickStart')),
      body: SafeArea(child: GestureDetector(
        child: mainContent(),
        behavior: HitTestBehavior.translucent,
        onTap: () => FocusScope.of(context).requestFocus(new FocusNode()),
      )),
    );
  }

  Widget mainContent() {
    return SingleChildScrollView(child: Column(children: [
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
            child: Text('DestroyEngine', style: TextStyle(fontSize: 14.0),),
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
        Text('Step1:', style: TextStyle(fontWeight: FontWeight.bold),),
        Row(children: [
          Column(children: [
            Text('AppID: ${ZegoConfig.instance.appID}', style: TextStyle(fontSize: 10)),
            Text('isTestEnv: ${ZegoConfig.instance.isTestEnv}', style: TextStyle(fontSize: 10))
          ], crossAxisAlignment: CrossAxisAlignment.start,),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _isEngineActive ? '✅ CreateEngine': 'CreateEngine',
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
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text('Step2:', style: TextStyle(fontWeight: FontWeight.bold),),
        Row(children: [
          Column(children: [
            Text('RoomID: $_roomID', style: TextStyle(fontSize: 10)),
            Text('UserID: ${ZegoConfig.instance.userID}', style: TextStyle(fontSize: 10)),
          ], crossAxisAlignment: CrossAxisAlignment.start,),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _roomState == ZegoRoomState.Connected ? '✅ LoginRoom' : 'LoginRoom',
                style: TextStyle(fontSize: 14.0),
              ),
              onPressed: loginRoom,
              padding: EdgeInsets.all(10.0),
            ),
          )
        ]),
        Divider(),
      ]
    );
  }

  Widget stepThreeStartPublishingStreamWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text('Step3:', style: TextStyle(fontWeight: FontWeight.bold),),
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
                enabledBorder: OutlineInputBorder(borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(borderSide: BorderSide(color: Color(0xff0e88eb)))
              ),
            ),
          ),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _publisherState == ZegoPublisherState.Publishing ? '✅ StartPublishing' : 'StartPublishing',
                style: TextStyle(fontSize: 14.0),
              ),
              onPressed: () {
                double pixelRatio = MediaQuery.of(context).devicePixelRatio;
                Size widgetSize = _previewViewContainerKey.currentContext.size;
                startPublishingStream(_publishingStreamIDController.text.trim(), width: widgetSize.width * pixelRatio, height: widgetSize.height * pixelRatio);
              },
              padding: EdgeInsets.all(10.0),
            ),
          )
        ]),
        Divider(),
      ]
    );
  }

  Widget stepFourStartPlayingStreamWidget() {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text('Step4:', style: TextStyle(fontWeight: FontWeight.bold),),
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
                enabledBorder: OutlineInputBorder(borderSide: BorderSide(color: Colors.grey)),
                focusedBorder: OutlineInputBorder(borderSide: BorderSide(color: Color(0xff0e88eb)))
              ),
            ),
          ),
          Spacer(),
          Container(
            width: MediaQuery.of(context).size.width / 2.5,
            child: CupertinoButton.filled(
              child: Text(
                _playerState == ZegoPlayerState.Playing ? '✅ StartPlaying' : 'StartPlaying',
                style: TextStyle(fontSize: 14.0),
              ),
              onPressed: () {
                double pixelRatio = MediaQuery.of(context).devicePixelRatio;
                Size widgetSize = _playViewContainerKey.currentContext.size;
                startPlayingStream(_playingStreamIDController.text.trim(), width: widgetSize.width * pixelRatio, height: widgetSize.height * pixelRatio);
              },
              padding: EdgeInsets.all(10.0),
            ),
          )
        ]),
        Divider(),
      ]
    );
  }

}
