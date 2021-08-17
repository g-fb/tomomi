#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <QAbstractListModel>
#include <Twitch>

using Channels = Twitch::Streams;
using ChannelItem = Twitch::Stream;

class ChannelsModel : public QAbstractListModel
{
    Q_OBJECT
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

//    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void getChannels(const QString &gameId, bool reset = true);
private:
    void resetModel();
    Channels m_channels;
    QString m_cursor;
    QString m_gameId;
};

#endif // CHANNELSMODEL_H
