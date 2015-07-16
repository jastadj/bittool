#include "bittool.hpp"

BitTool::BitTool()
{

    initCurses();

    mainLoop();
}

BitTool::~BitTool()
{

}

void BitTool::initCurses()
{
    initscr();

    scrollok(stdscr, true);
}

void BitTool::mainLoop()
{
    bool quit = false;

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
}
