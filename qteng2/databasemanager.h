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

    bool updateItem( int id,
                     const QString& question,
                     const QString& answer,
                     const QString& example,
                     bool reversable,
                     unsigned int asked,
                     unsigned int answered,
                     unsigned int reversedAsked = 0,
                     unsigned int reversedAnswered = 0 );

    bool insertItem( const QString& question,
                     const QString& answer,
                     const QString& example,
                     bool reversable );

private:
    DatabaseManager();
    DatabaseManager(DatabaseManager const&);
    void operator=(DatabaseManager const&);

private:
    QSqlDatabase m_db;
    QSqlQuery*   m_query;
};

#endif // DATABASEMANAGER_H

