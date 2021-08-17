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
        GameRole,
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
    void newLiveChannel(const QString &userName, const QString &userId, const QString &title);
    void rowCountChanged(int count);
    void gamesRetrieved(QMap<QString, QString> gameNames);

private:
    void getLiveChannels();
    FollowedChannels m_channels;
    QStringList m_followedChannels;
    QStringList m_oldFollowedChannels;
    QMap<QString, QString> m_gameNames;
};

#endif // FOLLOWEDCHANNELSMODEL_H
