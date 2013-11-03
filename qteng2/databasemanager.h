#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class QSqlQuery;
class Item;

class DatabaseManager
{
public:
    static DatabaseManager& getInstance()
    {
        static DatabaseManager instance; // Guaranteed to be destroyed.
        return instance;
    }

    bool open( const QString& filename );
    void close();
    qulonglong irreversableItemCount();
    qulonglong reversableItemCount();
    void itemsAtIndex(int idx, Item** item, Item** revItem);

private:
    DatabaseManager();
    DatabaseManager(DatabaseManager const&);
    void operator=(DatabaseManager const&);

private:
    QSqlDatabase m_db;
    QSqlQuery*   m_query;
};

#endif // DATABASEMANAGER_H

