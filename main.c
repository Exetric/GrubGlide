// Modules
#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>

// Sprites
#include "Assets/Worm.c"
#include "Assets/Ring.c"
#include "Assets/RingStruct.c"
#include "Assets/Numbers.c"

// Windows
#include "Assets/Fly.c"
#include "Assets/Score.c"

// Backgrounds
#include "Assets/Splash_data.c"
#include "Assets/Splash_map.c"
#include "Assets/Title_data.c"
#include "Assets/Title_map.c"
#include "Assets/Game_data.c"
#include "Assets/Game_map.c"
#include "Assets/Letters.c"

// Variables
UINT8 i;
UBYTE spritesize = 8;
UINT8 booster = 0;
UINT8 ringSpeed = 3;
UBYTE currentVel = 0;
UINT8 wormY = 78;
UBYTE playerAlive = 1;
UBYTE flyVel = 1;
UINT8 drop = 1;
UINT8 l;
UINT8 currentBest = 0;
UINT16 points = 0;

extern UINT16 highScore;
extern UBYTE save;

struct ringProp ringObj;

void moveRing(struct ringProp* ring, UINT8 x, UINT8 y) {
  move_sprite(12, x, y);
  move_sprite(13, x + (spritesize), y);
  move_sprite(14, x + (spritesize * 2), y);
  move_sprite(15, x, y + (spritesize));
  move_sprite(16, x + (spritesize), y + (spritesize));
  move_sprite(17, x + (spritesize * 2), y + (spritesize));
  move_sprite(18, x, y + (spritesize * 2));
  move_sprite(19, x + (spritesize), y + (spritesize * 2));
  move_sprite(20, x + (spritesize * 2), y + (spritesize * 2));
  move_sprite(21, x, y + (spritesize * 3));
  move_sprite(22, x + (spritesize), y + (spritesize * 3));
  move_sprite(23, x + (spritesize * 2), y + (spritesize * 3));
  move_sprite(24, x, y + (spritesize * 4));
  move_sprite(25, x + (spritesize), y + (spritesize * 4));
  move_sprite(26, x + (spritesize * 2), y + (spritesize * 4));
  move_sprite(27, x, y + (spritesize * 5));
  move_sprite(28, x + (spritesize), y + (spritesize * 5));
  move_sprite(29, x + (spritesize * 2), y + (spritesize * 5));
  move_sprite(30, x, y + (spritesize * 6));
  move_sprite(31, x + (spritesize), y + (spritesize * 6));
  move_sprite(32, x + (spritesize * 2), y + (spritesize * 6));
}

void performantdelay(UINT8 numloops) {
  UINT8 ii;
  for(ii = 0; ii < numloops; ii++) {
    wait_vbl_done();
  }
}

void fadeout() {
  for(i=0;i<4;i++) {
    switch(i){
      case 0:
        BGP_REG = 0xE4;
        break;
      case 1:
        BGP_REG = 0xF9;
        break;
      case 2:
        BGP_REG = 0xFE;
        break;
      case 3:
        BGP_REG = 0xFF;
        break;
    }
    performantdelay(5);
  }
}

void fadein() {
  for(i=0;i<3;i++) {
    switch(i){
      case 0:
        BGP_REG = 0xFE;
        break;
      case 1:
        BGP_REG = 0xF9;
        break;
      case 2:
        BGP_REG = 0xE4;
        break;
    }
    performantdelay(5);
  }
}

int firstDigit(int n)
{
  if(n >= 100) {
    n /= 100;
  } else {
    n = 0;
  }
  return n;
}

int lastDigit(int n)
{
  return (n % 10);
}

