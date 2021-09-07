#ifndef _ZINXTIMER_H_
#define _ZINXTIMER_H_

#include "zinx.h"

class ZinxTimer : public Ichannel{
    private:
        int mFd = -1;
    public:
        ZinxTimer();
        virtual ~ZinxTimer();

        virtual bool Init() override;
        virtual bool ReadFd(std::string & _Input) override;
        virtual bool WriteFd(std::string & _Output) override;
        virtual void Fini() override;
        virtual int GetFd() override;
        virtual std::string GetChannelInfo() override;
        virtual AZinxHandler *GetInputNextStage(BytesMsg &_oInput) override;

};

#endif
