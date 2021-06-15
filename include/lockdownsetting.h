#ifndef LOCKDOWNSETTING_H
#define LOCKDOWNSETTING_H

#include "abstractlockdownsetting.h"

namespace QRserver
{
    class LockDownSetting : public AbstractLockDownSetting
    {
        public:
            LockDownSetting()
            {
            }

            ~LockDownSetting()
            {
            }

            static LockDownSetting *Instance()
            {
                static LockDownSetting instance;
                return &instance;
            }
    };
};

#endif //LOCKDOWNSETTING_H
