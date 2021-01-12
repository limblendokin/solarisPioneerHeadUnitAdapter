enum Functions {TUNE_UP, TUNE_DOWN, SOURCE, MUTE, VOLUME_UP, VOLUME_DOWN, LENGTH};
int solarisResistances[LENGTH];
int inputPrecision = 500;
// length must be greater than lg2 of number of functions
int outputPinsLength = 3;
int outputPins[3];
enum Functions pressedFunction = LENGTH;
unsigned long pressedTime;
int analogPin= 0;
int raw= 0;
int Vin= 5;
float Vout= 0;
float R1= 10000;
float R2= 0;
float buffer= 0;

// testing vars
int SWCPins[LENGTH];
int currentSWCFunction = LENGTH;
int SWCSwitchButtonPin = 6;
int SWCSwitchButtonPrevState = LOW;

void setup()
{
  Serial.begin(9600);
  // Значения резисторов в мультируле
  solarisResistances[TUNE_UP] = 330; // 420;
  solarisResistances[TUNE_DOWN] = 1200; // 1100;
  solarisResistances[SOURCE] = 2200; // 2100;
  solarisResistances[MUTE] = 3200; // 3100;
  solarisResistances[VOLUME_UP] = 4700; // 4600;
  solarisResistances[VOLUME_DOWN] = 6700; // 6800;
  SWCPins[TUNE_UP] = 8;
  SWCPins[TUNE_DOWN] = 9;
  SWCPins[SOURCE] = 10;
  SWCPins[MUTE] = 11;
  SWCPins[VOLUME_UP] = 12;
  SWCPins[VOLUME_DOWN] = 13;
  for(int i = 0; i<outputPinsLength; i++){
    outputPins[i] = i+2;
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW);
  }
  
}

void loop()
{
  int SWCSwitchButtonState = digitalRead(SWCSwitchButtonPin);
  if(SWCSwitchButtonPrevState == HIGH && SWCSwitchButtonPrevState != SWCSwitchButtonState){
    pinMode(SWCPins[currentSWCFunction], INPUT);
    Serial.print("Switched to input pin ");
    Serial.println(SWCPins[currentSWCFunction]);
    if(++currentSWCFunction > LENGTH){
      currentSWCFunction = 0;
    }
    else if(currentSWCFunction != LENGTH){
      pinMode(SWCPins[currentSWCFunction], OUTPUT);
      digitalWrite(SWCPins[currentSWCFunction], HIGH);
      Serial.print("Switched to high output pin ");
      Serial.println(SWCPins[currentSWCFunction]);
    }
  }
  SWCSwitchButtonPrevState = SWCSwitchButtonState;
  raw= analogRead(analogPin);
  if(raw)
  {
    buffer= raw * Vin;
    Vout= (buffer)/1024.0;
    buffer= (Vin/Vout) -1;
    R2= R1 * buffer;
    for(int i = 0; i<LENGTH; i++){
      int minResistance = solarisResistances[i] - inputPrecision;
      int maxResistance = solarisResistances[i] + inputPrecision;
      if( (R2 > minResistance) && (R2<maxResistance)){
        // Если кнопку только нажали, запомнить
        if(pressedFunction == LENGTH){
          pressedFunction = (Functions)i;
          pressedTime = millis();
        }
        // Если нажали другую кнопку, то выполнить предыдущую функцию
        else if(pressedFunction != i){
          execute();
          pressedFunction = (Functions)i;
          pressedTime = millis();
        }
        break;
      }
    }
  }
  else{
    execute();
  }
}
void execute(){
  if(pressedFunction != LENGTH){
    Serial.print("Press time: ");
    Serial.print(millis()-pressedTime);
    Serial.print("ms\nFunction: ");
    Serial.println(pressedFunction);
    for(int i = 0; i<outputPinsLength; i++){
      Serial.print("bit ");
      Serial.print(i);
      Serial.print(" is ");
      Serial.println(bitRead(pressedFunction, i) ? 1 : 0);
      digitalWrite(outputPins[i], bitRead(pressedFunction, i) ? HIGH : LOW); 
    }
    pressedFunction = LENGTH;
  }
}
