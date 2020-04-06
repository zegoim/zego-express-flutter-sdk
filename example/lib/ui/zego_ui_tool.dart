import 'package:flutter/material.dart';
import 'package:flutter/cupertino.dart';

class ZegoUITool {

  static showAlert(BuildContext context, String message) {
    showDialog(context: context, builder: (BuildContext context) {
      return AlertDialog(
        title: Text('提示'),
        content: Text(message),
        actions: <Widget>[
          FlatButton(
            child: Text('确认'),
            onPressed: () {
              Navigator.of(context).pop();
            },
          )
        ],
      );
    });
  }

}