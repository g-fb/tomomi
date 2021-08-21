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
    case UserNameRole:
        return QVariant(video.m_userName);
    case TypeRole: {
        switch (video.m_type) {
        case Twitch::Video::VideoType::Upload:
            return QVariant("upload");
        case Twitch::Video::VideoType::Archive:
            return QVariant("upload");
        case Twitch::Video::VideoType::Highlight:
            return QVariant("upload");
        }
    }
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
        return QVariant(video.m_thumbnailUrl.replace("%{width}x%{height}", "320x180"));
    case ViewCountRole:
        return QVariant(video.m_viewCount);
    case LanguageRole:
        return QVariant(video.m_language);
    case DurationRole:
        return QVariant(video.m_duration);
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
    roles[UserNameRole] = "userName";
    roles[TypeRole] = "type";
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[CreatedAtRole] = "createdAt";
    roles[PublishedAtRole] = "publishedAt";
    roles[UrlRole] = "url";
    roles[ThumbnailUrlRole] = "thumbnailUrl";
    roles[ViewCountRole] = "viewCount";
    roles[LanguageRole] = "language";
    roles[DurationRole] = "duration";
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

//        Twitch::VideoReply *videoReply = api->getVideoById(videos.at(1).m_id);
//        connect(videoReply, &Twitch::VideoReply::finished, this, [=]() {
//            auto const video = videoReply->data().value<Twitch::Video>();
//            qDebug() << video.m_title;
//        });

    });
}

void VideosModel::resetModel()
{
    beginResetModel();
    m_videos.clear();
    endResetModel();

    m_cursor = QString();
}
