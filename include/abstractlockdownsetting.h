#ifndef ABSTRACTLOCKDOWNSETTING_H
#define ABSTRACTLOCKDOWNSETTING_H

#include "picojson.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

namespace QRserver
{
    class AbstractLockDownSetting
    {
        public:
            AbstractLockDownSetting()
            {
                m_bLockDown = false;
            }
            ~AbstractLockDownSetting()
            {
            }

            static AbstractLockDownSetting *Instance()
            {
                static AbstractLockDownSetting instance;
                return &instance;
            }

            bool isLockDown()
            {
                if (!loadRserverAdiminSetting()) {
                    cerr << "Load device related profile failed!" << endl;
                    return false;
                } else {
                    return m_bLockDown;
                }
            }

            bool isProfileLocked(string profPath)
            {
                bool bIsLocked = false;

                if (!getImmutable(profPath, bIsLocked)) {
                    cerr << "getImmutable failed!" << endl;
                    return false;
                } else {
                    return bIsLocked;
                }
            }

            bool isProfileLocked(char *profPath)
            {
                return isProfileLocked(string(profPath));
            }

            bool lockDownProfile(string profPath)
            {
                return setImmutable(profPath, true);
            }

            bool unlockDownProfile(string profPath)
            {
                return setImmutable(profPath, false);
            }

            bool getImmutable(const string &pathName, bool &bImmutable)
            {
                int fd = open(pathName.c_str(), O_RDONLY);

                if (fd < 0) {
                    close(fd);
                    return false;
                }

                int attr = 0;

                if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                if ((attr & FS_IMMUTABLE_FL) == FS_IMMUTABLE_FL) {
                    bImmutable = true;
                } else {
                    bImmutable = false;
                }

                close(fd);
                return true;
            }

            bool setImmutable(const string &pathName, bool bImmutable)
            {
                int fd = open(pathName.c_str(), O_RDONLY);

                if (fd < 0) {
                    close(fd);
                    return false;
                }

                int attr = 0;

                if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                if (bImmutable) {
                    attr |= FS_IMMUTABLE_FL;
                } else {
                    attr &= ~FS_IMMUTABLE_FL;
                }

                if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                close(fd);
                return true;
            }

            bool setCryptedMark(string pathName)
            {
                int fd = open(pathName.c_str(), O_RDONLY);

                if (fd < 0) {
                    close(fd);
                    return false;
                }

                int attr = 0;

                if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                attr |= FS_COMPR_FL;

                if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                close(fd);
                return true;
            }

            bool getCryptedMark(string profPath)
            {
                int fd = open(profPath.c_str(), O_RDONLY);

                if (fd < 0) {
                    close(fd);
                    return false;
                }

                int attr = 0;

                if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
                    close(fd);
                    return false;
                }

                close(fd);
                return ((attr & FS_COMPR_FL) == FS_COMPR_FL);
            }

        private:
            bool loadRserverAdiminSetting()
            {
                std::ifstream inFile(getRserverAdminSettingPathname().c_str());
                string inAll((std::istreambuf_iterator<char>(inFile)),
                             std::istreambuf_iterator<char>());
                picojson::value vAll;
                string errStr = picojson::parse(vAll, inAll);

                if (!errStr.empty()) {
                    cerr << errStr << endl;
                    return false;
                }

                picojson::value vAdminStatus = vAll.get("adminStatus");
                picojson::value vLockDown = vAll.get("lockDown");

                if (!(vAdminStatus.is<double>() && vLockDown.is<double>())) {
                    cerr << "Wrong json structure!" << endl;
                    return false;
                }

                m_bLockDown = ((vAdminStatus.get<double>() != 0)
                               && (vLockDown.get<double>() != 0));
                return true;
            }

            bool loadDevRelatedProf()
            {
                std::ifstream inFile(getDevRelatedProfPathname().c_str());
                string inAll((std::istreambuf_iterator<char>(inFile)),
                             std::istreambuf_iterator<char>());
                picojson::value vAll;
                string errStr = picojson::parse(vAll, inAll);

                if (!errStr.empty()) {
                    cerr << errStr << endl;
                    return false;
                }

                picojson::value vProfList = vAll.get("deviceRealtedProfileList");

                if (!vProfList.is<picojson::array>()) {
                    cerr << "Wrong json structure!" << endl;
                    return false;
                }

                picojson::array aProfArray = vProfList.get<picojson::array>();

                for (picojson::array::iterator i = aProfArray.begin(); i != aProfArray.end();
                     ++i) {
                    if (!i->is<picojson::object>()) {
                        cerr << "Wrong json structure!" << endl;
                        return false;
                    }

                    picojson::object oProfPath = i->get<picojson::object>();
                    picojson::value vProfPath = oProfPath["pathName"];

                    if (!vProfPath.is<string>()) {
                        cerr << "Wrong json structure!" << endl;
                        return false;
                    }

                    string path = vProfPath.get<string>();
                    m_devRelatedProfPaths.push_back(path);
                }

                return true;
            }

            string getRserverAdminSettingPathname() const
            {
#if defined(BUILD_X86)
                return getAppDirName() + string("/home/rserver/adminsetting.json");
#else
                return string("/home/rserver/adminsetting.json");
#endif
            }

            string getDevRelatedProfPathname() const
            {
#if defined(BUILD_X86)
                return getAppDirName() +
                       string("/../etc/rserver/devicerelatedprofile.json");
#else
                return string("/usr/local/etc/rserver/devicerelatedprofile.json");
#endif
            }

            string getAppDirName() const
            {
                char pidExePath[32];
                char appPathname[256];
                sprintf(pidExePath, "/proc/%d/exe", getpid());
                readlink(pidExePath, appPathname, sizeof(appPathname));
                return string(dirname(appPathname));
            }

        private:
            vector<string> m_devRelatedProfPaths;
            bool m_bLockDown;
    };
}

#endif // ABSTRACTLOCKDOWNSETTING_H
