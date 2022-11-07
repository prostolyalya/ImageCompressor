#include "include/file_model.h"

FileModel::FileModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int FileModel::rowCount(const QModelIndex& parent) const
{
    return m_model.size();
}

QVariant FileModel::data(const QModelIndex& index, int role) const
{
    if (index.row() >= m_model.size() || index.row() < 0)
    {
        return QVariant();
    }
    if (role == Type)
    {
        return m_model[index.row()].m_type;
    }
    else if (role == Name)
    {
        return m_model[index.row()].m_name;
    }
    else if (role == Size)
    {
        return m_model[index.row()].m_size;
    }
    else if (role == InProcess)
    {
        return m_model[index.row()].m_working;
    }
    return QVariant();
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Type] = "type";
    roles[Name] = "name";
    roles[Size] = "size";
    roles[InProcess] = "working";
    return roles;
}

void FileModel::change_model(QList<File> model)
{
    beginResetModel();
    m_model.clear();
    endResetModel();

    beginInsertRows({}, 0, model.count() - 1);
    for (const auto& item : model)
    {
        m_model.push_back(item);
    }
    endInsertRows();
}
