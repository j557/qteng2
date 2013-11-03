#include "databasemanager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QVariant>
#include "item.h"

DatabaseManager::DatabaseManager()
: m_query( NULL )
{
}

bool DatabaseManager::open( const QString& filename )
{
    close();

    if( !filename.length() )
    {
        qDebug("DB ERROR: empty filename");
        return false;
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName( filename );

    // Open databasee
    if( !m_db.open() )
    {
        qDebug("DB ERROR: open error");
        return false;
    }

    bool ok;
    QSqlQuery checkTableQuery("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Item'");
    checkTableQuery.next();
    QVariant val = checkTableQuery.value(0);
    qlonglong value = val.toULongLong(&ok);

    if( !ok || value != 1 )
    {
        qDebug("DB ERROR: scheme error");
        m_db.close();
        return false;
    }

    return true;
}

void DatabaseManager::close()
{
    if( m_db.isOpen() )
    {
        delete m_query;
        m_query = NULL;
        m_db.close();
    }
}

qulonglong DatabaseManager::irreversableItemCount()
{
    if( !m_db.isOpen() )
        return 0;

    QSqlQuery countQuery("select count(*) from Item where reversable='false'");
    countQuery.next();
    QVariant val = countQuery.value(0);
    bool ok;
    qulonglong valueCount = val.toULongLong(&ok);

    return ok ? valueCount : 0;
}

qulonglong DatabaseManager::reversableItemCount()
{
    if( !m_db.isOpen() )
        return 0;

    QSqlQuery countReversableQuery("select count(*) from Item where reversable='true'");
    countReversableQuery.next();
    QVariant val = countReversableQuery.value(0);
    bool ok;
    qulonglong valueReversable = val.toULongLong(&ok);

    return ok ? valueReversable : 0;
}

void DatabaseManager::itemsAtIndex(int idx, Item** item, Item** revItem)
{
    if( !m_query )
    {
        m_query = new QSqlQuery("select id,question,answer,example,reversable,timesAsked,"
                                "timesCorrect,timesAskedReversed,timesCorrectReversed from Item");
    }

    *item = NULL;
    *revItem = NULL;

    bool valid = m_query->seek(idx);

    if( !valid )
    {
        return;
    }

    QVariant id  = m_query->value(0);
    QVariant que = m_query->value(1);
    QVariant ans = m_query->value(2);
    QVariant exm = m_query->value(3);
    QVariant rev = m_query->value(4);
    QVariant ta  = m_query->value(5);
    QVariant tc  = m_query->value(6);
    QVariant tar = m_query->value(7);
    QVariant tcr = m_query->value(8);

    //TODO:add maybe some error checking here
    bool reversed = rev.toBool();

    *item = new Item(id.toInt(NULL), que.toString(), ans.toString(), exm.toString(), false, ta.toInt(NULL), tc.toInt(NULL) );

    if( reversed )
    {
        *revItem = new Item(id.toInt(NULL), que.toString(), ans.toString(), exm.toString(), true, tar.toInt(NULL), tcr.toInt(NULL) );
    }
}
