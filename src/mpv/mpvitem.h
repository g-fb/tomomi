/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MPVOBJECT_H
#define MPVOBJECT_H

#include <QtQuick/QQuickFramebufferObject>

#include "mpvcore.h"

class MpvRenderer;
class Track;

class MpvItem : public MpvCore
{
    Q_OBJECT

    Q_PROPERTY(QString mediaTitle READ mediaTitle NOTIFY mediaTitleChanged)
    Q_PROPERTY(double position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(double remaining READ remaining NOTIFY remainingChanged)
    Q_PROPERTY(bool pause READ pause WRITE setPause NOTIFY pauseChanged)
    Q_PROPERTY(bool mute READ mute WRITE setMute NOTIFY muteChanged)
    Q_PROPERTY(bool hwDecoding READ hwDecoding WRITE setHWDecoding NOTIFY hwDecodingChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged)
    Q_PROPERTY(int userId MEMBER m_userId READ userId WRITE setUserId NOTIFY userIdChanged)

    QString mediaTitle();

    double position();
    void setPosition(double value);

    double duration();

    double remaining();

    bool pause();
    void setPause(bool value);

    bool mute();
    void setMute(bool value);

    bool hwDecoding();
    void setHWDecoding(bool value);

    int volume();
    void setVolume(int value);

    int chapter();
    void setChapter(int value);

public:
    MpvItem(QQuickItem * parent = nullptr);
    ~MpvItem() = default;

    Q_INVOKABLE void loadFile(const QString &file, bool updateLastPlayedFile = true);
    void eventHandler() override;

signals:
    void mediaTitleChanged();
    void positionChanged();
    void durationChanged();
    void remainingChanged();
    void pauseChanged();
    void muteChanged();
    void hwDecodingChanged();
    void volumeChanged();
    void chapterChanged();
    void fileStarted();
    void fileLoaded();
    void endFile(QString reason);
    void userIdChanged();

private:
    void initProperties();
    int userId();
    void setUserId(int value);
    int m_userId {0};
};

#endif // MPVOBJECT_H
