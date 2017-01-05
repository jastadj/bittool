#include "bittool.hpp"
#include "math.h"
#include "stdint.h"
#include <sstream>
#include <iomanip>

BitTool::BitTool()
{

    initCurses();

    //init variables
    isSigned = false;
    protocolMode = P_STA;
    selection = 0;
    selection_start = -1;
    dataval = 0;
    //dataval_unsigned = 0;

    //init bitfield
    for(int i = 0; i < WS_2BYTE; i++) bits.push_back(false);

    //testing
    //setBitFieldFromDec(&bits, 0xca8efc51);

    //start main loop
    mainLoop();
}

BitTool::~BitTool()
{

}

void BitTool::initCurses()
{
    initscr();
    keypad(stdscr, true);
    start_color();

    //init colors
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

}

void BitTool::mainLoop()
{
    bool quit = false;

    //store key input value
    int ch = 0;

    while(!quit)
    {
        clear();

        //get data value for signed and unsigned
        dataval = getDecFromBitfield(&bits);
        //dataval_unsigned = dataval;

        //draw
        drawMenu();
        drawProtocolInfo();

        //debug
        mvprintw(24,0,"test : %d\n", ch);

        //get input
        ch = getch();

        //process input
        if(handleInput(ch) == -1) quit = true;

    }

}

void BitTool::drawMenu()
{

    mvprintw(0,0,"[d]ec value:");
    mvprintw(1,0,"[h]ex value:0x");
    int wordsize = int(bits.size());

    // if data is signed
    if(isSigned)
    {
        if(wordsize == WS_1BYTE) mvprintw(0, 12,"%zd", int8_t(dataval) );
        else if(wordsize == WS_2BYTE) mvprintw(0, 12,"%zd", int16_t(dataval) );
        else if(wordsize == WS_4BYTE) mvprintw(0, 12,"%zd", int32_t(dataval) );
        else mvprintw(0, 12,"%zd", int8_t(dataval) );
        move(1, 14);
        switch(wordsize)
        {
        case WS_NIBBLE:
            printw("%x\n\n", uint8_t(dataval)&0x00ff );
            break;
        case WS_1BYTE:
            printw("%02x\n\n", uint8_t(dataval) );
            break;
        case WS_2BYTE:
            printw("%04x\n\n", uint16_t(dataval) );
            break;
        case WS_4BYTE:
            printw("%08x\n\n", uint32_t(dataval) );
            break;
        default:
            break;
        }
    }
    //else data is unsigned
    else
    {
        mvprintw(0,12,"%zu", uint32_t(dataval) );
        move(1, 14);
        switch(wordsize)
        {
        case WS_NIBBLE:
            printw("%x\n\n", uint8_t(dataval) );
            break;
        case WS_1BYTE:
            printw("%02x\n\n", uint8_t(dataval) );
            break;
        case WS_2BYTE:
            printw("%04x\n\n", uint16_t(dataval) );
            break;
        case WS_4BYTE:
            printw("%08x\n\n", uint32_t(dataval) );
            break;
        default:
            break;
        }
    }

    //change hex formatting based on word size



    for(int i = 0; i < int(bits.size()); i++)
    {
        if(selection == i) attron(A_REVERSE);

        mvprintw(3 + i - floor((i/16))*16, 12*floor((i/16)) , "BIT %02d : %d\n", i, int(bits[i]));

        attroff(A_REVERSE);
    }
    mvprintw(21,0, "[c]lear bitfield");
    mvprintw(21,25, "[s]igned:");
    if(isSigned) printw("y");
    else printw("n");
    mvprintw(21, 50, "[p]rotocol mode:");
    switch(protocolMode)
    {
    case P_NONE:
        printw("None");
        break;
    case P_STA:
        printw("Serial Type A");
        break;
    case P_1553:
        printw("1553");
        break;
    default:
        printw("Error");
        break;
    }

    mvprintw(22,0, "[a]ll bits high");
    mvprintw(22,25,"[w]ord size:%d", int(bits.size()));

    mvprintw(23,0, "[i]nvert bits");

    //highlight ui components green
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(0,1,"d");
    mvprintw(1,1,"h");
    mvprintw(21,1, "c");
    mvprintw(21,26,"s");
    mvprintw(21,51,"p");
    mvprintw(22,1,"a");
    mvprintw(23,1,"i");
    mvprintw(22,26,"w");
    attroff(COLOR_PAIR(1) | A_BOLD);
}

