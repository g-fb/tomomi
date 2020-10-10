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
    case Game::Id:
        return QVariant(game["id"]);
    case Game::Display:
        return QVariant(game["name"]);
    case Game::Decoration:
        return QVariant(game["imageUrl"]);
    case Game::ImageWidth:
        return QVariant(game["imageWidth"]);
    case Game::ImageHeight:
        return QVariant(game["imageHeight"]);
    }

    return QVariant();
}

QHash<int, QByteArray> GamesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Game::Id] = "gameId";
    roles[Game::Display] = "display";
    roles[Game::Decoration] = "decoration";
    roles[Game::ImageWidth] = "imageWidth";
    roles[Game::ImageHeight] = "imageHeight";
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

    Twitch::GamesReply *reply = api->getTopGames(24);
    connect(reply, &Twitch::GamesReply::finished, this, [=]() {
        auto games = reply->data().value<Twitch::Games>();

        QStringList ids;
        QMap<QString, QString> _game;
        int i{ 0 };
        beginInsertRows(QModelIndex(), 0, games.count() - 1);
        for (auto game : games) {
            _game["id"] = game.m_id;
            _game["name"] = game.m_name;
            _game["imageUrl"] = game.m_boxArtUrl.replace("{width}x{height}", "200x265");
            _game["imageWidth"] = QString::number(200);
            _game["imageHeight"] = QString::number(265);
            m_games.insert(i, _game);
            ++i;
        }
        endInsertRows();
        reply->deleteLater();
    });
}
