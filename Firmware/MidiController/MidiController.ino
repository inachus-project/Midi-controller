// Midi Controller

#define SW1     2
#define SW2     3
#define SW3     4
#define SW4     5

#define PLAY      SW1
#define STOP      SW2
#define RECORD    SW3
#define FUNCTION  SW4

#define PLAY_CC       116
#define STOP_CC       117
#define RECORD_CC     118
#define FUNCTION_CC   119

#define BUTTON_VALUE    0x7F
#define BUTTON_CHANNEL  0x1

#define BUTTON_MODE INPUT_PULLUP

#define DEBOUNCE_MILLIS   100
elapsedMillis debounce_timer = 0;

#define CC19    14
#define CC11    15
#define CC1     16

#define SLIDER_INCREMENT    0.25
#define NEW_WEIGHT          0.05
#define SLIDER_MIN_VARIANCE 0.1
#define SENT_MIN_VARIANCE   1.0
#define SENT_MAX_VARIANCE   0.75

// Channel constants
#define CC_VOLUME_CONTROL_CHANNEL   0x1
#define CC19_VOLUME                 19
#define CC11_VOLUME                 11
#define CC1_VOLUME                  1
#define CC_VOLUME_VALUE_MASK        0x7F
#define CC_HEARTBEAT_TIMEOUT        5000

// Global variables
// CC19
float CC19_value = 0;
int CC19_last_value = (int)CC19_value;
int CC19_last_sent_value = CC19_last_value;
bool CC19_changing = false;
elapsedMillis CC19_heartbeat = 0;

// CC11
float CC11_value = 0;
int CC11_last_value = (int)CC11_value;
int CC11_last_sent_value = CC11_last_value;
bool CC11_changing = false;
elapsedMillis CC11_heartbeat = 0;

// CC1
float CC1_value = 0;
int CC1_last_value = (int)CC1_value;
int CC1_last_sent_value = CC1_last_value;
bool CC1_changing = false;
elapsedMillis CC1_heartbeat = 0;

// Buttons
bool PLAY_pushed = false;
bool STOP_pushed = false;
bool RECORD_pushed = false;
bool FUNCTION_pushed = false;

void setup() {
  
  // Initialize serial communication
  Serial.begin(115200);

  // Setup buttons
  pinMode(PLAY, BUTTON_MODE);
  pinMode(STOP, BUTTON_MODE);
  pinMode(RECORD, BUTTON_MODE);
  pinMode(FUNCTION, BUTTON_MODE);

  attachInterrupt(PLAY, PLAY_button_ISR, CHANGE);
  attachInterrupt(STOP, STOP_button_ISR, CHANGE);
  attachInterrupt(RECORD, RECORD_button_ISR, CHANGE);
  attachInterrupt(FUNCTION, FUNCTION_button_ISR, CHANGE);
  
}

void loop() {

  // Check channel 19
  check_channel_CC19();

  // Check channel 11
  check_channel_CC11();

  // Check channel 1
  check_channel_CC1();
  
}

// Check channel 19 slider
void check_channel_CC19(void){
  
  // Slider weight variable
  float OLD_WEIGHT = 1.0 - NEW_WEIGHT;

  // Read slider
  float temp_slide_value = SLIDER_INCREMENT * analogRead(CC19);

  // Check if noise or motion
  if(abs(temp_slide_value - CC19_value) > SENT_MIN_VARIANCE){

    CC19_changing = true;
    
  }
  else if(abs(temp_slide_value - CC19_value) < SENT_MAX_VARIANCE){

    CC19_changing = false;
    
  }

  // Update moving average
  if(CC19_changing){
    CC19_value = (NEW_WEIGHT * temp_slide_value) + (OLD_WEIGHT * CC19_value);
  }

  // Convert to int
  CC19_last_value = ((int)CC19_value) / 2;

  // Send value if changed
  // if((CC19_last_sent_value != CC19_last_value) || (CC19_heartbeat > CC_HEARTBEAT_TIMEOUT)){
  if(CC19_last_sent_value != CC19_last_value){
    
    CC19_last_sent_value = CC19_last_value;
    CC19_heartbeat = 0;

    Serial.print("CC19 is ");
    Serial.print(CC19_last_sent_value);
    Serial.print("\n");
    usbMIDI.sendControlChange(CC19_VOLUME, CC19_last_sent_value & CC_VOLUME_VALUE_MASK, CC_VOLUME_CONTROL_CHANNEL);
    
  }
  
}

