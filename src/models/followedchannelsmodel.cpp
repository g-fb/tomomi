#include "followedchannelsmodel.h"
#include "../application.h"

FollowedChannelsModel::FollowedChannelsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &FollowedChannelsModel::getFollowedChannelsFinished,
            this, &FollowedChannelsModel::getLiveChannels);
}

int FollowedChannelsModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_channels.count();
}

QVariant FollowedChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FollowedChannel channel = m_channels[index.row()];

    switch (role) {
    case TitleRole:
        return QVariant(channel.m_title);
    case UserNameRole:
        return QVariant(channel.m_userName);
    case UserIdRole:
        return QVariant(channel.m_userId);
    case ThumbnailUrlRole:
        return QVariant(channel.m_thumbnailUrl);
    case StartedAtRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel.m_startedAt.toSecsSinceEpoch();
        auto uptime = QDateTime::fromSecsSinceEpoch(secondsSinceStart - 10800).toUTC();
        return QVariant(uptime.toString("hh:mm:ss"));
    }
    case ViewerCountRole:
        return QVariant(channel.m_viewerCount);
    case GameIdRole:
        return QVariant(channel.m_gameId);
    case LanguageRole:
        return QVariant(channel.m_language);
    }

    return QVariant();
}

QHash<int, QByteArray> FollowedChannelsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[UserNameRole] = "userName";
    roles[UserIdRole] = "userId";
    roles[ThumbnailUrlRole] = "thumbnailUrl";
    roles[StartedAtRole] = "startedAt";
    roles[ViewerCountRole] = "viewerCount";
    roles[GameIdRole] = "gameId";
    roles[LanguageRole] = "language";

    return roles;
}

void FollowedChannelsModel::getFollowedChannels()
{
    m_followedChannels.clear();
    auto api = Application::instance()->getApi();
    Twitch::UserFollowsReply *reply = api->getUserFollowsFromId(QString::number(440287663));
    connect(reply, &Twitch::UserFollowsReply::finished, this, [=]() {
        auto const users = reply->data().value<Twitch::UserFollows>();

        for (const auto &follow : users.m_follows) {
            m_followedChannels << follow.m_toId;
        }
        emit getFollowedChannelsFinished();
        reply->deleteLater();
    });
}

void FollowedChannelsModel::getLiveChannels()
{
    if (m_followedChannels.isEmpty()) {
        return;
    }

    beginResetModel();
    m_channels.clear();
    endResetModel();

    auto api = Application::instance()->getApi();
    Twitch::StreamsReply *reply = api->getStreamsByUserIds(m_followedChannels);
    connect(reply, &Twitch::StreamsReply::finished, this, [=]() {
        auto const channels = reply->data().value<Twitch::Streams>();

        auto oldRowCount = rowCount();

        beginInsertRows(QModelIndex(), 0, channels.count() - 1);
        int i{ 0 };
        for (const auto &channel : channels) {
            m_channels.insert(i, channel);
            i++;
        }
        endInsertRows();
        if (oldRowCount != channels.count()) {
            emit rowCountChanged(m_channels.count());
        }
        reply->deleteLater();
    });
}
