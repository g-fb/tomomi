/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mpvitem.h"

#include <QCryptographicHash>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <QStandardPaths>
#include <QtGlobal>
#include <MpvController>

#include <KLocalizedString>
#include <KShell>
#include <twitchstreamreply.hpp>

#include "application.h"
#include "mpvproperties.h"

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

    connect(m_mpvController, &MpvController::propertyChanged,
            this, &MpvItem::onPropertyChanged, Qt::QueuedConnection);
}

void MpvItem::initProperties()
{
    //    setPropertyAsync("terminal", "yes");
    //    setPropertyAsync("msg-level", "all=v");

    setPropertyAsync("force-seekable", "yes");

    setPropertyAsync("hwdec", "auto-safe");
    setPropertyAsync("volume-max", "100");
    // set ytdl_path to yt-dlp or fallback to youtube-dl
//    setPropertyAsync("script-opts", QString("ytdl_hook-ytdl_path=%1").arg(Application::youtubeDlExecutable()));
//    setPropertyAsync("ytdl-format", PlaybackSettings::ytdlFormat());

//    setPropertyAsync("screenshot-template", VideoSettings::screenshotTemplate());
//    setPropertyAsync("screenshot-format", VideoSettings::screenshotFormat());

}

void MpvItem::onPropertyChanged(const QString &property, const QVariant &value)
{
    if (property == MpvProperties::self()->MediaTitle) {
        cachePropertyValue(property, value);
        Q_EMIT mediaTitleChanged();

    } else if (property == MpvProperties::self()->Position) {
        cachePropertyValue(property, value);
        m_formattedPosition = Application::formatTime(value.toDouble());
        Q_EMIT positionChanged();

    } else if (property == MpvProperties::self()->Remaining) {
        cachePropertyValue(property, value);
        m_formattedRemaining = Application::formatTime(value.toDouble());
        Q_EMIT remainingChanged();

    } else if (property == MpvProperties::self()->Duration) {
        cachePropertyValue(property, value);
        m_formattedDuration = Application::formatTime(value.toDouble());
        Q_EMIT durationChanged();

    } else if (property == MpvProperties::self()->Pause) {
        cachePropertyValue(property, value);
        Q_EMIT pauseChanged();

    } else if (property == MpvProperties::self()->Volume) {
        cachePropertyValue(property, value);
        Q_EMIT volumeChanged();

    } else if (property == MpvProperties::self()->Mute) {
        cachePropertyValue(property, value);
        Q_EMIT muteChanged();

    } else if (property == MpvProperties::self()->Chapter) {
        cachePropertyValue(property, value);
        Q_EMIT chapterChanged();

    }
}

QString MpvItem::mediaTitle()
{
    return getProperty("media-title").toString();
}

double MpvItem::position()
{
    return getProperty("time-pos").toDouble();
}

void MpvItem::setPosition(double value)
{
    if (value == position()) {
        return;
    }
    setPropertyAsync("time-pos", value);
}

double MpvItem::remaining()
{
    return getProperty("time-remaining").toDouble();
}

double MpvItem::duration()
{
    return getProperty("duration").toDouble();
}

QString MpvItem::formattedDuration() const
{
    return m_formattedDuration;
}

QString MpvItem::formattedRemaining() const
{
    return m_formattedRemaining;
}

QString MpvItem::formattedPosition() const
{
    return m_formattedPosition;
}

bool MpvItem::pause()
{
    return getProperty("pause").toBool();
}

void MpvItem::setPause(bool value)
{
    if (value == pause()) {
        return;
    }
    setPropertyAsync("pause", value);
}

int MpvItem::volume()
{
    return getProperty("volume").toInt();
}

void MpvItem::setVolume(int value)
{
    if (value == volume()) {
        return;
    }
    setPropertyAsync("volume", value);
}

bool MpvItem::mute()
{
    return getProperty("mute").toBool();
}

void MpvItem::setMute(bool value)
{
    if (value == mute()) {
        return;
    }
    setPropertyAsync("mute", value);
}

int MpvItem::chapter()
{
    return getProperty("chapter").toInt();
}

void MpvItem::setChapter(int value)
{
    if (value == chapter()) {
        return;
    }
    setPropertyAsync("chapter", value);
}

bool MpvItem::hwDecoding()
{
    if (getProperty("hwdec") == "yes") {
        return true;
    } else {
        return false;
    }
}

void MpvItem::setHWDecoding(bool value)
{
    if (value) {
        setPropertyAsync("hwdec", "yes");
    } else  {
        setPropertyAsync("hwdec", "no");
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
