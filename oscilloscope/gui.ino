void gui_task(void *pvParameters){
  // &&& task global space
  Graph *graphs = {static_cast<Graph*>(pvParameters)};
  TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
  unsigned short graphColor[] = {TFT_YELLOW, TFT_GREEN, TFT_RED, TFT_BLUE, TFT_DARKGREEN, TFT_CYAN, TFT_MAGENTA, TFT_WHITE};
  unsigned short lastGraphPix[GRAPH_CH_COUNT]; // save last pixel for graphs V line draw
  unsigned long long graphDrawTime;
  
  { // &&& setup space
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    //tft.fillRect(65, 65, 255, 127, TFT_BLACK);
    //tft.drawRect(64, 64, 256, 128, TFT_WHITE);
    for(byte gn = 0; gn < GRAPH_CH_COUNT; gn++){
      if(gn < ANALOG_CH_COUNT) graphs[gn].draw = true;
      else graphs[gn].draw = false;
      graphs[gn].doWrite = true;
      graphs[gn].trigger = false;
      graphs[gn].triggerVal = 0;
      graphs[gn].vSmallScroll = 0;
      graphs[gn].vLargeScroll = 8;
      graphs[gn].vScroll = 0;
      graphs[gn].scale = 4;
    }
  }

  while(1){ // loop space
    vTaskDelay(1);
    //unsigned long long startTime = micros();
    
    dockBar(tft, graphs, graphDrawTime);

    // Process Graphs
    for(byte gn = 0; gn < ANALOG_CH_COUNT; gn++){
      lastGraphPix[gn] = (GRAPH_ENDY - GRAPH_STARTY) / 2;
      graphs[gn].div = 0x0FFF >> graphs[gn].scale;
    }
    byte chCount = 0;
    for(byte gn = 0; gn < GRAPH_CH_COUNT; gn++){ // check unfinished graphs
      chCount += graphs[gn].draw && graphs[gn].doWrite;
    }
    
    // Draw Graphs On Screen
    if(chCount == 0){ // do not execute any if graph didnt finished
      graphDrawTime = micros(); //store start time here to math later
      for(unsigned short x = GRAPH_STARTX; x < GRAPH_ENDX; x++){ // loop through the graph columns
        // background
        if((x - GRAPH_STARTX) % GRAPH_VDIV_SIZE){ // anything diferent than 0 is not a V div line
          tft.drawFastVLine(x, GRAPH_STARTY, GRAPH_ENDY - GRAPH_STARTY, TFT_BLACK); // paint it black (I was listening to the Violet Orlandi cover when commented this LOL)
          for(unsigned short y = GRAPH_STARTY; y < GRAPH_ENDY; y += GRAPH_HDIV_SIZE) tft.drawPixel(x, y, TFT_DARKGREY); // draw H line's pixels
        }
        else{ // when on a V line
          tft.drawFastVLine(x, GRAPH_STARTY, GRAPH_ENDY - GRAPH_STARTY, TFT_DARKGREY); // paint it
        }
        //plots
        for(byte gn = 0; gn < GRAPH_CH_COUNT; gn++){ // loop to draw graph dots
          if(graphs[gn].draw){ // if the graph is marked to be displayed
            //unsigned short graphPix = ((~(graphs[gn].buffer[x] >> 4)) & 0x00FF) + GRAPH_STARTY;
            unsigned short graphPix = (~((graphs[gn].buffer[x] + graphs[gn].vScroll) >> graphs[gn].scale)) & graphs[gn].div;
            //graphPix = graphPix + ( / graphs[gn].div);
            if(graphPix > 4096) graphPix = 0; /// FIX THIS ============================================
            if(graphPix >= GRAPH_ENDY) graphPix = GRAPH_ENDY-1;
            if(graphPix < lastGraphPix[gn]) tft.drawFastVLine(x, graphPix, lastGraphPix[gn] - graphPix, graphColor[gn]);
            else if(graphPix > lastGraphPix[gn]) tft.drawFastVLine(x, lastGraphPix[gn], graphPix - lastGraphPix[gn], graphColor[gn]);
            else tft.drawPixel(x, graphPix, graphColor[gn]);
            lastGraphPix[gn] = graphPix;
          }
        }
      }
      graphDrawTime = micros() - graphDrawTime; //math start time with end time
    
      for(byte gn = 0; gn < ANALOG_CH_COUNT; gn++){
        //if(graphs[gn].draw){
          graphs[gn].doWrite = true;
        //}
      }
    }
  }
}
