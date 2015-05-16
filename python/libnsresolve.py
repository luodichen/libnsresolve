# encoding=utf-8
'''
Created on May 17, 2015

@author: luodichen
'''

from ctypes import *

clib = cdll.LoadLibrary("/usr/lib/libnsresolve.so")

class c_DNSHeader(Structure):
    _fields_ = [
        ("wTransId", c_uint16),
        ("wFlag", c_uint16),
        ("wQuestions", c_uint16),
        ("wAnswerRRs", c_uint16),
        ("wAuthorityRRs", c_uint16),
        ("wAdditionalRRs", c_uint16),
    ]

class c_NSRBaseRecord(Structure):
    _fields_ = [
        ("nRawDataLen", c_ulong),
        ("pRawData", c_void_p),
    ]

class c_NSResRecord(Structure):
    _filds_ = [
        ("sType", c_uint16),
        ("sClass", c_uint16),
        ("nTTL", c_uint32),
        ("sResLen", c_uint16),
        ("szDomainName", c_char_p),
        ("pResData", c_void_p),
    ]
    
class c_NSRResult(Structure):
    _fields_ = [
        ("header", c_DNSHeader),
        ("pAnswers", POINTER(c_NSResRecord)),
        ("pAuthoritys", POINTER(c_NSResRecord)),
        ("pAdditionals", POINTER(c_NSResRecord)),
    ]
    
def resolve(name, type, server, timeout):
    pResult = POINTER(c_NSRResult)()
    ret = clib.resolve(name, type, server, pointer(pResult), timeout)
    #result = cast(pResult.result, pointer(c_NSRResult))
    #return pResult.result
    print pResult[0].header.wTransId
    return pResult
    
def release(result):
    clib.release(result)

release(resolve("work.luodc.cn", 1, 0x72727272, 15))
