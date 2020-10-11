#ifndef GAMESMODEL_H
#define GAMESMODEL_H

#include <QAbstractListModel>

using Games = QMap<int, QMap<QString, QString>>;
using GameItem = QMap<QString, QString>;

class GamesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GamesModel(QObject *parent = nullptr);

    enum Game {
        Id = Qt::UserRole,
        Display,
        Decoration,
        ImageWidth,
        ImageHeight,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

//    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    void getGames();
    Games m_games;
};

#endif // GAMESMODEL_H