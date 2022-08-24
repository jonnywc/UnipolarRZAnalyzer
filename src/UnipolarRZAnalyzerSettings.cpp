#include "UnipolarRZAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


UnipolarRZAnalyzerSettings::UnipolarRZAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	m_Code_Periode( 1200 ), 
	m_Zero_Code_Max_High_Time( 400 ),
	m_Zero_Code_Min_High_Time( 200 ),
	m_Zero_Code_Min_Low_Time( 800 ),
	m_One_Code_Max_High_Time( 1000 ),
	m_One_Code_Min_High_Time( 580 ),
	m_One_Code_Min_Low_Time( 200 ),
	m_Reset_Code_Min_Low_Time( 80000 )
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "Channel", "Standard Unipolar Return-to-Zero" );
	mInputChannelInterface->SetChannel( mInputChannel );

	m_Code_Periode_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_Code_Periode_Interface->SetTitleAndTooltip( "Code periode (Min) in ns",  "Specify the length of one bit in nanoseconds." );
	m_Code_Periode_Interface->SetMax( 100000 );
	m_Code_Periode_Interface->SetMin( 500 );
	m_Code_Periode_Interface->SetInteger( m_Code_Periode );

	m_Zero_Code_Max_High_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_Zero_Code_Max_High_Time_Interface->SetTitleAndTooltip( "Zero_Code_Max_High_Time in ns",  "Specify in nanoseconds." );
	m_Zero_Code_Max_High_Time_Interface->SetMax( 100000 );
	m_Zero_Code_Max_High_Time_Interface->SetMin( 100 );
    m_Zero_Code_Max_High_Time_Interface->SetInteger( m_Zero_Code_Max_High_Time );

	m_Zero_Code_Min_High_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_Zero_Code_Min_High_Time_Interface->SetTitleAndTooltip( "Zero_Code_Min_High_Time in ns",  "Specify in nanoseconds." );
	m_Zero_Code_Min_High_Time_Interface->SetMax( 100000 );
    m_Zero_Code_Min_High_Time_Interface->SetMin( 100 );
    m_Zero_Code_Min_High_Time_Interface->SetInteger( m_Zero_Code_Min_High_Time );

	m_Zero_Code_Min_Low_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_Zero_Code_Min_Low_Time_Interface->SetTitleAndTooltip( "Zero_Code_Min_Low_Time in ns",  "Specify in nanoseconds." );
	m_Zero_Code_Min_Low_Time_Interface->SetMax( 100000 );
    m_Zero_Code_Min_Low_Time_Interface->SetMin( 100 );
    m_Zero_Code_Min_Low_Time_Interface->SetInteger( m_Zero_Code_Min_Low_Time );

	m_One_Code_Max_High_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_One_Code_Max_High_Time_Interface->SetTitleAndTooltip( "One_Code_Max_High_Time in ns",  "Specify in nanoseconds." );
	m_One_Code_Max_High_Time_Interface->SetMax( 100000 );
    m_One_Code_Max_High_Time_Interface->SetMin( 100 );
    m_One_Code_Max_High_Time_Interface->SetInteger( m_One_Code_Max_High_Time );

	m_One_Code_Min_High_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_One_Code_Min_High_Time_Interface->SetTitleAndTooltip( "One_Code_Min_High_Time in ns",  "Specify in nanoseconds." );
	m_One_Code_Min_High_Time_Interface->SetMax( 100000 );
    m_One_Code_Min_High_Time_Interface->SetMin( 100 );
    m_One_Code_Min_High_Time_Interface->SetInteger( m_One_Code_Min_High_Time );
	
	m_One_Code_Min_Low_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_One_Code_Min_Low_Time_Interface->SetTitleAndTooltip( "One_Code_Min_Low_Time in ns",  "Specify in nanoseconds." );
	m_One_Code_Min_Low_Time_Interface->SetMax( 100000 );
    m_One_Code_Min_Low_Time_Interface->SetMin( 100 );
    m_One_Code_Min_Low_Time_Interface->SetInteger( m_One_Code_Min_Low_Time );
	
	m_Reset_Code_Min_Low_Time_Interface.reset( new AnalyzerSettingInterfaceInteger() );
	m_Reset_Code_Min_Low_Time_Interface->SetTitleAndTooltip( "Reset_Code_Min_Low_Time in ns",  "Specify in nanoseconds." );
	m_Reset_Code_Min_Low_Time_Interface->SetMax( 1000000 );
	m_Reset_Code_Min_Low_Time_Interface->SetMin( 5000 );
    m_Reset_Code_Min_Low_Time_Interface->SetInteger( m_Reset_Code_Min_Low_Time );

	AddInterface( mInputChannelInterface.get() );
	AddInterface( m_Code_Periode_Interface.get() );
    AddInterface( m_Zero_Code_Max_High_Time_Interface.get() );
    AddInterface( m_Zero_Code_Min_High_Time_Interface.get() );
    AddInterface( m_Zero_Code_Min_Low_Time_Interface.get() );
    AddInterface( m_One_Code_Max_High_Time_Interface.get() );
    AddInterface( m_One_Code_Min_High_Time_Interface.get() );
    AddInterface( m_One_Code_Min_Low_Time_Interface.get() );
    AddInterface( m_Reset_Code_Min_Low_Time_Interface.get() );


	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Serial", false );
}

