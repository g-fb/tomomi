#ifndef CHANNELSMODEL_H
#define CHANNELSMODEL_H

#include <QAbstractListModel>

using Channels = QMap<int, QMap<QString, QString>>;
using ChannelItem = QMap<QString, QString>;

class ChannelsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChannelsModel(QObject *parent = nullptr);

    enum Channel {
        Id = Qt::UserRole,
        Title,
        ViewerCount,
        UserName,
        Thumbnail,
        ThumbnailWidth,
        ThumbnailHeight,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

//    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void getChannels(const QString &gameId);
private:
    Channels m_channels;
};

#endif // CHANNELSMODEL_H
