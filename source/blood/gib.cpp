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
#include "typedefs.h"
#include "actor.h"
#include "engine.h"
#include "callback.h"
#include "config.h"
#include "db.h"
#include "debug4g.h"
#include "eventq.h"
#include "fx.h"
#include "gib.h"
#include "levels.h"
#include "sfx.h"
#include "trig.h"

struct GIBFX
{
    FX_ID at0;
    int at1;
    int chance;
    int at9;
    int atd;
    int at11;
};


struct GIBTHING
{
    int at0;
    int at4;
    int chance;
    int atc;
    int at10;
};

struct GIBLIST
{
    GIBFX *at0;
    int at4;
    GIBTHING *at8;
    int atc;
    int at10;
};

GIBFX gibFxGlassT[] = {
    { FX_18, 0, 65536, 3, 200, 400 },
    { FX_31, 0, 32768, 5, 200, 400 }
};

GIBFX gibFxGlassS[] = {
    { FX_18, 0, 65536, 8, 200, 400 }
};

GIBFX gibFxBurnShard[] = {
    { FX_16, 0, 65536, 12, 500, 1000 }
};

GIBFX gibFxWoodShard[] = {
    { FX_17, 0, 65536, 12, 500, 1000 }
};

GIBFX gibFxMetalShard[] = {
    { FX_30, 0, 65536, 12, 500, 1000 }
};

GIBFX gibFxFireSpark[] = {
    { FX_14, 0, 65536, 8, 500, 1000 }
};

GIBFX gibFxShockSpark[] = {
    { FX_15, 0, 65536, 8, 500, 1000 }
};

GIBFX gibFxBloodChunks[] = {
    { FX_13, 0, 65536, 8, 90, 600 }
};

GIBFX gibFxBubblesS[] = {
    { FX_25, 0, 65536, 8, 200, 400 }
};

GIBFX gibFxBubblesM[] = {
    { FX_24, 0, 65536, 8, 200, 400 }
};

GIBFX gibFxBubblesL[] = {
    { FX_23, 0, 65536, 8, 200, 400 }
};

GIBFX gibFxIcicles[] = {
    { FX_31, 0, 65536, 15, 200, 400 }
};

GIBFX gibFxGlassCombo1[] = {
    { FX_18, 0, 65536, 15, 200, 400 },
    { FX_31, 0, 65536, 10, 200, 400 }
};

GIBFX gibFxGlassCombo2[] = {
    { FX_18, 0, 65536, 5, 200, 400 },
    { FX_20, 0, 53248, 5, 200, 400 },
    { FX_21, 0, 53248, 5, 200, 400 },
    { FX_19, 0, 53248, 5, 200, 400 },
    { FX_22, 0, 53248, 5, 200, 400 }
};

GIBFX gibFxWoodCombo[] = {
    { FX_16, 0, 65536, 8, 500, 1000 },
    { FX_17, 0, 65536, 8, 500, 1000 },
    { FX_14, 0, 65536, 8, 500, 1000 }
};

GIBFX gibFxMedicCombo[] = {
    { FX_18, 0, 32768, 7, 200, 400 },
    { FX_30, 0, 65536, 7, 500, 1000 },
    { FX_13, 0, 65536, 10, 90, 600 },
    { FX_14, 0, 32768, 7, 500, 1000 }
};

GIBFX gibFxFlareSpark[] = {
    { FX_28, 0, 32768, 15, 128, -128 }
};

GIBFX gibFxBloodBits[] = {
    { FX_13, 0, 45056, 8, 90, 600 }
};

GIBFX gibFxRockShards[] = {
    { FX_46, 0, 65536, 10, 300, 800 },
    { FX_31, 0, 32768, 10, 200, 1000 }
};

GIBFX gibFxPaperCombo1[] = {
    { FX_47, 0, 65536, 12, 300, 600 },
    { FX_14, 0, 65536, 8, 500, 1000 }
};

GIBFX gibFxPlantCombo1[] = {
    { FX_44, 0, 45056, 8, 400, 800 },
    { FX_45, 0, 45056, 8, 300, 800 },
    { FX_14, 0, 45056, 6, 500, 1000 }
};

GIBFX gibFx13BBA8[] = {
    { FX_49, 0, 65536, 4, 80, 300 }
};

GIBFX gibFx13BBC0[] = {
    { FX_50, 0, 65536, 4, 80, 0 }
};

GIBFX gibFx13BBD8[] = {
    { FX_50, 0, 65536, 20, 800, -40 },
    { FX_15, 0, 65536, 15, 400, 10 }
};

GIBFX gibFx13BC04[] = {
    { FX_32, 0, 65536, 8, 100, 0 }
};

