// Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UPDATE_ENGINE_CONSTANTS_H_
#define UPDATE_ENGINE_CONSTANTS_H_

namespace chromeos_update_engine {

// The name of the marker file used to trigger powerwash when post-install
// completes successfully so that the device is powerwashed on next reboot.
extern const char kPowerwashMarkerFile[];

// Path to the marker file we use to indicate we've recorded a system reboot.
extern const char kSystemRebootedMarkerFile[];

// The contents of the powerwash marker file.
extern const char kPowerwashCommand[];

// Directory for AU prefs that are preserved across powerwash.
extern const char kPowerwashSafePrefsDir[];

// The location where we store the AU preferences (state etc).
extern const char kPrefsDirectory[];

// Path to the stateful partition on the root filesystem.
extern const char kStatefulPartition[];

// Constants related to preferences.
extern const char kPrefsAttemptInProgress[];
extern const char kPrefsBackoffExpiryTime[];
extern const char kPrefsCertificateReportToSendDownload[];
extern const char kPrefsCertificateReportToSendUpdate[];
extern const char kPrefsCurrentBytesDownloaded[];
extern const char kPrefsCurrentResponseSignature[];
extern const char kPrefsCurrentUrlFailureCount[];
extern const char kPrefsCurrentUrlIndex[];
extern const char kPrefsDailyMetricsLastReportedAt[];
extern const char kPrefsDeltaUpdateFailures[];
extern const char kPrefsFullPayloadAttemptNumber[];
extern const char kPrefsInstallDateDays[];
extern const char kPrefsLastActivePingDay[];
extern const char kPrefsLastRollCallPingDay[];
extern const char kPrefsManifestMetadataSize[];
extern const char kPrefsMetricsAttemptLastReportingTime[];
extern const char kPrefsMetricsCheckLastReportingTime[];
extern const char kPrefsNumReboots[];
extern const char kPrefsNumResponsesSeen[];
extern const char kPrefsOmahaCohort[];
extern const char kPrefsOmahaCohortHint[];
extern const char kPrefsOmahaCohortName[];
extern const char kPrefsP2PEnabled[];
extern const char kPrefsP2PFirstAttemptTimestamp[];
extern const char kPrefsP2PNumAttempts[];
extern const char kPrefsPayloadAttemptNumber[];
extern const char kPrefsPreviousVersion[];
extern const char kPrefsResumedUpdateFailures[];
extern const char kPrefsRollbackVersion[];
extern const char kPrefsSystemUpdatedMarker[];
extern const char kPrefsTargetVersionAttempt[];
extern const char kPrefsTargetVersionInstalledFrom[];
extern const char kPrefsTargetVersionUniqueId[];
extern const char kPrefsTotalBytesDownloaded[];
extern const char kPrefsUpdateCheckCount[];
extern const char kPrefsUpdateCheckResponseHash[];
extern const char kPrefsUpdateDurationUptime[];
extern const char kPrefsUpdateFirstSeenAt[];
extern const char kPrefsUpdateOverCellularPermission[];
extern const char kPrefsUpdateServerCertificate[];
extern const char kPrefsUpdateStateNextDataLength[];
extern const char kPrefsUpdateStateNextDataOffset[];
extern const char kPrefsUpdateStateNextOperation[];
extern const char kPrefsUpdateStateSHA256Context[];
extern const char kPrefsUpdateStateSignatureBlob[];
extern const char kPrefsUpdateStateSignedSHA256Context[];
extern const char kPrefsUpdateTimestampStart[];
extern const char kPrefsUrlSwitchCount[];
extern const char kPrefsWallClockWaitPeriod[];

// A download source is any combination of protocol and server (that's of
// interest to us when looking at UMA metrics) using which we may download
// the payload.
typedef enum {
  kDownloadSourceHttpsServer,  // UMA Binary representation: 0001
  kDownloadSourceHttpServer,   // UMA Binary representation: 0010
  kDownloadSourceHttpPeer,     // UMA Binary representation: 0100

  // Note: Add new sources only above this line.
  kNumDownloadSources
} DownloadSource;

// A payload can be a Full or Delta payload. In some cases, a Full payload is
// used even when a Delta payload was available for the update, called here
// ForcedFull. The PayloadType enum is only used to send UMA metrics about the
// successfully applied payload.
typedef enum {
  kPayloadTypeFull,
  kPayloadTypeDelta,
  kPayloadTypeForcedFull,

  // Note: Add new payload types only above this line.
  kNumPayloadTypes
} PayloadType;

// Maximum number of times we'll allow using p2p for the same update payload.
const int kMaxP2PAttempts = 10;

// Maximum wallclock time we allow attempting to update using p2p for
// the same update payload - five days.
const int kMaxP2PAttemptTimeSeconds = 5 * 24 * 60 * 60;

// The maximum amount of time to spend waiting for p2p-client(1) to
// return while waiting in line to use the LAN - six hours.
const int kMaxP2PNetworkWaitTimeSeconds = 6 * 60 * 60;

// The maximum number of payload files to keep in /var/cache/p2p.
const int kMaxP2PFilesToKeep = 3;

// The maximum number of days to keep a p2p file;
const int kMaxP2PFileAgeDays = 5;

// The default number of UMA buckets for metrics.
const int kNumDefaultUmaBuckets = 50;

// General constants
const int kNumBytesInOneMiB = 1024 * 1024;

// Number of redirects allowed when downloading.
const int kDownloadMaxRedirects = 10;

// The minimum average speed that downloads must sustain...
//
// This is set low because some devices may have very poor
// connectivity and we want to make as much forward progress as
// possible. For p2p this is high (25 kB/second) since we can assume
// high bandwidth (same LAN) and we want to fail fast.
const int kDownloadLowSpeedLimitBps = 1;
const int kDownloadP2PLowSpeedLimitBps = 25 * 1000;

// ... measured over this period.
//
// For non-official builds (e.g. typically built on a developer's
// workstation and served via devserver) bump this since it takes time
// for the workstation to generate the payload. For p2p, make this
// relatively low since we want to fail fast.
const int kDownloadLowSpeedTimeSeconds = 90;
const int kDownloadDevModeLowSpeedTimeSeconds = 180;
const int kDownloadP2PLowSpeedTimeSeconds = 60;

// The maximum amount of HTTP server reconnect attempts.
//
// This is set high in order to maximize the attempt's chance of
// succeeding. When using p2p, this is low in order to fail fast.
const int kDownloadMaxRetryCount = 20;
const int kDownloadMaxRetryCountOobeNotComplete = 3;
const int kDownloadP2PMaxRetryCount = 5;

// The connect timeout, in seconds.
//
// This is set high because some devices may have very poor
// connectivity and we may be using HTTPS which involves complicated
// multi-roundtrip setup. For p2p, this is set low because we can
// the server is on the same LAN and we want to fail fast.
const int kDownloadConnectTimeoutSeconds = 30;
const int kDownloadP2PConnectTimeoutSeconds = 5;

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_CONSTANTS_H_
