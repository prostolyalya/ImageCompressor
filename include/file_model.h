#ifndef FILE_H
#define FILE_H
#include <QAbstractListModel>
#include <QHash>

enum FileType
{
    Folder,
    Image,
    CompressedImage
};

struct File
{
    FileType m_type;
    QString m_name;
    QString m_size;
    bool m_working;
};

class FileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FileProperties
    {
        Type = Qt::UserRole + 1,
        Name,
        Size,
        InProcess
    };

    FileModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void change_model(QList<File> model);

private:
    QList<File> m_model;
};
#endif // FILE_H
