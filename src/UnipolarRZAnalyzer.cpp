#include "UnipolarRZAnalyzer.h"
#include "UnipolarRZAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

UnipolarRZAnalyzer::UnipolarRZAnalyzer()
:	Analyzer2(),  
	mSettings( new UnipolarRZAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
    UseFrameV2();
}

UnipolarRZAnalyzer::~UnipolarRZAnalyzer()
{
	KillThread();
}

void UnipolarRZAnalyzer::SetupResults()
{
	mResults.reset( new UnipolarRZAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void UnipolarRZAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();
	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );
    double sample_periode_in_ns = 1 / double(mSampleRateHz) / 1.0e-9 /*ns per s*/;

    U32 code_periode_samples = U32( double( mSettings->m_Code_Periode ) / sample_periode_in_ns );

    U32 zero_max_high_samples = U32( double( mSettings->m_Zero_Code_Max_High_Time ) / sample_periode_in_ns );
    U32 zero_min_high_samples = U32( double( mSettings->m_Zero_Code_Min_High_Time ) / sample_periode_in_ns );
    U32 zero_min_low_samples = U32( double( mSettings->m_Zero_Code_Min_Low_Time ) / sample_periode_in_ns );

    U32 one_max_high_samples = U32( double( mSettings->m_One_Code_Max_High_Time ) / sample_periode_in_ns );
    U32 one_min_high_samples = U32( double( mSettings->m_One_Code_Min_High_Time ) / sample_periode_in_ns );
    U32 one_min_low_samples = U32( double( mSettings->m_One_Code_Min_Low_Time ) / sample_periode_in_ns );

    U32 reset_min_low_samples = U32( double( mSettings->m_Reset_Code_Min_Low_Time ) / sample_periode_in_ns );

	bool reset_detected = false;
    U64 last_rising_sample_number = 0;
    U64 current_rising_sample_number = 0;
    U64 last_falling_sample_number = 0;
    U64 current_sample_number = 0;

	// find first reset condition
    while( !reset_detected )
    {
        if( mSerial->GetBitState() == BIT_LOW )
        {
            last_falling_sample_number = mSerial->GetSampleNumber();
            mSerial->AdvanceToNextEdge();
            current_rising_sample_number = mSerial->GetSampleNumber();
            if( ( current_rising_sample_number - last_falling_sample_number ) > reset_min_low_samples )
            {
                reset_detected = true;
			}
		}
        else
        {
            mSerial->AdvanceToNextEdge();
		}
        if( !mSerial->DoMoreTransitionsExistInCurrentData() )
        {
            return;
		}
	}
	// after reset condition at rising edge 

	for( ; mSerial->DoMoreTransitionsExistInCurrentData() ; )
	{
        reset_detected = false;
        for( U32 index = 0; !reset_detected; index++ )
        {
            U8 data[ number_of_colors ] = { 0 };
            U8 error = 0;
            U64 starting_sample = mSerial->GetSampleNumber();

            for( colors_t color = colors_t( 0 ); color < number_of_colors; color = colors_t(color + 1) )
            {
                U64 color_starting_sample = mSerial->GetSampleNumber();
                for( U32 i = 0; i < 8; i++ )
                {
                    // shift one bit
                    data[ color ] = data[ color ] << 1;

                    // get all time points of the edges of one bit
                    last_rising_sample_number = current_rising_sample_number;
                    mSerial->AdvanceToNextEdge(); // falling edge -- beginning of the start bit
                    last_falling_sample_number = mSerial->GetSampleNumber();
                    mSerial->AdvanceToNextEdge();
                    current_rising_sample_number = mSerial->GetSampleNumber();

                    // calculate low and high time
                    U32 high_time_samples = last_falling_sample_number - last_rising_sample_number;
                    U32 low_time_samples = current_rising_sample_number - last_falling_sample_number;

                    // check high time
                    if( high_time_samples > one_min_high_samples && high_time_samples < one_max_high_samples )
                    {
                        // its a one
                        data[ color ] |= 1;
                        // let's put a dot exactly where we sample this bit:
                        mResults->AddMarker( last_falling_sample_number, AnalyzerResults::One, mSettings->mInputChannel );
                        // check low time
                        if( low_time_samples < one_min_low_samples )
                        {
                            // its a error
                            error += 1;
                            // let's put a dot exactly where we sample this bit:
                            mResults->AddMarker( current_rising_sample_number, AnalyzerResults::ErrorSquare, mSettings->mInputChannel );
                        }
                    }
                    else if( high_time_samples > zero_min_high_samples && high_time_samples < zero_max_high_samples )
                    {
                        // its a zero
                        // let's put a dot exactly where we sample this bit:
                        mResults->AddMarker( last_falling_sample_number, AnalyzerResults::Zero, mSettings->mInputChannel );
                        // check low time
                        if( low_time_samples < zero_min_low_samples )
                        {
                            // its a error
                            error += 1;
                            // let's put a dot exactly where we sample this bit:
                            mResults->AddMarker( current_rising_sample_number, AnalyzerResults::ErrorSquare, mSettings->mInputChannel );
                        }
                    }
                    else
                    {
                        // its a error
                        error += 1;
                        // let's put a dot exactly where we sample this bit:
                        mResults->AddMarker( last_falling_sample_number, AnalyzerResults::ErrorX, mSettings->mInputChannel );
                    }

                    // check low time for reset
                    if( low_time_samples > reset_min_low_samples )
                    {
                        reset_detected = true;
                        // let's put a dot exactly where we sample this bit:
                        mResults->AddMarker( last_falling_sample_number + reset_min_low_samples +1 , AnalyzerResults::Stop,
                                             mSettings->mInputChannel );
                        if( i != 7 )
                        {
                            // error, sequence not finished
                            error += 1;
                            // let's put a dot exactly where we sample this bit:
                            mResults->AddMarker( last_falling_sample_number + code_periode_samples + 1, AnalyzerResults::ErrorDot,
                                                 mSettings->mInputChannel );
                        }
                        break;
                    }
                }
                // we have a byte to save
                Frame frame;
                frame.mData1 = data[ color ] | (color << 8) ;
                frame.mData2 = index;
                frame.mFlags = error;
                frame.mStartingSampleInclusive = color_starting_sample;
                frame.mEndingSampleInclusive = last_falling_sample_number;
                mResults->AddFrame ( frame );

                if( reset_detected )
                {
                    break;
                }
            }
            // all colors are filled 
            FrameV2 frame_v2;
            frame_v2.AddInteger( "index", index );
            frame_v2.AddByte( "red", data[ red ] );
            frame_v2.AddByte( "green", data[ green ] );
            frame_v2.AddByte( "blue", data[ blue ] );
            frame_v2.AddByte( "error", error );

            mResults->AddFrameV2( frame_v2, "pixel", starting_sample, mSerial->GetSampleNumber());

            mResults->CommitResults();
            ReportProgress( mSerial->GetSampleNumber() );
        }
	}
}

bool UnipolarRZAnalyzer::NeedsRerun()
{
	return false;
}

U32 UnipolarRZAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 UnipolarRZAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->m_Code_Periode * 4;
}

const char* UnipolarRZAnalyzer::GetAnalyzerName() const
{
	return "Unipolar Return-to-Zero";
}

const char* GetAnalyzerName()
{
	return "Unipolar Return-to-Zero";
}

Analyzer* CreateAnalyzer()
{
	return new UnipolarRZAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}