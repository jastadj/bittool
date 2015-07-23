#include "bittool.hpp"
#include "math.h"
#include "stdint.h"


BitTool::BitTool()
{

    initCurses();

    //init variables
    isSigned = false;

    //mainLoop();
    bitfield();
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

void BitTool::bitfield()
{
    bool quit = false;
    std::vector<bool> bits;

    int ch = 0;
    int selection = 0;

    for(int i = 0; i < 16; i++) bits.push_back(false);

    //init word size
    int wordsize = WS_2BYTE;

    while(!quit)
    {
        clear();

        //calculate value of bools
        int boolval = 0;
        for(int i = 0; i < int(bits.size()); i++)
        {
            if(bits[i]) boolval += pow(2, i);
        }
        mvprintw(0,0,"[d]ec value:");
        if(isSigned)
        {
            int tempval = boolval;

            if(bits.back())
            {
                //clear temp val for signed calc
                tempval = 0;

                //copy and invert all bits
                std::vector<bool> tempbits;

                for(int i = 0; i < int(bits.size()-1); i++)
                {
                    //copy inverted bit to temp list
                    tempbits.push_back(!bits[i]);

                    //if bit is high, add to value
                    if(tempbits.back()) tempval += pow(2,i);
                }

                //add one to temp value then invert sign
                tempval = (tempval + 1) * (-1);
            }

            printw("%d", tempval);
        }
        else printw("%d\n", boolval);

        //change hex formatting based on word size
        switch(wordsize)
        {
        case WS_NIBBLE:
            mvprintw(1,0,"[h]ex value:0x%x\n\n", boolval);
            break;
        case WS_1BYTE:
            mvprintw(1,0,"[h]ex value:0x%02x\n\n", boolval);
            break;
        case WS_2BYTE:
            mvprintw(1,0,"[h]ex value:0x%04x\n\n", boolval);
            break;
        default:
            break;
        }

        for(int i = 0; i < int(bits.size()); i++)
        {
            if(selection == i) attron(A_REVERSE);

            mvprintw(3+i, 0, "BIT %02d : %d\n", i, int(bits[i]));

            attroff(A_REVERSE);
        }
        mvprintw(20,0, "[c]lear bitfield");
        mvprintw(20,25, "[s]igned:");
        if(isSigned) printw("y");
        else printw("n");
        mvprintw(21,0, "[a]ll bits high");

        mvprintw(21,25,"[w]ord size:%d", int(bits.size()));

        mvprintw(22,0, "[i]nvert bits");

        //highlight ui components green
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0,1,"d");
        mvprintw(1,1,"h");
        mvprintw(20,1, "c");
        mvprintw(20,26,"s");
        mvprintw(21,1,"a");
        mvprintw(22,1,"i");
        mvprintw(21,26,"w");
        attroff(COLOR_PAIR(1) | A_BOLD);


        //debug
        mvprintw(24,0,"test : %d\n", ch);

        ch = getch();

        if(ch == 27) quit = true;
        else if(ch == 258) selection++;
        else if(ch == 259) selection--;
        //toggle bits high/low with left/right arrow keys
        else if(ch == 260 || ch == 261 || ch == 10) bits[selection] = !bits[selection];
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
            wordsize++;
            if(wordsize == WS_TOTAL) wordsize = WS_NIBBLE;

            switch(wordsize)
            {
            case WS_NIBBLE:
                bits.resize(4);
                break;
            case WS_1BYTE:
                bits.resize(8);
                break;
            case WS_2BYTE:
                bits.resize(16);
                break;
            default:
                break;
            }
        }


        //trim selection value
        if(selection >= int(bits.size())) selection = 0;
        else if(selection < 0) selection = int(bits.size()-1);

    }

}

void BitTool::setBitFieldFromDec(std::vector<bool> *bits, int val)
{
    std::vector<bool> templist;
    int tval = val;

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

void BitTool::setBitFieldFromHex(std::vector<bool> *bits, int val)
{
    if(bits == NULL) return;
}

void BitTool::clearBitField(std::vector<bool> *bits)
{
    for(int i = 0; i < int(bits->size()); i++) (*bits)[i] = false;
}
