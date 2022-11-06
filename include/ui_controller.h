#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H
#pragma once

#include <QObject>
#include "include/file_model.h"

class UiController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uiStatus READ getStatusText WRITE setStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(QString uiPath READ getPathText WRITE setPathText NOTIFY pathChanged)
    Q_PROPERTY(QString uiError READ getErrorText WRITE setErrorText NOTIFY errorChanged)

public:
    UiController(QObject *parent = nullptr);
    ~UiController();

    QString getStatusText() const;
    void setStatusText(const QString &value);

    QString getPathText() const;
    void setPathText(const QString &value);

    QString getErrorText() const;

public slots:
    void changeStatus();
    void updateDir(const QString &path);
    void setErrorText(const QString &value);

signals:
    void statusTextChanged();
    void listFilesChanged();
    void pathChanged();
    void errorChanged();
    void change(QString filepath);

private:
    QString m_statusText = "";
    QString m_path = "";
    QString m_error;
};

#endif // UI_CONTROLLER_H
