__header = '''/**
 * AS - the open source Automotive Software on https://github.com/parai
 *
 * Copyright (C) 2015  AS <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
'''
import sys,os
from .GCF import *

__all__ = ['GenNvM']


__dir = '.'

def __GetBlockSize(block):
    Size = 0
    for data in GLGet(block,'DataList'):
        if(GAGet(data,'IsArray')=='True'):
            if(GAGet(data,'Type')=='uint32' or GAGet(data,'Type')=='sint32'):
                Size += 4*Integer(GAGet(data,'ArraySize'))
            elif(GAGet(data,'Type')=='uint16' or GAGet(data,'Type')=='uint16'):
                Size += 2*Integer(GAGet(data,'ArraySize'))
            elif(GAGet(data,'Type')=='uint8' or GAGet(data,'Type')=='uint8'):
                Size += 1*Integer(GAGet(data,'ArraySize'))
            else:
                assert(0)
        else:
            if(GAGet(data,'Type')=='uint32' or GAGet(data,'Type')=='sint32'):
                Size += 4
            elif(GAGet(data,'Type')=='uint16' or GAGet(data,'Type')=='uint16'):
                Size += 2
            elif(GAGet(data,'Type')=='uint8' or GAGet(data,'Type')=='uint8'):
                Size += 1
            else:
                assert(0)
    return int((Size+3)/4)*4
    
def GenNvM(root,dir):
    global __dir
    GLInit(root)
    __dir = '%s'%(dir)
    if(len(GLGet('BlockList')) == 0):return
    GenH()
    GenC()
    print('    >>> Gen NvM DONE <<<')

