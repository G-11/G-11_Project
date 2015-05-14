#include "Judge.h"
//=============================================================================
//引数:
//	pos0St			片方の線分の始点
//	pos0Ed			片方の線分の終点
//	pos1St			もう片方の線分の始点
//	pos1Ed			もう片方の線分の終点
//	OutPosCross		交点の座標
//	OutRate			始点から交点までの長さの線分上の比率
//	OutVecReflect	反射ベクトル
//=============================================================================
bool ColliLine2LineXZ(D3DXVECTOR3 pos0St,D3DXVECTOR3 pos0Ed,D3DXVECTOR3 pos1St,D3DXVECTOR3 pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	vec0 = pos0Ed-pos0St;
	vec1 = pos1Ed-pos1St;
	vec2 = pos0St-pos1St;

	//vec0-vec1の線分の外積(vec0がvec1より右側なら負)
	float CrossProduct01 = vec0.z*vec1.x-vec0.x*vec1.z;

	if(CrossProduct01==0.0f)
	{//平行なら終了
		return false;
	}

	//vec1-vec2の線分の外積(vec1がvec2より右側なら負)
	float CrossProduct12 = vec1.z*vec2.x-vec1.x*vec2.z;
	float Rate12 = CrossProduct12/CrossProduct01;
	if(Rate12 >= 0.0f && Rate12 <= 1.0f)
	{
		//vec0-vec1の線分の外積に対するvec2-vec0の線分の外積の内分比を算出
		//(vec1の線分がvec0の右側の時、1.0より大きい)
		//(vec1の線分がvec0の左側の時、0.0より小さい)
		if(OutPosCross != nullptr)
		{//交差する線分の交点
			*OutPosCross = pos0St+vec0*Rate12;
			//OutPosCross = pos1Sr+vec1*Rate02
		}
		if(OutRate != nullptr)
		{//線分上の交点までの内分比
			*OutRate =Rate12;
		}
		if(OutVecReflect != nullptr)
		{
			D3DXVECTOR3 vecMove,vecLine;
			vecMove = pos1Ed-pos1St;
			vecLine = pos0Ed-pos0St;
			
			//交差する線分同士の内積を算出
			float DotProduct = vecLine.x*vecMove.x+vecLine.z*vecMove.z;

			//交差する線分のなす角
			float ValueMove = sqrt(vecMove.x*vecMove.x+vecMove.z*vecMove.z);
			float ValueLine = sqrt(vecLine.x*vecLine.x+vecLine.z*vecLine.z);

			float Angle = -acosf(DotProduct/(ValueMove*ValueLine));
			//反射ベクトル
			D3DXVECTOR3 vecMoveNew = D3DXVECTOR3(cosf(Angle)*vecLine.x-sinf(Angle)*vecLine.z,0.0f,cosf(Angle)*vecLine.z+sinf(Angle)*vecLine.x);
			D3DXVec3Normalize(&vecMoveNew,&vecMoveNew);
			*OutVecReflect = vecMoveNew;
		}
		return true;
	}
	return false;
}

//===========================================================
//矩形
//===========================================================
bool RectHit(float aPosX,float aPosZ,float aSizeX,float aSizeZ,float bPosX,float bPosZ,float bSizeX,float bSizeZ)
{
	return ((aPosX+(aSizeX/2))>(bPosX-(bSizeX/2))&&(bPosX+(bSizeX/2))>(aPosX-(aSizeX/2))&&(aPosZ+(aSizeZ/2))>(bPosZ-(bSizeZ/2))&&(bPosZ+(bSizeZ/2))>(aPosZ-(aSizeZ/2)));

}
bool RectHit(const D3DXVECTOR2 &aPos,const D3DXVECTOR2 &aSize,const D3DXVECTOR2 &bPos,const D3DXVECTOR2 &bSize)
{
	return ((aPos.x + (aSize.x / 2))>(bPos.x - (bSize.x / 2)) && (bPos.x + (bSize.x / 2))>(aPos.x - (aSize.x / 2)) && (aPos.y + (aSize.y / 2))>(bPos.y - (bSize.y / 2)) && (bPos.y + (bSize.y / 2))>(aPos.y - (aSize.y / 2)));
}
//===========================================================
//立方体
//===========================================================
bool CubeHit(D3DXVECTOR3 aPos,D3DXVECTOR3 aSize,D3DXVECTOR3 bPos,D3DXVECTOR3 bSize)
{
	return ((aPos.x+(aSize.x/2))>(bPos.x-(bSize.x/2))&&(bPos.x+(bSize.x/2))>(aPos.x-(aSize.x/2))&&
		(aPos.z+(aSize.z/2))>(bPos.z-(bSize.z/2))&&(bPos.z+(bSize.z/2))>(aPos.z-(aSize.z/2))&&
		(aPos.y+(aSize.y/2))>=bPos.y&&(bPos.y+(bSize.y/2))>=aPos.y);
}
//===========================================================
// 円状の当たり判定
//===========================================================
bool CircleHit2D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x - apos.x)*(bpos.x - apos.x) + (bpos.y - apos.y)*(bpos.y - apos.y))<(ar*ar) + (br*br);

}

