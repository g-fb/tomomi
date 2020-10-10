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
    case Channel::Id:
        return QVariant(channel["id"]);
    case Channel::Title:
        return QVariant(channel["title"]);
    case Channel::ViewerCount:
        return QVariant(channel["viewerCount"]);
    case Channel::UserName:
        return QVariant(channel["userName"]);
    case Channel::Thumbnail:
        return QVariant(channel["thumbnailUrl"]);
    case Channel::ThumbnailWidth:
        return QVariant(channel["thumbnailWidth"]);
    case Channel::ThumbnailHeight:
        return QVariant(channel["thumbnailHeight"]);
    }

    return QVariant();
}

QHash<int, QByteArray> ChannelsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Channel::Id] = "id";
    roles[Channel::Title] = "title";
    roles[Channel::ViewerCount] = "viewerCount";
    roles[Channel::UserName] = "userName";
    roles[Channel::Thumbnail] = "thumbnailUrl";
    roles[Channel::ThumbnailWidth] = "thumbnailWidth";
    roles[Channel::ThumbnailHeight] = "thumbnailHeight";
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
    auto api = Application::instance()->getApi();

    Twitch::StreamsReply *reply = api->getStreamsByGameId(gameId);
    connect(reply, &Twitch::StreamsReply::finished, this, [=]() {
        auto channels = reply->data().value<Twitch::Streams>();

        QStringList ids;
        QMap<QString, QString> _channel;
        int i{ 0 };
        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        for (auto channel : channels) {
            _channel["id"] = channel.m_id;
            _channel["title"] = channel.m_title;
            _channel["viewerCount"] = channel.m_viewerCount;
            _channel["userName"] = channel.m_userName;
            _channel["thumbnailUrl"] = channel.m_thumbnailUrl.replace("{width}x{height}", "350x197");
            _channel["thumbnailWidth"] = QString::number(350);
            _channel["thumbnailHeight"] = QString::number(197);
            m_channels.insert(i, _channel);
            ++i;
        }
        endInsertRows();
        reply->deleteLater();
    });
}
