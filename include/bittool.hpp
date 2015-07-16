#ifndef CLASS_BITTOOL
#define CLASS_BITTOOL

#include "curses.h"

class BitTool
{
private:

    void initCurses();

    void mainLoop();
    void bitfield();

public:
    BitTool();
    ~BitTool();
};

#endif // CLASS_BITTOOL
