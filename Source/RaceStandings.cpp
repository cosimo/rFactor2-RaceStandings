//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Internals Example Source File                                   ﬁ
//›                                                                         ﬁ
//› Description: Declarations for the Internals Example Plugin              ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› This source code module, and all information, data, and algorithms      ﬁ
//› associated with it, are part of CUBE technology (tm).                   ﬁ
//›                 PROPRIETARY AND CONFIDENTIAL                            ﬁ
//› Copyright (c) 1996-2014 Image Space Incorporated.  All rights reserved. ﬁ
//›                                                                         ﬁ
//›                                                                         ﬁ
//› Change history:                                                         ﬁ
//›   tag.2005.11.30: created                                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "RaceStandings.hpp"    // corresponding header file
#include <assert.h>
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
#include <time.h>

unsigned long update_scoring_ticks = 0;
char datapath[MAX_FILENAME_LEN] = "";


// plugin information

extern "C" __declspec( dllexport )
const char * __cdecl GetPluginName()                   { return( "RaceStandings" ); }

extern "C" __declspec( dllexport )
PluginObjectType __cdecl GetPluginType()               { return( PO_INTERNALS ); }

extern "C" __declspec( dllexport )
int __cdecl GetPluginVersion()                         { return( 7 ); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec( dllexport )
PluginObject * __cdecl CreatePluginObject()            { return( (PluginObject *) new RaceStandingsPlugin ); }

extern "C" __declspec( dllexport )
void __cdecl DestroyPluginObject( PluginObject *obj )  { delete( (RaceStandingsPlugin *) obj ); }


// RaceStandingsPlugin class

void RaceStandingsPlugin::Log(const char *msg)
{
  FILE *fo;
  time_t curtime;
  struct tm localtime;
  char timestamp[26];

  fo = fopen( "RaceStandings.log", "a" );
  if( fo != NULL )
  {
	curtime = time(NULL);
	localtime_s(&localtime, &curtime);
	asctime_s(timestamp, 26, &localtime);
	timestamp[24] = 0;
    fprintf( fo, "[%s] %s\n", timestamp, msg );
    fclose( fo );
  }
}

void RaceStandingsPlugin::Startup( long version )
{
  char temp[80];
  sprintf( temp, "-STARTUP- (version %.3f)", (float) version / 1000.0f );

  // Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
  // example output data files.
  Log( temp );

  sprintf( temp, "-RACE STANDINGS PLUGIN VERSION %.3f-", (float) PLUGIN_VERSION / 100.0f );
  Log( temp );

  // default HW control enabled to true
  mEnabled = true;

}


void RaceStandingsPlugin::Shutdown()
{
  Log( "-SHUTDOWN-" );
}


void RaceStandingsPlugin::StartSession()
{
  Log( "--STARTSESSION--" );
  mLastLeaderLaps = 0;
  update_scoring_ticks = 0;
}


void RaceStandingsPlugin::EndSession()
{
  Log( "--ENDSESSION--" );
  mLastLeaderLaps = 0;
}


void RaceStandingsPlugin::EnterRealtime()
{
  // start up timer every time we enter realtime
  mET = 0.0;
  Log( "---ENTERREALTIME---" );
}


void RaceStandingsPlugin::ExitRealtime()
{
  Log( "---EXITREALTIME---" );
}


void RaceStandingsPlugin::UpdateTelemetry( const TelemInfoV01 &info )
{
  return;
}

void RaceStandingsPlugin::UpdateGraphics( const GraphicsInfoV01 &info )
{
  return;
}

bool RaceStandingsPlugin::CheckHWControl( const char * const controlName, double &fRetVal )
{
  return( false );
}

bool RaceStandingsPlugin::ForceFeedback( double &forceValue )
{
  return( false );
}

void RaceStandingsPlugin::UpdateScoring( const ScoringInfoV01 &info )
{
  char msg[1024];

  bool is_race_session = info.mSession < 10;
  bool race_begun = info.mGamePhase == 0;

  if( ! is_race_session || ! race_begun )
  {
    return;
  }

  // Print general scoring info
  //fprintf( fo, "TrackName=%s\n", info.mTrackName );
  //fprintf( fo, "Session=%d NumVehicles=%d CurET=%.3f mLastEmitted=%.3f\n", info.mSession, info.mNumVehicles, info.mCurrentET, mStandingsLastEmitted );
  //fprintf( fo, "EndET=%.3f MaxLaps=%d LapDist=%.1f\n", info.mEndET, info.mMaxLaps, info.mLapDist );

  // New version 2 stuff
  //fprintf( fo, "GamePhase=%d YellowFlagState=%d SectorFlags=(%d,%d,%d)\n", info.mGamePhase, info.mYellowFlagState,
  //         info.mSectorFlag[0], info.mSectorFlag[1], info.mSectorFlag[2] );
  //fprintf( fo, "InRealtime=%d StartLight=%d NumRedLights=%d\n", info.mInRealtime, info.mStartLight, info.mNumRedLights );
  //fprintf( fo, "PlayerName=%s PlrFileName=%s\n", info.mPlayerName, info.mPlrFileName );
  //fprintf( fo, "DarkCloud=%.2f Raining=%.2f AmbientTemp=%.1f TrackTemp=%.1f\n", info.mDarkCloud, info.mRaining, info.mAmbientTemp, info.mTrackTemp );
  //fprintf( fo, "Wind=(%.1f,%.1f,%.1f) MinPathWetness=%.2f MaxPathWetness=%.2f\n", info.mWind.x, info.mWind.y, info.mWind.z, info.mMinPathWetness, info.mMaxPathWetness );

  unsigned char classification[MAX_VEHICLES];
  unsigned char max_place = 0;

  update_scoring_ticks++;

  //if (update_scoring_ticks % 300 == 0)
  //{
  //	  sprintf(msg, "UpdateScoring() ticks so far: %d", update_scoring_ticks);
  //	  Log( msg );
  //}

  // Find out who is the leader vehicle
  for( unsigned char i = 0; i < info.mNumVehicles; ++i )
  {
    VehicleScoringInfoV01 &vinfo = info.mVehicle[ i ];
	classification[ vinfo.mPlace ] = i;
	if( vinfo.mPlace > max_place )
	{
	  max_place = vinfo.mPlace;
	}
  }

  // No players connected to the server yet
  if (max_place < 1)
  {
	if (update_scoring_ticks % 50 == 0)
    {
	  sprintf(msg, "No players connected to the server yet. Waiting.");
	  Log( msg );
    }
	return;
  }

  assert(max_place >= 1);

  VehicleScoringInfoV01 &leader = info.mVehicle[ classification[ 1 ] ];

  assert(&leader != NULL);

  if (leader.mTotalLaps <= mLastLeaderLaps)
  {
    if (update_scoring_ticks % 25 == 0)
	{
	  sprintf(msg, "Leader total-laps-now=%d total-laps-prev=%d", leader.mTotalLaps, mLastLeaderLaps);
      Log(msg);
	}
    return;
  }

  mLastLeaderLaps = leader.mTotalLaps;
  Log( "Emit new standings file. Leader completed another lap." );

  sprintf(msg, "Found %d (max_place) vehicles out of %d (info.mNumVehicles)", max_place, info.mNumVehicles);
  Log( msg );

  FILE *fo = fopen( "RaceStandings.txt", "w" );
  if( fo != NULL )
  {
    // Print CSV header
    fprintf( fo, "# Pos\tVehicleClass\tDriverName\tCarDescription\tLaps\tLast\tBest\n");
	Log("=== STANDINGS ===");

    for( long place = 1; place <= max_place; place++ )
    {
	  unsigned char vehicle_index = classification[ place ];
      VehicleScoringInfoV01 &vinfo = info.mVehicle[ vehicle_index ];
	  assert(&vinfo);

	  /* Pos, Class, Driver Name, Laps, Car name */
	  sprintf(msg, "%d\t%s\t%s\t%s\t%d\t%.3f\t%.3f",
	    place,
	    vinfo.mVehicleClass,
	    vinfo.mDriverName,
	    vinfo.mVehicleName,
	    vinfo.mTotalLaps,
	    vinfo.mLastLapTime,
	    vinfo.mBestLapTime );

	  Log(msg);
	  fprintf(fo, msg);
	  fprintf(fo, "\n");

      /*
      fprintf( fo, " ID=%d Vehicle=%s\n", vinfo.mID, vinfo.mVehicleName );
      fprintf( fo, " Laps=%d Sector=%d FinishStatus=%d\n", vinfo.mTotalLaps, vinfo.mSector, vinfo.mFinishStatus );
      fprintf( fo, " LapDist=%.1f PathLat=%.2f RelevantTrackEdge=%.2f\n", vinfo.mLapDist, vinfo.mPathLateral, vinfo.mTrackEdge );
      fprintf( fo, " Best=(%.3f, %.3f, %.3f)\n", vinfo.mBestSector1, vinfo.mBestSector2, vinfo.mBestLapTime );
      fprintf( fo, " Last=(%.3f, %.3f, %.3f)\n", vinfo.mLastSector1, vinfo.mLastSector2, vinfo.mLastLapTime );
      fprintf( fo, " Current Sector 1 = %.3f, Current Sector 2 = %.3f\n", vinfo.mCurSector1, vinfo.mCurSector2 );
      fprintf( fo, " Pitstops=%d, Penalties=%d\n", vinfo.mNumPitstops, vinfo.mNumPenalties );
      fprintf( fo, " IsPlayer=%d Control=%d InPits=%d LapStartET=%.3f\n", vinfo.mIsPlayer, vinfo.mControl, vinfo.mInPits, vinfo.mLapStartET );
      fprintf( fo, " Place=%d VehicleClass=%s\n", vinfo.mPlace, vinfo.mVehicleClass );
      fprintf( fo, " TimeBehindNext=%.3f LapsBehindNext=%d\n", vinfo.mTimeBehindNext, vinfo.mLapsBehindNext );
      fprintf( fo, " TimeBehindLeader=%.3f LapsBehindLeader=%d\n", vinfo.mTimeBehindLeader, vinfo.mLapsBehindLeader );
	  */
    }

    // Close file
    fclose( fo );
  }

}

bool RaceStandingsPlugin::RequestCommentary( CommentaryRequestInfoV01 &info )
{
  return( false );
}

const char * RaceStandingsPlugin::GetRF2DataPath()
{
	FILE* datapath_file = fopen("Core\\data.path", "r");
	if (datapath_file != NULL) {
		fscanf(datapath_file, "%s", &datapath);
		fclose(datapath_file);
	}
	return ( datapath );
}
