#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>
#include <QMap>
#include <iostream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server();

private slots:
    void incomingConnection();
    void readyRead();
    void socketDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState);

private:
    QMap<int, int>      game_field;
    QMap<int, QTcpSocket*> players;
    QTcpServer           *m_server;
    QByteArray                data;
    QJsonDocument             jDoc;
    QJsonObject               jObj;

    int id = 1;
    unsigned short state;
    bool game_started;

    void startNewGame();
    int checkVictory();
};

#endif // SERVER_H
