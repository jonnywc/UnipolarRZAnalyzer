#ifndef UNIPOLARRZ_ANALYZER_SETTINGS
#define UNIPOLARRZ_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class UnipolarRZAnalyzerSettings : public AnalyzerSettings
{
public:
	UnipolarRZAnalyzerSettings();
	virtual ~UnipolarRZAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 m_Code_Periode;

	U32 m_Zero_Code_Max_High_Time;
	U32 m_Zero_Code_Min_High_Time;
	U32 m_Zero_Code_Min_Low_Time;

	U32 m_One_Code_Max_High_Time;
	U32 m_One_Code_Min_High_Time;
	U32 m_One_Code_Min_Low_Time;

	U32 m_Reset_Code_Min_Low_Time;


protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_Code_Periode_Interface;

	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_Zero_Code_Max_High_Time_Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_Zero_Code_Min_High_Time_Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_Zero_Code_Min_Low_Time_Interface;
	
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_One_Code_Max_High_Time_Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_One_Code_Min_High_Time_Interface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_One_Code_Min_Low_Time_Interface;

	std::auto_ptr< AnalyzerSettingInterfaceInteger >	m_Reset_Code_Min_Low_Time_Interface;
};

#endif //UNIPOLARRZ_ANALYZER_SETTINGS
