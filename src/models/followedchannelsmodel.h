#ifndef FOLLOWEDCHANNELSMODEL_H
#define FOLLOWEDCHANNELSMODEL_H

#include <QAbstractListModel>
#include <Twitch>

class KNotification;

using FollowedChannels = Twitch::Streams;
using FollowedChannel = Twitch::Stream;

class FollowedChannelsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(NOTIFY notificationActionActivated)

public:
    explicit FollowedChannelsModel(QObject *parent = nullptr);

    enum {
        TitleRole = Qt::UserRole,
        UserNameRole,
        UserIdRole,
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

    Q_INVOKABLE void getFollowedChannels();

signals:
    void getFollowedChannelsFinished();
    void rowCountChanged(int count);
    void notificationActionActivated(const QString &userName, const QString &userId);

private:
    void getLiveChannels();
    FollowedChannels m_channels;
    QStringList m_followedChannels;
    QStringList m_oldFollowedChannels;
    KNotification *m_notification;
};

#endif // FOLLOWEDCHANNELSMODEL_H