def GenH():
    global __dir
    fp = open('%s/NvM_Cfg.h'%(__dir),'w')
    fp.write(GHeader('NvM')) 
    General=GLGet('General')
    BlockList = GLGet('BlockList')
    fp.write("""
#ifndef NVM_CFG_H_
#define NVM_CFG_H_

#include "NvM_Types.h"
#include "NvM_ConfigTypes.h"

#define NVM_DEV_ERROR_DETECT            STD_%s
#define NVM_VERSION_INFO_API            STD_%s
#define NVM_SET_RAM_BLOCK_STATUS_API    STD_%s

#define NVM_API_CONFIG_CLASS            NVM_API_CONFIG_%s     // Class 1-3
#define NVM_COMPILED_CONFIG_ID          0                          // 0..65535
#define NVM_CRC_NUM_OF_BYTES            0                          // 1..65535
#define NVM_DATASET_SELECTION_BITS      0                          // 0..8
#define NVM_DRV_MODE_SWITCH             STD_OFF                    // OFF = SLOW, ON = FAST
#define NVM_DYNAMIC_CONFIGURATION       STD_OFF                    // OFF..ON
#define NVM_JOB_PRIORIZATION            STD_OFF                    // OFF..ON
#define NVM_MAX_NUMBER_OF_WRITE_RETRIES 2                          // 0..7
#define NVM_POLLING_MODE                STD_%s                    // OFF..ON
#define NVM_SIZE_IMMEDIATE_JOB_QUEUE    8                          // 1..255
#define NVM_SIZE_STANDARD_JOB_QUEUE     8                          // 1..255\n\n"""%(
            GAGet(General,'DevelopmentErrorDetection'),
            GAGet(General,'VersionInfoApi'),
            GAGet(General,'RamBlockStatusApi'),
            GAGet(General,'ConfigClass'),
            GAGet(General,'PollingMode')))
    max_block_size = 0
    max_block_size_fee = 0
    max_block_size_ea  = 0
    for block in BlockList:
        block_size = __GetBlockSize(block)
        if(block_size>max_block_size):
            max_block_size = block_size 
            
        NvramDeviceId = GAGet(block,'NvramDeviceId')
        if(GAGet(block,'BlockUseCrc')=='True'):
            if(GAGet(block,'BlockCRCType')=='Crc16'):
                crc_len = 2
            else:
                crc_len = 4
        else:
            crc_len = 0
    
        # for python 3, / will change type to float
        block_size = int((block_size+crc_len+3)/4)*4
        if('Fee' == NvramDeviceId):
            BlockNumRef = GAGet(block,'BlockNumRef0')
            if(block_size>max_block_size_fee):
                max_block_size_fee = block_size 
            fp.write('#define NVM_FEE_BLOCK_SIZE_%-16s %s\n'%(BlockNumRef,block_size))
        else:
            BlockNumRef = GAGet(block,'BlockNumRef1') 
            if(block_size>max_block_size_ea):
                max_block_size_ea = block_size 
            fp.write('#define NVM_EA_BLOCK_SIZE_%-16s  %s\n'%(BlockNumRef,block_size))
            
    fp.write('#define NVM_FEE_MAX_BLOCK_LENGTH %s\n'%(max_block_size_fee))
    fp.write('#define NVM_EA_MAX_BLOCK_LENGTH  %s\n'%(max_block_size_ea))
                
    fp.write('#define NVM_MAX_BLOCK_LENGTH    %s\n\n'%(max_block_size))
    fp.write('#define NVM_NUM_OF_NVRAM_BLOCKS %s\n\n'%(len(BlockList)))
    #Zero Id reserved by NvM
    Id = 1
    for block in BlockList:
        fp.write('#define NVM_BLOCK_ID_%s %s\n'%(GAGet(block,'Name'),Id))
        Id += 1      
    #for each block, generate a readable memory map type
    for block in BlockList:
        cstr = '\ntypedef struct{\n'
        for data in GLGet(block,'DataList'):
            if(GAGet(data,'IsArray')=='False'):
                cstr += '\t%s _%s;\n'%(GAGet(data,'Type'),GAGet(data,'Name'))
            else:
                cstr += '\t%s _%s[%s];\n'%(GAGet(data,'Type'),GAGet(data,'Name'),GAGet(data,'ArraySize'))
        cstr += '}NvM_Block_%s_DataGroupType;\n\n'%(GAGet(block,'Name'))
        fp.write(cstr)       
    for block in GLGet('FeeBlockList'):
        cstr = '\nextern NvM_Block_%s_DataGroupType NvM_Block%s_DataGroup_RAM;\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        cstr+= 'extern const NvM_Block_%s_DataGroupType NvM_Block%s_DataGroup_ROM;\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        fp.write(cstr)      
    fp.write("""
#define Rte_NvMReadBuffer(GroupName)    ((uint8*)&NvM_Block_##GroupName##_DataGroup_RAM)    
#define Rte_NvMRead(GroupName,DataName) (NvM_Block_##GroupName##_DataGroup_RAM._##DataName)
#define Rte_NvMReadArrayBuffer(GroupName,DataName) ((uint8*)NvM_Block_##GroupName##_DataGroup_RAM._##DataName)
#define Rte_NvMReadArray(GroupName,DataName,Index) (NvM_Block_##GroupName##_DataGroup_RAM._##DataName[Index])

#define Rte_NvMReadBufferConst(GroupName)    ((uint8*)&NvM_Block_##GroupName##_DataGroup_ROM) 
#define Rte_NvMReadConst(GroupName,DataName) (NvM_Block_##GroupName##_DataGroup_ROM._##DataName)
#define Rte_NvMReadArrayBufferConst(GroupName,DataName) ((uint8*)NvM_Block_##GroupName##_DataGroup_ROM._##DataName)
#define Rte_NvMReadArrayConst(GroupName,DataName,Index) (NvM_Block_##GroupName##_DataGroup_ROM._##DataName[Index])

#define Rte_NvMWrite(GroupName,DataName,Value) (NvM_Block_##GroupName##_DataGroup_RAM._##DataName = Value)
#define Rte_NvMWriteArray(GroupName,DataName,Index,Value) (NvM_Block_##GroupName##_DataGroup_RAM._##DataName[Index] = Value)

#define Rte_NvmWriteBlock(GroupName) NvM_WriteBlock(NVM_BLOCK_ID_##GroupName,(uint8*)&NvM_Block_##GroupName##_DataGroup_RAM)
#define Rte_NvmReadBlock(GroupName)  NvM_ReadBlock(NVM_BLOCK_ID_##GroupName,(uint8*)&NvM_Block_##GroupName##_DataGroup_RAM)      \n\n""")
    
    for block in BlockList:
        cstr = '\nextern NvM_Block_%s_DataGroupType NvM_Block_%s_DataGroup_RAM;\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        cstr+= 'extern const NvM_Block_%s_DataGroupType NvM_Block_%s_DataGroup_ROM;\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        fp.write(cstr)
        
    fp.write('\n\n#endif /*NVM_CFG_H_*/\n\n')
    fp.close()
    
