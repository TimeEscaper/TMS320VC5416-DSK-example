#include <tonecfg_csl.h>
#include <math.h>
#include <dsplib.h>
#include <tms320.h>
#include "tonecfg.h"
#include "dsk5416.h"
#include "dsk5416_pcm3002.h"
#include "dsk5416_led.h"

//������ �������, ����������� ����������� �����, �������������� ����� - �� ������ ������,
//������ - �� ��������
#define SIGNAL_COMPL_SIZE 32
#define SIGNAL_COMPL_HALF 16 // SIGNAL_COMPL_SIZE/2

//������ �������, ����������� ������ �������������� �����
#define SIGNAL_REAL_SIZE 16
#define SIGNAL_REAL_HALF 8 //SIGNAL_REAL_SIZE/2

#define SCALE_FACTOR 0 //�������� Scale Factor ��� ������� dsplib
 
#define LOUD_OFFSET 4 //���������� ��������� ��� ���������������

typedef DATA signal_real_t[SIGNAL_REAL_SIZE]; //������, ���������� �������������� �����
typedef DATA signal_complex_t[SIGNAL_COMPL_SIZE]; //������, ���������� ����������� �����

#pragma DATA_SECTION(signal, ".fftdata1") //������������ ������� (���������� dsplib)
signal_real_t signal = {0}; //������� ������ � ������

#pragma DATA_SECTION(spectrum, ".fftdata2") //������������ ������� (���������� dsplib)
signal_complex_t spectrum = {0}; //��������� �������������� �����

//������������ ������
DSK5416_PCM3002_Config setup = {
    0x010d, // Set-Up Reg 0 - Left  channel DAC attenuation
    0x010d, // Set-Up Reg 1 - Right channel DAC attenuation
    0x0000, // Set-Up Reg 2 - Various ctl e.g. power-down modes
    0x0000  // Set-Up Reg 3 - Codec data format control
};

//������ ������ �� ������, �.�. ��������� �������� ��������������� �������
void pcm_read(DSK5416_PCM3002_CodecHandle hCodec, signal_real_t data)
{
	int i;
	for (i = 0; i < SIGNAL_REAL_SIZE; i++)
	{
		while(!DSK5416_PCM3002_read16(hCodec, &data[i]));
	}
}

//���������� ������� "�� ����": ������������ ������������� 
//� ����� ��������� ����� �������� �� ������
void pcm_refresh_signal(DSK5416_PCM3002_CodecHandle hCodec, signal_real_t signal)
{
	int i;
	for (i = 0; i < SIGNAL_REAL_SIZE; i++)
	{
		while(!DSK5416_PCM3002_write16(hCodec, signal[i] >> LOUD_OFFSET));
		while(!DSK5416_PCM3002_write16(hCodec, signal[i] >> LOUD_OFFSET));
		while(!DSK5416_PCM3002_read16(hCodec, &signal[i]));
	}
}

//������ ��� � �������������� �������� ��������� �������������� ��������������� �������
void rfft_symm(signal_real_t signal, signal_complex_t spectrum)
{
	int i;
	cbrev(signal, signal, SIGNAL_REAL_HALF); //Bit-reversal (���������� dsplib)
	rfft(signal, SIGNAL_REAL_SIZE, SCALE_FACTOR); //������ ��� ��� ��������������� �������
	for (i = 0; i < SIGNAL_REAL_SIZE; i++)
	{
		spectrum[i] = signal[i];
		spectrum[i+1] = signal[i+1];
		spectrum[SIGNAL_COMPL_SIZE-i] = signal[i];
		spectrum[SIGNAL_COMPL_SIZE-i+1] = -signal[i+1];
	}
}

//�������� ��� ��� ������������ ������� � ���������� ��������������� �������
void cifft_to_real(signal_complex_t spectrum, signal_real_t signal)
{
	int i;	
	cbrev(spectrum, spectrum,SIGNAL_COMPL_HALF); //Bit-reversal (���������� dsplib)
	cifft(spectrum, SIGNAL_COMPL_HALF, SCALE_FACTOR); //�������� ��� ��� ������������ �������
	signal[0] = spectrum[0];
 	for (i = 1; i < SIGNAL_REAL_SIZE; i++)
	{
		signal[i] = spectrum[i*2];
	}
}

//��������� ����������� �������: ������� ����� ������
void freq_shift(signal_complex_t spectrum)
{
	DATA last_real, last_imag;
	DATA buff;
	int i;
	last_real = spectrum[0];
	last_imag = spectrum[1];
	for (i = 2; i < SIGNAL_COMPL_SIZE; i*=2)
	{
		buff = spectrum[i];
		spectrum[i] = last_real;
		last_real = buff;

		buff = spectrum[i+1];
		spectrum[i+1] = last_imag;
		last_imag = buff;
	}
	spectrum[0] = last_real;
	spectrum[1] = last_imag;
}

void UserTask()
{
	//������������� ������
	DSK5416_PCM3002_CodecHandle hCodec = DSK5416_PCM3002_openCodec( 0, &setup );

	//������������� DIP-�������������
	DSK5416_DIP_init();

	//������������� �������� �������
	pcm_read(hCodec, signal);

	while (1)
	{
		rfft_symm(signal, spectrum); //���
		if (DSK5416_DIP_get(0) == 0) //���� ������������� ��������� � ������ ���������
		{
			freq_shift(spectrum); //����� ������
		}
		cifft_to_real(spectrum, signal); //����

		//������������� ���������� ������ � ��������� ����� ������ � ������
		pcm_refresh_signal(hCodec, signal);
	}
}

void main()
{
	//������������� Chip Support Library (CSL)
	CSL_init();
	cslCfgInit();
	
	//������������� Board Support Library (BSL)
    DSK5416_init();
}
