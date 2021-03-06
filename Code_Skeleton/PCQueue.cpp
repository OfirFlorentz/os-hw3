#include "PCQueue.hpp"

template <typename T>
PCQueue<T>::PCQueue() : m_sem(0), m_queue(), m_lock(){
    pthread_mutex_init(&m_lock, nullptr);
}

template <typename T>
T PCQueue<T>::pop() {
    m_sem.down();
    pthread_mutex_lock(&m_lock);
    T m_obj = m_queue.front();
    m_queue.pop();
    pthread_mutex_unlock(&m_lock);
    return m_obj;
}

template <typename T>
void PCQueue<T>::push(const T& item) {
    pthread_mutex_lock(&m_lock);
    m_queue.push(item);
    pthread_mutex_unlock(&m_lock);
    m_sem.up();
}
