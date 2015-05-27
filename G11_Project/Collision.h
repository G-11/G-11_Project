#ifndef _COLLISION_H_
#define _COLLISION_H_
#include "main.h"

class Collision
{
public:
	static bool Rect(float aPosX,float aPosY,float aSizeX,float aSizeY,float bPosX,float bPosY,float bSizeX,float bSizeY);
	static bool Rect(float aPosX,float aPosY,float aSize,float bPosX,float bPosY,float bSize);
	static bool Quad(const D3DXVECTOR3 pos1,const D3DXVECTOR3* quadPoint1,UINT num1,const D3DXVECTOR3 pos2,const D3DXVECTOR3* quadPoint2,UINT num2);
	static bool DotQuad(const D3DXVECTOR3& pos,const D3DXVECTOR3* quad,UINT num);
	static bool CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num);
	static bool CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num,const D3DXVECTOR3& speed,D3DXVECTOR3* reflectVec = nullptr);
	static bool Line(const D3DXVECTOR2& point1,const D3DXVECTOR2& vec1,const D3DXVECTOR2& point2,const D3DXVECTOR2& vec2,D3DXVECTOR3* reflectVec = nullptr);
	static bool Cube(const D3DXVECTOR3& aPos,const D3DXVECTOR3& aSize,const D3DXVECTOR3& bPos,const D3DXVECTOR3& bSize);
	static bool Circle(float ax,float ay,float ar,float bx,float by,float br);
	static bool Circle(const D3DXVECTOR3& aPos,float ar,const D3DXVECTOR3& bPos,float br,bool xz=false);
	static bool CircleDot(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3& point);
	static bool Sphere(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br);
	static bool Cylinder(const D3DXVECTOR3& aPos,float ar,float ah,const D3DXVECTOR3& bPos,float br,float bh);
	static bool LineCircle(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr);
	static bool LineCircle(const D3DXVECTOR3& line_start,const D3DXVECTOR3& line_end,const D3DXVECTOR3& circle,float radius);
	static bool LineCircle2(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r);
	static bool LineLineXZ(const D3DXVECTOR3& pos0St,const D3DXVECTOR3& pos0Ed,const D3DXVECTOR3& pos1St,const D3DXVECTOR3& pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect);

private:
	static D3DXVECTOR3 CreateReflectVec(const D3DXVECTOR3& point1Start,const D3DXVECTOR3& point1End,const D3DXVECTOR3& point2Start,const D3DXVECTOR3& point2End);
};

//=======================================================================================
//矩形の当たり判定
//=======================================================================================
inline bool Collision::Rect(float aPosX,float aPosY,float aSizeX,float aSizeY,float bPosX,float bPosY,float bSizeX,float bSizeY)
{
	return ((aPosX + (aSizeX / 2.0f)) > (bPosX - (bSizeX / 2.0f)) && (bPosX + (bSizeX / 2.0f)) > (aPosX - (bSizeX / 2.0f))
		&& (aPosY + (aSizeY / 2.0f)) > (bPosY - (bSizeY / 2.0f)) && (bPosY + (bSizeY / 2.0f)) > (aPosY - (bSizeY / 2.0f)));
}
//=======================================================================================
//正方形の当たり判定
//=======================================================================================
inline bool Collision::Rect(float aPosX,float aPosY,float aSize,float bPosX,float bPosY,float bSize)
{
	return ((aPosX + (aSize / 2.0f)) > (bPosX - (bSize / 2.0f)) && (bPosX + (bSize / 2.0f)) > (aPosX - (bSize / 2.0f))
		&& (aPosY + (aSize / 2.0f)) > (bPosY - (bSize / 2.0f)) && (bPosY + (bSize / 2.0f)) > (aPosY - (bSize / 2.0f)));
}
//=======================================================================================
//立方体の当たり判定
//=======================================================================================
inline bool Collision::Cube(const D3DXVECTOR3& aPos,const D3DXVECTOR3& aSize,const D3DXVECTOR3& bPos,const D3DXVECTOR3& bSize)
{
	return ((aPos.x + (aSize.x / 2))>(bPos.x - (bSize.x / 2)) && (bPos.x + (bSize.x / 2))>(aPos.x - (aSize.x / 2)) &&
		(aPos.z + (aSize.z / 2))>(bPos.z - (bSize.z / 2)) && (bPos.z + (bSize.z / 2))>(aPos.z - (aSize.z / 2)) &&
		(aPos.y + (aSize.y / 2)) >= bPos.y && (bPos.y + (bSize.y / 2)) >= aPos.y);
}
//=======================================================================================
//円の当たり判定
//=======================================================================================
inline bool Collision::Circle(float ax,float ay,float ar,float bx,float by,float br)
{
	float disx = (bx - ax) * (bx - ax);
	float disy = (by - ay) * (by - ay);
	float disr = (ar + br)*(ar + br);
	
	return ( (disx+disy) <= disr );
}
//=======================================================================================
//円の当たり判定
// xz:trueならXZ座標を使用、falseならXY座標を使用
//=======================================================================================
inline bool Collision::Circle(const D3DXVECTOR3& aPos,float ar,const D3DXVECTOR3& bPos,float br,bool xz)
{
	if (xz == true)
	{
		return Circle(aPos.x,aPos.z,ar,bPos.x,bPos.z,br);
	}
	else
	{
		return Circle(aPos.x,aPos.y,ar,bPos.x,bPos.y,br);
	}
}

