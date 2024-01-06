/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "videosmodel.h"

#include "application.h"

VideosModel::VideosModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int VideosModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_videos.count();
}

QVariant VideosModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    VideoItem video = m_videos[index.row()];

    switch (role) {
    case IdRole:
        return QVariant(video.m_id);
    case StreamIdRole:
        return QVariant(video.m_streamId);
    case UserIdRole:
        return QVariant(video.m_userId);
    case UserLoginRole:
        return QVariant(video.m_userLogin);
    case UserNameRole:
        return QVariant(video.m_userName);
    case TitleRole:
        return QVariant(video.m_title);
    case DescriptionRole:
        return QVariant(video.m_description);
    case CreatedAtRole:
        return QVariant(video.m_createdAt);
    case PublishedAtRole:
        return QVariant(video.m_publishedAt);
    case UrlRole:
        return QVariant(video.m_url);
    case ThumbnailUrlRole:
        return QVariant(video.m_thumbnailUrl.replace(u"%{width}x%{height}"_qs, u"320x180"_qs));
    case ViewableRole:
        return QVariant(video.m_viewable);
    case ViewCountRole:
        return QVariant(video.m_viewCount);
    case LanguageRole:
        return QVariant(video.m_language);
    case TypeRole: {
        switch (video.m_type) {
        case Twitch::Video::VideoType::Upload:
            return QVariant(u"upload"_qs);
        case Twitch::Video::VideoType::Archive:
            return QVariant(u"archive"_qs);
        case Twitch::Video::VideoType::Highlight:
            return QVariant(u"highlight"_qs);
        }
        return QVariant();
    }
    case DurationRole:
        return QVariant(video.m_duration);
    case MutedSegmentsRole:
        return QVariant::fromValue(video.m_mutedSegments);
    case ImageWidthRole:
        return QVariant(QString::number(320));
    case ImageHeightRole:
        return QVariant(QString::number(180));
    }

    return QVariant();
}

QHash<int, QByteArray> VideosModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[StreamIdRole] = "streamId";
    roles[UserIdRole] = "userId";
    roles[UserLoginRole] = "userLogin";
    roles[UserNameRole] = "userName";
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[CreatedAtRole] = "createdAt";
    roles[PublishedAtRole] = "publishedAt";
    roles[UrlRole] = "url";
    roles[ThumbnailUrlRole] = "thumbnailUrl";
    roles[ViewableRole] = "viewable";
    roles[ViewCountRole] = "viewCount";
    roles[LanguageRole] = "language";
    roles[TypeRole] = "type";
    roles[DurationRole] = "duration";
    roles[MutedSegmentsRole] = "mutedSegments";
    roles[ImageWidthRole] = "imageWidth";
    roles[ImageHeightRole] = "imageHeight";
    return roles;
}

void VideosModel::getVideos(const QString &userId, bool reset)
{
    if (reset) {
        resetModel();
    }

    auto api = Application::instance()->getApi();
    Twitch::VideosReply *reply = api->getVideosByUserId(userId);
    connect(reply, &Twitch::VideosReply::finished, this, [=]() {
        auto const videos = reply->data().value<Twitch::Videos>();

        for (const auto &video : videos) {
            beginInsertRows(QModelIndex(), m_videos.count(), m_videos.count());
            m_videos.insert(m_videos.end(), video);
            endInsertRows();
        }

        m_cursor = reply->cursor();
        reply->deleteLater();
    });
}

void VideosModel::resetModel()
{
    beginResetModel();
    m_videos.clear();
    endResetModel();

    m_cursor = QString();
}

#include "moc_videosmodel.cpp"
