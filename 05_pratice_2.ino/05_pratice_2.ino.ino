#define PIN_LED 7
unsigned int toggle;
unsigned int i;
unsigned int count;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
  i = 0;
  count = 0;
}

void loop() {
  if (count == 0){
    // 가장 처음 1초 동안 불이 들어
    toggle = toggle_state(toggle);
    digitalWrite(PIN_LED, 0);
    delay(1000);
    
    while(i < 10)
    {
      toggle = toggle_state(toggle);
      digitalWrite(PIN_LED, toggle);
      delay(100);
      i = i + 1;
    }
    count += 1;
  }
}

int toggle_state(int toggle) {
  if(toggle == 1){
    toggle = toggle - 1;
  }
  else{
    toggle = toggle + 1;
  }
  return toggle;
}