inline bool Collision::CircleDot(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3& point)
{
	D3DXVECTOR2 vec = D3DXVECTOR2(circle.x,circle.y)-D3DXVECTOR2(point.x,point.y);
	float length = D3DXVec2LengthSq(&vec);

	return (length < radius*radius);
}
//=======================================================================================
//球の当たり判定
//=======================================================================================
inline bool Collision::Sphere(D3DXVECTOR3 apos,float ar,D3DXVECTOR3 bpos,float br)
{
	D3DXVECTOR3 dis;
	dis.x = (bpos.x - apos.x)*(bpos.x - apos.x);
	dis.y = (bpos.y - apos.y)*(bpos.y - apos.y);
	dis.z = (bpos.z - apos.z)*(bpos.z - apos.z);
	float disr = (br - ar)*(br - ar);

	return (dis.x+dis.y+dis.z)<=disr;
	
}
//=======================================================================================
//円柱の当たり判定
//=======================================================================================
inline bool Collision::Cylinder(const D3DXVECTOR3& aPos,float ar,float ah,const D3DXVECTOR3& bPos,float br,float bh)
{
	return ((bPos.x - aPos.x)*(bPos.x - aPos.x) + (bPos.z - aPos.z)*(bPos.z - aPos.z))<(ar * ar) + (br * br) &&
		(aPos.y <= bPos.y + bh && aPos.y + ah >= bPos.y);
}
//=======================================================================================
//線と円の当たり判定
//=======================================================================================
inline bool Collision::LineCircle(float line_x,float line_y,float line_size_x,float line_size_y,float cx,float cy,float cr)
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

