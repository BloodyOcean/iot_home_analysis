// C++ code
//
int prVal = 0;

int terVal = 0;

int humVal = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);

  Serial.println("hello world");
}

void loop()
{
  terVal = analogRead(A1);
  prVal = analogRead(A0);
  humVal = analogRead(A2);
  Serial.println(prVal);
  Serial.println(terVal);
  Serial.println(humVal);
  delay(1000); // Delay a little bit to improve simulation performance
}