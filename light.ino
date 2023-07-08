const int led_pin = 13;
const int button_pin = 12;

bool on_off = false;

void setup_lamp(){
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
}

void update_lamp(){
  int val = digitalRead(button_pin);

  //Button has been clicked
  if(val == LOW) {
    on_off = !on_off;

      //On
    if(on_off){
      digitalWrite(led_pin, HIGH);
      //Serial.println("on");
    
    //Off
    } else {
      digitalWrite(led_pin, LOW);
      //Serial.println("off");
    }

    delay(500);
  }
}