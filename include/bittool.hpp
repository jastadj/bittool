#ifndef CLASS_BITTOOL
#define CLASS_BITTOOL

#include "curses.h"
#include <vector>

enum _WORDSIZE {
                WS_NIBBLE = 4,
                WS_1BYTE = 8,
                WS_2BYTE = 16,
                WS_4BYTE = 32
                };

enum _PROTOCOL {P_NONE, P_STA, P_TOTAL};

class BitTool
{
private:

    void initCurses();

    //main bit tool
    void mainLoop();

    //draw
    void drawMenu();
    void drawProtocolInfo();

    //handle input
    int selection;
    int handleInput(int ch);

    //bit data
    long int dataval;
    std::vector<bool> bits;
    bool isSigned;

    //options
    int protocolMode;

    //bitfield operations
    void setBitFieldFromDec(std::vector<bool> *bits, int val);
    void setBitFieldFromHex(std::vector<bool> *bits, int val);
    void clearBitField(std::vector<bool> *bits);
    int getDecFromBitfield(std::vector<bool> *bits);


public:
    BitTool();
    ~BitTool();
};

#endif // CLASS_BITTOOL
