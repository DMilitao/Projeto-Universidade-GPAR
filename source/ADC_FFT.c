#include <stdio.h>
#include <math.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include "fsl_tpm.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include <arm_math.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Define o módulo */
#define BOARD_TPM TPM2
/* Define as Interrupções */
#define BOARD_TPM_IRQ_NUM TPM2_IRQn
#define BOARD_TPM_HANDLER TPM2_IRQHandler
/* Define o clock do timer */
#define TPM_SOURCE_CLOCK_TIMER (CLOCK_GetFreq(kCLOCK_PllFllSelClk)/4)

//Define o endereço e canal do ADC
#define DEMO_ADC16_BASE ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 0U

//Define configurações do PWM
#define BOARD_TPM_BASEADDR TPM1
#define BOARD_TPM_CHANNEL kTPM_Chnl_1

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl1InterruptEnable
#define TPM_CHANNEL_FLAG kTPM_Chnl1Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM1_IRQn
#define TPM_LED_HANDLER TPM1_IRQHandler

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)




/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variaveis
 ******************************************************************************/
    //Timer
	uint32_t auxtimer = 0;
	uint32_t auxtimer2 = 0;
    uint32_t secondLoop = 5U; //Define a quantidade de Interrupções
    tpm_config_t tpmInfo; //Interrupção do timer
    uint32_t pwmflag=0;

    //ADC
    adc16_config_t adc16ConfigStruct;
	adc16_channel_config_t adc16ChannelConfigStruct;
	uint32_t leiturainst = 0;

	uint32_t saida = 0;
	uint16_t auxleit = 0;
	uint32_t altura = 0;
    int8_t auxmatlab = 0;
    uint32_t max = 0;
    uint32_t imax = 0;

    //FFT
    arm_rfft_instance_q15 fft_config;
    q15_t sinal_freq[512];
    q15_t mag_freq[256];
    q15_t leitura[256];
/*******************************************************************************
 * Funções Extras
 ******************************************************************************/
void InicializaFFT(void){
	fft_config.ifftFlagR = 0;
	fft_config.bitReverseFlagR = 0;
	arm_rfft_init_q15(&fft_config, 256, 0, 0);

}

int FFT(void){
	arm_rfft_q15(&fft_config, &leitura[0], &sinal_freq[0]);
	arm_cmplx_mag_squared_q15(&sinal_freq[0], &mag_freq[0], 256);
	max = mag_freq[1];
	imax = 1;
	for(uint16_t i = 2; i<=256; i++){
		if(mag_freq[i]>max){
			max = mag_freq[i];
			imax = i;
		}
	}

	return imax;
}

void InicializaTimer(uint16_t tempo){
	   TPM_GetDefaultConfig(&tpmInfo);
	   tpmInfo.prescale = kTPM_Prescale_Divide_4;
	   TPM_Init(BOARD_TPM, &tpmInfo);
	   TPM_SetTimerPeriod(BOARD_TPM, USEC_TO_COUNT(tempo, TPM_SOURCE_CLOCK_TIMER));
	   TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
	   EnableIRQ(BOARD_TPM_IRQ_NUM);
	   TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);
}
void InicializaADC(void){
	//Inicializa com as configurações padrões ADC
	ADC16_GetDefaultConfig(&adc16ConfigStruct);
	//Muda a resolução para 16 Bits
    adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	//Inicializa o conversor
	ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
	//Desabilita o trigger pelo hardware, apenas por software
	ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false);
	adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;  //Define o pino
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false; //Desabilita interrupção ao fim da conversão
	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
		adc16ChannelConfigStruct.enableDifferentialConversion = false;
	#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}

int LerADC(void){
	ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
	//Verifica se a leitura foi completa
	while (0U == (kADC16_ChannelConversionDoneFlag &
			ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
	{
		}

	return ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
}

/*******************************************************************************
 * Código Principal
 ******************************************************************************/
int main(void){

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Inicializa configuração do timer, argumento é o tempo em microssegundos */

    InicializaADC();

    PRINTF("\r\nInicializando o vetor Leitura\r\n");

    GETCHAR();
    InicializaFFT();
    InicializaTimer(1000); //Interrupção

    while (true){
    	if(pwmflag == true){
    		pwmflag = false;

    	}
    }
}

/*******************************************************************************
 * Interrupções
 ******************************************************************************/
void BOARD_TPM_HANDLER(void){
    /* Limpa a flag da Interrupção.*/
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    auxtimer++;

    if (auxtimer>=secondLoop){
        	auxtimer2++;
    		pwmflag = true;
    		auxtimer = 0;
            leitura[auxtimer2] = LerADC();
        	if(auxtimer2>256){
        		auxtimer=0;
    			PRINTF("Frequencia: %d", FFT());
        	}
        }
}
