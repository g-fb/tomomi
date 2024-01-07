/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

#include <TwitchQt>

using Channels = Twitch::Streams;
using ChannelItem = Twitch::Stream;

class ChannelsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ChannelsModel)

    Q_PROPERTY(QString gameId MEMBER m_gameId CONSTANT)

public:
    explicit ChannelsModel(QObject *parent = nullptr);

    enum Channel {
        IdRole = Qt::UserRole,
        TitleRole,
        ViewerCountRole,
        UserNameRole,
        UserIdRole,
        ThumbnailRole,
        ThumbnailWidthRole,
        ThumbnailHeightRole,
        StartedAtRole,
        TimestampRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void getChannels(const QString &gameId, bool reset = true);
private:
    void resetModel();
    Channels m_channels;
    QString m_cursor;
    QString m_gameId;
};

#endif // CHANNELSMODEL_H
