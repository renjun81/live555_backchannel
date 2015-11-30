// testLive555.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "G711Utils.h"

#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

#pragma comment(lib, "winmm.lib")


short int waveIn[44100 * 10];


void StartRecord()
{
	const int NUMPTS = 44100 * 10;   // 3 seconds
	int sampleRate = 44100;
	// 'short int' is a 16-bit type; I request 16-bit samples below
	// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit     types

	HWAVEIN      hWaveIn;
	MMRESULT result;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels = 1;                    //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;      // 44100
	pFormat.nAvgBytesPerSec = sampleRate * 2;   // = nSamplesPerSec * n.Channels *    wBitsPerSample/8
	pFormat.nBlockAlign = 2;                  // = n.Channels * wBitsPerSample/8
	pFormat.wBitsPerSample = 16;              //  16 for high quality, 8 for telephone-grade
	pFormat.cbSize = 0;

	// Specify recording parameters
	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	WAVEHDR      WaveInHdr;
	// Set up and prepare header for input
	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = NUMPTS * 2;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Insert a wave input buffer
	result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	// Commence sampling input
	result = waveInStart(hWaveIn);


	cout << "recording..." << endl;

	Sleep(10 * 1000);
	// Wait until finished recording

	waveInClose(hWaveIn);
}

void PlayRecord(void* pcm, long dwBufferLength, int wBitsPerSample, int sampleRate = 8000, int nChannels = 1)
{
	float playingTime = dwBufferLength / (float)(sampleRate * nChannels * wBitsPerSample / 8) * 1000;

	HWAVEIN  hWaveIn;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
	pFormat.nChannels = nChannels;            //  1=mono, 2=stereo
	pFormat.nSamplesPerSec = sampleRate;      // 44100
	pFormat.wBitsPerSample = wBitsPerSample;  //  16 for high quality, 8 for telephone-grade
	//nAvgBytesPerSec = nSamplesPerSec * n.Channels * wBitsPerSample/8
	pFormat.nAvgBytesPerSec = pFormat.nSamplesPerSec * pFormat.nChannels * pFormat.wBitsPerSample / 8;
	//nBlockAlign = n.Channels * wBitsPerSample/8
	pFormat.nBlockAlign = pFormat.nChannels * pFormat.wBitsPerSample / 8;
	
	pFormat.cbSize = 0;

	// Specify recording parameters

	waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	WAVEHDR      WaveInHdr;
	// Set up and prepare header for input
	WaveInHdr.lpData = (LPSTR)pcm;
	WaveInHdr.dwBufferLength = dwBufferLength;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	HWAVEOUT hWaveOut;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);
	waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr)); // Playing the data
	
	Sleep((int)playingTime); //Sleep for as long as there was recorded

	waveInClose(hWaveIn);
	waveOutClose(hWaveOut);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//StartRecord();

	cout << "Playing...." << endl;

	FILE* fp = fopen("beep16.pcm", "r");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *data = (char*)malloc(fsize);
	fread(data, fsize, 1, fp);
	fclose(fp);

	PlayRecord(data, 2 * fsize, 16, 8000, 1);

	

	int sampleRate = 8000;
	int wBitsPerSample = 16;
	int nChannels = 1;
	void* decoded = decodeMuLawStream((int8_t*)data, fsize);
	//PlayRecord(decoded, 2 * fsize, wBitsPerSample, sampleRate, nChannels);

	return 0;
}

