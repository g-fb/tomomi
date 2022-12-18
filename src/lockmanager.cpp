#include "lockmanager.h"
#include "screensaverdbusinterface.h"

#include <QDBusConnection>
#include <QDebug>

LockManager::LockManager(QObject *parent)
    : QObject(parent)
    , m_inhibit()
{
    m_iface = new OrgFreedesktopScreenSaverInterface(
                QStringLiteral("org.freedesktop.ScreenSaver"),
                QStringLiteral("/org/freedesktop/ScreenSaver"),
                QDBusConnection::sessionBus(),
                this);
}

void LockManager::setInhibitionOff()
{
    m_iface->UnInhibit(m_cookie);
}

void LockManager::setInhibitionOn()
{
    m_cookie = m_iface->Inhibit(
                QStringLiteral("Tomomi"),
                QStringLiteral("Twitch video streaming."));
}

#include "moc_lockmanager.cpp"
