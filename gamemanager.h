#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QAbstractListModel>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

class GameManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int dimention READ dimention CONSTANT)
public:
    using Position = std::pair<int, int>;

    static constexpr size_t defaultDimetion {3};

    explicit GameManager(const size_t boardDimension = defaultDimetion
            ,QObject *parent = nullptr);

    int dimention() const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void isPossibleToMove(int);
    Q_INVOKABLE void startGame();

    Position getRowCol(int);

public slots:
    void read();

signals:
    void oponentTurn(int sign, int indx);
    void waitSignal();
    void startTheGame();
    void victory(int winID);

private:
    const int m_dimention;
    const int      m_size;

    QTcpSocket    *socket;
    QJsonObject      jObj;
    QJsonDocument    jDoc;
    QByteArray   byteData;
};


#endif // GAMEMANAGER_H
