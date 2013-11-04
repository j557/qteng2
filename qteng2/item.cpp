#include "item.h"
#include "randomizer.h"

Item::Item( int id,
            const QString& question,
            const QString& answer,
            const QString& example,
            bool reversed,
            unsigned int asked,
            unsigned int answered )
: m_id( id )
, m_asked( asked )
, m_answered( answered )
, m_question( question )
, m_answer( answer )
, m_example( example )
, m_reversed( reversed )
, m_rank( -100 )
{
    if( m_asked < 5 )
    {
        m_rank = Randomizer::getFloat( -2.f, -1.f );
    }
    else
    {
        m_rank = (float)m_answered / (float)m_asked;
    }
}

float Item::GetRank() const
{
	return m_rank;
}

bool Compare(const Item* i1, const Item* i2)
{
	return i1->GetRank() < i2->GetRank();
}

bool Item::CompareDebug(const Item* i1, const Item* i2)
{
    return ( ( i1->m_question ).compare( i2->m_question ) < 0 );
}
