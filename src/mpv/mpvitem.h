/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef MPVOBJECT_H
#define MPVOBJECT_H

#include <QtQuick/QQuickFramebufferObject>
#include <MpvAbstractItem>

class MpvRenderer;
class Track;

class MpvItem : public MpvAbstractItem
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
    Q_PROPERTY(int viewCount MEMBER m_viewCount READ viewCount NOTIFY viewCountChanged)

    Q_PROPERTY(QString formattedPosition READ formattedPosition NOTIFY positionChanged)
    QString formattedPosition() const;

    Q_PROPERTY(QString formattedDuration READ formattedDuration NOTIFY durationChanged)
    QString formattedRemaining() const;

    Q_PROPERTY(QString formattedRemaining READ formattedRemaining NOTIFY remainingChanged)
    QString formattedDuration() const;


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

    Q_INVOKABLE void loadFile(const QString &file);
    Q_INVOKABLE void userViewCount();

Q_SIGNALS:
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
    void viewCountChanged();

private:
    void onPropertyChanged(const QString &property, const QVariant &value);
    void initProperties();
    int userId();
    void setUserId(int value);
    int viewCount();

    double m_position{0.0};
    QString m_formattedPosition;
    double m_remaining{0.0};
    QString m_formattedRemaining;
    double m_duration{0.0};
    QString m_formattedDuration;
    QString m_mediaTitle;
    bool m_pause{false};
    bool m_mute{false};
    int m_chapter{0};
    int m_volume{0};

    int m_userId {0};
    int m_viewCount {0};
};

#endif // MPVOBJECT_H
