/* This file is part of profile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QBINARYMESSAGE_H
#define QBINARYMESSAGE_H

#include "RServerClient.h"

/* all the rserver binary message types which should be acknowledged by rserver engine
 *
	1.Connect		1).send eConnect2Server	2).receive eConnect2Server

	2.Register		1).Connect	2).send eRegisterTargetExt 3).receive eRegistResult

	3.unregister	1).Connect	2).send eUnRegistTarget	3).receive eUnRegistResult

	4.check			1).Connect	2).send eCheckAll	    3).receive eCheckAll
		-> NOT_REGISTER
		-> REGISTER_NOT_AUTHORIZE
		-> REGISTER_AUTHORIZE	(include all information in st_TestsetAllCheckInfo)
		If Chassis, will send 4 and receive 5, else end.
		4).send eGetProfileInfo	    5).receive eProfileInfo/eProfileInfoEnd(several)

	5.download profile	1).send eGetProfile(several) 2).receive eProfileFileBlk/eProfileFileEnd(several)

	6.upload profile	1).send eUploadProfileBlock/eUploadProfileFinish(several)

	7.upload result		1).send eFolderInfo	2).send eFileBlock/eFileFinish(several)

	8.software upgrade	1).send eTestsetSWverionInfo	2).receive eTestsetSWinfo	3).receive eTestsetSWBLK/eTestsetSWEND(several)
		send eTestsetSWStopSend could stop the upgrade



	eConnect2Server
	eRegistResult,
	eUnRegistResult,
	eVerifyResult
	eGetServerInfo,
	eGetDevInfoExt,
	eGetOrgChartStatus,
	eGetOrgChartInfo,
	eGetProfileInfo,
	eProfileInfo,
	eProfileInfoEnd,
	eGetProfileFile,
	eProfileFileBlk,
	eProfileFileEnd,
	eGetAllProfile,

	eSoftwareVersion,
	eConnectPkgUp,
	eTestsetSWinfo,
	eTestsetSWBLK,
	eTestsetSWEND,

	eDownloadFail,
	eUpgradeFail,
	eSendCmdFail
	eServerClosed,
	eCheckAll,
*/

#endif
