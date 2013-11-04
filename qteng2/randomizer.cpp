#include "randomizer.h"
#include <QTime>

static bool s_seedGenerated = false;

void S_GenerateSeed()
{
    if( !s_seedGenerated )
    {
        QTime time = QTime::currentTime();
        qsrand( (uint)time.msec() );
        s_seedGenerated = true;
    }
}

int Randomizer::getInt( int min, int max )
{
    S_GenerateSeed();

    // Random number between low and high
    return qrand() % ( (max + 1) - min ) + min;
}

int Randomizer::getInt( int max )
{
    return Randomizer::getInt( 0, max );
}

float Randomizer::getFloat( float min, float max )
{
    S_GenerateSeed();

    float v = (float)qrand() / (float)RAND_MAX;
    v *= ( max - min );
    v += min;
    return v;
}

float Randomizer::getFloat( float max )
{
    return Randomizer::getFloat( 0, max );
}
