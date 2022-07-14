#include "Main.h"
#include "SSD.h"
#include "Timer.h"
#include "Time.h"
#include "Display.h"
#include "SW.h"


/* SSD_Update period */
#define DISP_PERIOD_MS   (20)
#define DISP_SECOND_BLINK_MS (1000)

static tByte SecondsDotState = 0;
static tWord DISP_blink_count = 0;

static void DISPToggleSecondsState(void);
static void DISP_BlinkSeconds(tTIM_Time *time);
static void DISP_BlinkHours(tTIM_Time *time);
static void DISP_BlinkMinutes(tTIM_Time *time);


void DISP_Init(void)
{
        SSD_Init(SSD_MINUTES_UNITS);
        SSD_Init(SSD_MINUTES_TENS);
        SSD_Init(SSD_HOURS_UNITS);
        SSD_Init(SSD_HOURS_TENS);

        SSD_SetValue(SSD_MINUTES_UNITS, SSD_0);
        SSD_SetValue(SSD_MINUTES_TENS,SSD_0);
        SSD_SetValue(SSD_HOURS_UNITS, SSD_0);
        SSD_SetValue(SSD_HOURS_TENS, SSD_0);
        /* Initialize TIM_LastTime */
}
void DISP_Update(void)
{
    static tWord DISP_counter = 0;


    tTIM_Time TIM_Time;
    tTIM_State TIM_State;

    /* Check if it is time for the DISP_Update to run */
    DISP_counter += TMR_TICK_MS;

    if (DISP_counter != DISP_PERIOD_MS)
    {
        return;
    }
    DISP_counter = 0;

    TIM_GetTime(&TIM_Time);

    /* Set SSD values based on the new time value */

    DISP_blink_count += DISP_PERIOD_MS;

    /* Blink the correct field */
    if (DISP_blink_count != DISP_SECOND_BLINK_MS)
    {

        return;
    }

    DISP_blink_count = 0;

    TIM_State = TIM_GetState();
    switch (TIM_State)
    {

        case TIM_NORMAL:
            /* Blink seconds dot  */
            DISP_BlinkSeconds(&TIM_Time);
            break;
        case TIM_SET_HOURS:
            /* Blink hours */
            DISP_BlinkHours(&TIM_Time);
            break;
        case TIM_SET_MINUTES:
            /* Blink minutes */
            DISP_BlinkMinutes(&TIM_Time);
            break;
        /* Should not be here */
        default:
            break;
    }

}
static void DISPToggleSecondsState(void)
{

    if (SecondsDotState == SSD_OFF)
    {
       SSD_SetDotState(SSD_ON);
       SecondsDotState = SSD_ON;
    }else
    {
        SSD_SetDotState(SSD_OFF);
        SecondsDotState = SSD_OFF;
    }
}
static void DISP_BlinkSeconds(tTIM_Time *time)
{
    SSD_SetValue(SSD_MINUTES_UNITS, time-> minutes %10);
    SSD_SetValue(SSD_MINUTES_TENS, time->minutes /10);
    SSD_SetValue(SSD_HOURS_UNITS, time->hours %10);
    SSD_SetValue(SSD_HOURS_TENS, time->hours /10);
    DISPToggleSecondsState();
}
static void DISP_BlinkHours(tTIM_Time *time)
{
    SecondsDotState = 0;
    DISP_blink_count = 0;

    if (SSD_GetValue(SSD_HOURS_UNITS) == SSD_NULL)
    {
        SSD_SetValue(SSD_HOURS_UNITS, time->hours %10);
        SSD_SetValue(SSD_HOURS_TENS, time->hours /10);
    } else
    {
        SSD_SetValue(SSD_HOURS_UNITS, SSD_NULL);
        SSD_SetValue(SSD_HOURS_TENS, SSD_NULL);
    }
    SSD_SetValue(SSD_MINUTES_UNITS, time-> minutes %10);
    SSD_SetValue(SSD_MINUTES_TENS, time->minutes /10);
    SSD_SetDotState(SSD_OFF);
}

static void DISP_BlinkMinutes(tTIM_Time *time)
{
    SecondsDotState = 0;
    DISP_blink_count = 0;

    if (SSD_GetValue(SSD_MINUTES_UNITS) == SSD_NULL)
    {
        SSD_SetValue(SSD_MINUTES_UNITS, time->minutes %10);
        SSD_SetValue(SSD_MINUTES_TENS, time->minutes /10);
    } else
    {
        SSD_SetValue(SSD_MINUTES_UNITS, SSD_NULL);
        SSD_SetValue(SSD_MINUTES_TENS, SSD_NULL);
    }
    SSD_SetValue(SSD_HOURS_UNITS, time-> hours %10);
    SSD_SetValue(SSD_HOURS_TENS, time->hours /10);

    SSD_SetDotState(SSD_OFF);
}
