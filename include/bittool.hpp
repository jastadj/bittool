#ifndef CLASS_BITTOOL
#define CLASS_BITTOOL

#include "curses.h"
#include <vector>

enum eWORDSIZE {WS_NIBBLE, WS_1BYTE, WS_2BYTE, WS_TOTAL};

class BitTool
{
private:

    void initCurses();

    //main bit tool
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
