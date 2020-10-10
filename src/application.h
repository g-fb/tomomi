#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <Twitch>

class QTcpServer;
class Settings;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);

    Twitch::Api *getApi() const;
    static Application *instance();

private:
    void startServer();
    void onRead();

    QTcpServer *m_server;
    Twitch::Api *m_api;
    Settings *m_Settings;
    static Application *sm_instance;
};


#endif // APPLICATION_H