GIBFX gibFx13BC1C[] = {
    { FX_56, 0, 65536, 8, 100, 0 }
};

GIBTHING gibHuman[] = {
    { 425, 1454, 917504, 300, 900 },
    { 425, 1454, 917504, 300, 900 },
    { 425, 1267, 917504, 300, 900 },
    { 425, 1267, 917504, 300, 900 },
    { 425, 1268, 917504, 300, 900 },
    { 425, 1269, 917504, 300, 900 },
    { 425, 1456, 917504, 300, 900 }
};

GIBTHING gibMime[] = {
    { 425, 2405, 917504, 300, 900 },
    { 425, 2405, 917504, 300, 900 },
    { 425, 2404, 917504, 300, 900 },
    { 425, 1268, 32768, 300, 900 },
    { 425, 1269, 32768, 300, 900 },
    { 425, 1456, 32768, 300, 900 },
};

GIBTHING gibHound[] = {
    { 425, 1326, 917504, 300, 900 },
    { 425, 1268, 32768, 300, 900 },
    { 425, 1269, 32768, 300, 900 },
    { 425, 1456, 32768, 300, 900 }
};

GIBTHING gibFleshGargoyle[] = {
    { 425, 1369, 917504, 300, 900 },
    { 425, 1361, 917504, 300, 900 },
    { 425, 1268, 32768, 300, 900 },
    { 425, 1269, 32768, 300, 900 },
    { 425, 1456, 32768, 300, 900 }
};

GIBTHING gibAxeZombieHead[] = {
    { 427, 3405, 917504, 0, 0 }
};

GIBLIST gibList[] = {
    { gibFxGlassT, 2, NULL, 0, 300 },
    { gibFxGlassS, 1, NULL, 0, 300 },
    { gibFxBurnShard, 1, NULL, 0, 0 },
    { gibFxWoodShard, 1, NULL, 0, 0 },
    { gibFxMetalShard, 1, NULL, 0, 0 },
    { gibFxFireSpark, 1, NULL, 0, 0 },
    { gibFxShockSpark, 1, NULL, 0, 0 },
    { gibFxBloodChunks, 1, NULL, 0, 0 },
    { gibFxBubblesS, 1, NULL, 0, 0 },
    { gibFxBubblesM, 1, NULL, 0, 0 },
    { gibFxBubblesL, 1, NULL, 0, 0 },
    { gibFxIcicles, 1, NULL, 0, 0 },
    { gibFxGlassCombo1, 2, NULL, 0, 300 },
    { gibFxGlassCombo2, 5, NULL, 0, 300 },
    { gibFxWoodCombo, 3, NULL, 0, 0 },
    { NULL, 0, gibHuman, 7, 0 },
    { gibFxMedicCombo, 4, NULL, 0, 0 },
    { gibFxFlareSpark, 1, NULL, 0, 0 },
    { gibFxBloodBits, 1, NULL, 0, 0 },
    { gibFxRockShards, 2, NULL, 0, 0 },
    { gibFxPaperCombo1, 2, NULL, 0, 0 },
    { gibFxPlantCombo1, 3, NULL, 0, 0 },
    { gibFx13BBA8, 1, NULL, 0, 0 },
    { gibFx13BBC0, 1, NULL, 0, 0 },
    { gibFx13BBD8, 2, NULL, 0, 0 },
    { gibFx13BC04, 1, NULL, 0, 0 },
    { gibFx13BC1C, 1, NULL, 0, 0 },
    { NULL, 0, gibAxeZombieHead, 1, 0 },
    { NULL, 0, gibMime, 6, 0 },
    { NULL, 0, gibHound, 4, 0 },
    { NULL, 0, gibFleshGargoyle, 5, 0 },
};

void gibCalcWallArea(int a1, int &a2, int &a3, int &a4, int &a5, int &a6, int &a7, int &a8)
{
    WALL *pWall = &wall[a1];
    a2 = (pWall->x+wall[pWall->point2].x)>>1;
    a3 = (pWall->y+wall[pWall->point2].y)>>1;
    int ceilZ, floorZ;
    int ceilZ2, floorZ2;
    short nSector = sectorofwall((short)a1);
    getzsofslope(nSector, a2, a3, &ceilZ, &floorZ);
    getzsofslope(pWall->nextsector, a2, a3, &ceilZ2, &floorZ2);
    ceilZ = ClipLow(ceilZ, ceilZ2);
    floorZ = ClipHigh(floorZ, floorZ2);
    a7 = floorZ-ceilZ;
    a5 = wall[pWall->point2].x-pWall->x;
    a6 = wall[pWall->point2].y-pWall->y;
    a8 = approxDist(a5>>4, a6>>4)*(a7>>8);
    a4 = (ceilZ+floorZ)>>1;
}

