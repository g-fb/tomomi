#ifndef FOLLOWEDCHANNELSMODEL_H
#define FOLLOWEDCHANNELSMODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <TwitchQt>

struct FollowedChannel {
    QString userId;
    QString userName;
    QString gameName;
    QString title;
    QString language;
    QString thumbnailUrl;
    QString profileImageUrl;
    QDateTime startedAt;
    int viewerCount {-1};
    bool isLive {false};
};

class ProxyFollowedChannelsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyFollowedChannelsModel(QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

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
        IsLiveRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void getFollowedChannels();
    Q_INVOKABLE void getUserInfo();

signals:
    void getFollowedChannelsFinished();
    void newLiveChannel(const QString &userName, const QString &userId, const QString &title);
    void openChannel(const QString &userName, const QString &userId);
    void rowCountChanged(int count);
    void gamesRetrieved(QMap<QString, QString> gameNames);

private:
    void getLiveChannels();
    void newLiveChannelNotification(const QString &userName, const QString &userId, const QString &title);
    QList<FollowedChannel*> m_channels;
    QStringList m_followedChannels;
    QStringList m_oldFollowedChannels;
    QMap<QString, QString> m_gameNames;
};

#endif // FOLLOWEDCHANNELSMODEL_H
