#include "anorms.h"

LVector3 g_anorms[NUMVERTEXNORMALS] =
{
  LVector3(-0.525731, 0.000000, 0.850651),
  LVector3(-0.442863, 0.238856, 0.864188),
  LVector3(-0.295242, 0.000000, 0.955423),
  LVector3(-0.309017, 0.500000, 0.809017),
  LVector3(-0.162460, 0.262866, 0.951056),
  LVector3(0.000000, 0.000000, 1.000000),
  LVector3(0.000000, 0.850651, 0.525731),
  LVector3(-0.147621, 0.716567, 0.681718),
  LVector3(0.147621, 0.716567, 0.681718),
  LVector3(0.000000, 0.525731, 0.850651),
  LVector3(0.309017, 0.500000, 0.809017),
  LVector3(0.525731, 0.000000, 0.850651),
  LVector3(0.295242, 0.000000, 0.955423),
  LVector3(0.442863, 0.238856, 0.864188),
  LVector3(0.162460, 0.262866, 0.951056),
  LVector3(-0.681718, 0.147621, 0.716567),
  LVector3(-0.809017, 0.309017, 0.500000),
  LVector3(-0.587785, 0.425325, 0.688191),
  LVector3(-0.850651, 0.525731, 0.000000),
  LVector3(-0.864188, 0.442863, 0.238856),
  LVector3(-0.716567, 0.681718, 0.147621),
  LVector3(-0.688191, 0.587785, 0.425325),
  LVector3(-0.500000, 0.809017, 0.309017),
  LVector3(-0.238856, 0.864188, 0.442863),
  LVector3(-0.425325, 0.688191, 0.587785),
  LVector3(-0.716567, 0.681718, -0.147621),
  LVector3(-0.500000, 0.809017, -0.309017),
  LVector3(-0.525731, 0.850651, 0.000000),
  LVector3(0.000000, 0.850651, -0.525731),
  LVector3(-0.238856, 0.864188, -0.442863),
  LVector3(0.000000, 0.955423, -0.295242),
  LVector3(-0.262866, 0.951056, -0.162460),
  LVector3(0.000000, 1.000000, 0.000000),
  LVector3(0.000000, 0.955423, 0.295242),
  LVector3(-0.262866, 0.951056, 0.162460),
  LVector3(0.238856, 0.864188, 0.442863),
  LVector3(0.262866, 0.951056, 0.162460),
  LVector3(0.500000, 0.809017, 0.309017),
  LVector3(0.238856, 0.864188, -0.442863),
  LVector3(0.262866, 0.951056, -0.162460),
  LVector3(0.500000, 0.809017, -0.309017),
  LVector3(0.850651, 0.525731, 0.000000),
  LVector3(0.716567, 0.681718, 0.147621),
  LVector3(0.716567, 0.681718, -0.147621),
  LVector3(0.525731, 0.850651, 0.000000),
  LVector3(0.425325, 0.688191, 0.587785),
  LVector3(0.864188, 0.442863, 0.238856),
  LVector3(0.688191, 0.587785, 0.425325),
  LVector3(0.809017, 0.309017, 0.500000),
  LVector3(0.681718, 0.147621, 0.716567),
  LVector3(0.587785, 0.425325, 0.688191),
  LVector3(0.955423, 0.295242, 0.000000),
  LVector3(1.000000, 0.000000, 0.000000),
  LVector3(0.951056, 0.162460, 0.262866),
  LVector3(0.850651, -0.525731, 0.000000),
  LVector3(0.955423, -0.295242, 0.000000),
  LVector3(0.864188, -0.442863, 0.238856),
  LVector3(0.951056, -0.162460, 0.262866),
  LVector3(0.809017, -0.309017, 0.500000),
  LVector3(0.681718, -0.147621, 0.716567),
  LVector3(0.850651, 0.000000, 0.525731),
  LVector3(0.864188, 0.442863, -0.238856),
  LVector3(0.809017, 0.309017, -0.500000),
  LVector3(0.951056, 0.162460, -0.262866),
  LVector3(0.525731, 0.000000, -0.850651),
  LVector3(0.681718, 0.147621, -0.716567),
  LVector3(0.681718, -0.147621, -0.716567),
  LVector3(0.850651, 0.000000, -0.525731),
  LVector3(0.809017, -0.309017, -0.500000),
  LVector3(0.864188, -0.442863, -0.238856),
  LVector3(0.951056, -0.162460, -0.262866),
  LVector3(0.147621, 0.716567, -0.681718),
  LVector3(0.309017, 0.500000, -0.809017),
  LVector3(0.425325, 0.688191, -0.587785),
  LVector3(0.442863, 0.238856, -0.864188),
  LVector3(0.587785, 0.425325, -0.688191),
  LVector3(0.688191, 0.587785, -0.425325),
  LVector3(-0.147621, 0.716567, -0.681718),
  LVector3(-0.309017, 0.500000, -0.809017),
  LVector3(0.000000, 0.525731, -0.850651),
  LVector3(-0.525731, 0.000000, -0.850651),
  LVector3(-0.442863, 0.238856, -0.864188),
  LVector3(-0.295242, 0.000000, -0.955423),
  LVector3(-0.162460, 0.262866, -0.951056),
  LVector3(0.000000, 0.000000, -1.000000),
  LVector3(0.295242, 0.000000, -0.955423),
  LVector3(0.162460, 0.262866, -0.951056),
  LVector3(-0.442863, -0.238856, -0.864188),
  LVector3(-0.309017, -0.500000, -0.809017),
  LVector3(-0.162460, -0.262866, -0.951056),
  LVector3(0.000000, -0.850651, -0.525731),
  LVector3(-0.147621, -0.716567, -0.681718),
  LVector3(0.147621, -0.716567, -0.681718),
  LVector3(0.000000, -0.525731, -0.850651),
  LVector3(0.309017, -0.500000, -0.809017),
  LVector3(0.442863, -0.238856, -0.864188),
  LVector3(0.162460, -0.262866, -0.951056),
  LVector3(0.238856, -0.864188, -0.442863),
  LVector3(0.500000, -0.809017, -0.309017),
  LVector3(0.425325, -0.688191, -0.587785),
  LVector3(0.716567, -0.681718, -0.147621),
  LVector3(0.688191, -0.587785, -0.425325),
  LVector3(0.587785, -0.425325, -0.688191),
  LVector3(0.000000, -0.955423, -0.295242),
  LVector3(0.000000, -1.000000, 0.000000),
  LVector3(0.262866, -0.951056, -0.162460),
  LVector3(0.000000, -0.850651, 0.525731),
  LVector3(0.000000, -0.955423, 0.295242),
  LVector3(0.238856, -0.864188, 0.442863),
  LVector3(0.262866, -0.951056, 0.162460),
  LVector3(0.500000, -0.809017, 0.309017),
  LVector3(0.716567, -0.681718, 0.147621),
  LVector3(0.525731, -0.850651, 0.000000),
  LVector3(-0.238856, -0.864188, -0.442863),
  LVector3(-0.500000, -0.809017, -0.309017),
  LVector3(-0.262866, -0.951056, -0.162460),
  LVector3(-0.850651, -0.525731, 0.000000),
  LVector3(-0.716567, -0.681718, -0.147621),
  LVector3(-0.716567, -0.681718, 0.147621),
  LVector3(-0.525731, -0.850651, 0.000000),
  LVector3(-0.500000, -0.809017, 0.309017),
  LVector3(-0.238856, -0.864188, 0.442863),
  LVector3(-0.262866, -0.951056, 0.162460),
  LVector3(-0.864188, -0.442863, 0.238856),
  LVector3(-0.809017, -0.309017, 0.500000),
  LVector3(-0.688191, -0.587785, 0.425325),
  LVector3(-0.681718, -0.147621, 0.716567),
  LVector3(-0.442863, -0.238856, 0.864188),
  LVector3(-0.587785, -0.425325, 0.688191),
  LVector3(-0.309017, -0.500000, 0.809017),
  LVector3(-0.147621, -0.716567, 0.681718),
  LVector3(-0.425325, -0.688191, 0.587785),
  LVector3(-0.162460, -0.262866, 0.951056),
  LVector3(0.442863, -0.238856, 0.864188),
  LVector3(0.162460, -0.262866, 0.951056),
  LVector3(0.309017, -0.500000, 0.809017),
  LVector3(0.147621, -0.716567, 0.681718),
  LVector3(0.000000, -0.525731, 0.850651),
  LVector3(0.425325, -0.688191, 0.587785),
  LVector3(0.587785, -0.425325, 0.688191),
  LVector3(0.688191, -0.587785, 0.425325),
  LVector3(-0.955423, 0.295242, 0.000000),
  LVector3(-0.951056, 0.162460, 0.262866),
  LVector3(-1.000000, 0.000000, 0.000000),
  LVector3(-0.850651, 0.000000, 0.525731),
  LVector3(-0.955423, -0.295242, 0.000000),
  LVector3(-0.951056, -0.162460, 0.262866),
  LVector3(-0.864188, 0.442863, -0.238856),
  LVector3(-0.951056, 0.162460, -0.262866),
  LVector3(-0.809017, 0.309017, -0.500000),
  LVector3(-0.864188, -0.442863, -0.238856),
  LVector3(-0.951056, -0.162460, -0.262866),
  LVector3(-0.809017, -0.309017, -0.500000),
  LVector3(-0.681718, 0.147621, -0.716567),
  LVector3(-0.681718, -0.147621, -0.716567),
  LVector3(-0.850651, 0.000000, -0.525731),
  LVector3(-0.688191, 0.587785, -0.425325),
  LVector3(-0.587785, 0.425325, -0.688191),
  LVector3(-0.425325, 0.688191, -0.587785),
  LVector3(-0.425325, -0.688191, -0.587785),
  LVector3(-0.587785, -0.425325, -0.688191),
  LVector3(-0.688191, -0.587785, -0.425325)
};