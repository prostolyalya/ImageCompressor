#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QQmlApplicationEngine>
#include <memory>
#include <image_compress/include/ImageCompressor.h>
#include <QThread>
#include "include/ui_controller.h"
#include "include/file_model.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QString path, QQmlApplicationEngine& engine, QObject* parent = nullptr);

signals:
    void changeStatus();
    void updateDirUi(const QString& path);
    void errorTriggered(const QString& error);
    void changeErrorText(const QString& error);
    void updateModel(const QList<File>& model);

public slots:
    void change(const QString& path);
    void showErrorMessage(const QString& error);

private:
    void updateDir();

private:
    QString m_current_path;
    std::unique_ptr<UiController> m_ui_controller;
    std::unique_ptr<FileModel> m_model;
    QQmlApplicationEngine& m_qml_engine;
};

#endif // CONTROLLER_H
