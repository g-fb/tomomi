#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <Twitch>

class QAbstractItemModel;
class KColorSchemeManager;
class QTcpServer;
class Settings;

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* colorSchemesModel READ colorSchemesModel CONSTANT)

public:
    explicit Application(QObject *parent = nullptr);

    Twitch::Api *getApi() const;
    static Application *instance();
    Q_INVOKABLE void activateColorScheme(const QString &name);
    Q_SCRIPTABLE void openChannel(const QString &userName, const QString &userId);

signals:
    void qmlOpenChannel(const QString &userName, const QString &userId);

private:
    QAbstractItemModel *colorSchemesModel();
    void startServer();
    void onRead();

    QTcpServer *m_server;
    Twitch::Api *m_api;
    Settings *m_settings;
    KColorSchemeManager *m_schemes;

    static Application *sm_instance;
};


#endif // APPLICATION_H
