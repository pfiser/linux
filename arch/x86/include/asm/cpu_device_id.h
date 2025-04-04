/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CPU_DEVICE_ID
#define _ASM_X86_CPU_DEVICE_ID

/*
 * Can't use <linux/bitfield.h> because it generates expressions that
 * cannot be used in structure initializers. Bitfield construction
 * here must match the union in struct cpuinfo_86:
 *	union {
 *		struct {
 *			__u8	x86_model;
 *			__u8	x86;
 *			__u8	x86_vendor;
 *			__u8	x86_reserved;
 *		};
 *		__u32		x86_vfm;
 *	};
 */
#define VFM_MODEL_BIT	0
#define VFM_FAMILY_BIT	8
#define VFM_VENDOR_BIT	16
#define VFM_RSVD_BIT	24

#define	VFM_MODEL_MASK	GENMASK(VFM_FAMILY_BIT - 1, VFM_MODEL_BIT)
#define	VFM_FAMILY_MASK	GENMASK(VFM_VENDOR_BIT - 1, VFM_FAMILY_BIT)
#define	VFM_VENDOR_MASK	GENMASK(VFM_RSVD_BIT - 1, VFM_VENDOR_BIT)

#define VFM_MODEL(vfm)	(((vfm) & VFM_MODEL_MASK) >> VFM_MODEL_BIT)
#define VFM_FAMILY(vfm)	(((vfm) & VFM_FAMILY_MASK) >> VFM_FAMILY_BIT)
#define VFM_VENDOR(vfm)	(((vfm) & VFM_VENDOR_MASK) >> VFM_VENDOR_BIT)

#define	VFM_MAKE(_vendor, _family, _model) (	\
	((_model) << VFM_MODEL_BIT) |		\
	((_family) << VFM_FAMILY_BIT) |		\
	((_vendor) << VFM_VENDOR_BIT)		\
)

/*
 * Declare drivers belonging to specific x86 CPUs
 * Similar in spirit to pci_device_id and related PCI functions
 *
 * The wildcard initializers are in mod_devicetable.h because
 * file2alias needs them. Sigh.
 */
#include <linux/mod_devicetable.h>
/* Get the INTEL_FAM* model defines */
#include <asm/intel-family.h>
/* And the X86_VENDOR_* ones */
#include <asm/processor.h>

/* Centaur FAM6 models */
#define X86_CENTAUR_FAM6_C7_A		0xa
#define X86_CENTAUR_FAM6_C7_D		0xd
#define X86_CENTAUR_FAM6_NANO		0xf

/* x86_cpu_id::flags */
#define X86_CPU_ID_FLAG_ENTRY_VALID	BIT(0)

/**
 * X86_MATCH_CPU -  Base macro for CPU matching
 * @_vendor:	The vendor name, e.g. INTEL, AMD, HYGON, ..., ANY
 *		The name is expanded to X86_VENDOR_@_vendor
 * @_family:	The family number or X86_FAMILY_ANY
 * @_model:	The model number, model constant or X86_MODEL_ANY
 * @_steppings:	Bitmask for steppings, stepping constant or X86_STEPPING_ANY
 * @_feature:	A X86_FEATURE bit or X86_FEATURE_ANY
 * @_data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 *
 * Use only if you need all selectors. Otherwise use one of the shorter
 * macros of the X86_MATCH_* family. If there is no matching shorthand
 * macro, consider to add one. If you really need to wrap one of the macros
 * into another macro at the usage site for good reasons, then please
 * start this local macro with X86_MATCH to allow easy grepping.
 */
#define X86_MATCH_CPU(_vendor, _family, _model, _steppings, _feature, _type, _data) { \
	.vendor		= _vendor,					\
	.family		= _family,					\
	.model		= _model,					\
	.steppings	= _steppings,					\
	.feature	= _feature,					\
	.flags		= X86_CPU_ID_FLAG_ENTRY_VALID,			\
	.type		= _type,					\
	.driver_data	= (unsigned long) _data				\
}

