void analogReader_task(void *pvParameters){
  Graph *graphs = {static_cast<Graph*>(pvParameters)};
  
  while(1){
    bool doWrite[ANALOG_CH_COUNT];
    byte chCount;
    unsigned short *bf[ANALOG_CH_COUNT];
    do{
      chCount = 0;
      for(byte gn = 0; gn < ANALOG_CH_COUNT; gn++){
        chCount += doWrite[gn] = graphs[gn].doWrite && graphs[gn].draw;
        bf[gn] = graphs[gn].buffer;
        delay(10);
      }
    }while(chCount == 0);
    do{
      chCount = 0;
      for(byte gn = 0; gn < ANALOG_CH_COUNT; gn++){
        chCount += doWrite[gn] = graphs[gn].doWrite && graphs[gn].draw;
        bf[gn] = graphs[gn].buffer;
        delay(10);
      }
    }while(chCount == 0);

    for(uint8_t i = 0; i < ANALOG_CH_COUNT; i++){ // trigger
      if(graphs[i].trigger){
        byte pin = chPin[i];
        uint16_t *val = &graphs[i].triggerVal;
        bool *active = &graphs[i].trigger;
        while((analogRead(pin) > *val) && *active) vTaskDelay(1); // wait till trigger CH is lower
        while((analogRead(pin) < *val) && *active) vTaskDelay(1); // wait till it get higher
        i = ANALOG_CH_COUNT;
      }
    }
    unsigned long long startTime = micros();
    for(unsigned short i = 0; i < BUFFER_SAMPLES; i++){
      if(doWrite[0]) *bf[0] = analogRead(CH0);
      if(doWrite[1]) *bf[1] = analogRead(CH1);
      if(doWrite[2]) *bf[2] = analogRead(CH2);
      if(doWrite[3]) *bf[3] = analogRead(CH3);
      bf[0]++;
      bf[1]++;
      bf[2]++;
      bf[3]++;
    }
    graphs->readTime = micros() - startTime;
    //Serial.println(micros() - startTime);
    for(byte gn = 0; gn < ANALOG_CH_COUNT; gn++){
      graphs[gn].doWrite = false;
    }
    vTaskDelay(1);
  }
}
