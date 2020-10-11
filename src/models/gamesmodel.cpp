#include "gamesmodel.h"
#include "../application.h"

GamesModel::GamesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    getGames();
}

int GamesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_games.count();
}

QVariant GamesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    GameItem game = m_games[index.row()];

    switch (role) {
    case IdRole:
        return QVariant(game.m_id);
    case DisplayRole:
        return QVariant(game.m_name);
    case BoxArtUrlRole:
        return QVariant(game.m_boxArtUrl.replace("{width}x{height}", "200x265"));
    case ImageWidthRole:
        return QVariant(QString::number(200));
    case ImageHeightRole:
        return QVariant(QString::number(265));
    }

    return QVariant();
}

QHash<int, QByteArray> GamesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "gameId";
    roles[DisplayRole] = "display";
    roles[BoxArtUrlRole] = "boxArtUrl";
    roles[ImageWidthRole] = "imageWidth";
    roles[ImageHeightRole] = "imageHeight";
    return roles;
}

//bool GamesModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//    return true;
//}

//bool GamesModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//    return true;
//}

void GamesModel::getGames()
{
    auto api = Application::instance()->getApi();

    beginResetModel();
    m_games.clear();
    endResetModel();

    Twitch::GamesReply *reply = api->getTopGames(24);
    connect(reply, &Twitch::GamesReply::finished, this, [=]() {
        beginInsertRows(QModelIndex(), 0, 24 - 1);
        m_games = reply->data().value<Twitch::Games>();
        endInsertRows();

        reply->deleteLater();
    });
}
