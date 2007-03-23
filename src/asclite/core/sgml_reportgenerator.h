/*
 * asclite
 * Author: Jerome Ajot, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and Technology by
 * employees of the Federal Government in the course of their official duties.  Pursuant to
 * Title 17 Section 105 of the United States Code this software is not subject to copyright
 * protection within the United States and is in the public domain. asclite is
 * an experimental system.  NIST assumes no responsibility whatsoever for its use by any party.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef SGML_REPORTGENERATOR_H
#define SGML_REPORTGENERATOR_H

#include "reportgenerator.h"
#include "alignment.h"
#include "segment.h"
#include "logger.h"

struct AlignedSegmentSequenceComparator
{
	inline bool operator() (AlignedSegment* first, AlignedSegment* second) const
	{
		return (first->GetReferenceSegment()->GetSourceElementNum() < second->GetReferenceSegment()->GetSourceElementNum());
	}
};

class Speaker
{
	public:
		Speaker(string _name, int seq)
		{
			name = _name;
			sequence = seq;
		};
		
		~Speaker()
		{
			segments.clear();
		};
		
		void SortSegments()
		{
			sort(segments.begin(), segments.end(), AlignedSegmentSequenceComparator());
		};
		
		vector < AlignedSegment* > GetSegments() const
		{
			return segments;
		};
		
		void AddSegment(AlignedSegment* segment)
		{
			int seq = segment->GetReferenceSegment()->GetSourceElementNum();
			if(seq < sequence) sequence = seq;
			segments.push_back(segment);
		};
		
		string GetName()
		{
			return name;
		};
		
		int GetSequence()
		{
			return sequence;
		};
		
	private:
		int sequence;
		string name;
		vector< AlignedSegment* > segments; 
};

/** Predicate on Speaker: determines if the passed Speaker has the given name */
class SpeakerNamePredicate
{
	public:
		SpeakerNamePredicate(string _wantedName = "")
		{
			wantedName = _wantedName;
		};
		
		~SpeakerNamePredicate()
		{
		};
		
		void SetWantedName(string name)
		{
			wantedName = name;
		};
		
		inline bool operator() (Speaker* speaker)
		{
			return wantedName == speaker->GetName();
		}
		
	private:
		string wantedName;
};

/** Compares two Speakers based on their sequence number. */
struct SpeakerSequenceComparator
{
	inline bool operator() (Speaker* key1, Speaker* key2) const
	{
		return (key1->GetSequence() < key2->GetSequence());
	}
};

/**
 * Generate a SGML like report based on
 * how sclite does it.
 */
class SGMLReportGenerator : public ReportGenerator
{
	private:
		bool m_bCaseSensitive;
		bool m_bRefHasTimes;
		bool m_bRefHasConf;
		bool m_bHypHasConf;
		bool m_bHypHasTimes;
		
		static Logger* m_pLogger;
	public:
		/** class constructor */
		SGMLReportGenerator() {}
		/** class destructor */
		virtual ~SGMLReportGenerator() {}
		/** Generate the SGML report */
        void Generate(Alignment* alignment, int where);
		/** Generate the SGML report by system */
        void GenerateSystem(Alignment* alignment, string systm, ostream &output);
	
	private:
		void GenerateSpeaker(Speaker* speaker, string systm, ostream &output);
	
		void GeneratePath(AlignedSegment* alignedSegment, string systm, ostream &output);
	
        void GenerateTokenAlignment(TokenAlignment* tokenAlign, string systm, ostream &output);
		
		void HandleWordAux(ostream &output);
		
		void PreProcessWordAux(Speaker* speaker, string systm);
		
		void OutputTextFor(Token* token, ostream &output);
};

#endif // SGML_REPORTGENERATOR_H
