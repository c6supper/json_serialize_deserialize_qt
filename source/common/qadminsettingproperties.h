
#ifndef QADMINSETTINGPROFILEPROPERTIES_H
#define QADMINSETTINGPROFILEPROPERTIES_H

#include "qprofile_global.h"
#include "qabstractadminsettingprofile.h"

namespace QRserver
{
    class QAdminSettingProfileProperties : public QAbstractAdminSettingProfile
    {
            Q_OBJECT

        public:
            QAdminSettingProfileProperties(QObject *parent = nullptr):
                QAbstractAdminSettingProfile(
                    parent)
            {
            }


        private:
            virtual void pureVirtualMock() {};

    };
}

#endif /* QADMINSETTINGPROFILEPROPERTIES_H */
