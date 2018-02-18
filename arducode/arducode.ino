#include <Adafruit_NeoPixel.h>


#define PIN 9
#define PIN_2 10
#define PIN_3 6
#define NUMPIX 72
#define NUMPIX2 130
#define NUMPIX3 146

#define NUMSTRIPS 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIX, PIN, NEO_GRB + NEO_KHZ800); //initialize behind the bar lights
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIX2, PIN_2, NEO_GRB + NEO_KHZ800); // initialize under bed light
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUMPIX3, PIN_3, NEO_GRB + NEO_KHZ800); // initialize jds nigga its lit
Adafruit_NeoPixel strips[NUMSTRIPS]={strip,strip2,strip3}; //an array which contains both of the strips
int striplen[NUMSTRIPS]={NUMPIX,NUMPIX2,NUMPIX3};
int numpix=0;
int integerValue=0;
char incomingByte;
char incomingchar;
int gabe = 0; //sets if JD or gabes stuff should be used 

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
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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

void JD_funct(int selected_strip, int integerValue){
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

void JD_funct2(int selected_strip, int integerValue, int tmp){
  int center_val =strips[selected_strip].numPixels()/2;  
  int move =map(integerValue, 1, 70, 1, strips[selected_strip].numPixels());// integerValue/2;
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

void JD_funct3(int selected_strip, int integerValue, int tmp){
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
  // back = 0;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  // for(int i=0; i<move; i++){ //set remaining lights from left end to black
  //   strips[selected_strip].setPixelColor(i,0,0,255);
  // }
  start = move;
  end_ = center_val-move;
  // back = 0;
  rainbowCycle_for_audio(50, selected_strip, start, end_, back, tmp);
  // for(int i=move; i<center_val-move; i++){ //set lights up to numPixels() to black
  //   strips[selected_strip].setPixelColor(i,0,0,0);
  // }
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.


void setup() {
  Serial.begin(9600);
  colorWipe(strip.Color(255,0,0),10,1);
  for(int i=0; i<NUMSTRIPS; i++){ //loop begins and erases all strips
    strips[i].begin(); //starts strip
    strips[i].show(); //sets strip to blank
  }
}

int tmp =0;
void loop() {
  int input=SerialRead();
  if(incomingchar=='a'){
    /*for(int i=0;i<NUMSTRIPS;i++){
      single_side_vis(strips[i],input);
    }*/
    single_side_vis(0,input);
    single_side_vis(1,input);
    single_side_vis(2,input);
  }
  else if(incomingchar=='b'){
    for(int i=0;i<NUMSTRIPS;i++){
      JD_funct(i,input);
    }
    //JD_funct(1,input);
    
    //JD_funct(2,input);//only jds strip
  }
  else if(incomingchar=='c'){
    weatherfunct(input,1);
    weatherfunct(input,0);
    weatherfunct(input,2);
  }
  else if(incomingchar=='d'){
    JD_funct2(2,input, tmp);
    JD_funct2(1,input, tmp);
    JD_funct2(0,input, tmp); //only jds strip
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
    colorWipe(strip.Color(255,0,0),10,1);
    colorWipe(strip.Color(0,0,0),10,1);
  }
}
