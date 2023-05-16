import QtQuick 2.0
import QtQuick.Controls 2.12
import Game 1.0

GridView{
    id: _gameBoard
    model: GameManagerModel{
    }

    cellWidth:  width /  _gameBoard.model.dimention
    cellHeight: height / _gameBoard.model.dimention

    property int winnerID: 0

    Column{
        id: _waitLabel
        anchors.centerIn: _gameBoard
        visible: false
        Text{
            text: "Wait for oponent"
            color: "green"
            font{
                pointSize: Math.min(_gameBoard.width, _gameBoard.height) / 15
                bold: true
            }
        }
    }

    delegate: Item{
        id: _backgroundDelegate
        width:  _gameBoard.cellWidth
        height: _gameBoard.cellHeight

        Tile{
            id: _tile
            anchors.fill: _backgroundDelegate

            Image{
                id: _tileSign
                anchors.fill: parent
                anchors.margins: 40
            }
            Connections{
                target: _gameManager
                onOponentTurn:{
                    var qml_sign  = sign
                    var qml_index = indx

                    if(qml_sign === 1 && index === qml_index)
                        _tileSign.source =  "qrc:/img/cross.png"
                    if(qml_sign === 2 && index === qml_index)
                        _tileSign.source =  "qrc:/img/circle.png"
                }
                onVictory:{
                    winnerID = winID
                    _backgroundDelegate.visible = false
                    _winLabel.visible           = true
                    _reStartBut.visible         = true
                }
                onStartTheGame:{
                    _tileSign.source = ""
                    _backgroundDelegate.visible = true
                }
            }
            MouseArea{
                anchors.fill: _tile
                onClicked: {
                    _gameManager.isPossibleToMove(index)
                }
            }
        }
    }
    Rectangle{
        id: _winLabel
        anchors.centerIn: _gameBoard
        visible: false

        Text {
            anchors.centerIn: parent
            text: "Player - " + winnerID + " has won"
            color: "green"
            font{
                pointSize: Math.min(_gameBoard.width, _gameBoard.height) / 17
                bold: true
            }
        }

    }
    Button{
        id: _reStartBut
        width: parent.width / 4
        height: parent.height / 7
        y: parent.height / 2 + 30
        x: parent.width / 2 - _reStartBut.width / 2
        visible: false

        background: Rectangle{
            color: "lightgreen"
            radius: 16
        }


        Text{
            anchors.centerIn: _reStartBut
            text: "new game"
            color: "black"
            font{
                pointSize: Math.min(_reStartBut.width, _reStartBut.height) / 4
                bold: true
            }
        }
        onClicked: {
            _winLabel.visible      = false
            _reStartBut.visible    = false
            _busyIndicator.running = true
            _gameManager.startGame();
        }
    }
}
