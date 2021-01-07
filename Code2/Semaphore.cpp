//
//#include "Semaphore.hpp"
//
//// Constructs a new semaphore with a counter of 0
//Semaphore::Semaphore() : counter(0) {
//    pthread_mutex_init(&mutex, NULL);
//    pthread_cond_init(&cond, NULL);
//}
//
//// Constructs a new semaphore with a counter of val
//Semaphore::Semaphore(unsigned val) : counter(val) {
//    pthread_mutex_init(&mutex, NULL);
//    pthread_cond_init(&cond, NULL);
//}
//
//// Mark: 1 Thread has left the critical section
//void Semaphore::up() {
//    // a thread has left the critical section, so we can add to the counter, so other processes waiting can start doing work
//    pthread_mutex_lock(&mutex);
//    // now we have the lock, so we can change counter
//    counter++;
//    pthread_cond_signal(&cond);
//    // we have finished the critical section, so we unlock the mutex
//    pthread_mutex_unlock(&mutex);
//}
//
//// Block until counter >0, and mark - One thread has entered the critical section.
//// Wait until there is an available thread, and then use an available thread for the critical section
//void Semaphore::down() {
//    pthread_mutex_lock(&mutex);
//    while (counter == 0) {
//        // wait until a thread is available, the wait will stop by sending a signal to cond
//        pthread_cond_wait(&cond, &mutex);
//    }
//    // thread has become available, need to use a thread, and take out a request from the wait itemQueue
//    // now we have the lock, so we can change counter
//    counter--;
//    // we have finished the critical section, so we unlock the mutex
//    pthread_mutex_unlock(&mutex);
//}
//
//Semaphore::~Semaphore() {
//    pthread_cond_destroy(&cond);
//    pthread_mutex_destroy(&mutex);
//}




#include "Semaphore.hpp"

Semaphore::Semaphore() : counter(0), min(0), next(0) { // TODO i changed to counter(0). so start it already lock
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
    pthread_cond_broadcast(&c);  // TODO speed vs Deadlock
//    pthread_cond_signal(&c); // TODO speed vs Deadlock
    pthread_mutex_unlock(&m);
}

void Semaphore::down() {
    pthread_mutex_lock(&m);

    unsigned long id = next++;

    while (counter == 0 || id > counter+min) {
//    while (counter == 0) {
        pthread_cond_wait(&c, &m);
    }
    assert(counter!=0);
    --counter;
    ++min;
    pthread_mutex_unlock(&m);
}
