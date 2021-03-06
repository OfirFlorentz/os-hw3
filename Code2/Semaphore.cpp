
#include "Semaphore.hpp"

Semaphore::Semaphore() : counter(0), min(0), next(0) {
    pthread_mutex_init(&m, nullptr);
    pthread_cond_init(&c, nullptr);
}

Semaphore::Semaphore(unsigned val) : counter((unsigned long)val), min(0), next(0) {
    pthread_mutex_init(&m, nullptr);
    pthread_cond_init(&c, nullptr);
}

void Semaphore::up() {
    pthread_mutex_lock(&m);
    ++counter;
    pthread_cond_broadcast(&c);
    pthread_mutex_unlock(&m);
}

void Semaphore::down() {
    pthread_mutex_lock(&m);

    unsigned long id = next++;

    while (counter == 0 || id > counter+min) {
        pthread_cond_wait(&c, &m);
    }
    assert(counter!=0);
    --counter;
    ++min;
    pthread_mutex_unlock(&m);
}