bool CircleHit3D(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x-apos.x)*(bpos.x-apos.x)+(bpos.z-apos.z)*(bpos.z-apos.z))<(ar*ar)+(br*br);

}
//===========================================================
// 円状の当たり判定
//===========================================================
bool SphereHit(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	return ((bpos.x - apos.x)*(bpos.x - apos.x) + (bpos.y - apos.y)*(bpos.y - apos.y)+(bpos.z - apos.z)*(bpos.z - apos.z))<(ar*ar) + (br*br);

}
//===========================================================
//円柱
//===========================================================
bool CylinderHit(D3DXVECTOR3 aPos,float aR,float aH,D3DXVECTOR3 bPos,float bR,float bH)
{
	return ((bPos.x-aPos.x)*(bPos.x-aPos.x)+(bPos.z-aPos.z)*(bPos.z-aPos.z))<(aR*aR)+(bR*bR)&&
		(aPos.y<=bPos.y+bH && aPos.y+aH>=bPos.y);
}

//円と線分のあたり判定
bool CollisionLC(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr)
{
	if ((line_size_x*(cx - line_x) + line_size_y*(cy - line_y)) <= 0)
	{
		// 始点を通る､線分に垂直な線を置いたとき､円の中心が線分の範囲外にあったとき
		// ｢線分の始点から円の中心までの距離の２乗｣と｢円の半径の２乗｣との比較
		return (cr*cr >= (cx - line_x)*(cx - line_x) + (cy - line_y)*(cy - line_y));
	}
	else if (((-line_size_x)*(cx - (line_x + line_size_x)) + (-line_size_y)*(cy - (line_y + line_size_y))) <= 0)
	{
		// 終点を通る､線分に垂直な線を置いたとき､円の中心が線分の範囲外にあったとき
		// ｢線分の終点から円の中心までの距離の２乗｣と｢円の半径の２乗｣との比較
		return (cr*cr >= (cx - (line_x + line_size_x))*(cx - (line_x + line_size_x)) + (cy - (line_y + line_size_y))*(cy - (line_y + line_size_y)));
	}
	else
	{
		// 線分の始点終点に垂線を引っ張ったとき､円の中心がその範囲内にあったとき

		float e = sqrt(line_size_x*line_size_x + line_size_y*line_size_y);	// これでx,y成分を割れば単ベクトルになる
		float c2 = (cx - line_x)*(cx - line_x) + (cy - line_y)*(cy - line_y);
		float b = (cx - line_x)*(line_size_x / e) + (cy - line_y)*(line_size_y / e);	// 内積で算出した､隣辺の長さ
		return (cr*cr >= c2 - b*b);
	}
}
//===========================================================
//円と厚みのある線の当たり判定
//引数:
//	D3DXVECTOR3 線の始点
//	D3DXVECTOR3 線の終点
//	float		線の太さ
//	float		線の傾き
//	D3DXVECTOR3 円の中心位置
//	float		円の半径
//===========================================================
bool CollisionLC(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r)
{
	D3DXVECTOR3 pos = line_head;
	D3DXVECTOR3 dis = line_head - line_tail;
	float length = t_sqrtF((dis.x*dis.x) + (dis.y*dis.y));
	float c2l = ((line_head.x - obj_pos.x)*(line_head.x - obj_pos.x)) + ((line_head.y - obj_pos.y)*(line_head.y - obj_pos.y));

	if (length*length > c2l)
	{
		D3DXVECTOR3 move;
		move.x = -sinf(angle)*width;
		move.y = -cosf(angle)*width;
		move.z = t_sqrtF((move.x*move.x) + (move.y*move.y));

		float len = 0;
		while (len < length)
		{
			if (CircleHit2D(pos,width / 2.0f,obj_pos,r))
			{
				return true;
			}

			pos.x += move.x;
			pos.y += move.y;
			len += move.z;
			if (len > length)
			{
				pos = line_tail;
			}
		}
	}
	return false;


}