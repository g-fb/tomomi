#include "channelsmodel.h"
#include "../application.h"

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

void ChannelsModel::getChannels(const QString &gameId)
{
    beginResetModel();
    m_channels.clear();
    endResetModel();

    auto api = Application::instance()->getApi();

    Twitch::StreamsReply *reply = api->getStreamsByGameId(gameId, 24);
    connect(reply, &Twitch::StreamsReply::finished, this, [=]() {

        beginInsertRows(QModelIndex(), 0, 24 - 1);
        m_channels = reply->data().value<Twitch::Streams>();
        endInsertRows();

        reply->deleteLater();
    });
}
