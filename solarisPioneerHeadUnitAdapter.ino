enum Functions {TUNE_UP, TUNE_DOWN, SOURCE, MUTE, VOLUME_UP, VOLUME_DOWN, LENGTH};
int solarisResistances[LENGTH];
int inputPrecision = 500;
// length must be greater than lg2 of number of functions
int outputPinsLength = 3;
int outputPins[3];
enum Functions prevPressedFunction = LENGTH;
unsigned long pressedTime;
int analogPin= 0;
int raw= 0;
int Vin= 5;
float Vout= 0;
float R1= 10000;
float R2= 0;
float buffer= 0;

// testing vars

void setup()
{
  Serial.begin(9600);
  // Значения резисторов в мультируле
  solarisResistances[TUNE_UP] = 420;
  solarisResistances[TUNE_DOWN] = 1100;
  solarisResistances[SOURCE] = 2100;
  solarisResistances[MUTE] = 3100;
  solarisResistances[VOLUME_UP] = 4600;
  solarisResistances[VOLUME_DOWN] = 6800;
  for(int i = 0; i<outputPinsLength; i++){
    outputPins[i] = i+2;
    pinMode(outputPins[i], OUTPUT);
    digitalWrite(outputPins[i], LOW);
  }
  
}

void loop()
{
  enum Functions pressedFunction = getPressedFunction();
  if(prevPressedFunction!=LENGTH && prevPressedFunction != pressedFunction){

  }
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
enum Functions getPressedFunction(){
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
        return (Functions)i;
      }
    }
  }
  return LENGTH;
}
void startExecutingFunction(enum Functions function){

}
void endExecution(){
  
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