UnipolarRZAnalyzerSettings::~UnipolarRZAnalyzerSettings()
{
}

bool UnipolarRZAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	m_Code_Periode = m_Code_Periode_Interface->GetInteger();

	m_Zero_Code_Max_High_Time = m_Zero_Code_Max_High_Time_Interface->GetInteger();
    m_Zero_Code_Min_High_Time = m_Zero_Code_Min_High_Time_Interface->GetInteger();
    m_Zero_Code_Min_Low_Time = m_Zero_Code_Min_Low_Time_Interface->GetInteger();

    m_One_Code_Max_High_Time = m_One_Code_Max_High_Time_Interface->GetInteger();
    m_One_Code_Min_High_Time = m_One_Code_Min_High_Time_Interface->GetInteger();
    m_One_Code_Min_Low_Time = m_One_Code_Min_Low_Time_Interface->GetInteger();

    m_Reset_Code_Min_Low_Time = m_Reset_Code_Min_Low_Time_Interface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannel, "Unipolar Return-to-Zero", true );

	return true;
}

void UnipolarRZAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
	m_Code_Periode_Interface->SetInteger( m_Code_Periode );

    m_Zero_Code_Max_High_Time_Interface->SetInteger( m_Zero_Code_Max_High_Time );
    m_Zero_Code_Min_High_Time_Interface->SetInteger( m_Zero_Code_Min_High_Time );
    m_Zero_Code_Min_Low_Time_Interface->SetInteger( m_Zero_Code_Min_Low_Time );

    m_One_Code_Max_High_Time_Interface->SetInteger( m_One_Code_Max_High_Time );
    m_One_Code_Min_High_Time_Interface->SetInteger( m_One_Code_Min_High_Time );
    m_One_Code_Min_Low_Time_Interface->SetInteger( m_One_Code_Min_Low_Time );

    m_Reset_Code_Min_Low_Time_Interface->SetInteger( m_Reset_Code_Min_Low_Time );
}

void UnipolarRZAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> m_Code_Periode;

	text_archive >> m_Zero_Code_Max_High_Time;
    text_archive >> m_Zero_Code_Min_High_Time;
    text_archive >> m_Zero_Code_Min_Low_Time;

    text_archive >> m_One_Code_Max_High_Time;
    text_archive >> m_One_Code_Min_High_Time;
    text_archive >> m_One_Code_Min_Low_Time;

    text_archive >> m_Reset_Code_Min_Low_Time;

	ClearChannels();
	AddChannel( mInputChannel, "Unipolar Return-to-Zero", true );

	UpdateInterfacesFromSettings();
}

const char* UnipolarRZAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << m_Code_Periode;

	text_archive << m_Zero_Code_Max_High_Time;
    text_archive << m_Zero_Code_Min_High_Time;
    text_archive << m_Zero_Code_Min_Low_Time;

    text_archive << m_One_Code_Max_High_Time;
    text_archive << m_One_Code_Min_High_Time;
    text_archive << m_One_Code_Min_Low_Time;

    text_archive << m_Reset_Code_Min_Low_Time;

	return SetReturnString( text_archive.GetString() );
}
