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
#include "typedefs.h"
#include "engine.h"
#include "controls.h"
#include "key.h"
#include "resource.h"
#include "smkplay.h"
//#include "smack.h"
#include "sound.h"
//#include "svga.h"


CSMKPlayer::CSMKPlayer()
{
    f_0 = xdim;
    f_4 = ydim;
    f_c = 0;
}

CSMKPlayer::~CSMKPlayer()
{
    if (f_c)
    {
#if 0
        SVGASetup(f_0, f_4);
        SVGASetGraph();
#endif
    }
}

int CSMKPlayer::PlaySMK(const char *a1)
{
    return PlaySMKWithWAV(a1, -1);
}

int CSMKPlayer::PlaySMKWithWAV(const char *a1, int a2)
{
    return 1;
#if 0
    int v4;
    u32 i;
    Smack *smk;
    func_2906C();
    v4 = 0;

    smk = SmackOpen(a1, 0xfe100, -1);
    if (smk)
    {
        SVGADetect(1);
        if (SVGASetup(smk->Width, smk->Height))
        {
            u32 ox = (WidthToUse-smk->Width)>>1;
            u32 oy = (HeightToUse-smk->Height)>>1;
            SmackToScreen(smk, ox,oy, SVGABytesPS(), SVGAWinTbl(), SVGASetBank());
            SVGASetGraph();
            f_c = TRUE;
            if (a2 != -1)
                sndStartWavID(a2, 255, -1);
            for (i = 1; i <= smk->Frames; i++)
            {
                if (smk->NewPalette)
                    SVGASetPalette(smk->Palette);
                SmackDoFrame(smk);
                if (i != smk->Frames)
                    SmackNextFrame(smk);
                do
                {
                } while (SmackWait(smk));
                if (keyGet() == bsc_Esc)
                {
                    sndKillAllSounds();
                    break;
                }
            }
        }
        else
            v4 = 2;
        SmackClose(smk);
    }
    else
        v4 = 1;
    sndKillAllSounds();
    func_2906C();
    return v4;
#endif
}

int CSMKPlayer::PlaySMKWithWAV(const char *a1, const char *a2)
{
    return 1;
#if 0
    int v4;
    u32 i;
    Smack *smk;
    func_2906C();
    v4 = 0;

    smk = SmackOpen(a1, 0xfe100, -1);
    if (smk)
    {
        SVGADetect(1);
        if (SVGASetup(smk->Width, smk->Height))
        {
            u32 ox = (WidthToUse-smk->Width)>>1;
            u32 oy = (HeightToUse-smk->Height)>>1;
            SmackToScreen(smk, ox,oy, SVGABytesPS(), SVGAWinTbl(), SVGASetBank());
            SVGASetGraph();
            f_c = TRUE;
            if (a2)
                sndStartWavDisk(a2, 255, -1);
            for (i = 1; i <= smk->Frames; i++)
            {
                if (smk->NewPalette)
                    SVGASetPalette(smk->Palette);
                SmackDoFrame(smk);
                if (i != smk->Frames)
                    SmackNextFrame(smk);
                do
                {
                } while (SmackWait(smk));
                if (keyGet() == bsc_Esc)
                {
                    sndKillAllSounds();
                    break;
                }
            }
        }
        else
            v4 = 2;
        SmackClose(smk);
    }
    else
        v4 = 1;
    sndKillAllSounds();
    func_2906C();
    return v4;
#endif
}

#if 0
RCFUNC void PTR4* RADLINK radmalloc(u32 numbytes)
{
    byte *temp;
    byte va;
    if (numbytes == 0 || numbytes == -1)
        return 0;
    temp = (byte*)Resource::Alloc(numbytes + 16);
    if (!temp)
        return 0;

    va = 16 - ((int)temp & 15);
    temp += va;
    temp[-1] = va;

    return temp;
}

RCFUNC void RADLINK radfree(void PTR4* ptr)
{
    byte *temp = (byte*)ptr;
    int va;
    if (!ptr)
        return;
    va = temp[-1];
    temp -= va;
    Resource::Free(temp);
}
#endif