int ChanceToCount(int a1, int a2)
{
    int vb = a2;
    if (a1 < 0x10000)
    {
        for (int i = 0; i < a2; i++)
            if (!Chance(a1))
                vb--;
    }
    return vb;
}

void GibFX(SPRITE *pSprite, GIBFX *pGFX, CGibPosition *pPos, CGibVelocity *pVel)
{
    int nSector = pSprite->sectnum;
    if (gbAdultContent && gGameOptions.nGameType == GAMETYPE_0)
    {
        switch (pGFX->at0)
        {
            case FX_13:
                return;
        }
    }
    CGibPosition gPos(pSprite->x, pSprite->y, pSprite->z);
    if (pPos)
        gPos = *pPos;
    int ceilZ, floorZ;
    getzsofslope(nSector, gPos.x, gPos.y, &ceilZ, &floorZ);
    int nCount = ChanceToCount(pGFX->chance, pGFX->at9);
    int dz2 = gPos.z-ceilZ;
    int dz1 = floorZ-gPos.z;
    int top, bottom;
    GetSpriteExtents(pSprite, &top, &bottom);
    for (int i = 0; i < nCount; i++)
    {
        if (pPos == NULL)
        {
            switch (pSprite->cstat & kSpriteMask)
            {
                case 0:
                {
                    int nAngle = Random(2048);
                    gPos.x = pSprite->x+mulscale30(pSprite->clipdist<<2, Cos(nAngle));
                    gPos.y = pSprite->y+mulscale30(pSprite->clipdist<<2, Sin(nAngle));
                    gPos.z = bottom-Random(bottom-top);
                    break;
                }
            }
        }
        SPRITE *pFX = gFX.fxSpawn(pGFX->at0, nSector, gPos.x, gPos.y, gPos.z);
        if (pFX)
        {
            if (pGFX->at1 < 0)
                pFX->pal = pSprite->pal;
            if (pVel == NULL)
            {
                xvel[pFX->index] = Random2((pGFX->atd<<18)/120);
                yvel[pFX->index] = Random2((pGFX->atd<<18)/120);
                switch(pSprite->cstat&kSpriteMask)
                {
                case 16:
                    zvel[pFX->index] = Random2((pGFX->at11<<18)/120);
                    break;
                default:
                    if (dz2 < dz1 && dz2 < 0x4000)
                    {
                        zvel[pFX->index] = 0;
                    }
                    else if (dz1 < dz2  && dz1 < 0x4000)
                    {
                        zvel[pFX->index] = -Random((klabs(pGFX->at11)<<18)/120);
                    }
                    else
                    {
                        if ((pGFX->at11<<18)/120 < 0)
                            zvel[pFX->index] = -Random((klabs(pGFX->at11)<<18)/120);
                        else
                            zvel[pFX->index] = Random2((pGFX->at11<<18)/120);
                    }
                    break;
                }
            }
            else
            {
                xvel[pFX->index] = pVel->vx+Random2(pGFX->atd);
                yvel[pFX->index] = pVel->vy+Random2(pGFX->atd);
                zvel[pFX->index] = pVel->vz-Random(pGFX->at11);
            }
        }
    }
}

void GibThing(SPRITE *pSprite, GIBTHING *pGThing, CGibPosition *pPos, CGibVelocity *pVel)
{
    if (gbAdultContent && gGameOptions.nGameType <= GAMETYPE_0)
        switch (pGThing->at0)
        {
        case 425:
        case 427:
            return;
        }

    if (pGThing->chance == 65536 || Chance(pGThing->chance))
    {
        SPRITE *pGib = NULL;
        int nSector = pSprite->sectnum;
        int top, bottom;
        GetSpriteExtents(pSprite, &top, &bottom);
        int x, y, z;
        if (pPos == NULL)
        {
            int nAngle = Random(2048);
            x = pSprite->x+mulscale30(pSprite->clipdist<<2, Cos(nAngle));
            y = pSprite->y+mulscale30(pSprite->clipdist<<2, Sin(nAngle));
            z = bottom-Random(bottom-top);
        }
        else
        {
            x = pPos->x;
            y = pPos->y;
            z = pPos->z;
        }
        int ceilZ, floorZ;
        getzsofslope(nSector, x, y, &ceilZ, &floorZ);
        int dz2 = z-ceilZ;
        int dz1 = floorZ-z;
        pGib = actSpawnThing(nSector, x, y, z, pGThing->at0);
        dassert(pGib != NULL, 452);
        if (pGThing->at4 > -1)
            pGib->picnum = pGThing->at4;
        if (pVel == NULL)
        {
            xvel[pGib->index] = Random2((pGThing->atc<<18)/120);
            yvel[pGib->index] = Random2((pGThing->atc<<18)/120);
            switch (pSprite->cstat&kSpriteMask)
            {
            case 16:
                zvel[pGib->index] = Random2((pGThing->at10<<18)/120);
                break;
            default:
                if (dz2 < dz1 && dz2 < 0x4000)
                {
                    zvel[pGib->index] = 0;
                }
                else if (dz1 < dz2 && dz1 < 0x4000)
                {
                    zvel[pGib->index] = -Random((pGThing->at10<<18)/120);
                }
                else
                {
                    zvel[pGib->index] = Random2((pGThing->at10<<18)/120);
                }
                break;
            }
        }
        else
        {
            xvel[pGib->index] = pVel->vx+Random2(pGThing->atc);
            yvel[pGib->index] = pVel->vy+Random2(pGThing->atc);
            zvel[pGib->index] = pVel->vz-Random(pGThing->at10);
        }
    }
}

