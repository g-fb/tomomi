#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <Twitch>

class QAbstractItemModel;
class QTcpServer;
class Settings;

using FollowedChannels = Twitch::Streams;

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr);
    FollowedChannels channels();
    void getFollowedChannels();

signals:
    void getFollowedChannelsFinished();
    void newLiveChannel(const QString &userName, const QString &userId, const QString &title);
    void rowCountChanged(int count);

private:
    void getLiveChannels();
    void startServer();
    void onRead();

    QTcpServer *m_server;
    Settings *m_settings;
    Twitch::Api *m_api;
    FollowedChannels m_channels;
    QStringList m_followedChannels;
    QStringList m_oldFollowedChannels;

};

#endif // APPLICATION_H
