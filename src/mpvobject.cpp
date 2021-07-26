/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "application.h"
#include "mpvobject.h"

#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <Twitch>

void on_mpv_redraw(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<MpvObject*>(ctx), "update", Qt::QueuedConnection);
}

static void *get_proc_address_mpv(void *ctx, const char *name)
{
    Q_UNUSED(ctx)

    QOpenGLContext *glctx = QOpenGLContext::currentContext();
    if (!glctx) return nullptr;

    return reinterpret_cast<void *>(glctx->getProcAddress(QByteArray(name)));
}

MpvRenderer::MpvRenderer(MpvObject *new_obj)
    : obj{new_obj}
{}

void MpvRenderer::render()
{
    obj->window()->resetOpenGLState();

    QOpenGLFramebufferObject *fbo = framebufferObject();
    mpv_opengl_fbo mpfbo;
    mpfbo.fbo = static_cast<int>(fbo->handle());
    mpfbo.w = fbo->width();
    mpfbo.h = fbo->height();
    mpfbo.internal_format = 0;

    mpv_render_param params[] = {
        // Specify the default framebuffer (0) as target. This will
        // render onto the entire screen. If you want to show the video
        // in a smaller rectangle or apply fancy transformations, you'll
        // need to render into a separate FBO and draw it manually.
        {MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };
    // See render_gl.h on what OpenGL environment mpv expects, and
    // other API details.
    mpv_render_context_render(obj->mpv_gl, params);

//    obj->window()->resetOpenGLState();
}

QOpenGLFramebufferObject * MpvRenderer::createFramebufferObject(const QSize &size)
{
    // init mpv_gl:
    if (!obj->mpv_gl)
    {
        mpv_opengl_init_params gl_init_params{get_proc_address_mpv, nullptr, nullptr};
        mpv_render_param params[]{
            {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
            {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
            {MPV_RENDER_PARAM_INVALID, nullptr}
        };

        if (mpv_render_context_create(&obj->mpv_gl, obj->mpv, params) < 0)
            throw std::runtime_error("failed to initialize mpv GL context");
        mpv_render_context_set_update_callback(obj->mpv_gl, on_mpv_redraw, obj);
        emit obj->ready();
    }

    return QQuickFramebufferObject::Renderer::createFramebufferObject(size);
}

MpvObject::MpvObject(QQuickItem * parent)
    : QQuickFramebufferObject(parent)
    , mpv{mpv_create()}
    , mpv_gl(nullptr)
{
    if (!mpv)
        throw std::runtime_error("could not create mpv context");

//    mpv_set_option_string(mpv, "terminal", "yes");
//    mpv_set_option_string(mpv, "msg-level", "all=v");
    setProperty("hwdec", "auto-safe");
    mpv_observe_property(mpv, 0, "media-title", MPV_FORMAT_STRING);
    mpv_observe_property(mpv, 0, "volume", MPV_FORMAT_INT64);
    mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_FLAG);
    mpv_observe_property(mpv, 0, "mute", MPV_FORMAT_FLAG);

    if (mpv_initialize(mpv) < 0)
        throw std::runtime_error("could not initialize mpv context");

    mpv_set_wakeup_callback(mpv, MpvObject::mpvEvents, this);
}

MpvObject::~MpvObject()
{
    // only initialized if something got drawn
    if (mpv_gl) {
        mpv_render_context_free(mpv_gl);
    }
    mpv_terminate_destroy(mpv);
}

QString MpvObject::mediaTitle()
{
    return getProperty("media-title").toString();
}

bool MpvObject::pause()
{
    return getProperty("pause").toBool();
}

void MpvObject::setPause(bool value)
{
    if (value == pause()) {
        return;
    }
    setProperty("pause", value);
    emit pauseChanged();
}

bool MpvObject::mute()
{
    return getProperty("mute").toBool();
}

void MpvObject::setMute(bool value)
{
    if (value == mute()) {
        return;
    }
    setProperty("mute", value);
    emit muteChanged();
}

int MpvObject::volume()
{
    return getProperty("volume").toInt();
}

void MpvObject::setVolume(int value)
{
    if (value == volume()) {
        return;
    }
    setProperty("volume", value);
    emit volumeChanged();
}

int MpvObject::userId()
{
    return m_userId;
}

void MpvObject::setUserId(int value)
{
    if (value == userId()) {
        return;
    }
    m_userId = value;
    emit userIdChanged();
}

int MpvObject::viewCount()
{
    return m_viewCount;
}

void MpvObject::userViewCount()
{
    auto api = Application::instance()->getApi();
    Twitch::StreamReply *reply = api->getStreamByUserId(QString::number(m_userId));

    connect(reply, &Twitch::StreamReply::finished, this, [=]() {
        auto const channel = reply->data().value<Twitch::Stream>();
        m_viewCount = channel.m_viewerCount;
        emit viewCountChanged();
        reply->deleteLater();
    });
}

QQuickFramebufferObject::Renderer *MpvObject::createRenderer() const
{
    window()->setPersistentOpenGLContext(true);
    window()->setPersistentSceneGraph(true);
    return new MpvRenderer(const_cast<MpvObject *>(this));
}

void MpvObject::mpvEvents(void *ctx)
{
    QMetaObject::invokeMethod(static_cast<MpvObject*>(ctx), "eventHandler", Qt::QueuedConnection);
}

void MpvObject::eventHandler()
{
    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE) {
            break;
        }
        switch (event->event_id) {
        case MPV_EVENT_FILE_LOADED: {
            emit fileLoaded();
            break;
        }
        case MPV_EVENT_END_FILE: {
            auto prop = (mpv_event_end_file *)event->data;
            if (prop->reason == MPV_END_FILE_REASON_EOF) {
                emit endOfFile();
            }
            break;
        }
        case MPV_EVENT_PROPERTY_CHANGE: {
            mpv_event_property *prop = (mpv_event_property *)event->data;

            if (strcmp(prop->name, "media-title") == 0) {
                if (prop->format == MPV_FORMAT_STRING) {
                    emit mediaTitleChanged();
                }
            } else if (strcmp(prop->name, "volume") == 0) {
                if (prop->format == MPV_FORMAT_INT64) {
                    emit volumeChanged();
                }
            } else if (strcmp(prop->name, "pause") == 0) {
                if (prop->format == MPV_FORMAT_FLAG) {
                    emit pauseChanged();
                }
            } else if (strcmp(prop->name, "mute") == 0) {
                if (prop->format == MPV_FORMAT_FLAG) {
                    emit muteChanged();
                }
            }
            break;
        }
        default: ;
            // Ignore uninteresting or unknown events.
        }
    }
}

int MpvObject::setProperty(const QString &name, const QVariant &value)
{
    return mpv::qt::set_property(mpv, name, value);
}

QVariant MpvObject::getProperty(const QString &name)
{
    auto value = mpv::qt::get_property(mpv, name);
    return value;
}

QVariant MpvObject::command(const QVariant &params)
{
    return mpv::qt::command(mpv, params);
}
