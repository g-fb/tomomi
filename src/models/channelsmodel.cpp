/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

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
        return QVariant(channel.m_thumbnailUrl.replace(u"{width}x{height}"_qs, u"440x248"_qs));
    case ThumbnailWidthRole:
        return QVariant(QString::number(440));
    case ThumbnailHeightRole:
        return QVariant(QString::number(248));
    case TimestampRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel.m_startedAt.toSecsSinceEpoch();
        return QVariant(secondsSinceStart);
    }
    case StartedAtRole: {
        auto secondsSinceStart = QDateTime::currentSecsSinceEpoch() - channel.m_startedAt.toSecsSinceEpoch();
        auto uptime = QDateTime::fromSecsSinceEpoch(secondsSinceStart).toUTC();
        return QVariant(uptime.toString(u"hh:mm:ss"_qs));
    }
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
    roles[TimestampRole] = "timestamp";
    roles[StartedAtRole] = "startedAt";
    return roles;
}

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

#include "moc_channelsmodel.cpp"
