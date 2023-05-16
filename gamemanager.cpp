#include "gamemanager.h"
#include <QDebug>

GameManager::GameManager(const size_t boardDimension, QObject *parent) :
    QAbstractListModel(parent),
    m_dimention(boardDimension),
    m_size(m_dimention * m_dimention)
{
    socket = new QTcpSocket(this);
}

int GameManager::dimention() const
{
    return m_dimention;
}

QVariant GameManager::data(const QModelIndex &index, int role) const
{
    return 0;
}

void GameManager::isPossibleToMove(int index)
{
    jObj["command"] = "move";
    jObj["index"]   = QString::number(index);
    jDoc.setObject(jObj);
    byteData = jDoc.toJson();

    socket->write(byteData);
}

void GameManager::startGame()
{
    socket->connectToHost("127.0.0.1", 2021);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

GameManager::Position GameManager::getRowCol(int index)
{
    int row = index / m_dimention;
    int col = index % m_dimention;

    return std::make_pair(col, row);
}

int GameManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_size;
}

void GameManager::read()
{
    byteData = socket->readAll();
    jDoc = QJsonDocument::fromJson(byteData);
    jObj = jDoc.object();
//QString jstring = QString::fromUtf8(jDoc.toJson());qDebug() << jstring;
    if(jObj.contains("winner")){
         int winID = jObj["winner"].toString().toInt();

         if(winID != 0)
            emit victory(winID);
    }

    if(jObj.contains("token") && jObj.contains("index")){
        int sign  = jObj["token"].toString().toInt();
        int index = jObj["index"].toString().toInt();

        if(sign != 0)
            emit oponentTurn(sign, index);
    }
    if(jObj["command"].toString() == QString("start"))
        emit startTheGame();
}
