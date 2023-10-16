/*
PMPS-Automated-Hand-Sanitiser-Dispenser
Jasmina Piric
Omar Brbutovic

Connections:
PA2 - RX
PA3 - TX
PC5 - Water level sensor S
PD10 - Relay IN1
PD12 - HCSR-04 Trigger
PD13 - Echo
PE0 - LED Green
PE1 - LED Yellow
PE2 - LED Red

*/
#include "adc.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

void Init();
void HCSR04();
void Relay();
void WaterLevel();
void isEmpty();
void isOff();

volatile uint32_t counter;
uint32_t distance;

uint32_t adc_value;
uint32_t nivo;

uint8_t buttonState = 0;

int main(void) {

  Init(); // Initialize all configured peripherals

  printUSART2("-> System running... \n");
  while (1) {
    // ON/OFF toggle
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
      printUSART2("Button pressed \n");
      buttonState = !buttonState;
      delay_ms(100);
    }

    if (buttonState) {
      isOff();
    } else {
      // Set known state to relay (OFF)
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
      HCSR04();
      Relay();
      WaterLevel();
    }
  }
}

void Init() {
  // Initialize hardware abstraction layer (HAL)
  HAL_Init();

  // Initialize USART2 communication at 921600 baud rate
  initUSART2(921600);

  // Initialize Analog-to-Digital Converter (ADC)
  initADC1();

  // Trigger pin (PD12) for HCSR-04
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Echo pin (PD13) for HCSR-04
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Relay pin (PD10)
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Initialize GPIO pins for LEDs (PE0, PE1, PE2)
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // Initialize push button on PA0
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void HCSR04() {
  counter = 0;

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0x01);
  delay_us(10);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0x00);

  while (!HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13))
    ;

  while (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)) {
    counter++;
  }

  distance = counter / 470;

  delay_ms(100);
  printUSART2("-> Distance [%d]  \n", distance);
}

void Relay() {
  // Activate pump if the distance is less than or equal to 10 cm
  if (distance <= 10) {
    printUSART2("-> Activate pump \n");
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 0);
    delay_ms(1000);

    // Block pump for 2 seconds
    printUSART2(" \n -> Block pump \n");
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, 1);
    delay_ms(2000);
  }
}

void WaterLevel() {
  // Read ADC value for water level
  adc_value = getADC();
  nivo = (adc_value * 3000) / 4095;
  printUSART2("-> ADC: Value V[%d]\n", nivo);

  // Turn on the Green LED
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0x01);

  // Check water level and update LEDs accordingly
  if (nivo >= 2000) {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0x01);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0x00);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, 0x00);
    printUSART2("Green \n");
  } else if (nivo < 1900 && nivo >= 1000) {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0x00);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0x01);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, 0x00);
    printUSART2("Yellow \n");
  } else if (nivo < 900) {
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0x00);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0x00);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, 0x01);
    printUSART2("RED \n");
    isEmpty();
  }
}

// Continuously check and handle low water level condition
void isEmpty() {
  while (nivo <= 300) {
    printUSART2("Do nothing is empty \n");
    delay_ms(100);
    WaterLevel();
  }
}

// Turn off the system
void isOff() {
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, 0x00);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, 0x00);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, 0x01);
  printUSART2("OFF \n");
  delay_ms(300);
}
