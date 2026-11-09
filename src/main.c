#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <stdint.h>
#include <zephyr/logging/log.h>

// Configurações
#define PRIORIDADE 5
#define STACK_SIZE 1024

// Recurso compartilhado
volatile int saldo_vitrine = 0;

// ----------------------------------------------------
// THREAD A
// ----------------------------------------------------
void thread_A(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

	while(1){

	if(saldo_vitrine < 10){
	    saldo_vitrine++;
		printk("Pao Pronto        (Vitrine: %d)\n", saldo_vitrine);
	}      
	else{
		printk("Pao Pronto, porem vitrine cheia\n");
	}

	k_msleep(1000);

    }

}

// ----------------------------------------------------
// THREAD B
// ----------------------------------------------------
void thread_B(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

	while(1){

	if(saldo_vitrine >= 1){
	    saldo_vitrine--;
		printk("Cliente Pegou Pao (Vitrine: %d)\n", saldo_vitrine);
	}
	else{
		printk("Cliente quer pao mas nao ha na vitrine\n");
	}

	k_msleep(1500);

	}

}

// ----------------------------------------------------
// DEFINIÇÃO DAS THREADS
// ----------------------------------------------------
K_THREAD_DEFINE(a_tid, STACK_SIZE, thread_A,
                NULL, NULL, NULL,
                PRIORIDADE, 0, 0);

K_THREAD_DEFINE(b_tid, STACK_SIZE, thread_B,
                NULL, NULL, NULL,
                PRIORIDADE, 0, 0);

// ----------------------------------------------------
// FUNÇÃO PRINCIPAL
// ----------------------------------------------------
void main(void) {

	printk("Padaria Aberta: %d pães\n", saldo_vitrine);
    // A thread principal permanece suspensa
    k_sleep(K_FOREVER);
}