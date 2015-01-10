/**
* conio4linux (Conio.h for linux)
* Copyright (C) 2009 by Carlos J. Pinto B.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
* @author Carlos J. Pinto B. <pintocar83@gmail.com>
* @date 2009-11-19
* @version 0.2
*/

/*
List funtions
=============
    cprintf
    cscanf
    gotoxy          
    clrscr          
    textcolor       
    textbackground  
    wherex         
    wherey        
    getch          
    getche   
    ungetch
    kbhit          
    putch           
    putchar        
    cputs         
    clreol         
    insline (not implemented)       
    delline (not implemented)       
    cgets   (not implemented)       
    getpass (not implemented)        
    gettext (not implemented)
    _cprintf        
    _cscanf         
    _cputs          
    _getche         
    _kbhit          
    _putch          
    _ungetch
    
Instalation for linux
=====================
    Copy file conio.h to /usr/include/
    Run as root user:
      cp conio.h /usr/include/

Example program
===============
//File name: main.cpp
//Compilation: gcc main.cpp -o program_conio -lstdc++ -lncurses
//Test program: ./program_conio
#include<conio.h>
int main(int argc, char *argv[]){
  textbackground(BLUE);
  clrscr();  
  textcolor(WHITE);
  gotoxy(30,5);
  cprintf("Hello World WHITE");
  textcolor(YELLOW);
  gotoxy(30,6);
  cprintf("Hello World YELLOW");    
  for(int i=0;i<16;i++){
    textcolor(i);
    gotoxy(1,2+i);
    cprintf("Hello World"); 
    }  
  getch();
  return 0;
  }

I need help for
===============
  *Document funtions.
  *Implement funtions not implemented.
  *Add funtions new.
  *Add values news in getche() funtion. (keys new). Example: values 256 and 257 in the switch.
  *Report bugs.
  
  For any case: 
  Send a mail to pintocar83@gmail.com with the modifications and information: Email, Autor and changes.
  Thanks!!!

Contributions
=============




*/

#ifndef __CONIO4LINUX_H
#define __CONIO4LINUX_H

#include <ncurses.h>

#define cprintf printw
#define cscanf scanw

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15
#define BLINK        128

class __CONIO_H{
  private:
    short matrix_pair[8][8];
    short now_pair;
    int colorNCurses(int color){
      switch(color){
        case BLACK:             return COLOR_BLACK;
        case BLUE:              return COLOR_BLUE;
        case CYAN:              return COLOR_CYAN;
        case RED:               return COLOR_RED;
        case MAGENTA:           return COLOR_MAGENTA;
        case BROWN:             return COLOR_YELLOW;
        case LIGHTGRAY:         return COLOR_WHITE;
        case DARKGRAY:          return COLOR_BLACK;
        case LIGHTBLUE:         return COLOR_BLUE;
        case LIGHTGREEN:        return COLOR_GREEN;
        case LIGHTCYAN:         return COLOR_CYAN;
        case LIGHTRED:          return COLOR_RED;
        case LIGHTMAGENTA:      return COLOR_MAGENTA;
        case YELLOW:            return COLOR_YELLOW;
        case WHITE:             return COLOR_WHITE;
        }
      return color;
      }
  public:    
    __CONIO_H(){
      initscr();
      keypad(stdscr,TRUE);
      scrollok(stdscr,TRUE);
      start_color();     
      for(short i=0,n=1;i<8;i++)//N background
        for(short j=0;j<8;j++,n++){//N textcolor
          init_pair(n,j,i);
          matrix_pair[i][j]=n;
          }
      now_pair=8;
      attron(COLOR_PAIR(now_pair));      
      }
    ~__CONIO_H(){
      endwin();
      }
      
    void textbackground(short color){
      short now_bgc,now_tc;
      pair_content(now_pair,&now_tc,&now_bgc);
      now_pair=matrix_pair[colorNCurses(color)%8][now_tc];
      attron(COLOR_PAIR(now_pair));
      }
      
