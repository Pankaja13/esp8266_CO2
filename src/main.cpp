#include <SoftwareSerial.h>

const long samplePeriod = 10000L;
SoftwareSerial sensor(5, 4); // RX, TX

long lastSampleTime = 0;

int readPPMSerial();

void setup() {
	Serial.begin(115200);
	sensor.begin(9600);
}

void loop() {
	long now = millis();
	if (now > lastSampleTime + samplePeriod) {
		lastSampleTime = now;
		Serial.println("Reading:");
		int ppmS = readPPMSerial();
		Serial.println(ppmS);
	}
}

int readPPMSerial() {
	if (millis() < 1000*60*3){ // 3 minute heat-up time
		return -1;
	}

	const byte requestReading[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
	byte result[9];

	for (unsigned char i : requestReading) {
		sensor.write(i);
	}
	while (sensor.available() < 9) {}; // wait for response
	for (unsigned char & i : result) {
		i = sensor.read();
	}
	int high = result[2];
	int low = result[3];
	return high * 256 + low;
}

