/*
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 ** TX - pin 2
 ** RX - pin 3
 */
#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>  

File myFile;
SoftwareSerial cameraconnection = SoftwareSerial(2, 3);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
byte jpeg_head;
int start;
void setup() {
	Serial.begin(9600);
	cameraconnection.begin(38400);
	if (!SD.begin(10)) {
		Serial.println("error initialization failed!");
		return;
	}
	if (cam.begin()) {
		Serial.println("Camera Found:");
	}
	else {
		Serial.println("No camera found?");
		return;
	}
	char *reply = cam.getVersion();
	if (reply == 0) {
		Serial.print("Failed to get version");
	}
	else {
		Serial.println("-----------------");
		Serial.print(reply);
		Serial.println("-----------------");
	}
	cam.setImageSize(VC0706_320x240);
}

void loop() {
	if (Serial.available() > 0)
	{
		String file_name = Serial.readStringUntil('\n');
		cam.takePicture();
		File imgFile = SD.open(file_name, FILE_WRITE);
		uint16_t jpglen = cam.frameLength();
		while (jpglen > 0) {
			uint8_t *buffer;
			uint8_t bytesToRead = min(8, jpglen);
			buffer = cam.readPicture(bytesToRead);
			imgFile.write(buffer, bytesToRead);
            Serial.write(buffer,bytesToRead);
			jpglen -= bytesToRead;
		}
		imgFile.close();
        cam.resumeVideo();
        
	}
}


