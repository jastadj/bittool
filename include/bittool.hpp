#ifndef CLASS_BITTOOL
#define CLASS_BITTOOL

#include "curses.h"
#include <vector>

class BitTool
{
private:

    void initCurses();

    void mainLoop();
    void bitfield();

    //bit variables
    bool isSigned;


    //bitfield operations
    void setBitFieldFromDec(std::vector<bool> *bits, int val);
    void setBitFieldFromHex(std::vector<bool> *bits, int val);
    void clearBitField(std::vector<bool> *bits);

public:
    BitTool();
    ~BitTool();
};

#endif // CLASS_BITTOOL
