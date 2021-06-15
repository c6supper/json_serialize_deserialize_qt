#ifndef QRSERVERSTRINGTABLE_H_
#define QRSERVERSTRINGTABLE_H_

/* Common */
#define TRANSLATE_STR_RSERVER_INITIALIZING                         QObject::tr("R-Server is initializing, please wait!")
#define TRANSLATE_STR_RSERVER_INITIALIZE_FAILED            QObject::tr("R-Server Initialize failed: could not get the device information!")
#define TRANSLATE_STR_RSERVER_TIMEOUT                            QObject::tr("Time out, client is busy, please try again later!")
#define TRANSLATE_STR_RSERVER_PROGRESS                           QObject::tr("Please wait...")
#define TRANSLATE_STR_RSERVER_IPC_ERROR                              QObject::tr("Internal connection error!")

/* Messagebox title */
#define TRANSLATE_STR_RSERVER_WARNING                            QObject::tr("Warning")
#define TRANSLATE_STR_RSERVER_INFORMATION                        QObject::tr("Information")

/* Tab name */
#define TRANSLATE_STR_RSERVER_TAB_REGISTER                       QObject::tr("Register")
#define TRANSLATE_STR_RSERVER_TAB_UPLOAD                         QObject::tr("Upload")
#define TRANSLATE_STR_RSERVER_TAB_PROFILE                        QObject::tr("Profile")
#define TRANSLATE_STR_RSERVER_TAB_RESULT                         QObject::tr("Result")
#define TRANSLATE_STR_RSERVER_TAB_DOWNLOAD                       QObject::tr("Download")
#define TRANSLATE_STR_RSERVER_TAB_SOFTWARE                       QObject::tr("Software")

/* Button */
#define TRANSLATE_STR_RSERVER_BTN_OK                             QObject::tr("OK")
#define TRANSLATE_STR_RSERVER_BTN_REFRESH                        QObject::tr("Refresh")
#define TRANSLATE_STR_RSERVER_BTN_RELEASE                           QObject::tr("Release")
#define TRANSLATE_STR_RSERVER_BTN_CONTINUE                          QObject::tr("Continue")
#define TRANSLATE_STR_RSERVER_BTN_EXIT                                      QObject::tr("Exit")
#define TRANSLATE_STR_RSERVER_BTN_CANCEL                                QObject::tr("Cancel")

#define TRANSLATE_STR_RSERVER_BTN_REGISTER                       QObject::tr("Register")
#define TRANSLATE_STR_RSERVER_BTN_UNREGISTER                     QObject::tr("Unregister")
#define TRANSLATE_STR_RSERVER_BTN_CHECK                          QObject::tr("Check")
#define TRANSLATE_STR_RSERVER_BTN_MODE_RSERVER                          QObject::tr("R-Server")
#define TRANSLATE_STR_RSERVER_BTN_MODE_VCONNECT                          QObject::tr("V-Connect")
#define TRANSLATE_STR_RSERVER_BTN_UPLOAD                         QObject::tr("Upload")
#define TRANSLATE_STR_RSERVER_BTN_JOB                         QObject::tr("Job")
#define TRANSLATE_STR_RSERVER_BTN_UPLOAD_DEL                     QObject::tr("Upload&&Del")
#define TRANSLATE_STR_RSERVER_BTN_DOWNLOAD                       QObject::tr("Download")
#define TRANSLATE_STR_RSERVER_BTN_UPGRADE                        QObject::tr("Upgrade")
#define TRANSLATE_STR_RSERVER_BTN_UPDATE                        QObject::tr("Update")

/* Register */
#define TRANSLATE_STR_RSERVER_REGISTER_SERVER_ADDR               QObject::tr("Server Address/URL")
#define TRANSLATE_STR_RSERVER_REGISTER_TECH_ID                   QObject::tr("Tech ID")
#define TRANSLATE_STR_RSERVER_REGISTER_TECH_NAME                 QObject::tr("Tech Name")
#define TRANSLATE_STR_RSERVER_REGISTER_COMPANY                   QObject::tr("Company")
#define TRANSLATE_STR_RSERVER_REGISTER_SUPERVISOR_ID             QObject::tr("Supervisor ID")
#define TRANSLATE_STR_RSERVER_REGISTER_REGION                    QObject::tr("Region")
#define TRANSLATE_STR_RSERVER_REGISTER_DISTRICT                  QObject::tr("District")
#define TRANSLATE_STR_RSERVER_REGISTER_SYSTEM                    QObject::tr("System")
#define TRANSLATE_STR_RSERVER_REGISTER_VCONNECT                          QObject::tr("  V-Connect")

