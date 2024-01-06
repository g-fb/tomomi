#include "lockmanager.h"

#include "screensaverdbusinterface.h"

#include <QDBusConnection>

LockManager::LockManager(QObject *parent)
    : QObject(parent)
{
    m_iface = std::make_unique<OrgFreedesktopScreenSaverInterface>(
                QStringLiteral("org.freedesktop.ScreenSaver"),
                QStringLiteral("/org/freedesktop/ScreenSaver"),
                QDBusConnection::sessionBus(),
                this);
}
LockManager::~LockManager() = default;

void LockManager::setInhibitionOff()
{
    m_iface->UnInhibit(m_cookie);
}

void LockManager::setInhibitionOn()
{
    m_cookie = m_iface->Inhibit(QStringLiteral("Tomomi"), QStringLiteral("Twitch video streaming."));
}

#include "moc_lockmanager.cpp"
