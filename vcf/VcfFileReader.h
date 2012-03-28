/*
 *  Copyright (C) 2010-2011  Regents of the University of Michigan,
 *                           Hyun Min Kang, Matthew Flickenger, Matthew Snyder,
 *                           and Goncalo Abecasis
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VCF_FILE_READER_H__
#define __VCF_FILE_READER_H__

#include "VcfFile.h"
#include "VcfRecord.h"
#include "VcfSubsetSamples.h"

/// This header file provides interface to read/write VCF files.
class VcfFileReader : public VcfFile
{
public:
    static const int DISCARD_NON_PHASED = 0x1;
    static const int DISCARD_MISSING_GT = 0x2;
    static const int DISCARD_FILTERED = 0x4;
    static const int DISCARD_MULTIPLE_ALTS = 0x8;


    /// Default Constructor, initializes the variables, but does not open
    /// any files.
    VcfFileReader();
    /// Destructor
    virtual ~VcfFileReader();

    /// Open the vcf file with the specified filename for reading.
    /// This method does no sample subsetting.
    /// \param  filename the vcf file to open for reading.
    /// \param header to be read from the file
    /// \return true = success; false = failure.
    virtual bool open(const char* filename, VcfHeader& header);

    /// Open the vcf file with the specified filename for reading
    /// subsetting the samples in the file to just the samples specified
    /// in the sample file.
    /// \param  filename the vcf file to open for reading.
    /// \param header to be read from the file
    /// \param sampleFileName file containing samples to keep
    /// \param delims deliminators separating the samples in the file ('\n'
    /// is always considered a delimiter even if it isn't specified).  When
    /// any of the specified delimiter characters is found in the file it
    /// indicates the end of a sample name.
    /// \return true = success; false = failure.
    virtual bool open(const char* filename, VcfHeader& header,
                      const char* sampleFileName, const char* delims = "\n");

    /// Read the next Vcf data line from the file.
    /// \param record record to populate with the next data line.
    /// \return true if successful, false if not.
    bool readRecord(VcfRecord& record);

    /// Returns whether or not the end of the file has been reached.
    /// \return true = EOF; false = not eof.
    /// If the file is not open, true is returned.
    bool isEOF();

    /// Get the number of VCF records that have been processed (read/written)
    /// so far excluding any discarded records.
    int getNumKeptRecords() {return(myNumKeptRecords);}

    /// Set which rules should be applied for discarding records.
    /// OR in all discard rules to be applied.
    /// For example:: reader.setDiscards(DISCARD_NON_PHASED | 
    ///                                  DISCARD_MISSING_GT);
    /// NOTE: Discard rules are NOT reset when a file is reset, closed, or a new
    /// one opened, but are reset when this is called.
    void setDiscardRules(uint32_t discards) { myDiscardRules = discards; }

    /// Add additional discard rule,  OR in all additional discard rules to
    /// be applied.
    /// For example:: reader.setDiscards(DISCARD_NON_PHASED | 
    ///                                  DISCARD_MISSING_GT);
    /// NOTE: Discard rules are NOT reset when a file is reset, closed, or a new
    /// one opened, and are NOT reset when this is called.
    void addDiscardRules(uint32_t discards) { myDiscardRules |= discards; }

protected: 
    virtual void resetFile();

private:
    VcfSubsetSamples mySampleSubset;
    bool myUseSubset;

    uint32_t myDiscardRules;

    // Number of records read/written so far that were not discarded.
    int myNumKeptRecords;
};

#endif
