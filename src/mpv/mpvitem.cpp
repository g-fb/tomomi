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

#include <KLocalizedString>
#include <KShell>

#include <MpvController>

#include "application.h"
#include "mpvproperties.h"

MpvItem::MpvItem(QQuickItem *parent)
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

    connect(mpvController(), &MpvController::propertyChanged,
            this, &MpvItem::onPropertyChanged, Qt::QueuedConnection);
}

void MpvItem::initProperties()
{
    //    setPropertyAsync("terminal", "yes");
    //    setPropertyAsync("msg-level", "all=v");

    setPropertyAsync(u"vo"_qs, u"libmpv"_qs);
    setPropertyAsync(u"force-seekable"_qs, u"yes"_qs);

    setPropertyAsync(u"hwdec"_qs, u"vaapi"_qs);
    setPropertyAsync(u"volume-max"_qs, u"100"_qs);
    // set ytdl_path to yt-dlp or fallback to youtube-dl
//    setPropertyAsync("script-opts", QString("ytdl_hook-ytdl_path=%1").arg(Application::youtubeDlExecutable()));
//    setPropertyAsync("ytdl-format", PlaybackSettings::ytdlFormat());

//    setPropertyAsync("screenshot-template", VideoSettings::screenshotTemplate());
//    setPropertyAsync("screenshot-format", VideoSettings::screenshotFormat());

}

void MpvItem::onPropertyChanged(const QString &property, const QVariant &value)
{
    if (property == MpvProperties::self()->MediaTitle) {
        m_mediaTitle = value.toString();
        Q_EMIT mediaTitleChanged();

    } else if (property == MpvProperties::self()->Position) {
        m_position = value.toDouble();
        m_formattedPosition = Application::formatTime(m_position);
        Q_EMIT positionChanged();

    } else if (property == MpvProperties::self()->Remaining) {
        m_remaining = value.toDouble();
        m_formattedRemaining = Application::formatTime(m_remaining);
        Q_EMIT remainingChanged();

    } else if (property == MpvProperties::self()->Duration) {
        m_duration = value.toDouble();
        m_formattedDuration = Application::formatTime(m_duration);
        Q_EMIT durationChanged();

    } else if (property == MpvProperties::self()->Pause) {
        m_pause = value.toBool();
        Q_EMIT pauseChanged();

    } else if (property == MpvProperties::self()->Volume) {
        m_volume = value.toInt();
        Q_EMIT volumeChanged();

    } else if (property == MpvProperties::self()->Mute) {
        m_mute = value.toBool();
        Q_EMIT muteChanged();

    } else if (property == MpvProperties::self()->Chapter) {
        m_chapter = value.toInt();
        Q_EMIT chapterChanged();

    }
}

QString MpvItem::mediaTitle()
{
    return m_mediaTitle;
}

double MpvItem::position()
{
    return m_position;
}

void MpvItem::setPosition(double value)
{
    if (qFuzzyCompare(value, position())) {
        return;
    }
    setProperty(MpvProperties::self()->Position, value);
}

double MpvItem::remaining()
{
    return m_remaining;
}

double MpvItem::duration()
{
    return m_duration;
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
    return m_pause;
}

void MpvItem::setPause(bool value)
{
    if (value == pause()) {
        return;
    }
    setProperty(MpvProperties::self()->Pause, value);
}

int MpvItem::volume()
{
    return m_volume;
}

void MpvItem::setVolume(int value)
{
    if (value == volume()) {
        return;
    }
    setProperty(MpvProperties::self()->Volume, value);
}

bool MpvItem::mute()
{
    return m_mute;
}

void MpvItem::setMute(bool value)
{
    if (value == mute()) {
        return;
    }
    setProperty(MpvProperties::self()->Mute, value);
}

int MpvItem::chapter()
{
    return m_chapter;
}

void MpvItem::setChapter(int value)
{
    if (value == chapter()) {
        return;
    }
    setProperty(MpvProperties::self()->Chapter, value);
}

bool MpvItem::hwDecoding()
{
    if (getProperty(u"hwdec"_qs) == u"yes"_qs) {
        return true;
    } else {
        return false;
    }
}

void MpvItem::setHWDecoding(bool value)
{
    if (value) {
        setPropertyAsync(u"hwdec"_qs, u"yes"_qs);
    } else  {
        setPropertyAsync(u"hwdec"_qs, u"no"_qs);
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
    Q_EMIT userIdChanged();
}

void MpvItem::loadFile(const QString &file)
{
    command(QStringList() << u"loadfile"_qs << file);
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
        Q_EMIT viewCountChanged();
        reply->deleteLater();
    });
}

#include "moc_mpvitem.cpp"