void GibSprite(SPRITE *pSprite, GIBTYPE nGibType, CGibPosition *pPos, CGibVelocity *pVel)
{
    dassert(pSprite != NULL, 491);
    dassert(nGibType >= 0 && nGibType < kGibMax, 492);
    if (pSprite->sectnum < 0 || pSprite->sectnum >= numsectors)
        return;
    GIBLIST *pGib = &gibList[nGibType];
    for (int i = 0; i < pGib->at4; i++)
    {
        GIBFX *pGibFX = &pGib->at0[i];
        dassert(pGibFX->chance > 0, 507);
        GibFX(pSprite, pGibFX, pPos, pVel);
    }
    for (int i = 0; i < pGib->atc; i++)
    {
        GIBTHING *pGibThing = &pGib->at8[i];
        dassert(pGibThing->chance > 0, 516);
        GibThing(pSprite, pGibThing, pPos, pVel);
    }
}

void GibFX(int nWall, GIBFX * pGFX, int a3, int a4, int a5, int a6, CGibVelocity * pVel)
{
    dassert(nWall >= 0 && nWall < numwalls, 525);
    WALL *pWall = &wall[nWall];
    int nSector = sectorofwall(nWall);
    int nCount = ChanceToCount(pGFX->chance, pGFX->at9);
    for (int i = 0; i < nCount; i++)
    {
        int r1 = Random(a6);
        int r2 = Random(a5);
        SPRITE *pGib = gFX.fxSpawn(pGFX->at0, nSector, pWall->x+Random(a4), pWall->y+r2, a3+r1, 0);
        if (pGib)
        {
            if (pGFX->at1 < 0)
                pGib->pal = pWall->pal;
            if (pVel == NULL)
            {
                xvel[pGib->index] = Random2((pGFX->atd<<18)/120);
                yvel[pGib->index] = Random2((pGFX->atd<<18)/120);
                zvel[pGib->index] = -Random((pGFX->at11<<18)/120);
            }
            else
            {
                xvel[pGib->index] = Random2((pVel->vx<<18)/120);
                yvel[pGib->index] = Random2((pVel->vy<<18)/120);
                zvel[pGib->index] = -Random((pVel->vz<<18)/120);
            }
        }
    }
}

void GibWall(int nWall, GIBTYPE nGibType, CGibVelocity *pVel)
{
    dassert(nWall >= 0 && nWall < numwalls, 563);
    dassert(nGibType >= 0 && nGibType < kGibMax, 564);
    WALL *pWall = &wall[nWall];
    int cx = (pWall->x+wall[pWall->point2].x)>>1;
    int cy = (pWall->y+wall[pWall->point2].y)>>1;
    int ceilZ, floorZ;
    int ceilZ2, floorZ2;
    int nSector = sectorofwall(nWall);
    getzsofslope(nSector, cx, cy, &ceilZ, &floorZ);
    getzsofslope(pWall->nextsector, cx, cy, &ceilZ2, &floorZ2);
    ceilZ = ClipLow(ceilZ, ceilZ2);
    floorZ = ClipHigh(floorZ, floorZ2);
    int cz = (ceilZ+floorZ)>>1;
    int wz = floorZ-ceilZ;
    int wx = wall[pWall->point2].x-pWall->x;
    int wy = wall[pWall->point2].y-pWall->y;
    GIBLIST *pGib = &gibList[nGibType];
    sfxPlay3DSound(cx, cy, cz, pGib->at10, nSector);
    for (int i = 0; i < pGib->at4; i++)
    {
        GIBFX *pGibFX = &pGib->at0[i];
        dassert(pGibFX->chance > 0, 597);
        GibFX(nWall, pGibFX, ceilZ, wx, wy, wz, pVel);
    }
}