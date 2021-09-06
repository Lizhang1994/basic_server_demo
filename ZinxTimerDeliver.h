#ifndef _ZINXTIMERDELIVER_H_
#define _ZINXTIMERDELIVER_H_

#include "zinx.h"
#include <vector>
#include <list>

class TimerOutProc{  /* timeout task abstract class  */
    public:
        TimerOutProc(){}  
        virtual ~TimerOutProc(){}
        virtual void Proc() = 0;    /* call it when timeout  */
        virtual int GetTimerSec() = 0;  /* get current task timeout interval  */

};

struct WheelNode{
int LastCount = -1;
TimerOutProc *pProc = nullptr;

};

class ZinxTimerDeliver : public AZinxHandler{
    private:
        static ZinxTimerDeliver m_Single;   /* single case  */
        int m_Cur_Index = 0;   /* current wheel index  */
        std::vector<std::list<WheelNode>> m_TimerWheel; /* time wheel vector,every element is a list of node contains turns and timer  */
    public:
        ZinxTimerDeliver();
        ~ZinxTimerDeliver();

        static ZinxTimerDeliver &GetInstance(){
        return m_Single;
        }
        bool RegisterProcObject(TimerOutProc &_Proc); /* register a timer handle object  */
        void UnRegisterProcObject(TimerOutProc &_Proc); /* logoff a timer handle object  */
        
        virtual IZinxMsg *InternalHandle(IZinxMsg &_oInput) override;
        virtual AZinxHandler *GetNextHandler(IZinxMsg &_oNextMsg) override;

};

#endif
