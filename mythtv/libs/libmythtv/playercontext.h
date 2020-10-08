#ifndef PLAYER_CONTEXT_H
#define PLAYER_CONTEXT_H

#include <vector>
#include <deque>

// Qt headers
#include <QWidget>
#include <QString>
#include <QMutex>
#include <QHash>
#include <QRect>
#include <QObject>

// MythTV headers
#include "videoouttypes.h"
#include "mythtimer.h"
#include "mythtvexp.h"
#include "mythdeque.h"
#include "mythdate.h"
#include "mythtypes.h"
#include "tv.h"

class TV;
class RemoteEncoder;
class MythPlayer;
class MythMediaBuffer;
class ProgramInfo;
class LiveTVChain;
class QPainter;
class MythMainWindow;

struct osdInfo
{
    InfoMap             text;
    QHash<QString,int>  values;
};

enum PseudoState
{
    kPseudoNormalLiveTV  = 0,
    kPseudoChangeChannel = 1,
    kPseudoRecording     = 2,
};

using StringDeque = deque<QString>;

class MTV_PUBLIC PlayerContext
{
  public:
    explicit PlayerContext(QString inUseID = QString("Unknown"));
    ~PlayerContext();

    // Actions
    bool CreatePlayer(TV* Tv, MythMainWindow* MainWindow, TVState State,
                      bool Embed, const QRect &EmbedBounds = QRect(), bool Muted = false);
    void TeardownPlayer(void);
    bool StartPlaying(int maxWait = -1);
    void StopPlaying(void) const;
    void UpdateTVChain(const QStringList &data = QStringList());
    bool ReloadTVChain(void);
    bool StartEmbedding(const QRect &rect) const;
    void StopEmbedding(void) const;
    void    PushPreviousChannel(void);
    QString PopPreviousChannel(void);

    void ChangeState(TVState newState);
    void ForceNextStateNone(void);
    TVState DequeueNextState(void);

    bool HandlePlayerSpeedChangeFFRew(void);
    bool HandlePlayerSpeedChangeEOF(void);

    // Locking
    void LockState(void) const;
    void UnlockState(void) const;

    void LockPlayingInfo(const char *file, int line) const;
    void UnlockPlayingInfo(const char *file, int line) const;

    void LockDeletePlayer(const char *file, int line) const;
    void UnlockDeletePlayer(const char *file, int line) const;

    void LockOSD(void) const;
    void UnlockOSD(void) const;

    // Sets
    void SetInitialTVState(bool islivetv);
    void SetPlayer(MythPlayer *newplayer);
    void SetRecorder(RemoteEncoder *rec);
    void SetTVChain(LiveTVChain *chain);
    void SetRingBuffer(MythMediaBuffer *Buffer);
    void SetPlayingInfo(const ProgramInfo *info);
    void SetPlayGroup(const QString &group);
    void SetPseudoLiveTV(const ProgramInfo *pi, PseudoState new_state);
    void SetPlayerChangingBuffers(bool val) { m_playerUnsafe = val; }
    void SetNoHardwareDecoders(bool Disallow = true) { m_nohardwaredecoders = Disallow; }

    // Gets
    QString  GetPreviousChannel(void) const;
    bool     CalcPlayerSliderPosition(osdInfo &info,
                                   bool paddedFields = false) const;
    uint     GetCardID(void) const { return m_lastCardid; }
    QString  GetFilters(const QString &baseFilters) const;
    QString  GetPlayMessage(void) const;
    TVState  GetState(void) const;
    bool     GetPlayingInfoMap(InfoMap &infoMap) const;

    // Boolean Gets
    bool IsPlayerChangingBuffers(void) const { return m_playerUnsafe; }
    bool IsEmbedding(void) const;
    bool HasPlayer(void) const;
    bool IsPlayerErrored(void) const;
    bool IsPlayerPlaying(void) const;
    bool IsRecorderErrored(void) const;
    bool InStateChange(void) const;
    /// This is set if the player encountered some irrecoverable error.
    bool IsErrored(void) const { return m_errored; }
    bool IsSameProgram(const ProgramInfo &p) const;
    bool IsValidLiveTV(void) const
        { return m_player && m_tvchain && m_recorder && m_buffer; }

  public:
    QString             m_recUsage;
    MythPlayer         *m_player             {nullptr};
    volatile bool       m_playerUnsafe       {false};
    RemoteEncoder      *m_recorder           {nullptr};
    LiveTVChain        *m_tvchain            {nullptr};
    MythMediaBuffer    *m_buffer             {nullptr};
    ProgramInfo        *m_playingInfo        {nullptr}; ///< Currently playing info
    long long           m_playingLen         {0};  ///< Initial CalculateLength()
    bool                m_nohardwaredecoders {false}; // < Disable use of VDPAU decoding
    int                 m_lastCardid         {-1}; ///< CardID of current/last recorder
    /// 0 == normal, +1 == fast forward, -1 == rewind
    int                 m_ffRewState         {0};
    /// Index into m_ffRewSpeeds for FF and Rewind speeds
    int                 m_ffRewIndex         {0};
    /// Caches value of m_ffRewSpeeds[m_ffRewIndex]
    int                 m_ffRewSpeed         {0};
    TVState             m_playingState       {kState_None};

    bool                m_errored            {false};

    // Previous channel functionality state variables
    StringDeque         m_prevChan; ///< Previous channels

    // Recording to play next, after LiveTV
    ProgramInfo        *m_pseudoLiveTVRec    {nullptr};
    PseudoState         m_pseudoLiveTVState  {kPseudoNormalLiveTV};

    int                 m_fftime             {0};
    int                 m_rewtime            {0};
    int                 m_jumptime           {0};
    /** \brief Time stretch speed, 1.0F for normal playback.
     *
     *  Begins at 1.0F meaning normal playback, but can be increased
     *  or decreased to speedup or slowdown playback.
     *  Ignored when doing Fast Forward or Rewind.
     */
    float               m_tsNormal           {1.0F};
    float               m_tsAlt              {1.5F};

    mutable QMutex      m_playingInfoLock    {QMutex::Recursive};
    mutable QMutex      m_deletePlayerLock   {QMutex::Recursive};
    mutable QMutex      m_stateLock          {QMutex::Recursive};

    // Signal info
    mutable QStringList m_lastSignalMsg;
    mutable MythTimer   m_lastSignalMsgTime;
    mutable InfoMap     m_lastSignalUIInfo;
    mutable MythTimer   m_lastSignalUIInfoTime;

    // tv state related
    MythDeque<TVState>  m_nextState;

    /// Timeout after last Signal Monitor message for ignoring OSD when exiting.
    static const uint kSMExitTimeout;
    static const uint kMaxChannelHistory;
};

#endif // PLAYER_CONTEXT_H
