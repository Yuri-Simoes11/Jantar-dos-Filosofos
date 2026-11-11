#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <stdint.h>
#include <zephyr/logging/log.h>

// Configurações
#define PRIORIDADE 5
#define STACK_SIZE 1024
#define PADEIRO 1000
#define CLIENTE 1500

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
	int total_paes = 0;
	int aux = 0;

	while(1){

	aux = saldo_vitrine;

	if(saldo_vitrine < 10){
	    aux++;
		total_paes++;
		saldo_vitrine = aux;
		printk("Pao Pronto #%d        (Vitrine: %d)\n", 
			    total_paes, saldo_vitrine);
	}                     
	else{
		printk("Pao Pronto, porem vitrine cheia\n");
	}

	k_busy_wait(PADEIRO*1000);

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
	int paes_pegos = 0;
	int aux1 = 0;

	while(1){

	aux1 = saldo_vitrine;

	if(saldo_vitrine >= 1){
	    aux1--;
		paes_pegos++;
		saldo_vitrine = aux1;
		printk("Cliente Pegou Pao #%d (Vitrine: %d)\n", 
			    paes_pegos, saldo_vitrine);
	}
	else{
		printk("Cliente quer pao mas nao ha na vitrine\n");
	}

	k_msleep(CLIENTE);

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

	printk("Padaria Aberta!\n");
    // A thread principal permanece suspensa
    k_sleep(K_FOREVER);
}