/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mpvitem.h"
#include "application.h"

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
    : MpvCore(parent)
{
    mpv_observe_property(m_mpv, 0, "media-title",    MPV_FORMAT_STRING);
    mpv_observe_property(m_mpv, 0, "time-pos",       MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "time-remaining", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "duration",       MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "pause",          MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "volume",         MPV_FORMAT_INT64);
    mpv_observe_property(m_mpv, 0, "mute",           MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "chapter",        MPV_FORMAT_INT64);

    initProperties();
}

void MpvItem::initProperties()
{
    //    setProperty("terminal", "yes");
    //    setProperty("msg-level", "all=v");

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
    setProperty("time-pos", value);
    Q_EMIT positionChanged();
}

double MpvItem::remaining()
{
    return getProperty("time-remaining").toDouble();
}

double MpvItem::duration()
{
    return getProperty("duration").toDouble();
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
    setProperty("pause", value);
    Q_EMIT pauseChanged();
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
    setProperty("volume", value);
    Q_EMIT volumeChanged();
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
    setProperty("mute", value);
    emit muteChanged();
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
    setProperty("chapter", value);
    Q_EMIT chapterChanged();
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

void MpvItem::loadFile(const QString &file, bool updateLastPlayedFile)
{
    command(QStringList() << "loadfile" << file);
}

void MpvItem::eventHandler()
{
    while (m_mpv) {
        mpv_event *event = mpv_wait_event(m_mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        switch (event->event_id) {
        case MPV_EVENT_START_FILE: {
            Q_EMIT fileStarted();
            break;
        }
        case MPV_EVENT_FILE_LOADED: {
            Q_EMIT fileLoaded();
            break;
        }
        case MPV_EVENT_END_FILE: {
            auto prop = (mpv_event_end_file *)event->data;
            if (prop->reason == MPV_END_FILE_REASON_EOF) {
                Q_EMIT endFile("eof");
            } else if(prop->reason == MPV_END_FILE_REASON_ERROR) {
                Q_EMIT endFile("error");
            }
            break;
        }
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property *prop = (mpv_event_property *)event->data;

            if (strcmp(prop->name, "time-pos") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    Q_EMIT positionChanged();
                }
            } else if (strcmp(prop->name, "media-title") == 0) {
                if (prop->format == MPV_FORMAT_STRING) {
                    Q_EMIT mediaTitleChanged();
                }
            } else if (strcmp(prop->name, "time-remaining") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    Q_EMIT remainingChanged();
                }
            } else if (strcmp(prop->name, "duration") == 0) {
                if (prop->format == MPV_FORMAT_DOUBLE) {
                    Q_EMIT durationChanged();
                }
            } else if (strcmp(prop->name, "volume") == 0) {
                if (prop->format == MPV_FORMAT_INT64) {
                    Q_EMIT volumeChanged();
                }
            } else if (strcmp(prop->name, "pause") == 0) {
                if (prop->format == MPV_FORMAT_FLAG) {
                    Q_EMIT pauseChanged();
                }
            } else if (strcmp(prop->name, "chapter") == 0) {
                if (prop->format == MPV_FORMAT_INT64) {
                    Q_EMIT chapterChanged();
                }
            }
            break;
        }
        default: ;
            // Ignore uninteresting or unknown events.
        }
    }
}

int MpvItem:: viewCount()
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