#define TRANSLATE_STR_RSERVER_REGISTER_ENTER_SERVER_ADDR         QObject::tr("Please enter the server address or URL!")
#define TRANSLATE_STR_RSERVER_REGISTER_INVALID_SERVER_ADDR       QObject::tr("Server address or URL is invalid!")
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_CONFIRM        QObject::tr("Confirm that you want to 'Unregister' this meter from the R-Server.\nPress 'OK' to continue, 'Cancel' to exit.")
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_SUCCESS        QObject::tr("Un-Registration succeeded !")
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_FAIL           QObject::tr("Failed to unregister!")
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_APPROVED       QObject:: tr("Un-registration approved.")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTRATION_LOCK             QObject::tr("Registration locked, please contact system administrator.")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTER_FAIL             QObject::tr("Failed to register!")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_WAIT         QObject:: tr("Registration completed. Waiting for authorization.")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AND_AUTH         QObject:: tr("Registration %1.\nCurrently registered with")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTER_NOT_AUTH         QObject:: tr("Registered but not authorized.")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTER_AUTHORIZED       QObject::tr("Registered and authorized !")
#define TRANSLATE_STR_RSERVER_REGISTER_NOT_REG                   QObject:: tr("Not registered.")
#define TRANSLATE_STR_RSERVER_REGISTER_UPDATE_TECHNAME_SUCCESS   QObject::tr("Update Tech Name succeeded !")
#define TRANSLATE_STR_RSERVER_REGISTER_UPDATE_TECHNAME_FAIL      QObject::tr("Update Tech Name failed, code = %1!")

#define TRANSLATE_STR_RSERVER_REGISTER_REJECTED                  QObject:: tr("Rejected, please contact network supervisor.")
#define TRANSLATE_STR_RSERVER_REGISTER_CHECK_FAIL             QObject:: tr("Failed to check!")
#define TRANSLATE_STR_RSERVER_REGISTER_DNS_FAIL                  QObject::tr("Hostname could not be resolved to an IP address,\nPlease check the DNS setting.")
#define TRANSLATE_STR_RSERVER_REGISTER_NETWORK_ERROR             QObject::tr("Failed to connect to the server,\nPlease check your server address and network connection!")
#define TRANSLATE_STR_RSERVER_REGISTER_NETWORK_ERROR_CODE             QObject::tr("Nework error, code = %1!")
#define TRANSLATE_STR_RSERVER_REGISTER_TECHINFO_EMPTY                   QObject:: tr("Please enter test set information!")
#define TRANSLATE_STR_RSERVER_REGISTER_CHECK_TECHINFO                 QObject:: tr("The information you filled in is exactly the same as before,\nplease reset test set information and try again.")
#define TRANSLATE_STR_RSERVER_REGISTER_REGISTRATION_BOMB_EXPIRED                   QObject:: tr("Registration Bomb locked, please change the test set information,\nthen try updating with server. ")
#define TRANSLATE_STR_RSERVER_REGISTER_UPDATE_DEVICEINFO_FAILED                   QObject:: tr("Failed to update test set information!")
#define TRANSLATE_STR_RSERVER_REGISTER_UPDATE_DEVICEINFO_SCCESS                   QObject:: tr("Update test set information successfully !")
#define TRANSLATE_STR_RSERVER_REGISTER_LOCKDOWN     	QObject::tr("Profile need to be locked down,\ndownloading will be processed!")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_APPROVED			QObject:: tr("Approved")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_LOANED			QObject:: tr("Loaned")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_MISSING			QObject:: tr("Missing")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_Broken			QObject:: tr("Broken")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_Calibration		QObject:: tr("Calibration")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_SOLD				QObject:: tr("Sold")
#define TRANSLATE_STR_RSERVER_REGISTER_TESTSET_STATUS_RETIRED			QObject:: tr("Retired")

#define TRANSLATE_STR_RSERVER_REGISTER_CLIENTVERSION                    QObject::tr("Client: %1")
/* Upload */
#define TRANSLATE_STR_RSERVER_UPLOAD_NAME                       QObject::tr("Name")
#define TRANSLATE_STR_RSERVER_UPLOAD_TYPE                       QObject::tr("Type")
#define TRANSLATE_STR_RSERVER_UPLOAD_DATE                       QObject::tr("Date")
#define TRANSLATE_STR_RSERVER_UPLOAD_TYPEVALUE                       QObject::tr("TypeValue")
#define TRANSLATE_STR_RSERVER_UPLOAD_FULLPATH                       QObject::tr("FullPath")
#define TRANSLATE_STR_RSERVER_UPLOAD_JOBID                       QObject::tr("JobId")
#define TRANSLATE_STR_RSERVER_UPLOAD_LOCATION                       QObject::tr("Location")

#define TRANSLATE_STR_RSERVER_UPLOAD_FILTER                       QObject::tr("Filter")
#define TRANSLATE_STR_RSERVER_UPLOAD_FILTER_JOBID                       QObject::tr("Job ID:")
#define TRANSLATE_STR_RSERVER_UPLOAD_FILTER_LOCATION                       QObject::tr("Location:")

#define TRANSLATE_STR_RSERVER_UPLOAD_LOADFILES_FAILED                    QObject:: tr("Failed to load file list, code = %1!")

