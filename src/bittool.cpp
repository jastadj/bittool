#include "bittool.hpp"
#include "math.h"
#include <vector>

BitTool::BitTool()
{

    initCurses();

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

}

void BitTool::bitfield()
{
    bool quit = false;
    std::vector<bool> bits;

    int ch = 0;
    int selection = 0;

    for(int i = 0; i < 16; i++) bits.push_back(false);

    while(!quit)
    {
        clear();

        //calculate value of bools
        int boolval = 0;
        for(int i = 0; i < int(bits.size()); i++)
        {
            if(bits[i]) boolval += pow(2, i);
        }
        printw("Dec value:%d\n", boolval);
        printw("Hex value:%x\n\n", boolval);

        for(int i = 0; i < int(bits.size()); i++)
        {
            if(selection == i) attron(A_REVERSE);

            printw("BIT %02d : %d\n", i, int(bits[i]));

            attroff(A_REVERSE);
        }
        printw("test : %d\n", ch);

        ch = getch();

        if(ch == 27) quit = true;
        else if(ch == 258) selection++;
        else if(ch == 259) selection--;
        else if(ch == 260 || ch == 261) bits[selection] = !bits[selection];


        //trim selection value
        if(selection >= int(bits.size())) selection = 0;
        else if(selection < 0) selection = int(bits.size()-1);

    }

}

void BitTool::mainLoop()
{
    bool quit = false;

    scrollok(stdscr, true);

    clear();

    while(!quit)
    {
        char buf[50];
        int bufval;
        printw("Enter hex value :");

        scanw("%x", &bufval);


        printw("You entered :%x (%d)\n", bufval, bufval);

        for(int i = 15; i >= 0; i--)
        {
            if( 0x01 & (bufval >> i) ) printw("%0d : 1\n", i);
            else printw("%0d : 0\n", i);
        }

        printw("\n");
    }


    getch();

    scrollok(stdscr, false);
}
