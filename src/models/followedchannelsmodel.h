#ifndef FOLLOWEDCHANNELSMODEL_H
#define FOLLOWEDCHANNELSMODEL_H

#include <QAbstractListModel>
#include <TwitchQt>

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
        GameRole,
        UserIdRole,
        ThumbnailUrlRole,
        TimestampRole,
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
    void newLiveChannel(const QString &userName, const QString &userId, const QString &title);
    void openChannel(const QString &userName, const QString &userId);
    void rowCountChanged(int count);
    void gamesRetrieved(QMap<QString, QString> gameNames);

private:
    void getLiveChannels();
    void newLiveChannelNotification(const QString &userName, const QString &userId, const QString &title);
    FollowedChannels m_channels;
    QStringList m_followedChannels;
    QStringList m_oldFollowedChannels;
    QMap<QString, QString> m_gameNames;
};

#endif // FOLLOWEDCHANNELSMODEL_H
