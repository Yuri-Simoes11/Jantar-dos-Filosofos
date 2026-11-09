#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <stdint.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(race, LOG_LEVEL_INF);

// Configurações
#define ITERACOES 1000
#define PRIORIDADE 5
#define STACK_SIZE 1024

// Recurso compartilhado
volatile int contador = 0;

// Sincronização
K_SEM_DEFINE(fim_sem, 0, 2);
K_MUTEX_DEFINE(contador_mutex);

// ----------------------------------------------------
// THREAD A
// ----------------------------------------------------
void thread_A(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    for (int i = 0; i < ITERACOES; i++) {

        k_mutex_lock(&contador_mutex, K_FOREVER);

        int temp = contador;
        contador = temp + 1;

        k_mutex_unlock(&contador_mutex);
    }

    LOG_INF("Thread A finalizou.");

    k_sem_give(&fim_sem);
}

// ----------------------------------------------------
// THREAD B
// ----------------------------------------------------
void thread_B(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    for (int i = 0; i < ITERACOES; i++) {

        k_mutex_lock(&contador_mutex, K_FOREVER);

        int temp = contador;
        contador = temp + 1;

        k_mutex_unlock(&contador_mutex);
    }

    LOG_INF("Thread B finalizou.");

    k_sem_give(&fim_sem);
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
void main(void)
{
    LOG_INF("=== DEMONSTRACAO DE MUTEX ===");
    LOG_INF("Contador inicial: %d", contador);

    // Aguarda a finalização das duas threads
    k_sem_take(&fim_sem, K_FOREVER);
    k_sem_take(&fim_sem, K_FOREVER);

    LOG_INF("Contador final: %d", contador);
    LOG_INF("Valor esperado: %d", ITERACOES * 2);

    if (contador == ITERACOES * 2) {
        LOG_INF("SUCESSO: contador correto!");
    } else {
        LOG_ERR("ERRO: houve perda de atualizacoes!");
    }
}
