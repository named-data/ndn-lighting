/*
 * Copyright (c) 2008,2009, University of California, Los Angeles and 
 * Colorado State University All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of NLnetLabs nor the names of its
 *       contributors may be used to endorse or promote products derived from this
 *       software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _PROC_DATA_DAO_H
#define _PROC_DATA_DAO_H

#include <string>

#include "ps_dao.h"
#include "ps_defs.h"

class RawDataDao;
class ScraperDao;

class ProcessedDataDao : public PsDao
{
  // Member Variables
  public:
    static const char *s_kszDaoName;

    static const char *s_szSelect;
    static const char *s_szSelectRawIDs;
    static const char *s_szSelectProcIDs;
    static const char *s_szInsert;
    static const char *s_szInsertRawID;
    static const char *s_szInsertProcID;
    static const char *s_szUpdate;

  private:
    int m_iID;
    int m_iScraperID;
    int m_iScraperParamID;
    time_t m_tDate;
    std::string m_sName;
    std::string m_sData;
    ScraperDao *m_pScraper;
    IdList_t m_oRawIDList;
    IdList_t m_oProcIDList;
    RawDaoList_t m_oRawDaoList;
    ProcDaoList_t m_oProcDaoList;

  // Methods
  public:
    ProcessedDataDao();
    virtual ~ProcessedDataDao();

    RawDaoIter_t beginRawDaos();
    RawDaoIter_t endRawDaos();
    int numRawDaos();
    void addRawDao(RawDataDao *p_pDao);

    ProcDaoIter_t beginProcDaos();
    ProcDaoIter_t endProcDaos();
    int numProcDaos();
    void addProcDao(ProcessedDataDao *p_pDao);

    IdIter_t beginRawIDs();
    IdIter_t endRawIDs();
    int numRawIDs();
    void addRawID(int p_iRawID);

    IdIter_t beginProcIDs();
    IdIter_t endProcIDs();
    int numProcIDs();
    void addProcID(int p_iProcID);

    int getScraperID();
    void setScraperID(int p_iScraperID);

    int getScraperParamID();
    void setScraperParamID(int p_iScraperParamID);

    ScraperDao *getScraper();
    void setScraper(ScraperDao *p_pDao);

    std::string &getName();
    void setName(std::string &p_sName);

    time_t getDate();
    void setDate(time_t p_tDate);

    std::string &getData();

    virtual bool process() = 0;

    virtual bool serialize();
    virtual bool deserialize();
    virtual bool deserialize(DaoList_t &p_oOutputList);

//    virtual std::string daoName();
//    virtual IPsDupable *dup();

  protected:
    void setData(std::string &p_sData);

    bool loadRawDaos();
    bool loadProcDaos();
    bool loadScraperDao();

    void clearRawDaos();
    void clearProcDaos();

  private:
    bool loadRawIDs();
    bool loadProcIDs();

};

#endif