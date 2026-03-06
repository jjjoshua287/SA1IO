/*
    sa1-ninja.h
    
    This file contains definitions for SA1-specific Ninja structures and functions.
    COPYRIGHT (C) SEGA ENTERPRISES,LTD.    
*/
#ifndef SEGA_SA1_NINJA_H
#define SEGA_SA1_NINJA_H

#include <stdint.h>

#define BIT(n) (1u << n)

typedef int32_t Angle;

/*--------------------------------------*/
/*      Model Define                    */
/*--------------------------------------*/
#define NJD_EVAL_UNIT_POS   BIT(0)     /* ignore translation */
#define NJD_EVAL_UNIT_ANG   BIT(1)     /* ignore rotation */
#define NJD_EVAL_UNIT_SCL   BIT(2)     /* ignore scaling */
#define NJD_EVAL_HIDE       BIT(3)     /* do not draw model */
#define NJD_EVAL_BREAK      BIT(4)     /* terminate tracing children */
#define NJD_EVAL_ZXY_ANG    BIT(5) 
#define NJD_EVAL_SKIP		BIT(6)
#define NJD_EVAL_SHAPE_SKIP	BIT(7)
#define NJD_EVAL_CLIP		BIT(8)
#define NJD_EVAL_MODIFIER	BIT(9)
#define NJD_EVAL_MASK       0x3ff

/* flags */
#define NJD_FLAG_IGNORE_LIGHT		(BIT(25))
#define NJD_FLAG_USE_FLAT			(BIT(24))
#define NJD_FLAG_DOUBLE_SIDE		(BIT(23))
#define NJD_FLAG_USE_ENV			(BIT(22))
#define NJD_FLAG_USE_TEXTURE		(BIT(21))
#define NJD_FLAG_USE_ALPHA			(BIT(20))
#define NJD_FLAG_IGNORE_SPECULAR	(BIT(19))
#define NJD_FLAG_FLIP_U				(BIT(18))
#define NJD_FLAG_FLIP_V				(BIT(17))
#define NJD_FLAG_CLAMP_U			(BIT(16))
#define NJD_FLAG_CLAMP_V			(BIT(15))
#define NJD_FLAG_USE_ANISOTROPIC	(BIT(12))
#define NJD_FLAG_PICK				(BIT(7))

/* Mask for flip and clamp */
#define NJD_FLAG_FLIP_MASK  (NJD_FLAG_FLIP_U| NJD_FLAG_FLIP_V)
#define NJD_FLAG_CLAMP_MASK  (NJD_FLAG_CLAMP_U|NJD_FLAG_CLAMP_V)

/* Mask for the flags evaluated by H/W */
#define NJD_FLAG_HARD_MASK (NJD_FLAG_USE_ALPHA	        \
	| NJD_FLAG_FLIP_MASK|NJD_FLAG_CLAMP_MASK        \
	| NJD_FLAG_USE_ANISOTROPIC)

/* Mask for the flags evaluated by the library */
#define NJD_FLAG_SOFT_MASK ( NJD_FLAG_IGNORE_LIGHT	\
	|NJD_FLAG_PICK| NJD_FLAG_USE_TEXTURE		\
	| NJD_FLAG_DOUBLE_SIDE| NJD_FLAG_USE_ENV	\
	| NJD_FLAG_USE_FLAT)

/* Mask for entire flag */
#define NJD_FLAG_MASK (NJD_FLAG_HARD_MASK		\
	| NJD_FLAG_SOFT_MASK)

/* Default user mask */
#define NJD_DEFAULT_USER_MASK	(BIT(2)|BIT(1)|BIT(0))

/* Default systm mask */
#define NJD_DEFAULT_SYS_MASK	(~NJD_DEFAULT_USER_MASK)
 

/* meshset type bits (type_addrId 14-15 bit) */
#define NJD_MESHSET_3           0x0000  /* polygon3 meshset         */
#define NJD_MESHSET_4           0x4000  /* polygon4 meshset         */
#define NJD_MESHSET_N           0x8000  /* polygonN meshset         */
#define NJD_MESHSET_TRIMESH     0xc000  /* trimesh meshset          */
#define NJD_MESHSET_MASK        0xc000  /* meshset type mask        */