void main(){
  ENABLE_RAM_MBC1;
  if(save != 's') {
    highScore = 0;
    save = 's';
  }

  ringObj.x = 240;
  ringObj.y = 16 + rand() % (72 - 16);

  NR52_REG = 0x80;
  NR50_REG = 0x77;
  NR51_REG = 0xFF;

  // Splash Background
  set_bkg_data(0, 113, Splash_data);
  set_bkg_tiles(0, 0, 20, 18, Splash_map);

  // Splash Sound
  NR10_REG = 0x74;
  NR11_REG = 0x35;
  NR12_REG = 0x8F;
  NR13_REG = 0xC6;
  NR14_REG = 0x86;

  SHOW_BKG;
  DISPLAY_ON;

  delay(2500);
  fadeout();

  titleScreen:

  // Title Screen
  set_bkg_tiles(0, 0, 20, 18, Title_map);
  set_bkg_data(0, 240, Title_data);
  fadein();

  waitpad(J_A);

  NR10_REG = 0x00;
  NR11_REG = 0x01;
  NR12_REG = 0x43;
  NR13_REG = 0xD6;
  NR14_REG = 0x86;

  fadeout();

  set_sprite_data(0, 12, worm);
  set_sprite_data(12, 32, ring);
  set_sprite_data(33, 48, numbers);

  retry:

  set_bkg_data(145, 164, Letters);
  set_win_tiles(0, 0, 21, 3, Fly);

  move_win(1,120);

  set_bkg_tiles(0, 0, 32, 18, Game_map);
  set_bkg_data(0, 145, Game_data);

  // Draw Sprites

  set_sprite_tile(0, 0);
  set_sprite_tile(2, 1);
  set_sprite_tile(1, 2);
  set_sprite_tile(3, 3);

  for(l = 4; l <= 48; ++l) {
    set_sprite_tile(l, l);
  }

  move_sprite(0, 48, 78);
  move_sprite(1, 56, 78);
  move_sprite(3, 56, 86);
  move_sprite(2, 48, 86);

  move_sprite(33, 152, 24); // Third Digit
  move_sprite(34, 145, 24); // Second Digit
  move_sprite(35, 138, 24); // First Digit

  move_sprite(36, 30, 24); // Third Digit Hi
  move_sprite(37, 23, 24); // Second Digit Hi
  move_sprite(38, 16, 24); // First Digit Hi

  set_sprite_tile(34, 76);
  set_sprite_tile(35, 76);
  set_sprite_tile(37, 76);
  set_sprite_tile(38, 76);



  set_sprite_tile(36, 39+lastDigit(highScore));
  set_sprite_tile(37, 39+(highScore / 10) % 10);
  set_sprite_tile(38, 39+firstDigit(highScore));

  if(firstDigit(highScore) == 0) {
    set_sprite_tile(36, 76);
    set_sprite_tile(37, 39+lastDigit(highScore));
    set_sprite_tile(38, 39+(highScore / 10) % 10);
  }

  if((highScore / 10) % 10 == 0 && highScore < 10) {
    set_sprite_tile(36, 76);
    set_sprite_tile(37, 76);
    set_sprite_tile(38, 39+lastDigit(highScore));
  }



  fadein();

  SHOW_SPRITES;
  SHOW_WIN;

  waitpad(J_A);

  HIDE_WIN;

  // Update

  while(1) {
    set_sprite_tile(0, 0);
    set_sprite_tile(2, 1);
    set_sprite_tile(1, 2);
    set_sprite_tile(3, 3);

    drop = 1;

    if(points > highScore) {
      currentBest = 1;
    }

    if(currentBest == 1) {
      highScore = points;
    }

    // Press A
    switch(joypad()) {
      case J_A:
        currentVel -= flyVel;
        drop=0;
        break;
    }

    currentVel += drop;

    if(currentVel == 3 && playerAlive == 1) {
      currentVel = 2;
      set_sprite_tile(0, 4);
      set_sprite_tile(1, 6);
      set_sprite_tile(2, 5);
      set_sprite_tile(3, 7);
    }

    if(currentVel == -3 && playerAlive == 1) {
      currentVel = -2;
      set_sprite_tile(0, 8);
      set_sprite_tile(1, 10);
      set_sprite_tile(2, 9);
      set_sprite_tile(3, 11);
    }

    if(wormY > 144 + 16 && wormY < 144 + 64) {
      NR41_REG = 0x3F;
      NR42_REG = 0xF1;
      NR43_REG = 0x40;
      NR44_REG = 0x80;
      delay(500);

      HIDE_SPRITES;

      if(currentBest == 1) {
        set_win_tiles(0, 0, 21, 3, Score);
        SHOW_WIN;
        NR10_REG = 0x55;
        NR11_REG = 0x17;
        NR12_REG = 0xF3;
        NR13_REG = 0x7C;
        NR14_REG = 0x86;

        delay(1500);
      }

      HIDE_WIN;

      fadeout();

      currentVel = 0;
      wormY = 78;
      playerAlive = 1;
      flyVel = 1;
      move_bkg(0,0);
      ringObj.x = 180;
      moveRing(&ringObj, ringObj.x, ringObj.y);
      points = 0;
      booster = 0;
      ringSpeed = 3;
      currentBest = 0;

      goto retry;
    }

    if(wormY > 144 + 32 && playerAlive == 1) {
      playerAlive = 0;

      NR10_REG = 0x5D;
      NR11_REG = 0x85;
      NR12_REG = 0x55;
      NR13_REG = 0xED;
      NR14_REG = 0x86;
    }

    ringObj.x -= ringSpeed;
    scroll_bkg(ringSpeed/2,0);

    scroll_sprite(0,0,currentVel);
    scroll_sprite(1,0,currentVel);
    scroll_sprite(2,0,currentVel);
    scroll_sprite(3,0,currentVel);

    moveRing(&ringObj, ringObj.x, ringObj.y);
    wormY += currentVel;

    if(ringObj.x < 49 && ringObj.x > 20) {
      if(wormY > ringObj.y - 8 && (wormY+16) < (ringObj.y+64)) {

        ringObj.x = 240;
        ringObj.y = ((rand()%((35+1)-19) + 19)*2)+16;
        moveRing(&ringObj, ringObj.x, ringObj.y);

        if(points < 999) {
          points++;
        }

        if(points <= 150) {
          booster++;
        }

        set_sprite_tile(33, 39+lastDigit(points));
        set_sprite_tile(34, 39+(points / 10) % 10);
        set_sprite_tile(35, 39+firstDigit(points));

        if(firstDigit(points) == 0) {
          set_sprite_tile(35, 76);
        }

        if((points / 10) % 10 == 0 && points < 10) {
          set_sprite_tile(34, 76);
        }

        if(booster == 50 && points <= 150) {
          NR10_REG = 0x67;
          NR11_REG = 0x01;
          NR12_REG = 0xC2;
          NR13_REG = 0x38;
          NR14_REG = 0x87;

          booster = 0;
          ringSpeed++;
        } else {
          NR10_REG = 0x00;
          NR11_REG = 0xC1;
          NR12_REG = 0x72;
          NR13_REG = 0x38;
          NR14_REG = 0x87;
        }

      }
    }

    if(ringObj.x < 19 && playerAlive==1) {
      playerAlive = 0;

      NR10_REG = 0x5D;
      NR11_REG = 0x85;
      NR12_REG = 0x55;
      NR13_REG = 0xED;
      NR14_REG = 0x86;
    }
    if(playerAlive==0) {
      flyVel=0;
      currentVel=3;

      set_sprite_tile(0, 4);
      set_sprite_tile(1, 6);
      set_sprite_tile(2, 5);
      set_sprite_tile(3, 7);
    }

    performantdelay(1);
  }
  DISABLE_RAM_MBC1;
}
