#include "criticalinterrupt.h"

// Критические прерывания

void criticalInterruptInit(void) {
	printf("Critical interrupt init start!\n");
	printf("Critical interrupt init Ok!\n");
}

void NMI_Handler(void) {
	printf("e_NMI \n");
	NVIC_SystemReset();	// Системный сброс
}

void HardFault_Handler(void) {
	printf("e_HF \n");
	NVIC_SystemReset();	// Системный сброс
}

void SVC_Handler(void) {
	printf("e_SVC \n");
	NVIC_SystemReset();	// Системный сброс
}

void PendSV_Handler(void) {
	printf("e_PendSV \n");
	NVIC_SystemReset();	// Системный сброс
}

void MemManage_Handler(void){
	printf("e_MemManage \n");
	NVIC_SystemReset();	// Системный сброс
}

void BusFault_Handler(void) {
	printf("e_BusFault \n");
	NVIC_SystemReset();	// Системный сброс
}

void UsageFault_Handler(void) {
	printf("e_UsageFault \n");
	NVIC_SystemReset();	// Системный сброс
}
