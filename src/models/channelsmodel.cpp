#include "channelsmodel.h"
#include "application.h"

ChannelsModel::ChannelsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ChannelsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_channels.count();
}

QVariant ChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ChannelItem channel = m_channels[index.row()];

    switch (role) {
    case IdRole:
        return QVariant(channel.m_id);
    case TitleRole:
        return QVariant(channel.m_title);
    case ViewerCountRole:
        return QVariant(channel.m_viewerCount);
    case UserNameRole:
        return QVariant(channel.m_userName);
    case UserIdRole:
        return QVariant(channel.m_userId);
    case ThumbnailRole:
        return QVariant(channel.m_thumbnailUrl.replace("{width}x{height}", "440x248"));
    case ThumbnailWidthRole:
        return QVariant(QString::number(440));
    case ThumbnailHeightRole:
        return QVariant(QString::number(248));
    }

    return QVariant();
}

QHash<int, QByteArray> ChannelsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[ViewerCountRole] = "viewerCount";
    roles[UserNameRole] = "userName";
    roles[UserIdRole] = "userId";
    roles[ThumbnailRole] = "thumbnailUrl";
    roles[ThumbnailWidthRole] = "thumbnailWidth";
    roles[ThumbnailHeightRole] = "thumbnailHeight";
    return roles;
}

//bool ChannelsModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//    return true;
//}

//bool ChannelsModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//    return true;
//}

void ChannelsModel::getChannels(const QString &gameId, bool reset)
{
    if (reset) {
        resetModel();
    }

    auto api = Application::instance()->getApi();
    Twitch::StreamsReply *reply = api->getStreamsByGameId(gameId, 24, m_cursor);

    connect(reply, &Twitch::StreamsReply::finished, this, [=]() {
        auto const channels = reply->data().value<Twitch::Streams>();

        for (const auto &channel : channels) {
            beginInsertRows(QModelIndex(), m_channels.count(), m_channels.count());
            m_channels.insert(m_channels.end(), channel);
            endInsertRows();
        }

        m_gameId = gameId;
        m_cursor = reply->cursor();

        reply->deleteLater();
    });
}

void ChannelsModel::resetModel()
{
        beginResetModel();
        m_channels.clear();
        endResetModel();

        m_gameId = QString();
        m_cursor = QString();
}
