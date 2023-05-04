
#define DOCK_MENU_ITEMS_COUNT 3 // minus 1, first is 0
PROGMEM const char dockMenuNames[] = "Main \0CH Options\0CH Info\0Trigger";

PROGMEM const char on_off[] = "Off\0On";

void dockBar(TFT_eSPI &tft, Graph *graphs, unsigned long long graphDrawTime){
  uint16_t tx = 0, ty = 0;
  static unsigned long long nextTouch = 0;
  static uint16_t menu = 0, oldMenu = 0, draw = 1, drawConstUpdated = 1;
  
  if(tft.getTouch(&tx, &ty)){
    if(nextTouch < millis()){
      nextTouch = millis() + 200;
      tx = map(tx, 480, 0, 0, 480);

      strSelector(tft, 0, 8, 3, draw, tx, ty, menu, dockMenuNames, DOCK_MENU_ITEMS_COUNT);
      if(oldMenu != menu){
        oldMenu = menu;
        draw = true;
        tft.fillRect((GRAPH_HDIV_SIZE * 3)+1, GRAPH_VDIV_SIZE * 8, GRAPH_HDIV_SIZE * 12, GRAPH_VDIV_SIZE * 2, TFT_BLACK);
      }

      switch(menu){
        case 0:{ // main
          ;
        }break;
        case 1:{ // ch options
          static uint16_t ch = 0, oldCh = 0;
          nSelector(tft, 1, 9, 2, draw, tx, ty, ch, 0, 3);
          if(oldCh != ch){
            oldCh = ch;
            draw = true;
          }
          uint8_t on = graphs[ch].draw;
          strSelector(tft, 3, 8, 2, draw, tx, ty, on, on_off, 1);
          graphs[ch].draw = on;
          barSelector(tft, 5, 8, 4, draw, tx, ty, graphs[ch].scale, 0, 4);           // scale
          barSelector(tft, 9, 8, 6, draw, tx, ty, graphs[ch].vLargeScroll, 1, 15);    // large step scroll
          barSelector(tft, 3, 9, 12, draw, tx, ty, graphs[ch].vSmallScroll, -511, 511); // sensible scroll
          graphs[ch].vScroll = ((short)(graphs[ch].vLargeScroll << 9)-4095) + graphs[ch].vSmallScroll;
        }break;

        case 2:{ // ch info
          static uint16_t ch = 0, oldCh = 0;
          nSelector(tft, 1, 9, 2, draw, tx, ty, ch, 0, 3);
          if(oldCh != ch){
            oldCh = ch;
            draw = true;
          }
        }break;

        case 3:{ // trigger
          static uint16_t ch = 0, oldCh = 0, on = false;
          nSelector(tft, 1, 9, 2, draw, tx, ty, ch, 0, 3);
          if(oldCh != ch){
            oldCh = ch;
            for(int i = 0; i < ANALOG_CH_COUNT; i++) graphs[ch].trigger = (i == ch) && on;
            draw = true;
          }
          strSelector(tft, 3, 8, 2, draw, tx, ty, on, on_off, 1);
          graphs[ch].trigger = on;
          barSelector(tft, 3, 9, 12, draw, tx, ty, graphs[ch].triggerVal, 0, 4095);
        }break;

        default: break;
      }
    }

    drawConstUpdated = draw;
    draw = false;
  }

  //Constant Updates
  switch(menu){
    case 0:{ // main
      static unsigned long long lastReadTime;
      intViewer(tft, 3, 8, 3, drawConstUpdated, lastReadTime != graphDrawTime, graphDrawTime);
      lastReadTime = graphDrawTime;
    }break;

    case 1:{ // ch options
      ;
    }break;

    case 2:{ // ch info
      static unsigned long long lastReadTime;
      intViewer(tft, 3, 8, 3, drawConstUpdated, lastReadTime != graphs->readTime, graphs->readTime);
      lastReadTime = graphs->readTime;
    }break;

    default: break;
  }
  
  drawConstUpdated = false;
}
