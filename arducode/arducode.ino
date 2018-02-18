#include <Adafruit_NeoPixel.h>

#define NUMSTRIPS 3

int pins[NUMSTRIPS]={6,9,10};
int lengths[NUMSTRIPS]={72,25,146};
Adafruit_NeoPixel strips[NUMSTRIPS];
Adafruit_NeoPixel strip;
int centerValues[NUMSTRIPS];
int integerValue=0;
char incomingByte;
char incomingchar;


int SerialRead(){
  /*
  Reads the serial input and stores the data in an easily accessable way
  */
  if(Serial.available()){
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if(isAlpha(incomingByte) && incomingByte!='\n'){
        incomingchar=incomingByte;
      }
      else{
        if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
        if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
        integerValue *= 10;  // shift left 1 decimal place
        // convert ASCII to integer, add, and shift left 1 decimal place
        integerValue = ((incomingByte - 48) + integerValue);
      }
    }
    return integerValue;
  }   
}

void colorWipe(uint32_t c, uint8_t wait, int strip_num) {
  for(uint16_t i=0; i<strips[strip_num].numPixels(); i++) {
    if(Serial.available()){
      break;
    }
    strips[strip_num].setPixelColor(i, c);
    strips[strip_num].show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strips[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strips[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strips[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbowCycle_for_audio(uint8_t wait,int selected_strip, int start, int end_, int back, int j) {
  uint16_t i;

   // 5 cycles of all colors on wheel
    if(back == 0)
    {
      // for(j=0; j<256; j++) {
        for(i=start; i <= end_*2; i++) {
          strips[selected_strip].setPixelColor(i, Wheel(((i * 256 / strips[selected_strip].numPixels()) + j) & 255));
        }
      // }
    }
    else
    {
      // for(j=0; j<256; j++) {
        for(i=start*2; i >= end_ ; i--) {
          strips[selected_strip].setPixelColor(i, Wheel(((i * 256 / strips[selected_strip].numPixels()) + j) & 255));
        }
      // }
    }
    // strip.show();
    // delay(wait);
    return;
}

void sunnylight(int stripnum){
  int randomnum=random(strips[stripnum].numPixels()-5);
  for(int i=0;i<5;i++){
    strips[stripnum].setPixelColor(randomnum+i,255,255,255);
  }
  strips[stripnum].show();
  delay(20);
}

void weatherfunct(int weathercode, int stripnum){
  if(weathercode==1){
    while(!Serial.available()){
      sunnylight(stripnum);
      colorWipe(strip.Color(0,0,211),10,stripnum);
    }
  }
  else if(weathercode==2){
    colorWipe(strip.Color(211,211,211),10,stripnum);
  }
  else if(weathercode==3){
    colorWipe(strip.Color(255,255,255),10,stripnum);
  }
  else if(weathercode==4){
    colorWipe(strip.Color(211,211,211),10,stripnum);
  }
  else if(weathercode==5){
    colorWipe(strip.Color(211,211,211),10,stripnum);
  }
  else if(weathercode==6){
    colorWipe(strip.Color(211,211,211),10,stripnum);
  }
  else if(weathercode==7){
    colorWipe(strip.Color(211,0,0),10,stripnum);
    colorWipe(strip.Color(0,0,0),10,stripnum);
  }
}

void center_vis(int selected_strip, int integerValue){
  int center_val =strips[selected_strip].numPixels()/2;  
  int move =map(integerValue, 1, 70, 1, strips[selected_strip].numPixels());// integerValue/2;
  if(selected_strip==9){ 
    for(int i=center_val; i >= center_val-move && i>0; i--){ //select lights from center towards pos 0
      strips[selected_strip].setPixelColor(i,255,0,0);
    }
    for(int i=0; i<center_val-move; i++){ //set remaining lights from left end to black
      strips[selected_strip].setPixelColor(i,0,0,0);
    }
    for(int i=center_val; i<= (center_val+move) && i<strips[selected_strip].numPixels(); i++){ //select lights from center towards numPixels()
      strips[selected_strip].setPixelColor(i,255,0,0);
    }
    for(int i=center_val + move; i<strips[selected_strip].numPixels(); i++){ //set lights up to numPixels() to black
      strips[selected_strip].setPixelColor(i,0,0,0);
    }
    strips[selected_strip].show();
  }
  else{
    for(int i=center_val; i >= center_val-move && i>0; i--){ //select lights from center towards pos 0
      strips[selected_strip].setPixelColor(i,255,0,0);
    }
    for(int i=0; i<center_val-move; i++){ //set remaining lights from left end to black
      strips[selected_strip].setPixelColor(i,0,0,0);
    }
    for(int i=center_val; i<= (center_val+move) && i<strips[selected_strip].numPixels(); i++){ //select lights from center towards numPixels()
      strips[selected_strip].setPixelColor(i,255,0,0);
    }
    for(int i=center_val + move; i<strips[selected_strip].numPixels(); i++){ //set lights up to numPixels() to black
      strips[selected_strip].setPixelColor(i,0,0,0);
    }
    strips[selected_strip].show();
  }
}

void rainbow_vis(int selected_strip, int integerValue, int tmp){ //if values get too high this crashes
  int center_val = centerValues[selected_strip];  
  int move = map(integerValue, 1, 100, 1, centerValues[selected_strip]);//strips[selected_strip].numPixels());// integerValue/2;
  int start, end_, back = 0;

  start = center_val;
  end_ = center_val-move;
  back = 1;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp); //select lights from center towards pos 0
  for(int i=0; i<center_val-move; i++){ 
      strips[selected_strip].setPixelColor(i,0,0,0);
  }
  start = center_val;
  end_ = center_val+move;
  back = 0;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  for(int i=center_val + move; i<strips[selected_strip].numPixels(); i++){ //set lights up to numPixels() to black
      strips[selected_strip].setPixelColor(i,0,0,0);
  }
  strips[selected_strip].show();
  return;
}

void JD_funct3(int selected_strip, int integerValue, int tmp){////if values get too high this crashes
  int center_val =strips[selected_strip].numPixels()/2;  
  int move =map(integerValue, 1, 70, 1, strips[selected_strip].numPixels());// integerValue/2;
  
  int start,end_,back=0;
  start = strips[selected_strip].numPixels()-move;
  end_ = strips[selected_strip].numPixels();
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  back = 0;
  start = center_val;
  end_ = center_val+move;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  for(int i=center_val+move; i <= strips[selected_strip].numPixels()-move && i>0; i++){ //select lights from center towards pos 0
    strips[selected_strip].setPixelColor(i,0,0,0);
  }

  start = 0;
  end_ = move;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  start = move;
  end_ = center_val-move;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  for(int i=move; i< center_val-move; i++){ //select lights from center towards numPixels()
    strips[selected_strip].setPixelColor(i,0,0,0);
  }
  strips[selected_strip].show();
  return;
}

void single_side_vis(int selected_strip, int integerValue){
  integerValue=map(integerValue, 1, 70, 1, strips[selected_strip].numPixels());
  if(selected_strip==0){
    for(int i=0;i<=integerValue;i++){
      strips[selected_strip].setPixelColor(i,255,255,0);
    }
    for(int i=integerValue;i<strips[selected_strip].numPixels();i++){
      strips[selected_strip].setPixelColor(i,0,0,0);
    }
    strips[selected_strip].show();
  }
  else{
    for(int i=0;i<=integerValue;i++){
      strips[selected_strip].setPixelColor(i,255,255,0);
    }
    for(int i=integerValue;i<strips[selected_strip].numPixels();i++){
      strips[selected_strip].setPixelColor(i,0,0,255);
    }
    strips[selected_strip].show();
  }
}
// Fill the dots one after the other with a color
void clear_all(int numstrip){
  //makes the entire strip display black
  for(uint16_t i=0;i<strips[numstrip].numPixels();i++){
    strips[numstrip].setPixelColor(i,0,0,0);
  }
  strips[numstrip].show();
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle_for_button(uint8_t wait, int j) {
  uint16_t i;

  // for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< 300; i++) {
      for(int z=0; z<3;z++){
        strips[z].setPixelColor(i, Wheel(((i * 256 / strips[z].numPixels()) + j) & 255));
      }
    }
    strips[0].show();
    strips[1].show();
    strips[2].show();
    delay(wait); 
    return;   //added return so incoming char can still be read
  // }
}



void setup() {
  Serial.begin(9600);
  for(int i=0; i<NUMSTRIPS; i++){ //initialize all strips and set to black, initialize center value array
    strips[i]=Adafruit_NeoPixel(lengths[i], pins[i], NEO_GRB + NEO_KHZ800);
    centerValues[i]=strips[i].numPixels()/2;
    strips[i].begin(); //starts strip
    strips[i].show(); //sets strip to blank
  }
  strip=strips[0];
}

int tmp =0;
void loop() {
  int input=SerialRead();
  if(incomingchar=='a'){ //Works as expected, exits as expected
    for(int i=0;i<NUMSTRIPS;i++){
      single_side_vis(i,input);
    }
  }
  else if(incomingchar=='b'){ //Works as expected, exits as expected
    for(int i=0;i<NUMSTRIPS;i++){
      center_vis(i,input);
    }
  }
  else if(incomingchar=='c'){ //works as expected exits does not exit to default for some reason
    for(int i=0;i<NUMSTRIPS;i++){
      weatherfunct(input,i);
    }
  }
  else if(incomingchar=='d'){
    rainbow_vis(2,input, tmp);
    rainbow_vis(1,input, tmp);
    rainbow_vis(0,input, tmp); //only jds strip
    ++tmp;
    if(tmp==256*5) {tmp=0;}
  }
  else if(incomingchar=='e'){
    rainbowCycle_for_button(25,tmp);
    
    ++tmp;
    if(tmp==256*5) {tmp=0;}
  }
  else if(incomingchar=='f'){
    JD_funct3(2,input,tmp); //only jds strip
    JD_funct3(0,input,tmp);
    JD_funct3(1,input,tmp);
    ++tmp;
    if(tmp==256*5) {tmp=0;}
  }
  else{
    for(int i=0;i<NUMSTRIPS;i++){
      colorWipe(strips[i].Color(255,0,0),10,1);
    }
    for(int i=0;i<NUMSTRIPS;i++){
      colorWipe(strips[i].Color(0,0,0),10,1);
    }
  }
}
