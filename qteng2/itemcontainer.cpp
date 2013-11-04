#include "itemcontainer.h"
#include <QDebug>
#include "assert.h"
#include <QtAlgorithms>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QDir>
#include "item.h"
#include "databasemanager.h"

bool defaultSortLessOperator( Item* i1, Item* i2 )
{
    double r1 = i1->GetRank();
    double r2 = i2->GetRank();
    return r1 < r2;
}

ItemContainer::ItemContainer()
{
    clear();
}

ItemContainer::~ItemContainer()
{
    clear();
}

bool ItemContainer::create( const QString& filename )
{
    clear();

    if( !DatabaseManager::getInstance().create( filename ) )
    {
        return false;
    }
    m_filename = filename;

    return true;
}

bool ItemContainer::load( const QString& filename )
{
    clear();

    DatabaseManager& dbMan = DatabaseManager::getInstance();
    if( !dbMan.open( filename ) )
    {
        return false;
    }
    m_filename = filename;

    m_questionsCount = dbMan.irreversableItemCount() +  2 * dbMan.reversableItemCount();

    const int ITEMS_COUNT = dbMan.irreversableItemCount() + dbMan.reversableItemCount();

    for( int i = 0; i < ITEMS_COUNT; ++i )
    {
        Item* item = NULL;
        Item* revItem = NULL;
        dbMan.itemsAtIndex(i, &item, &revItem);
        if( item )
        {
            m_items.append( item );
        }
        if( revItem )
        {
            m_items.append( revItem );
        }
    }

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
    DatabaseManager::getInstance().close();

    m_filename = QString();
    qDeleteAll( m_items );
    m_items.clear();
    m_answeredItems.clear();
    m_asked        = 0;
    m_answered     = 0;
    m_currentItem  = NULL;
    m_nextPrepared = false;
}
