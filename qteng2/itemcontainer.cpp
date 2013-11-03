#include "itemcontainer.h"
#include <QDebug>
#include "assert.h"
#include <QtAlgorithms>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDir>
#include "item.h"


bool defaultSortLessOperator( Item* i1, Item* i2 )
{
    double r1 = i1->GetRank();
    double r2 = i2->GetRank();
    return r1 < r2;
}

QString StringWithCorrectedQuotes( const QString& src )
{
    QString dst(src);
    dst.replace('\'', "''");
    return dst;
}

ItemContainer::ItemContainer()
{
    clear();
}

bool ItemContainer::load( const QString& filename )
{
    clear();

    if( !filename.length() )
    {
        qDebug("DB ERROR: empty filename");
        return false;
    }

    m_filename = filename;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( m_filename );

    // Open databasee
    if( !db.open() )
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
        db.close();
        return false;
    }

    //count how many items is in database
    QSqlQuery countQuery("select count(*) from Item");
    countQuery.next();
    val = countQuery.value(0);
    qlonglong valueCount = val.toULongLong(&ok);

    //and how many of them is reversable (these will be splitted to two separate items)
    QSqlQuery countReversableQuery("select count(*) from Item where reversable='true'");
    countReversableQuery.next();
    val = countReversableQuery.value(0);
    qlonglong valueReversable = val.toULongLong(&ok);

    m_questionsCount = valueCount + valueReversable;

    m_items.reserve(m_questionsCount);

    QSqlQuery selectQuery("select id,question,answer,example,reversable,timesAsked,"
                          "timesCorrect,timesAskedReversed,timesCorrectReversed from Item");
    while( selectQuery.next() )
    {
        QVariant id  = selectQuery.value(0);
        QVariant que = selectQuery.value(1);
        QVariant ans = selectQuery.value(2);
        QVariant exm = selectQuery.value(3);
        QVariant rev = selectQuery.value(4);
        QVariant ta  = selectQuery.value(5);
        QVariant tc  = selectQuery.value(6);
        QVariant tar = selectQuery.value(7);
        QVariant tcr = selectQuery.value(8);

        //TODO:add maybe some error checking here
        bool reversed = rev.toBool();

        Item* i = new Item(id.toInt(NULL), que.toString(), ans.toString(), exm.toString(),
                           false, ta.toInt(NULL), tc.toInt(NULL) );
        m_items.append(i);

        if( reversed )
        {
            Item* i = new Item(id.toInt(NULL), que.toString(), ans.toString(), exm.toString(),
                               true, tar.toInt(NULL), tcr.toInt(NULL) );
            m_items.append(i);
        }
    }
    db.close();

    qSort( m_items.begin(), m_items.end(), defaultSortLessOperator );

    return true;
}

bool ItemContainer::reload()
{
    return load( m_filename );
}

void ItemContainer::prepareNextQuestion()
{
    if( !m_items.size() )
    {
        m_currentItem = NULL;
        return;
    }

    m_currentItem = m_items.at(0);
    m_nextPrepared = true;
}

const Item* ItemContainer::getCurrentQuestion() const
{
    if( m_nextPrepared && m_currentItem )
    {
        m_currentItem->IncreaseAsked();
        m_asked++;
        m_nextPrepared = false;
    }

    return m_currentItem;
}

void ItemContainer::currentQuestionAnsweredProperly()
{
    m_answered++;
    assert( m_currentItem );
    m_currentItem->IncreaseAnswered();

    m_answeredItems.append( m_currentItem );
    m_items.pop_front();
}

void ItemContainer::currentQuestionAnsweredNotProperly()
{
    //TODO:
    //question will be asked again, so add it to some random place in list
//    int rand = Randomizer::getInt( m_items.size() - 1 );
//    m_items.insert( rand, m_currentItem );
    m_items.pop_front();
}

int ItemContainer::getNumberOfAskedQuestions() const
{
    return m_asked;
}

int ItemContainer::getNumberOfCorrectAnswers() const
{
    return m_answered;
}

int ItemContainer::getNumberOfQuestionsLeft() const
{
    return m_items.size();
}

int ItemContainer::getNumberOfQuestionsTotal() const
{
    return m_questionsCount;
}

void ItemContainer::clear()
{
    m_filename = QString();
    qDeleteAll( m_items );
    m_items.clear();
    m_answeredItems.clear();
    m_asked        = 0;
    m_answered     = 0;
    m_currentItem  = NULL;
    m_nextPrepared = false;
}
