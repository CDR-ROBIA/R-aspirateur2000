char arr[30]; 
char dep[2];
int angle;
int translation;


void setup() { 
	Serial.begin(9600); 
	Serial.setTimeout(1); 
} 
void loop() { 
	while (!Serial.available());{
	Serial.readString().toCharArray(arr, sizeof(arr));
	Serial.print(2);
	Serial.print(",");
	Serial.print(3);
	Serial.print("\n");
	}
} 
