#include "audiodata.h"  // wav data with header 
// Audio dac 
#include <driver/dac.h>

//wav parameter
uint32_t chunkSize;
uint16_t numChannels;
uint32_t sampleRate;
uint16_t bitsPerSample;

uint8_t  data8;
uint16_t data16;

uint8_t  left;
// uint8_t  right;
uint16_t delayus;

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  // sigmaDeltaAttachPin(14,0); //attach pin 25 to channel 0 (other pin is also OK)
  dac_output_enable(DAC_CHANNEL_1); // use DAC_CHANNEL_1 (pin 25 fixed)
}

int play(const unsigned char *audio, uint32_t length ) {
  uint32_t count = 0;

  //check 'RIFF'
  if(strncmp((char*)audio, "RIFF", 4)){
    Serial.println( "Error: It is not RIFF.");
    return -1; 
  }

  //check 'WAVE'
  if(strncmp((char*)audio + 8, "WAVE", 4)){
    Serial.println( "Error: It is not WAVE.");
    return -1; 
  }

  //check 'fmt '
  if(strncmp((char*)audio + 12, "fmt ", 4)){
    Serial.println( "Error: fmt not found.");
    return -1; 
  }

  //get 'fmt ' data
  memcpy(&chunkSize, (char*)audio + 16, sizeof(chunkSize)); // size of fmt chunk
  memcpy(&numChannels, (char*)audio + 22, sizeof(numChannels)); // 1: mono or 2: stereo
  memcpy(&sampleRate, (char*)audio + 24, sizeof(sampleRate)); // 8kHz:8000 , 16kHz:16000, ...
  memcpy(&bitsPerSample, (char*)audio + 34, sizeof(bitsPerSample)); // 8bit or 16bit 
  
  //skip to tne next chunk
  count = 16 + sizeof(chunkSize) + chunkSize;

  //skip to the data chunk
  while (strncmp((char*)audio + count , "data", 4)) {
    count += 4;
    memcpy(&chunkSize, (char*)audio + count, sizeof(chunkSize));   
    count += sizeof(chunkSize) + chunkSize;
    if ( count > length ) return -1;
  }
  //skip to data area
  count += 4 + sizeof(chunkSize);

  //set playing parameter
  delayus = 1000000/sampleRate;  // 8000Hz = 125 , 16kHz = 62 ...
  //sigmaDeltaSetup(0, sampleRate); // setup channel 0 with sampleRate
  //sigmaDeltaSetup(0, 22050); // setup channel 0 with sampleRate
  //sigmaDeltaWrite(0, 0); //initialize channel 0 to off 

  //play wav data
  while (count < length) {
    if (bitsPerSample == 16) {
      memcpy(&data16, (char*)audio + count, sizeof(data16));
      left = ((uint16_t) data16 + 32767) >> 8;
      count += sizeof(data16);
      if (numChannels == 2) count += sizeof(data16);
    } else {
      memcpy(&data8, (char*)audio + count, sizeof(data8));
      left = data8;
      count += sizeof(data8);
      if (numChannels == 2) count += sizeof(data8);
    }
    //sigmaDeltaWrite(0, left);  
    dac_output_voltage(DAC_CHANNEL_1, left);
    
    ets_delay_us(delayus);
  }
  //sigmaDeltaWrite(127, 0);
  dac_output_voltage(DAC_CHANNEL_1, 127);

}

void loop()
{
  play(Mono4Khz , sizeof(Mono4Khz));
  delay(2000);
  play(Mono8Khz , sizeof(Mono8Khz));
  delay(2000);
  play(Mono16Khz , sizeof(Mono16Khz));
  delay(2000);
  play(Mono22Khz ,sizeof(Mono22Khz));
  delay(5000);
}
