import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Game 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TicTacToe")

    GameBoard{
        id: _gameBoard
        visible: false
        anchors.fill: parent
    }

    GameManagerModel{
        id: _gameManager
        onStartTheGame: {
            _gameBoard.visible   = true
            _busyIndicator.running = false
        }
    }

    Button{
        id: _startButton
        anchors.centerIn: parent
        width: parent.width / 4
        height: parent.height / 7

        background: Rectangle{
            color: "lightblue"
            radius: 16
        }

        Text{
            anchors.centerIn: _startButton
            text: "Start"
            color: "black"
                font{
                    pointSize: Math.min(parent.width, parent.height) / 2
                    bold: true
                }
        }
        onClicked: {
            _startButton.visible = false
            _busyIndicator.running = true
            _gameManager.startGame();
        }
    }

    BusyIndicator{
        id: _busyIndicator
        running: false
        anchors.centerIn: parent
    }
}