#define NJD_MESHSET_TYPE(x) ((x) & NJD_MESHSET_MASK)
#define NJD_MESHSET_MATID(x) ((x) & ~NJD_MESHSET_MASK)

/* meshset trimesh end bit */
#define NJD_TRIMESH_END         0x8000  /* trimesh end bit          */


/* Callback bits (attr_texId 30-31 bit */
#define	NJD_POLYGON_CALLBACK	(BIT(31))			/* polygon callback		*/
#define	NJD_MATERIAL_CALLBACK	(BIT(30))			/* material callback	*/
#define	NJD_CALLBACK_MASK		(BIT(31) | BIT(30))	/* callback mask		*/

/*--------------------------------------*/
/*      Motion define                   */
/*--------------------------------------*/
/* motion type bits */

#define NJD_MTYPE_POS_0			BIT(0)
#define NJD_MTYPE_ANG_1			BIT(1)
#define NJD_MTYPE_SCL_2			BIT(2)
#define NJD_MTYPE_VEC_3			BIT(3)
#define NJD_MTYPE_VERT_4		BIT(4)
#define NJD_MTYPE_NORM_5		BIT(5)
#define	NJD_MTYPE_TARGET_3		BIT(6)
#define	NJD_MTYPE_ROLL_6		BIT(7)
#define	NJD_MTYPE_ANGLE_7		BIT(8)
#define	NJD_MTYPE_RGB_8			BIT(9)
#define	NJD_MTYPE_INTENSITY_9	BIT(10)
#define	NJD_MTYPE_SPOT_10		BIT(11)
#define	NJD_MTYPE_POINT_10		BIT(12)
#define NJD_MTYPE_QUAT_1        BIT(13)

#define NJD_MTYPE_LINER         0x0000    /* use liner                */
#define NJD_MTYPE_SPLINE        0x0040    /* use spline               */
#define NJD_MTYPE_USER          0x0080    /* use user function        */
#define NJD_MTYPE_MASK          0x00c0    /* motion function mask     */

/*--------------------------------------*/
/*      MACRO                           */
/*--------------------------------------*/
/*
 *  COLOR MACRO
 */
#define NJM_COLOR_A(col)        ((col) >> 24)
#define NJM_COLOR_R(col)        (((col) >> 16) & 0xFF)
#define NJM_COLOR_G(col)        (((col) >> 8) & 0xFF)
#define NJM_COLOR_B(col)        ((col) & 0xFF)

/*
 *  ANGLE MACRO
 */
#define NJD_PI  3.141592
#define NJM_DEG_RAD(n)  ((n) * NJD_PI / 180.0) /*  deg ¨ rad  */
#define NJM_DEG_ANG(n)  ((Angle)(((n) * 65536.0) / 360.0))  /*  deg ¨ ang  */
#define NJM_RAD_ANG(n)  ((Angle)(((n) * 65536.0) / (2 * NJD_PI))) /*  rad ¨ ang  */
#define NJM_RAD_DEG(n)  ((n) * 180.0 / NJD_PI) /*  rad ¨ deg  */
#define NJM_ANG_DEG(n)  ((n) * 360.0 / 65536.0) /*  ang ¨ deg  */
#define NJM_ANG_RAD(n)  ((n) * (2 * NJD_PI) / 65536.0) /*  ang ¨ rad  */

/*--------------------------------------*/
/*      Texture Structure               */
/*--------------------------------------*/

typedef struct{
	uint32_t Type;
	uint32_t BitDepth;
	uint32_t PixelFormat;
	uint32_t nWidth;
	uint32_t nHeight;
	uint32_t TextureSize;
	uint32_t fSurfaceFlags;
	uint32_t *pSurface;
	uint32_t *pVirtual;
	uint32_t *pPhysical;
} NJS_TEXSURFACE;
	
typedef struct{
	void*			   texaddr; /* texture address   			*/
	NJS_TEXSURFACE	texsurface;	/* texture surface 				*/
} NJS_TEXINFO;

