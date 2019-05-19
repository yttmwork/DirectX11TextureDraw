#ifndef GRAPHICS_UTILITY_H_
#define GRAPHICS_UTILITY_H_

//=====================================================================//
//! ポリゴン出力用カスタムバーテックス構造体
//=====================================================================//
struct CustomVertex {
    float pos[ 3 ];		// 座標(x, y, z)
    float col[ 4 ];		// 色(r, g, b, a)
	float tex_pos[2];	// テクスチャ座標
};

#endif
