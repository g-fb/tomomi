#ifndef MPVOBJECT_H
#define MPVOBJECT_H

#include <QtQuick/QQuickFramebufferObject>
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include "qthelper.h"

class MpvRenderer;

class MpvObject : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString mediaTitle
               READ mediaTitle
               NOTIFY mediaTitleChanged)

    Q_PROPERTY(bool pause
               READ pause
               WRITE setPause
               NOTIFY pauseChanged)

    Q_PROPERTY(bool mute
               READ mute
               WRITE setMute
               NOTIFY muteChanged)

    Q_PROPERTY(int volume
               READ volume
               WRITE setVolume
               NOTIFY volumeChanged)

    Q_PROPERTY(double duration
               READ duration
               NOTIFY durationChanged)

    Q_PROPERTY(double position
               READ position
               WRITE setPosition
               NOTIFY positionChanged)

    Q_PROPERTY(double remaining
               READ remaining
               NOTIFY remainingChanged)

    Q_PROPERTY(int viewCount
               MEMBER m_viewCount
               READ viewCount
               NOTIFY viewCountChanged)

    Q_PROPERTY(int userId
               MEMBER m_userId
               READ userId
               WRITE setUserId
               NOTIFY userIdChanged)

public:
    MpvObject(QQuickItem * parent = 0);
    virtual ~MpvObject();
    virtual Renderer *createRenderer() const;

    Q_INVOKABLE void userViewCount();

public slots:
    static void mpvEvents(void *ctx);
    void eventHandler();
    int setProperty(const QString &name, const QVariant &value);
    QVariant getProperty(const QString &name);
    QVariant command(const QVariant &params);

signals:
    void mediaTitleChanged();
    void pauseChanged();
    void muteChanged();
    void volumeChanged();
    void viewCountChanged();
    void durationChanged();
    void positionChanged();
    void remainingChanged();
    void userIdChanged();
    void fileLoaded();
    void endOfFile();
    void ready();

private:
    QString mediaTitle();
    bool pause();
    void setPause(bool value);
    bool mute();
    void setMute(bool value);
    int volume();
    void setVolume(int value);
    double duration();
    double remaining();
    double position();
    void setPosition(double value);

    int userId();
    void setUserId(int value);
    int viewCount();

    mpv_handle *mpv;
    mpv_render_context *mpv_gl;
    int m_userId {0};
    int m_viewCount {0};

    friend class MpvRenderer;
};

class MpvRenderer : public QQuickFramebufferObject::Renderer
{
public:
    MpvRenderer(MpvObject *new_obj);
    ~MpvRenderer() = default;

    MpvObject *obj;

    // This function is called when a new FBO is needed.
    // This happens on the initial frame.
    QOpenGLFramebufferObject * createFramebufferObject(const QSize &size);

    void render();
};

#endif // MPVOBJECT_H
