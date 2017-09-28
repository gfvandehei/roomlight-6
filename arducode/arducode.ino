#include <Adafruit_NeoPixel.h>


#define PIN 9
#define NUMPIX 72


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIX, PIN, NEO_GRB + NEO_KHZ800);
int** ciroc;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //allocate the ciroc array
  ciroc=(int**)malloc(72*sizeof(int*));
  for(int i=0;i<72;i++){
    ciroc[i]=(int*)malloc(3*sizeof(int));
  }
  //set colors
  for(int i=0;i<10;i++){
    ciroc[i][0]=255;
    ciroc[i][1]=255;
    ciroc[i][2]=255;
  }
  for(int i=10;i<16;i++){
    ciroc[i][0]=255;
    ciroc[i][1]=0;
    ciroc[i][2]=0;
  }
  for(int i=16;i<21;i++){
    ciroc[i][0]=0;
    ciroc[i][1]=0;
    ciroc[i][2]=255;
  }
  for(int i=21;i<26;i++){
    ciroc[i][0]=0;
    ciroc[i][1]=255;
    ciroc[i][2]=0;
  }
  for(int i=26;i<36;i++){
    ciroc[i][0]=255;
    ciroc[i][1]=165;
    ciroc[i][2]=0;
  }
  for(int i=36;i<55;i++){
    ciroc[i][0]=255;
    ciroc[i][1]=0;
    ciroc[i][2]=0;
  }
  for(int i=55;i<60;i++){
    ciroc[i][0]=255;
    ciroc[i][1]=215;
    ciroc[i][2]=0;
  }
  for(int i=60;i<65;i++){
    ciroc[i][0]=0;
    ciroc[i][1]=255;
    ciroc[i][2]=0;
  }
  for(int i=65;i<72;i++){
    ciroc[i][0]=0;
    ciroc[i][1]=0;
    ciroc[i][2]=255;
  }
  

}

int numpix=0;
int integerValue=0;
char incomingByte;
void loop() {
<<<<<<< HEAD
  if(Serial.available()){
    integerValue = 0;         // throw away previous integerValue
    while(1) {            // force into a loop until 'n' is received
      incomingByte = Serial.read();
      if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
      if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
      integerValue *= 10;  // shift left 1 decimal place
      // convert ASCII to integer, add, and shift left 1 decimal place
      integerValue = ((incomingByte - 48) + integerValue);
    }
        
    for(int i=0;i<=integerValue;i++){
      strip.setPixelColor(i,ciroc[i][0],ciroc[i][1],ciroc[i][2]);
    }
    for(int i=integerValue;i<strip.numPixels();i++){
      strip.setPixelColor(i,0,0,0);
    }
    strip.show();
  }
  
}

// Fill the dots one after the other with a color
void clear_all(){
  //makes the entire strip display black
  for(uint16_t i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,0,0,0);
  }
  strip.show();
}

void bounce(){
  for(int i=0;i<strip.numPixels()-10;i++){
    for(int u=i;u<i+10;u++){
      strip.setPixelColor(u,ciroc[u][0],ciroc[u][1],ciroc[u][2]);
    }
    for(int u=0;u<i;u++){
      strip.setPixelColor(u,0,0,0);
    }
    delay(10);
    strip.show();
  }
  for(int i=strip.numPixels()-10;i>=0;i--){
    for(int u=i;u<i+10;u++){
      strip.setPixelColor(u,ciroc[u][0],ciroc[u][1],ciroc[u][2]);
    }
    for(int u=strip.numPixels();u>i+10;u--){
      strip.setPixelColor(u,0,0,0);
    }
    delay(10);
    strip.show();
  }
}

void color_area(int start, int num, int r, int g, int b, int brightness){
  //colors a certain area of the strip
  strip.setBrightness(brightness);
  for(int i=start;i<=start+num;i++){
    strip.setPixelColor(i,r,g,b);
  }
  strip.show();
}

void set_work_light(char* ltype){
  if(ltype=="white"){
    color_area(0,NUMPIX,255,255,255,255);
  }
  else if(ltype=="yellow"){
    color_area(0,NUMPIX,255,255,147,255);
  }
  else{
    printf("invalid work light");
  }
}


//start of original functions
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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