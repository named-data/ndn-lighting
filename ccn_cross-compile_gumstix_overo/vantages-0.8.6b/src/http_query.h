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

#ifndef _HTTP_QUERY_H
#define _HTTP_QUERY_H

#include <string>
#include <sstream>
#include <iostream>

#include <curl/curl.h>
#include <curl/types.h>

class HttpQuery
{
  // Member Variables
  private:
    bool m_bAuth;
    CURL *m_pHandle;
    struct curl_httppost *m_pFormPost;
    struct curl_httppost *m_pFormLast;
    std::string m_sQuery;
    std::string m_sResp;
    std::string m_sUser;
    std::string m_sPass;
    std::ostringstream m_oSS;
//    std::ostringstream m_oPostDataSS;
    std::string m_sPostData;

  // Methods
  public:
    HttpQuery();
    virtual ~HttpQuery();

    void setPostData(std::string &p_sName, std::string &p_sData);
    bool query(std::string &p_sQuery);
    void addChunk(std::string &p_sChunk);
    std::string &getResponse();

    std::string &getUser();
    std::string &getPass();
    void setUserPass(std::string &p_sUser, std::string &p_sPass);

    bool getAuth();
    void setAuth(bool p_bAuth);

    static std::string urlDecode(std::string &p_sInput);
};

#endif
