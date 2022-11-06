#include "include/controller.h"
#include <future>
#include <QDir>
#include <QQmlContext>

Controller::Controller(QString path, QQmlApplicationEngine& engine, QObject* parent)
    : m_qml_engine(engine)
    , m_ui_controller(std::make_unique<UiController>())
    , m_model(std::make_unique<FileModel>())
    , m_current_path(path)
    , QObject(parent)
{
    m_qml_engine.rootContext()->setContextProperty("uiController", m_ui_controller.get());
    m_qml_engine.rootContext()->setContextProperty("uiModel", m_model.get());

    connect(m_ui_controller.get(), &UiController::change, this, &Controller::change,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::updateDirUi, m_ui_controller.get(), &UiController::updateDir,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::changeStatus, m_ui_controller.get(), &UiController::changeStatus,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::errorTriggered, this, &Controller::showErrorMessage,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::changeErrorText, m_ui_controller.get(), &UiController::setErrorText,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::updateModel, m_model.get(), &FileModel::updateModel,
            Qt::ConnectionType::QueuedConnection);

    updateDir();
}

QString convertErrorToString(ImageCompress::CompressError error)
{
    switch (error)
    {
    case ImageCompress::CompressError::INTERNAL_ERROR:
        return "INTERNAL_ERROR";
    case ImageCompress::CompressError::READING_IMAGE:
        return "READING_IMAGE_ERROR";
    case ImageCompress::CompressError::OK:
    default:
        break;
    }
    return {};
}

void Controller::change(const QString& path)
{
    if (path.endsWith(ImageCompress::file_extension_compressed))
    {
        emit changeStatus();
        auto callback = [&](ImageCompress::CompressError error) {
            emit errorTriggered(convertErrorToString(error));
        };
        auto task =
            std::async(std::launch::async, &ImageCompress::uncompress_image, path.toStdString(), callback);
    }
    else if (path.endsWith(ImageCompress::file_extension_uncompressed))
    {
        emit changeStatus();
        auto callback = [&](ImageCompress::CompressError error) {
            emit errorTriggered(convertErrorToString(error));
        };
        auto task =
            std::async(std::launch::async, &ImageCompress::compress_image, path.toStdString(), callback);
    }
    else
    {
        if (path == "..")
        {
            m_current_path.truncate(m_current_path.lastIndexOf("/"));
        }
        else
        {
            m_current_path += "/" + path;
        }
        updateDir();
    }
}

void Controller::showErrorMessage(const QString& error)
{
    emit changeStatus();
    if (!error.isEmpty())
    {
        emit changeErrorText(error);
        const QUrl url(QStringLiteral("qrc:/ui/error.qml"));
        m_qml_engine.load(url);
    }
    updateDir();
}

void Controller::updateDir()
{
    QFileInfo fi(m_current_path);
    if (fi.exists() && fi.isFile())
    {
        m_current_path.truncate(m_current_path.lastIndexOf("/"));
    }
    QDir dir(m_current_path);
    auto listImages = dir.entryList({ QString("*") + ImageCompress::file_extension_uncompressed });
    auto listComressed = dir.entryList({ QString("*") + ImageCompress::file_extension_compressed });
    auto listFolders = dir.entryList(QDir::Dirs | QDir::NoDot);
    QList<File> model;
    model.reserve(listImages.size() + listComressed.size() + listFolders.size());
    for (const auto& folder : listFolders)
    {
        auto size = QFileInfo(folder).size() / 1000;

        model.push_back({ FileType::Folder, folder, QString::number(size) + " kB" });
    }
    for (const auto& image : listImages)
    {
        auto size = QFileInfo(image).size() / 1000;

        model.push_back({ FileType::Image, image, QString::number(size) + " kB" });
    }
    for (const auto& image : listComressed)
    {
        auto size = QFileInfo(image).size() / 1000;

        model.push_back({ FileType::CompressedImage, image, QString::number(size) + " kB" });
    }
    emit updateModel(model);
    emit updateDirUi(m_current_path);
}
