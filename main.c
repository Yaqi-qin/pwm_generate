#include "driverlib.h"
#include "msp430.h"

//******************************************************************************
//                  generate pwm by compare mode
//
//TA0.1 was initialized to 75%
//The clock source is SMCLK(SMCLK = MCLK = TACLK = default DCO ~1048576Hz)
//The divider coefficient is 1
//The load value is 512-1
//Therefore, the resulting PWM frequency is 1MHz/1/512，pwm period is ~500us
//
//******************************************************************************
//#define TIMER_PERIOD 512
//void Timer_A_PWM_Init(void)
//{
//    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2);  //复用P1.2输出
//
//    Timer_A_initUpModeParam htim = {0};
//    htim.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
//    htim.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;  //
//    htim.timerPeriod = TIMER_PERIOD - 1;  //Write the initial value to CCR0
//    htim.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
//    htim.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
//    htim.timerClear = TIMER_A_DO_CLEAR;
//    htim.startTimer = true;
//    Timer_A_initUpMode(TIMER_A0_BASE, &htim);
//
//
//    Timer_A_initCompareModeParam htim_PWM = {0};
//    htim_PWM.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
//    htim_PWM.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;  //OUTMOD_7
//
//    htim_PWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;  //CCR1<-->P1.2
//    htim_PWM.compareValue = 0.75*TIMER_PERIOD;  //Write the compare value to CCR1
//    //Initialize compare mode to generate PWM1
//    Timer_A_initCompareMode(TIMER_A0_BASE, &htim_PWM);
//
//}


//******************************************************************************
//                          generate pwm by pwm mode
//pwm模式可能是一种封装好的比较模式
//pwm模式默认增计数模式
//******************************************************************************
#define TIMER_PERIOD 512
void Timer_A_PWM_Init()
{
    Timer_A_outputPWMParam htim = {0};

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2);  //P1DIR |= BIT2;P1SEL |= BIT2;

    htim.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;  //TA0CTL = TASSEL_2
    htim.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    htim.timerPeriod = TIMER_PERIOD - 1;
    htim.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;  //CCR1<-->P1.2
    htim.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;  //OUTMOD_7
    htim.dutyCycle = TIMER_PERIOD*0.25 ;  //Write the compare value to CCR1
    Timer_A_outputPWM(TIMER_A0_BASE, &htim);
}



//******************************************************************************
//                   generate pwm by compare mode(register level)
//比较模式时，要有options select操作
//
//******************************************************************************
//WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//P1DIR |= BIT2;                       // P1.2 output
//P1SEL |= BIT2;                       // P1.2 options select
//
//TA0CCR0 = 512-1;                          // PWM Period
//TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
//TA0CCR1 = 384;                            // CCR1 PWM duty cycle
//TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
//
//__bis_SR_register(LPM0_bits);             // Enter LPM0
//__no_operation();
//


void main(void) {
    Timer_A_PWM_Init();

    WDT_A_hold(WDT_A_BASE);

}


//aha
