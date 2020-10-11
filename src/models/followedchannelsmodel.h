#ifndef FOLLOWEDCHANNELSMODEL_H
#define FOLLOWEDCHANNELSMODEL_H

#include <QAbstractListModel>
#include <Twitch>

using FollowedChannels = Twitch::Streams;
using FollowedChannel = Twitch::Stream;

class FollowedChannelsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FollowedChannelsModel(QObject *parent = nullptr);

    enum {
        TitleRole = Qt::UserRole,
        UserNameRole,
        ThumbnailUrlRole,
        StartedAtRole,
        ViewerCountRole,
        GameIdRole,
        LanguageRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void getChannels();
signals:
    void getFollowedChannelsNameFinished();
    void rowCountChanged(int count);

private:
    void getFollowedChannelsName();
    FollowedChannels m_channels;
    QStringList m_followedChannels;
};

#endif // FOLLOWEDCHANNELSMODEL_H
