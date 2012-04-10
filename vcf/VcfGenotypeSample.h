/*
 *  Copyright (C) 2012  Regents of the University of Michigan
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


#ifndef __VCF_GENOTYPE_SAMPLE_H__
#define __VCF_GENOTYPE_SAMPLE_H__

#include "VcfGenotypeFormat.h"

/// This header file provides interface to read/write VCF files.
class VcfGenotypeSample : public VcfGenotypeField
{
public:
    /// Default Constructor, initializes the variables.
    VcfGenotypeSample();

    /// Destructor
    virtual ~VcfGenotypeSample();
    
    /// Read this sample from the file up until the next \t,\n, or EOF.
    /// \param filePtr IFILE to read from.
    /// \param format the VCF Genotype Format field description.
    /// \return true if a tab ended the field, false if it was \n or EOF.
    bool read(IFILE filePtr, VcfGenotypeFormat& format);

    /// Get a pointer to the string containing the value associated with the
    /// specified key(the pointer will be invalid if the field is
    /// changed/reset).  
    /// \param key to find the falue for.
    /// \return const pointer to the string value for this key, NULL if
    /// the sample or the key was not found.
    const std::string* getString(const std::string& key);

    /// Set the string associated with the specified key, returns true if set,
    /// false if not.
    bool setString(const std::string& key, const std::string& value);

    inline bool isPhased()              { return(myPhased); }
    inline bool isUnphased()            { return(myUnphased); }
    inline bool hasAllGenotypeAlleles() { return(myHasAllGenotypeAlleles); }

protected:
    /// reset the sample for a new entry.
    virtual void internal_reset();

    void parseGT();

private:
    VcfGenotypeFormat* myFormatPtr;

    bool myPhased;
    bool myUnphased;
    bool myHasAllGenotypeAlleles;
};

#endif
