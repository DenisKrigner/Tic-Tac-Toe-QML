#include "server.h"

Server::Server()
{
    m_server = new QTcpServer(this);
    game_started = false;

    if(m_server->listen(QHostAddress::Any, 2021))
        std::cout << "Server has started. port 2021"<< std::endl;
    else
        std::cout << "Server failed to start error: "
                  << m_server->errorString().toStdString() << std::endl;

    connect(m_server, &QTcpServer::newConnection, this, &Server::incomingConnection);
}

void Server::incomingConnection()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    players[id] = client;

    connect(client, &QTcpSocket::readyRead,    this, &Server::readyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
    connect(client, &QTcpSocket::stateChanged, this, &Server::socketStateChanged);

    std::cout << "New Client connected IP:" << client->peerAddress().toString().toStdString()
              << " ID: " << id << std::endl;
    id++;
    if(id > 2)
    {
        std::cout << "Start new game " << std::endl;
        startNewGame();      
    }
}

void Server::readyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    data   = client->readAll();
    jDoc   = QJsonDocument::fromJson(data);
    jObj   = jDoc.object();
    int id = players.key(client);

    QString command = jObj["command"].toString();

    if(command == "move")
    {
        if(state != id || !game_started){
            client->write(QString::number(0).toUtf8());
            return;
        }
        QString index = jObj["index"].toString();
        if(game_field[index.toInt()] == 0){
            game_field[index.toInt()] = id;
            int winnerID = checkVictory();
            jObj["token"]  = QString::number(id);
            jObj["index"] = index;

            if(winnerID != 0){
                jObj["winner"] = QString::number(winnerID);
                game_started = false;
                jDoc.setObject(jObj);
                data = jDoc.toJson();
                players[2]->write(data);
                players[1]->write(data);
                players[1]->disconnectFromHost();
                players[2]->disconnectFromHost();
                jObj = QJsonObject();

                std::cout << "Player - " << winnerID << " won" << std::endl;
                return;
            }
            jDoc.setObject(jObj);
            data = jDoc.toJson();
            players[2]->write(data);
            players[1]->write(data);

            state == 1 ? state = 2 : state = 1;
            std::cout << "Client id: " << id << "make turn - " << index.toInt() << std::endl;
            jObj = QJsonObject();
        }
    }

}

void Server::startNewGame()
{
    for(int i = 0; i < 8; i++)
    {
       game_field[i] = 0;
    }
    state = 1;
    game_started = true;

    jObj["command"] = QString("start");
    jDoc.setObject(jObj);
    data = jDoc.toJson();
    players[1]->write(data);
    players[2]->write(data);

    jObj = QJsonObject();
}

int Server::checkVictory()
{
    if((game_field[0] == 1 && game_field[1] == 1 && game_field[2] == 1) ||
       (game_field[3] == 1 && game_field[4] == 1 && game_field[5] == 1) ||
       (game_field[6] == 1 && game_field[7] == 1 && game_field[8] == 1) ||
       (game_field[0] == 1 && game_field[4] == 1 && game_field[8] == 1) ||
       (game_field[2] == 1 && game_field[4] == 1 && game_field[6] == 1) ||
       (game_field[0] == 1 && game_field[3] == 1 && game_field[6] == 1) ||
       (game_field[1] == 1 && game_field[4] == 1 && game_field[7] == 1) ||
       (game_field[2] == 1 && game_field[5] == 1 && game_field[8] == 1))
        return 1;
    else if((game_field[0] == 2 && game_field[1] == 2 && game_field[2] == 2) ||
            (game_field[3] == 2 && game_field[4] == 2 && game_field[5] == 2) ||
            (game_field[6] == 2 && game_field[7] == 2 && game_field[8] == 2) ||
            (game_field[0] == 2 && game_field[4] == 2 && game_field[8] == 2) ||
            (game_field[2] == 2 && game_field[4] == 2 && game_field[6] == 2) ||
            (game_field[0] == 2 && game_field[3] == 2 && game_field[6] == 2) ||
            (game_field[1] == 2 && game_field[4] == 2 && game_field[7] == 2) ||
            (game_field[2] == 2 && game_field[5] == 2 && game_field[8] == 2))
        return 2;
    else
        return 0;
}


void Server::socketDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAdrr     = client->peerAddress().toString();

    id = 1;
    game_started = false;
    std::cout << "Clent disconnected IP: " << ipAdrr.toStdString() << std::endl;;
}

void Server::socketStateChanged(QAbstractSocket::SocketState)
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString ipAdrr     = client->peerAddress().toString();
    QString desc;

    switch (state) {
    case QAbstractSocket::UnconnectedState:
        desc = "The socket is not connected.";
        break;
    case QAbstractSocket::HostLookupState:
        desc = "The socket is performing a host name lookup.";
        break;
    case QAbstractSocket::ConnectingState:
        desc = "The socket has started establishing a connection.";
        break;
    case QAbstractSocket::ConnectedState:
         desc = "A connection is established.";
        break;
    case QAbstractSocket::BoundState:
        desc = "The socket is bound to an address and port.";
        break;
    case QAbstractSocket::ClosingState:
        desc = "The socket is about to close (data may still be waiting to be written).";
        break;
    case QAbstractSocket::ListeningState:
        desc = "For internal use only.";
        break;
    }
    std::cout << "Client state changed (" << "IP: "<< ipAdrr.toStdString()
              << " state: " <<desc.toStdString() << std::endl;
}
