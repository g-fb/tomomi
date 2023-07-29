/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mpvitem.h"
#include "application.h"
#include "mpvproperties.h"

#include <QCryptographicHash>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <QStandardPaths>
#include <QtGlobal>

#include <KLocalizedString>
#include <KShell>
#include <twitchstreamreply.hpp>

MpvItem::MpvItem(QQuickItem * parent)
    : MpvAbstractItem(parent)
{
    observeProperty(MpvProperties::self()->MediaTitle, MPV_FORMAT_STRING);
    observeProperty(MpvProperties::self()->Position, MPV_FORMAT_DOUBLE);
    observeProperty(MpvProperties::self()->Remaining, MPV_FORMAT_DOUBLE);
    observeProperty(MpvProperties::self()->Duration, MPV_FORMAT_DOUBLE);
    observeProperty(MpvProperties::self()->Pause, MPV_FORMAT_FLAG);
    observeProperty(MpvProperties::self()->Volume, MPV_FORMAT_INT64);
    observeProperty(MpvProperties::self()->Mute, MPV_FORMAT_FLAG);
    observeProperty(MpvProperties::self()->Chapter, MPV_FORMAT_INT64);

    initProperties();
}

void MpvItem::initProperties()
{
    //    setProperty("terminal", "yes");
    //    setProperty("msg-level", "all=v");

    setProperty("force-seekable", "yes");

    setProperty("hwdec", "auto-safe");
    setProperty("volume-max", "100");
    // set ytdl_path to yt-dlp or fallback to youtube-dl
//    setProperty("script-opts", QString("ytdl_hook-ytdl_path=%1").arg(Application::youtubeDlExecutable()));
//    setProperty("ytdl-format", PlaybackSettings::ytdlFormat());

//    setProperty("screenshot-template", VideoSettings::screenshotTemplate());
//    setProperty("screenshot-format", VideoSettings::screenshotFormat());

}

QString MpvItem::mediaTitle()
{
    return getPropertySynchronous("media-title").toString();
}

double MpvItem::position()
{
    return getPropertySynchronous("time-pos").toDouble();
}

void MpvItem::setPosition(double value)
{
    if (value == position()) {
        return;
    }
    setProperty("time-pos", value);
    Q_EMIT positionChanged();
}

double MpvItem::remaining()
{
    return getPropertySynchronous("time-remaining").toDouble();
}

double MpvItem::duration()
{
    return getPropertySynchronous("duration").toDouble();
}

bool MpvItem::pause()
{
    return getPropertySynchronous("pause").toBool();
}

void MpvItem::setPause(bool value)
{
    if (value == pause()) {
        return;
    }
    setProperty("pause", value);
    Q_EMIT pauseChanged();
}

int MpvItem::volume()
{
    return getPropertySynchronous("volume").toInt();
}

void MpvItem::setVolume(int value)
{
    if (value == volume()) {
        return;
    }
    setProperty("volume", value);
    Q_EMIT volumeChanged();
}

bool MpvItem::mute()
{
    return getPropertySynchronous("mute").toBool();
}

void MpvItem::setMute(bool value)
{
    if (value == mute()) {
        return;
    }
    setProperty("mute", value);
    emit muteChanged();
}

int MpvItem::chapter()
{
    return getPropertySynchronous("chapter").toInt();
}

void MpvItem::setChapter(int value)
{
    if (value == chapter()) {
        return;
    }
    setProperty("chapter", value);
    Q_EMIT chapterChanged();
}

bool MpvItem::hwDecoding()
{
    if (getPropertySynchronous("hwdec") == "yes") {
        return true;
    } else {
        return false;
    }
}

void MpvItem::setHWDecoding(bool value)
{
    if (value) {
        setProperty("hwdec", "yes");
    } else  {
        setProperty("hwdec", "no");
    }
    Q_EMIT hwDecodingChanged();
}

int MpvItem::userId()
{
    return m_userId;
}

void MpvItem::setUserId(int value)
{
    if (value == userId()) {
        return;
    }
    m_userId = value;
    emit userIdChanged();
}

void MpvItem::loadFile(const QString &file)
{
    command(QStringList() << "loadfile" << file);
}

int MpvItem::viewCount()
{
    return m_viewCount;
}

void MpvItem::userViewCount()
{
    auto api = Application::instance()->getApi();
    Twitch::StreamReply *reply = api->getStreamByUserId(QString::number(m_userId));

    connect(reply, &Twitch::StreamReply::finished, this, [=]() {
        auto const channel = reply->data().value<Twitch::Stream>();
        m_viewCount = channel.m_viewerCount;
        emit viewCountChanged();
        reply->deleteLater();
    });
}

#include "moc_mpvitem.cpp"
