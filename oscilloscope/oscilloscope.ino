#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
//#include "CH_Graph.h"

#define GRAPH_STARTX 0
#define GRAPH_STARTY 0
#define GRAPH_ENDX 480
#define GRAPH_ENDY 257
#define GRAPH_HDIV_SIZE 32
#define GRAPH_VDIV_SIZE 32
#define GRAPH_CH_COUNT 4
#define ANALOG_CH_COUNT 4
#define BUFFER_SAMPLES 480
#define CH0 34
#define CH1 35
#define CH2 32
#define CH3 33
const int chPin[4] = {CH0, CH1, CH2, CH3};

struct Graph{
  unsigned short buffer[BUFFER_SAMPLES];
  bool draw, doWrite, trigger;
  short vScroll, vSmallScroll; //sensible scroll
  unsigned short triggerVal, scale, div, vLargeScroll; //large step scroll
  unsigned long long readTime;
};

void setup() {
  Serial.begin(115200);
  Graph *graphs = new Graph[GRAPH_CH_COUNT];
  if(graphs == NULL){
    Serial.println("graphs array not alocated");
    while(1) delay(5000);
  }
  xTaskCreatePinnedToCore(gui_task, "gui", 4096, graphs, 20, NULL, 0);
  xTaskCreatePinnedToCore(analogReader_task, "analogReader", 2048, graphs, 1, NULL, 1);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

}

/*
  Calibration of CH1

  At 11.61V -> scale>>3 gives 5div(5*32=160) + 8points = 168points
  168 << 3 = 1344 points of ADC scale
  11.61 / 1344 = 0.008638393 V per point

  At 3.72 -> scale>>1 gives 6div(6*32=192) + 14points = 206
  206 << 1 = 412 points of ADC scale
  3.72 / 412 = 0.009029126 V per point

  At 15.52 (.53) -> scale>>4 gives 4div(4*32=127)
  127~128 << 4 = 2032~2048
  V / pts = 0.007637795 ~ 0.007583008

  Estimated as 8 mV per ADC point
*/