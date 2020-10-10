#include "settings.h"

#include <KConfig>
#include <KConfigGroup>
#include <QVariant>
#include <QDebug>

Settings* Settings::sm_instance = nullptr;

Settings::Settings(QObject *parent)
    : QObject(parent)
{
    m_defaultSettings = {
        {"ClientId",         QVariant("hfj3gocxu0x62vpz6hpiclybuh493q")},
        {"AccessToken",      QVariant(QStringLiteral())},
        {"Login",            QVariant(QStringLiteral())},
        {"UserId",           QVariant(QStringLiteral())},
        {"IsValidToken",     QVariant(false)},
        {"ColorScheme",      QVariant(QStringLiteral())},
    };
    m_config = KSharedConfig::openConfig("georgefb/Tomomi/tomomi.conf");
}

QString Settings::colorScheme()
{
    return get("ColorScheme").toString();
}

void Settings::setColorScheme(const QString &scheme)
{
    if (scheme == colorScheme()) {
        return;
    }
    set("ColorScheme", scheme);
    emit colorSchemeChanged();
}

QString Settings::twitchClientId()
{
    return get("ClientId").toString();
}

void Settings::setTwitchClientId(const QString& clientId)
{
    if (clientId == twitchClientId()) {
        return;
    }
    set("ClientId", clientId);
    emit twitchClientIdChanged();
}

QString Settings::twitchAccessToken()
{
    return get("AccessToken").toString();
}

void Settings::setTwitchAccessToken(const QString& accessToken)
{
    if (accessToken == twitchAccessToken()) {
        return;
    }
    set("AccessToken", accessToken);
    emit twitchAccessTokenChanged();
}

QString Settings::twitchLogin()
{
    return get("Login").toString();
}

void Settings::setTwitchLogin(const QString& login)
{
    if (login == twitchLogin()) {
        return;
    }
    set("Login", login);
    emit twitchLoginChanged();
}

QString Settings::twitchUserId()
{
    return get("UserId").toString();
}

void Settings::setTwitchUserId(const QString& userId)
{
    if (userId == twitchUserId()) {
        return;
    }
    set("UserId", userId);
    emit twitchUserIdChanged();
}

bool Settings::isValidToken()
{
    return get("IsValidToken").toBool();
}

void Settings::setIsValidToken(bool isValid)
{
    if (isValid == isValidToken()) {
        return;
    }
    m_defaultSettings["IsValidToken"] = isValid;
    emit isValidTokenChanged();
}

QVariant Settings::get(const QString &key)
{
    return m_config->group(CONFIG_GROUP).readEntry(key, m_defaultSettings[key]);
}

void Settings::set(const QString &key, const QString &value)
{
    m_config->group(CONFIG_GROUP).writeEntry(key, value);
    m_config->sync();

    emit settingsChanged();
}

Settings *Settings::instance()
{
    if (!sm_instance)
        sm_instance = new Settings();
    return sm_instance;
}
