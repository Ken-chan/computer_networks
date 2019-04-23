#include <pthread.h>
#include <iostream>
#include <queue>
#include <unistd.h>
//------------------------------------------------------

class job
{
    int jobID;

    static int jobsFin;
    static pthread_mutex_t  jobLock;

public:
    job(int id) : jobID(id) { ; }
    virtual ~job(){ ; }

    virtual void work()
    {
        std::cout << "JOB: " << jobID << " starts!\n";
        sleep(1);

        pthread_mutex_lock(&jobLock);
        jobsFin++;
        pthread_mutex_unlock(&jobLock);
    }
};
int job::jobsFin = 0;
pthread_mutex_t job::jobLock = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------------------

class thread_pool
{
    std::queue<job*> Q;
    int thN;
    pthread_t* threads;

    pthread_mutex_t qLock;
    pthread_cond_t  qCond;

    static void* thread_func(void* arg)
    {
        thread_pool* self = (thread_pool*)arg;
        job* ajob = NULL;
        while( self->loadJob(ajob) ){
            ajob->work();
            delete ajob;
            ajob = NULL;
        }
    }

    bool loadJob(job*& ajob)
    {
        pthread_mutex_lock(&qLock);
        while( Q.empty() )
            pthread_cond_wait(&qCond, &qLock);
        ajob = Q.front();
        Q.pop();
        pthread_mutex_unlock(&qLock);
        return true;
    }

public:
    thread_pool(int N) : thN(N),
        qLock(PTHREAD_MUTEX_INITIALIZER),
        qCond(PTHREAD_COND_INITIALIZER) {

        threads = new pthread_t[N];
        for(int n = 0; n < N; ++n)
        {
            pthread_create(&threads[n], NULL, thread_func, (void*)this);
        }
    }
    virtual ~thread_pool(){
        while(!Q.empty()){
            delete Q.front();
            Q.pop();
        }
    }

    void addJob(job* ajob)
    {
        pthread_mutex_lock(&qLock);
        Q.push(ajob);
        pthread_mutex_unlock(&qLock);
        pthread_cond_signal(&qCond);
    }
};
//------------------------------------------------------

int main(int argc, char* argv[])
{
    const int N = 4;
    const int J = 20;

    thread_pool* tpool = new thread_pool(N);
    for( int n = 0; n < J; ++n )
    {
        //---
        job* o = new job(n);
        //---
        tpool->addJob(0);
    }

    delete tpool;
}
