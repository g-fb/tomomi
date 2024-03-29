/*
 * SPDX-FileCopyrightText: 2024 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef VIDEOSMODEL_H
#define VIDEOSMODEL_H

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

#include <TwitchQt>

using VideoItem = Twitch::Video;

class VideosModel : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(VideosModel)

public:
    explicit VideosModel(QObject *parent = nullptr);

    enum Video {
        IdRole = Qt::UserRole,
        StreamIdRole,
        UserIdRole,
        UserLoginRole,
        UserNameRole,
        TitleRole,
        DescriptionRole,
        CreatedAtRole,
        PublishedAtRole,
        UrlRole,
        ThumbnailUrlRole,
        ViewableRole,
        ViewCountRole,
        LanguageRole,
        TypeRole,
        DurationRole,
        MutedSegmentsRole,
        ImageWidthRole,
        ImageHeightRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void getVideos(const QString &userId, bool reset = true);

private:
    void resetModel();
    Twitch::Videos m_videos;
    QString m_cursor;

};

#endif // VIDEOSMODEL_H
