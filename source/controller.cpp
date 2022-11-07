#include "include/controller.h"
#include <future>
#include <QDir>
#include <QQmlContext>

Controller::Controller(QString path, QQmlApplicationEngine& engine, QObject* parent)
    : m_ui_controller(std::make_unique<UiController>(engine))
    , m_current_path(path)
    , QObject(parent)
{
    connect(m_ui_controller.get(), &UiController::change, this, &Controller::change,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::updateDirUi, m_ui_controller.get(), &UiController::updateDir,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::errorTriggered, this, &Controller::showErrorMessage,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::changeErrorText, m_ui_controller.get(), &UiController::setErrorText,
            Qt::ConnectionType::QueuedConnection);
    connect(this, &Controller::updateModel, m_ui_controller->getFileModel(), &FileModel::change_model,
            Qt::ConnectionType::QueuedConnection);
    if (m_current_path.contains("./"))
    {
        m_current_path = QDir::homePath();
    }
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
        auto filepath = m_current_path + "/" + path;
        m_working_files.append(filepath);
        updateDir();
        auto callback = [&](ImageCompress::CompressError error) {
            emit errorTriggered(convertErrorToString(error), filepath);
        };
        auto task = std::async(std::launch::async, &ImageCompress::uncompress_image, filepath.toStdString(),
                               callback);
    }
    else if (path.endsWith(ImageCompress::file_extension_uncompressed))
    {
        auto filepath = m_current_path + "/" + path;
        m_working_files.append(filepath);
        updateDir();
        auto callback = [&](ImageCompress::CompressError error) {
            emit errorTriggered(convertErrorToString(error), filepath);
        };
        auto task =
            std::async(std::launch::async, &ImageCompress::compress_image, filepath.toStdString(), callback);
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

void Controller::showErrorMessage(const QString& error, const QString& path)
{
    m_working_files.removeAt(m_working_files.indexOf(path));
    if (!error.isEmpty())
    {
        emit changeErrorText(error);
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
    auto listFolders = m_current_path == QDir::homePath()
        ? dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot)
        : dir.entryList(QDir::Dirs | QDir::NoDot);
    QList<File> model;
    model.reserve(listImages.size() + listComressed.size() + listFolders.size());
    for (const auto& folder : listFolders)
    {
        auto size = QFile(folder).size() / 1000;

        model.push_back({ FileType::Folder, folder, QString::number(size) + " kB", false });
    }
    for (const auto& image : listImages)
    {
        auto size = QFile(image).size() / 1000;

        model.push_back({ FileType::Image, image, QString::number(size) + " kB",
                          m_working_files.contains(m_current_path + "/" + image) });
    }
    for (const auto& image : listComressed)
    {
        auto size = QFile(image).size() / 1000;

        model.push_back({ FileType::CompressedImage, image, QString::number(size) + " kB",
                          m_working_files.contains(m_current_path + "/" + image) });
    }
    emit updateModel(model);
    emit updateDirUi(m_current_path);
}
