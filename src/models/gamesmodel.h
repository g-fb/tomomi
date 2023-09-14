#ifndef GAMESMODEL_H
#define GAMESMODEL_H

#include <QAbstractListModel>
#include <TwitchQt>

using Games = Twitch::Games;
using GameItem = Twitch::Game;

class GamesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GamesModel(QObject *parent = nullptr);

    enum Game {
        IdRole = Qt::UserRole,
        DisplayRole,
        CoverRole,
        ImageWidthRole,
        ImageHeightRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

//    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void getGames(bool reset = true);

private:
    void resetModel();
    void downloadGameCover(const QUrl &url, int gameId) const;
    Games m_games;
    QString m_cursor;
};

#endif // GAMESMODEL_H
