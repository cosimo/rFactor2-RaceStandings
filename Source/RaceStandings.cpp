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
#include <string>

unsigned long update_scoring_ticks = 0;
char datapath[MAX_FILENAME_LEN] = "";
bool debug = false;
const char * finishStatus[4] = { "Racing", "Finished", "DNF", "DQ" };			

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

  fo = fopen( RACE_STANDINGS_LOG_FILENAME, "a" );
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
  if (debug)
  {
    char temp[80];
    sprintf( temp, "-STARTUP- (version %.3f)", (float) version / 1000.0f );

    // Open ports, read configs, whatever you need to do.  For now, I'll just clear out the
    // example output data files.
    Log( temp );

    sprintf( temp, "-RACE STANDINGS PLUGIN VERSION %.3f-", (float) PLUGIN_VERSION );
    Log( temp );
  }

  // default HW control enabled to true
  mEnabled = true;
}


void RaceStandingsPlugin::Shutdown()
{
  if (debug)
  {
    Log( "-SHUTDOWN-" );
  }
}


void RaceStandingsPlugin::StartSession()
{
  if (debug)
  {
    Log( "--STARTSESSION--" );
  }
  mLastLeaderLaps = 0;
  update_scoring_ticks = 0;
}


void RaceStandingsPlugin::EndSession()
{
  if (debug)
  {
    Log( "--ENDSESSION--" );
  }
  mLastLeaderLaps = 0;
}


void RaceStandingsPlugin::EnterRealtime()
{
  // start up timer every time we enter realtime
  mET = 0.0;
  if (debug)
  {
    Log( "---ENTERREALTIME---" );
  }
}


void RaceStandingsPlugin::ExitRealtime()
{
  if (debug)
  {
    Log( "---EXITREALTIME---" );
  }
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

  bool is_race_session = info.mSession >= 10;
  bool race_begun = info.mGamePhase >= 5;

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
	if (debug && update_scoring_ticks % 50 == 0)
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
    if (debug && update_scoring_ticks % 25 == 0)
	{
	  sprintf(msg, "Leader total-laps-now=%d total-laps-prev=%d", leader.mTotalLaps, mLastLeaderLaps);
      Log(msg);
	}
    return;
  }

  mLastLeaderLaps = leader.mTotalLaps;

  if (debug)
  {
    Log( "Emit new standings file. Leader completed another lap." );
    sprintf(msg, "Found %d (max_place) vehicles out of %d (info.mNumVehicles)", max_place, info.mNumVehicles);
    Log( msg );
  }


  std::string EditGridFile;
  EditGridFile = "UserData\\Log\\Results\\editgrid_";
  EditGridFile.append(std::to_string(leader.mTotalLaps));
  FILE* foa = fopen(EditGridFile.c_str(), "w");
  if (foa != NULL)
  {
	  sprintf(msg, "# editgrid" );

	  fprintf(foa, msg);
	  fprintf(foa, "\n");
	  fclose(foa);
  }

  std::string ChangeLaps;
  ChangeLaps = "UserData\\Log\\Results\\changelaps_";
  ChangeLaps.append(std::to_string(leader.mTotalLaps));
  FILE* fob = fopen(ChangeLaps.c_str(), "w");
  if (fob != NULL)
  {
	  sprintf(msg, "# changelaps");

	  fprintf(fob, msg);
	  fprintf(fob, "\n");
	  fclose(fob);
  }


  for (long place = 1; place <= max_place; place++)
  {
	  unsigned char vehicle_index = classification[place];
	  VehicleScoringInfoV01& vinfo = info.mVehicle[vehicle_index];
	  assert(&vinfo);

	  FILE* foa = fopen(EditGridFile.c_str(), "a");
	  if (foa != NULL)
	  {
		  sprintf(msg, "/editgrid %d %s",
			  place,
			  vinfo.mDriverName
		  );

		  fprintf(foa, msg);
		  fprintf(foa, "\n");
		  fclose(foa);
	  }

	  FILE* fob = fopen(ChangeLaps.c_str(), "a");
	  if (fob != NULL)
	  {
		  short coveredLaps;
		  if (vinfo.mPlace > 1)
		  {
			  coveredLaps = vinfo.mTotalLaps + 1;
		  }
		  else
		  {
			  coveredLaps = vinfo.mTotalLaps;
		  }

		  sprintf(msg, "/changelaps +%d %s",
			  coveredLaps,
			  // vinfo.mLapsBehindLeader,
			  vinfo.mDriverName
		  );

		  fprintf(fob, msg);
		  fprintf(fob, "\n");
	  }
	  fclose(fob);
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
