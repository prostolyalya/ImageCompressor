#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H
#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>
#include "include/file_model.h"

class UiController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uiPath READ getPathText WRITE setPathText NOTIFY pathChanged)
    Q_PROPERTY(QString uiError READ getErrorText WRITE setErrorText NOTIFY errorChanged)

public:
    UiController(QQmlApplicationEngine &engine, QObject *parent = nullptr);
    ~UiController();

    QString getPathText() const;
    void setPathText(const QString &value);

    QString getErrorText() const;

    FileModel *getFileModel();
public slots:
    void updateDir(const QString &path);
    void setErrorText(const QString &value);

signals:
    void listFilesChanged();
    void pathChanged();
    void errorChanged();
    void change(QString filepath);

private:
    QString m_path = "";
    QString m_error;
    QQmlApplicationEngine &m_engine;
    std::unique_ptr<FileModel> m_model;
};

#endif // UI_CONTROLLER_H