int BitTool::handleInput(int ch)
{

    int wordsize = int(bits.size());

    if(ch == 27) return -1;
    else if(ch == 258)
    {
        selection_start = -1;
        selection++;
    }
    else if(ch == 259)
    {
        selection_start = -1;
        selection--;
    }
    //if possible, shift to adjacent bit
    //shift to the left
    else if(ch == 260)
    {
        selection_start = -1;
        selection -= 16;
    }
    //shift to the right
    else if(ch == 261)
    {
        selection_start = -1;
        selection += 16;
    }
    else if(ch == 480)
    {
        if(selection_start == -1) selection_start = selection;
        selection--;
    }
    else if(ch == 481)
    {
        if(selection_start == -1) selection_start = selection;
        selection ++;
    }
    //toggle bit with space bar or enter key
    else if(ch == 32 || ch == 10)
    {
        bits[selection] = !bits[selection];
    }
    //clear bit field 'c'
    else if(ch == 99)
    {
        clearBitField(&bits);
    }
    //set decimal value 'd'
    else if(ch == 100)
    {
        int newval = 0;
        mvprintw(12,30,"Enter decimal value:");
        scanw("%d", &newval);

        if(newval > pow(2, bits.size()) )
        {
            mvprintw(14,30, "Value is too large!");
            getch();
        }
        else setBitFieldFromDec(&bits, newval);
    }
    //set hex value 'h'
    else if(ch == 104)
    {
        int newval = 0;
        mvprintw(12,30,"Enter hex value:");
        scanw("%x", &newval);

        if(newval > pow(2, bits.size()) )
        {
            mvprintw(14,30, "Value is too large!");
            getch();
        }
        else setBitFieldFromDec(&bits, newval);

    }
    //invert bits 'i'
    else if(ch == 105)
    {
        for(int i = 0; i < int(bits.size()); i++) bits[i] = !bits[i];

    }
    //all bits set high 'a'
    else if(ch == 97)
    {
        for(int i = 0; i < int(bits.size()); i++) bits[i] = true;
    }
    //toggle signed/unsigned 's'
    else if(ch == 115)
    {
        isSigned = !isSigned;
    }
    else if(ch == 119)
    {
        selection_start = -1;
        selection = 0;
        switch(wordsize)
        {
        case WS_NIBBLE:
            wordsize = WS_1BYTE;
            bits.resize(WS_1BYTE);
            break;
        case WS_1BYTE:
            wordsize = WS_2BYTE;
            bits.resize(WS_2BYTE);
            break;
        case WS_2BYTE:
            wordsize = WS_4BYTE;
            bits.resize(WS_4BYTE);
            break;
        case WS_4BYTE:
            wordsize = WS_NIBBLE;
            bits.resize(WS_NIBBLE);
        default:
            break;
        }
    }
    // 'p' is pressed, cycle protocol modes
    else if(ch == 112)
    {
        protocolMode++;
        if(protocolMode >= P_TOTAL) protocolMode = 0;
    }


    //trim selection value
    if(selection >= int(bits.size()))
    {
        selection -= 16;
    }
    else if(selection < 0)
    {
        selection += 16;
    }

    return 0;
}

void BitTool::setBitFieldFromDec(std::vector<bool> *bits, uint32_t val)
{
    std::vector<bool> templist;
    uint32_t tval = val;

    //if bit field list is null
    if(bits == NULL) return;

    //if value is larger than container
    if(val > pow(2, bits->size()) ) return;

    //calculate binary value
    while(tval != 0)
    {
        if(tval%2) templist.push_back(true);
        else templist.push_back(false);

        tval = tval/2;
    }

    //clear bits list
    clearBitField(bits);

    for(int i = 0; i < int(templist.size()); i++)
    {
        (*bits)[i] = templist[i];
    }
}

void BitTool::clearBitField(std::vector<bool> *bits)
{
    for(int i = 0; i < int(bits->size()); i++) (*bits)[i] = false;
}

uint32_t BitTool::getDecFromBitfield(std::vector<bool> *bits)
{
    uint32_t boolval = 0;

    for(int i = 0; i < int(bits->size()); i++)
    {
        if((*bits)[i]) boolval += pow(2, i);
    }

    return boolval;
}

void BitTool::drawProtocolInfo()
{
    const static double resolution_1553_geo = pow(2, -31);

    if(protocolMode == P_NONE) return;
    else if(protocolMode == P_STA)
    {
        mvprintw(5,55, "Serial Type A");
        if( int(bits.size()) == WS_2BYTE)
        {
            mvprintw(7,55, "16b angle: %f", getAngle16(dataval));
        }
        else if( int(bits.size()) == WS_4BYTE)
        {
            mvprintw(7,55, "32b angle: %f", getAngle32(dataval));
        }

    }
    else if(protocolMode == P_1553)
    {
        mvprintw(5,55, "1553");
        if( int(bits.size()) == WS_4BYTE)
        {
            //geo coord
            mvprintw(7,55, "Lat/Lon: %s", degToGeoString(dataval*resolution_1553_geo*180).c_str());

            //time
            long int seconds = (dataval / 1000);
            long int minutes = seconds/60;
            long int hours = minutes/60;
            std::stringstream mytime;
            mytime << hours << ":";
            minutes = minutes - (hours*60);
            if( minutes < 10) mytime << "0";
            mytime << minutes << ":";
            seconds = seconds - (minutes*60) - (hours*60*60);
            if(seconds < 10) mytime << "0";
            mytime << seconds;
            mvprintw(8,55, "Time  %s", mytime.str().c_str());
        }
        else if(int(bits.size()) == WS_2BYTE)
        {
            mvprintw(7,55, "16b angle: %f", getAngle16(dataval));
            mvprintw(8,55+1, "Altitude: %dft", dataval -1000);
        }

    }
}

std::string BitTool::degToGeoString(float deg)
{
    std::stringstream geoss;
    geoss << floor(deg) << char(248) << " ";

    float gclock = (fabs(deg) - fabs(floor(deg)))*60;
    int hours = floor(gclock);
    float minutes = (fabs(gclock) - fabs(floor(gclock))) *60;

    geoss << hours << "' " << std::fixed << std::setprecision(2) << minutes << "\"";


    return geoss.str();
}

float BitTool::getAngle16(int val)
{
    //else return
    return float( val*360.f / 0xffff);
}

float BitTool::getAngle32(int val)
{
    return float( val*360.f / 0xffffffff);
}
