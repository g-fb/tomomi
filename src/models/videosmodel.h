#ifndef VIDEOSMODEL_H
#define VIDEOSMODEL_H

#include <QAbstractListModel>
#include <Twitch>

using VideoItem = Twitch::Video;

class VideosModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit VideosModel(QObject *parent = nullptr);

    enum Video {
        IdRole = Qt::UserRole,
        StreamIdRole,
        UserIdRole,
        UserNameRole,
        TypeRole,
        TitleRole,
        DescriptionRole,
        CreatedAtRole,
        PublishedAtRole,
        UrlRole,
        ThumbnailUrlRole,
        ViewCountRole,
        LanguageRole,
        DurationRole,
        ImageWidthRole,
        ImageHeightRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void getVideos(const QString &userId, bool reset = true);

private:
    void resetModel();
    Twitch::Videos m_videos;
    QString m_cursor;

};

#endif // VIDEOSMODEL_H