// Check channel 11 slider
void check_channel_CC11(void){
  
  // Slider weight variable
  float OLD_WEIGHT = 1.0 - NEW_WEIGHT;

  // Read slider
  float temp_slide_value = SLIDER_INCREMENT * analogRead(CC11);

  // Check if noise or motion
  if(abs(temp_slide_value - CC11_value) > SENT_MIN_VARIANCE){

    CC11_changing = true;
    
  }
  else if(abs(temp_slide_value - CC11_value) < SENT_MAX_VARIANCE){

    CC11_changing = false;
    
  }

  // Update moving average
  if(CC11_changing){
    CC11_value = (NEW_WEIGHT * temp_slide_value) + (OLD_WEIGHT * CC11_value);
  }

  // Convert to int
  CC11_last_value = ((int)CC11_value) / 2;

  // Send value if changed
  // if((CC11_last_sent_value != CC11_last_value) || (CC11_heartbeat > CC_HEARTBEAT_TIMEOUT)){
  if(CC11_last_sent_value != CC11_last_value){
    
    CC11_last_sent_value = CC11_last_value;
    CC11_heartbeat = 0;

    Serial.print("CC11 is ");
    Serial.print(CC11_last_sent_value);
    Serial.print("\n");
    usbMIDI.sendControlChange(CC11_VOLUME, CC11_last_sent_value & CC_VOLUME_VALUE_MASK, CC_VOLUME_CONTROL_CHANNEL);
    
  }
  
}

// Check channel 1 slider
void check_channel_CC1(void){
  
  // Slider weight variable
  float OLD_WEIGHT = 1.0 - NEW_WEIGHT;

  // Read slider
  float temp_slide_value = SLIDER_INCREMENT * analogRead(CC1);

  // Check if noise or motion
  if(abs(temp_slide_value - CC1_value) > SENT_MIN_VARIANCE){

    CC1_changing = true;
    
  }
  else if(abs(temp_slide_value - CC1_value) < SENT_MAX_VARIANCE){

    CC1_changing = false;
    
  }

  // Update moving average
  if(CC1_changing){
    CC1_value = (NEW_WEIGHT * temp_slide_value) + (OLD_WEIGHT * CC1_value);
  }

  // Convert to int
  CC1_last_value = ((int)CC1_value) / 2;

  // Send value if changed
  // if((CC1_last_sent_value != CC1_last_value) || (CC1_heartbeat > CC_HEARTBEAT_TIMEOUT)){
  if(CC1_last_sent_value != CC1_last_value){
    
    CC1_last_sent_value = CC1_last_value;
    CC1_heartbeat = 0;

    Serial.print("CC1 is ");
    Serial.print(CC1_last_sent_value);
    Serial.print("\n");
    usbMIDI.sendControlChange(CC1_VOLUME, CC1_last_sent_value & CC_VOLUME_VALUE_MASK, CC_VOLUME_CONTROL_CHANNEL);
    
  }
  
}

void PLAY_button_ISR(void){
  
  // Check button
  if(!digitalRead(PLAY) && !PLAY_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(digitalRead(PLAY)) return;  
      
    }
    
    PLAY_pushed = true;
    Serial.print("Play pushed\n");
    usbMIDI.sendControlChange(PLAY_CC, BUTTON_VALUE, BUTTON_CHANNEL);
    
  }
  else if(digitalRead(PLAY) && PLAY_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(!digitalRead(PLAY)) return;  
      
    }

    Serial.print("Play released\n");
    PLAY_pushed = false;
    
  }
  
}

void STOP_button_ISR(void){
  
  // Check button
  if(!digitalRead(STOP) && !STOP_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(digitalRead(STOP)) return;  
      
    }
    
    STOP_pushed = true;
    Serial.print("Stop pushed\n"); 
    usbMIDI.sendControlChange(STOP_CC, BUTTON_VALUE, BUTTON_CHANNEL);
    
  }
  else if(digitalRead(STOP) && STOP_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(!digitalRead(STOP)) return;  
      
    }

    Serial.print("Stop released\n");
    STOP_pushed = false;
    
  }
  
}

void RECORD_button_ISR(void){
  
  // Check button
  if(!digitalRead(RECORD) && !RECORD_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(digitalRead(RECORD)) return;  
      
    }
    
    RECORD_pushed = true;
    Serial.print("Record pushed\n"); 
    usbMIDI.sendControlChange(RECORD_CC, BUTTON_VALUE, BUTTON_CHANNEL);
    
  }
  else if(digitalRead(RECORD) && RECORD_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(!digitalRead(RECORD)) return;  
      
    }

    Serial.print("Record released\n");
    RECORD_pushed = false;
    
  }
  
}

void FUNCTION_button_ISR(void){
  
  // Check button
  if(!digitalRead(FUNCTION) && !FUNCTION_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(digitalRead(FUNCTION)) return;  
      
    }
    
    FUNCTION_pushed = true;
    Serial.print("Function pushed\n");  
    usbMIDI.sendControlChange(FUNCTION_CC, BUTTON_VALUE, BUTTON_CHANNEL);
    
  }
  else if(digitalRead(FUNCTION) && FUNCTION_pushed){

    debounce_timer = 0;
    while(debounce_timer < DEBOUNCE_MILLIS){
      
      if(!digitalRead(FUNCTION)) return;  
      
    }

    Serial.print("Function released\n");
    FUNCTION_pushed = false;
    
  }
  
}