inline bool Collision::LineCircle(const D3DXVECTOR3& line_start,const D3DXVECTOR3& line_end,const D3DXVECTOR3& circle,float radius)
{
	D3DXVECTOR2 start(line_start.x,line_start.y);
	D3DXVECTOR2 end(line_end.x,line_end.y);
	D3DXVECTOR2 cir(circle.x,circle.y);
	D3DXVECTOR2 length(0,0);
	
	D3DXVECTOR2 vec1 = end-start;
	D3DXVECTOR2 vec2 = cir-start;

	float vec1Length = D3DXVec2LengthSq(&vec1);
	float vec2Length = D3DXVec2LengthSq(&vec2);
	float dot = D3DXVec2Dot(&vec1,&vec2);

	if (dot / vec1Length < 0 || dot / vec1Length > 1){ return false; }

	float phd2 = (dot*dot)/vec1Length;

	if ((vec2Length - phd2) < radius*radius)
	{
		return true;
	}
	
	return false;
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
inline bool Collision::LineCircle2(const D3DXVECTOR3& line_head,const D3DXVECTOR3& line_tail,float width,float angle,const D3DXVECTOR3& obj_pos,float r)
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
			if (Circle(pos,width / 2.0f,obj_pos,r))
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
inline bool Collision::LineLineXZ(const D3DXVECTOR3& pos0St,const D3DXVECTOR3& pos0Ed,const D3DXVECTOR3& pos1St,const D3DXVECTOR3& pos1Ed,D3DXVECTOR3* OutPosCross,float* OutRate,D3DXVECTOR3* OutVecReflect)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	vec0 = pos0Ed - pos0St;
	vec1 = pos1Ed - pos1St;
	vec2 = pos0St - pos1St;

	//vec0-vec1の線分の外積(vec0がvec1より右側なら負)
	float CrossProduct01 = vec0.z*vec1.x - vec0.x*vec1.z;

	if (CrossProduct01 == 0.0f)
	{//平行なら終了
		return false;
	}

	//vec1-vec2の線分の外積(vec1がvec2より右側なら負)
	float CrossProduct12 = vec1.z*vec2.x - vec1.x*vec2.z;
	float Rate12 = CrossProduct12 / CrossProduct01;
	if (Rate12 >= 0.0f && Rate12 <= 1.0f)
	{
		//vec0-vec1の線分の外積に対するvec2-vec0の線分の外積の内分比を算出
		//(vec1の線分がvec0の右側の時、1.0より大きい)
		//(vec1の線分がvec0の左側の時、0.0より小さい)
		if (OutPosCross != nullptr)
		{//交差する線分の交点
			*OutPosCross = pos0St + vec0*Rate12;
			//OutPosCross = pos1Sr+vec1*Rate02
		}
		if (OutRate != nullptr)
		{//線分上の交点までの内分比
			*OutRate = Rate12;
		}
		if (OutVecReflect != nullptr)
		{
			D3DXVECTOR3 vecMove,vecLine;
			vecMove = pos1Ed - pos1St;
			vecLine = pos0Ed - pos0St;

			//交差する線分同士の内積を算出
			float DotProduct = vecLine.x*vecMove.x + vecLine.z*vecMove.z;

			//交差する線分のなす角
			float ValueMove = sqrt(vecMove.x*vecMove.x + vecMove.z*vecMove.z);
			float ValueLine = sqrt(vecLine.x*vecLine.x + vecLine.z*vecLine.z);

			float Angle = -acosf(DotProduct / (ValueMove*ValueLine));
			//反射ベクトル
			D3DXVECTOR3 vecMoveNew = D3DXVECTOR3(cosf(Angle)*vecLine.x - sinf(Angle)*vecLine.z,0.0f,cosf(Angle)*vecLine.z + sinf(Angle)*vecLine.x);
			D3DXVec3Normalize(&vecMoveNew,&vecMoveNew);
			*OutVecReflect = vecMoveNew;
		}
		return true;
	}
	return false;
}
//=============================================================================
// 回転する矩形同士の当たり判定
//引数：
// pos1 … 一つ目の矩形の中心座標
// quadPoint1 … 一つ目の矩形の頂点配列
// num1 … 一つ目の矩形の頂点数
// pos2 … 2つ目の矩形の中心座標
// quadPoint2 … 2つ目の矩形の頂点配列
// num2 … 2つ目の矩形の頂点数
//=============================================================================
inline bool Collision::Quad(const D3DXVECTOR3 pos1,const D3DXVECTOR3* quadPoint1,UINT num1,const D3DXVECTOR3 pos2,const D3DXVECTOR3* quadPoint2,UINT num2)
{
	//それぞれの矩形の変動しが交差していたらtrue
	for (UINT cnt = 0;cnt < num1;cnt++)
	{
		D3DXVECTOR2 point1(quadPoint1[cnt].x,quadPoint1[cnt].y);
		D3DXVECTOR2 vec1(quadPoint1[(cnt + 1) % num1].x - quadPoint1[cnt].x,quadPoint1[(cnt + 1) % num1].y - quadPoint1[cnt].y);
		for (UINT cnt2 = 0;cnt2 < num2;cnt2++)
		{
			D3DXVECTOR2 point2(quadPoint2[cnt2].x,quadPoint2[cnt2].y);
			D3DXVECTOR2 vec2(quadPoint2[(cnt2 + 1) % num2].x - quadPoint2[cnt2].x,quadPoint2[(cnt2 + 1) % num2].y - quadPoint2[cnt2].y);
			if (Line(point1,vec1,point2,vec2))
			{
				return true;
			}
		}
	}

	//片方の矩形の中心がもう片方の矩形に含まれているとtrue
	if (DotQuad(pos2,quadPoint1,num1))
	{
		return true;
	}
	if (DotQuad(pos1,quadPoint2,num2))
	{
		return true;
	}

	return false;
}

