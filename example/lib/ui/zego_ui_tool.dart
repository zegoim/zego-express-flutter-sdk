import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

class ZegoUITool {

  static showAlert(BuildContext context, String message) {
    showDialog(context: context, builder: (BuildContext context) {
      return AlertDialog(
        title: Text('Tips'),
        content: Text(message),
        actions: <Widget>[
          FlatButton(
            child: Text('OK'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          )
        ],
      );
    });
  }

  static showImage(BuildContext context, MemoryImage imageData) {
    showDialog(context: context, builder: (BuildContext context) {
      if (imageData == null) {
        print('[ZegoUITool][showImage] image data is null!');
        return null;
      }

      return AlertDialog(
        content: Image(image: imageData, width: MediaQuery.of(context).size.width / 2, height: MediaQuery.of(context).size.width / 2),
        actions: <Widget>[
          FlatButton(
            child: Text('OK'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          )
        ],
      );
    });
  }

}