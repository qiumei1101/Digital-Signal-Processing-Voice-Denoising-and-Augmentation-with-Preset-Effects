// delay_intr.c

#include "audio.h"

volatile int16_t audio_chR=0;
volatile int16_t audio_chL=0;

#define DELAY_BUF_SIZE 32000 //also recording time for now, 3s@8kHz
#define REC_TIME 16000 // keep delay betwn original and modified signal 1s@8kHz
#define WAIT_TIME 24000



 int16_t buffer[DELAY_BUF_SIZE];
 int16_t recording[REC_TIME];
 int16_t i = 0;
 int16_t bptr = 0;
 int16_t read_ptr = 0;
 int16_t rec_ptr=0; //recording ptr
 int16_t wptr = 0;
 int16_t wait_value=0;
 
 
 void I2S_HANDLER(void) {
	 
	 int16_t delayed_sample;
	 int16_t audio_out_chL = 0;

	 audio_IN = i2s_rx();
	 audio_chL = (audio_IN & 0x0000FFFF);
	 audio_chR = ((audio_IN >>16)& 0x0000FFFF);
	 if (wptr==0){wait_value=DELAY_BUF_SIZE;}
	 else {wait_value=WAIT_TIME;}
	 if(bptr<wait_value){
			//delayed_sample = buffer[bptr]; //read from buffer containing 0, only 0 can be assigned too
			delayed_sample = 0;
			bptr=bptr+1;
		}
		else{ 
			wptr=1;
			delayed_sample=recording[read_ptr]; //now read from downsampled recording
		
	      //read_ptr=(read_ptr+1)%REC_TIME;
			read_ptr=read_ptr+1;
			if (read_ptr==REC_TIME){
				bptr=0;
				//wptr=1;
				read_ptr=0;
				
			}				
		}
//		else {
//			delayed_sample=0;
//			count+=1;
//			if (count==12000){
//			wptr=0;
//				count=0;
//			}
//		}
	 if (i%2==0) //read every 2 samples, downsampling by 2
	{

		 recording[rec_ptr] = audio_chL; //circular buffer
		 rec_ptr=(rec_ptr+1)%REC_TIME;
			//read_ptr=(read_ptr+1)%REC_TIME; // delayed sample will read same values for each sample in recording
	 
	}
		 
		 
		
		 
		 //audio_out_chL = delayed_sample + audio_chL;
	 audio_out_chL = delayed_sample;
		 //buffer[bptr] = audio_chL;
		 

		 audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL & 0x0000FFFF); //audio_OUT already declared in audio.h
		//audio_OUT= audio_out_chL;

		 
		 //bptr=(bptr+1)%DELAY_BUF_SIZE;
		 //bptr=(bptr+1);
	
		

	//i = (i+1) % (2*REC_TIME); // for 1s recording time, @downsampling by 2, we need to read 2*REC_TIME input
	i=(i+1)%32000;
		//i = (i+1) % DELAY_BUF_SIZE;
	i2s_tx(audio_OUT); //audio_OUT already declared in audio.h
	
	//if (i%2==0){i2s_tx(audio_OUT);}
		
 
 }

 int main(void)
 {
 audio_init ( hz8000, dmic_in, intr, I2S_HANDLER);
 

 while(1){}
 }
 