    void textcolor(short color){
      short now_bgc,now_tc;
      pair_content(now_pair,&now_tc,&now_bgc);     
      now_pair=matrix_pair[now_bgc][colorNCurses(color)%8];
      attron(COLOR_PAIR(now_pair));
      color>=8?attr_on(A_BOLD,NULL):attr_off(A_BOLD,NULL);  
      }
    
    void gotoxy(int x,int y){
      move(y-1,x-1);  
      }

    void clrscr(){
      bkgd(COLOR_PAIR(now_pair));
      clear();
      }
    
    int wherex(){
      int x,y;
      getyx(stdscr,y,x);
      return x+1;
      }
      
    int wherey(){
      int x,y;
      getyx(stdscr,y,x);
      return y+1;
      }
      
    int _getche(){//return getch();
      static int specialKey=-1;
      int key;
      if(specialKey>0){
        key=specialKey;
        specialKey=-1;
        return key;
        }
      key=getch();
      if(key>255){//Parsen DOS keys specials
        switch(key){
          //case 256: specialKey=;break;?
          //case 257: specialKey=;break;?
          case 258: specialKey=80;break;//down
          case 259: specialKey=72;break;//up
          case 260: specialKey=75;break;//left
          case 261: specialKey=77;break;//right
          case 262: specialKey=71;break;//home
          //case 263: specialKey=;break;?
          //case 264: specialKey=;break;?
          case 265:
          case 266:
          case 267:
          case 268:
          case 269:
          case 270:
          case 271:
          case 272:
          case 273:
          case 274: specialKey=key-206;return 0;//F1 to F10                    
          case 275: specialKey=133;break;//F11
          case 276: specialKey=134;break;//F12
          //...?
          case 330: specialKey=83;break;//del
          case 331: specialKey=82;break;//ins
	  //...?
          case 338: specialKey=81;break;//pg down
          case 339: specialKey=73;break;//pg up
	  //...?
          case 360: specialKey=79;break;//end
          default:  specialKey = key;return 0;
          }         
        return 224;
        }
      return key;
      }
      
    int _getch(){
      noecho();
      int c=_getche();
      echo();
      return c;
      }
    
    int kbhit(){
      int key;
      timeout(0);
      key=getch();
      nodelay(stdscr,FALSE);
      if(key == ERR) return 0;
      ungetch(key);
      return 1;
      }
      
    int putch(const char c){
      echochar(c);
      return (int)c;
      }
      
    int cputs(const char*str){
      cprintf(str);
      return 0;
      }
      
    void clreol(){
      clrtoeol();
      }
      
    void insline(){
      
      }
      
    void delline(){

      }
      
    char*cgets(char*){
      
      }
      
    char*getpass(const char*prompt){
      
      }
      
    int gettext(int l,int t,int r,int b,void*destination){
      
      }
      
};//___CONIO_H;

#define gotoxy          ___CONIO_H.gotoxy
#define clrscr          ___CONIO_H.clrscr
#define textcolor       ___CONIO_H.textcolor
#define textbackground  ___CONIO_H.textbackground
#define wherex          ___CONIO_H.wherex
#define wherey          ___CONIO_H.wherey
#undef getch
#define getch           ___CONIO_H._getch
#define getche          ___CONIO_H._getche
#define kbhit           ___CONIO_H.kbhit
#define putch           ___CONIO_H.putch
#define putchar         ___CONIO_H.putch
#define cputs           ___CONIO_H.cputs
#define clreol          ___CONIO_H.clreol
#define insline         ___CONIO_H.insline
#define delline         ___CONIO_H.delline
#define cgets           ___CONIO_H.cgets
#define getpass         ___CONIO_H.getpass
#define gettext         ___CONIO_H.gettext


//DEV C++ only have the next funtions (version: 4.9.9.2)
#define _cprintf        cprintf
#define _cscanf         cscanf
#define _cputs          cputs
#define _getche         getche
#define _kbhit          kbhit
#define _putch          putch
#define _ungetch        ungetch



#endif
