/*
 * SPDX-FileCopyrightText: 2023 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef MPVPROPERTIES_H
#define MPVPROPERTIES_H

#include <QObject>

class MpvProperties : public QObject
{
    Q_OBJECT

public:
    static MpvProperties *self()
    {
        static MpvProperties p;
        return &p;
    }

    Q_PROPERTY(QString MediaTitle MEMBER MediaTitle CONSTANT)
    const QString MediaTitle{QStringLiteral("media-title")};

    Q_PROPERTY(QString Position MEMBER Position CONSTANT)
    const QString Position{QStringLiteral("time-pos")};

    Q_PROPERTY(QString Remaining MEMBER Remaining CONSTANT)
    const QString Remaining{QStringLiteral("time-remaining")};

    Q_PROPERTY(QString Duration MEMBER Duration CONSTANT)
    const QString Duration{QStringLiteral("duration")};

    Q_PROPERTY(QString Pause MEMBER Pause CONSTANT)
    const QString Pause{QStringLiteral("pause")};

    Q_PROPERTY(QString Volume MEMBER Volume CONSTANT)
    const QString Volume{QStringLiteral("volume")};

    Q_PROPERTY(QString Mute MEMBER Mute CONSTANT)
    const QString Mute{QStringLiteral("mute")};

    Q_PROPERTY(QString Chapter MEMBER Chapter CONSTANT)
    const QString Chapter{QStringLiteral("chapter")};

private:
    explicit MpvProperties(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    MpvProperties(const MpvProperties &) = delete;
    MpvProperties &operator=(const MpvProperties &) = delete;
    MpvProperties(MpvProperties &&) = delete;
    MpvProperties &operator=(MpvProperties &&) = delete;
};

#endif // MPVPROPERTIES_H
