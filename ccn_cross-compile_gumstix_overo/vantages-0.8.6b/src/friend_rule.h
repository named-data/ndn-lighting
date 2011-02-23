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

#ifndef _FRIEND_RULE_H
#define _FRIEND_RULE_H

#include <string>
#include <map>

#include "ps_defs.h"

class DnskeyConsistencyDao;
class GpgmeCryptMgr;
class PsCryptMgr;

class FriendRule
{
  // Types and Enums
  public:
    typedef std::map<std::string, DnskeyConsistencyDao *> KeyMap_t;
    typedef KeyMap_t::iterator KeyMapIter_t;

  // Member Variables
  private:
    GpgmeCryptMgr *m_pCryptMgr;
    StrList_t m_oNameList;
    DaoList_t m_oDaoList;
    KeyMap_t m_oKeyMap;

  // Methods
  public:
    FriendRule();
    virtual ~FriendRule();

    bool init(UrlIter_t p_tBegin,
              UrlIter_t p_tEnd,
              int p_iThresh,
              PsCryptMgr *p_pCryptMgr);
    bool process();
    StrListIter_t beginZones();
    StrListIter_t endZones();
    size_t numZones();
};

#endif