def GenC():
    global __dir
    fp = open('%s/NvM_Cfg.c'%(__dir),'w')
    fp.write('#ifdef USE_NVM\n')
    fp.write(GHeader('NvM')) 
    fp.write('''#include "NvM.h"
#ifdef USE_FEE
#include "Fee.h"
#endif
#ifdef USE_EA
#include "Ea.h"
#endif\n\n''') 
    BlockList = GLGet('BlockList')
    for block in BlockList:
        cstr = '\nNvM_Block_%s_DataGroupType NvM_Block_%s_DataGroup_RAM;\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        cstr+= 'const NvM_Block_%s_DataGroupType NvM_Block_%s_DataGroup_ROM={\n'%(GAGet(block,'Name'),GAGet(block,'Name'))
        for data in GLGet(block,'DataList'):
            if( GAGet(data,'IsArray')=='False' ):
                cstr += '\t._%s=%s,\n'%(GAGet(data,'Name'),GAGet(data,'DefaultValue'))
            else:
                cstr += '\t._%s=%s,\n'%(GAGet(data,'Name'),GAGet(data,'DefaultValue'))
        cstr += '};\n\n'
        fp.write(cstr)       
    cstr = 'const NvM_BlockDescriptorType BlockDescriptorList[] = {\n'
    for block in BlockList:
        NvramDeviceId = GAGet(block,'NvramDeviceId')
        if('Fee' == NvramDeviceId):
            BlockNumRef = GAGet(block,'BlockNumRef0')
        else:
            BlockNumRef = GAGet(block,'BlockNumRef1')
        cstr += """
    {
        .BlockManagementType = NVM_BLOCK_%s,
        .SelectBlockForReadall = %s,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = %s,
        .BlockUseCrc  = %s,
        .BlockCRCType =NVM_%s,
        .RamBlockDataAddress = (uint8*)&NvM_Block_%s_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = %s_BLOCK_NUM_%s,
        .NvramDeviceId = %s_INDEX,
        .NvBlockBaseNumber = %s_BLOCK_NUM_%s,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_Block_%s_DataGroup_ROM,
    },\n"""%(
             GAGet(block,'BlockManagementType'),
             GAGet(block,'SelectBlockForReadall'),
             __GetBlockSize(block),
             GAGet(block,'BlockUseCrc').upper(),
             GAGet(block,'BlockCRCType').upper(),
             GAGet(block,'Name'),
             NvramDeviceId.upper(),BlockNumRef,
             NvramDeviceId.upper(),
             NvramDeviceId.upper(),BlockNumRef,
             GAGet(block,'Name'),
             )
    cstr += """};

const NvM_ConfigType NvM_Config = {
    .Common = {
        .MultiBlockCallback = NULL,
    },
    .BlockDescriptor = BlockDescriptorList,        
};\n\n"""
    fp.write(cstr)
    fp.write('#endif /* USE_NVM */\n')
    fp.close()
