#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "WavReadEncode.h"

#include <lame/lame.h>

//Not Exposed API
bool lameencode(const char *file,int IN_SAMPLERATE,int IN_CHANNELS,int IN_SAMPLESIZE,long IN_TOTAL_SAMPLES);

bool WavReadEncode (const char* fileName)
{
	FILE *fin = fopen(fileName, "rb");
	bool bRet=false; 
	if(fin!=NULL)
	{
        //Read WAV header
        wav_header_t header;
        memset(&header,'\0',sizeof(header));
		int size=fread(&header, sizeof(header), 1, fin);
		if(size == 1)
		{
    		if((strncmp(header.WAVE,"WAVE",4) == 0) && (header.AudioFormat == 1 ) && header.NumOfChan==2)
    	    {
    			long num_samples = (8 * header.Subchunk2Size) / (header.NumOfChan * header.bitsPerSample);
    			long size_of_each_sample = (header.NumOfChan * header.bitsPerSample) / 8;
                bRet = lameencode(fileName,header.SamplesPerSec,header.NumOfChan,size_of_each_sample,num_samples);
            }
	    	else
	    	{
	        	printf("file is not supported %s and audioformat=%d and no of channels=%d\n",header.WAVE,header.AudioFormat,header.NumOfChan);
	    	}
        }
		else
		{
	    	printf("file size is less than 44 bytes which is not valid format=%d \n",size);
		}
		fclose(fin);
    }
    else
    {
		printf("File open has issue=%s",fileName);
    }
    return bRet;
}


bool lameencode(const char *file,int IN_SAMPLERATE,int IN_CHANNELS,int IN_SAMPLESIZE,long IN_TOTAL_SAMPLES)
{
    if(file==NULL)
    {
	printf("file pointer is null\n");
	return false;
    }

    FILE *pcm = fopen(file, "rb");
    int bRet=false;
    if(pcm!=NULL)
    {
    	int read, write;
	char *destfile=(char *)malloc(strlen(file)+1);
    	memset(destfile,'\0',strlen(file)+1);
        strncpy(destfile,file,strlen(file));
	char *pCh=NULL;
        pCh=strstr(destfile,".wav");
	if(pCh!=NULL)
    	{
		strncpy(pCh,".mp3",4);
    		FILE *mp3 = fopen(destfile, "wb");
		if(mp3!=NULL)
		{
			const int PCM_SIZE = 8192;
    			const int MP3_SIZE = 8192;
			short int pcm_buffer[PCM_SIZE*2];
    			unsigned char mp3_buffer[MP3_SIZE];
			long MAX_SAMPLES_READ=(PCM_SIZE*2)/IN_SAMPLESIZE;
    			long samplesLeft=IN_TOTAL_SAMPLES;
    
    			lame_t lame = lame_init();
			if(lame!=NULL)
			{
    				lame_set_in_samplerate(lame, IN_SAMPLERATE);
    				lame_set_num_channels(lame, IN_CHANNELS);
    				lame_set_brate(lame, 128);
    				lame_set_VBR(lame, vbr_off);
    				lame_set_quality(lame, 5);//Fixed Good Quality
	    			lame_set_mode(lame,0);//Fixed Stereo Mode is selected
					
				if(lame_init_params(lame)!=-1)
				{
					fseek ( pcm , 44 , SEEK_SET );
					int flushCount=0;
	    				int bufferWrite=0;
					do 
					{
	    					if(MAX_SAMPLES_READ<samplesLeft)
	    					{
        						read = fread(pcm_buffer, IN_SAMPLESIZE, MAX_SAMPLES_READ, pcm);
	        					samplesLeft=samplesLeft-MAX_SAMPLES_READ;
	    					}
	    					else
						{
		    					read = fread(pcm_buffer, IN_SAMPLESIZE, samplesLeft, pcm);
		    					samplesLeft=samplesLeft-samplesLeft;
	    					}
        					if (read == 0)
						{
	    						printf("flush count=%d\n",flushCount);
	    						flushCount++;
            						write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
						}
        					else
						{
	    						printf("buffer count=%d\n",bufferWrite);
	    						bufferWrite++;
            						write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
						}
        					fwrite(mp3_buffer, write, 1, mp3);
    					} while (read != 0);
					bRet=true;// AT this moment MP3 is created so return TRUE value set
    					lame_close(lame);
    					fclose(mp3);
				}
				else
				{
					printf("lame init params reported error\n");
				}
			}
			else
			{
				printf("lame_init call failed\n");
			}

		}
		else
		{
			printf("Not able to create file mp3 in destination folder=%s",destfile);
		}
	}
	else
	{
		printf("strstr not able to find .wav in given name of file=%s\n",file);
	}
	free(destfile);
	destfile=NULL;
    	fclose(pcm);
    }
    else
    {
	    printf("Input File fopen failed=%s\n",file);
    }

    return bRet;

}
