#ifndef LOCKMANAGER_H
#define LOCKMANAGER_H

#include <QObject>

#include <memory>

class OrgFreedesktopScreenSaverInterface;

class LockManager : public QObject
{
    Q_OBJECT

public:
    explicit LockManager(QObject *parent = nullptr);
    ~LockManager();

public Q_SLOTS:
    void setInhibitionOn();
    void setInhibitionOff();
private:
    std::unique_ptr<OrgFreedesktopScreenSaverInterface> m_iface;
    int m_cookie;
};

#endif // LOCKMANAGER_H