#define TRANSLATE_STR_RSERVER_UPLOAD_SUCCESS                     QObject:: tr("Upload files to R-Server successfully !")
#define TRANSLATE_STR_RSERVER_UPLOAD_FAILED                    QObject:: tr("Upload files to R-Server failed, code = %1!")
#define TRANSLATE_STR_RSERVER_UPLOAD_FILENAME_LENGTH_TOOLONG     QObject:: tr("The file name length could not be exceed %1 characters!")

/* Download */
#define TRANSLATE_STR_RSERVER_DOWNLOAD_SUCCESS                   QObject:: tr("Downloaded test profiles from the R-Server successfully!")
#define TRANSLATE_STR_RSERVER_DOWNLOAD_FAILED                    QObject:: tr("Download profiles from R-Server failed, code = %1!")
#define TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKDOWNFAILDE            QObject:: tr("Lock profile failed, code = %1!")
#define TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKINFO            		 QObject:: tr("<a href=\"detail\">Profiles have been locked,click to check the detail.</a>")
#define TRANSLATE_STR_RSERVER_DOWNLOAD_LOCKEDLIST         		 QObject:: tr("Locked Profile Type List")
#define TRANSLATE_STR_RSERVER_DOWNLOAD_MAINTENANCE               QObject:: tr("R-Server is in Maintenance Mode, profiles \ncannot be downloaded at this moment")
#define TRANSLATE_STR_RSERVER_DOWNLOADING_IN_LOCKDOWN_MODE       QObject:: tr("Profile Lockdown is enforced. \nDownloading test profiles…")

/* Software */
#define TRANSLATE_STR_RSERVER_SOFTWARE_TYPE                      QObject::tr("Type")
#define TRANSLATE_STR_RSERVER_SOFTWARE_CURRENT_VERSION           QObject::tr("Current Version")
#define TRANSLATE_STR_RSERVER_SOFTWARE_NEW_VERSION               QObject::tr("New Version")

#define TRANSLATE_STR_RSERVER_SOFTWARE_UPGRADE_TO                QObject::tr("Please select one package you want to upgrade to.")
#define TRANSLATE_STR_RSERVER_SOFTWARE_MEASUREMENT_RUNNING       QObject::tr("Measurement is running, please stop first!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_NO_AVAILABLE_VERSION      QObject::tr("No valid version found in R-Server.")
#define TRANSLATE_STR_RSERVER_SOFTWARE_CURRENT_VERSION_LATEST    QObject::tr("Installed version is latest!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_TOO_MUCH_CONNECTIONS      QObject::tr("Too much connections in R-Server!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_IMAGE_SUCCESS    QObject::tr("Software download completed successfully.\nUpgrading, please wait!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_PROGRESS         QObject::tr("Downloading(%1%),\ndon't power off, please wait...")
#define TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_PROGRESS_WITHOUT_PERCENT_SIGN         QObject::tr("Downloading(%1),\ndon't power off, please wait...")
#define TRANSLATE_STR_RSERVER_SOFTWARE_MD5MISMATCH               QObject::tr("Checking md5 failed, please check!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_FAILED           QObject::tr("Download the upgrade package failed!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_NOT_ENOUGH_SPACE          QObject::tr("Not enough space!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_UNKOWN_ERROR              QObject::tr("Unknown error!")
#define TRANSLATE_STR_RSERVER_SOFTWARE_MODULE_VERSIONI_EMPTY   QObject::tr("There is no module software installed.")
#define TRANSLATE_STR_RSERVER_SOFTWARE_EXCEED_CAP_OF_PAC_SIZE QObject::tr("The package is too large to download, the limit is 100M")
#define TRANSLATE_STR_RSERVER_SOFTWARE_DOWNLOAD_FAILED_WITH_ERR_CODE QObject::tr("Failed to download, the error code = %1")

/* Release App */
#define TRANSLATE_STR_RSERVER_SOFTWARE_TEST_IN_PROGRESS                 QObject::tr("Test in progress, please close the application(s) first. \nTo release the application(s), please tap \"Release\", \n to resume testing, please tap \"Cancel\".")
#define TRANSLATE_STR_RSERVER_SOFTWARE_RELEASE_APP                              QObject::tr("Test application will be released, tap \"Continue\" to release.")
#define TRANSLATE_STR_RSERVER_SOFTWARE_RELEASE_APP_ONGOING          QObject::tr("Releasing test application, please wait...")
#define TRANSLATE_STR_RSERVER_SOFTWARE_RELEASE_APP_FAILED          QObject::tr("Failed to release test application, please try again!")

/*Stolen or Revoked*/
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_REVOKED                            QObject::tr("Operation Failed! Testset is revoked!")
#define TRANSLATE_STR_RSERVER_REGISTER_UNREGISTER_STOLEN                             QObject::tr("Operation Failed! Testset is stolen!")

#endif  /* QRSERVERSTRINGTABLE_H_ */

