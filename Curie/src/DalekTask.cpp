#include "WPILib.h"
#include "DalekTask.h"
#include <string>

DalekTask::DalekTask()
{
    m_enabled = false;
    m_running = true;
    m_dead    = false;
    pthread_create(&ptask, NULL, &DalekTask::DalekTaskStarter, (void*)this);
}

DalekTask::~DalekTask()
{
	void* res;

	this->Terminate();
    pthread_join(ptask, &res);
    free(res);
}

void* 
DalekTask::DalekTaskStarter(void* vtask)
{
	DalekTask *task = (DalekTask *)vtask;
	
    while (task->m_running) {
        if (task->m_enabled) {
            task->Run();
            Wait(0.04);
        }
        else
            Wait(0.05);
    }

    task->m_dead = true;
    return NULL;
}

void 
DalekTask::Start()
{
    m_enabled = true;
}

void 
DalekTask::Pause()
{
    m_enabled = false;
}

void 
DalekTask::Terminate()
{
    m_running = false;

    while (!m_dead)
        Wait(0.02);
}

