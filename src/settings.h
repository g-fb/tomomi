#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <KSharedConfig>
#include <QQmlEngine>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString colorScheme
               READ colorScheme
               WRITE setColorScheme
               NOTIFY colorSchemeChanged)

    Q_PROPERTY(QString twitchClientId
               READ twitchClientId
               WRITE setTwitchClientId
               NOTIFY twitchClientIdChanged)

    Q_PROPERTY(QString twitchBearerToken
               READ twitchBearerToken
               WRITE setTwitchBearerToken
               NOTIFY twitchBearerTokenChanged)

    Q_PROPERTY(QString twitchLogin
               READ twitchLogin
               WRITE setTwitchLogin
               NOTIFY twitchLoginChanged)

    Q_PROPERTY(QString twitchUserId
               READ twitchUserId
               WRITE setTwitchUserId
               NOTIFY twitchUserIdChanged)

    Q_PROPERTY(bool isValidToken
               READ isValidToken
               WRITE setIsValidToken
               NOTIFY isValidTokenChanged)

public:
    explicit Settings(QObject *parent = nullptr);

    QString colorScheme();
    void setColorScheme(const QString &scheme);

    QString twitchClientId();
    void setTwitchClientId(const QString &clientId);

    QString twitchBearerToken();
    void setTwitchBearerToken(const QString &accessToken);

    QString twitchLogin();
    void setTwitchLogin(const QString &login);

    QString twitchUserId();
    void setTwitchUserId(const QString &userId);

    bool isValidToken();
    void setIsValidToken(bool isValid);

    Q_INVOKABLE QVariant get(const QString &key);
    Q_INVOKABLE void set(const QString &key, const QString &value);

    static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Settings::instance();
    }

    static Settings *instance();

signals:
    void settingsChanged();
    void colorSchemeChanged();
    void twitchClientIdChanged();
    void twitchBearerTokenChanged();
    void twitchLoginChanged();
    void twitchUserIdChanged();
    void isValidTokenChanged();

protected:
    QHash<QString, QVariant> m_defaultSettings;
    KSharedConfig::Ptr m_config;
    QString CONFIG_GROUP = QStringLiteral("General");

    static Settings *sm_instance;
};

#endif // SETTINGS_H
