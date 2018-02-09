#ifndef _DALEKTASK_H_
#define _DALEKTASK_H_
#include <pthread.h>

#include "WPILib.h"

class DalekTask
{
    public:
        DalekTask(const char* taskName = NULL);
        virtual ~DalekTask();

        static void* DalekTaskStarter(void* task);
        void Start();
        void Pause();
        void Terminate();
        virtual void Run() = 0;

    private:
        bool m_enabled;
        bool m_running;
        bool m_dead;
        pthread_t ptask;
};

#endif  // _DALEKTASK_H_

