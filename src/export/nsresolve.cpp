/*
 * nsresolve.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "../include/nsresolve.h"

static size_t record_size(const LResourceRecord *pRecord)
{
    // TODO:::::::::
    return sizeof(NSRESRECORD) + pRecord->GetBufferSize() + pRecord->GetDomainName().length() + 1 + 128;
}

static size_t record_set_size(std::vector<const LResourceRecord *> &set)
{
    size_t ret = 0;

    std::vector<const LResourceRecord *>::const_iterator iter = set.begin();
    for (; iter != set.end(); iter++)
    {
        ret += record_size(*iter);
    }

    return ret;
}

static uint8_t *fill_res_data(uint8_t *pCur, const LResourceRecord *pResourceRecord)
{
    const BaseRecord *pBaseRecord = pResourceRecord->GetRecord();
    switch (pBaseRecord->GetType())
    {
    case QTYPE::A: {
        ARecord *pRecord = (ARecord *)pBaseRecord;
        PNSRARECORD pTarget = (PNSRARECORD)pCur;
        pTarget->address = pRecord->GetIPAddress();

        pCur += sizeof(*pTarget);
        break;
    }
    case QTYPE::MX: {
        MXRecord *pRecord = (MXRecord *)pBaseRecord;
        PNSRMXRECORD pTarget = (PNSRMXRECORD)pCur;
        pTarget->sPreference = pRecord->GetPreference();

        pCur += sizeof(*pTarget);
        pTarget->szDomainName = (const char *)pCur;

        std::string strDomainName = pRecord->GetDomainName();
        memcpy((void *)pCur, (void *)strDomainName.c_str(), strDomainName.length());
        pCur += strDomainName.length();

        *(pCur++) = '\0';

        break;
    }

    case QTYPE::NS: {
        NSRecord *pRecord = (NSRecord *)pBaseRecord;
        PNSRNSRECORD pTarget = (PNSRNSRECORD)pCur;

        pCur += sizeof(*pTarget);

        pTarget->szDomainName = (const char *)pCur;
        std::string strDomainName = pRecord->GetDomainName();
        memcpy((void *)pCur, (void *)strDomainName.c_str(), strDomainName.length());
        pCur += strDomainName.length();

        *(pCur++) = '\0';

        break;
    }

    default: {
        BaseRecord *pRecord = (BaseRecord *)pBaseRecord;
        PNSRBASERECORD pTarget = (PNSRBASERECORD)pCur;

        pTarget->nRawDataLen = pRecord->GetRawDataLength();
        pCur += sizeof(*pTarget);

        pTarget->pRawData = (uint8_t *)pCur;
        memcpy((void *)pCur, (void *)pRecord->GetRawData(), pRecord->GetRawDataLength());
        pCur += pRecord->GetRawDataLength();

        break;
    }
    }

    return pCur;
}

static uint8_t *fill_result_buf(uint8_t *pCur, const NSRESRECORD **ppRecord,
        std::vector<const LResourceRecord *> &set)
{
    int i = 0;

    std::vector<const LResourceRecord *>::const_iterator iter = set.begin();
    for (; iter != set.end(); iter++)
    {
        NSRESRECORD *pRecord = (NSRESRECORD *)pCur;
        ppRecord[i] = pRecord;

        pRecord->sType = (*iter)->GetType();
        pRecord->sClass = (*iter)->GetClass();
        pRecord->nTTL = (*iter)->GetTTL();
        pRecord->sResLen = (*iter)->GetBufferSize();
        pCur += sizeof(*pRecord);

        pRecord->szDomainName = (const char *)pCur;
        std::string strDomainName = (*iter)->GetDomainName();

        memcpy((void *)pCur, strDomainName.c_str(), strDomainName.length());
        pCur += strDomainName.length();
        *(pCur++) = '\0';

        pRecord->pResData = pCur;
        /*
        size_t nResLen = (*iter)->GetBufferSize();
        memcpy((void *)pCur, (*iter)->GetBuffer(NULL), nResLen);
        pCur += nResLen;
        */
        pCur = fill_res_data(pCur, *iter);
        i++;
    }
    ppRecord[i] = NULL;

    return pCur;
}

int resolve(const char *szName, uint16_t sType, in_addr_t server, NSRRESULT **pResult, time_t timeout)
{
    LDNSResolver resolver(server, timeout);
    int result = resolver.Query(szName, sType);
    if (result < 0)
        return result;

    const LDNSParser *parser = resolver.GetParser();
    std::vector<const LResourceRecord *> answers = parser->GetAnswers();
    std::vector<const LResourceRecord *> authoritys = parser->GetAuthoritys();
    std::vector<const LResourceRecord *> additionals = parser->GetAdditionals();

    size_t bufsize = 0;
    bufsize += record_set_size(answers);
    bufsize += record_set_size(authoritys);
    bufsize += record_set_size(additionals);

    size_t nResultBodySize = sizeof(PNSRESRECORD)
            * (answers.size() + authoritys.size() + additionals.size() + 3);

    bufsize += nResultBodySize;
    bufsize += sizeof(NSRRESULT);

    uint8_t *pBuffer = new uint8_t[bufsize];
    memset((void *)pBuffer, 0, bufsize);

    *pResult = (PNSRRESULT)pBuffer;
    uint8_t *pCur = pBuffer + sizeof(NSRRESULT);
    (*pResult)->pAnswers = (const NSRESRECORD **)pCur;
    pCur += (answers.size() + 1) * sizeof(PNSRESRECORD);
    (*pResult)->pAuthoritys = (const NSRESRECORD **)pCur;
    pCur += (authoritys.size() + 1) * sizeof(PNSRESRECORD);
    (*pResult)->pAdditionals = (const NSRESRECORD **)pCur;
    pCur += (additionals.size() + 1) * sizeof(PNSRESRECORD);

    pCur = fill_result_buf(pCur, (*pResult)->pAnswers, answers);
    pCur = fill_result_buf(pCur, (*pResult)->pAuthoritys, authoritys);
    pCur = fill_result_buf(pCur, (*pResult)->pAdditionals, additionals);

    return 0;
}

void release(NSRRESULT *pResult)
{
    if (NULL != pResult)
        delete [] ((uint8_t *)pResult);
}