/**
 * X86_MATCH_VENDOR_FAM_FEATURE - Macro for matching vendor, family and CPU feature
 * @vendor:	The vendor name, e.g. INTEL, AMD, HYGON, ..., ANY
 *		The name is expanded to X86_VENDOR_@vendor
 * @family:	The family number or X86_FAMILY_ANY
 * @feature:	A X86_FEATURE bit
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 */
#define X86_MATCH_VENDOR_FAM_FEATURE(vendor, family, feature, data)		\
	X86_MATCH_CPU(X86_VENDOR_##vendor, family, X86_MODEL_ANY,		\
		      X86_STEPPING_ANY, feature, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VENDOR_FEATURE - Macro for matching vendor and CPU feature
 * @vendor:	The vendor name, e.g. INTEL, AMD, HYGON, ..., ANY
 *		The name is expanded to X86_VENDOR_@vendor
 * @feature:	A X86_FEATURE bit
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 */
#define X86_MATCH_VENDOR_FEATURE(vendor, feature, data)				\
	X86_MATCH_CPU(X86_VENDOR_##vendor, X86_FAMILY_ANY, X86_MODEL_ANY,	\
		      X86_STEPPING_ANY, feature, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_FEATURE - Macro for matching a CPU feature
 * @feature:	A X86_FEATURE bit
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 */
#define X86_MATCH_FEATURE(feature, data)					\
	X86_MATCH_CPU(X86_VENDOR_ANY, X86_FAMILY_ANY, X86_MODEL_ANY,		\
		      X86_STEPPING_ANY, feature, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VENDOR_FAM_MODEL - Match vendor, family and model
 * @vendor:	The vendor name, e.g. INTEL, AMD, HYGON, ..., ANY
 *		The name is expanded to X86_VENDOR_@vendor
 * @family:	The family number or X86_FAMILY_ANY
 * @model:	The model number, model constant or X86_MODEL_ANY
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 */
#define X86_MATCH_VENDOR_FAM_MODEL(vendor, family, model, data)			\
	X86_MATCH_CPU(X86_VENDOR_##vendor, family, model, X86_STEPPING_ANY,	\
		      X86_FEATURE_ANY, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VENDOR_FAM - Match vendor and family
 * @vendor:	The vendor name, e.g. INTEL, AMD, HYGON, ..., ANY
 *		The name is expanded to X86_VENDOR_@vendor
 * @family:	The family number or X86_FAMILY_ANY
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is casted to unsigned long internally.
 */
#define X86_MATCH_VENDOR_FAM(vendor, family, data)				\
	X86_MATCH_CPU(X86_VENDOR_##vendor, family, X86_MODEL_ANY,		\
		      X86_STEPPING_ANY, X86_FEATURE_ANY, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VFM - Match encoded vendor/family/model
 * @vfm:	Encoded 8-bits each for vendor, family, model
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is cast to unsigned long internally.
 */
#define X86_MATCH_VFM(vfm, data)						\
	X86_MATCH_CPU(VFM_VENDOR(vfm), VFM_FAMILY(vfm),	VFM_MODEL(vfm),		\
		      X86_STEPPING_ANY, X86_FEATURE_ANY, X86_CPU_TYPE_ANY, data)

#define __X86_STEPPINGS(mins, maxs)    GENMASK(maxs, mins)
/**
 * X86_MATCH_VFM_STEPS - Match encoded vendor/family/model and steppings
 *			 range.
 * @vfm:	Encoded 8-bits each for vendor, family, model
 * @min_step:	Lowest stepping number to match
 * @max_step:	Highest stepping number to match
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is cast to unsigned long internally.
 */
#define X86_MATCH_VFM_STEPS(vfm, min_step, max_step, data)			\
	X86_MATCH_CPU(VFM_VENDOR(vfm), VFM_FAMILY(vfm), VFM_MODEL(vfm),		\
		      __X86_STEPPINGS(min_step, max_step), X86_FEATURE_ANY,	\
		      X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VFM_FEATURE - Match encoded vendor/family/model/feature
 * @vfm:	Encoded 8-bits each for vendor, family, model
 * @feature:	A X86_FEATURE bit
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is cast to unsigned long internally.
 */
#define X86_MATCH_VFM_FEATURE(vfm, feature, data)				\
	X86_MATCH_CPU(VFM_VENDOR(vfm), VFM_FAMILY(vfm), VFM_MODEL(vfm),		\
		      X86_STEPPING_ANY, feature, X86_CPU_TYPE_ANY, data)

/**
 * X86_MATCH_VFM_CPU_TYPE - Match encoded vendor/family/model/type
 * @vfm:	Encoded 8-bits each for vendor, family, model
 * @type:	CPU type e.g. P-core, E-core
 * @data:	Driver specific data or NULL. The internal storage
 *		format is unsigned long. The supplied value, pointer
 *		etc. is cast to unsigned long internally.
 */
#define X86_MATCH_VFM_CPU_TYPE(vfm, type, data)				\
	X86_MATCH_CPU(VFM_VENDOR(vfm), VFM_FAMILY(vfm), VFM_MODEL(vfm),	\
		      X86_STEPPING_ANY, X86_FEATURE_ANY, type, data)

extern const struct x86_cpu_id *x86_match_cpu(const struct x86_cpu_id *match);
extern bool x86_match_min_microcode_rev(const struct x86_cpu_id *table);

#endif /* _ASM_X86_CPU_DEVICE_ID */
