/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "followedchannelsmodel.h"

#include <QApplication>

#include <KNotifications/KNotification>

#include "application.h"

FollowedChannelsModel::FollowedChannelsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &FollowedChannelsModel::getFollowedChannelsFinished,
            this, &FollowedChannelsModel::getUserInfo);

    connect(this, &FollowedChannelsModel::newLiveChannel,
            this, &FollowedChannelsModel::newLiveChannelNotification);

    connect(this, &FollowedChannelsModel::gamesRetrieved, this, [=](QMap<QString, QString> games) {
        m_gameNames = games;
        Q_EMIT dataChanged(index(0, 0), index(rowCount(), 0));
    });
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
        return QVariant(channel.title);
    case UserNameRole:
        return QVariant(channel.userName);
    case GameRole:
        return QVariant(m_gameNames[channel.gameName]);
    case UserIdRole:
        return QVariant(channel.userId);
    case ThumbnailUrlRole:
        return QVariant(channel.thumbnailUrl.replace(u"{width}x{height}"_qs, u"440x248"_qs));
    case ProfileImageUrlRole:
        return QVariant(channel.profileImageUrl.replace(u"{width}x{height}"_qs, u"300x300"_qs));
    case TimestampRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel.startedAt.toSecsSinceEpoch();
        return QVariant(secondsSinceStart);
    }
    case StartedAtRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel.startedAt.toSecsSinceEpoch();
        auto uptime = QDateTime::fromSecsSinceEpoch(secondsSinceStart).toUTC();
        return QVariant(uptime.toString(u"hh:mm:ss"_qs));
    }
    case ViewerCountRole:
        return QVariant(channel.viewerCount);
    case GameIdRole:
        return QVariant(channel.gameName);
    case LanguageRole:
        return QVariant(channel.language);
    case IsLiveRole:
        return QVariant(channel.isLive);
    }

    return QVariant();
}

QHash<int, QByteArray> FollowedChannelsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[UserNameRole] = "userName";
    roles[GameRole] = "game";
    roles[UserIdRole] = "userId";
    roles[TimestampRole] = "timestamp";
    roles[ThumbnailUrlRole] = "thumbnailUrl";
    roles[ProfileImageUrlRole] = "profileImageUrl";
    roles[StartedAtRole] = "startedAt";
    roles[ViewerCountRole] = "viewerCount";
    roles[GameIdRole] = "gameId";
    roles[LanguageRole] = "language";
    roles[IsLiveRole] = "isLive";

    return roles;
}

void FollowedChannelsModel::getFollowedChannels()
{
    m_channels.clear();
    m_followedChannels.clear();
    auto api = Application::instance()->getApi();
    Twitch::ChannelsReply *reply = api->getFollowedChannel(QString::number(440287663));
    connect(reply, &Twitch::ChannelsReply::finished, this, [=]() {
        auto const channels = reply->data().value<Twitch::Channels>();

        for (const Twitch::Channel &channel : channels) {
            m_followedChannels << channel.broadcasterId;
        }
        Q_EMIT getFollowedChannelsFinished();
        reply->deleteLater();
    });
}

void FollowedChannelsModel::getUserInfo()
{
    // todo: search a local cache for the ids and only make a request for ids not in the cache

    auto api = Application::instance()->getApi();
    Twitch::UsersReply *reply = api->getUserByIds(m_followedChannels);


    beginResetModel();
    m_channels.clear();
    endResetModel();

    auto onReplyFinished = [=]() {
        auto const users = reply->data().value<Twitch::Users>();

        for (const auto &user : users) {
            FollowedChannel channel;
            channel.userId = user.m_id;
            channel.userName = user.m_displayName;
            channel.profileImageUrl = user.m_profileImageUrl;

            m_channels.append(channel);
        }

        std::sort(m_channels.begin(), m_channels.end(), [](FollowedChannel a, FollowedChannel b) {
            return a.userName < b.userName;
        });

        reply->deleteLater();

        getLiveChannels();
    };

    connect(reply, &Twitch::StreamsReply::finished, this, onReplyFinished);
}

void FollowedChannelsModel::getLiveChannels()
{
    auto api = Application::instance()->getApi();
    Twitch::StreamsReply *reply = api->getStreamsByUserIds(m_followedChannels);

    auto onReplyFinished = [=]() {
        auto const streams = reply->data().value<Twitch::Streams>();

        QStringList ids;
        for (const auto &stream : streams) {
            for (int i {0}; i < m_channels.count(); ++i) {
                auto &channel = m_channels[i];

                if (channel.userId == stream.m_userId) {
                    channel.title = stream.m_title;
                    channel.gameName = stream.m_gameId;
                    channel.language = stream.m_language;
                    channel.thumbnailUrl = stream.m_thumbnailUrl;
                    channel.startedAt = stream.m_startedAt;
                    channel.viewerCount = stream.m_viewerCount;
                    channel.isLive = true;
                    m_channels.move(i, 0);
                    break;
                }
            }
            ids << stream.m_gameId;
            if (!m_oldFollowedChannels.contains(stream.m_userId)) {
                Q_EMIT newLiveChannel(stream.m_userName, stream.m_userId, stream.m_title);
            }
        }

        setLiveChannelsCount(streams.count());

        beginInsertRows(QModelIndex(), 0, m_channels.count() - 1);
        endInsertRows();

        m_oldFollowedChannels = m_followedChannels;

        Twitch::GamesReply *gamesReply = api->getGameByIds(ids);
        connect(gamesReply, &Twitch::GamesReply::finished, this, [=]() {
            auto games = gamesReply->data().value<Twitch::Games>();
            QMap<QString, QString> gameNames;
            for (const auto &game : std::as_const(games)) {
                gameNames.insert(game.m_id, game.m_name);
            }
            Q_EMIT gamesRetrieved(gameNames);
        });
        reply->deleteLater();
    };
    connect(reply, &Twitch::StreamsReply::finished, this, onReplyFinished);
}

void FollowedChannelsModel::newLiveChannelNotification(const QString &userName,
                                                       const QString &userId,
                                                       const QString &title)
{
    auto *notification = new KNotification(u"newlivechannel"_qs, KNotification::CloseOnTimeout, this);
    notification->setComponentName(u"tomomi"_qs);
    notification->setUrgency(KNotification::LowUrgency);
    notification->setTitle(u"%1 is live"_qs.arg(userName));
    notification->setText(title);
    KNotificationAction *action = notification->addAction(u"Open"_qs);
    KNotificationAction *defaultAction = notification->addDefaultAction(u"Open"_qs);
    notification->sendEvent();

    QObject::connect(action, &KNotificationAction::activated, this, [=]() {
        Q_EMIT openChannel(userName, userId);
    });
    QObject::connect(defaultAction, &KNotificationAction::activated, this, [=]() {
        Q_EMIT openChannel(userName, userId);
    });
}

int FollowedChannelsModel::liveChannelsCount() const
{
    return m_liveChannelsCount;
}

void FollowedChannelsModel::setLiveChannelsCount(int _liveChannelsCount)
{
    if (m_liveChannelsCount == _liveChannelsCount) {
        return;
    }
    m_liveChannelsCount = _liveChannelsCount;
    Q_EMIT liveChannelsCountChanged();
}

ProxyFollowedChannelsModel::ProxyFollowedChannelsModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(FollowedChannelsModel::IsLiveRole);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

bool ProxyFollowedChannelsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex titleIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    bool isLive = sourceModel()->data(titleIndex, FollowedChannelsModel::IsLiveRole).toBool();

    return isLive;
}


#include "moc_followedchannelsmodel.cpp"