struct NJS_TEXNAME {
	void            *filename;  /* texture filename strings     */
	uint32_t        attr;       /* texture attribute            */
	uint32_t        texaddr;    /* texture memory list address  */
};

struct NJS_TEXLIST {
	struct NJS_TEXNAME     *textures;  /* texture array                */
	uint32_t               nbTexture;  /* texture count                */
};

typedef struct {
	uint32_t			globalIndex;/* global unique texture ID     */
	uint32_t			bank;		/* palette bank 				*/
	uint32_t			tspparambuffer;/* TSPParambuffer			*/
	uint32_t			texparambuffer;/* TextureParambuffer		*/
	uint32_t			texaddr;	/* texture flag					*/
	NJS_TEXINFO		    texinfo;	/* texinfo						*/
	uint16_t			count;		/* texture count  				*/
	uint16_t			dummy;
} NJS_TEXMEMLIST;

typedef struct {
	void 			    *palette;	/*	palette pointer				*/
	uint16_t 			mode;		/*	color mode					*/
	int16_t 			bank;		/*	bank						*/
	int16_t 			offset;		/*	entry offset				*/
	int16_t 			count;		/*	entry count					*/
} NJS_TEXPALETTE;

typedef struct {
	uint32_t	n;
	uint32_t	globalIndex;
	int32_t	texerr;
	int32_t	gdstat;
	int32_t	gderr;
	int32_t	reserved0;
	int32_t	reserved1;
	int32_t	reserved2;
} NJS_TEXERRSTAT;

typedef struct{
	uint32_t nTextureType;
	uint16_t nWidth;
	uint16_t nHeight;
} NJS_PVRHEADERDATA;

/*--------------------------------------*/
/*      Model Structure                 */
/*--------------------------------------*/
/*
 NJS_MODEL_SADX
*/

struct NJS_TEX {
	uint16_t  u;
	uint16_t  v;
};

struct NJS_BGRA {
	uint8_t   b;
	uint8_t   g;
	uint8_t   r;
	uint8_t   a;
};

typedef union {
	uint32_t  color;
	struct NJS_BGRA argb;
} NJS_COLOR;

struct NJS_POINT3 {
	float   x;
	float   y;
	float   z;
};

struct NJS_VECTOR {
	float   x;
	float   y;
	float   z;
};

struct NJS_MATERIAL {
	NJS_COLOR         diffuse;
	NJS_COLOR         specular;
	float             exponent;
	uint32_t          attr_texId;  /* attribute and texture ID in texlist        */
	uint32_t          attrflags;   /* attribute flags */
};

typedef struct {
	uint16_t          type_matId; /* meshset type and attr index
	                               14-15 : meshset type bits
	                                0-13 : material id(0-4095)  */
	uint16_t          nbMesh;     /* mesh count                   */
	int16_t          *meshes;    /* mesh array                   */
	uint32_t          *attrs;     /* attribure                    */
	struct NJS_VECTOR      *normals;   /* mesh normal list             */
	NJS_COLOR       *vertcolor; /* polygon vertex color list    */
	struct NJS_TEX         *vertuv;    /* polygon vertex uv list       */
	void            *buffer;    /* A pointer to MeshSetBuffer. See definition in direct3d.h. */
                                /* skip when importing since Linux */
} NJS_MESHSET_SADX;

struct NJS_MODEL_SADX {
	struct NJS_POINT3      *points;    /* vertex list                  */
	struct NJS_VECTOR      *normals;   /* vertex normal list           */
	int32_t                nbPoint;    /* vertex count                 */
	NJS_MESHSET_SADX       *meshsets;  /* meshset list                 */
	struct NJS_MATERIAL    *mats;      /* material list                */
	uint16_t               nbMeshset;  /* meshset count                */
	uint16_t               nbMat;      /* material count               */
	struct NJS_POINT3      center;     /* model center                 */
	float			       r;		   /* ??????????? */
	void                   *buffer;    /* skip when importing since Linux */
};

/*
 * NJS_OBJECT
 */
