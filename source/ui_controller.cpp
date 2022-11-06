#include "include/ui_controller.h"
#include <QVariant>
constexpr const char *compressingText = "Compressing/Uncomressing...";

UiController::UiController(QObject *parent)
    : QObject(parent)
{
}

UiController::~UiController()
{
}

void UiController::changeStatus()
{
    if (m_statusText.isEmpty())
    {
        m_statusText = compressingText;
    }
    else
    {
        m_statusText.clear();
    }
    emit statusTextChanged();
}

void UiController::updateDir(const QString &path)
{
    setPathText(path);
}

void UiController::setErrorText(const QString &value)
{
    m_error = value;
    emit errorChanged();
}

QString UiController::getStatusText() const
{
    return m_statusText;
}

void UiController::setStatusText(const QString &value)
{
    m_statusText = value;
    emit statusTextChanged();
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
