#include <tonecfg_csl.h>
#include <math.h>
#include <dsplib.h>
#include <tms320.h>
#include "tonecfg.h"
#include "dsk5416.h"
#include "dsk5416_pcm3002.h"
#include "dsk5416_led.h"

//Размер вектора, содержащего комплексные числа, действительная часть - на четных местах,
//мнимая - на нечетных
#define SIGNAL_COMPL_SIZE 32
#define SIGNAL_COMPL_HALF 16 // SIGNAL_COMPL_SIZE/2

//Размер вектора, содержащего только действительные числа
#define SIGNAL_REAL_SIZE 16
#define SIGNAL_REAL_HALF 8 //SIGNAL_REAL_SIZE/2

#define SCALE_FACTOR 0 //Параметр Scale Factor для функций dsplib
 
#define LOUD_OFFSET 4 //Уменьшение громкости при воспроизведении

typedef DATA signal_real_t[SIGNAL_REAL_SIZE]; //Вектор, содержащий действительные числа
typedef DATA signal_complex_t[SIGNAL_COMPL_SIZE]; //Вектор, содержащий комплексные числа

#pragma DATA_SECTION(signal, ".fftdata1") //Выравнивание вектора (требование dsplib)
signal_real_t signal = {0}; //Входной сигнал с кодека

#pragma DATA_SECTION(spectrum, ".fftdata2") //Выравнивание вектора (требование dsplib)
signal_complex_t spectrum = {0}; //Результат преобразования Фурье

//Конфигурация кодека
DSK5416_PCM3002_Config setup = {
    0x010d, // Set-Up Reg 0 - Left  channel DAC attenuation
    0x010d, // Set-Up Reg 1 - Right channel DAC attenuation
    0x0000, // Set-Up Reg 2 - Various ctl e.g. power-down modes
    0x0000  // Set-Up Reg 3 - Codec data format control
};

//Чтение данных из кодека, т.е. получение входного действительного сигнала
void pcm_read(DSK5416_PCM3002_CodecHandle hCodec, signal_real_t data)
{
	int i;
	for (i = 0; i < SIGNAL_REAL_SIZE; i++)
	{
		while(!DSK5416_PCM3002_read16(hCodec, &data[i]));
	}
}

//Обновление сигнала "на лету": одновременно воспроизводим 
//и сразу считываем новое значение из кодека
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

//Прямое БПФ с использованием свойства симметрии преобразования действительного сигнала
void rfft_symm(signal_real_t signal, signal_complex_t spectrum)
{
	int i;
	cbrev(signal, signal, SIGNAL_REAL_HALF); //Bit-reversal (требование dsplib)
	rfft(signal, SIGNAL_REAL_SIZE, SCALE_FACTOR); //Прямое БПФ для действительного сигнала
	for (i = 0; i < SIGNAL_REAL_SIZE; i++)
	{
		spectrum[i] = signal[i];
		spectrum[i+1] = signal[i+1];
		spectrum[SIGNAL_COMPL_SIZE-i] = signal[i];
		spectrum[SIGNAL_COMPL_SIZE-i+1] = -signal[i+1];
	}
}

//Обратное БПФ для комплексного сигнала с выделением действительного сигнала
void cifft_to_real(signal_complex_t spectrum, signal_real_t signal)
{
	int i;	
	cbrev(spectrum, spectrum,SIGNAL_COMPL_HALF); //Bit-reversal (требование dsplib)
	cifft(spectrum, SIGNAL_COMPL_HALF, SCALE_FACTOR); //Обартное БПФ для комплексного сигнала
	signal[0] = spectrum[0];
 	for (i = 1; i < SIGNAL_REAL_SIZE; i++)
	{
		signal[i] = spectrum[i*2];
	}
}

//Обработка полученного спектра: простой сдвиг частот
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
	//Инициализация кодека
	DSK5416_PCM3002_CodecHandle hCodec = DSK5416_PCM3002_openCodec( 0, &setup );

	//Инициализация DIP-переключателя
	DSK5416_DIP_init();

	//Инициализация входного сигнала
	pcm_read(hCodec, signal);

	while (1)
	{
		rfft_symm(signal, spectrum); //БПФ
		if (DSK5416_DIP_get(0) == 0) //Если переключатель находится в нужном положении
		{
			freq_shift(spectrum); //Сдвиг частот
		}
		cifft_to_real(spectrum, signal); //ОБПФ

		//Воспроизводим полученный сигнал и считываем новый сигнал с кодека
		pcm_refresh_signal(hCodec, signal);
	}
}

void main()
{
	//Инициализация Chip Support Library (CSL)
	CSL_init();
	cslCfgInit();
	
	//Инициализация Board Support Library (BSL)
    DSK5416_init();
}
