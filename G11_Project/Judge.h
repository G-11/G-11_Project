#ifndef _JUDGE_H_
#define _JUDGE_H_

#include "main.h"

bool ColliLine2LineXZ(D3DXVECTOR3 pos0St,D3DXVECTOR3 pos0Ed,D3DXVECTOR3 pos1St,D3DXVECTOR3 pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect);
bool CubeHit(D3DXVECTOR3 aPos,D3DXVECTOR3 aSize,D3DXVECTOR3 bPos,D3DXVECTOR3 bSize);
bool RectHit(float aPosX,float aPosZ,float aSizeX,float aSizeZ,float bPosX,float bPosZ,float bSizeX,float bSizeZ);
bool RectHit(const D3DXVECTOR2 &aPos,const D3DXVECTOR2 &aSize,const D3DXVECTOR2 &bPos,const D3DXVECTOR2 &bSize);
bool CircleHit2D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br);
bool CircleHit3D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br);
bool SphereHit(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br);
bool CylinderHit(D3DXVECTOR3 aPos,float aR,float aH,D3DXVECTOR3 bPos,float bR,float bH);
bool CollisionLC(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr);
bool CollisionLC(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r);

#endif