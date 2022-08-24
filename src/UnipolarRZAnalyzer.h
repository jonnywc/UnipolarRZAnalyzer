#ifndef UNIPOLARRZ_ANALYZER_H
#define UNIPOLARRZ_ANALYZER_H

#include <Analyzer.h>
#include "UnipolarRZAnalyzerResults.h"
#include "UnipolarRZSimulationDataGenerator.h"

typedef enum colors
{
    green,
    red,
    blue,
    number_of_colors
} colors_t;

class UnipolarRZAnalyzerSettings;
class ANALYZER_EXPORT UnipolarRZAnalyzer : public Analyzer2
{
public:
	UnipolarRZAnalyzer();
	virtual ~UnipolarRZAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< UnipolarRZAnalyzerSettings > mSettings;
	std::auto_ptr< UnipolarRZAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	UnipolarRZSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //UNIPOLARRZ_ANALYZER_H
