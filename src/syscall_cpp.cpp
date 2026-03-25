//
// Created by os on 5/16/24.
//
#include"../h/syscall_cpp.hpp"
void* operator new (size_t n){
    return mem_alloc(n);
}
void *operator new[](size_t n){
    return mem_alloc(n);
}
void operator delete (void* p){
    mem_free(p);
}
void operator delete[](void *p)
{
    mem_free(p);
}
void Thread::wrapper(void* thread) {
    if(thread) ((Thread*)thread)->run();
}
Thread::Thread(void (*body)(void *), void *arg) {
    this->body=body;
    this->arg=arg;
    //thread_createCpp(&myHandle,body,arg);
}

Thread::~Thread() {}

int Thread::start() {
    return thread_create(&myHandle,this->body,this->arg);
    //::start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

Thread::Thread() {
    this->body=&wrapper;
    this->arg=this;
    //thread_createCpp(&myHandle,&wrapper,this);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t time) {
    return sem_timedwait(myHandle,time);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

void PeriodicThread::terminate() {
    this->period=-1;
}

PeriodicThread::PeriodicThread(time_t period) :period(period) {
    Thread(wrapper1,this);
}

void PeriodicThread::run () {
    while(this->period!=(size_t)(-1)){
        PeriodicThread::periodicActivation();
        Thread::sleep(this->period);
    }
}

void PeriodicThread:: wrapper1(void* thread){
    if(thread) ((PeriodicThread*)thread)->run();
}

