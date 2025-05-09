/*
 * Copyright (C) 2018, 2022 nukeykt
 *
 * This file is part of Blood-RE.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "system.h"
#include "error.h"
#include "fire.h"
#include "globals.h"
#include "misc.h"
#include "resource.h"
#include "tile.h"

int gDamping = 6;
int fireSize = 128;

extern "C" byte CoolTable[1024];

byte CoolTable[1024];

extern "C" void CellularFrame(byte *pFrame, int sizeX, int sizeY);

byte FrameBuffer[17280];
byte SeedBuffer[16][128];
byte *gCLU;

void InitSeedBuffers(void)
{
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < fireSize; j += 2)
        {
            byte v = wrand();
            SeedBuffer[i][j] = v;
            SeedBuffer[i][j + 1] = v;
        }
}

void BuildCoolTable(void)
{
    for (int i = 0; i < 1024; i++)
        CoolTable[i] = ClipLow((i-gDamping) / 4, 0);
}

void DoFireFramePragma(byte *a1, byte *a2)
{
    byte* s = FrameBuffer;
    int i = fireSize;
    do
    {
        int j = fireSize;
        byte* p = a2;
        do
        {
            *p = a1[*s++];
            p += fireSize;
        } while (--j);
        a2++;
        
    } while (--i);
}

void DoFireFrame(void)
{
    int nRand = qrand()&15;
    for (int i = 0; i < 3; i++)
    {
        memcpy(&FrameBuffer[(i+132)*128], SeedBuffer[nRand], 128);
    }
    CellularFrame(FrameBuffer, 128, 132);
    byte *pData = tileLoadTile(2342);
    DoFireFramePragma(gCLU, pData);
}

void FireInit(void)
{
    memset(FrameBuffer, 0, sizeof(FrameBuffer));
    BuildCoolTable();
    InitSeedBuffers();
    DICTNODE *pNode = gSysRes.Lookup("RFIRE", "CLU");
    if (!pNode)
        ThrowError(141)("RFIRE.CLU not found");
    gCLU = (byte*)gSysRes.Lock(pNode);
    for (int i = 0; i < 100; i++)
        DoFireFrame();
}

void FireProcess(void)
{
    static int32_t lastUpdate;
    if (gGameClock < lastUpdate || lastUpdate + 2 < gGameClock)
    {
        DoFireFrame();
        lastUpdate = gGameClock;
    }
}
