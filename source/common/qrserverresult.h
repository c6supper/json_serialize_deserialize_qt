/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QRSERVERRESULT_H
#define QRSERVERRESULT_H

#include "qrserverfile.h"
#include "qabstractresultfilter.h"
#include <QMutexLocker>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace QRserver
{
    class QRserverResult: public QRserverFile
    {
            Q_OBJECT
            Q_ENUMS(ResultType)

        public:
            enum ResultType {
                //cx 0
                eRstAnalogChannel = 0,
                eRstDigitalChannel,
                eRstInstallationCheck,
                eRstReturnPath,
                eRstReturnPathIGM,
                eRstReturnPathSweep,
                eRstSystemScan,
                eRstSpectrum350,
                eRstSpectrum380,
                eRstSpectrumIGM,
                eRstTilt,
                eRstCableModem,
                eRstTDR,
                eRstMPEG_TS,
                eRstInServiceSweep,
                eRstRPB,
                eRstHIPAutoTest,
                eRstOFDM,
                eRstIngressUnderQAM,
                eRstMultiTableScan,
                eRstInChannelFrequencyResponse,
                eRstTimedStats,
                eRstHeadendCheck,
                eRstFCC,
                eRstForwardPathSweep,
                eRstQAMWiz,

                //cx cable modem 100
                eRstCMFtpGet = 100,
                eRstCMFtpPut,
                eRstCMHttpGet,
                eRstCMWeb,
                eRstCMPing,
                eRstCMTraceRoute,
                eRstCMVoIPCheck,
                eRstCMVoIPClient,
                eRstCMVoIPServer,
                eRstCMVoIPPhone,
                eRstCMSpeedTest,

                //1GC1 Ethernet Testing
                eRstBert1GC1 = 1000,
                eRstThrpt1GC1 = 1010,
                eRstRfc25441GC1 = 1005,
                eRstVsam1GC1 = 1015,
                //1GC2 Ethernet Testing
                eRstBert1GC2 = 1001,
                eRstThrpt1GC2 = 1011,
                eRstRfc25441GC2 = 1006,
                eRstVsam1GC2 = 1016,

                eRstEth1GC1Min = 2050,
                eRstLoop1GC1 = eRstEth1GC1Min,
                eRstScan1GC1,
                eRstIpPing1GC1,
                eRstIpTrace1GC1,
                eRstIpArp1GC1,
                eRstIpVoipCheck1GC1,
                eRstIpVoipClient1GC1,
                eRstIpVoipIpPhone1GC1,
                eRstIpVoipNextragen1GC1,
                eRstIptvProbe1GC1,
                eRstIptvAnalys1GC1,
                eRstPcap1GC1,
                eRstEsmc1GC1,
                eRstPtpv21GC1, //1588
                eRstWan1GC1, //eRstWan1GF1 //wander results of 1GC1/1GF1 saved in a same dir //without .html
                eRstVperf1GC1,
                eRstFtpget1GC1,
                eRstFtpput1GC1 = eRstFtpget1GC1,
                eRstHttpget1GC1,
                eRstPtpv2PassThrough1GC1, //todo ptpv2passthrough of 1GC1 is not existed, the test is unsupported
                eRstL2cp1GC1,
                eRstNwdisc1GC1, //todo nwdisc of 1GC1 is not existed, the test is unsupported
                eRstAdvmonitor1GC1,
                eRstVsuite1GC1,

                eRstEth1GC2Min = 2100,
                eRstLoop1GC2 = eRstEth1GC2Min,
                eRstScan1GC2,
                eRstIpPing1GC2,
                eRstIpTrace1GC2,
                eRstIpArp1GC2,
                eRstIpVoipCheck1GC2,
                eRstIpVoipClient1GC2,
                eRstIpVoipIpPhone1GC2,
                eRstIpVoipNextragen1GC2,
                eRstIptvProbe1GC2,
                eRstIptvAnalys1GC2,
                eRstPcap1GC2,
                eRstEsmc1GC2,
                eRstPtpv21GC2, //1588
                eRstWan1GC2, //eRstWan1GF2 //wander results of 1GC2/1GF2 saved in a same dir //without .html
                eRstVperf1GC2,
                eRstFtpget1GC2,
                eRstFtpput1GC2 = eRstFtpget1GC2,
                eRstHttpget1GC2,
                eRstPtpv2PassThrough1GC2, //todo ptpv2passthrough of 1GC2 is not existed, the test is unsupported
                eRstL2cp1GC2,
                eRstNwdisc1GC2, //todo nwdisc of 1GC2 is not existed, the test is unsupported
                eRstAdvmonitor1GC2,//todo advmonitor of 1GC2 is not existed, the test is unsupported
                eRstVsuite1GC2,

                //1GF1 Ethernet Testing
                eRstBert1GF1 = 1002,
                eRstThrpt1GF1 = 1012,
                eRstRfc25441GF1 = 1007,
                eRstVsam1GF1 = 1017,

                eRstEth1GF1Min = 2150,
                eRstLoop1GF1 = eRstEth1GF1Min,
                eRstScan1GF1,
                eRstIpPing1GF1,
                eRstIpTrace1GF1,
                eRstIpArp1GF1,
                eRstIpVoipCheck1GF1,
                eRstIpVoipClient1GF1,
                eRstIpVoipIpPhone1GF1,
                eRstIpVoipNextragen1GF1,
                eRstIptvProbe1GF1,
                eRstIptvAnalys1GF1,
                eRstPcap1GF1,
                eRstEsmc1GF1,
                eRstPtpv21GF1, //1588
                eRstWan1GF1, //eRstWan1GC1 //wander results of 1GC1/1GF1 saved in a same dir //without .html
                eRstVperf1GF1,
                eRstFtpget1GF1,
                eRstFtpput1GF1 = eRstFtpget1GF1,
                eRstHttpget1GF1,
                eRstPtpv2PassThrough1GF1, //todo ptpv2passthrough of 1GF1 is not existed, the test is unsupported
                eRstL2cp1GF1,
                eRstNwdisc1GF1, //todo nwdisc of 1GF1 is not existed, the test is unsupported
                eRstAdvmonitor1GF1,
                eRstVsuite1GF1,

                //1GF2 Ethernet Testing
                eRstBert1GF2 = 1003,
                eRstThrpt1GF2 = 1013,
                eRstRfc25441GF2 = 1008,
                eRstVsam1GF2 = 1018,

                eRstEth1GF2Min = 2200,
                eRstLoop1GF2 = eRstEth1GF2Min,
                eRstScan1GF2,
                eRstIpPing1GF2,
                eRstIpTrace1GF2,
                eRstIpArp1GF2,
                eRstIpVoipCheck1GF2,
                eRstIpVoipClient1GF2,
                eRstIpVoipIpPhone1GF2,
                eRstIpVoipNextragen1GF2,
                eRstIptvProbe1GF2,
                eRstIptvAnalys1GF2,
                eRstPcap1GF2,
                eRstEsmc1GF2,
                eRstPtpv21GF2, //1588
                eRstWan1GF2, //eRstWan1GC2 //wander results of 1GC2/1GF2 saved in a same dir //without .html
                eRstVperf1GF2,
                eRstFtpget1GF2,
                eRstFtpput1GF2 = eRstFtpget1GF2,
                eRstHttpget1GF2,
                eRstPtpv2PassThrough1GF2, //todo ptpv2passthrough of 1GF2 is not existed, the test is unsupported
                eRstL2cp1GF2,
                eRstNwdisc1GF2, //todo nwdisc of 1GF2 is not existed, the test is unsupported
                eRstAdvmonitor1GF2,//todo advmonitor of 1GF2 is not existed, the test is unsupported
                eRstVsuite1GF2,

                //10GC1 Ethernet Testing
                eRstBert10G = 1004,
                eRstThrpt10G = 1014,
                eRstRfc254410G = 1009,
                eRstVsam10G = 1019,

                eRstEth10GMin = 2250,
                eRstLoop10G = eRstEth10GMin,
                eRstScan10G,
                eRstIpPing10G,
                eRstIpTrace10G,
                eRstIpArp10G,
                eRstIpVoipCheck10G,
                eRstIpVoipClient10G,
                eRstIpVoipIpPhone10G,
                eRstIpVoipNextragen10G,
                eRstIptvProbe10G,
                eRstIptvAnalys10G,
                eRstPcap10G,
                eRstEsmc10G,
                eRstPtpv210G, //1588
                eRstWan10G, //wander //without .html
                eRstVperf10G,
                eRstAdvmonitor10G,
                eRstL2cp10G,
                eRstFtpget10G,
                eRstFtpput10G = eRstFtpget10G,
                eRstHttpget10G,
                eRstEcpriThrpt10G,//ecpri Testing
                eRstEcpriPcap10G,//ecpri Testing

                //10GC2 Ethernet Testing
                eRstEth10G2Min = 2300,
                eRstBert10G2 = eRstEth10G2Min,
                eRstThrpt10G2,
                eRstLoop10G2,
                eRstRfc254410G2,
                eRstVsam10G2,
                eRstScan10G2,
                eRstIpPing10G2,
                eRstIpTrace10G2,
                eRstIpArp10G2,
                eRstIpVoipCheck10G2,
                eRstIpVoipClient10G2,
                eRstIpVoipIpPhone10G2,
                eRstIpVoipNextragen10G2,
                eRstIptvProbe10G2,
                eRstIptvAnalys10G2,
                eRstPcap10G2,
                eRstEsmc10G2,
                eRstPtpv210G2, //1588
                eRstWan10G2, //wander //without .html
                eRstVperf10G2,
                eRstAdvmonitor10G2,//todo advmonitor of 10G2 is not existed, the test is unsupported
                eRstL2cp10G2,
                eRstFtpget10G2,
                eRstFtpput10G2 = eRstFtpget10G2,
                eRstHttpget10G2,
                eRstEcpriThrpt10G2,//ecpri Testing
                eRstEcpriPcap10G2,//ecpri Testing

                //2G5 OTN/SDH/SONET/PDH/Dsn Testing
                //results of port1/port2 saved in a same dir1
                eRstSdh2p5G1Min = 2350,
                eRstBert2p5G1 = eRstSdh2p5G1Min,
                eRstPulse2p5G1,
                eRstIsdn2p5G1,
                eRstJit2p5G1,
                eRstWan2p5G1,
                eRstMtj2p5G1,
                eRstJtf2p5G1,
                eRstDs12p5G1,//using eRstBert2p5G1 instead
                eRstDs32p5G1,//using eRstBert2p5G1 instead
                eRstSonet2p5G1,//using eRstBert2p5G1 instead

                eRstSdh2p5G2Min = 2400,
                eRstBert2p5G2 = eRstSdh2p5G2Min,//using eRstBert2p5G1 instead
                eRstPulse2p5G2,//using eRstPulse2p5G1 instead
                eRstIsdn2p5G2,//using eRstIsdn2p5G1 instead
                eRstJit2p5G2,//using eRstJit2p5G1 instead
                eRstWan2p5G2,//using eRstWan2p5G1 instead
                eRstMtj2p5G2,//using eRstMtj2p5G1 instead
                eRstJtf2p5G2,//using eRstJtf2p5G1 instead
                eRstDs12p5G2,//using eRstBert2p5G1 instead
                eRstDs32p5G2,//using eRstBert2p5G1 instead
                eRstSonet2p5G2,//using eRstBert2p5G1 instead
                eRstOwd2p5G2,

                //100G OTN/SDH/SONET/PDH/Dsn Testing
                eRstSdh100GMin = 2370,
                eRstSdhBert100G = eRstSdh100GMin,

                //OTU-Xe(1e/2e) Testing
                eRstOtuxMin = 2450,
                eRstOtuxP1 = eRstOtuxMin,//todo results of port1/port2 saved in a dir
                eRstOtuxP2,//using eRstOtuxP1 instead

                //OTU3/OTU4 Testing
                eRstOTU3Min = 3410,
                eRstOTU3Transport = eRstOTU3Min,

                eRstOTU4Min = 3420,
                eRstOTU4Transport = eRstOTU4Min,

                //1/2/4G Fiber Channel Testing
                eRstFc1GF1Min = 2500,
                eRstFcBert1GF1 = eRstFc1GF1Min,
                eRstFcThrpt1GF1,
                eRstFcLoop1GF1,//todo can't save result
                eRstFcRfc25441GF1,
                eRstFcPort1GF1,//todo FcPort of 1GF1 is not existed, the test is unsupported

                eRstFc1GF2Min = 2550,
                eRstFcBert1GF2 = eRstFc1GF2Min,
                eRstFcThrpt1GF2,
                eRstFcLoop1GF2,//todo can't save result
                eRstFcRfc25441GF2,
                eRstFcPort1GF2,//todo FcPort of 1GF2 is not existed, the test is unsupported

                //8/10G Fiber Channel Testing
                eRstFc10GF1Min = 2600,
                eRstFcBert10GF1 = eRstFc10GF1Min,
                eRstFcThrpt10GF1,
                eRstFcLoop10GF1,//todo can't save result
                eRstFcRfc254410GF1,
                eRstFcPort10GF1,//todo FcPort of 10GF1 is not existed, the test is unsupported

                eRstFc10GF2Min = 2650,
                eRstFcBert10GF2 = eRstFc10GF2Min,
                eRstFcThrpt10GF2,
                eRstFcLoop10GF2,//todo can't save result
                eRstFcRfc254410GF2,
                eRstFcPort10GF2,//todo FcPort of 10GF2 is not existed, the test is unsupported

                //CPRI/OBSAI Testing
                eRstCpri1GF1Min = 2700,
                eRstCpri1GF1 = eRstCpri1GF1Min,

                eRstCpri1GF2Min = 2750,
                eRstCpri1GF2 = eRstCpri1GF2Min,

                //40GE Testing
                eRstEth40GEMin = 3450,
                eRstBert40GE = eRstEth40GEMin,
                eRstThrpt40GE,
                eRstLoop40GE,
                eRstRfc254440GE,
                eRstVsam40GE,
                eRstScan40GE,
                eRstIpPing40GE,
                eRstIpTrace40GE,
                eRstIpArp40GE,
                eRstIpVoipCheck40GE,//todo voip of 40GE is not existed, the test is unsupported
                eRstIpVoipClient40GE,//todo voip of 40GE is not existed, the test is unsupported
                eRstIpVoipIpPhone40GE,//todo voip of 40GE is not existed, the test is unsupported
                eRstIpVoipNextragen40GE,//todo voip of 40GE is not existed, the test is unsupported
                eRstIptvProbe40GE,//todo iptv of 40GE is not existed, the test is unsupported
                eRstIptvAnalys40GE,//todo iptv of 40GE is not existed, the test is unsupported
                eRstPcap40GE,
                eRstEsmc40GE,//todo esmc of 40GE is not existed, the test is unsupported
                eRstPtpv240GE, //todo 1588 of 40GE is not existed, the test is unsupported
                eRstWan40GE, //todo wander of 40GE is not existed, the test is unsupported
                eRstVperf40GE,//todo layer4 of 40GE is not existed, the test is unsupported
                eRstAdvmonitor40GE,
                eRstLaneBert40GE,
                eRstPcs40GE,
                eRstBert40GE2 = 3475,
                eRstThrpt40GE2,
                eRstLoop40GE2,
                eRstRfc254440GE2,
                eRstVsam40GE2,
                eRstScan40GE2,
                eRstIpPing40GE2,
                eRstIpTrace40GE2,
                eRstIpArp40GE2,
                eRstIpVoipCheck40GE2,//todo voip of 40GE2 is not existed, the test is unsupported
                eRstIpVoipClient40GE2,//todo voip of 40GE2 is not existed, the test is unsupported
                eRstIpVoipIpPhone40GE2,//todo voip of 40GE2 is not existed, the test is unsupported
                eRstIpVoipNextragen40GE2,//todo voip of 40GE2 is not existed, the test is unsupported
                eRstIptvProbe40GE2,//todo iptv of 40GE2 is not existed, the test is unsupported
                eRstIptvAnalys40GE2,//todo iptv of 40GE2 is not existed, the test is unsupported
                eRstPcap40GE2,
                eRstEsmc40GE2,//todo esmc of 40GE is not existed, the test is unsupported
                eRstPtpv240GE2, //todo 1588 of 40GE2 is not existed, the test is unsupported
                eRstWan40GE2, //todo wander of 40GE2 is not existed, the test is unsupported
                eRstVperf40GE2,//todo layer4 of 40GE2 is not existed, the test is unsupported
                eRstAdvmonitor40GE2,
                eRstLaneBert40GE2,
                eRstPcs40GE2,

                //100GE Testing
                eRstEth100GEMin = 3550,
                eRstBert100GE = eRstEth100GEMin,
                eRstThrpt100GE,
                eRstLoop100GE,
                eRstRfc2544100GE,
                eRstVsam100GE,
                eRstScan100GE,
                eRstIpPing100GE,
                eRstIpTrace100GE,
                eRstIpArp100GE,
                eRstIpVoipCheck100GE,//todo voip of 100GE is not existed, the test is unsupported
                eRstIpVoipClient100GE,//todo voip of 100GE is not existed, the test is unsupported
                eRstIpVoipIpPhone100GE,//todo voip of 100GE is not existed, the test is unsupported
                eRstIpVoipNextragen100GE,//todo voip of 100GE is not existed, the test is unsupported
                eRstIptvProbe100GE,//todo iptv of 100GE is not existed, the test is unsupported
                eRstIptvAnalys100GE,//todo iptv of 100GE is not existed, the test is unsupported
                eRstPcap100GE,
                eRstEsmc100GE,//todo esmc of 100GE is not existed, the test is unsupported
                eRstPtpv2100GE, //todo 1588 of 100GE is not existed, the test is unsupported
                eRstWan100GE, //todo wander of 100GE is not existed, the test is unsupported
                eRstVperf100GE,
                eRstAdvmonitor100GE,
                eRstLaneBert100GE,
                eRstPcs100GE,
                eRstBert100GE2 = 3575,
                eRstThrpt100GE2,
                eRstLoop100GE2,
                eRstRfc2544100GE2,
                eRstVsam100GE2,
                eRstScan100GE2,
                eRstIpPing100GE2,
                eRstIpTrace100GE2,
                eRstIpArp100GE2,
                eRstIpVoipCheck100GE2,//todo voip of 100GE2 is not existed, the test is unsupported
                eRstIpVoipClient100GE2,//todo voip of 100GE2 is not existed, the test is unsupported
                eRstIpVoipIpPhone100GE2,//todo voip of 100GE2 is not existed, the test is unsupported
                eRstIpVoipNextragen100GE2,//todo voip of 100GE2 is not existed, the test is unsupported
                eRstIptvProbe100GE2,//todo iptv of 100GE2 is not existed, the test is unsupported
                eRstIptvAnalys100GE2,//todo iptv of 100GE2 is not existed, the test is unsupported
                eRstPcap100GE2,
                eRstEsmc100GE2,//todo esmc of 100GE2 is not existed, the test is unsupported
                eRstPtpv2100GE2, //todo 1588 of 100GE2 is not existed, the test is unsupported
                eRstWan100GE2, //todo wander of 100GE2 is not existed, the test is unsupported
                eRstVperf100GE2,
                eRstAdvmonitor100GE2,
                eRstLaneBert100GE2,
                eRstPcs100GE2,

                //25G Testing
                eRstEth25GMin = 3800,
                eRstBert25G = eRstEth25GMin,
                eRstThrpt25G,
                eRstLoop25G,
                eRstRfc254425G,
                eRstVsam25G,
                eRstScan25G,
                eRstIpPing25G,
                eRstIpTrace25G,
                eRstIpArp25G,
                eRstIpVoipCheck25G,//todo voip of 25G is not existed, the test is unsupported
                eRstIpVoipClient25G,//todo voip of 25G is not existed, the test is unsupported
                eRstIpVoipIpPhone25G,//todo voip of 25G is not existed, the test is unsupported
                eRstIpVoipNextragen25G,//todo voip of 25G is not existed, the test is unsupported
                eRstIptvProbe25G,//todo iptv of 25G is not existed, the test is unsupported
                eRstIptvAnalys25G,//todo iptv of 25G is not existed, the test is unsupported
                eRstPcap25G,
                eRstEsmc25G,//todo esmc of 25G is not existed, the test is unsupported
                eRstPtpv225G, //todo 1588 of 25G is not existed, the test is unsupported
                eRstWan25G, //todo wander of 25G is not existed, the test is unsupported
                eRstVperf25G,//todo layer4 of 25G is not existed, the test is unsupported
                eRstL2cp25G,
                eRstEcpriThrpt25G,//ecpri Testing
                eRstEcpriPcap25G,//ecpri Testing
                eRstBert25G2 = 3825,
                eRstThrpt25G2,
                eRstLoop25G2,
                eRstRfc254425G2,
                eRstVsam25G2,
                eRstScan25G2,
                eRstIpPing25G2,
                eRstIpTrace25G2,
                eRstIpArp25G2,
                eRstIpVoipCheck25G2,//todo voip of 25G2 is not existed, the test is unsupported
                eRstIpVoipClient25G2,//todo voip of 25G2 is not existed, the test is unsupported
                eRstIpVoipIpPhone25G2,//todo voip of 25G2 is not existed, the test is unsupported
                eRstIpVoipNextragen25G2,//todo voip of 25G2 is not existed, the test is unsupported
                eRstIptvProbe25G2,//todo iptv of 25G2 is not existed, the test is unsupported
                eRstIptvAnalys25G2,//todo iptv of 25G2 is not existed, the test is unsupported
                eRstPcap25G2,
                eRstEsmc25G2,//todo esmc of 25G2 is not existed, the test is unsupported
                eRstPtpv225G2, //todo 1588 of 25G2 is not existed, the test is unsupported
                eRstWan25G2, //todo wander of 25G2 is not existed, the test is unsupported
                eRstVperf25G2,//todo layer4 of 25G2 is not existed, the test is unsupported
                eRstL2cp25G2,
                eRstEcpriThrpt25G2,//ecpri Testing
                eRstEcpriPcap25G2,//ecpri Testing

                //50GE Testing
                eRstEth50GEMin = 3850,
                eRstBert50GE = eRstEth50GEMin,
                eRstThrpt50GE,
                eRstLoop50GE,
                eRstRfc254450GE,
                eRstVsam50GE,
                eRstScan50GE,
                eRstIpPing50GE,
                eRstIpTrace50GE,
                eRstIpArp50GE,
                eRstIpVoipCheck50GE,//todo voip of 50GE is not existed, the test is unsupported
                eRstIpVoipClient50GE,//todo voip of 50GE is not existed, the test is unsupported
                eRstIpVoipIpPhone50GE,//todo voip of 50GE is not existed, the test is unsupported
                eRstIpVoipNextragen50GE,//todo voip of 50GE is not existed, the test is unsupported
                eRstIptvProbe50GE,//todo iptv of 50GE is not existed, the test is unsupported
                eRstIptvAnalys50GE,//todo iptv of 50GE is not existed, the test is unsupported
                eRstPcap50GE,
                eRstEsmc50GE,//todo esmc of 50GE is not existed, the test is unsupported
                eRstPtpv250GE, //todo 1588 of 50GE is not existed, the test is unsupported
                eRstWan50GE, //todo wander of 50GE is not existed, the test is unsupported
                eRstVperf50GE,//todo layer4 of 50GE is not existed, the test is unsupported
                eRstPcs50GE,
                eRstBert50GE2 = 3875,
                eRstThrpt50GE2,
                eRstLoop50GE2,
                eRstRfc254450GE2,
                eRstVsam50GE2,
                eRstScan50GE2,
                eRstIpPing50GE2,
                eRstIpTrace50GE2,
                eRstIpArp50GE2,
                eRstIpVoipCheck50GE2,//todo voip of 50GE2 is not existed, the test is unsupported
                eRstIpVoipClient50GE2,//todo voip of 50GE2 is not existed, the test is unsupported
                eRstIpVoipIpPhone50GE2,//todo voip of 50GE2 is not existed, the test is unsupported
                eRstIpVoipNextragen50GE2,//todo voip of 50GE2 is not existed, the test is unsupported
                eRstIptvProbe50GE2,//todo iptv of 50GE2 is not existed, the test is unsupported
                eRstIptvAnalys50GE2,//todo iptv of 50GE2 is not existed, the test is unsupported
                eRstPcap50GE2,
                eRstEsmc50GE2,//todo esmc of 50GE2 is not existed, the test is unsupported
                eRstPtpv250GE2, //todo 1588 of 50GE2 is not existed, the test is unsupported
                eRstWan50GE2, //todo wander of 50GE2 is not existed, the test is unsupported
                eRstVperf50GE2,//todo layer4 of 50GE2 is not existed, the test is unsupported
                eRstPcs50GE2,

                //OSA Testing
                eRstOSAMin = 4000,
                eRstOSARst = eRstOSAMin,
                eRstOCCRst,

                //Common
                eRstComMin = 2800,
                eRstComFiberscopt = eRstComMin,
                eRstComOpm,
                eRstComOdm,
                eRstComScreen,

                eRstComWifiMin = 2850,
                eRstComWifiIpPing = eRstComWifiMin,
                eRstComWifiIpTrace,
                eRstComWifiIpArp,
                eRstComWifiSSID,

                //layer1/layer2 CPRI Testing
                eRstCpriFramedL1P1Min = 2900,
                eRstCpriFramedL1P1Rst = eRstCpriFramedL1P1Min,
                eRstCpriFramedL1P1Rtd,

                eRstCpriFramedL1P2Min = 2950,
                eRstCpriFramedL1P2Rst = eRstCpriFramedL1P2Min,
                eRstCpriFramedL1P2Rtd,

                eRstCpriFramedL2P1Min = 3000,
                RstCpriFramedL2P1Rst = eRstCpriFramedL2P1Min,
                RstCpriFramedL2P1Wan,

                eRstCpriFramedL2P2Min = 3050,
                eRstCpriFramedL2P2Rst = eRstCpriFramedL2P2Min,

                //VIPAG Testing
                eRstVipag10GP1Min = 3100,
                eRstVipag10GP1 = eRstVipag10GP1Min,//todo vipag of 10G is not existed, the test is unsupported

                eRstVipag1GMin = 3101,
                eRstVipag1GP1 = eRstVipag1GMin,

                eRstVipag10GP2Min = 3150,
                eRstVipag10GP2 = eRstVipag10GP2Min,//todo vipag of 10G is not existed, the test is unsupported

                eRstVipag100GEMin = 3160,
                eRstVipag100GE = eRstVipag100GEMin,

                eRstOdu0Min = 3200,
                eRstOdu0Bert = eRstOdu0Min,

                eRstMTT6BMin = 4020,
                eRstMTT6BRst = eRstMTT6BMin,

                eRstMTT8Min = 4040,
                eRstMTT8T1BE = eRstMTT8Min,
                eRstMTT8T1EV,

                eRstMTT9Min = 4060,
                eRstMTT9Rst = eRstMTT9Min,

                eRstMTT14BMin = 4080,
                eRstMTT14BRst = eRstMTT14BMin,

                eRstMTT16AMin = 4100,
                eRstMTT16ADCVT = eRstMTT16AMin,
                eRstMTT16AACVT,
                eRstMTT16AOHMT,
                eRstMTT16ACAPT,
                eRstMTT16AAMPT,

                eRstMTT19AMin = 4120,
                eRstMTT19ARst = eRstMTT19AMin,

                eRstMTT24Min = 4140,
                eRstMTT24Rst = eRstMTT24Min,

                eRstMTT27Min = 4160,
                eRstMTT27E1BE = eRstMTT27Min,
                eRstMTT27E1EV,
                eRstMTT27E1VF,
                eRstMTT27E1JM,
                eRstMTT27E1JO,
                eRstMTT27E1JR,
                eRstMTT27E1GS,

                eRstMTT38Min = 4180,
                eRstMTT38ATM = eRstMTT38Min,
                eRstMTT38SDHp,
                eRstMTT38SDHe,
                eRstMTT38SDHm,

                eRstMTT45Min = 4200,
                eRstMTT45Rst = eRstMTT45Min,

                eRstMTT48Min = 4220,
                eRstMTT48STAT = eRstMTT48Min,
                eRstMTT48ADVF,
                eRstMTT48MEAS,
                eRstMTT48CHNT,
                eRstMTT48TMPE,
                eRstMTT48DECM,
                eRstMTT48PLAM,
                eRstMTT48TSME,
                eRstMTT48PIDA,
                eRstMTT48MDIM,
                eRstMTT48CHAN,
                eRstMTT48CNFG,
                eRstMTT48TSRT,
                eRstMTT48UDSL,

                eRstMTT50Min = 4240,
                eRstMTT50STAT = eRstMTT50Min,
                eRstMTT50ETH,
                eRstMTT50BERT,
                eRstMTT50QTST,
                eRstMTT50BWSP,
                eRstMTT50IPTH,
                eRstMTT50RFC,
                eRstMTT50RESV,
                eRstMTT50RTD,

                eRstMTT51Min = 4260,
                eRstMTT51STAT = eRstMTT51Min,

                eRst2600Min = 4280,
                eRst2600SHDSL = eRst2600Min,
                eRst2600PING,

                eRstMTTWiFiAutoTest = 4300,
                eRstMTTWiFiAPScan,
                eRstMTTWiFiClientScan,
                eRstMTTWiFiVPerf,
                eRstMTTWiFiPoE,
                eRstMTTWiFiSpectrumAnalysis,
                eRstMTTWiFiPing,
                eRstMTTWiFiTraceRoute,
                eRstMTTWiFiARP,
                eRstMTTWiFiSpeedTest,

                eRstMTTGpon =  4320,
                eRstMTTGponPLOAM,
                eRstMTTGponOMCI,
                eRstMTTGponOPM,
                eRstMTTGponWizard,

                eRstMTT260 =  4340,

                eRstMTT520DSL =  4360,
                eRstMTT520TDR,
                eRstMTT520DMM,
                eRstMTT520PSD,
                eRstMTT520RFL,
                eRstMTT520IMPN,
                eRstMTT520LB,
                eRstMTT520LOSS,
                eRstMTT520Noise,
                eRstMTT520Ringers,

                eRstMTT522DSL =  4380,
                eRstMTT522TDR,
                eRstMTT522DMM,
                eRstMTT522PSD,
                eRstMTT522RFL,
                eRstMTT522IMPN,
                eRstMTT522LB,
                eRstMTT522LOSS,
                eRstMTT522Noise,
                eRstMTT522Ringers,

                eRstMTT523DSL =  4400,
                eRstMTT523TDR,
                eRstMTT523DMM,
                eRstMTT523PSD,
                eRstMTT523RFL,
                eRstMTT523IMPN,
                eRstMTT523LB,
                eRstMTT523LOSS,
                eRstMTT523Noise,
                eRstMTT523Ringers,

                eRstMTT521DSL =  4420,
                eRstMTT521TDR,
                eRstMTT521DMM,
                eRstMTT521PSD,
                eRstMTT521RFL,
                eRstMTT521IMPN,
                eRstMTT521LB,
                eRstMTT521LOSS,
                eRstMTT521Noise,
                eRstMTT521Ringers,

                eRstG703OWD = 4440,
                eRstG703PULSE,
                eRstG703RSLT,
                eRstG703RTD,

                eRstC3794RTD = 4460,
                eRstC3794OWD,
                eRstC3794RSLT,
                eRstC3794MonitorRSLT,

                //copper SFP+
                eRstBertCSP1 = 4550,
                eRstThrptCSP1,
                eRstRfc2544CSP1,
                eRstVsamCSP1,
                eRstLoopCSP1,
                eRstIpPingCSP1,
                eRstIpTraceCSP1,
                eRstIpArpCSP1,
                eRstIpVoipCheckCSP1,
                eRstIpVoipClientCSP1,
                eRstIpVoipIpPhoneCSP1,
                eRstIpVoipNextragenCSP1,
                eRstIpSecCSP1,
                eRstVperfCSP1,
                eRstFtpgetCSP1,
                eRstFtpputCSP1 = eRstFtpgetCSP1,
                eRstHttpgetCSP1,

                eRstBertCSP2 = 4600,
                eRstThrptCSP2,
                eRstRfc2544CSP2,
                eRstVsamCSP2,
                eRstLoopCSP2,
                eRstIpPingCSP2,
                eRstIpTraceCSP2,
                eRstIpArpCSP2,
                eRstIpVoipCheckCSP2,
                eRstIpVoipClientCSP2,
                eRstIpVoipIpPhoneCSP2,
                eRstIpVoipNextragenCSP2,
                eRstIpSecCSP2,
                eRstVperfCSP2,
                eRstFtpgetCSP2,
                eRstFtpputCSP2 = eRstFtpgetCSP2,
                eRstHttpgetCSP2,

                //common 10000
                eRstComIPARP = 10000,
                eRstComIPFtpGet,
                eRstComIPFtpPut,
                eRstComIPHttpGet,
                eRstComIPWeb,
                eRstComIPPing,
                eRstComIPTraceRoute,
                eRstComIPVoIPCheck,
                eRstComIPVoIPClient,
                eRstComIPVoIPServer,
                eRstComIPVoIPPhone,
                eRstComIPIPTVProbe,
                eRstComIPIPTVAnalysis,
                eRstComIPIPTVScan,
                eRstComIPIPTVView,
                eRstComIPNetWizDiscovery,
                eRstComIPNetWizCableTest,
                eRstComIPTcpThroughput,
                eRstComIPPcap,
                eRstComWifiSpectrumAnalysis,
                eRstComWifiServiceSetIdentifier,

                //OTDR
                eRstOTDRVSCOUTE = 40001,
                eRstOTDRSORS = 40002,
                eRstOTDROLTS = 40003,
                eRstOTDRSCOPE = 40004
            };
        public:
            QRserverResult(QObject *parent = nullptr);
            ~QRserverResult();
            virtual const QString type2String(const qint32 type);
            virtual const qint32 stringToType(const QString typeString);
            static const QVariant GetFileType(const QString &filePath);
            static const QString GetFilterPattern();
            static const QVariant GetResultKeyFileVar();
            static QRserverResult *Instance();
            static const QStringList GetRelatedFileList(const QString absoluteKeyPath,
                    const QString measureMode, const QString measureType);
            static const QStringList GetFileListNeedToBeDeleted(const QString
                    absoluteKeyPath,
                    const QString measureMode, const QString measureType);
            static const bool postUploading(const QString absoluteKeyPath,
                                            const QString measureMode, const QString measureType,
                                            const quint32 commandStatus);

        protected:
            virtual bool isKey(const QString &absolutePath,
                               const QString measureMode = "", const QString measureType = "");
            virtual const QString getDisplayName(const QString &filePath,
                                                 const QString measureMode = "", const QString measureType = "");

        public:
            static const QString DefaultRserverResultConfig;

        private:
            const QString getFilterPattern();
            QAbstractResultFilter *filter(const QString measureMode,
                                          const QString measureType);
            const QVariant getResultKeyFileVar();

        private:
            static QRserverResult 							*m_instance;
            QList<QAbstractResultFilter *>					m_resultFilterList;
            QMutex											m_mutex;

    };
}


#endif /* QRSERVERPROFILE_H */
