/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERDEVICETYPE_H
#define QRSERVERDEVICETYPE_H

#include <QVariant>
#include "qprofile_debug.h"
#include "qprofile_global.h"
#include "qabstractjsonelement.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverDeviceType: public QProfile::QAbstractJsonElement
    {
            Q_OBJECT
            Q_ENUMS(DeviceType)
            Q_ENUMS(ImageType)
            Q_ENUMS(Style)

            Q_PROPERTY(quint32 deviceType READ deviceType WRITE setDeviceType)
            Q_PROPERTY(qint32 imageType READ imageType WRITE setImageType)
            Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName)
            Q_PROPERTY(QString uncompressDir READ uncompressDir WRITE setUncompressDir)
            Q_PROPERTY(QString targetDir READ targetDir WRITE setTargetDir)
            Q_PROPERTY(QString regExpSN READ regExpSN WRITE setRegExpSN)
            Q_PROPERTY(bool isChassis READ isChassis WRITE setIsChassis)
            Q_PROPERTY(bool supportProfile READ supportProfile WRITE setSupportProfile)
            Q_PROPERTY(bool canReleaseAppManually READ canReleaseAppManually WRITE
                       setCanReleaseAppManually)
            Q_PROPERTY(bool canAssignOption READ canAssignOption WRITE setCanAssignOption)
            Q_PROPERTY(QStringList extraOptionModuleList READ extraOptionModuleList WRITE
                       setExtraOptionModuleList)
            Q_PROPERTY(quint32 style READ style WRITE setStyle)

        public:
            static const QString PropertyDeviceTypeTagName;
            static const QString PropertyImageTypeTagName;
            static const QString PropertyDeviceNameTagName;
            static const QString PropertyUncompressDirTagName;
            static const QString PropertyTargetDirTagName;
            static const QString PropertyRegExpSNTagName;
            static const QString PropertyIsChassisTagName;
            static const QString PropertySupportProfileTagName;
            static const QString PropertyCanReleaseAppManuallyTagName;
            static const QString PropertyCanAssignOptionTagName;
            static const QString PropertyExtraOptionModuleListTagName;
            static const QString PropertyStyleTagName;

        public:
            enum DeviceType {
                eRServerUnknownType = -1,

                eRServerRTU300Type = 105,
                eRServerRTU300TX320SMType = 106,
                eRServerRTU300TX300SMType = 107,
                eRServerRTU300TX320S100GQType = 108,
                eRServerRTU300TX320S100GType = 109,
                eRServerRTU300TX300S100GXType = 110,
                eRServerRTU300TX300SOTDRType = 111,

                eRServerRXT1200PLUSPLATFORMType = 159,
                eRServerRXT1200PLATFORMType = 160,
                eRServerRXT1200COMBOType = 161,
                eRServerRXT1200OSAType = 162,
                eRServerRXT1200OTDRType = 163,
                eRServerRXT1200SLMType = 164,
                eRServerRXT1200CFP2Type = 165,
                eRServerRXT12003900Type = 166,
                eRServerRXT12006000EType = 167,
                eRServerRXT12006000PLUSType = 168,
                //	eRServerRXT1200MTT5Type=169,
                eRServerRXT1200MTT6BType = 170,
                eRServerRXT1200MTT8Type = 171,
                eRServerRXT1200MTT9Type = 172,
                eRServerRXT1200MTT14BType = 173,
                //	eRServerRXT1200MTT14CType=174,
                eRServerRXT1200MTT16AType = 175,
                eRServerRXT1200MTT19AType = 176,
                eRServerRXT1200MTT24Type = 177,
                eRServerRXT1200MTT27Type = 178,
                eRServerRXT1200MTT38Type = 179,
                eRServerRXT1200MTT45Type = 180,
                eRServerRXT1200MTT48Type = 181,
                eRServerRXT1200MTT50Type = 182,
                eRServerRXT1200MTT51Type = 183,
                eRServerRXT1200CARRIERType = 184,
                eRServerRXT1200SHDSLType = 185,
                eRServerRXT12006200Type = 186,
                eRServerRXT12003400Type = 187,
                eRServerRXT12006400Type = 188,
                eRServerTX320S100GType = 189,
                eRServerTX320S100GQType = 190,
                eRServerTX340S100GType = 191,
                eRServerTX320SPLATFORMType = 192,
                eRServerTX340SPLATFORMType = 193,
                eRServerTX300SOTDRType = 194,
                eRServerTX300S100GType = 195,
                eRServerTX340S100GQType = 196,
                eRServerTX300STX300SMType = 197,
                eRServerTX300STX320SMType = 198,
                eRServerTX300STX340SMType = 199,
                /*
                Name="TX130" Value="200"
                Name="TX150" Value="201"
                Name="TX300" Value="202"
                Name="TX130M" Value="203"
                Name="TX300M" Value="204"
                */
                eRServerTX300SPLATFORMType = 205,
                /*
                Name="TX130N" Value="206"
                Name="TX380"Value="207"
                Name="TX390" Value="208"
                */
                eRServerMTX150Type = 209,
                eRServerFX100Type = 210,
                eRServerFX150Type = 211,
                eRServerFX180Type = 212,
                eRServerFX180XType = 213,
                eRServerNETBOXType = 214,
                eRServerWX150Type = 215,
                eRServerFX150LType = 216,
                eRServerMTX150XType = 217,
                eRServerUX400Type = 220,
                eRServerTX300SANYDSLType = 221,/* not support. */
                eRServerTX300S100GXType = 222,
                eRServerTX340S100GXType = 223,
                eRServerRXT12006400XType = 224,
                eRServerMTTPLUS600Type = 228,
                eRServerMTTPLUS340Type = 229,
                eRServerMTTPLUS320Type = 230,
                eRServerMTTPLUS410PLUSType = 231,
                eRServerMTTPLUS900Type = 232,
                eRServerMTTPLUS420Type = 233,
                eRServerMTTPLUS260Type = 234,
                eRServerMTTPLUSMTT5Type = 235,
                eRServerMTTPLUSMTT6BType = 236,
                eRServerMTTPLUSMTT8Type = 237,
                eRServerMTTPLUSMTT9Type = 238,
                eRServerMTTPLUSMTT14BType = 239,
                eRServerMTTPLUSPLATFORMType = 240,
                eRServerMTTPLUSMTT14CType = 241,
                eRServerMTTPLUSMTT16AType = 242,
                eRServerMTTPLUSMTT19AType = 243,
                eRServerMTTPLUSMTT24Type = 244,
                eRServerMTTPLUSMTT27Type = 245,
                eRServerMTTPLUSMTT38Type = 246,
                eRServerMTTPLUSMTT45Type = 247,
                eRServerMTTPLUSMTT48Type = 248,
                eRServerMTTPLUSMTT50Type = 249,
                eRServerMTTPLUSMTT51Type = 250,
                eRServerMTTPLUS520Type = 251,
                eRServerMTTPLUS522Type = 252,
                eRServerMTTPLUS523Type = 253,
                eRServerMTTPLUS521Type = 254,
            };
            enum ImageType {
                eRServerWX150ImageType = 288,
                eRServerMTX150ImageType = 289,
                eRServerFX100ImageType = 290,
                eRServerFX150ImageType = 291,
                eRServerFX180ImageType = 292,
                eRServerFX180XImageType = 293,
                eRServerNETBOXImageType = 294,
                eRServerTX300SOTDRImageType = 295,
                eRServerTX300S100GImageType = 296,
                eRServerTX300SANYDSLImageType = 297,/* not support. */
                eRServerTX300STX300SMImageType = 298,
                eRServerTX300STX320SMImageType = 299,
                /*
                Name="TX300" SWImage="300"
                Name="TX380" SWImage="301"
                Name="TX390" SWImage="302"
                */
                eRServerTX300SPLATFORMImageType = 303,
                /*
                Name="TX300M" SWImage="304"
                */
                eRServerTX300STX340SMImageType = 305,
                eRServerTX300S100GXImageType = 306,

                eRServerMTTPLUSPLATFORMImageType = 310,
                eRServerMTTPLUS320ImageType = 311,
                eRServerMTTPLUS410PLUSImageType = 312,
                eRServerMTTPLUS900ImageType = 313,
                eRServerMTTPLUS420ImageType = 314,
                eRServerMTTPLUS260ImageType = 315,
                eRServerMTTPLUSMTT5ImageType = 316,
                eRServerMTTPLUSMTT6BImageType = 317,
                eRServerMTTPLUSMTT8ImageType = 318,
                eRServerMTTPLUSMTT9ImageType = 319,
                eRServerMTTPLUSMTT14BImageType = 320,
                eRServerMTTPLUSMTT14CImageType = 321,
                eRServerMTTPLUSMTT16AImageType = 322,
                eRServerMTTPLUSMTT19AImageType = 323,
                eRServerMTTPLUSMTT24ImageType = 324,
                eRServerMTTPLUSMTT27ImageType = 325,
                eRServerMTTPLUSMTT38ImageType = 326,
                eRServerMTTPLUSMTT45ImageType = 327,
                eRServerMTTPLUSMTT48ImageType = 328,
                eRServerMTTPLUSMTT50ImageType = 329,
                eRServerMTTPLUSMTT51ImageType = 330,
                eRServerMTTPLUS520ImageType = 331,
                eRServerMTTPLUS522ImageType = 332,
                eRServerMTTPLUS523ImageType = 333,
                eRServerMTTPLUS521ImageType = 334,
                eRServerMTTPLUS340ImageType = 335,
                eRServerRXT12006400XImageType = 336,
                eRServerMTTPLUS600ImageType = 337,

                eRServerRXT1200PLATFORMImageType = 340,
                eRServerRXT1200COMBOImageType = 341,
                eRServerRXT1200OSAImageType = 342,
                eRServerRXT1200OTDRImageType = 343,
                eRServerRXT1200SLMImageType = 344,
                eRServerRXT1200CFP2ImageType = 345,
                eRServerRXT12003900ImageType = 346,
                eRServerRXT12006000EImageType = 347,
                eRServerRXT12006000PLUSImageType = 348,
                //	eRServerRXT1200MTT5ImageType=349,
                eRServerRXT1200MTT6BImageType = 350,
                eRServerRXT1200MTT8ImageType = 351,
                eRServerRXT1200MTT9ImageType = 352,
                eRServerRXT1200MTT14BImageType = 353,
                //	eRServerRXT1200MTT14CImageType=354,
                eRServerRXT1200MTT16AImageType = 355,
                eRServerRXT1200MTT19AImageType = 356,
                eRServerRXT1200MTT24ImageType = 357,
                eRServerRXT1200MTT27ImageType = 358,
                eRServerRXT1200MTT38ImageType = 359,
                eRServerRXT1200MTT45ImageType = 360,
                eRServerRXT1200MTT48ImageType = 361,
                eRServerRXT1200MTT50ImageType = 362,
                eRServerRXT1200MTT51ImageType = 363,
                eRServerRXT1200CARRIERImageType = 364,
                eRServerRXT1200SHDSLImageType = 365,
                eRServerRXT12006200ImageType = 366,
                eRServerRXT12003400ImageType = 367,
                eRServerRXT12006400ImageType = 368,
                eRServerMTX150XImageType = 369,
                eRServerFX150LImageType = 370,
                eRServerRTU300ImageType = 371,
                eRServerRTU300TX320SMImageType = 372,
                eRServerRTU300TX300SMImageType = 373,
                eRServerRTU300TX320S100GQImageType = 374,
                eRServerRTU300TX320S100GImageType = 375,
                eRServerRTU300TX300S100GXImageType = 376,
                eRServerRTU300TX300SOTDRImageType = 377


                                                    /*
                                                    Name="TX150" SWImage="401"
                                                    Name="TX130" SWImage="402"
                                                    Name="TX130N" SWImage="403"
                                                    Name="TX130M" SWImage="404"
                                                    */

            };
            enum Style {
                eDefaultStyle,
                eFX150LStyle
            };
        public:
            QRserverDeviceType(QObject *parent = nullptr);
            ~QRserverDeviceType();
            virtual void pureVirtualMock() {};

            quint32 deviceType() const;
            void setDeviceType(quint32 deviceType);

            qint32 imageType() const;
            void setImageType(qint32 imageType);

            QString deviceName() const;
            void setDeviceName(QString deviceName);

            QString uncompressDir() const;
            void setUncompressDir(QString uncompressDir);

            QString targetDir() const;
            void setTargetDir(QString targetDir);

            QString regExpSN() const;
            void setRegExpSN(QString regExpSN);

            bool isChassis() const;
            void setIsChassis(bool b);

            bool supportProfile() const;
            void setSupportProfile(bool b);

            bool canReleaseAppManually() const;
            void setCanReleaseAppManually(bool b);

            bool canAssignOption() const;
            void setCanAssignOption(bool b);

            QStringList extraOptionModuleList() const;
            void setExtraOptionModuleList(QStringList list);

            quint32 style() const;
            void setStyle(quint32 style);

            bool isValid();

        public:
            static const QString ImageType2String(const qint32 imageType);
            static const qint32 String2ImageType(const QString imageTypeString);
            static const QString DeviceType2String(const quint32 deviceType);
            static const quint32 String2DeviceType(const QString deviceTypeString);
            static const quint32 String2Style(const QString styleString);

        private:
            quint32			 m_deviceType;
            qint32				 m_imageType;
            QString			 m_deviceName;
            QString                     m_uncompressDir;
            QString                     m_targetDir;
            QString                     m_regExpSN;
            bool                            m_isChassis;
            bool                            m_supportProfile;
            bool                            m_canReleaseAppManually;
            bool 			m_canAssignOption;
            QStringList m_extraOptionModuleList;
            quint32         m_style;
    };
}

#endif