typedef struct obj {
	uint32_t          evalflags;  /* evalation flags              */
	void            *model;       /* model data pointer           */
	float           pos[3];       /* translation                  */
	Angle           ang[3];       /* rotation                     */
	float           scl[3];       /* scaling                      */
	struct obj      *child;       /* child object                 */
	struct obj      *sibling;     /* sibling object               */
} NJS_OBJECT, NJS_CNK_OBJECT;

static inline struct NJS_MODEL_SADX *get_basic_dx_model(const NJS_OBJECT *obj)
{
		return (struct NJS_MODEL_SADX *)obj->model;
}

static inline void put_basic_dx_model(NJS_OBJECT *obj, struct NJS_MODEL_SADX *model)
{
		obj->model = model;
}

/*
 * NJS_MOTION
 */
/* key frame animation */
typedef struct {
	uint32_t          keyframe;
	float           key[3];
} NJS_MKEY_F;

typedef struct {
	uint32_t          keyframe;
	Angle           key[3];     /* angle                        */
} NJS_MKEY_A;

typedef struct {
	uint32_t          keyframe;
	Angle           angle;      /* angle                        */
	float           axe[3];     /* axe vector                   */
} NJS_MKEY_AX;

typedef struct 
{
	uint32_t          keyframe;
	void            *key;        /* pointer                     */
} NJS_MKEY_P;

typedef struct {
	uint32_t          keyframe;
	float           key;
} NJS_MKEY_F1;

typedef struct {
	uint32_t          keyframe;
	float           key[2];
} NJS_MKEY_F2;

typedef struct {
	uint32_t          keyframe;
	float           nrang;
	float           frang;
	Angle           iang;
	Angle           oang;
} NJS_MKEY_SPOT;

typedef struct {
	uint32_t          keyframe;
	uint32_t          key;
} NJS_MKEY_UI32;

typedef struct {
	uint32_t          keyframe;
	int32_t          key;
} NJS_MKEY_SI32, NJS_MKEY_A1;

/*
the layout is [w, x, y, z]
*/
typedef struct {
	uint32_t          keyframe;
	float           key[4];
} NJS_MKEY_QUAT;

/*
 * NJS_MDATA
 */
/* 1 factor : pos or ang or scl or shape or vec */
typedef struct {
	void            *p[1];      /* factor pointer               */
	uint32_t          nb[1];      /* factor count                 */
} NJS_MDATA1;

/* 2 factors : pos and ang, pos and vec(light motion), etc      */
typedef struct {
	void            *p[2];      /* factor pointer               */
	uint32_t          nb[2];      /* factor count                 */
} NJS_MDATA2;

/* 3 factors : pos and ang and scl, pos and ang and shape, etc  */
typedef struct {
	void            *p[3];      /* factor pointer               */
	uint32_t          nb[3];      /* factor count                 */
} NJS_MDATA3;

/* 4 factors : pos and ang and scl and shape, etc               */
typedef struct {
	void            *p[4];      /* factor pointer               */
	uint32_t          nb[4];      /* factor count                 */
} NJS_MDATA4;

/* 5 factors : etc               */
typedef struct {
	void            *p[5];      /* factor pointer               */
	uint32_t          nb[5];      /* factor count                 */
} NJS_MDATA5;

struct NJS_MOTION {
	void            *mdata;     /* NJS_MDATA array              */
	uint32_t        nbFrame;    /* frame count                  */
	uint16_t        type;       /* motion type  NJD_MTYPE_...   */
	uint16_t        inp_fn;     /* interpolation & factor count */
};

struct NJS_ACTION {
	NJS_OBJECT             *object;    /* Object Tree top pointer      */
	struct NJS_MOTION      *motion;    /* motion                       */
};

struct NJS_MOTION_LINK
{
	struct NJS_MOTION		*motion[2];		/* motion's pointers */
	float			        frame[2];		/* current frames */
};

struct NJS_ACTION_LINK
{
	NJS_OBJECT		        *object;		/* object's pointer */
	struct NJS_MOTION_LINK	*motionlink;	/* motion-link's pointer */
};


#endif /* SEGA_SA1_NINJA_H */