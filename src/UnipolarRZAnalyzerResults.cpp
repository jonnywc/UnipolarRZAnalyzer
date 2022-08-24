#include "UnipolarRZAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "UnipolarRZAnalyzer.h"
#include "UnipolarRZAnalyzerSettings.h"
#include <iostream>
#include <fstream>

UnipolarRZAnalyzerResults::UnipolarRZAnalyzerResults( UnipolarRZAnalyzer* analyzer, UnipolarRZAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

UnipolarRZAnalyzerResults::~UnipolarRZAnalyzerResults()
{
}

void UnipolarRZAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );
    char bubble_string[ 150 ];
	char number_str[40];
    char* color;
	AnalyzerHelpers::GetNumberString( frame.mData1 & 0xFF, display_base, 8, number_str, 40 );
    switch( (frame.mData1 >> 8) & 0xFF )
    {
    case red:;
        color = "red";
        break;
    case green:;
        color = "green";
        break;
    case blue:;
        color = "blue";
        break;
    default:
        color = "unknown";
        break;
    }
    sprintf_s( bubble_string, 150, "value: %s, color: %s, index: %d", number_str, color, int(frame.mData2) );
    AddResultString( bubble_string );
}

void UnipolarRZAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void UnipolarRZAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddTabularText( number_str );
#endif
}

void UnipolarRZAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void UnipolarRZAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}