inline bool Collision::Line(const D3DXVECTOR2& point1,const D3DXVECTOR2& vec1,const D3DXVECTOR2& point2,const D3DXVECTOR2& vec2,D3DXVECTOR3* reflectVec)
{
	D3DXVECTOR2 vec = point2 - point1;
	float Cross_v1_v2 = D3DXVec2CCW(&vec1,&vec2);

	if (Cross_v1_v2 == 0){ return false; }

	float Cross_vec_v1 = D3DXVec2CCW(&vec,&vec1);
	float Cross_vec_v2 = D3DXVec2CCW(&vec,&vec2);

	float t1 = Cross_vec_v2 / Cross_v1_v2;
	float t2 = Cross_vec_v1 / Cross_v1_v2;

	float eps = 0.00001f;

	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) 
	{
		// 交差していない
		return false;
	}
	else
	{
		if (reflectVec != nullptr)
		{
			D3DXVECTOR3 point1Start(point1.x,point1.y,0);
			D3DXVECTOR3 point2Start(point2.x,point2.y,0);
			D3DXVECTOR3 point1End = point1Start + D3DXVECTOR3(vec1.x,vec1.y,0);
			D3DXVECTOR3 point2End = point2Start + D3DXVECTOR3(vec2.x,vec2.y,0);
			*reflectVec = CreateReflectVec(point1Start,point1End,point2Start,point2End);
		}
		return true;
	}
}

inline bool Collision::DotQuad(const D3DXVECTOR3& pos,const D3DXVECTOR3* quad,UINT num)
{
	D3DXVECTOR3 vecline;
	D3DXVECTOR3 vectarget;
	bool hit = true;
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		vecline = quad[(cnt + 1) % num] - quad[cnt];
		vectarget = pos - quad[cnt];
		if ((vecline.y*vectarget.x - vecline.x*vectarget.y) > 0)
		{
			return false;
		}
	}

	return true;
}

inline bool Collision::CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num)
{
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (CircleDot(circle,radius,quad[cnt]))
		{
			return true;
		}
	}
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (LineCircle(quad[(cnt + 1) % num],quad[cnt],circle,radius))
		{
			return true;
		}
	}

	if (DotQuad(circle,quad,num))
	{
		return true;
	}

	return false;
}

inline bool Collision::CircleQuad(const D3DXVECTOR3& circle,float radius,const D3DXVECTOR3* quad,UINT num,const D3DXVECTOR3& speed,D3DXVECTOR3* reflectVec)
{
	D3DXVECTOR3 reflect(0,0,0);
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (CircleDot(circle,radius,quad[cnt]))
		{
			if (reflectVec != nullptr){ *reflectVec = -speed; }
			return true;
		}
	}
	for (UINT cnt = 0;cnt < num;cnt++)
	{
		if (LineCircle(quad[(cnt + 1) % num],quad[cnt],circle,radius))
		{
			reflect = CreateReflectVec(quad[(cnt + 1) % num],quad[cnt],circle,circle+(speed*2.0f));
			*reflectVec = reflect;
			return true;
		}
	}

	if (DotQuad(circle,quad,num))
	{
		if (reflectVec != nullptr){ *reflectVec = reflect; }
		return true;
	}

	return false;
}

inline D3DXVECTOR3 Collision::CreateReflectVec(const D3DXVECTOR3& point1Start,const D3DXVECTOR3& point1End,const D3DXVECTOR3& point2Start,const D3DXVECTOR3& point2End)
{
	D3DXVECTOR3 vec0,vec1,vec2;

	D3DXVECTOR3 VecNormal, Vec1,Vec2;
	float Length1, Length2;

	D3DXVec3Normalize(&VecNormal, &(point1End - point1Start));
	Vec1 = point2Start - point1Start;
	Length1 = D3DXVec3Dot(&VecNormal, &Vec1);

	D3DXVECTOR3 Point11 = point1Start + (VecNormal*Length1);
	Point11 -= point2Start;

	D3DXVECTOR3 PointAStart = point2Start + (Point11*2.0f);

	Vec2 = point2End - point1Start;
	Length2 = D3DXVec3Dot(&VecNormal, &Vec2);

	D3DXVECTOR3 Point21 = point1Start + (VecNormal*Length2);
	Point21 -= point2End;

	D3DXVECTOR3 PointAEnd = point2End + (Point21*2.0f);

	D3DXVECTOR3 reflect = PointAEnd - PointAStart;
	D3DXVec3Normalize(&reflect, &reflect);

	return reflect;
}



#endif