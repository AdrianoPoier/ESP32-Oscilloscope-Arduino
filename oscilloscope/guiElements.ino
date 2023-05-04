
void baseGuiElement(TFT_eSPI &tft, uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye){
  tft.drawLine(xs+2, ys+15, xs+14, ys+3, TFT_YELLOW);
  tft.drawLine(xs+2, ys+16, xs+14, ys+28, TFT_YELLOW);
  tft.drawLine(xe-2, ys+15, xe-14, ys+3, TFT_YELLOW);
  tft.drawLine(xe-2, ys+16, xe-14, ys+28, TFT_YELLOW);
  unsigned short h = (xe - xs) - 29;
  tft.drawFastHLine(xs+15, ys+2, h, TFT_YELLOW);
  tft.drawFastHLine(xs+15, ys+29, h, TFT_YELLOW);
}

template <typename t>
bool nSelector(TFT_eSPI &tft, uint8_t px, uint8_t py, uint8_t s, bool draw, uint16_t tx, uint16_t ty, t &n, int16_t min, int16_t max){
  uint16_t xs = px * GRAPH_HDIV_SIZE, ys = py * GRAPH_VDIV_SIZE;
  uint16_t xe = xs + (GRAPH_HDIV_SIZE * s), ye = ys + GRAPH_VDIV_SIZE;
  bool update = false;
  if(draw){
    baseGuiElement(tft, xs, ys, xe, ye);
  }

  if((tx>xs) && (tx<xe) && (ty>ys) && (ty<ye)){ // touch detected
    if(tx > (((xe-xs)/2)+xs)){ // at right
      if(n < max){
        n++;
        update = true;
      }
    }else{ // at left
      if(n > min){
        n--;
        update = true;
      }
    }
  }

  if(draw || update){
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    tft.fillRect(xs+10, ys+8, (xe - xs) - 29, 16, TFT_BLACK);
    tft.drawNumber(n, xs+((xe-xs)/2), ys+8);
  }

  return update;
}

template <typename t>
bool barSelector(TFT_eSPI &tft, uint8_t px, uint8_t py, uint8_t s, bool draw, uint16_t tx, uint16_t ty, t &n, int16_t min, int16_t max){
  uint16_t xs = px * GRAPH_HDIV_SIZE, ys = py * GRAPH_VDIV_SIZE;
  uint16_t xe = xs + (GRAPH_HDIV_SIZE * s), ye = ys + GRAPH_VDIV_SIZE;
  uint16_t cxs = xs + 15, cxe = xe - 15;
  bool update = false;
  if(draw){
    baseGuiElement(tft, xs, ys, xe, ye);
  }

  if((tx>xs) && (tx<xe) && (ty>ys) && (ty<ye)){ // touch detected
    if(tx > cxe){ // at right
      if(n < max){
        n++;
        update = true;
      }
    }else if(tx >= cxs){ // at center
      n = map(tx, cxs, cxe, min, max);
      update = true;
    }else{ // at left
      if(n > min){
        n--;
        update = true;
      }
    }
  }

  if(draw || update){
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    uint16_t xmap = map(n, min, max, cxs, cxe);
    tft.fillRect(cxs, ys+3, (xmap - cxs), 25, TFT_DARKGREEN);
    tft.fillRect(xmap, ys+3, (cxe - xmap), 25, TFT_BLUE);
    tft.drawNumber(n, xs+((xe-xs)/2), ys+8);
  }

  return update;
}

template <typename t>
bool strSelector(TFT_eSPI &tft, uint8_t px, uint8_t py, uint8_t s, bool draw, uint16_t tx, uint16_t ty, t &n, const char *opts, int16_t max){
  uint16_t xs = px * GRAPH_HDIV_SIZE, ys = py * GRAPH_VDIV_SIZE;
  uint16_t xe = xs + (GRAPH_HDIV_SIZE * s), ye = ys + GRAPH_VDIV_SIZE;
  bool update = false;
  if(draw){
    baseGuiElement(tft, xs, ys, xe, ye);
  }

  if((tx>xs) && (tx<xe) && (ty>ys) && (ty<ye)){ // touch detected
    if(tx > (((xe-xs)/2)+xs)){ // at right
      if(n < max){
        n++;
        update = true;
      }
    }else{ // at left
      if(n > 0){
        n--;
        update = true;
      }
    }
  }

  if(draw || update){
    for(uint16_t i = 0; i < n; opts++) if(*opts == '\0') i++;
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    tft.fillRect(xs+10, ys+8, (xe-xs)-20, 16, TFT_BLACK);
    tft.drawString(opts, xs+((xe-xs)/2), ys+8);
  }

  return update;
}

bool barSelector(TFT_eSPI &tft, uint8_t px, uint8_t py, uint8_t s, bool draw, uint16_t tx, uint16_t ty, float &n, int16_t min, int16_t max){
  uint16_t xs = px * GRAPH_HDIV_SIZE, ys = py * GRAPH_VDIV_SIZE;
  uint16_t xe = xs + (GRAPH_HDIV_SIZE * s), ye = ys + GRAPH_VDIV_SIZE;
  uint16_t cxs = xs + 15, cxe = xe - 15;
  bool update = false;
  if(draw){
    baseGuiElement(tft, xs, ys, xe, ye);
  }

  if((tx>xs) && (tx<xe) && (ty>ys) && (ty<ye)){ // touch detected
    if((tx > cxe) && (tx >= cxs)){ // at center
      n = map(tx, cxs, cxe, min, max);
      update = true;
    }
  }

  if(draw || update){
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    uint16_t xmap = map(n, min, max, cxs, cxe);
    tft.fillRect(cxs, ys+3, (xmap - cxs), 25, TFT_DARKGREEN);
    tft.fillRect(xmap, ys+3, (cxe - xmap), 25, TFT_BLUE);
    tft.drawFloat(n, 2, xs+((xe-xs)/2), ys+8);
  }

  return update;
}

void intViewer(TFT_eSPI &tft, uint8_t px, uint8_t py, uint8_t s, bool draw, bool update, int n){
  uint16_t xs = px * GRAPH_HDIV_SIZE, ys = py * GRAPH_VDIV_SIZE;
  uint16_t xe = xs + (GRAPH_HDIV_SIZE * s), ye = ys + GRAPH_VDIV_SIZE;

  if(draw){
    tft.drawRect(xs+2, ys+2, xe-xs-4, 28, TFT_YELLOW);
  }

  if(draw || update){
    tft.setTextColor(TFT_WHITE);
    tft.setTextFont(2);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(1);
    tft.fillRect(xs+3, ys+8, (xe - xs) - 6, 16, TFT_BLACK);
    tft.drawNumber(n, xs+((xe-xs)/2), ys+8);
  }
}
