#ifndef APPLICATION_H
#define APPLICATION_H

#include <QKeyEvent>
#include <QObject>
#include <Twitch>

class QAbstractItemModel;
class QQmlApplicationEngine;
class KColorSchemeManager;
class QTcpServer;
class Settings;

class ApplicationEventFilter : public QObject
{
    Q_OBJECT

signals:
    void applicationMouseLeave();
    void applicationMouseEnter();

protected:
    bool eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::Enter) {
            emit applicationMouseEnter();
            return true;
        } else if (event->type() == QEvent::Leave) {
            emit applicationMouseLeave();
            return true;
        } else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
    }
};

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *colorSchemesModel READ colorSchemesModel CONSTANT)

public:
    static Application *instance();

    void setQmlEngine(QQmlApplicationEngine *qmlEngine);
    Twitch::Api *getApi() const;
    Q_INVOKABLE void activateColorScheme(const QString &name);
    Q_INVOKABLE void userId(const QString &userName);
    Q_INVOKABLE QString formatTime(const double time);
    Q_SCRIPTABLE void openChannel(const QString &userName, const QString &userId);

signals:
    void qmlOpenChannel(const QString &userName, const QString &userId);
    void qmlApplicationMouseLeave();
    void qmlApplicationMouseEnter();
    void userIdRetrieved(const QString &userName, const QString &userId);

private:
    Q_DISABLE_COPY_MOVE(Application)
    explicit Application(QObject *parent = nullptr);
    ~Application() = default;

    QAbstractItemModel *colorSchemesModel();
    void startServer();
    void onRead();

    QTcpServer *m_server;
    Twitch::Api *m_api;
    Settings *m_settings;
    KColorSchemeManager *m_schemes;
    QQmlApplicationEngine *m_qmlEngine;
};


#endif // APPLICATION_H
