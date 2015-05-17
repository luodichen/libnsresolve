# encoding=utf-8
'''
   Copyright (C) 2015 luodichen

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
'''

'''
Created on May 17, 2015

@author: luodichen
'''

import platform
from ctypes import *

if platform.system() == "Windows":
    clib = cdll.LoadLibrary("libnsresolve.dll")
else:
    clib = cdll.LoadLibrary("libnsresolve.so")

TYPE_A = 0x0001
TYPE_NS = 0x0002
TYPE_CNAME = 0x0005
TYPE_MX = 0x000f

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
    
class c_NSRARecord(Structure):
    _fields_ = [
        ("address", c_uint32),
    ]
    
class c_NSRNSRecord(Structure):
    _fields_ = [
        ("szDomainName", c_char_p),
    ]
    
class c_NSRCNAMERecord(Structure):
    _fields_ = [
        ("szDomainName", c_char_p),
    ]
    
class c_NSRMXRecord(Structure):
    _fields_ = [
        ("sPreference", c_uint16),
        ("szDomainName", c_char_p),
    ]

class c_NSResRecord(Structure):
    _fields_ = [
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
        ("pAnswers", POINTER(POINTER(c_NSResRecord))),
        ("pAuthoritys", POINTER(POINTER(c_NSResRecord))),
        ("pAdditionals", POINTER(POINTER(c_NSResRecord))),
    ]
    
class NSRResRecord(object):
    type = 0
    nclass = 0
    ttl = 0
    domain_name = ""
    res = None
    
class NSRARecord(object):
    address = 0
    
class NSRNSRecord(object):
    domain_name = ""
    
class NSRCNAMERecord(object):
    domain_name = ""
    
class NSRMXRecord(object):
    preference = ""
    domain_name = ""
    
class NSRResult(object):
    answers = []
    authoritys = []
    additionals = []
    
def make_result(record_point):
    index = 0
    ret = []
    
    while bool(record_point[index]):
        c_record = record_point[index][0]
        res_record = NSRResRecord()
        
        type = c_record.sType
        res_record.type = type
        res_record.nclass = c_record.sClass
        res_record.ttl = c_record.nTTL
        res_record.domain_name = c_record.szDomainName
        
        res_data = None
        
        if TYPE_A == type:
            res_data = NSRARecord
            c_res = cast(c_record.pResData, POINTER(c_NSRARecord))
            res_data.address = c_res[0].address
        elif TYPE_CNAME == type:
            res_data = NSRCNAMERecord
            c_res = cast(c_record.pResData, POINTER(c_NSRCNAMERecord))
            res_data.domain_name = c_res[0].szDomainName
        elif TYPE_MX == type:
            res_data = NSRMXRecord
            c_res = cast(c_record.pResData, POINTER(c_NSRMXRecord))
            res_data.preference = c_res[0].sPreference
            res_data.domain_name = c_res[0].szDomainName
        elif TYPE_NS == type:
            res_data = NSRNSRecord
            c_res = cast(c_record.pResData, POINTER(c_NSRNSRecord))
            res_data.domain_name = c_res[0].szDomainName
        
        res_record.res = res_data
        ret.append(res_record)
        index = index + 1
    
    return ret
    
def resolve(name, type, server, timeout):
    pResult = POINTER(c_NSRResult)()
    
    if clib.resolve(name, type, server, pointer(pResult), timeout) < 0:
        return None
    
    ret = NSRResult()
    ret.answers = make_result(pResult[0].pAnswers)
    ret.pAuthoritys = make_result(pResult[0].pAuthoritys)
    ret.pAdditionals = make_result(pResult[0].pAdditionals)
    
    clib.release(pResult)
    
    return ret
