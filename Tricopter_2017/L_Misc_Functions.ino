int ledState;

float deg(float rad) {
  float angle;
  angle = rad * (180 / PI);
  return angle;
}

float rad(float deg) {
  float angle;
  angle = deg * (PI / 180);
  return angle;
}

int floatToInt(float _float) {
  _float = _float * 100.0;
  return int(_float);
}

void blinkLED(int interval) {
  if (millis() - blinkTimer >= interval) {
    blinkTimer = millis();
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
}
