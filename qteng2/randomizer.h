#ifndef RANDOMIZER_H
#define RANDOMIZER_H

class Randomizer
{
public:
    static int   getInt( int min, int max );
    static int   getInt( int max );
    static float getFloat( float min, float max );
    static float getFloat( float max );
};

#endif // RANDOMIZER_H
