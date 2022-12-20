#include "followedchannelsmodel.h"
#include "application.h"

#include <QApplication>

#include <KNotifications/KNotification>

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

    FollowedChannel *channel = m_channels[index.row()];

    switch (role) {
    case TitleRole:
        return QVariant(channel->title);
    case UserNameRole:
        return QVariant(channel->userName);
    case GameRole:
        return QVariant(m_gameNames[channel->gameName]);
    case UserIdRole:
        return QVariant(channel->userId);
    case ThumbnailUrlRole:
        return QVariant(channel->thumbnailUrl.replace("{width}x{height}", "440x248"));
    case ProfileImageUrlRole:
        return QVariant(channel->profileImageUrl.replace("{width}x{height}", "300x300"));
    case TimestampRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel->startedAt.toSecsSinceEpoch();
        return QVariant(secondsSinceStart);
    }
    case StartedAtRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel->startedAt.toSecsSinceEpoch();
        auto uptime = QDateTime::fromSecsSinceEpoch(secondsSinceStart).toUTC();
        return QVariant(uptime.toString("hh:mm:ss"));
    }
    case ViewerCountRole:
        return QVariant(channel->viewerCount);
    case GameIdRole:
        return QVariant(channel->gameName);
    case LanguageRole:
        return QVariant(channel->language);
    case IsLiveRole:
        return QVariant(channel->isLive);
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

void FollowedChannelsModel::getUserInfo()
{
    // todo: search a local cache for the ids and only make a request for ids not in the cache

    auto api = Application::instance()->getApi();
    Twitch::UsersReply *reply = api->getUserByIds(m_followedChannels);


    auto onReplyFinished = [=]() {
        auto const users = reply->data().value<Twitch::Users>();

        beginInsertRows(QModelIndex(), 0, users.count() - 1);
        for (const auto &user : users) {
            auto channel = new FollowedChannel();
            channel->userId = user.m_id;
            channel->userName = user.m_displayName;
            channel->profileImageUrl = user.m_profileImageUrl;

            m_channels.append(channel);
        }
        endInsertRows();

        reply->deleteLater();

        getLiveChannels();
    };

    connect(reply, &Twitch::StreamsReply::finished, this, onReplyFinished);
}

void FollowedChannelsModel::getLiveChannels()
{
    auto oldRowCount = rowCount();

    beginResetModel();
    endResetModel();

    auto api = Application::instance()->getApi();
    Twitch::StreamsReply *reply = api->getStreamsByUserIds(m_followedChannels);
    auto onReplyFinished = [=]() {
        auto const streams = reply->data().value<Twitch::Streams>();

        QStringList ids;
        for (const auto &stream : streams) {
            for (int i {0}; i < m_channels.count(); ++i) {
                auto *channel = m_channels[i];
                if (channel->userId == stream.m_userId) {
                    channel->title = stream.m_title;
                    channel->gameName = stream.m_gameId;
                    channel->language = stream.m_language;
                    channel->thumbnailUrl = stream.m_thumbnailUrl;
                    channel->startedAt = stream.m_startedAt;
                    channel->viewerCount = stream.m_viewerCount;
                    channel->isLive = true;
                    Q_EMIT dataChanged(index(i, 0), index(i, 0));
                    break;
                }
            }

            ids << stream.m_gameId;
            if (!m_oldFollowedChannels.contains(stream.m_userId)) {
                emit newLiveChannel(stream.m_userName, stream.m_userId, stream.m_title);
            }
        }
        m_oldFollowedChannels = m_followedChannels;

        if (oldRowCount != streams.count()) {
            emit rowCountChanged(streams.count());
        }

        Twitch::GamesReply *gamesReply = api->getGameByIds(ids);
        connect(gamesReply, &Twitch::GamesReply::finished, this, [=]() {
            auto games = gamesReply->data().value<Twitch::Games>();
            QMap<QString, QString> gameNames;
            for (const auto &game : qAsConst(games)) {
                gameNames.insert(game.m_id, game.m_name);
            }
            emit gamesRetrieved(gameNames);
        });
        reply->deleteLater();
    };
    connect(reply, &Twitch::StreamsReply::finished, this, onReplyFinished);
}

void FollowedChannelsModel::newLiveChannelNotification(const QString &userName,
                                                       const QString &userId,
                                                       const QString &title)
{
    if (qApp->focusWindow()) {
        return;
    }
    auto *notification = new KNotification("newlivechannel", KNotification::CloseOnTimeout, this);
    notification->setComponentName(QStringLiteral("tomomi"));
    notification->setUrgency(KNotification::LowUrgency);
    notification->setTitle(QString("%1 is live").arg(userName));
    notification->setText(title);
    notification->setActions(QStringList("Open"));
    notification->setDefaultAction(QStringLiteral("Open"));
    notification->sendEvent();

    QObject::connect(notification, &KNotification::action1Activated, this, [=]() {
        emit openChannel(userName, userId);
    });
    QObject::connect(notification, &KNotification::defaultActivated, this, [=]() {
        emit openChannel(userName, userId);
    });
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
