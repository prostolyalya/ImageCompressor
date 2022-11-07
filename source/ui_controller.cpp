#include "include/ui_controller.h"
#include <QVariant>
#include <QQmlContext>

UiController::UiController(QQmlApplicationEngine &engine, QObject *parent)
    : m_engine(engine)
    , m_model(std::make_unique<FileModel>())
    , QObject(parent)
{
    m_engine.rootContext()->setContextProperty("uiController", this);
    m_engine.rootContext()->setContextProperty("uiModel", m_model.get());
}

UiController::~UiController()
{
}

void UiController::updateDir(const QString &path)
{
    setPathText(path);
}

void UiController::setErrorText(const QString &value)
{
    m_error = value;
    emit errorChanged();
    const QUrl url(QStringLiteral("qrc:/ui/error.qml"));
    m_engine.load(url);
}

QString UiController::getPathText() const
{
    return m_path;
}

void UiController::setPathText(const QString &value)
{
    m_path = value;
    emit pathChanged();
}

QString UiController::getErrorText() const
{
    return m_error;
}

FileModel *UiController::getFileModel()
{
    return m_model.get();
}
