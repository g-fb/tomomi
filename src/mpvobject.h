/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

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
               NOTIFY pauseChanged)

    Q_PROPERTY(bool mute
               READ mute
               NOTIFY muteChanged)

    Q_PROPERTY(int volume
               READ volume
               WRITE setVolume
               NOTIFY volumeChanged)

    QString mediaTitle();
    bool pause();
    bool mute();
    int volume();
    void setVolume(int value);

    mpv_handle *mpv;
    mpv_render_context *mpv_gl;

    friend class MpvRenderer;
public:
    MpvObject(QQuickItem * parent = 0);
    virtual ~MpvObject();
    virtual Renderer *createRenderer() const;

public slots:
    static void on_mpv_events(void *ctx);
    void eventHandler();
    int setProperty(const QString &name, const QVariant &value);
    QVariant getProperty(const QString &name);
    QVariant command(const QVariant &params);

signals:
    void mediaTitleChanged();
    void pauseChanged();
    void muteChanged();
    void volumeChanged();
    void fileLoaded();
    void endOfFile();
    void ready();
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
