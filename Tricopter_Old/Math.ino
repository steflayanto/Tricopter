float deg(float rad) {
  float angle;
  angle=rad*(180/PI);
  return angle;
}

float rad(float deg) {
  float angle;
  angle=deg*(PI/180);
  return angle;
}

int floatToInt(float _float){
  _float = _float*100.0;
  return int(_float);
}
