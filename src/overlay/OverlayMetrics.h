#pragma once

// Copyright 2019 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

// This structure just exists to cache frequently-accessed, overlay-wide
// (non-peer-specific) metrics. Some of these metrics are subsequently
// tabulated at a per-peer level for purposes of identifying and
// disconnecting overloading peers, see LoadManager for details.

namespace medida
{
class Timer;
class Meter;
class Counter;
}

namespace stellar
{

class Application;

struct OverlayMetrics
{
    OverlayMetrics(Application& app);
    medida::Meter& mMessageRead;
    medida::Meter& mMessageWrite;
    medida::Meter& mByteRead;
    medida::Meter& mByteWrite;
    medida::Meter& mErrorRead;
    medida::Meter& mErrorWrite;
    medida::Meter& mTimeoutIdle;
    medida::Meter& mTimeoutStraggler;

    medida::Timer& mRecvErrorTimer;
    medida::Timer& mRecvHelloTimer;
    medida::Timer& mRecvAuthTimer;
    medida::Timer& mRecvDontHaveTimer;
    medida::Timer& mRecvGetPeersTimer;
    medida::Timer& mRecvPeersTimer;
    medida::Timer& mRecvGetTxSetTimer;
    medida::Timer& mRecvTxSetTimer;
    medida::Timer& mRecvTransactionTimer;
    medida::Timer& mRecvGetSCPQuorumSetTimer;
    medida::Timer& mRecvSCPQuorumSetTimer;
    medida::Timer& mRecvSCPMessageTimer;
    medida::Timer& mRecvGetSCPStateTimer;

    medida::Timer& mRecvSCPPrepareTimer;
    medida::Timer& mRecvSCPConfirmTimer;
    medida::Timer& mRecvSCPNominateTimer;
    medida::Timer& mRecvSCPExternalizeTimer;

    medida::Meter& mSendErrorMeter;
    medida::Meter& mSendHelloMeter;
    medida::Meter& mSendAuthMeter;
    medida::Meter& mSendDontHaveMeter;
    medida::Meter& mSendGetPeersMeter;
    medida::Meter& mSendPeersMeter;
    medida::Meter& mSendGetTxSetMeter;
    medida::Meter& mSendTransactionMeter;
    medida::Meter& mSendTxSetMeter;
    medida::Meter& mSendGetSCPQuorumSetMeter;
    medida::Meter& mSendSCPQuorumSetMeter;
    medida::Meter& mSendSCPMessageSetMeter;
    medida::Meter& mSendGetSCPStateMeter;

    medida::Meter& mMessagesBroadcast;
    medida::Counter& mPendingPeersSize;
    medida::Counter& mAuthenticatedPeersSize;

    medida::Meter& mUniqueFloodBytesRecv;
    medida::Meter& mDuplicateFloodBytesRecv;
    medida::Meter& mUniqueFetchBytesRecv;
    medida::Meter& mDuplicateFetchBytesRecv;
};
}
